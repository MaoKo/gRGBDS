
macro _record_fields? namespace?*, index?*, type?*, parameter?*&
    local virtual_data
    iterate fields, parameter
        match =scalar?, type
            iterate <instr,name>, fields
                iirpv field, index, namespace._#name
                    instr field
                    if (field eqtype "")
                        db $00
                    end if
                end iirpv
            end iterate
        else match =composite?, type
            iirpv field, index, namespace._#fields
                iterate virtual_space, field
                    virtual virtual_space
                        load virtual_data : $ from $$
                    end virtual
                    db virtual_data
                end iterate
            end iirpv
        else
            err "syntax error: unknown type, must be either scalar or composite"
        end match
    end iterate
end macro

macro _rgbds_format?
    local rgbds_magic, not_import_label, section_contain_data, section_data, patchs_count, patch_data
    rgbds_magic := "RGB6"

    db rgbds_magic
    dd $00 ; _symbols._total
    dd _sections._user_sections._counter
;    repeat _symbols._total, i:$00
;        iirpv type, i, _symbols._type
;            not_import_label = $00
;            if (type <> _symbols.IMPORT)
;                not_import_label = $01
;            end if
;        end iirpv
;        _record_fields _symbols, i, scalar, db,name, db,type
;        if (not_import_label)
;            _record_fields _symbols, i, scalar, db,filename, dd,line_num,\
;                                                dd,_section_id, dd,value,
;        end if
;    end repeat
    if (_sections._user_sections._counter)
        irpv user_section, _sections._user_sections_list
            db string (user_section), $00
            namespace _sections._user_sections.user_section?
                db _start
                db _bank
                db _align
                if (__native)
                    virtual _area
                        load virtual_data : $ from $$
                    end virtual
                    db virtual_data
                    db _count_patchs
                    irpv patch, _patchs
                        virtual patch
                            load virtual_data : $ from $$
                        end virtual
                        db virtual_data
                    end irpv
                end if
            end namespace
        end irpv
    end if
end macro

postpone ?
;    end _catch_line
    end _handle_defined_data
    _rgbds_format
end postpone
