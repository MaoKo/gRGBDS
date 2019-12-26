
_cpu_gbz80? := $00
namespace _cpu_gbz80?
    element _register_r08?
    element _register_r16?
    iterate <type*,value*>, A,$07, B,$00, C,$01, D,$02, E,$03, H,$04, L,$05, _Mem_HL,$06
        element type? : _register_r08 + value
    end iterate
end namespace

macro _bound_limit? value?*, bitness?*, type?
    local slimit, ulimit
    _assert_numeric (value), (bitness)
    if (elementsof (value))
        error "_bound_limit", "invalid operand: linear polynomial cannot be part of immediate."
    end if
    ulimit := ($01 shl bitness)
    slimit := ($01 shl (bitness - $01))
    match _, type
        rawmatch =SIGNED?, _
            if (((value) >= slimit) | ((value) < (-slimit)))
                error "_bound_limit", "invalid operand: immediate value not fit in signed range."
            end if
        else rawmatch =UNSIGNED?, _
            if (((value) < $00) | ((value) >= ulimit))
                error "_bound_limit", "invalid operand: immediate value not fit in unsigned range."
            end if
        else
            error "_bound_limit", "invalid operand: operand must be either SIGNED or UNSIGNED."
        end rawmatch
    else
        if (((value) < (-_slimit)) | ((value) >= ulimit))
            error "_bound_limit", "invalid operand: immediate value not fit properly."
        end if
    end match
end macro

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
