
iterate <type*,value*>, string,"", numeric,$00, float,0.0f
    macro _assert_#type?: line?*&
        iterate text, line
            if (~((text) eqtype value))
                local macro_name, identifier, target
                redefine target text
                identifier _fasmg_identifier text
                if (identifier = __FASMG_IDENTIFIER) & (~((text) eqtype 0.0f))
                    if ((text) eqtype $00)
                        target _itoa_base (text), $0A
                    end if
                    eval "redefine target """, target, """"
                else if ((text) eqtype $00) | ((text) eqtype 0.0f)
                    redefine target (`text)
                end if
                eval "macro_name = ""_assert_", (`type), """"
                irpv _, target
                    error macro_name, <"The parameter ", (_), " must be of type ", (`type), ".">
                end irpv
            end if
        end iterate
    end macro

    ; TODO
    match =string?, type
        macro _assert_string_not_empty? line?*&
            iterate string, line
                _assert_string string
                if (~(lengthof (string)))
                    err "The string ", (`string), " is an empty string"
                end if
            end iterate
        end macro
    else
        macro _assert_#type#_unsigned? line?*&
            iterate expression, line
                _assert_#type expression
                if (~((expression) > value))
                    err "Value ", (`expression), " is not an unsigned ", (`type)
                end if
            end iterate
        end macro
    end match
end iterate

macro _assert_fasmg_identifier? token?    
    local valid
    _fasmg_identifier valid, token
    if (~(valid))
        err "The token '", (`token) "' is not a valid identifier"
    end if
end macro

;macro _assert_defined_field? namespace?*, field?*
;    if (elementsof (field)) & (field relativeto namespace._undefined_field)
;        err "syntax error: field '", (`field), "' have an undefined value"
;    end if
;end macro
