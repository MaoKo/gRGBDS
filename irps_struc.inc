
struc _append_string? src?*&
    assert (. eqtype "")
    iterate value*, src
        . = string (((value) shl ($08 * lengthof (.))) or (.))
    end iterate
end struc

macro _irps_invoker?!
end macro

struc _body_irps_template? n?*
    local total_rept, placeholder, super_macro, irps_macros, count
    total_rept := ($01 shl (n))
    super_macro = ""
    irps_macros = ""
    matchs_body = ""
    super_macro _append_string  "macro ?! line?&",           $0A,\
                                "match =__OFF?, line",       $0A,\
                                "purge ?",                   $0A,\
                                "else",                      $0A,\
                                "esc macro _irps_invoker?!", $0A,\
                                "esc _irps_invoker",         $0A,\
                                "line",                      $0A,\
                                "esc end macro",             $0A,\
                                "end match",                 $0A,\
                                "end macro",                 $0A
    matchs_body _append_string "match _, buffer", $0A
    repeat ((bsr total_rept) + $01)
        count = ($01 shl (%% - %))
        redefine placeholder _1
        repeat (count - $01), i:$02
            eval "placeholder reequ placeholder, _", (`i)
        end repeat
        match _, placeholder
            if ((%) > $01)
                matchs_body _append_string "else", $0A
            end if
            irps_macros _append_string "macro _irps_match_", (`%), " "
            matchs_body _append_string "rmatch "
            iterate item, _
                irps_macros _append_string (`item), "?*, "
                matchs_body _append_string (`item), " "
            end iterate
            irps_macros _append_string "_?*, __ispace?*, token_1?*", $0A
            matchs_body _append_string "__remain, _",        $0A,\
                                       "_irps_match_", (`%), " "
            iterate item, _
                matchs_body _append_string "_", (`%), ", "
            end iterate
            matchs_body _append_string "_, __ispace, token_1",      $0A,\
                                       "redefine buffer __remain",  $0A
        end match
        repeat count, i:$01
            irps_macros _append_string "define token_1 _", (`%), $0A
            redefine placeholder =_1
            repeat (count - $01), j:$02
                if ((j - $01) = i)
                    placeholder reequ placeholder=
                    break
                end if
                eval "placeholder reequ placeholder =_", (`j)
            end repeat
            if (%% = %)
                placeholder reequ placeholder=
            end if
            match _, placeholder
                irps_macros _append_string "rmatch =1 ", (`_), " __insert, __ispace _", $0A,\
                                           "define token_1"  ,                          $0A,\
                                           "end rmatch",                                $0A
            end match
        end repeat
        irps_macros _append_string "end macro", $0A
    end repeat
    matchs_body _append_string "else",                    $0A,\
                               "rmatch __remain, _",      $0A,\
                               "define token_1 __remain", $0A,\ ; TODO HANDLE SPACE
                               "end rmatch",              $0A,\
                               "break",                   $0A
    repeat ((bsr total_rept) + $01)
        matchs_body _append_string "end rmatch", $0A
    end repeat
    matchs_body _append_string "end match", $0A
    . = ""
    . _append_string irps_macros,\
                     super_macro,\
                     matchs_body,\
                     "__OFF", $0A
end struc

__irps_pattern _body_irps_template $04
eval __irps_pattern

_irps_options? := $00
namespace _irps_options?
    iterate option, space,ident
        option? := (%)
    end iterate
end namespace

macro irps?! parameter?*, text?*&
    local iterator, identifier, stream, inc_space, inc_ident, buffer, token_1, token_2
    redefine iterator parameter
    rawmatch it =[ options =], parameter
        redefine iterator it
        rawmatch =+ option_1 =| =+ option_2, options
            if (definite _irps_options.option_1) & (definite _irps_options.option_2)\
                    & (_irps_options.option_1 <> _irps_options.option_2)
                redefine inc_space 1
                redefine inc_ident 1
            else
                err "syntax error: bad irps options"
            end if
        else rawmatch =+ option, options
            if (definite _irps_options.option)
                eval "redefine inc_", (`option), " 1"
            else
                err "syntax error: bad irps option"
            end if
        end rawmatch
    else
        redefine inc_space 0
        redefine inc_ident 0
    end rawmatch
    redefine identifier
    redefine stream token_1
    buffer equ text
    match __ispace __iident, inc_space inc_ident
        while $01
            _irps_invoker
        end while
    end match
;    if (inc_ident)
;        redefine stream token_2
;        irpv item, token_1
;            ivalid _fasmg_identifier item
;            match _, identifier
;                if (ivalid)
;                    rawmatch =., item
;                        eval "redefine identifier ", (`_), "."
;                    else
;                        define  _.
;                        match __dotend, _
;                            eval "redefine identifier ", (`_), (`item)
;                        else
;                            define token_2 _
;                            if (ivalid = _fasmg_token_lookup.__FASMG_IDENTIFIER)
;                                redefine identifier item
;                            end if
;                        end match
;                        restore _.
;                    end rawmatch
;                else
;                    redefine identifier
;                    define   token_2 _
;                end if
;            else
;                if (ivalid = _fasmg_token_lookup.__FASMG_IDENTIFIER)
;                    redefine identifier item
;                end if
;            end match
;            if (~(ivalid))
;                define token_2 item
;            end if
;        end irpv
;        match _, identifier
;            define token_2 _
;        end match
;    end if
    match it st, iterator stream
        outscope irpv it, st
end macro

macro end?.irps?!
        end irpv
    end match
end macro

