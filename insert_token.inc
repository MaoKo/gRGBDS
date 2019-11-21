
struc _insert_token? source?&
    local prefix, suffix, empty
    define  .   ; force variable
    restore .   ; force variable
    redefine prefix ". reequ "
    redefine suffix ""
    redefine empty
    irpv _, .
        indx (%%)
        redefine suffix (`_)
        break
    end irpv
    match _, source
        suffix reequ suffix, (`_)
        match __insert= , source
            suffix reequ suffix, (" empty")
        end match
    else
        suffix reequ suffix, (" empty")
    end match
    eval prefix, suffix
end struc
