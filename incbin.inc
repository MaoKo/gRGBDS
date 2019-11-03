
macro incbin? path?*, offset?&
    local bytes
    virtual at $00
        match _, offset
            match start =, size, _
                file path:start, size
            else
                err "INCBIN: syntax error"
            end match
        else
            file path
        end match
        load bytes : $ from $00
    end virtual
    db bytes
end macro
