
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
                                  __unary_not,$07,_RASSOC
                                
        oprt? := $00
        namespace oprt?
            _precedence?    := precd
            _associativity? := assoc
        end namespace
        define _list_operator? oprt
    end iterate
    macro enqueue? queue?*, operator?*
        rawmatch =__unary_add?, operator ; __unary_add get vanish during this step
            queue _insert_token $00 __add
        else
            queue _insert_token operator
        end rawmatch
        queue _insert_token ; ** insert empty ** ;
    end macro
end namespace

; Shunting-Yard Algorithm
struc _rpn_builder? polynomial?, expression?*&
    local stack, present, empty
    rawmatch _, polynomial
        polynomial = $00
    end rawmatch
    redefine empty
    redefine .
    irps item, ( expression ) ; avoid the need to pop-off all the stack operator
        rawmatch _, item
            rawmatch =(, item
                define stack (
            else rawmatch =), item
                present = $00
                irpv operator, stack
                    indx (((%%) - (%)) + $01) ; reverse ordering
                    restore stack
                    rawmatch =operator, (
                        present = $01
                        break
                    end rawmatch
                    _rpn_symbols.enqueue ., operator
                end irpv
                if (~(present))
                    err "syntax error: missing left parenthesis"
                end if
            else
                if (definite _rpn_symbols.item)
                    irpv operator, stack
                        indx (((%%) - (%)) + $01) ; reverse ordering
                        rawmatch =(, operator
                            break
                        else
                            if (_rpn_symbols.operator._precedence > _rpn_symbols.item._precedence)
                                break
                            else if (_rpn_symbols.operator._precedence = _rpn_symbols.item._precedence)
                                if (_rpn_symbols.operator._associativity = NASSOC)
                                    err "syntax error: operator '", (`operator), "' is not-associative"
                                else if (_rpn_symbols.operator._associativity = LASSOC))
                                    break
                                end if
                            end if
                        end rawmatch
                        restore stack
                        _rpn_symbols.enqueue ., operator
                    end irpv
                    define stack item
                else
                    if (elementsof (item))
                        rawmatch _, polynomial
                            polynomial = $01
                        end rawmatch
                    end if
                    . _insert_token item empty
                end if
            end rawmatch
        end rawmatch
    end irps
end struc

struc _rpn_evaluator? expression?*
    local stack, stack_defined, relational, relational_equ, operand_1, operand_2
    stack_defined = $00
    redefine .
    irps item, expression
        rawmatch _, item
            if (defined _rpn_symbols.item)
                if (~(stack_defined))
                    err "_rpn_evaluator: invalid expression `", (`expression), "`"
                end if
                operand_2 = stack ; FIFO ordering
                restore stack
                match =__unary_neg, item
                    result = ((not operand_2) + $01)
;               else match =__unary_add, item
;                   result = (+ operand_2)
                else match =__unary_sub, item
                    result = (- operand_2)
                else match =__unary_not, item
                    result = (not operand_2)
                else
                    relational = $01
                    operand_1  = stack
                    restore stack
                    iterate <operator,builtin>,  __mul,  *, __div,  /, __mod,mod,\
                                                 __shl,shl, __shr,shr,\
                                                 __and,and,  __or, or, __xor,xor,\
                                                 __add,  +, __sub,  -
                        match =operator?, item
                            . = (operand_1) builtin (operand_2)
                            relational = $00
                            break
                        end match
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
                end match
                irpv computed, .
                    define stack computed
                end irpv
            else
                stack_defined = $01
                define stack item
            end if
        end rawmatch
    end irps
    . = stack
end struc
