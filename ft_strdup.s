section .text
global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy

; char *ft_strdup(const char *s)
; s in RDI
; return pointer in RAX

ft_strdup:
    push    rdi               ; save src (s) on stack

    ; len = ft_strlen(s)
    call    ft_strlen         ; RAX = len(s)
    inc     rax               ; +1 for '\0'

    ; p = malloc(len + 1)
    mov     rdi, rax          ; size_t size in RDI
    call    malloc            ; RAX = pointer or NULL

    test    rax, rax
    je      .malloc_fail      ; if NULL, return NULL

    ; success: call ft_strcpy(p, s)
    mov     rdi, rax          ; dest = p
    pop     rsi               ; src = original s (from stack)
    jmp     ft_strcpy         ; tail call: returns dest in RAX

.malloc_fail:
    pop     rcx               ; balance the stack, discard saved s
    ret                       ; RAX is already NULL
