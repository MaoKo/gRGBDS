
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"
include "insert_token.inc"

include "irps.inc"
;include "string.inc"

include "preprocessor.inc"
include "rpn.inc"
include "evaluate_argument.inc"
include "string_function.inc"
include "fasmg_identifier.inc"
;include "sections.inc"
;include "rpn.inc"

include "reverse.inc"
include "forward.inc"
include "itoa_base.inc"
include "error.inc"

ret _expand_single_line_macro STRLEN("abc" 1 __add __string)
match _, ret
_
end match)

;result _expand_single_line_macro\
;    abc strlen (4)
;
;match _, result
;_
;end match

