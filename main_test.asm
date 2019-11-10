
;include "irps.inc"
;include "string.inc"
;include "assert.inc"
;include "rpn.inc"
include "else_wrapper.inc"

iterate A, 1,2,3
    if (A <> 3)
        display "A = ", `A, $0A
    else
        ;break
    end if
else iterate    
    display "HERE", $0A
end iterate
