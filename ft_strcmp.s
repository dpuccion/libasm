section .text
global ft_strcmp

; int ft_strcmp(const char *s1, const char *s2)
; SysV AMD64: s1 in RDI, s2 in RSI, return in EAX/RAX
; Compares bytes as *unsigned char* and returns <0, 0, or >0. :contentReference[oaicite:0]{index=0}

ft_strcmp:
.loop:
    mov     al, [rdi]        ; AL = *s1 (1 byte)
    mov     dl, [rsi]        ; DL = *s2 (1 byte)
    cmp     al, dl           ; compare current bytes
    jne     .diff            ; if different, compute and return difference

    cmp     al, 0            ; reached '\0' on s1?
    je      .equal           ; if both bytes were zero, strings equal

    inc     rdi              ; advance s1 pointer
    inc     rsi              ; advance s2 pointer
    jmp     .loop            ; continue

.diff:
    ; Return (unsigned char)al - (unsigned char)dl
    movzx   eax, al          ; zero-extend AL to 32 bits (EAX) :contentReference[oaicite:1]{index=1}
    movzx   edx, dl          ; zero-extend DL to 32 bits (EDX)
    sub     eax, edx         ; EAX = EAX - EDX (negative/zero/positive per strcmp)
    ret

.equal:
    xor     eax, eax         ; EAX = 0 (strings equal)
    ret
