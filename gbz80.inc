
_cpu_gbz80? := $00
namespace _cpu_gbz80?
    element _register_type_r08?
    ;element _register_type_r16? TODO
    iterate <type,value>, A,$07, B,$00, C,$01, D,$02, E,$03, H,$04, L,$05, _Mem_HL,$06
        element type : _register_type_r08 + value
    end iterate
end namespace

macro nop?
    db 00000000b
end macro

macro stop?
    db 0001000b
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
