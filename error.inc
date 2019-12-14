
macro error? macro_name?*, messages?*&
    local line, ident
    eval "ident _fasmg_identifier ", macro_name
    line _itoa_base_core __line__, $0A
    display "**** ERROR DETECTED ****", string ($0A0D)
    display "**** FILE : ", __source__, " ****", string($0A0D)
    display "**** LINE : ", line, " ****", string ($0A0D)
    if (ident <> __FASMG_IDENTIFIER)
        display "WARNING : '", macro_name, "' NOT A VALID IDENTIFIER.", string($0A0D)
    end if
    iterate message, messages
        display "[", macro_name, "]: ", message, string ($0A0D)
    end iterate
    err ""
end macro
