
struc _convert_float_fixed? source?
    local intpart, decpart
    match _, source
        . = (source)
    end match
    _assert_float (.)
    intpart = trunc (.)
    decpart = (.) - (float (intpart))
    . = (intpart shl $10) or (trunc ($10000 * ((decpart * 10.0) / 10.0)))
end struc

macro _convert_fixed_float? dst?*, src?
    err "not yet implemented"
end macro
