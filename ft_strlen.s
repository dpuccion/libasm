section .text
global ft_strlen

ft_strlen:
   xor rax, rax           ; set RAX (length) to 0
   jmp .next_char
.next_char:
   cmp byte [rdi + rax], 0 ; compare current byte with 0
   je .done               ; if 0, jump to done
   inc rax                ; increment RAX
   jmp .next_char         ; repeat
.done:
   ret                    ; return length in RAX