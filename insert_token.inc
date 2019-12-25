
_insert_token? := $00
redefine _insert_token?.EMPTY?

struc _insert_token? source?&
    local prefix, suffix, insert
    define  .                       ; force variable ;
    restore .                       ; force variable ;
    redefine prefix ". reequ "
    redefine suffix ""
    insert = $00
    irpv _, .
        indx (%%)
        redefine suffix (`_)
        break
    end irpv
    match _, source
        suffix reequ suffix, (`_)
        match __insert_space= , source
            insert = $01
        end match
    else
        insert = $01
    end match
    if (insert)
        suffix reequ suffix, (" _insert_token.EMPTY ")
    end if
    eval prefix, suffix
end struc
