
; for full documentation reference see (http://goldencrystal.free.fr/GBZ80Opcodes.pdf)

_cpu_gbz80? := $00
namespace _cpu_gbz80?
    _NULL_PLACEHOLDER := -$01
    _INCREMENT := 0b
    _DECREMENT := 1b
    iterate <action*,instructions*>, _INCREMENT,<inc,ldi, pop>,\
                                     _DECREMENT,<dec,ldd,push>
        iterate instruction, instructions
            _#instruction? := action
        end iterate
    end iterate
    iterate <instruction*,pattern*>, bit,01b, res,10b, set,11b
        _#instruction? := pattern
    end iterate
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
        element type? : _register_r16 + (`type) ; debug purpose
        element type?.proxy? :  (_placeholder_1 * p1) +\
                                (_placeholder_2 * p2) +\
                                (_placeholder_3 * p3)
    end iterate
    _condition? := $00
    namespace _condition?
        ; Not  Zero - NZ
        ;      Zero -  Z
        ; Not Carry - NC
        ;     Carry -  C
        element _condition_type?
        iterate <cc*,value*>, NZ,00b, Z,01b, NC,10b, C,11b
            element cc? : _condition_type + value
        end iterate
    end namespace
    struc retreive_scale? target?*, index?*
        namespace _cpu_gbz80?
            _assert_element,_register_r16, target
            . = ((target.proxy metadata $01) scale index)
            if ((.) = _NULL_PLACEHOLDER)
                error "retreive_scale",\
                            <"instruction error: '", string ((target metadata $01) scale $00),\
                            "' 16-bit register non valid.">
            end if
        end namespace
    end struc
    struc retreive_register? target?*
        namespace _cpu_gbz80?
            . = _Memory_HL
            match register, target
                rawmatch      =[ =HL? =], register
                else rawmatch =( =HL? =), register
                else rawmatch =[ =HL? =), register ; support 'all' syntax
                else rawmatch =( =HL? =], register
                else
                    . = #register ; get rid of the context associated with 'register'
                    if (definite #register.proxy) ; 16-bit register
                        .proxy = #register.proxy
                    end if
                end rawmatch
            end match
        end namespace
    end struc
end namespace

macro nop?
    db (00000000b)
end macro

iterate type*, inc,dec
    macro type? r08_r16?*
        local target, proxy, feedback
        target _cpu_gbz80.retreive_register r08_r16
        _assert_element feedback, _cpu_gbz80._register_r08, target
        if (~(feedback))
            db (((target metadata $01 scale $00) shl $03) or (10b shl $01) or (_cpu_gbz80._#type))
        else
            proxy _cpu_gbz80.retreive_scale target, $02
            db ((proxy shl $04) or (_cpu_gbz80._#type shl $03) or (011b))
        end if
    end macro
end iterate

macro stop?
    db (00001000b)
end macro

macro jr? arguments?*&
    local destination, postfix, polynomial
    match condition =, target, arguments
        _assert_element,_cpu_gbz80._condition._condition_type, condition
        redefine destination target
        db ((001b shl $05) or (((condition) metadata $01 scale $00) shl $03))
    else
        redefine destination arguments
        db (00011000b)
    end match
    destination _rpn_single_pass postfix, polynomial, destination
    if (~(polynomial))
        _bound_limit (destination - $02), $08, SIGNED
        db (destination - $02)
    else
        db $00
        _section.request_patch postfix, _JR_BYTE
    end if
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

iterate type*, pop,push
    macro type? r16?*
        local target, proxy
        target _cpu_gbz80.retreive_register r16
        proxy _cpu_gbz80.retreive_scale target, $03
        db ((11b shl $06) or (proxy shl $04) or (_cpu_gbz80._#type shl $02) or (01b))
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

iterate type*, bit,res,set
    macro type? offset?*, r08?*
        local target
        if (elementsof (offset))
            error (`type), "Non constant expression."
        end if
        _bound_limit offset, $03, UNSIGNED
        target _cpu_gbz80.retreive_register r08
        _assert_element,_cpu_gbz80._register_r08, target
        db ((11001011b) or (_cpu_gbz80._#type shl $06) or (offset shl $03) or (target metadata $01 scale $00))
    end macro
end iterate
