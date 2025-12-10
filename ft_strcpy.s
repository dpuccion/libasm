section .text
global ft_strcpy

; char *ft_strcpy(char *dest, const char *src)
; SysV AMD64: dest in RDI, src in RSI, return in RAX
; Copies bytes from src to dest, including the terminating '\0';
; returns dest. (C strcpy semantics). :contentReference[oaicite:0]{index=0}

ft_strcpy:
    mov     rax, rdi        ; keep original dest in RAX for the return value

.copy:
    mov     dl, [rsi]       ; DL = *src (read next byte)
    mov     [rdi], dl       ; *dest = DL (store that byte)
    inc     rsi             ; advance src pointer
    inc     rdi             ; advance dest pointer
    test    dl, dl          ; set flags from DL; ZF=1 if DL == 0 ('\0')
    jnz     .copy           ; if not zero, keep copying

    ret                     ; return original dest (in RAX), per strcpy spec. :contentReference[oaicite:1]{index=1}
