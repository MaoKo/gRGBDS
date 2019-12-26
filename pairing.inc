
macro _pairing? list?*&
    iterate name*, list
        macro name?
            purge name?
        end macro
    end iterate
end macro
