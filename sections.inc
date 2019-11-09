
_sections? := $00
namespace _sections?
    _LIMIT_OPTION :=  $02
    _NULL_SECTION := -$01
    element _section_type?
    iterate <type,value>, ROM0, $03, ROMX, $02, VRAM,$01, SRAM,$05,\
                          WRAM0,$00, WRAMX,$05,  OAM,$07, HRAM,$04
        element type : _section_type + value
    end iterate
    _current_section? = _NULL_SECTION
    _total? = $00
    _patchs? := $00
    namespace _patchs?
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
        db _sections._patchs.kind
        dd rpn_size
        virtual at $00
            irps item[+ident], expression
                if (defined _sections._patchs.item) ; may be a problem here
                    db _sections._patchs.item
                else
                    if (elementsof (item))
                        db _sections._patchs._LONG_SYM
                        if (_symbols._symbols_type relativeto (item metadata $01))
                            dd ((item metadata $01) scale $00)
                        else
                            err "syntax error: '", (`item), "' is not an label."
                        end if
                    else
                        db _sections._patchs._LONG_INT
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

macro _catch_sections?
    local directive_link
    directive_link equ db,dw,dl

    iterate disable, dd,dp,dq,dt,ddq,dqq,ddqq,\
                     rb,rw,rd,rp,rq,rt,rdq,rqq,rdqq
        iterate kind, macro,struc
            kind disable? line&
                err "syntax error: directive unknown"
            end kind
        end iterate
    end iterate

    iterate directive, db,dw,dl
        macro directive? line?*&
            err "Code generation before SECTION directive"
        end macro
    end iterate

    macro section? name?*, type?*, options?&
        local kind, org, bank, align, found, space
        must_string (name)
        align = -$01
        bank = -$01
        org = -$01
        match _ =[ base =], type
            must_numeric_unsigned (base)
            org  = base
            kind = _sections._
        else match _, type
            kind = _sections._
        end match
        if ((kind) metadata $01 relativeto _sections._section_type)\
         & ((kind)  element $01 eq (kind))
            iterate option, options
                if (%% > _sections._LIMIT_OPTION)
                    err "invalid operand: too many section options"
                end if
                iterate option_kind, align,bank
                    match =option_kind? =[ value =], option
                        must_numeric_unsigned (value)
                        if (option_kind <> -$01)
                            err "syntax error: option redefinition"
                        end if
                        option_kind = value
                    end match
                end iterate
            end iterate

            if (align <> -$01)
                if (org <> -$01)
                    err "Align can't be specified with address"
                else if (align < $00) | (align > $10)
                    err "Align must fit between $00-$10" 
                end if
                align = ($01 shl align)
            end if
            if (bank <> -$01)
                if ((bank <= $000) | (bank >= $1FF))
                    err "Bank number must fit between $000-$1FF"
                else if ((kind <> _sections.ROMX) & (kind <> _sections.VRAM)\
                       & (kind <> _sections.SRAM) & (kind <> _sections.WRAMX))
                    err "Bank only available for ROMX, WRAMX, SRAM or VRAM sections"
                end if
            end if

            found = $00
            irpv it, _sections?._name
                if (name = it)
                    found = %
                    break
                end if
            end irpv

            if (~(found))
                namespace _sections?
                    _name?         =: name
                    _kind?         =: kind
                    _org?          =: org
                    _bank?         =: bank
                    _align?        =: align
                    _patchs        equ
                    _area?         equ space
                    virtual at $00
                        space::
                    end virtual
                    match _, directive_link ; preserve original context
                        iterate directive, _
                            purge directive?
                            if ((kind relativeto _sections.ROM0) | (kind relativeto _sections.ROMX))
                                macro directive? line?*&
                                    local rpn_expression, rpn_result, polynomial, patch_space
                                    _rpn_builder rpn_expression, polynomial, line
                                    if (polynomial) ; if polynomial are present let the linker do the computation
                                        ;_request_patch patch_space, rpn_expression, ???
                                        namespace _section?
                                            match,_patchs
                                                _patchs reequ patch_space
                                            else
                                                _patchs reequ _patchs, patch_space
                                            end match
                                        end namespace
                                        rpn_result = $00
                                    else
                                        _rpn_evaluator rpn_result, rpn_expression
                                    end if
                                    virtual space
                                        directive rpn_result
                                    end virtual
                                end macro
                            else
                                macro directive? line?*&
                                    err "Section '", name, "' cannot contain code or data (nor ROM0 or ROMX)"
                                end macro
                            end if
                        end iterate
                    end match
                    _total? = _total + $01
                    _current_section? = _total
                end namespace
                section org
            else
                _sections._current_section = found
                iterate attr, kind,org,bank,align
                    iirpv it, found, section?._#attr
                        if (~(attr relativeto it)) | (attr <> it)
                            err "Section exists but with a different type"
                        end if
                    end iirpv
                end iterate
            end if
        else
            err "Section type unknown"
        end if
    end macro
end macro

macro end?._catch_sections?
    purge db?,dw?,dl?, section?
    _sections?._current_section = _sections._NULL_SECTION
    iterate disable, dd,dp,dq,dt,ddq,dqq,ddqq,\
                     rb,rw,rd,rp,rq,rt,rdq,rqq,rdqq
        purge   disable?
        restruc disable?
    end iterate
end macro

macro pushs?
    err "not yet implemented"
end macro

macro pops?
    err "not yet implemented"
end macro

