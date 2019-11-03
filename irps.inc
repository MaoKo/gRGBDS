
macro irps?! parameter?*, text?&
    local iterator, incspace, nextspace, token, buffer
    iterator equ parameter
    incspace = $00
    match it=[=+=space?=], parameter
        iterator equ it
        incspace = $01
    end match
    nextspace = $00
    buffer equ text
    while $01
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
                buffer equ remain
            else match _, buffer
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

macro end?.irps?!
        end irpv
    end match
end macro

macro iirpv?! parameter?*, index?*, targets?*&
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

macro end?.iirpv?!
                break
            end irpv
        end match
    end iterate
end macro
