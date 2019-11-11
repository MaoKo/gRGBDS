
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
    dd _symbols._total
    dd _sections._section_counter
    repeat _symbols._total, i:$00
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
    end repeat
    match list, _sections._user_sections_list
        iterate user_section, list
            db string (user_section), $00
            namespace _sections._user_sections._#user_section?
                db _org
                db _bank
                db _align
;                if (???)
;                    virtual _area
;                        load virtual_data : $ from $$
;                    end virtual
;                    db virtual_data
;                    db _count_patchs
;                    iterate patch, _patchs
;                        virtual patch
;                            load virtual_data : $ from $$
;                        end virtual
;                        db virtual_data
;                    end iterate
;                end if
            end namespace
        end iterate
    end match

    repeat _sections._section_counter
        ;iirpv kind, %, _sections._kind
        ;    section_contain_data = $00
        ;    if ((kind relativeto _sections.ROM0) | (kind relativeto _sections.ROMX))
        ;        section_contain_data = $01
        ;    end if
        ;end iirpv
        ;iirpv name, 1, _sections._name
        ;    db name
        ;end iirpv
        ;_record_fields _sections, %, scalar, <db,name>; <TODO>:  <dd,kind>, <dd,org>, <dd,bank>, <dd,align>
        ;_record_fields _sections, %, scalar, <dd,org>, <dd,bank>, <dd,align>
        ;if (section_contain_data)
        ;    _record_fields _sections, %, composite, area
        ;    _record_fields _sections, %, scalar, <db,_count_patchs>
        ;    _record_fields _sections, %, composite, patchs
        ;end if
    end repeat
end macro

postpone ?
;    end _catch_line
    end _handle_defined_data
    _rgbds_format
end postpone
