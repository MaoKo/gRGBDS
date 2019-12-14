
struc _itoa_base? src?*, base?*
    local quotien, cbyte
    _assert_numeric (src), (base)
    if (base <= $01)
        error "_itoa_base", <"Base too small `", (`base), "`">
    else if (base > $10)
        error "_itoa_base", <"Base too large `", (`base), "`">
    else if (~(definite __hexadecimal_digit))
        virtual at $00
            __hexadecimal_digit::
            emit $01: "0123456789ABCDEF"
        end virtual
    end if
    quotien = src
    . = ""
    while (quotien)
        load cbyte : byte from __hexadecimal_digit : (quotien mod base)
        quotien = (quotien / base)
        .  = string ((. shl $08) + cbyte)
    end while
end struc
