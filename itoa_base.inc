
restore __hexadecimal_symbols?
if (~(definite __hexadecimal_symbols))
    __hexadecimal_symbols? = $00
    virtual at $00
        __hexadecimal_digit::
        __hexadecimal_upper_digit:
            emit $01: "0123456789ABCDEF"
        __hexadecimal_lower_digit:
            emit $01: "0123456789abcdef"
    end virtual
end if

struc _itoa_base?: source?*, base?*, choice?
    local quotien, digit, sign, default
    _assert_numeric (source), (base)
    if (base <= $01)
        error "_itoa_base", <"base too small: '", (`base), "'.">
    else if (base > $10)
        error "_itoa_base", <"base too large: '", (`base), "'.">
    else
        default = __hexadecimal_upper_digit
        match _, choice
            rawmatch =LOWER?,_
                default = __hexadecimal_lower_digit
            else rawmatch =UPPER?,_
            else
                error "_itoa_base", <"bad choice argument: '", (`_), "'.">
            end rawmatch
        end match
        quotien = (+source)
        sign = $00
        if (source < $00)
            quotien = (-source)
            sign = $01
        end if
        if (base = $0A) & (~(sign))
            repeat $01, i:(quotien)
                . = (`i)
            end repeat
        else
            . = ""
            while (quotien)
                load digit : byte from __hexadecimal_digit : default + (quotien mod base)
                quotien = (quotien / base)
                .  = string ((. shl $08) + digit)
            end while
            if (sign)
                .  = string ((. shl $08) + "-")
            end if
        end if
   end if
end struc
