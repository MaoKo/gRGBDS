
define __identifier?
iterate type, TOKEN, NUMERIC, IDENTIFIER
    __FASMG_#type? := ((%) - $01)
end iterate

struc _fasmg_identifier? token?*&
    . = __FASMG_TOKEN
    redefine __identifier?.token.
    match,__identifier?.token.
        . = __FASMG_NUMERIC
        namespace __identifier?
            rawmatch _, token.
                match,_
                    . = __FASMG_IDENTIFIER
                end match
            end rawmatch
        end namespace
        restore __identifier?.token.
    end match
end struc


repeat $100000
    x _fasmg_identifier A.2.C
end repeat
