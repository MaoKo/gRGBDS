
iterate rept_macro, iterate,irp,repeat,rept,irpv
    macro rept_macro?! parameter?*&
        local used_break
        used_break = $00
        macro break?
            used_break = $01
            break
        end macro
        macro else?.rept_macro?!
            end rept_macro
            macro end?.rept_macro?!
                end if
                purge end?.rept_macro?
            end macro
            if (~(used_break))
        end macro
        outscope rept_macro parameter
    end macro
    macro end?.rept_macro?!
        end rept_macro
        purge break?, else?.rept_macro?
    end macro
end iterate
