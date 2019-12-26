
_iterate_string_cache? := $00
macro _core_iterate_string? parameter?*, text?*
    local start, iterator
    _assert_string (text)
    start = $00
    redefine iterator parameter
    rawmatch _ =: begin, parameter
        _assert_numeric (begin)
        start = begin
        redefine iterator  _
    end rawmatch
    repeat $01, number:text
        namespace _iterate_string_cache?
            if (~(definite ?number?))
                redefine ?number?
                restore  ?number?
                ?number? = $00
                namespace ?number?
                    repeat (lengthof (text) - start), i:(start)
                        node =: string (((text) shr ($08 * i)) and $FF)
                    end repeat
                end namespace
            end if
        end namespace
        match _, iterator
            outscope irpv _, _iterate_string_cache?.number?.node
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

struc _append_string? sources?*&
    define  .
    restore .
    if (~(definite .))
        . = ""
    end if
    _assert_string (.)
    iterate source*, sources
        _assert_string (source)
        . = string (((source) shl ($08 * lengthof (.))) or (.))
    end iterate
end struc

struc _eval_string? text?*
    local escape, lbrace, subname, identifier
    _assert_string (text)
    escape = $00
    lbrace = $00
    _iterate_string char, text
        if (escape)
            switch (char)
                case '\': case '"':
                case ';': case '{':
                case '}':           . _append_string char
                                    break

                case 'n':           . _append_string string ($0A)
                                    break

                case 't':           . _append_string string ($09)
                                    break

                case '@':           break ; ** TODO ** ;
                case '0' ... '9':   break ; ** TODO ** ;
                default:            error "_eval_string", <"illegal character escaped: ", char, ".">
            end switch
            escape = $00
        else if (lbrace)
            if (char <> '}')
                ;if (char = ' ')
                ;    error "_eval_string", "invalid name: Space can't be added in the name."
                ;end if
                subname _append_string char
            else
                lbrace = $00
                eval "identifier _fasmg_identifier ", subname
                if (identifier <> __FASMG_IDENTIFIER)
                    error "_eval_string", "Bad identifier name."
                end if
                eval "subname = ", subname
                if (subname eqtype $00) | (subname eqtype 0.0f)
                    if (subname eqtype 0.0f)
                        ;??? _convert_float_fixed subname TODO
                    end if
                    subname _itoa_base subname, $10
                    subname = string ((subname shl $08) or '$')
                end if 
                . _append_string subname
            end if
        else if (char = '\')
            escape = $01
        else if (char = '{')
            lbrace = $01
            subname = ""
        else
            . _append_string char
        end if
    end _iterate_string
    iterate <condition*,message*>,  escape,"a backslash '\' can't end a string",\
                                    lbrace,"a right brace '}' is missing"
        if (condition)
            error "_eval_string", <"syntax error: ", message, ".">
        end if
    end iterate
end struc
