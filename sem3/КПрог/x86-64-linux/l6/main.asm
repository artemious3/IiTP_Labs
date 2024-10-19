include 'linux.inc'
format ELF64 executable 3
ARG_MIN_COUNT = 3
PID_MAX_CHAR = 9
segment readable executable

atoi:
         ; rsi - string pointer; 
                 xor  rbx, rbx  
                 xor  rax, rax                         

    .atoiloop:    
                 lodsb
                 test al, al
                 jz .ok
                 sub al, '0'   
                 cmp al, 0
                 jl .wrong
                 cmp al, 9
                 jg .wrong 

                 imul rbx, 10
                 jc .wrong
                 add rbx, rax 
                 jc .wrong
                 jmp .atoiloop
                  
    .wrong:     
                 stc
                 ret
    .ok:         
                 mov rax, rbx
                 clc
                 ret 


; pid_to_str:  xor  bh,bh
;              mov  bl, PID_MAX_CHAR
;              dec  bx
;     lp:      xor  dx, dx
;              mov  cx, 10
;              div  rax, 10
;              add  dl, '0'
;              mov  pidstr[bx], dl
;              cmp  bx, 0
;              dec  bx
;              jge  lp
;              ret

entry $

   process_args:  
                  ; ARGC
                  pop rax
                  cmp rax, ARG_MIN_COUNT
                  jb wrong_argc
                  
                  ; ARGV[0] - CURRENT PROGRAM NAME
                  pop rax           

                  ; ARGV[1] - PROGRAM TO EXECUTE
                  pop rax
                  mov [program_name_ptr], rax


                 ; ARGV[2] - NUMBER OF REPEATS
                  pop rsi
                  call atoi
                  mov [repnum], rax
                  jc wrong_argc
            
                  ; we will pass all other arguments to child process
                  ; by convention argv[0] is program name
                  mov rax, [program_name_ptr]
                  push rax
                  mov [child_argv], rsp

                  mov rcx, [repnum]
    fork:
                  push rcx

                  syscall sys_fork
                  test rax, rax
                  jz exec
                  cmp rax, -1
                  je error_fork

                
                  syscall sys_waitid, 0, 0, 0, 0x4
                  ;;still does not work
                  
                  pop rcx

                  loop fork
                  
                 jmp normal_exit

    exec:         
                
                syscall sys_execve, [program_name_ptr], [child_argv], 0

                ; if reached here, could not perform exec
                jmp exec_err
                

    wrong_argc:  syscall sys_write, STDOUT, usage_str, usage_sz
                 jmp error_exit       

    error_fork:  syscall sys_write, STDOUT, err_str, err_sz
                 jmp error_exit

    exec_err:    syscall sys_write, STDOUT, exec_err_str, exec_err_str_sz
                 jmp error_exit


    error_exit:  syscall sys_exit, 1
    normal_exit: syscall sys_exit, 0

    
segment readable writable

    usage_str db "Usage: ./main <executable> <number> [args]", 0xA
    usage_sz = $-usage_str

    err_str db "Error occured while starting new process", 0xA
    err_sz = $-err_str

    exec_err_str db "Unable to execute given process", 0xA
    exec_err_str_sz = $-exec_err_str


    argc dq ? 
    pidstr db 0 dup 9

    program_name_ptr dq ?
    repnum dq ?
    child_argv dq ?

    _status dq ?
    

    
  
