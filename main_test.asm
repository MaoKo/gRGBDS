
;include "gRGBDS.inc"

include "pairing.inc"
include "irps.inc"
include "string.inc"
include "assert.inc"
include "preprocessor.inc"

repeat $10000
    irps abc[+ident], A B C
    end irps
end repeat
