
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
        define _list_operator oprt
    end iterate
end namespace

; Shunting-Yard Algorithm
macro _rpn_builder? result?*, polynomial?*, expression?*&
    local stack, present, empty
    result equ
    empty  equ
    polynomial = $00
    macro enqueue_operator? operator?*
        match =__unary_add?, operator ; __unary_add get vanish during this step
            _insert_token result, $00 __add empty
        else
            _insert_token result, operator empty
        end match
    end macro
    irps item[+ident], ( expression ) ; avoid the need to pop-off all the stack operator
;        match =(, item
;            stack equ (
;        else match =), item
;            present = $01
;            irpv operator, stack
;                reverse
;                restore stack
;                match =operator, (
;                    present = $00
;                    break
;                end match
;                enqueue_operator operator
;            end irpv
;            if (present)
;                err "syntax error: missing left parenthesis"
;            end if
;        else
;            if (definite _rpn_symbols.item)
;                irpv operator, stack
;                    reverse
;                    match =(, operator
;                        break
;                    else
;                        if (_rpn_symbols.operator._precedence > _rpn_symbols.item._precedence)
;                            break
;                        else if (_rpn_symbols.operator._precedence = _rpn_symbols.item._precedence)
;                            if (_rpn_symbols.operator._associativity = NASSOC)
;                                err "syntax error: operator '", `operator, "' is not-associative"
;                            else if (_rpn_symbols.operator._associativity = LASSOC))
;                                break
;                            end if
;                        end if
;                    end match
;                    restore stack
;                    enqueue_operator operator
 ;               end irpv
 ;               stack equ item
 ;           else
 ;;               if (elementsof (item))
 ;                   polynomial = $01
 ;               end if
 ;               _insert_token result, item empty
 ;           end if
 ;       end match
    end irps
    purge enqueue_operator?
end macro

macro _rpn_evaluator? result?*, expression?*
    local stack, relational, relational_equ, operand_1, operand_2
    irps item[+ident], expression
        if (defined _rpn_symbols.item)
            operand_1 = stack
            restore stack
            match =__unary_neg, item
                result = ((not operand_1) + $01)
            else match =__unary_add, item
                result = (+ operand_1)
            else match =__unary_sub, item
                result = (- operand_1)
            else match =__unary_not, item
                result = (not operand_1)
            else
                relational = $01
                operand_2  = stack
                restore stack
                iterate <operator,builtin>,  __mul,*,   __div,/,  __mod,mod,\
                                             __shl,shl, __shr,shr,\
                                             __and,and,  __or,or, __xor,xor,\
                                             __add,+,   __sub,-
                    match =operator?, item
                        result = (operand_1) builtin (operand_2)
                        relational = $00
                        break
                    end match
                end iterate
                if (relational)
                    result = $00
                    iterate <operator,builtin>, __gt,">", __ge,">=", __lt,"<", __le,"<=",\
                                                __eq,"=", __ne,"<>", __logical_and,"&", __logical_or,"|"
                        eval "relational_equ equ ", builtin
                        match =operator? _, item relational_equ
                            if ((operand_1) _ (operand_2))
                                result = $01
                            end if
                        end match
                    end iterate
                end if
            end match
            irpv value, result
                stack equ value
            end irpv
        else
            stack equ item
        end if
    end irps
    result = stack
end macro
