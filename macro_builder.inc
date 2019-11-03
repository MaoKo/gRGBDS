
_builder? := $00
namespace _builder?
    _count_random?  = $00
    _in_macro?      = $00
    _in_rept?       = $00
    _macro_name?    equ
end namespace

macro _define_macro?! name?*
    _builder._macro_name? reequ name
    ?name? := $00
    namespace name?
        macro build?
            esc macro name? arguments?&
            _builder._in_macro =: $01
            name?._total? = $00
            match not_empty, arguments
                iterate argument, arguments
                    match,argument
                        err "syntax error: empty argument not allowed"
                    end match
                    name?._#% equ argument
                    name?._total? = %%
                end iterate
            end match
       end macro
    end namespace
end macro

macro end?._define_macro?!
    match name, _builder._macro_name
        namespace name?
            macro build?
                name?.build?
                repeat name?._total, i:$00
                    restore name?._#%
                end repeat
                restore name?._total?
                restore _builder._in_macro?
                esc end macro
            end macro
        end namespace
    end match
    restore _builder._macro_name?
end macro

macro _shift_arguments? name?*, amount?*
end macro

macro _restore_arguments? name?*
end macro

