
struc _append_string? src?*&
    assert (. eqtype "")
    iterate value*, src
        . = string (((value) shl ($08 * lengthof (.))) or (.))
    end iterate
end struc

macro _irps_invoker?!
end macro

struc _body_irps_template? n?*
    local total_rept, placeholder, super_macro, matchs_body, futur_param, count
    total_rept := ($01 shl (n))
    super_macro = ""
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
            matchs_body _append_string "rmatch "
            futur_param = ""
            iterate item, _
                futur_param _append_string "<", (`item), ">"
                if ((%%) > (%))
                    futur_param _append_string ", "
                end if
                matchs_body _append_string (`item), " "
            end iterate
            matchs_body _append_string "__remain, _",                     $0A,\
                                       "irps_options <_>, ", futur_param, $0A,\
                                       "redefine buffer __remain",        $0A
        end match
    end repeat
    matchs_body _append_string "else",                       $0A,\
                               "rmatch __remain, _",         $0A,\
                               "irps_options <_>, __remain", $0A,\
                               "if (end_space)",             $0A,\
                               "define token",               $0A,\
                               "end if",                     $0A,\
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

macro __irps_handle_options?!
    if (inc_space) & (inc_ident)
        macro irps_options? _?*, items?*&
            redefine reach_item
            iterate item, items
                id_valid _fasmg_identifier item
                if (identifier) & (id_valid)
                    match __token, token
                        rawmatch =., item
                            redefine token  __token.
                        else
                            rawmatch __dotend =., __token
                               redefine token __dotend.item
                            else rawmatch     =., __token
                               redefine token         .item
                            else
                                define token item
                                if (id_valid <> __FASMG_IDENTIFIER)
                                    identifier = $00
                                end if
                            end rawmatch
                        end rawmatch
                    end match
                else
                    identifier = $00
                    if (id_valid = __FASMG_IDENTIFIER)
                        identifier = $01
                    end if
                    define token item
                end if
                irpv __reach, reach_item
                    rawmatch __reach =item= __insert, _
                        identifier = $00
                        define token
                    end rawmatch
                end irpv
                reach_item reequ reach_item =item
            end iterate
        end macro
    else if (inc_space)
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
    else if (inc_ident)
        macro irps_options? _?*, items?*&
            iterate item, items
                id_valid _fasmg_identifier item
                if (identifier) & (id_valid)
                    match __token, token
                        rawmatch =., item
                            redefine token  __token.
                        else
                            rawmatch __dotend =., __token
                               redefine token __dotend.item
                            else rawmatch     =., __token
                               redefine token         .item
                            else
                                define token item
                                if (id_valid <> __FASMG_IDENTIFIER)
                                    identifier = $00
                                end if
                            end rawmatch
                        end rawmatch
                    end match
                else
                    identifier = $00
                    if (id_valid = __FASMG_IDENTIFIER)
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
    end if
end macro

_irps_options? := $00
namespace _irps_options?
    space? := "ident"
    ident? := "space"
end namespace

macro irps?! parameter?*, text?*&
    local iterator, identifier, inc_space, inc_ident, buffer, token, reach_item, id_valid, end_space
    redefine iterator parameter
    rawmatch it =[ options =], parameter
        redefine iterator it
        rawmatch =+ option_1 =| =+ option_2, options
            if (definite _irps_options.option_1) & (definite _irps_options.option_2)\
                    & (_irps_options.option_1 <> _irps_options.option_2)
                inc_space = $01
                inc_ident = $01
            else
                err "syntax error: bad irps options"
            end if
        else rawmatch =+ option, options
            if (definite _irps_options.option)
                eval "inc_", (`option), " = ", "$01"
                eval "inc_", (_irps_options.option), " = ", "$00"
            else
                err "syntax error: bad irps option"
            end if
        end rawmatch
    else
        inc_space = $00
        inc_ident = $00
    end rawmatch
    buffer  equ text
    identifier = $00
    end_space = $00
    if (inc_space)
        match _= ,buffer
            end_space = $01
        end match
    end if
    __irps_handle_options
    while $01
        _irps_invoker
    end while
    purge irps_options?
    match it, iterator
        outscope irpv it, token
end macro

macro end?.irps?!
        end irpv
    end match
end macro
