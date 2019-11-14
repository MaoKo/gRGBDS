
_iterate_string_symbols_table? := $00
namespace _iterate_string_symbols_table?
end namespace

macro _core_iterate_string? parameter?*, text?*
    local start, iterator
    _assert_string (text)
    start = $00
    define iterator parameter
    rawmatch _ =: begin, parameter
        _assert_numeric (begin)
        start = begin
        define iterator  _
    end rawmatch
    repeat $01, text_number:text
        namespace _iterate_string_symbols_table?
                if (~(definite ?text_number?))
                    define  ?text_number?
                    restore ?text_number?
                    ?text_number? = $00
                    namespace ?text_number?
                        repeat (lengthof (text) - start), i:(start)
                            node =: (((text) shr ($08 * i)) and $FF)
                        end repeat
                    end namespace
                end if
        end namespace
        match _, iterator
            outscope irpv _, _iterate_string_symbols_table?.text_number?.node
end macro

macro _iterate_string?! line?*&
    _pairing repeat, match, irpv
    repeat
    match
    irpv
    outscope _core_iterate_string line
end macro

macro end?._iterate_string?!
            end irpv
        end match
    end repeat
end macro

;macro _append_string? dst?*, src?*&
;    _assert_string (dst)
;    iterate value, src
;        _assert_string (value)
;        dst = string (((value) shl ($08 * lengthof (dst))) or (dst))
;    end iterate
;end macro

;macro _itoa_base? base?*, dst?*, src?
;    local quotien, cbyte
;    match _, src
;        dst = (src)
;    end match
;    _assert_numeric (base), (dst)
;    if ((base <= $01) | (base > $10))
;        err "base too small or to large"
;    else if (~(definite __hex_digit))
;        virtual at $00
;            __hexadecimal_digit::
;            emit $01: "0123456789ABCDEF"
;        end virtual
;    end if
;    quotien = dst
;    dst = ""
;    while (quotien)
;        load cbyte : byte from __hexadecimal_digit : (quotien mod base)
;        quotien = (quotien / base)
;        dst  = string ((dst shl $08) + cbyte)
;    end while
;end macro

;macro _eval_string? result?*, text?*
;    local escape, lbrace, subname
;    _assert_string (text)
;    escape = $00
;    lbrace = $00
;    result = ""
;    _iterate_string char, text
;        if (escape)
;            switch (char)
;                case '\': case '"':
;                case ';': case '{':
;                case '}':           _append_string result, char
;                                    break
;
;                case 'n':           _append_string result, string ($0A)
;                                    break
;
;                case 't':           _append_string result, string ($09)
;                                    break
;
;                case '@':           break ;** TODO **
;                case '0' ... '9':   break ;** TODO **;
;                default:            err "illegal character escaped: ", char
;            end switch
;            escape = $00
;        else if (lbrace)
;            if (char <> '}')
;                _append_string subname, char
;            else
;                lbrace = $00
;                _assert_fasmg_identifier subname
;                eval "subname = ", subname
;                if (subname eqtype $00) | (subname eqtype 0.0)
;                    if (subname eqtype 0.0)
;                        _convert_float_fixed subname
;                    end if
;                    _itoa_base $10, subname
;                    subname = string ((subname shl $08) or '$')
;                end if 
;                _append_string result, subname
;            end if
;        else if (char = '\')
;            escape = $01
;        else if (char = '{')
;            lbrace = $01
;            subname = ""
;        else
;            _append_string result, char
;        end if
;    end _iterate_string
;    iterate <condition,error>,  escape,"a backslash '\' can't end a string",\
;                                lbrace,"right brace '}' missing"
;        if (condition)
;            err "syntax error: ", error
;        end if
;    end iterate
;end macro
