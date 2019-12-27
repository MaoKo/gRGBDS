
_sections? := $00
namespace _sections?
    _NULL_SECTION? := -$01
    _FLOAT_CONFIG? := -$01
    element _section_type?
    iterate <type*,value*>, ROM0, $03, ROMX, $02, VRAM,$01, SRAM,$05,\
                            WRAM0,$00, WRAMX,$05,  OAM,$07, HRAM,$04
        element type? : _section_type + value
    end iterate
    _patch_builder?  := $00
    namespace _patch_builder?
        _types? := $00
        namespace _types?
            _BYTE?    := $00
            _WORD?    := $01
            _LONG?    := $02
            _JR_BYTE? := $03
        end namespace
        _symbols? := $00
        namespace _symbols?
            iterate <symbol*,value*>,          __add,$00,         __sub,$01,         __mul,$02,\
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
    end namespace
    _user_sections? := $00
    namespace _user_sections?
        redefine _current? _NULL_SECTION
        _counter? = $00
    end namespace
end namespace

;struc _request_patch? expression?*, kind?*
;    local space, offset, rpn_size, rpn_data
;    redefine . space
;    offset = ($ - $$)
;    virtual at $00
;        space::
;        db __file__, $00
;        dd __line__
;        dd offset
;        db _sections._patch_builder.types.kind
;        dd rpn_size
;        virtual at $00
;            irps item, expression
;                rawmatch _, item
;                    if (defined _sections._patch_builder.symbols.item)
;                        db _sections._patch_builder.symbols.item
;                    else
;                        if (elementsof (item))
;                            db _sections._patch_builder.symbols._LONG_SYM
;                           if (_symbols._symbols_type relativeto (item metadata $01))
;                                dd ((item metadata $01) scale $00)
;                            else
;                                error "_request_patch", <"syntax error: '", (`item), "' is not an label.">
;                            end if
;                        else
;                            db _sections._patch_builder.symbols._LONG_INT
;                            dd item
;                        end if
;                    end if
;                end rawmatch
;            end irps
;            load rpn_data : $ from $$
;        end virtual
;        rpn_size := (lengthof (rpn_data))
;        db rpn_data
;    end virtual
;end struc

macro _handle_defined_data?
    iterate disable*, dd,dp,dq,dt,ddq,dqq,ddqq,\
                      rb,rw,rd,rp,rq,rt,rdq,rqq,rdqq
        iterate kind*, macro,struc
            kind disable? line&
                error (`disable), "syntax error: directive unknown."
            end kind
        end iterate
    end iterate
    iterate <directive*,type*>, db,_BYTE, dw,_WORD, dl,_LONG
        macro directive? line?*&
            local rpn_expression, rpn_result, polynomial, patch_space
            if (_sections._user_sections._current = _sections._NULL_SECTION)
                error (`directive), "program error: code generation before SECTION directive."
            end if
            repeat $01, i:(_sections._user_sections._current)
                namespace _sections._user_sections.i?
                    if (~(__native))
                        error (`directive), <"program error: section '", name,\
                            "' cannot contain code or data (nor ROM0 or ROMX).">
                    end if
                    ;rpn_expression _rpn_builder polynomial, line
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
            end repeat
        end macro
    end iterate
end macro

macro end?._handle_defined_data?
    _sections?._user_sections._current? = _sections._NULL_SECTION
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
    align = _sections._FLOAT_CONFIG
    bank  = _sections._FLOAT_CONFIG
    start = _sections._FLOAT_CONFIG
    match _, type
        rawmatch type_2 =[ base =], _
            _bound_limit base, $10, UNSIGNED
            start = base
            kind  = _sections.type_2
        else
            kind  = _sections._
        end rawmatch
    end match
    _assert_element,_sections._section_type, kind
    iterate option, options
        if ((%%) > $02)
            error "_section", "invalid operand: too many section options."
        end if
        iterate _*, align,bank
            match =_? =[ value =], option
                _bound_limit value, $10, UNSIGNED
                if ((_) <> _sections._FLOAT_CONFIG)
                    error "_section", "syntax error: option redefinition."
                end if
                _ = value
            end match
        end iterate
    end iterate
    if (align <> _sections._FLOAT_CONFIG)
        if (start <> -$01)
            error "section", "invalid operand: align can't be specified with address."
        else if (align < $00) | (align > $10)
            error "section", "invalid operand: align must fit between $00-$10." 
        end if
        align = ($01 shl align)
    else
        align = $01
    end if
    if (bank <> _sections._FLOAT_CONFIG)
        if ((bank <= $000) | (bank >= $1FF))
            error "section", "invalid operand: bank number must fit between $000-$1FF."
        else if ((kind <> _sections.ROMX) & (kind <> _sections.VRAM)\
               & (kind <> _sections.SRAM) & (kind <> _sections.WRAMX))
            error "section", "invalid operand: bank only available for ROMX, WRAMX, SRAM or VRAM sections."
        end if
    end if
    repeat $01, i:(name)
        if (~(definite _sections._user_sections.i))
            namespace _sections?
                namespace _user_sections?
                    define  ?i?
                    restore ?i?
                    ?i? = name
                    namespace ?i?
                        __index?    := _counter
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
                            redefine _area? space
                        end if
                    end namespace
                    _counter? = _counter + $01
                end namespace
                _user_sections_list? =: i
            end namespace
        else
            namespace _sections?._user_sections?.i?
                iterate field, kind,start,bank,align
                    if (~(field relativeto _#field)) | (field <> _#field)
                        error "section", "program error: section exist but with a different type."
                    end if
                end iterate
            end namespace
        end if
        _sections._user_sections._current? =: i
    end repeat
end macro

macro pushs?
    err "not yet implemented"
end macro

macro pops?
    err "not yet implemented"
end macro

