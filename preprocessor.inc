
_constant_pool? := $00
namespace _constant_pool?
    iterate kind, string,float
        _constant_pool?._#kind? := $00
        namespace _#kind?
            __index = $00
        end namespace
    end iterate
end namespace

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

_symbols_operator? := $00
namespace _symbols_operator?
    __MODULUS equ % ; ensure to not expand even when in loop
    _one_symbols_operator? := $00
    namespace _one_symbols_operator? 
        _unary_symbols? := $00
        iterate <sop*,sub*,unary>,  "+", __add, __unary_add,\
                                    "-", __sub, __unary_sub,\
                                    "*", __mul,,            \
                                    "/", __div,,            \
                                    "^", __xor,,            \
                                    "~", __unary_neg,,      \
                                    "%", __mod,,            \
                                    "<",  __lt,,            \
                                    ">",  __gt,,            \
                                    "|",  __or,,            \
                                    "&", __and,,            \
                                    "!", __unary_not,
            repeat $01, i:(sop)
                ?i? = (`sub)
                rawmatch _, unary
                    namespace _unary_symbols?
                        ?i? = (`unary)
                    end namespace
                end rawmatch
            end repeat
        end iterate
    end namespace
    _two_symbols_operator? := $00
    namespace _two_symbols_operator?
        iterate begin, "<",">","|","&","=","!"
            repeat $01, i:begin
                ?i? = $01
            end repeat
        end iterate
        iterate <sop,sub>,          "<<",         __shl,\
                                    ">>",         __shr,\
                                    "||",  __logical_or,\
                                    "&&", __logical_and,\
                                    "<=",          __le,\
                                    ">=",          __ge,\
                                    "==",          __eq,\
                                    "!=",          __ne
            repeat $01, i:(sop)
                ?i? = (`sub)
            end repeat
        end iterate
    end namespace
end namespace

struc _replace_symbols_operator? line?*&
    ;local wrap_item, stream, empty, operator, operand, unary, complex_equ, part_1_equ, part_2_equ, operator_equ
    local transform, multi, unary, token
    multi = $00
    transform = ""
    unary = $01
    redefine .
    irps item, line
        redefine token item
        namespace _symbols_operator?
            repeat $01, symbol_1:(`item)
                if (multi)
                    repeat $01, symbol_2:(multi or (symbol_1 shl $08))
                        if (definite _two_symbols_operator?.symbol_2?)
                            eval "redefine token ", _two_symbols_operator.symbol_2
                            token _insert_token ; ** insert empty ** ;
                        else
                            eval "redefine token ", transform
                            token _insert_token ; ** insert empty ** ;
                        end if
                    end repeat
                    multi = $00
                else
                    if (definite _one_symbols_operator?.symbol_1?) | (definite _two_symbols_operator?.symbol_1?)
                        transform = (`item)
                        if (definite _one_symbols_operator?.symbol_1?)
                            transform = _one_symbols_operator.symbol_1
                            if (unary) & (definite _one_symbols_operator?._unary_symbols?.symbol_1?)
                                transform = _one_symbols_operator._unary_symbols.symbol_1
                            end if
                        end if

                        if (definite _two_symbols_operator?.symbol_1?)
                            multi = symbol_1
                        else if (definite _one_symbols_operator?.symbol_1?)
                            eval "redefine token ", transform
                            token _insert_token ; ** insert empty ** ;
                        end if
                        . _insert_token ; ** insert empty **
                        unary = $01
                    else rawmatch =(, item 
                        unary = $01
                    else rawmatch =), item
                        unary = $01
                    else
                        unary = $00
                    end rawmatch
                end if
            end repeat
        end namespace
        if (~(multi))
            . _insert_token token
        end if
        ;match mod_token not_empty, _MODULUS wrap_item
        ;end match
;        match _, stream
;            _insert_token _, wrap_item
;        end match
    end irps
    if (multi)
        eval ". _insert_token ", transform
    end if
end struc

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

