
define __identifier?
struc _fasmg_identifier? token?*&
    . = $00
    redefine  __identifier?.token.
    namespace __identifier?
        . = $01
        rawmatch _, token.
            match,_
                . = $02
            end match
        end rawmatch
    end namespace
end struc
