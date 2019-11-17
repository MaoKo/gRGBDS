
struc _append_string? src?*&
    assert (. eqtype "")
    iterate value, src
        . =: string (((value) shl ($08 * lengthof (.))) or (.))
    end iterate
end struc

macro invoker?!
end macro

struc _body_irps_n? n?*
    local total_rept, count, match_line, space_line
    total_rept := ($01 shl (n))
    define match_line
    define space_line
    . = ""
    . _append_string "macro ?! line?&",     $0A,\
                     "match =__OFF?, line", $0A,\
                     "purge ?",             $0A,\
                     "else",                $0A,\
                     "esc macro invoker?!", $0A,\
                     "esc invoker",         $0A,\
                     "line",                $0A,\
                     "esc end macro",       $0A,\
                     "end match",           $0A,\
                     "end macro",           $0A,\
                     "match _, buffer",     $0A
    repeat ((bsr total_rept) + $01)
        count = ($01 shl (%% - %))
        repeat $01, i:$00
            eval "redefine match_line _", (`i)
        end repeat
        repeat (count - $01)
            eval "match_line reequ match_line _", (`%)
        end repeat
        match _, match_line
            if (% > $01)
                . _append_string "else "
            end if
            . _append_string "rmatch ", (`_), " __remain, _", $0A
        end match
        repeat count, i:$00
            . _append_string "define token _", (`i), $0A
            redefine space_line =_0
            repeat (count - $01), j:$00
                if (i = j)
                    space_line reequ space_line=
                    break
                end if
                eval "space_line reequ space_line =_", (`%)
            end repeat
            if (%% = %)
                space_line reequ space_line=
            end if
            match _, space_line
                . _append_string "rmatch ", `_, " __insert, _", $0A,\
                                 "define token",               $0A,\
                                 "end rmatch",                  $0A
            end match
        end repeat
        . _append_string "redefine buffer __remain", $0A
    end repeat
    . _append_string "else",                  $0A,\
                     "rmatch __remain, _",    $0A,\
                     "define token __remain", $0A,\
                     "end rmatch",            $0A,\
                     "break",                 $0A,\
                     "end rmatch",            $0A,\
                     "end match",             $0A,\
                     "__OFF",                 $0A
end struc

result  _body_irps_n $04
;eval    result


macro invoker?!
match _, buffer
rmatch _0 _1 _2 _3 _4 _5 _6 _7 _8 _9 _10 _11 _12 _13 _14 _15 __remain, _
define token _0
rmatch =_0= __insert, _
define token
end rmatch
define token _1
rmatch =_0 =_1= __insert, _
define token
end rmatch
define token _2
rmatch =_0 =_1 =_2= __insert, _
define token
end rmatch
define token _3
rmatch =_0 =_1 =_2 =_3= __insert, _
define token
end rmatch
define token _4
rmatch =_0 =_1 =_2 =_3 =_4= __insert, _
define token
end rmatch
define token _5
rmatch =_0 =_1 =_2 =_3 =_4 =_5= __insert, _
define token
end rmatch
define token _6
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6= __insert, _
define token
end rmatch
define token _7
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7= __insert, _
define token
end rmatch
define token _8
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8= __insert, _
define token
end rmatch
define token _9
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9= __insert, _
define token
end rmatch
define token _10
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9 =_10= __insert, _
define token
end rmatch
define token _11
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9 =_10 =_11= __insert, _
define token
end rmatch
define token _12
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9 =_10 =_11 =_12= __insert, _
define token
end rmatch
define token _13
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9 =_10 =_11 =_12 =_13= __insert, _
define token
end rmatch
define token _14
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9 =_10 =_11 =_12 =_13 =_14= __insert, _
define token
end rmatch
define token _15
rmatch =_0 =_1 =_2 =_3 =_4 =_5 =_6 =_7 =_8 =_9 =_10 =_11 =_12 =_13 =_14 =_15= __insert, _
define token
end rmatch
redefine buffer __remain
end rmatch
end match
    match _, buffer
        rmatch _0 _1 _2 _3 __remain, _
            define token _0
            rmatch =_0= __insert, _
            define token
            end rmatch
            define token _1
            rmatch =_0 =_1= __insert, _
            define token
            end rmatch
            define token _2
            rmatch =_0 =_1 =_2= __insert, _
            define token
            end rmatch
            define token _3
            rmatch =_0 =_1 =_2 =_3= __insert, _
            define token
            end rmatch
            redefine buffer __remain
        end rmatch
    end match
    match _, buffer
        rmatch _0 _1 __remain, _
            define token _0
            rmatch =_0= __insert, _
            define token
            end rmatch
            define token _1
            rmatch =_0 =_1= __insert, _
            define token
            end rmatch
            redefine buffer __remain
        end rmatch
    end match
    match _, buffer
        rmatch _0 __remain, _
            define token _0
            rmatch =_0= __insert, _
            define token
            end rmatch
            redefine buffer __remain
        else
            rmatch __remain, _
                define token __remain,
            end rmatch
            break
        end rmatch
    end match
end macro

macro irps?! parameter?*, text?*&
    local iterator, buffer, token, token_1, token_2
    define iterator parameter
    rawmatch it =[ options =], parameter
        redefine iterator it
        rawmatch =+ option_1 =| =+ option_2, options
            rawmatch =space? =ident?, option_1 option_2
            else rawmatch =ident? =space?, option_1 option_2
            else
                err "syntax error: bad irps option"
            end rawmatch
            redefine inc_space 1
            redefine inc_ident 1
        else rawmatch =+ =space?, options
            redefine inc_space 1
        else rawmatch =+ =ident?, options
            redefine inc_ident 1
        else
            err "syntax error: bad irps option"
        end rawmatch
    end rawmatch
    buffer equ text
    while $01
        invoker
    end while
    match _, iterator
        outscope irpv _, token
end macro

;macro end?.irps?!
;    end irpv
;end macro

repeat $10000
irps A, _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
end irpv
end match
end repeat
