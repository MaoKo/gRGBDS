
; Experiment 

macro _local_namespace?! prefixs?, instructions?, labeleds?, expressions?
    local base_space, macro_link, outscope_link, struc_link,\
          define_link, end_link, symbols_list
    macro_link      equ macro
    outscope_link   equ outscope
    struc_link      equ struc
    define_link     equ define
    end_link        equ end
    match   macro? outscope? struc? define? end?,\
            macro_link outscope_link struc_link define_link end_link

        symbols_list reequ
        iterate instruction, end,instructions
            match _, instruction
                irpv item, symbols_list
                    match =_?, item
                        err "invalid operand: redefinition of '", `_, "' instruction class"
                    end match
                end irpv
                iterate prefix, prefixs
                    macro base_space.prefix#_?! line&
                        esc outscope _ line
                    end macro
                end iterate
                symbols_list equ _
            end match
        end iterate

        symbols_list reequ
        iterate labeled, labeleds
            match _, labeled
                irpv item, symbols_list
                    match =_?, item
                        err "invalid operand: redefinition of '", `_, "' labeled instruction"
                    end match
                end irpv
                iterate prefix, prefixs
                    struc base_space.prefix#_?! line&
                        . _ line
                    end struc
                end iterate
                symbols_list equ _
            end match
        end iterate

        symbols_list reequ
        iterate expression, expressions
            match _, expression
                irpv item, symbols_list
                    match =_?, item
                        err "invalid operand: redefinition of '", `_, "' expression class"
                    end match
                end irpv
                iterate prefix, prefixs
                    define base_space.prefix#_ expression
                end iterate
                symbols_list equ _
            end match
        end iterate

        iterate prefix, prefixs
            macro base_space.prefix#macro?! line&
                esc macro line
            end macro
            struc base_space.prefix#struc?! line&
                esc esc struc line
            end struc
        end iterate
    end match
    namespace base_space
end macro

; Example
;_local_namespace <,__>, <display,match>,,
;    __match _, 1
;        __display `_, $0A
;    end match
;end namespace
