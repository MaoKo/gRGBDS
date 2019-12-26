
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"
include "insert_token.inc"

include "irps.inc"
;include "string.inc"

include "preprocessor.inc"
include "rpn.inc"
include "string_function.inc"
include "fasmg_identifier.inc"
;include "sections.inc"
;include "rpn.inc"

include "reverse.inc"
include "forward.inc"
include "itoa_base.inc"
include "error.inc"

include "string.inc"
include "switch.inc"

A=255
abc _eval_string "\tA\nB{D+H}"
display "'", abc, "'", $0A

;result _expand_single_line_macro\
;    abc strlen (4)
;
;match _, result
;_
;end match

