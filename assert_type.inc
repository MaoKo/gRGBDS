
iterate <type,value>, string,"", numeric,$00, float,0.0f
    macro _assert_#type? line?*&
        iterate text, line
            if (~((text) eqtype value))
                err "The parameter ", (`text), " must be of type ", (`type)
            end if
        end iterate
    end macro
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
