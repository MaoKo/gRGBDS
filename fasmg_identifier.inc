
_fasmg_token_lookup? := $00
namespace _fasmg_token_lookup?
    iterate kind, TOKEN, NUMERIC, IDENTIFIER
        __FASMG_#kind? := ((%) - $01)
    end iterate
end namespace

define __identifier?
struc __fasmg_token_entry?
    . = _fasmg_token_lookup.__FASMG_TOKEN
    redefine  __identifier?.token.
    match,__identifier?.token.
        . = _fasmg_token_lookup.__FASMG_NUMERIC
        namespace __identifier?
            rawmatch _, token.
                match,_
                    . = _fasmg_token_lookup.__FASMG_IDENTIFIER
                end match
            end rawmatch
        end namespace
    end match
end struc

struc _fasmg_identifier? token?&
    match _, token
        repeat $01, i:(`token)
            if (definite _fasmg_token_lookup.i)
                . = _fasmg_token_lookup.i
            else
                . __fasmg_token_entry
                restore _fasmg_token_lookup.i
                _fasmg_token_lookup.i = .
            end if
        end repeat
    else
        . = $00
    end match
end struc
