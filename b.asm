format elf64 executable $09
entry _start
segment executable writable
_itoa_base:
    pop     rcx
    xor     rax, rax
    not     rax         ; <- set to -1
    cmp     rsi, $02
    jl      ._end
    cmp     rsi, $10
    jg      ._end
    mov     rax, rdi
    mov     rdi, rsp
@@: xor     rdx, rdx
    div     rsi
    dec     rsp
    mov     dl, byte [_base_digit + rdx]
    mov     byte [rsp], dl
    test    rax, rax
    jnz     @b
    sub     rdi, rsp
    xchg    rax, rdi
._end:
    jmp     rcx

struc timeval tv_sec,tv_usec {
    .tv_sec     dq tv_sec
    .tv_usec    dq tv_usec
}

struc timezone tz_minuteswest,tz_dsttime {
    .tz_minuteswest dd tz_minuteswest
    .tz_dsttime     dd tz_dsttime
}

irp struct, timeval,timezone {
    virtual at $00
        struct  struct $00, $00
        struct#.size = ($ - $$)
    end virtual
}

_start:
    sub     rsp, timeval.size
    mov     rax, $74
    mov     rdi, rsp
    xor     rsi, rsi
    syscall
    mov     rdi, qword [rsp + timeval.tv_sec]
    mov     rsi, $0A
    call    _itoa_base 
    mov     rdx, rax
    mov     rax, $04
    mov     rdi, $01
    mov     rsi, rsp
    syscall
    mov     rax, $01
    xor     rdi, rdi
    syscall

segment readable
_base_digit: db "0123456789ABCDEF"
