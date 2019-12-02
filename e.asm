
_irps_line? := $00
namespace _irps_line?
end namespace

macro irps?! parameter?*, text?*&
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
        while $01
            match __input?, buffer
                redefine __prefix?
                redefine __prefix?.__input __delimiter__
                match,__prefix
                    match _1 =__delimiter__? _2, __prefix?.__input
                        local shift, input
                        input = (`__input)
                        shift = lengthof(input) - lengthof(`_2) 
                        eval "match _, ", string ((($01 shl (shift * $08)) - $01) and input)
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
    end if
    match _, finals
        outscope irpv parameter, _
end macro

macro end?.irps!
        end irpv
    end match
end macro

rept $2
    display "RESTART", $A
irps A,_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    display "A = ", `A, $A
end irps
end rept

;line equ A + 4
;match __line, line
;    redefine __abc?
;    redefine __abc?.__line |
;    match,__abc?
;        match _1 =| _2, __abc?.__line
;            =_2
;        end match
;    end match
;end match

