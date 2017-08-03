
;include "gRGBDS.inc"

;end _catch_line

;_replace_bracket_operator result, 1

;match _, 4
;_
;end match

;display "TEST", $A

;_catch_line
;irp A, 1
;    %
;end irpv

include "assert_type.inc"
include "string.inc"
include "irps.inc"
include "preprocessor.inc"

empty equ 
irps a[+ident|+space], a..b.e+ e . l .@ =
    display `a, $0A
end irps
