
_irps_special? := $00
_irps_cache? := $00
namespace _irps_special
    redefine __MODULUS %    ; ensure to not expand even when in loop
end namespace

macro _irps_core? parameter?*, text?*&
    local buffer, active, token, final, space
    buffer reequ text
    active = $00
    irpv _, buffer
        repeat $01, i:(`_)
            if (~(definite _irps_cache.i.active))
                restore _irps_cache.i.active
                define  _irps_cache.i.active
                _irps_cache.i.active = $00
                redefine _irps_cache.i token
                active = $01
            end if
            final reequ _irps_cache.i
        end repeat
    end irpv
    if (active)
        space = $00
        match __terminate= ,buffer
            space = $01
        end match
        match __MODULUS, _irps_special.__MODULUS
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
                                define token _
                                if ((`_) <> (target))
                                    define token
                                end if
                                restore __prefix?._
                            eval "end match"
                            redefine buffer _2
                        else match _, __input
                            define token _
                            restore __prefix?._
                            break
                        end match
                    else
                        rawmatch _ __remain, __input
                            define token _
                            rawmatch =_= __insert, __input
                                define token
                            end rawmatch
                            redefine buffer __remain
                        else
                            define token __input
                            break
                        end rawmatch
                    end match
                else
                    define token
                end match
            end while
        end match
        if (space)
            define token
        end if
    end if
    match _, final
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

