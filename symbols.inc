
_symbols? := $00
namespace _symbols?
    _total?  = $00
    ?LOCAL? := $00
    IMPORT? := $01
    EXPORT? := $02
end namespace

macro _catch_symbols?!
    local last_name
    last_name equ
    struc (name?) ?! line&
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
                    element #local : _symbols._total
                    if (used #local)
                        full_used = $01
                    end if
                end namespace
                eval "full_name equ ", (`scope), ".", (`local)
            end match
            element name : _symbols._total
            if (used name) | (full_used)
                namespace _symbols?
                    match _, full_name
                        _name   =: _
                    end match
                    _type?      =: type
                    _filename?  =: __file__
                    _linenum?   =: __line__
                    _sectionid? =: _sections._current_section
                    _value?     =: ($$ - $)
                    _total?     =  _total + $01
                end namespace
            end if
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

; TODO RPN EXPR
macro _rpn?
end macro
