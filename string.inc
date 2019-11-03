
include "switch.inc"
include "fixed_point.inc"
include "macro_builder.inc"

macro _iterate_string?! parameter?*, text?*
    local char, start, iterator
    must_string (text)
    start = $00
    iterator equ parameter
    match it =: begin, parameter
        must_numeric (begin)
        start = begin
        iterator equ it
    end match
    repeat (lengthof (text) - start), i:(start)
        char =: string (((text) shr ($08 * i)) and $FF)
    end repeat
    match it, iterator
        outscope irpv it, char
end macro

macro end?._iterate_string?!
        end irpv
    end match
end macro

macro _append_string? dst?*, src?*&
    must_string (dst)
    iterate value, src
        must_string (value)
        dst = string (((value) shl ($08 * lengthof (dst))) or (dst))
    end iterate
end macro

macro _check_identifier? result?*, identifier?*
    must_string (identifier)
    result = $01
    if (~(lengthof (identifier)))
        result = $00
    else
        _iterate_string char, identifier
            if (~ (((char >= 'a') & (char <= 'z'))\
                 | ((char >= 'A') & (char <= 'Z'))\
                 |  (char = '_')))
                result = $00
                break
            end if
        end _iterate_string
    end if
end macro

macro _ensure_identifier? identifier?*&
    local check
    iterate ident, identifier
        _check_identifier check, ident
        if (~(check))
            if (~(lengthof (identifier)))
                err "syntax error: empty identifier"
            end if
            err "syntax error: bad identifier"
        end if
    end iterate
end macro

macro _itoa_base? base?*, dst?*, src?
    local quotien, cbyte
    match _, src
        dst = (src)
    end match
    must_numeric (base), (dst)
    if ((base <= $01) | (base > $10))
        err "base too small or to large"
    else if (~(definite __hex_digit))
        virtual at $00
            __hex_digit::
            emit $01: "0123456789ABCDEF"
        end virtual
    end if
    quotien = dst
    dst = ""
    while (quotien)
        load cbyte : byte from __hex_digit : (quotien mod base)
        quotien = (quotien / base)
        dst  = string ((dst shl $08) + cbyte)
    end while
end macro

macro _eval_string? result?*, text?*
    local escape, lbrace, subname
    must_string (text)
    escape = $00
    lbrace = $00
    result = ""
    _iterate_string char, text
        if (escape)
            switch (char)
                case '\': case '"':
                case ';': case '{':
                case '}':           _append_string result, char
                                    break

                case 'n':           _append_string result, string ($0A)
                                    break

                case 't':           _append_string result, string ($09)
                                    break

                case '@':           break ;** TODO **
                case '0' ... '9':   break ;** TODO **;
                default:            err "illegal character escaped: ", char
            end switch
            escape = $00
        else if (lbrace)
            if (char <> '}')
                _append_string subname, char
            else
                lbrace = $00
                _ensure_identifier subname
                eval "subname = ", subname
                if (subname eqtype $00) | (subname eqtype 0.0)
                    if (subname eqtype 0.0)
                        _convert_float_fixed subname
                    end if
                    _itoa_base $10, subname
                    subname = string ((subname shl $08) or '$')
                end if 
                _append_string result, subname
            end if
        else if (char = '\')
            escape = $01
        else if (char = '{')
            lbrace = $01
            subname = ""
        else
            _append_string result, char
        end if
    end _iterate_string
    iterate <condition,error>,  escape,"A backslash '\' can't end a string",\
                                lbrace,"Right brace '}' missing"
        if (condition)
            err error
        end if
    end iterate
end macro
