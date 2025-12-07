section .text
global ft_strcmp

; int ft_strcmp(const char *s1, const char *s2)
; s1 in RDI, s2 in RSI, return in EAX

ft_strcmp:
.loop:
    mov     al, [rdi]        ; load *s1
    mov     dl, [rsi]        ; load *s2
    cmp     al, dl
    jne     .diff            ; bytes differ -> compute result

    cmp    al, 0           ; al == 0 ?
    je      .equal           ; both zero, strings equal

    inc     rdi              ; advance both pointers
    inc     rsi
    jmp     .loop

.diff:
    ; return (unsigned char)al - (unsigned char)dl
    movzx   eax, al          ; zero-extend al to 32 bits
    movzx   edx, dl          ; zero-extend dl to 32 bits
    sub     eax, edx         ; eax = eax - edx
    ret

.equal:
    xor     eax, eax         ; return 0
    ret