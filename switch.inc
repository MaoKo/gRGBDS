
macro switch? item?*
    local default_match, case_match, count_invoker, used_default, already_match, list_value
    default_match   = $00
    case_match      = $00
    count_invoker   = $00
    used_default    = $00
    already_match   = $00

    redefine list_value
    restore  list_value

    macro case? value?*
        irpv it, list_value
            if (it eq (value))
                error "switch::case", "syntax error: case value redifinition."
            end if
        end irpv
        list_value equ value
        if (item eq (value))
            reset_invoker
            case_match = $01
        end if
    end macro

    macro default?
        if (used_default)
            error "default::case", "syntax error: default label has been encounter more than one."
        end if
        default_match = $01
        used_default  = $01
    end macro

    macro switch_statement? line?*&
        local buffer, range
        buffer reequ line
        while $01
            match =case? _, buffer
                match value =: rest, _
                    range reequ value
                    buffer reequ rest
                else match value =:, _
                    range reequ value
                    redefine buffer
                else
                    error "switch", "syntax error: case must be followed by a ':' character."
                    break
                end match
                match low =.=.=. high, range
                    if (high <= low)
                        error "switch", "syntax error: bad case range."
                        break
                    end if
                    repeat ((high - low) + $01), i:(low)
                        case i
                    end repeat
                else
                    case range
                end match
            else match =default? _, buffer
                match =: rest, _
                    buffer reequ rest
                else match =:, _
                    redefine buffer
                else
                    error "switch", "syntax error: default must be follow by a ':' character."
                    break
                end match
                default
            else
                match _, buffer
                    rawmatch =break?, _
                        if (case_match)
                            already_match = $01
                        end if
                        case_match    = $00
                        default_match = $00
                    else rawmatch =case?, _
                        error "switch", "syntax error: case cannot be alone."
                    else rawmatch =default?, _
                        error "switch", "syntax error: default cannot be alone."
                    end rawmatch
                end match
                break
            end match
        end while
        if ((case_match) | (default_match)) & (~(already_match))
            add_invoker buffer
        end if
    end macro

    macro add_invoker? line&
        match command, line
            count_invoker = count_invoker + $01
            macro invoker?
                invoker
                command
            end macro
        end match
    end macro

    macro reset_invoker?
        repeat count_invoker
            purge invoker?
        end repeat
        count_invoker = $00
    end macro

    macro invoker?
    end macro

    macro ?! line&
        local end_reach
        end_reach = $00
        match =end? =switch?, line
            end_reach = $01
        else match =end? =. =switch?, line
            end_reach = $01
        end match
        if (~(end_reach))
            match _, line
                rawmatch =macro? parameter, _
                    error "switch", "syntax error: macro can't be declared here."
                end rawmatch
                switch_statement line
            end match
        else
            end switch
        end if
    end macro
end macro

macro end?.switch?
    invoker
    reset_invoker
    purge add_invoker?, reset_invoker?, invoker?,\
          switch_statement?, case?, default?, ?
end macro
