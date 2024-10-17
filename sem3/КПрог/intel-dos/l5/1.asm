    .model small
    .stack 256d

    BUFSIZE             EQU   127
    INT16MAXLEN         EQU   5
    FILENAME_MAXLEN EQU 10
.code

                         org   100h

    start:               
                         call  open_arg_file
                         call  count_ne_str_in_file

                         mov   dx, offset result_str
                         call  print
                        
                         mov   ax, [counter]
                         call  itostr
                         mov   dx, offset counter_str
                         call  print
                         
                         call exit
                       
    ;-------------------------------------------------

open_arg_file proc near
                         mov   bp,sp
                         mov   cl, ds:[80h]
                         cmp   cl, 1d
                         jbe   no_file
                         cmp   cl, FILENAME_MAXLEN
                         jae   long_fname

                         lea   si, [ds:82h]
                         mov   di, offset filename

    find_fname:          
                         lodsb
                         stosb
                         loop  find_fname

                         mov   byte ptr [di-1], 0h

                         mov   si, di
                         mov   di, 0
    open_file:           mov   ah, 3Dh
                         mov   al, 010B
                         mov   dx, offset filename
                         int   21h

                         jc    file_err

                         mov   bx, DGROUP
                         mov   ds, bx
                         mov   file_descriptor, ax
                         ret

    file_err:            
                         mov   dx, offset err_str
                         call  print
                         call  exit

    no_file:             
                         mov   dx, offset no_file_err
                         call  print
                         call  exit
    long_fname:          
                         mov   dx, offset long_fname_str
                         call  print
                         call  exit
open_arg_file endp

count_ne_str_in_buf proc near
                         cld                                       ; count nonempty strings. cx is size of string
                         mov   ax,  DGROUP
                         mov   es, ax
                         mov   di, offset buf
                         mov   ax, 0Ah                             ; ascii code for '\n'
                         
    scan:                repne scasb
                         je    reached_newline
                         jmp   finalize_scan

    reached_newline:     dec   di
                         mov   bl, byte ptr [di-1]
                         cmp   byte ptr [di], bl
                         je    increment_counter
                         jmp   continue_scan

    increment_counter:   inc   counter

    continue_scan:       inc   di
                         test  cx, cx
                         jz    finalize_scan
                         jmp   scan

    finalize_scan:       dec   di
                         cmp   byte ptr [di], 0Ah
                         je    prebuf_newln
                         jmp   prebuf_reset
    prebuf_newln:        mov   [_prebuf_newln], 0Ah
                         ret
    prebuf_reset:        mov   [_prebuf_newln], 0h
                         ret
                        
count_ne_str_in_buf endp



count_ne_str_in_file proc
                         mov   [last_newln_pos], (offset buf)-1
    readbuf:             mov   ax, DGROUP
                         mov   ds, ax
                         mov   bx, ds:file_descriptor
                         mov   ah, 3Fh
                         xor   al, al
                         mov   cx, BUFSIZE
                         mov   dx, offset buf
                         int   21h
                         
                         jc    error                               ; read BUFSIZE bytes to buffer
                    
                         mov   cx, ax
                         push  cx
                         call  count_ne_str_in_buf
                         pop   cx
                         cmp   cx, BUFSIZE
                         je    readbuf
                         ret

    error:               
                         mov   dx, offset buf_err_str
                         call  print
                         call  exit

count_ne_str_in_file endp


print proc FAR
                         mov   ax, DGROUP
                         mov   ds, ax
                         mov   ah, 9h
                         int   21h
                         ret
print endp

exit proc FAR
                         mov   ax, 4C00h
                         int   21h
exit endp

itostr proc
    beg:                 xor   bh,bh
                         mov   bl, INT16MAXLEN
                         dec   bx
    lp:                  xor   dx, dx
                         mov   cx, 10
                         div   cx
                         add   dl, '0'
                         mov   counter_str[bx], dl
                         cmp   bx, 0
                         dec   bx
                         jge   lp
                         ret
itostr endp
    ;------------------------------------------------
.data
    filename        DB 16 DUP (?)

    _prebuf_newln   DB 0Ah
    buf             DB BUFSIZE DUP (?), 0

    last_newln_pos  DW -1

    file_descriptor DW (?)

    counter         DW (?)
    counter_str     DB INT16MAXLEN DUP (?), '$'

    result_str      DB "Number of empty lines in given file:$"
    buf_err_str     DB "Error reading buffer", 0Ah, 0Dh, '$'
    long_fname_str  DB "Too long file name", 0Ah, 0Dh, '$'
    no_file_err     DB "No file specified",0Ah, 0Dh, '$'
    err_str         DB "Unable to open file", 0Ah, 0Dh, '$'
    end start   