section .text
global ft_write
extern __errno_location        ; glibc helper: returns &errno for this thread

; ssize_t ft_write(int fd, const void *buf, size_t count)
; SysV AMD64: fd=RDI, buf=RSI, count=RDX, return=RAX
ft_write:
    mov     rax, 1             ; syscall number for write(2)
    syscall                    ; do write(fd, buf, count)

    cmp     rax, 0
    jge     .done              ; >=0: success → return bytes written

    ; error path: kernel returned negative (-errno)
    neg     rax                ; RAX = errno (positive)
    mov     edi, eax           ; keep errno in EDI (int)

    sub     rsp, 8             ; align stack (16B) before calling C function
    call    __errno_location wrt ..plt   ; get &errno (thread-local)
    add     rsp, 8

    mov     [rax], edi         ; *errno = errno_value
    mov     rax, -1            ; libc convention: return -1 on error
.done:
    ret
