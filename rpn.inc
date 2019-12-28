
_rpn_symbols? := $00
namespace _rpn_symbols?
    _LASSOC? := $00
    _RASSOC? := $01
    _NASSOC? := $02
    iterate <oprt,precd,assoc>,   __unary_neg,$00,_RASSOC,  __unary_add,$00,_RASSOC, __unary_sub,$00,_RASSOC,\
                                        __mul,$01,_LASSOC,        __div,$01,_LASSOC,       __mod,$01,_LASSOC,\
                                        __shl,$02,_LASSOC,        __shr,$02,_LASSOC,\
                                        __and,$03,_LASSOC,         __or,$03,_LASSOC,       __xor,$03,_LASSOC,\
                                        __add,$04,_LASSOC,        __sub,$04,_LASSOC,\
                                         __ne,$05,_LASSOC,         __eq,$05,_LASSOC,        __le,$05,_LASSOC,\
                                         __ge,$05,_LASSOC,         __lt,$05,_LASSOC,        __gt,$05,_LASSOC,\
                                __logical_and,$06,_LASSOC, __logical_or,$06,_LASSOC,\
                                  __unary_not,$07,_RASSOC,\
                                     __string,$08,_RASSOC
        repeat $01, i:(`oprt)
            ?i? := $00
            namespace ?i?
                _precedence?    := precd
                _associativity? := assoc
            end namespace
        end repeat
    end iterate
    macro enqueue? queue?*, operator?*
        local replace
        redefine replace operator
        rawmatch =__unary_add?, operator ; __unary_add get vanish during this step
            redefine replace $00 __add
        end rawmatch
        queue _insert_token replace _insert_token.EMPTY
    end macro
end namespace

; Shunting-Yard Algorithm
struc _rpn_builder? polynomial?, expression?*
    local stack, present, identifier
    rawmatch _, polynomial
        polynomial = $00
    end rawmatch
    redefine stack
    redefine .
    irps item, ( expression ) ; avoid the need to pop-off all the stack operator
        rawmatch _, item
            rawmatch =(, item
                define stack (
            else rawmatch =), item
                present = $00
                irpv pushed, stack
                    reverse
                    restore stack
                    rawmatch =pushed, (
                        present = $01
                        break
                    end rawmatch
                    eval "_rpn_symbols.enqueue ., ", string pushed
                end irpv
                if (~(present))
                    error "_rpn_builder", "syntax error: missing left parenthesis."
                end if
            else
                repeat $01, i:(`item)
                    if (definite _rpn_symbols.i)
                        irpv pushed, stack
                            reverse
                            rawmatch =(, pushed
                                break
                            else
                                if (_rpn_symbols.pushed._precedence > _rpn_symbols.i._precedence)
                                    break
                                else if (_rpn_symbols.pushed._precedence = _rpn_symbols.i._precedence)
                                    if (_rpn_symbols.pushed._associativity = _rpn_symbols._NASSOC)
                                        error "_rpn_builder", <"syntax error: operator '",\
                                                            (`operator), "' is not-associative.">
                                    else if (_rpn_symbols.pushed._associativity = _rpn_symbols._LASSOC)
                                        break
                                    end if
                                end if
                            end rawmatch
                            restore stack
                            eval "_rpn_symbols.enqueue ., ", string pushed
                        end irpv
                        define stack i
                    else
                        rawmatch,polynomial
                        else
                            identifier _fasmg_identifier item
                            if (identifier = __FASMG_IDENTIFIER)
                                if (definite item) & (elementsof (item))
                                    polynomial = $01
                                end if
                            end if
                        end rawmatch
                        . _insert_token item _insert_token.EMPTY
                    end if
                end repeat
            end rawmatch
        end rawmatch
    end irps
    match _, stack
        error "_rpn_builder", "syntax error: stack not empty."
    end match
end struc

struc _rpn_evaluator? expression?*
    local stack, relational, relational_equ, operand_1, operand_2
    redefine stack
    redefine .
    irps item, expression
        rawmatch _, item
            repeat $01, i:(`item)
                if (definite _rpn_symbols.i)
                    match,stack
                        error "_rpn_evaluator", <"missing operand for operator: ", (`item), ".">
                    end match
                    operand_2 = stack
                    restore stack
                    rawmatch =__unary_neg, item
                        . = ((not operand_2) + $01)
                    else rawmatch =__unary_sub, item
                        . = (- operand_2)
                    else rawmatch =__unary_not, item
                        . = (not operand_2)
                    else rawmatch =__string, item
                        . = (string operand_2)
                    else
                        relational = $01
                        match,stack
                            error "_rpn_evaluator", <"missing operand for operator: ", (`item), ".">
                        end match
                        operand_1  = stack
                        restore stack
                        iterate <operator,builtin>,  __mul,  *, __div,  /, __mod,mod,\
                                                     __shl,shl, __shr,shr,\
                                                     __and,and,  __or, or, __xor,xor,\
                                                     __add,  +, __sub,  -
                            rawmatch =operator?, item
                                . = (operand_1) builtin (operand_2)
                                relational = $00
                                break
                            end rawmatch
                        end iterate
                        if (relational)
                            . = $00
                            iterate <operator,builtin>, __logical_and,"&", __logical_or, "|",\
                                                                 __gt,">",         __ge,">=",\
                                                                 __lt,"<",         __le,"<=",\
                                                                 __eq,"=",         __ne,"<>"
                                eval "relational_equ reequ ", builtin
                                match =operator? _, item relational_equ
                                    if ((operand_1) _ (operand_2))
                                        . = $01
                                    end if
                                end match
                            end iterate
                        end if
                    end rawmatch
                    irpv computed, .
                        define stack computed
                    end irpv
                else
                    define stack item
                end if
            end repeat
        end rawmatch
    end irps
    irpv _, stack
        if ((%%) > $02)
            error "_rpn_evaluator", <"invalid expression: `", (`expression), "`.">
            break
        end if
    end irpv 
    . = stack
end struc

_rpn_cache? := $00
struc _rpn_single_pass? postfix?, polynomial?, expression?*
    local build, variable
    match _, expression
        repeat $01, i:(`_)
            if (definite _rpn_cache.i)
                iterate <j,k>    postfix,_p#ostfix,\
                              polynomial,_p#olynomial
                    rawmatch _1, j
                        j reequ _rpn_cache.i.k
                    end rawmatch
                end iterate
                . = _rpn_cache.i
            else
                build _rpn_builder variable, expression
                . _rpn_evaluator build
                define  _rpn_cache.i?
                restore _rpn_cache.i?
                _rpn_cache.i? = .
                namespace _rpn_cache?.i?
                    _postfix?       reequ build
                    _polynomial?    reequ variable
                end namespace
            end if
        end repeat
    end match
end struc

struc _rpn_compute expressions?*&
    ; TODO: Add cached rpn expression ;
    local rpn_build, polynomial, rpn_evaluate
    redefine .
    iterate expression*, expressions
        rpn_build _rpn_builder polynomial, expression
        rpn_evaluate _rpn_evaluator rpn_build
        irpv computed, rpn_evaluate
            match _, .
                . reequ _, computed
            else
                . reequ    computed
            end match
        end irpv
    end iterate
end struc

