
macro calminstruction?.purge? variable?
        asmcmd  =irpv =_, =variable
        asmcmd  =restore =variable
        asmcmd  =end =irpv
end macro
