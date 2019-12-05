
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"
include "insert_token.inc"

include "irps.inc"
;include "string.inc"

include "preprocessor.inc"
;include "sections.inc"
;include "rpn.inc"

result _expand_single_line_macro abc strlen ((1 + 4), 5 << 6)
match _, result
_
end match
