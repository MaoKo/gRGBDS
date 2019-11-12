
macro _comment? begin?*
    local end_comment
    end_comment := (`begin) bswap (lengthof (`begin))
    macro ?! line?&
        if ((`line) = end_comment)
            purge ?
        end if
    end macro
end macro
