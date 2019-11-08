
macro _pairing? list?*&
    iterate name, list
        macro name?
            purge name?
        end macro
    end iterate
end macro

macro _fasmg_identifier? result?*, token?&
    local string_token
    string_token = (`token)
    result = $01
    if ((string_token and $FF) >= "0") & ((string_token and $FF) <= "9")
        result = $00
    else if (string_token = "")
        result = $00
    else if  ((string_token and $FF) = "$")
        string_token = string (string_token shr $08)
        iterate <lbound,ubound>, "0","9", "A","Z", "a","z"
            if ((string_token and $FF) >= lbound) & ((string_token and $FF) <= ubound)
                result = $00
                break
            end if
        end iterate
    end if
    if (result)
        _iterate_string char, string_token
            if (char <> "$") & (char <> "%") & (char <> ".") & ((char < "0") | (char > "9"))\
             & (char <> "@") & ((char < "A") | (char > "Z")) & (char <> "^") & (char <> "_")\
             & ((char < "a") | (char > "z"))
                result = $00
                break
            end if
        end _iterate_string
    end if
end macro

macro _core_irps? parameter?*, text?&
    local   iterator, stream, inc_space, inc_ident, next_space, end_space,\
            identifier, valid_id, last_dot, buffer, token1, token2, empty
    iterator equ parameter
    inc_space = $00
    inc_ident = $00
    match it=[ options =], parameter
        iterator equ it
        match =+ option_1 =| =+ option_2, options
            match =space? =ident?, option_1 option_2
            else match =ident? =space?, option_1 option_2
            else
                err "syntax error: bad irps option"
            end match
            inc_space = $01
            inc_ident = $01
        else match =+ =space?, options
            inc_space = $01
        else match =+ =ident?, options
            inc_ident = $01
        else
            err "syntax error: bad irps option"
        end match
    end match
    next_space = $00
    buffer equ text
    empty  equ
    while $01
        if (next_space)
            next_space = $00
            token1 equ
        else
            match _ remain, buffer
                if (inc_space)
                    match =_= remain2, buffer
                        next_space = $01
                    end match
                end if
                end_space = $00
                match remain2= , buffer
                    end_space = $01
                end match
                token1 equ _
                buffer equ remain
                if (end_space)
                    buffer equ buffer empty
                end if
            else match _, buffer
                if (inc_space)
                    match =_= , buffer
                        next_space = $01
                    end match
                end if
                token1 equ _
                buffer equ
            else
                break
            end match
        end if
    end while
    define stream token1
    if (inc_ident)
        identifier equ
        last_dot = $00
        irpv item, token1
            _fasmg_identifier valid_id, item
            if (valid_id) 
                if ((`item) = ".") | (last_dot)
                    _insert_token identifier, item
                    if ((`item) = ".")
                        last_dot = $01
                    end if
                else match,identifier
                    identifier equ item
                else
                    token2 equ identifier
                    identifier equ item
                end match
            else
                match _, identifier
                    token2 equ identifier
                    token2 equ item
                    identifier equ
                else
                    token2 equ item
                end match
            end if
            if ((`item) <> ".")
                last_dot = $00
            end if
        end irpv
        match _, identifier
            token2 equ identifier
        end match
        define stream token2
    end if
    match it st, iterator stream
        outscope irpv it, st
end macro

macro irps?! line?*&
    _pairing match, irpv
    match
    irpv
    outscope _core_irps line
end macro

macro end?.irps?!
        end irpv
    end match
end macro

macro _core_iirpv? parameter?*, index?*, targets?*&
    local state
    state  equ index
    iterate target, targets
        match _, target
            outscope irpv parameter, target
                iterate <type,position>, FIRST,%,\
                                         LAST,%%
                    match =type?, index
                        state equ position
                        break
                    end match
                end iterate
                indx state
end macro

macro iirpv?! line?*&
    _pairing iterate, match, irpv
    iterate
    match
    irpv
    outscope _core_iirpv line
end macro

macro end?.iirpv?!
                break
            end irpv
        end match
    end iterate
end macro
