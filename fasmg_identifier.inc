
_fasmg_identifier_symbols_table? := $00
namespace _fasmg_identifier_symbols_table?
    repeat $100, i:$00
        define  ?i?
        restore ?i?
        if (i <> "$") & (i <> "%") & (i <> ".") & ((i < "0") | (i > "9"))\
         & (i <> "@") & ((i < "A") | (i > "Z")) & (i <> "^") & (i <> "_")\
         & ((i < "a") | (i > "z"))
            ?i? = $00
        else
            ?i? = $01
        end if
    end repeat
end namespace

macro _fasmg_identifier? result?*, token?&
    _assert_string (token)
    namespace _fasmg_identifier_symbols_table?
        repeat $01, identifier:(token)
            if (definite ?identifier?)
                result = ?identifier?
                break
            end if
            match =0, identifier
                result = $00
            else
                result = $01
                if ((identifier and $FF) >= "0") & ((identifier and $FF) <= "9")
                    result = $00
                    break
                else if  ((identifier and $FF) = "$")
                    repeat $01, identifier_2:(identifier shr $08)
                        iterate <lbound,ubound>, "0","9", "A","Z", "a","z"
                            if ((identifier_2 and $FF) >= lbound) & ((identifier_2 and $FF) <= ubound)
                                result = $00
                                break
                            end if
                        end iterate
                    end repeat
                end if
                if (result)
                    _iterate_string char, string (identifier)
                        if (~(?char?))
                            result = $00
                            break
                        end if
                    end _iterate_string
                end if
            end match
            define  ?identifier?
            restore ?identifier?
            ?identifier? = result
        end repeat
    end namespace
end macro

