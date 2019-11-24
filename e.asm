macro irps?! parameter?*, text?*&
    local buffer, token
    buffer reequ text
    while $01
        match __input?, buffer
            redefine __prefix?
            redefine __prefix?.__input __delimiter__
            match,__prefix
                rawmatch __suffix?, __input
                    match _1 =__delimiter__? _2, __prefix?.__suffix
                        local shift, tname
                        shift = lengthof(`__input) - lengthof(`_2) 
                        tname = string ((($01 shl (shift * $08)) - $01) and (`__suffix))
                        eval "define token ", tname
                        eval "restore __prefix?.", tname
                        redefine buffer _2
                    else match name, __input
                        define token name
                        restore __prefix?.name
                        break
                    end match
                end rawmatch
            else
                rawmatch first remain, __input
                    define token first
                    redefine buffer remain
                else
                    rawmatch remain, __input
                        define token remain
                    end rawmatch
                    break
                end rawmatch
            end match
        end match
   end while
    outscope irpv parameter, token
end macro

macro end?.irps!
    end irpv
end macro

rept $10000
irps A, A.C .L A.A.A.A.A.A.A.A.A.A.A.A.A.A.A.A.A.A. O+1 + A.A A.A A.A A.A A.A A.A A.A A.A A.A _ _ _ _ _ _ _ _ _ _ _ 
end irps
end rept
