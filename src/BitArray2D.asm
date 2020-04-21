
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
        ;index
        imul rdx, rsi
        add rdx, rcx
        push rdx
        ;byte offset
        mov rax, rdx
        shr rax, 3
        mov rsi, rax
        ;bit offset
        pop rdx
        push rsi
        imul rsi, 8
        sub rdx, rsi
        ;mask
        mov rcx, 7
        sub rcx, rdx
        mov rdx, 1
        ;shift left
        .shift:
        cmp rcx, 0
        je .end
        dec rcx
        sal rdx, 1
        jnz .shift
        ;or together
        .end:
        pop rsi
        mov rcx, [rdi + rsi]
        or rcx, rdx
        mov [rdi + rsi], rcx


        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here - for now returning 0
        ;index
        imul rdx, rsi
        add rdx, rcx
        push rdx
        ;byte offset
        mov rax, rdx
        shr rax, 3
        mov rsi, rax
        ;bit offset
        pop rdx
        push rsi
        imul rsi, 8
        sub rdx, rsi
        ;mask
        mov rcx, 7
        sub rcx, rdx
        mov rdx, 1
        ;shift left
        .shift:
        cmp rcx, 0
        je .end
        dec rcx
        sal rdx, 1
        jnz .shift
        ;or together
        .end:
        pop rsi
        mov rcx, [rdi + rsi]
        and rcx, rdx
        cmp rcx, 0
        setg al
        movsx rax, al

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
