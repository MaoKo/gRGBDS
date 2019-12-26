
_constant_pool? := $00
namespace _constant_pool?
    iterate kind, string,float
        _constant_pool?._#kind? := $00
        namespace _#kind?
            _index = $00
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
        repeat (_constant_pool._#kind._index), i:($00)
            if (value = _constant_pool._#kind.i)
                found equ kind.i
                break
            end if
        end repeat
        match,found
            repeat ($01), i:(_constant_pool._#kind._index)
                _constant_pool._#kind.i = value
                found equ kind.i
            end repeat
            _constant_pool._#kind._index = _constant_pool._#kind._index + $01
        end match
        _insert_token result, found
    else
        err "syntax error: constant pool type unknown"
    end match
end macro

_symbols_operator? := $00
namespace _symbols_operator?
    _one_symbols_operator? := $00
    _two_symbols_operator? := $00
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
                                    "!", __unary_not,,      \
                                    "`", __string,
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
    local transform, continue, multi, unary, token
    multi = $00
    transform = ""
    unary = $01
    redefine .
    irps item, line
        redefine token item
        continue = $01
        namespace _symbols_operator?
            repeat $01, symbol_1:(`item)
                if (multi)
                    rawmatch _, item
                        repeat $01, symbol_2:(multi or (symbol_1 shl $08))
                            if (definite _two_symbols_operator?.symbol_2?)
                                eval "redefine token ", _two_symbols_operator.symbol_2
                                token _insert_token ; ** insert empty ** ;
                                continue = $00
                            end if
                        end repeat
                    end rawmatch
                    if (continue)
                        eval ". _insert_token ", transform
                        . _insert_token ; ** insert empty ** ;
                    end if
                    multi = $00
                end if
                if (continue) &\
                        ((definite _one_symbols_operator?.symbol_1?) |\
                        (definite  _two_symbols_operator?.symbol_1?))

                    transform = (`item)
                    if (definite _one_symbols_operator?.symbol_1?)
                        transform = _one_symbols_operator.symbol_1
                        if (unary) & (definite _one_symbols_operator?._unary_symbols?.symbol_1?)
                            transform = _one_symbols_operator._unary_symbols.symbol_1
                        end if
                    end if

                    if (definite _two_symbols_operator?.symbol_1?)
                        multi = (`item)
                    else if (definite _one_symbols_operator?.symbol_1?)
                        eval "redefine token ", transform
                        token _insert_token ; ** insert empty ** ;
                    end if
                    . _insert_token ; ** insert empty ** ;
                    unary = $01
                else rawmatch =(, item 
                    unary = $01
                else
                    unary = $00
                end rawmatch
            end repeat
        end namespace
        if (~(multi))
            . _insert_token token
        end if
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

_single_line_macro? := $00
namespace _single_line_macro?
end namespace

; Single-Line macro is defined as case-insensitive symbol.
; Single-Line macro act like C-function. Parameter must have a defined type.
macro _add_single_line_macro? list?*&
    namespace _single_line_macro?
        iterate <func*,pred*>, list
            _symbols? equ func,pred
        end iterate
    end namespace
end macro

_add_single_line_macro STRLEN,strlen, STRCAT,strcat, STRCMP,strcmp,  STRIN, strin,\
                       STRRIN,strrin, STRSUB,strsub, STRUPR,strupr, STRLWR,strlwr,\
                       STRREP,strrep

struc _expand_single_line_macro? line?*&
    local function, callargs, computed, matched, strip, return, stream, cparen
    redefine function
    redefine stream .
    matched = $00
    redefine .
    irps item, line
        strip = $00
        rawmatch _, item
            match pred, function
                rawmatch =(, item
                    cparen = cparen + $01
                    if (matched)
                        strip = $01
                    end if
                else rawmatch =), item
                    cparen = cparen - $01
                    if (~(cparen))
                        match list, callargs
                            computed _rpn_compute list
                        end match
                        match list, computed
                            return pred list
                        end match
                        restore function, callargs, stream, cparen
                        match insert, stream
                            if (return eqtype $00)
                                repeat $01, i:(return)
                                    insert _insert_token i
                                end repeat
                            else
                                if (return eqtype "")
                                    _eval_string new_string, return
                                    _add_constant_pool pool_entry, new_string
                                else if (return eqtype 0.0f)
                                    _add_constant_pool pool_entry, return
                                else
                                    err "unknwown type"
                                end if
                                insert _insert_token pool_entry
                            end if
                        end match
                        strip = $01
                    else if (cparen < $00)
                        error "_expand_single_line_macro",\
                            <"syntax error: parenthesis not properly balanced.">
                    end if
                end rawmatch
                if (matched) & (~(cparen))
                    error "_expand_single_line_macro",\
                            <"syntax error: left parenthesis '(' not follow the function call.">
                end if
                matched = $00
            end match
            namespace _single_line_macro?
                irpv pairs, _symbols
                    match func =, pred, pairs
                        rawmatch =item?, func
                            define function pred
                            define callargs
                            define stream callargs
                            cparen =: $00
                            matched = $01
                            break
                        end rawmatch
                    end match
                end irpv
            end namespace
        end rawmatch
        if (~(matched)) & (~(strip))
            match _, stream
                _ _insert_token item
            end match
        end if
    end irps
    match pred, function
        error "_expand_single_line_macro", <"syntax error: missing arguments to the function '", (`pred), "'.">
    end match
end struc

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

