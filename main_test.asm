
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"
include "insert_token.inc"

include "irps.inc"
;include "string.inc"

include "preprocessor.inc"
include "rpn.inc"
include "string_function.inc"
;include "sections.inc"
;include "rpn.inc"

result _expand_single_line_macro\
    abc strlen ("ABC")

match _, result
_
end match

