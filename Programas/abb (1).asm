;-----------------------
                mov [es], es
                add [es], 4

                call main
                stop 
;-----------------------
\\include "alloc.asm"
;-----------------------
; crea nodo abb
; entrada: +8) valor
;-----------------------
abb_new:        push    bp
                mov     bp,sp
                
                push    12 
                call    alloc
                add     sp, 4

                cmp     eax, null
                jz      abb_new_end

                mov     [eax], [bp+8]
                mov     [eax+4], null
                mov     [eax+8], null

abb_new_end:    mov     sp, bp
                pop     bp
                ret
;-----------------------
; inserta nodo abb
; entrada: +8)  **ROOT
;          +12) *nodo
;-----------------------
abb_insert:         push    bp
                    mov     bp, sp
                    push    eax
                    push    ebx
                    push    edx

                    mov     edx, [bp+8]
                    cmp     edx, null
                    jz      abb_insert_end

                    mov     eax, [bp+12]
                    cmp     eax, null
                    jz      abb_insert_end

                    mov     ebx, [edx]  ; ebx apunta al nodo

                    cmp     ebx, null
                    jz      abb_insert_aqui

                    cmp     [ebx], [eax]
                    jz      abb_insert_end
                    jp      abb_insert_left
abb_insert_rigth:   add     ebx, 4
abb_insert_left:    add     ebx, 4
                    push    eax
                    push    ebx
                    call    abb_insert
                    add     sp, 8
                    jmp     abb_insert_end
abb_insert_aqui:    mov     [edx], eax
abb_insert_end:     pop     edx
                    pop     ebx
                    pop     eax
                    mov     sp, bp
                    pop     bp
                    ret
;-----------------------
; remueve nodo abb
; entrada: +8)  **ROOT
;          +12) valor
; devuelve en eax el puntero al nodo removido
;-----------------------
abb_remove:         push    bp
                    mov     bp, sp
                    push    ebx
                    push    edx

                    mov     edx, [bp+8]  ; **root
                    cmp     edx, null
                    jz      abb_remove_end

                    mov     ebx, [edx]  ; ebx apunta al nodo candidato
                    cmp     ebx, null
                    jz      abb_remove_end

                    mov     eax, [bp+12] ; valor
                    cmp     [ebx], eax
                    jz      abb_remove_aqui
                    jp      abb_remove_left
abb_remove_rigth:   add     ebx, 4
abb_remove_left:    add     ebx, 4
                    push    ecx
                    push    ebx
                    call    abb_remove
                    add     sp, 8
                    jmp     abb_remove_end

abb_remove_aqui:    mov     eax, ebx
                    add     ebx, 4
                    push    [eax+8]
                    push    ebx
                    call    abb_insert
                    add     sp, 8
                    mov     [edx], [eax+4]
                    mov     [eax+4],null
                    mov     [eax+8],null
                    jmp     abb_remove_end

abb_remove_end:     pop     edx
                    pop     ebx
                    mov     sp, bp
                    pop     bp
                    ret
;-----------------------
; print nodo abb
; entrada: +8)  *ROOT;
;-----------------------
abb_print:          push    bp
                    mov     bp, sp
                    push    eax
                    push    ecx
                    push    edx
                    mov     edx, [bp+8]
                    cmp     edx, null
                    jz      abb_print_end

                    push    [edx+4]
                    call    abb_print
                    add     sp, 4

                    mov     ch, 4
                    mov     cl, 1
                    mov     al, 1
                    sys     2

                    push    [edx+8]
                    call    abb_print
                    add     sp, 4

abb_print_end:      pop     edx
                    pop     ecx
                    pop     eax
                    mov     sp, bp
                    pop     bp
                    ret
;-----------------------
; MAIN
;-----------------------
main:               push    bp
                    mov     bp, sp
                    sub     sp, 4

                    mov     edx, BP
                    sub     edx, 4
                    mov     [edx], null ; root

                    push    10
                    call    abb_new
                    add     sp, 4

                    push    eax
                    push    edx
                    call    abb_insert
                    add     sp, 8

                    push    5
                    call    abb_new
                    add     sp, 4

                    push    eax
                    push    edx
                    call    abb_insert
                    add     sp, 8

                    push    20
                    call    abb_new
                    add     sp, 4

                    push    eax
                    push    edx
                    call    abb_insert
                    add     sp, 8

                    push    [edx]
                    call    abb_print
                    add     sp, 4

                    push    10
                    push    edx
                    call    abb_remove
                    add     sp, 8

                    push    [edx]
                    call    abb_print
                    add     sp, 4

                    mov     sp, bp
                    pop     bp
                    ret