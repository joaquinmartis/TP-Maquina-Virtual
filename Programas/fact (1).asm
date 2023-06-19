      mov [0], 5
      mov [4], 1
      mov ecx, [0]
otro: cmp ecx, 0
      jz fin
      mul [4], ecx
      sub ecx, 1
      jmp otro
fin:  mov edx, ds
      add edx, 4
      mov ch, 4
      mov cl, 1
      mov al, 1
      sys 2
      stop