
_sections? := $00
namespace _sections?
    element _section_type?
    iterate <type,value>, ROM0, $03, ROMX, $02, VRAM,$01, SRAM,$05,\
                          WRAM0,$00, WRAMX,$05,  OAM,$07, HRAM,$04
        element type : _section_type + value
    end iterate
    NULL := -$01
    _current_section? = NULL
    _total? = $00
end namespace

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
                if (%% > $02)
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
                    _name?      =: name
                    _kind?      =: kind
                    _org?       =: org
                    _bank?      =: bank
                    _align?     =: align
                    _patchs?    =: $00
                    virtual at $00
                        space::
                        _area?  equ space
                    end virtual
                    match _, directive_link
                        iterate directive, _
                            purge directive?
                            if ((kind relativeto _sections.ROM0)\
                              | (kind relativeto _sections.ROMX))
                                macro directive? line?*&
                                    virtual space
                                        directive line
                                    end virtual
                                end macro
                            else
                                macro directive? line?*&
                                    err "Section '", name,\
                                        "' cannot contain code or data (nor ROM0 or ROMX)"
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
    purge db?,dw?,dl?,\
          section?
    namespace _sections?
        repeat _total
            restore _name?, _kind?, _org?, _bank?,\
                    _align?, _patchs?, _area?
        end repeat
        _current_section = NULL
        _total = $00
    end namespace
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

