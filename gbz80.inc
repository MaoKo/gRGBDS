
_cpu_gbz80? := $00
namespace _cpu_gbz80?
    element _register_r08?
    iterate <type*,value*>, A,$07, B,$00, C,$01, D,$02, E,$03, H,$04, L,$05, _Memory_HL,$06
        element type? : _register_r08 + value
    end iterate
    element _register_r16?
    ;iterate AF,BC,DE,HL
    ;end iterate
end namespace

macro nop?
    db (00000000b)
end macro

macro stop?
    db (00001000b)
end macro

macro daa?
    db (00100111b)
end macro

macro cpl?
    db (00101111b)
end macro

macro scf?
    db (00110111b)
end macro

macro ccf?
    db (00111111b)
end macro

macro halt?
    db (01110110b)
end macro

iterate type*, push,pop
    macro type? r16?*
        _assert_element _register_r16, r16       
    end macro
end iterate

macro rst? vector?*
    local immediate
    immediate _rpn_compute vector
    _assert_numeric (immediate)
    if (elementsof (immediate))
        error "rst", "invalid operand: address for RST must be absolute."
    else if (immediate mod $08)
        immediate _itoa_base immediate, $0A
        error "rst", <"invalid address ", (immediate), " for RST.">
    end if
    immediate = immediate shr $03
    _bound_limit immediate, $03, UNSIGNED
    db ((11b shl $06) or (immediate shl $03) or (111b))
end macro

macro ret?
    db (11001001b)
end macro

macro reti?
    db (11011001b)
end macro

macro di?
    db (11110011b)
end macro

macro ei?
    db (11111011b)
end macro
