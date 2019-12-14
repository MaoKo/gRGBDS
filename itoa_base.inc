
struc _itoa_base_core? source?*, base?*
    local quotien, digit, sign
    if (~(definite __hexadecimal_digit))
        virtual at $00
            __hexadecimal_digit::
            emit $01: "0123456789ABCDEF"
        end virtual
    end if
    quotien = (+source)
    sign = $00
    if (source < $00)
        quotien = (-source)
        sign = $01
    end if
    . = ""
    while (quotien)
        load digit : byte from __hexadecimal_digit : (quotien mod base)
        quotien = (quotien / base)
        .  = string ((. shl $08) + digit)
    end while
    if (sign)
        .  = string ((. shl $08) + "-")
    end if
end struc

struc _itoa_base? source?*, base?*
    _assert_numeric (source), (base)
    if (base <= $01)
        error "_itoa_base", <"Base too small `", (`base), "`">
    else if (base > $10)
        error "_itoa_base", <"Base too large `", (`base), "`">
    else
        . _itoa_base_core source, base
    end if
end struc
