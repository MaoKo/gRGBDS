
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"

include "irps.inc"
include "string.inc"

;include "preprocessor.inc"
;include "sections.inc"
;include "rpn.inc"

include "fasmg_identifier.inc"

repeat $10000
_iterate_string A, "DEADxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
end _iterate_string
end repeat

;repeat $10000
;    irps item[+space|+ident], A B C D E F G R A CX FG E Z R C _ _ _ _ _ _ 
;    end irps
;end repeat
