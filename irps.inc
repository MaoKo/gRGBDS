
_irps_line? := $00
namespace _irps_line?
end namespace
define _irps_line._MODULUS %

macro _irps_core? parameter?*, text?*&
    local buffer, active, tokens, finals
    buffer reequ text
    active = $00
    irpv _, buffer
        repeat $01, i:(`_)
            if (~(definite _irps_line.i.active))
                restore _irps_line.i.active
                define  _irps_line.i.active
                _irps_line.i.active = $00
                define _irps_line.i tokens
                active = $01
            end if
            finals equ _irps_line.i
        end repeat
    end irpv
    if (active)
        match __MODULUS, _irps_line._MODULUS
            while $01
                match __input?, buffer
                    redefine __prefix?
                    redefine __prefix?.__input __delimiter__
                    match,__prefix
                        match _1 =__delimiter__? _2, __prefix?.__input
                            local target, shifts
                            shifts = lengthof((`__input)) - lengthof(`_2) 
                            target = string ((($01 shl (shifts * $08)) - $01) and (`__input))
                            if (target = "%") | (target = "% ")
                                target = "__MODULUS"
                            end if
                            eval "match _, ", target
                                define tokens _
                                restore __prefix?._
                            eval "end match"
                            redefine buffer _2
                        else match _, __input
                            define tokens _
                            restore __prefix?._
                            break
                        end match
                    else
                        rawmatch _ __remain, __input
                            define tokens _
                            redefine buffer __remain
                        else
                            define tokens __input
                            break
                        end rawmatch
                    end match
                else
                    define tokens
                end match
            end while
        end match
    end if
    match _, finals
        outscope irpv parameter, _
end macro

macro irps?! parameter?*, text?*&
    _pairing match, irpv
    match
    irpv
    outscope _irps_core parameter, text
end macro

macro end?.irps!
        end irpv
    end match
end macro

