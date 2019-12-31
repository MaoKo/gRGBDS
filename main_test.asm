
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"
include "insert_token.inc"

include "irps.inc"
include "string.inc"

include "preprocessor.inc"
;include "string_function.inc"
include "fasmg_identifier.inc"
include "rpn.inc"

include "reverse.inc"
include "forward.inc"
include "itoa_base.inc"
include "error.inc"

include "bound_limit.inc"
include "gbz80.inc"
include "sections.inc"
include "symbols.inc"
;include "postpone.inc"

B = 2
repeat $10000
A _rpn_single_pass,, 1 __add B
end repeat

;result _expand_single_line_macro\
;    abc strlen (4)
;
;match _, result
;_
;end match

