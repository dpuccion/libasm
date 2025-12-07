section .text
global ft_strcpy

; char *ft_strcpy(char *dest, const char *src)
; dest in RDI, src in RSI, return dest in RAX

ft_strcpy:
    mov     rax, rdi        ; save original dest pointer for return

.copy:
    mov     dl, [rsi]       ; load byte from src
    mov     [rdi], dl       ; store byte into dest
    inc     rsi             ; advance src
    inc     rdi             ; advance dest
    test    dl, dl          ; was that byte 0?
    jnz     .copy           ; if not 0, keep copying

    ret                     ; dest (original) is in RAX
