main:       mov [es], es
            add [es], 4
            
            push    4
            call    alloc
            add     sp, 4
            mov     ebx, eax
            mov     [ebx], null ;lista vacía
            
            push    5
            call    new_nodo
            add     sp, 4

            push    eax
            push    ebx
            call    insert_nodo
            add     sp, 8

            ;mov     [ebx], eax

            push    10
            call    new_nodo
            add     sp, 4

            push    eax
            push    ebx
            call    insert_nodo
            add     sp, 8

            ;mov     ecx, [ebx]
            ;mov     [ecx+4], eax

            push    [ebx]
            call    printlist
            add     sp, 4

            stop

printlist:  push    bp
            mov     bp, sp

            mov     eax, 1
            mov     ch, 4
            mov     cl, 1
            mov     edx, [bp+8]

printotro:  cmp     edx, null
            jz      printfin

            sys     2
            mov     edx, [edx+4]

            jmp     printotro

printfin:   pop     edx
            pop     ecx
            pop     eax
            mov     sp, bp
            pop     bp
            ret           

\\include "alloc.asm"
\\include  "listas.asm"