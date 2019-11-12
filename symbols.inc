
_symbols? := $00
namespace _symbols?
    ?LOCAL? := $00
    IMPORT? := $01
    EXPORT? := $02
    element _symbols_type?
    element _undefined_field?
    _total?  = $00
    _user_symbols? := $00
    namespace _user_symbols?
    end namespace
end namespace

; TODO: element @ : _symbols._symbols_type
macro _catch_symbols?!
    local last_name
    last_name equ
    struc (name?) ?! line?&
        local label, type, found, remain, dots, local_name, full_name, full_used
        label = $01
        type = _symbols.LOCAL
        found = $00
        remain equ line
        iterate kind, ==, =set?, =equ?, =equs?, ::, :
            match kind _, line
                found = $01
                remain equ _
            else match kind, line
                found = $01
            end match
            if (found)
                if (% <= $05)
                    if (% <> $05)
                        label = $00
                    end if
                    type = _symbols.EXPORT
                end if
                break
            end if
        end iterate
        if (label)
            dots = $00
            irps token, name
                match ., token
                    dots = dots + $01
                end match
            end irps
            if (dots > $01)
                err "syntax error: too many dots in the label" 
            end if
            local_name equ
            match scope? . local?, name
                local_name equ local
                match =scope, last_name
                else
                    err "Not currently in the scope of '", (`scope), "'"
                end match
            else match . local?, name
                local_name equ local
                match,last_name
                   err "Local label '", (`local), "' in main scope"
                end match
            else
                last_name equ name
            end match
            full_name equ name
            full_used = $00
            match scope? . local?, last_name . local_name
                namespace scope
                    element #local : _symbols._symbols_type + _symbols._total
                    if (used #local)
                        full_used = $01
                    end if
                end namespace
                eval "full_name equ ", (`scope), ".", (`local)
            end match
            element name : _symbols._symbols_type + _symbols._total
            if (used name) | (full_used)
                match _full_name, full_name
                    namespace _symbols?
                        _name?   =: _full_name
                        _type?   =: type
                        if (_type <> _symbols.IMPORT)
                            _filename?   =: __file__
                            _line_num?   =: __line__
                            _section_id? =: _sections._current_section
                            _value?      =: ($$ - $)
                        else
                            iterate field, filename,line_num,section_id,value
                                _#field? =: _undefined_field
                            end iterate
                        end if
                        _total?  =  _total + $01
                    end namespace
                end match
            end if
            ; TODO 
            ;match _, remain
            ;    _
            ;end match
        else
            ;name line
        end if
    end struc
end macro

macro end?._catch_symbols?!
    restruc ?
end macro

macro export? names?*&
end macro

macro global? names?*&
end macro
