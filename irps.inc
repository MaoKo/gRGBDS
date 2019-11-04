
macro _pairing? list?*&
    iterate name, list
        macro name?
            purge name?
        end macro
    end iterate
end macro

macro _core_irps? parameter?*, text?&
    local iterator, incspace, nextspace, end_space, token, buffer, empty
    iterator equ parameter
    incspace = $00
    match it=[=+=space?=], parameter
        iterator equ it
        incspace = $01
    end match
    nextspace = $00
    buffer equ text
    empty  equ
    while $01
        end_space = $00
        if (nextspace)
            nextspace = $00
            token equ
        else
            match _ remain, buffer
                if (incspace)
                    match =_= remain2, buffer
                        nextspace = $01
                    end match
                end if
                token  equ _
                match remain2= , buffer
                    end_space = $01
                end match
                buffer equ remain
                if (end_space)
                    buffer equ buffer empty
                end if
            else match _, buffer
                if (incspace)
                    match =_= , buffer
                        nextspace = $01
                    end match
                end if
                token  equ _
                buffer equ
            else
                break
            end match
        end if
    end while
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
