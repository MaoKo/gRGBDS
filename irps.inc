
macro _core_irps? parameter?*, text?&
    local  iterator, stream, inc_space, inc_ident, identifier, valid_id, last_dot, str_item, buffer, token
    define iterator parameter
    define inc_space 0
    define inc_ident 0
    rawmatch it =[ options =], parameter
        redefine iterator it
        rawmatch =+ option_1 =| =+ option_2, options
            rawmatch =space? =ident?, option_1 option_2
            else rawmatch =ident? =space?, option_1 option_2
            else
                err "syntax error: bad irps option"
            end rawmatch
            redefine inc_space 1
            redefine inc_ident 1
        else rawmatch =+ =space?, options
            redefine inc_space 1
        else rawmatch =+ =ident?, options
            redefine inc_ident 1
        else
            err "syntax error: bad irps option"
        end rawmatch
    end rawmatch
    buffer equ text ; expands all symbolic variable once
    while $01
        match _ remain, buffer
            define token _
            match =1 =_= insert_space, inc_space buffer
                define token
            end match
            redefine buffer remain
        else 
            match _, buffer
                define token _
                match =1 =_= , inc_space buffer
                    define token
                end match
            end match
            break
        end match
    end while
    match =1, inc_ident
        define identifier
        last_dot = $00
        irpv item, token
            rawmatch =1, %
                repeat %%
                    restore token ; purge all values of token
                end repeat
            end rawmatch
            str_item = (`item)
            _fasmg_identifier valid_id, str_item
            if (valid_id) 
                if ((str_item) = ".") | (last_dot)
                    _insert_token identifier, item
                    if ((str_item) = ".")
                        last_dot = $01
                    end if
                else 
                    match _, identifier
                        define token _
                    end match
                    redefine identifier item
                end if
            else
                match _, identifier
                    define token _
                    redefine identifier
                end match
                define token item
            end if
            if (last_dot) & ((str_item) <> ".")
                last_dot = $00
            end if
        end irpv
        match _, identifier
            define token _
        end match
    end match
    match it, iterator
        outscope irpv it, token
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
