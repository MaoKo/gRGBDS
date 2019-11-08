
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

include "assert.inc"
include "string.inc"
include "irps.inc"
include "preprocessor.inc"

_replace_symbols_operator result, 1 =! 3
match _, result
_
end match


