
macro matchs?! parameter?*
    local final, stream, wrap, bracket, condition, cbrack, cbrace, escape
    bracket   equ
    condition equ
    cbrack = $00
    cbrace = $00
    final equ
    define stream final
    irps item[+space|+ident], parameter
        match =[, item
            cbrack = $01
            define stream bracket
        else match =], item
            ;if (~(escape))
            ;    bracket = $00
            ;end if
            match __pattern, bracket
                match __pattern
                end match
            end match
            bracket equ
            define stream final
        else if ={, item
            if (~(cbrack))
                cbrace = $01
                define stream condition
            end if
        else if =}, item
            match __pattern, condition
                if __pattern
                end if
            end match
            condition equ
            define stream final
        else match _, stream
            wrap equ item
            if (~(cbrack)) & (~(cbrace))
                iterate <kind,value> ~,__unary_neg, &,__logical_and, |,__logical_or
                    match =kind, item
                        wrap equ value
                    end match
                end iterate
            end if
            _insert_token _, wrap
        end match
        escape = 00
        match =\, item
            escape = $01
        end match
    end irps
end macro

macro end?.matchs?!
end macro

;matchs ~ [ =a?, a\] ] | [ =1, 2 ] & {A = 4}
;end matchs
