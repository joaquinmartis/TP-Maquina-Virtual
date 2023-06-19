\\EXTRA 3072
NULL equ -1
size_es equ 1024
mov [es],es
add [es],4
mov ebx,es
mov eex,%89AB
shl eex,16
add eex,%CDEF
mov [4],eex
call main;
stop

alloc:                  push bp
                        mov bp,sp
                        push ebx
                        mov edx,NULL
                        mov ebx,[es]
                        add ebx,[bp+8]
                        cmp bx,size_es
                        jp fin_alloc
                        mov edx,[es]
                        add [es],[bp+8]

fin_alloc:              pop ebx
                        mov sp,bp
                        pop bp
                        ret


main:                   push bp
                        mov bp,sp

                        
                        mov ebx,es
                        mov ah,2
                        mov al,4
                        mov ch,127
                        mov cl,127
                        mov dh, 126
                        mov dl, 0

                        sys %d

                        mov [0],al
                        mov edx, es
                        mov cl, 10
                        mov ch, 4
                        mov al, %01
                        sys 2
			sys %F
                        mov sp,bp
                        pop bp
			ret	
                       