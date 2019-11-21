
define __identifier?
iterate kind, TOKEN, NUMERIC, IDENTIFIER
    __FASMG_#kind? := ((%) - $01)
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
    end match
    restore  __identifier?.token.
end struc
