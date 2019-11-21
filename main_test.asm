
;include "gRGBDS.inc"

;include "pairing.inc"
;include "assert.inc"
;include "insert_token.inc"

;include "irps.inc"
;include "irps_struc.inc"
;include "string.inc"

;include "preprocessor.inc"
;include "sections.inc"
;include "rpn.inc"

;include "fasmg_identifier.inc"

;repeat $10000
;irps A[+space], _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
;end irpv
;end match
;end repeat

macro inst?
end macro

define A 1

match __A, A
    repeat $80000
        if (__A)
        end if
        if (__A)
        end if
    end repeat
end match
