section .text
global ft_write
extern __errno_location       ; from glibc

; ssize_t ft_write(int fd, const void *buf, size_t count)
; fd   in RDI
; buf  in RSI
; count in RDX
; return value in RAX

ft_write:
    mov     rax, 1            ; sys_write
    syscall

    cmp     rax, 0
    jge     .done             ; rax >= 0 → success, just return

    ; error: rax = -errno
    neg     rax               ; rax = errno
    mov     rdi, rax          ; arg for __errno_location(errno_value)
    call    __errno_location  ; returns int* in RAX
    mov     [rax], edi        ; *errno = (int)errno_value
    mov     rax, -1           ; write() returns -1 on error

.done:
    ret
