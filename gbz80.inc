
_cpu_gbz80? := $00
namespace _cpu_gbz80?
    _NULL_PLACEHOLDER := -$01
    _INCREMENT := 0b
    _DECREMENT := 1b
    element _register_r08?
    iterate <type*,value*>, A,$07, B,$00, C,$01, D,$02, E,$03, H,$04, L,$05, _Memory_HL,$06
        element type? : _register_r08 + value
    end iterate
    element _register_r16?
    repeat $03, i:($01)
        element _placeholder_#i?
    end repeat
    iterate <type*,p1*,p2*,p3*>, BC,               0b,              00b,              00b,\
                                 DE,               1b,              01b,              01b,\
                                 HL,_NULL_PLACEHOLDER,              10b,              10b,\
                                 SP,_NULL_PLACEHOLDER,              11b,_NULL_PLACEHOLDER,\
                                 AF,_NULL_PLACEHOLDER,_NULL_PLACEHOLDER,              11b,\
                                 PC,_NULL_PLACEHOLDER,_NULL_PLACEHOLDER,_NULL_PLACEHOLDER
        element type? : _register_r16 + $00
        element _#type#_proxy? : (_placeholder_1 * p1) +\
                                 (_placeholder_2 * p2) +\
                                 (_placeholder_3 * p3)
    end iterate
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

iterate <type*,action*>, push,_DECREMENT, pop,_INCREMENT
    macro type? r16?*
        local target, proxy
        match reg, r16
            target = _cpu_gbz80.reg
            proxy  = _cpu_gbz80._#reg#_proxy
        end match
        _assert_element _cpu_gbz80._register_r16, target
        proxy = ((proxy metadata $01) scale $03)
        if (proxy = _cpu_gbz80._NULL_PLACEHOLDER)
            error (`type), "instruction error: register 16-bit non valid."
        end if
        db ((11b shl $06) or (proxy shl $04) or (_cpu_gbz80.action shl $02) or (01b))
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
