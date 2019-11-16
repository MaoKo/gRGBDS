
struc _append_string? src?*&
    assert (. eqtype "")
    iterate value, src
        . =: string (((value) shl ($08 * lengthof (.))) or (.))
    end iterate
end struc

struc _int_pow base?*, exp?*
    if (exp)
        . = base
        repeat (exp - $01)
            . = base * .
        end repeat
    else
        . = $01
    end if
end struc

struc _match_n? n?*
    local match_line, begin, count, total_rept
    macro invoker?!
    end macro
    total_rept _int_pow 2, n
    begin = $01
    . = ""
    . _append_string "macro ?! line?&",     $0A
    . _append_string "match =_OFF, line",   $0A
    . _append_string "purge ?",             $0A
    . _append_string "else",                $0A
    . _append_string "esc macro invoker?!", $0A
    . _append_string "esc invoker",         $0A
    . _append_string "line",                $0A
    . _append_string "esc end macro",       $0A
    . _append_string "end match",           $0A
    . _append_string "end macro",           $0A
    repeat (total_rept)
        count = (%% - %) + $01
        if (bsr count) = (bsf count)
            repeat $01, i:$00
                eval "match_line equ _", (`i)
            end repeat
            repeat (count - $01)
                eval "match_line reequ match_line _", (`%)
            end repeat
            match _, match_line
                if (~(begin))
                    . _append_string "else", " "
                end if
                . _append_string "match ", (`_), " remain, buffer"
            end match
            repeat count, i:$00
                . _append_string string($0A), "define token _", (`i)
            end repeat
            . _append_string string($0A), "redefine buffer remain", string($0A)
            begin = $00
        end if
    end repeat
    . _append_string "else",                $0A
    . _append_string "match _, buffer",     $0A
    . _append_string "define token _",      $0A
    . _append_string "end match",           $0A
    . _append_string "break",               $0A
    . _append_string "end match",           $0A
    . _append_string "_OFF",                $0A
end struc

result _match_n $04
eval result

macro irps?! parameter?*, text?*&
    local buffer, token
    buffer equ text
    while $01
        invoker
    end while
    outscope irpv parameter, token
end macro

;macro end?.irps?!
;    end irpv
;end macro

repeat $10000
irps A, _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
end irpv
end repeat
