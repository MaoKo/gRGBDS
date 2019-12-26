
_sections? := $00
namespace _sections?
    _LIMIT_OPTION? :=  $02
    _NULL_SECTION? := -$01
    element _section_type?
    iterate <type,value>, ROM0, $03, ROMX, $02, VRAM,$01, SRAM,$05,\
                          WRAM0,$00, WRAMX,$05,  OAM,$07, HRAM,$04
        element type : _section_type + value
    end iterate
    _patchs_builder?  := $00
    namespace _patchs_builder?
        _BYTE?    := $00
        _WORD?    := $01
        _LONG?    := $02
        _JR_BYTE? := $03
        iterate <symbol,value>,           __add,$00,         __sub,$01,         __mul,$02,\
                                          __div,$03,         __mod,$04, __unary_minus,$05,\
                                           __or,$10,         __and,$11,         __xor,$12,\
                                 __unary_negate,$13, __logical_and,$21,  __logical_or,$22,\
                                    __unary_not,$23,          __eq,$30,          __ne,$31,\
                                           __gt,$32,          __lt,$33,          __ge,$34,\
                                           __le,$35,         __shl,$40,         __shr,$41
            symbol? := value
        end iterate
        _LONG_INT? := $80
        _LONG_SYM? := $81
    end namespace
    _user_sections? := $00 ; user defined section
    namespace _user_sections?
    end namespace
    _current_section?  reequ _NULL_SECTION
    _sections_counter? = $00
end namespace

macro _request_patch? result?*, expression?*, kind?*
    local space, offset, rpn_size, rpn_data
    result equ space
    offset = ($ - $$)
    virtual at $00
        space::
        db __file__, $00
        dd __line__
        dd offset
        db _sections._patchs_builder.kind
        dd rpn_size
        virtual at $00
            irps item[+ident], expression
                if (defined _sections._patchs_builder.item)
                    db _sections._patchs_builder.item
                else
                    if (elementsof (item))
                        db _sections._patchs_builder._LONG_SYM
                       if (_symbols._symbols_type relativeto (item metadata $01))
                            dd ((item metadata $01) scale $00)
                        else
                            err "syntax error: '", (`item), "' is not an label."
                        end if
                    else
                        db _sections._patchs_builder._LONG_INT
                        dd item
                    end if
                end if
            end irps
            load rpn_data : $ from $$
        end virtual
        rpn_size := (lengthof (rpn_data))
        db rpn_data
    end virtual
end macro

macro _handle_defined_data?
    iterate disable, dd,dp,dq,dt,ddq,dqq,ddqq,\
                     rb,rw,rd,rp,rq,rt,rdq,rqq,rdqq
        iterate kind, macro,struc
            kind disable? line&
                err "syntax error: directive unknown"
            end kind
        end iterate
    end iterate
    iterate <directive,type>, db,_BYTE, dw,_WORD, dl,_LONG
        macro directive? line?*&
            local rpn_expression, rpn_result, polynomial, patch_space
            if (_sections._current_section = _sections._NULL_SECTION)
                err "program error: code generation before SECTION directive"
            end if
            match user_section, _sections._current_section
                namespace _sections._user_sections._#user_section?
                    if (~(__native))
                        err "program error: section '", name, "' cannot contain code or data (nor ROM0 or ROMX)"
                    end if
                    rpn_expression _rpn_builder polynomial, line
                    ;if (polynomial) ; if polynomial are present let the linker do the computation
                    ;    _request_patch patch_space, rpn_expression, _sections._patchs.type
                    ;    _patchs equ patch_space
                    ;    _count_patchs? = _count_patchs + $01
                    ;    rpn_result = $00
                    ;else
                    ;    _rpn_evaluator rpn_result, rpn_expression
                    ;end if
                    virtual _area
                        directive line ; rpn_result
                    end virtual
                end namespace
            end match
        end macro
    end iterate
end macro

macro end?._handle_defined_data?
    _sections?._current_section? reequ _sections._NULL_SECTION
    purge db?,dw?,dl?
    iterate disable, dd,dp,dq,dt,ddq,dqq,ddqq,\
                     rb,rw,rd,rp,rq,rt,rdq,rqq,rdqq
        purge   disable?
        restruc disable?
    end iterate
end macro

macro section? name?*, type?*, options?&
   local kind, bank, start, align, space
    _assert_string (name)
    align   = -$01
    bank    = -$01
    start   = -$01
    match _ =[ base =], type
        _assert_numeric_unsigned (base)
        start  = base
        kind   = _sections._
    else match _, type
        kind   = _sections._
    end match
    if ((kind) metadata $01 relativeto _sections._section_type) & ((kind)  element $01 eq (kind))
        iterate option, options
            if (%% > _sections._LIMIT_OPTION)
                err "invalid operand: too many section options"
            end if
            iterate option_kind, align,bank
                match =option_kind? =[ value =], option
                    _assert_numeric_unsigned (value)
                    if (option_kind <> -$01)
                        err "syntax error: option redefinition"
                    end if
                    option_kind = value
                end match
            end iterate
        end iterate
        if (align <> -$01)
            if (start <> -$01)
                err "invalid operand: align can't be specified with address"
            else if (align < $00) | (align > $10)
                err "invalid operand: align must fit between $00-$10" 
            end if
            align = ($01 shl align)
        end if
        if (bank <> -$01)
            if ((bank <= $000) | (bank >= $1FF))
                err "invalid operand: bank number must fit between $000-$1FF"
            else if ((kind <> _sections.ROMX) & (kind <> _sections.VRAM)\
                   & (kind <> _sections.SRAM) & (kind <> _sections.WRAMX))
                err "invalid operand: bank only available for ROMX, WRAMX, SRAM or VRAM sections"
            end if
        end if

        repeat $01, i:name
            if (~(definite _sections._user_sections._#i)) ; check if already exist
                namespace _sections?
                    namespace _user_sections?
                        _#i? := name
                        namespace _#i?
                            __index?    := _sections_counter
                            __native?    = $00
                            _kind?      := kind
                            _start?     := start
                            _bank?      := bank
                            _align?     := align
                            if ((kind relativeto _sections.ROM0) | (kind relativeto _sections.ROMX))
                                virtual at $00 ; create a new virtual area for the section
                                    space::
                                end virtual
                                __native?       =  $01
                                _count_patchs?  =: $00
                                _area?         equ space
                            end if
                        end namespace
                    end namespace
                    _sections_counter? = _sections_counter + $01
                    _user_sections_list? =: i
                end namespace
            else
                namespace _sections?._user_sections?._#i?
                    iterate field, kind,start,bank,align
                        if (~(field relativeto _#field)) | (field <> _#field)
                            err "program error: section exist but with a different type"
                        end if
                    end iterate
                end namespace
            end if
            _sections._current_section? reequ i
        end repeat
    else
        err "syntax error: section type unknown"
    end if
end macro

macro pushs?
    err "not yet implemented"
end macro

macro pops?
    err "not yet implemented"
end macro

