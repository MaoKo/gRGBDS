
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

;A = 5
;_assert_string A

a _itoa_base -255, 10
display a, $0A

;    display "I = ", `i, $A

;result _expand_single_line_macro\
;    abc strlen (4)
;
;match _, result
;_
;end match

