
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

result _rpn_builder, 1 __sub __unary_add 5
result_2 _rpn_evaluator 5 __add
result_3 _itoa_base result_2, $0A
display result_3, $A

;result _expand_single_line_macro\
;    abc strlen (4)
;
;match _, result
;_
;end match

