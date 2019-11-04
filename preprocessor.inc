
_constant_pool? := $00
namespace _constant_pool?
    iterate kind, string,float
        _constant_pool?._#kind? := $00
        namespace _#kind?
            __index = $00
        end namespace
    end iterate
end namespace

macro _insert_token? dst?*, src?&
    local prefix, suffix, empty
    define  dst ; force variable
    restore dst
    prefix equ "dst reequ "
    suffix equ ""
    empty  equ
    iirpv value, LAST, dst
        suffix equ (`value)
    end iirpv
    match value, src
        suffix equ suffix, (`value)
        match value2= , src
            suffix equ suffix, (" empty")
        end match
    else
        suffix equ suffix, (" empty")
    end match
    eval prefix, suffix
end macro

macro _add_constant_pool? result?*, value?*
    local type, found
    result equ _constant_pool._
    type   equ
    found  equ
    iterate <literal,target>, "",string, 0.0,float
        if (value eqtype literal)
            type equ target
            break
        end if
    end iterate
    match kind, type
        repeat (_constant_pool._#kind.__index), i:($00)
            if (value = _constant_pool._#kind.i)
                found equ kind.i
                break
            end if
        end repeat
        match,found
            repeat ($01), i:(_constant_pool._#kind.__index)
                _constant_pool._#kind.i = value
                found equ kind.i
            end repeat
            _constant_pool._#kind.__index = _constant_pool._#kind.__index + $01
        end match
        _insert_token result, found
    else
        err "syntax error: constant pool type unknown"
    end match
end macro

macro _replace_bracket_operator? result?*, line?*&
    local wrap_item, stream, empty, link, operator, _shl,_shr,_gt,_ge,_lt,_le
    result   equ
    empty    equ
    operator equ
    _shl equ <<
    _shr equ >>
    _gt  equ  >
    _ge  equ >=
    _lt  equ  <
    _le  equ <=
    define stream result
    irps item[+space], line empty ; when '<' or '>' reach the end
        wrap_item equ item
        match =result?, stream
            match =<, item 
                define stream operator
            else match =>, item
                define stream operator
            end match
        else
            match =< =>, operator wrap_item
                wrap_item equ gt
            else match => =<, operator wrap_item
                wrap_item equ lt
            end match
            match not_empty _, item wrap_item
                irps lookahead, <=>
                    match =_, lookahead
                        _insert_token operator, _
                        wrap_item equ
                        break
                    end match
                end irps
            end match
            match bracket, operator
                iterate kind, shl,shr,gt,ge,lt,le
                    eval "link equ _", (`kind)
                    match =bracket, link
                        _insert_token result, ; ensure correct spacing
                        wrap_item equ kind wrap_item
                        break
                    end match
                end iterate
            end match
            operator equ
            define stream result
        end match
        match _, stream
            _insert_token _, wrap_item
        end match
    end irps
end macro

; TODO: HANDLE MORE LITERAL (BINARY, OCTAL, AND MAYBE GAMEBOY GRAPHICS)
macro _subround_string? result?*, line?*&
    local lbrace, follow, identifier, prev_space
    result equ
    lbrace = $00
    identifier equ
    prev_space = $00
    irps item[+space], line
        match ={, item
            lbrace = $01
            follow = $01
        else match =}, item
            if (lbrace)
                if (prev_space)
                    err "syntax error: a space can't follow '}'"
                end if
                match newstr, {identifier}
                    _insert_token result, (`newstr)
                end match
                lbrace = $00
                identifier equ
            else
                _insert_token result, item
            end if
        else if (lbrace)
            prev_space = $00
            match,item
                if (follow)
                    err "syntax erorr: a space can't follow '{'"
                end if
                prev_space = $01
            end match
            match _, identifier
                err "syntax error: bad identifier"
            end match
            _ensure_identifier (`item)
            identifier equ item
            follow = $00
        else
            _insert_token result, item
        end if
    end irps
end macro

macro _eval_each_string? result?*, line?*&
    local check, stream, identifier, new_string, pool_entry, add_entry, end_item, wrap_item
    result equ
    identifier equ
    define stream result
    end_item = $00
    irps item[+space], line
        wrap_item equ item
        _check_identifier check, (`item)
        if (check) | ((`item) = '.')
            define stream identifier
            if (% = %%)
                _insert_token identifier, item
                define stream result
                end_item = $01
            end if
        else
            define stream result
        end if

        match =result?, stream
            add_entry = $00
            new_string equ
            if (((`item) and $FF) = "'") | (((`item) and $FF) = '"')
                _eval_string new_string, item
                _insert_token result, identifier
                add_entry = $01
            else match id, identifier
                match =end?, id
                else match =else?, id
                else
                    if (definite identifier) & (identifier eqtype "")
                        _eval_string new_string, identifier
                        add_entry = $01
                    end if
                end match
                match,new_string
                    wrap_item equ identifier
                end match
            end match
            if (add_entry)
                _add_constant_pool pool_entry, new_string
                wrap_item equ pool_entry
            end if
            if (~(end_item))
                match _, identifier
                    _insert_token wrap_item, item
                end match
            end if
            identifier equ
        end match
        match _, stream
            _insert_token _, wrap_item
        end match
    end irps
end macro

macro _expand_macro_parameter? result?*, line?*&
    if (~(_builder._in_macro)) | (~(_builder._in_rept))
        err ""
    end if
    ; expand \1 - \9 and \@
    ; TODO
    err "not yet implemented"
end macro

macro _expand_single_line_macro? result?*, line?*&
    local return, lparen, global, insert, stream, partial, function, callargs, new_string, pool_entry
    result   equ
    function equ
    lparen = $00
    global = $01
    irps item[+space], line
        define stream result
        insert = $01
        match _, function
            match =(, item
                insert = $00
                lparen = lparen + $01
            else match =), item
                insert = $00
                if (lparen <= $00)
                    err "syntax error: single-line macro parenthesis not properly nested"
                end if
                lparen = lparen - $01
                match list, callargs
                    _ list, return
                else
                    _ return
                end match
                restore function
                restore callargs
                define partial callargs ; where the output of func () must go: in the result or callargs
                match,function
                    define partial result
                end match
                match insret, partial
                    if (return eqtype $00)
                        repeat $01, i:(return)
                            _insert_token insret, i
                        end repeat
                    else
                        if (return eqtype "")
                            _eval_string new_string, return
                            _add_constant_pool pool_entry, new_string
                        else
                            _add_constant_pool pool_entry, return
                        end if
                        _insert_token insret, pool_entry
                    end if
                end match
            else
                if (~(lparen))
                    match not_space, item
                        err "syntax error: single line macro must be followed by '('"
                    end match
                end if
                define stream callargs
            end match
        end match
        if (global) ; func not preceded by .
            iterate <func,pred>, STRLEN,strlen, STRCAT,strcat, STRCMP,strcmp,  STRIN, strin,\
                                 STRRIN,strrin, STRSUB,strsub, STRUPR,strupr, STRLWR,strlwr,\
                                 STRREP,strrep
                match =item?, func
                    function equ pred
                    callargs equ 
                    insert = $00
                    break
                end match
            end iterate
        end if
        global = $01
        if (insert)
            if ((`item) = '.')
                global = $00
            end if
            match _, stream
                _insert_token _, item
            end match
        end if
    end irps
    ; TODO WHEN LAST TOKEN IS A FUNC
    if (lparen > $00); | (findfunc)
        err "syntax error: single-line macro parenthesis not properly nested"
    end if
end macro

macro _preprocess_line? result?*, line?*&
    result equ line
    iterate invoke, _replace_bracket_operator, _subround_string,\
                    _eval_each_string, _expand_single_line_macro
        match _, result
            invoke result, _
        end match
    end iterate
end macro

macro _catch_line?!
    macro ?! line&
        local expand_line
        match not_empty, line
            match macro_name =: =macro?, line
                err "not implemented yet"
            else
                _preprocess_line expand_line, line
                match _, expand_line
                    ; _catch_symbols
                    ;invoker
                    ;macro invoker?!
                        ;invoker
                        ;purge invoker?
                        display "NEW LINE = ", (`_), $0A
                        _
                    ;end macro
                    ; end _catch_symbols
                end match
            end match
        end match
        ;invoker
    end macro
end macro

macro end?._catch_line?!
    purge ?
end macro

