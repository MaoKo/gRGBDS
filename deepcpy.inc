
macro calminstruction?.deepcpy? dst?*, src?*
    asmcmd  =irpv =_, src
    asmcmd  =define dst =_
    asmcmd  =end =irpv
end macro
