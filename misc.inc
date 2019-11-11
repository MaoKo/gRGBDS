
macro _comment? begin?*
    local end_comment
    end_comment := (`begin) bswap (lengthof (`begin))
    macro ?! line?&
        if ((`line) = end_comment)
            purge ?
        end if
    end macro
end macro

macro _append_list head?*, node?&
    local exist
    exist = $00
    irpv value, head
        exist = $01
        break
    end irpv
    if (exist)
        head equ head, node
    else
        head equ node
    end if
end macro
