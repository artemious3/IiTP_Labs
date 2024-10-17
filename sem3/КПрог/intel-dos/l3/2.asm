.model tiny
.code
                   org   100h
    start:         mov   dx, offset input_msg
                   call  print_str
                   call  read_str
                   call  reverse_words
                   mov   dx, offset output_msg
                   call  print_str
                   mov   dx, offset buf
                   call  print_str
                   ret
                
read_str proc
                   mov   ah, 0Ah
                   mov   dx, offset maxchar
                   int   21h


                   xor   bh, bh
                   mov   bl, [readchar]

                   mov   buf[bx], ' '
                   mov   buf[bx+1], 0Dh
                   mov   buf[bx+2], 0Ah
                   mov   buf[bx+3], '$'
                   inc   [readchar]
                   ret

read_str endp


reverse_words proc
                   mov   di, offset buf
                   xor   ch, ch
                   mov   cl, -1

    find_not_space:
                   mov   ax, ' '

    scan_ns:       inc   cl
                   cmp   cl, [readchar]
                   jae   fin
                   scasb
                   je    scan_ns

                   mov   si, cx

    find_space:    
                   mov   ax, ' '

    scan_s:        inc   cl
                   scasb
                   jne   scan_s

                   mov   bx, cx
                   dec   bx
                  
                   
    inv:           call  inverse
                   cmp   cl, [readchar]
                   jb    find_not_space
    fin:           ret
                        
    inverse:       
                   push  si
                   push  bx
    inv_loop:      mov   ah, buf[si]
                   mov   al, buf[bx]
                   mov   buf[bx], ah
                   mov   buf[si], al
                   inc   si
                   dec   bx
                   cmp   bx,si
                   jge   inv_loop
                   pop   bx
                   pop   si
                   ret

reverse_words endp

 

print_str proc
                   mov   ah, 09h
                   int   21h
                   ret
print_str endp

    maxchar        db    200
    readchar       db    0
    buf            db    204 dup (?)
    input_msg      db    "Input the string to be reversed:", 0Dh, 0Ah, '$'
    output_msg     db    0Ah,"String with reversed words:", 0Dh, 0Ah, '$'
end start