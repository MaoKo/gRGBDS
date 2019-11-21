
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
            irps_macros _append_string "_?*, __ispace?*, token?*, handle_ident?*", $0A,\
                                       "iterate item, "
            matchs_body _append_string "__remain, _",        $0A,\
                                       "irps_options _, "
            iterate item, _
                ;irps_macros _append_string "_", (`%)
                matchs_body _append_string "_", (`%)
                if ((%%) > (%))
                    matchs_body _append_string ", "
                end if
            end iterate
           ; irps_macros _append_string $0A, "define token item", $0A, "end iterate", $0A
            matchs_body _append_string $0A, "redefine buffer __remain", $0A
            ;"_, __ispace, token, __irps_identifier", $0A,\
        end match
        ;repeat count, i:$01
        ;    ;irps_macros _append_string "handle_ident _", (`%), $0A
        ;    redefine placeholder =_1
        ;    repeat (count - $01), j:$02
        ;        if ((j - $01) = i)
        ;            placeholder reequ placeholder=
        ;            break
        ;        end if
        ;        eval "placeholder reequ placeholder =_", (`j)
        ;    end repeat
        ;    if (%% = %)
        ;        placeholder reequ placeholder=
        ;    end if
        ;    match _, placeholder
        ;    ;    irps_macros _append_string "rmatch =1 ", (`_), " __insert, __ispace _", $0A,\
        ;    ;                               "define token",                              $0A,\
        ;    ;                               "identifier = $00",                          $0A,\
        ;    ;                               "end rmatch",                                $0A
        ;    end match
        ;end repeat
        ;irps_macros _append_string "end macro", $0A
    end repeat
    matchs_body _append_string "else",                       $0A,\
                               "rmatch __remain, _",         $0A,\
                               "define token __remain",      $0A,\ ; TODO HANDLE SPACE
                               "end rmatch",                 $0A,\
                               "break",                      $0A
    repeat ((bsr total_rept) + $01)
        matchs_body _append_string "end rmatch", $0A
    end repeat
    matchs_body _append_string "end match", $0A
    . = ""
    . _append_string super_macro,\
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

macro __irps_handle_options?!
    rawmatch =1 =1, __ispace __iident
        macro irps_options? _?*, items?*&
            redefine reach_item
            iterate item, items
                ivalid _fasmg_identifier item
                if (identifier) & (ivalid)
                    match _, token
                        rawmatch =., item
                            redefine token  _.
                        else
                            rawmatch __dotend =., _
                               redefine token __dotend.item
                            else
                                define token item
                                if (ivalid <> __FASMG_IDENTIFIER)
                                    identifier = $00
                                end if
                            end rawmatch
                        end rawmatch
                    end match
                else
                    identifier = $00
                    if (ivalid = __FASMG_IDENTIFIER)
                        identifier = $01
                    end if
                    define token item
                end if
                irpv __reach, reach_item
                    rawmatch __reach =item= __insert, _
                        define token
                    end rawmatch
                end irpv
                reach_item reequ reach_item =item
            end iterate
        end macro
    else rawmatch =1, __ispace
        macro irps_options? _?*, items?*&
            redefine reach_item
            iterate item, items
                define token item
                irpv __reach, reach_item
                    rawmatch __reach =item= __insert, _
                        define token
                    end rawmatch
                end irpv
                reach_item reequ reach_item =item
            end iterate
        end macro
    else rawmatch =1, __iident
        macro irps_options? _?*, items?*&
            iterate item, items
                ivalid _fasmg_identifier item
                if (identifier) & (ivalid)
                    match _, token
                        rawmatch =., item
                            redefine token  _.
                        else
                            rawmatch __dotend =., _
                               redefine token __dotend.item
                            else
                                define token item
                                if (ivalid <> __FASMG_IDENTIFIER)
                                    identifier = $00
                                end if
                            end rawmatch
                        end rawmatch
                    end match
                else
                    identifier = $00
                    if (ivalid = __FASMG_IDENTIFIER)
                        identifier = $01
                    end if
                    define token item
                end if
            end iterate
        end macro
    else
        macro irps_options? _?*, items?*&
            iterate item, items
                define token item
            end iterate
        end macro
    end rawmatch
end macro

macro irps?! parameter?*, text?*&
    local iterator, identifier, inc_space, inc_ident, buffer, token, reach_item, ivalid
    redefine iterator parameter
    redefine inc_space 0
    redefine inc_ident 0
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
                eval "redefine", " ", "inc_", (`option), " ", "1"
            else
                err "syntax error: bad irps option"
            end if
        end rawmatch
    end rawmatch
    buffer  equ text
    identifier = $00
    match __ispace __iident, inc_space inc_ident
        __irps_handle_options
    end match
    while $01
        _irps_invoker
    end while
    match it, iterator
        outscope irpv it, token
end macro

macro end?.irps?!
        end irpv
    end match
end macro
