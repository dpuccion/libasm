section .text
global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy

; char *ft_strdup(const char *s)
; SysV AMD64: s in RDI, return in RAX
ft_strdup:
    push    rdi                ; save s on stack AND make %rsp 16-byte aligned before calls

    call    ft_strlen          ; RAX = strlen(s)                (bytes, not counting '\0')
    inc     rax                ; RAX = len + 1                  (space for the '\0')

    mov     rdi, rax           ; arg1 = size_t size
    call    malloc wrt ..plt   ; p = malloc(size)  (PIE-safe PLT call)

    test    rax, rax           ; p == NULL ?
    je      .malloc_fail       ; if malloc failed, return NULL

    mov     rdi, rax           ; arg1 = dest = p
    pop     rsi                ; arg2 = src  = saved s
    jmp     ft_strcpy          ; tail-call: strcpy(p, s); returns dest in RAX

.malloc_fail:
    pop     rcx                ; balance stack (discard saved s)
    ret                        ; RAX already NULL
