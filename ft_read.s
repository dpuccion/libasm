section .text
global ft_read
extern __errno_location    ; function to get pointer to errno

; ssize_t ft_read(int fd, void *buf, size_t count)
; Uses Linux syscall read

ft_read:
    mov     rax, 0          ; syscall number for read
    syscall                 ; call kernel: read(fd = RDI, buf = RSI, count = RDX)

    cmp     rax, 0          ; check result
    jge     .done           ; if >= 0: success or EOF → return that value

    ; --- error path: syscall returned negative (‑errno) ---
    neg     rax             ; rax = errno (positive)
    mov     edi, eax        ; copy errno into edi (int)

    sub     rsp, 8          ; align stack to 16‑byte boundary (for function call)
    call    __errno_location wrt ..plt  ; get address of errno
    add     rsp, 8          ; restore stack

    mov     [rax], edi      ; *errno = errno_value
    mov     rax, -1         ; return ‑1 on error
.done:
    ret                     ; return (rax = #bytes read, 0 = EOF, or -1 on error)
