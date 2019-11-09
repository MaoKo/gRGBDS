
macro matchs?! parameter?*
    macro else?.matchs?!
    end macro
end macro

macro end?.matchs?!
    purge else?.matchs?
end macro

;matchs ~ [ =a?, a\] ] | [ =1, 2 ]
;end matchs
