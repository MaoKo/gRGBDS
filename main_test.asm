
;include "gRGBDS.inc"

include "pairing.inc"
include "assert.inc"
include "insert_token.inc"

include "irps.inc"
include "string.inc"

include "preprocessor.inc"
;include "sections.inc"
;include "rpn.inc"

include "fasmg_identifier.inc"

repeat $100
    irps item[+space|+ident], 1 * 4 * 1 * 1 * 2 * 3 * 4 * 6
    end irps
end repeat
