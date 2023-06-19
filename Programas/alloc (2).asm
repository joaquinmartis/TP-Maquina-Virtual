\\EXTRA 1024
null    equ     -1
size_es equ     1024
; inicializar 
; mov [es], es
; add [es], 4
alloc:      push    bp
            mov     bp, sp
            push    bx
            mov     eax, null
            mov     bx, [es]
            add     bx, [bp+8]
            cmp     bx, size_es
            jp      allocfin
            mov     eax, [es]
            add     [es], [bp+8]
allocfin:   pop     bx
            mov     sp, bp  
            pop     bp
            ret