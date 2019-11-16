
macro _core_irps? parameter?*, text?&
    local  iterator, stream, inc_space, inc_ident, identifier, buffer, token
    define iterator parameter
    define inc_space 0
    define inc_ident 0
    ;rawmatch it =[ options =], parameter
    ;    redefine iterator it
    ;    rawmatch =+ option_1 =| =+ option_2, options
    ;        rawmatch =space? =ident?, option_1 option_2
    ;        else rawmatch =ident? =space?, option_1 option_2
    ;        else
    ;            err "syntax error: bad irps option"
    ;        end rawmatch
    ;        redefine inc_space 1
    ;        redefine inc_ident 1
    ;    else rawmatch =+ =space?, options
    ;        redefine inc_space 1
    ;    else rawmatch =+ =ident?, options
    ;        redefine inc_ident 1
    ;    else
    ;        err "syntax error: bad irps option"
    ;    end rawmatch
    ;end rawmatch

    ;macro handle_space?
    ;    match _, itentifier
    ;        define identifier
    ;        define token _
    ;    end match
    ;    define token
    ;end macro

    ;macro handle_ident? item?*&
    ;    local dotend, valid
    ;    valid _fasmg_identifier item
    ;    if (valid)
    ;        virtual at $00
    ;            db $00
    ;            match _, identifier
    ;                db (`_)
    ;            end match
    ;            load dotend:byte from ($ - $01)
    ;        end virtual
    ;        if (dotend = $2E) | (`item = '.')
    ;            _insert_token identifier, item
    ;        else
    ;            match _, identifier
    ;                define token _
    ;            end match
    ;            if (valid = $02)
    ;                redefine identifier item
    ;            else
    ;                define token item
    ;            end if
    ;        end if
    ;    else
    ;        match _, identifier
    ;            redefine identifier
    ;            define   token _
    ;        end match
    ;        define token item
    ;    end if
    ;end macro

    buffer equ text ; expands all symbolic variable once
    define identifier
    while $01
        match _ remain, buffer
            ;handle_ident _
            ;match =1 =_= insert_space, inc_space buffer
                ;handle_space
            ;end match
            redefine buffer remain
        else 
            match _, buffer
                ;handle_ident _
                ;match =1 =_= , inc_space buffer
                    ;handle_space
                ;end match
            end match
            break
        end match
    end while
    ;match _, identifier
    ;    define token _
    ;end match
    ;purge handle_space?, handle_ident?
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
