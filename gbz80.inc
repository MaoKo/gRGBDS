
_cpu_gbz80? := $00
namespace _cpu_gbz80?
    element _register_type_r08?
    ;element _register_type_r16? TODO
    iterate <type,value>, A,$07, B,$00, C,$01, D,$02, E,$03, H,$04, L,$05, _Mem_HL,$06
        element type : _register_type_r08 + value
    end iterate
end namespace

macro _bound_limit? value?*, bitness?*, kind?
    local slimit, ulimit
    _assert_numeric (value)
    if (elementsof (value))
        err "invalid operand: linear polynomial cannot be part of immediate"
    end if
    ulimit := ($01 shl bitness)
    slimit := ($01 shl (bitness - $01))
    match =SIGNED?, kind
        if (((value) >= slimit) | ((value) < (-slimit)))
            err "invalid operand: immediate value not fit in signed range"
        end if
    else match =UNSIGNED?, kind
        if (((value) < $00) | ((value) >= _ulimit))
            err "invalid operand: immediate value not fit in unsigned range"
        end if
    else match,kind
        if (((value) < (-_slimit)) | ((value) >= _ulimit))
            err "invalid operand: immediate value not fit properly" ; change error message later
        end if
    else
        err "invalid operand: operand must be either SIGNED or UNSIGNED"
    end match
end macro

macro nop?
    db 00000000b
end macro

macro stop?
    db 00001000b
end macro

macro daa?
    db 00100111b
end macro

macro cpl?
    db 00101111b
end macro

macro scf?
    db 00110111b
end macro

macro ccf?
    db 00111111b
end macro

macro halt?
    db 01110110b
end macro

macro rst? vector?*
    
end macro

macro rst? vector?*
    local expression, absolute
    _rpn_builder expression, absolute, vector
    if (absolute)
        err "invalid operand: address for RST must be absolute"
    end if
    _rpn_evaluator expression, expression
end macro

macro ret?
    db 11001001b
end macro

macro reti?
    db 11011001b
end macro

macro di?
    db 11110011b
end macro

macro ei?
    db 11111011b
end macro
