

 BASE    EQU  10
 MAXCHAR_C   EQU 6
.model tiny
.code
                 org  100h

    start:       mov  dx, offset inp_str
                 call print
                 call read_str
                 call atoi
                 cmp  dx, 0
                 jne wrong_inp
                 mov  [num1], ax

                 mov  dx, offset inp_str
                 call print
                 call read_str
                 call atoi
                 cmp  dx, 0
                 jne wrong_inp
                 mov  [num2], ax

                 

                 mov  dx, offset newln
                 call print

                 mov  ax, [num1]
                 mov  bx, [num2]
                 and  ax, bx
                 call itostr
                 mov  dx, offset and_str
                 call print
                 mov  dx, offset outbuf
                 call print

                 mov  ax, [num1]
                 mov  bx, [num2]
                 or   ax, bx
                 call itostr
                 mov  dx, offset or_str
                 call print
                 mov  dx, offset outbuf
                 call print

                 mov  ax, [num1]
                 mov  bx, [num2]
                 xor  ax, bx
                 call itostr
                 mov  dx, offset xor_str
                 call print
                 mov  dx, offset outbuf
                 call print

    fin:         ret


    wrong_inp:   mov  dx, offset wr_str
                 call print
                 ret
          

    print:       mov  ah, 09h
                 int  21h
                 ret

    read_str:    
                 mov  ah, 0Ah
                 mov  dx, offset maxchar
                 int  21h

                 xor  bh, bh
                 mov  bl, [readchar]
                 mov  inpbuf[bx], '$'
                 ret

    atoi:                                                 ; PUTS inpbuf INTO AX AND 0 IN DX IF OK
                 xor  bx, bx
                 xor  ax, ax
                 xor  dx, dx
                 mov  cx, base                            ; cx = 10

    atoiloop:    
                 mul  cx
                 jc   wrong
                 mov  dl, inpbuf[bx]
                 sub  dl, '0'                             ; dl = current number ;

                 cmp  dl, 0
                 jl   wrong
                 cmp  dl, 9
                 jg   wrong

                 add  ax, dx
                 jc   wrong                               ; ax += dl
                 inc  bl
                 cmp  bl, [readchar]
                 jl   atoiloop
                 jmp  ok
    wrong:       mov  dx, 1
                 ret
    ok:          xor  dx, dx
                 ret


    itostr:      xor  bh,bh
                 mov  bl, [maxchar]
                 dec  bx
    lp:          xor  dx, dx
                 mov  cx, 10
                 div  cx
                 add  dl, '0'
                 mov  outbuf[bx], dl
                 cmp  bx, 0
                 dec  bx
                 jge  lp
                 ret
                    

    maxchar      DB   MAXCHAR_C
    readchar     DB   (?)
    inpbuf       DB   MAXCHAR_C DUP (?)

    outbuf_newln DB   0Ah, 0Dh
    outbuf       DB   MAXCHAR_C DUP (?), 0Ah, 0Dh, '$'

    inp_str      DB   0Ah, 0Dh, "Input number:$"
    wr_str       DB   0Ah, 0Dh, "Wrong input", 0Ah, 0Dh, '$'

    and_str      DB   "AND: $"
    or_str       DB   "OR : $"
    xor_str      DB   "XOR: $"

    newln        DB   0Ah, 0Dh, '$'

    num1         DW   (?)
    num2         Dw   (?)

end startclear