
macro error? macro_name?*, messages?*&
    local line
    line _itoa_base __line__, $0A
    display "**** ERROR DETECTED ****", string ($0A0D)
    display "**** FILE : ", __source__, " ****", string($0A0D)
    display "**** LINE : ", line, " ****", string ($0A0D)
    iterate message, messages
        display "[", macro_name, "]: ", message, string ($0A0D)
    end iterate
    err ""
end macro
