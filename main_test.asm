
include "gRGBDS.inc"

_replace_symbols_operator result, 1 * / 0 + 2
match _, result
_
end match
