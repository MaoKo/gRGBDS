
macro _bound_limit? value?*, bitness?*, type?
    local slimit, ulimit
    _assert_numeric (value), (bitness)
    if (elementsof (value))
        error "_bound_limit", "invalid operand: linear polynomial cannot be part of immediate."
    end if
    ulimit := ($01 shl bitness)
    slimit := ($01 shl (bitness - $01))
    match _, type
        rawmatch =SIGNED?, _
            if (((value) >= slimit) | ((value) < (-slimit)))
                error "_bound_limit", "invalid operand: immediate value not fit in signed range."
            end if
        else rawmatch =UNSIGNED?, _
            if (((value) < $00) | ((value) >= ulimit))
                error "_bound_limit", "invalid operand: immediate value not fit in unsigned range."
            end if
        else
            error "_bound_limit", "invalid operand: operand must be either SIGNED or UNSIGNED."
        end rawmatch
    else if (((value) < (-_slimit)) | ((value) >= ulimit))
        error "_bound_limit", "invalid operand: immediate value not fit properly."
    end if
end macro
