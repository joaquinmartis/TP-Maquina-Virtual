; listas dinámicas enlasadas de enteros
; [0] valor entero
; [4] puntero a siguiente elemento

; crear un nodo
; parametro +4: valor entero
; devuelve en eax el puntero al nodo creado
new_nodo:   push    bp
            mov     bp,sp
            
            push    8
            call    alloc
            add     sp,4

            mov     [eax], [bp+8]
            mov     [eax+4], null

            mov     sp,bp
            pop     bp
            ret     

; insertar un nodo en una lista ordenada
; parametro +4: doble puntero a la lista
; parametro +8: puntero al nodo a insertar
insert_nodo:    push    bp
                mov     bp,sp
                push    eax
                push    ebx
                push    edx
                
                mov     edx, [bp+8]     ; doble puntero a la lista
                mov     ebx, [edx]      ; puntero a la lista
                mov     eax, [bp+12]    ; puntero al nodo a insertar

                cmp     ebx, null
                jz      insert_actual

                cmp     [eax], [ebx]    ; si el nodo a insertar es menor que el primero
                jnp     insert_actual   ; insertar al principio

                add     ebx, 4
                push    eax
                push    ebx
                call    insert_nodo
                add     sp,8
                jmp     insert_fin

insert_actual:  mov     [eax+4], [edx]   ; insertar al principio
                mov     [edx], eax
                jmp     insert_fin
insert_fin:     pop     edx
                pop     ebx
                pop     eax
                mov     sp,bp
                pop     bp
                ret