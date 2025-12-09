section .text
global ft_read
extern __errno_location

; ssize_t ft_read(int fd, void *buf, size_t count)
; fd   in RDI
; buf  in RSI
; count in RDX
; return value in RAX

ft_read:
    mov     rax, 0            ; sys_read
    syscall

    cmp     rax, 0
    jge     .done             ; rax >= 0 → success

    ; error: rax = -errno
    neg     rax               ; rax = errno
    mov     rdi, rax
    call    __errno_location  ; returns &errno
    mov     [rax], edi        ; *errno = (int)errno_value
    mov     rax, -1           ; read() returns -1 on error

.done:
    ret
