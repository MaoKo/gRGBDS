iterate <type,value>, string,"", numeric,$00, float,0.0f
    macro must_#type? line?*&
        iterate text, line
            if (~((text) eqtype value))
                err "The parameter ", (`text), " must be of type ", (`type)
            end if
        end iterate
    end macro
    match =string?, type
    else
        macro must_#type#_unsigned? expr?*
            must_#type (expr)
            if (~((expr) > value))
                err "Value ", (`expr), " is not an unsigned ", (`type)
            end if
        end macro
    end match
end iterate
