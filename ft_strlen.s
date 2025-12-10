section .text
global ft_strlen

; size_t ft_strlen(const char *s)
; SysV AMD64: s in RDI, return in RAX
; Count bytes until the first '\0' (not included), then return the count.

ft_strlen:
    xor     eax, eax            ; len = 0 (RAX will be our counter/return)

.loop:
    mov     dl, [rdi+rax]       ; DL = s[len]  (read next byte)
    test    dl, dl              ; is it '\0' ?
    jz      .done               ; yes -> we're done
    inc     rax                 ; len++
    jmp     .loop               ; continue

.done:
    ret                         ; return len in RAX
