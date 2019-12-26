
iterate <type*,value*>, string,"", numeric,$00, float,0.0f
    macro _assert_#type?: expressions?*&
        iterate expression, expressions
            if (~((expression) eqtype value))
                local macro_name, identifier, target
                redefine target expression
                identifier _fasmg_identifier expression
                if (identifier = __FASMG_IDENTIFIER) & (~((expression) eqtype 0.0f))
                    if ((expression) eqtype $00)
                        target _itoa_base (expression), $0A
                    end if
                    eval "redefine target """, target, """"
                else if ((expression) eqtype $00) | ((expression) eqtype 0.0f)
                    redefine target (`expression)
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

macro _assert_element? element?*, expressions?*&
    iterate expression*, expressions
    end iterate
end macro

