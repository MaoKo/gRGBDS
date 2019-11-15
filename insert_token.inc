
macro _insert_token? dst?*, src?&
    local prefix, suffix, empty
    define  dst ; force variable
    restore dst
    define prefix "dst reequ "
    define suffix ""
    define empty
    irpv value_dst, dst
        indx %%
        define suffix (`value_dst)
        break
    end irpv
    match value_src, src
        suffix reequ suffix, (`value_src)
        match insert_space= , src
            suffix reequ suffix, (" empty")
        end match
    else
        suffix reequ suffix, (" empty")
    end match
    eval prefix, suffix
end macro
