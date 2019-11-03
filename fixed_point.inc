
macro _convert_float_fixed? dst?*, src?
    local intpart, decpart
    match _, src
        dst = (src)
    end match
    must_float (dst)
    intpart = trunc (dst)
    decpart = (dst) - (float (intpart))
    dst = (intpart shl $10) or (trunc ($10000 * ((decpart * 10.0) / 10.0)))
end macro

macro _convert_fixed_float? dst?*, src?
    err "not yet implemented"
end macro
