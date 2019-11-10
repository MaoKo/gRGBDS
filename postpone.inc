
macro _record_fields? namespace?*, index?*, type?*, parameter?*&
    local virtual_data
    macro assert_not_undefined_field? field?*
        if (elementsof (field)) & (field relativeto namespace._undefined_field)
            err "syntax error: field '", (`field), "' have an undefined value"
        end if
    end macro
    match =scalar?, type
        iterate <instr,field>, parameter
            iirpv field, namespace._#field
                assert_not_undefined_field
                instr field
                if (field eqtype "")
                    db $00
                end if
            end iirpv
        end iterate
    else match =compostite?, type
        iterate field, parameter
            iirpv field, namespace._#field
                assert_not_undefined_field
                iterate virtual_space, field
                    virtual virtual_space
                        load virtual_data : $ from $$
                    end virtual
                    db virtua_data
                end iterate
            end iirpv
        end iterate
    else
        err "syntax error: unknown type, must be either scalar or composite"
    end match
    purge assert_not_undefined_field?
end macro

macro _rgbds_format?
    local rgbds_magic, not_import_label, section_contain_data, section_data, patchs_count, patch_data
    rgbds_magic := "RGB6"

    db rgbds_magic
    dd _symbols._total
    dd _sections._total
    repeat _symbols._total, i:$00
        iirpv type, i, _symbols._type
            not_import_label = $00
            if (type <> _symbols.IMPORT)
                not_import_label = $01
            end if
        end iirpv
        _record_fields _symbols, i, scalar, db,name, db,type
        if (not_import_label)
            _record_fields _symbols, i, scalar, db,filename, dd,line_num,\
                                                dd,_section_id, dd,value,
        end if
    end repeat
    repeat _sections._total, i:$00
        iirpv kind, i, _sections._kind
            section_contain_data = $00
            if ((kind relativeto _sections.ROM0) | (kind relativeto _sections.ROMX))
                section_contain_data = $01
            end if
        end iirpv
        _record_fields _sections, i, scalar, db,name, dd,kind, dd,org, dd,bank, dd,align
        if (section_contain_data)
            _record_fields _sections, i, composite, area
            _record_fields _sections, i, scalar, db,_count_patchs
            _record_fields _sections, i, composite, patchs
        end if
    end repeat
end macro

postpone ?
;    end _catch_line
;    end _catch_sections
    rgbds_format
end postpone
