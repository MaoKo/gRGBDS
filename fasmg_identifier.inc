
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
        repeat $01, string_token:(token)
            if (definite ?string_token?)
                result = ?string_token?
                break
            end if
            if (string_token <> "")
                result = $01
                if ((string_token and $FF) >= "0") & ((string_token and $FF) <= "9")
                    result = $00
                    break
                else if  ((string_token and $FF) = "$")
                    repeat $01, string_token_2:(string_token shr $08)
                        iterate <lbound,ubound>, "0","9", "A","Z", "a","z"
                            if ((string_token_2 and $FF) >= lbound) & ((string_token_2 and $FF) <= ubound)
                                result = $00
                                break
                            end if
                        end iterate
                    end repeat
                end if
                if (result)
                    _iterate_string char, string (string_token)
                        if (~(?char?))
                            result = $00
                            break
                        end if
                    end _iterate_string
                end if
            else
                result = $00
            end if
            define  ?string_token?
            restore ?string_token?
            ?string_token? = result
        end repeat
    end namespace
end macro
