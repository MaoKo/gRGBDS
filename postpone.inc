
postpone ?
    end _catch_line
    end _catch_sections
    db "RGB6"
    dd _symbols._total
    dd _sections._total
    repeat _symbols._total, i:$00
        iirpv name, i, _symbols._name
            db name, $00
        end iirpv
;        iirpv type, i, _symbols._type
;            db type
;        end iirpv
    end repeat
    repeat _sections._total
    end repeat
end postpone
