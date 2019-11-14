
macro _core_irps? parameter?*, text?&
    local   iterator, stream, inc_space, inc_ident, identifier, valid_id, last_dot, buffer, token1, token2
    define iterator parameter
    inc_space = $00
    inc_ident = $00
    match it=[ options =], parameter
        define iterator it
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
    buffer equ text ; expands all symbolic variable once
    while $01
        match _ remain, buffer
            define token1 _
            if (inc_space)
                match =_= insert_space, buffer
                    define token1
                end match
            end if
            define buffer remain
        else 
            match _, buffer
                define token1 _
            end match
            break
        end match
    end while
    if (inc_space)
        match _= , text
            define token1
        end match
    end if
    define stream token1
    if (inc_ident)
        define identifier
        last_dot = $00
        irpv item, token1
            ;_fasmg_identifier valid_id, `item
            valid_id = $01
            if (valid_id) 
                ;if ((`item) = ".") | (last_dot)
                ;    _insert_token identifier, item
                ;    if ((`item) = ".")
                ;        last_dot = $01
                ;    end if
                ;else match _, identifier
                ;    define token2 identifier
                ;end match
                define identifier item
            else
                match _, identifier
                    define token2 identifier
                    define identifier
                end match
                define token2 item
            end if
            ;if (last_dot) & ((`item) <> ".")
            ;    last_dot = $00
            ;end if
        end irpv
        match _, identifier
            define token2 identifier
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

macro reverse?
    indx ((%% - %) + $01)
end macro

macro forward?
    indx (%)
end macro
