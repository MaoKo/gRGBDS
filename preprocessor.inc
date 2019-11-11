
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

__MODULUS equ % ; ensure to not expand even when in loop
macro _replace_symbols_operator? result?*, line?*&
    local wrap_item, stream, empty, operator, operand, unary, complex_equ, part_1_equ, part_2_equ, operator_equ
    result   equ
    operator equ
    empty    equ
    unary  = $01
    define stream result
    irps item[+space|+ident], line empty ; when '<' or '>' reach the end
        wrap_item equ item
        match =operator?, stream ; operator of more than 1 token
            match _1 _2, operator item ; ensure item is empty
                iterate <part_1,part_2>, "|","|", "&","&", "=","=", "!","=",\
                                         "<","<", "<","=", ">",">", ">","="
                    eval "part_1_equ equ ", part_1
                    eval "part_2_equ equ ", part_2
                    match =_1 =_2, part_1_equ part_2_equ
                        _insert_token operator, =_2
                        wrap_item equ
                        break
                    end match
                end iterate
            end match
            match _, operator
                iterate <kind,shape>,   __or,"|",  __logical_or,"||",\
                                       __and,"&", __logical_and,"&&",\
                                       __eq,"==",          __ne,"!=",\
                                      __shl,"<<",         __shr,">>",\
                                        __gt,">",          __ge,">=",\
                                        __lt,"<",          __le,"<=",\
                                 __unary_not,"!"
                    eval "operator_equ equ ", shape
                    match =_, operator_equ
                        _insert_token result, ; ensure correct spacing
                        _insert_token result, kind empty
                        break
                    end match
                    operator_equ equ
                end iterate
            end match
            match,operator_equ
                _insert_token result, operator
            end match
            operator equ
            define stream result
        end match
        match not_empty, wrap_item
            operand = $00
            iterate complex_operator, "<",">","|","&","=","!"
                eval "complex_equ equ ", complex_operator
                match =item, complex_equ
                    define stream operator
                    break
                end match
                complex_equ equ
            end iterate
            match,complex_equ
                match =+, item
                    wrap_item equ __add
                    if (unary)
                        wrap_item equ __unary_add
                    end if
                else match =-, item
                    wrap_item equ __sub
                    if (unary)
                        wrap_item equ __unary_sub
                    end if
                else match =*, item
                    wrap_item equ __mul
                else match =/, item
                    wrap_item equ __div
                else match =^, item
                    wrap_item equ __xor
                else match =~, item
                    wrap_item equ __unary_neg
                else match =(, item ; preserve unary
                else match =), item ; preserve unary
                else
                    match mod_token, __MODULUS
                        match =mod_token, item
                            wrap_item equ __mod
                        else
                            operand = $01
                            unary = $00
                        end match
                    end match
                end match
            end match
            if (~(operand))
                unary = $01
            end if
            match _, wrap_item
                if ((`_) <> (`item)) ; add space around replaced operator
                    _insert_token result,
                    _insert_token wrap_item,
                end if
            end match
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
    local check, new_string, pool_entry, add_entry, wrap_item
    result equ
    irps item[+space|+ident], line
        wrap_item equ item
        _fasmg_identifier check, item
        if (check) | (((`item) and $FF) = "'") | (((`item) and $FF) = '"')
            add_entry = $00
            if (((`item) and $FF) = "'") | (((`item) and $FF) = '"')
                _eval_string new_string, item
                add_entry = $01
            else
                match =end?, item
                else match =else?, item
                else
                    if (definite item) & (item eqtype "")
                        _eval_string new_string, item
                        add_entry = $01
                    end if
                end match
            end if
            if (add_entry)
                _add_constant_pool pool_entry, new_string
                wrap_item equ pool_entry
            end if
        end if
        _insert_token result, wrap_item
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
    local return, empty, lparen, global, insert, stream, partial_ret, function, callargs, new_string, pool_entry
    result   equ
    empty    equ
    function equ
    lparen = $00
    global = $01
    irps item[+space], line empty
        define stream result
        insert = $01
        match _, function
            match =(, item
                insert = $00
                lparen = lparen + $01
                define stream callargs
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
                define partial_ret callargs ; where the output of func() must go: in the result or another callargs
                match,function
                    define partial_ret result
                end match
                match insret, partial_ret
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
            else if (~(lparen))
                match not_space, item
                    err "syntax error: single line macro must be followed by '('"
                end match
            end if
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
    if (lparen > $00) | (~(insert))
        err "syntax error: single-line macro parenthesis not properly nested"
    end if
end macro

macro _preprocess_line? result?*, line?*&
    result equ line
    iterate invoke, _replace_symbols_operator, _subround_string,\
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

