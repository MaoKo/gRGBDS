
_rpn_symbols? := $00
namespace _rpn_symbols?
    LASSOC? := $00
    RASSOC? := $01
    NASSOC? := $02
    iterate <oprt,precd,assoc>,   __unary_neg,$00,RASSOC,  __unary_add,$00,RASSOC, __unary_sub,$00,RASSOC,\
                                        __mul,$01,LASSOC,        __div,$01,LASSOC,       __mod,$01,LASSOC,\
                                        __shl,$02,LASSOC,        __shr,$02,LASSOC,\
                                        __and,$03,LASSOC,         __or,$03,LASSOC,       __xor,$03,LASSOC,\
                                        __add,$04,LASSOC,        __sub,$04,LASSOC,\
                                         __ne,$05,LASSOC,         __eq,$05,LASSOC,        __le,$05,LASSOC,\
                                         __ge,$05,LASSOC,         __lt,$05,LASSOC,        __gt,$05,LASSOC,\
                                __logical_and,$06,LASSOC, __logical_or,$06,LASSOC,\
                                  __unary_not,$07,RASSOC
                                
        oprt? := $00
        namespace oprt?
            precedence?    := precd
            associativity? := assoc
        end namespace
        _list_operator equ oprt
    end iterate
end namespace

; Shunting-Yard Algorithm
macro _rpn_generator? result?*, expression?*
    local stack, missing, empty
    result equ
    empty  equ
    irps item[+space|+ident], ( expression ) ; avoid the need to pop-off all the stack 'stack'
        match =(, item
            stack equ (
        else match =), item
            missing = $01
            irpv operator, stack
                reverse
                restore stack
                match =operator, (
                    missing = $00
                    break
                end match
                _insert_token result, operator empty
            end irpv
            if (missing)
                err "syntax error: missing left parenthesis"
            end if
        else match not_empty, item
            missing = $01
            irpv operator, _rpn_symbols._list_operator
                match =operator?, item
                    missing = $00
                    break
                end match
            end irpv
            if (missing)
                _insert_token result, item empty
            else
                irpv operator, stack
                    reverse
                    match =(, operator
                        break
                    else
                        if (_rpn_symbols.operator.precedence > _rpn_symbols.item.precedence)
                            break
                        else if (_rpn_symbols.operator.precedence = _rpn_symbols.item.precedence)
                            if (_rpn_symbols.operator.associativity = NASSOC)
                                err "syntax error: operator '", `operator, "' is not-associative"
                            else if (_rpn_symbols.operator.associativity = LASSOC))
                                break
                            end if
                        end if
                    end match
                    restore stack
                    _insert_token result, operator empty
                end irpv
                stack equ item
            end if
        end match
    end irps
end macro
