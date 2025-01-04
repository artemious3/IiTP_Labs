include 'linux.inc'
format ELF64 executable 3

ARG_MIN_COUNT = 3
PID_MAX_CHAR = 9
MAX_NUMBER_OF_FORK = 255

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


pid_to_str:  
	mov rcx, 10
	xor rdx, rdx
	mov rbx, PID_MAX_CHAR-1
.lp:
	div rcx
	test rax, rax
	jz  .fin
	add dl, '0'
	mov  byte[pidstr+rbx], dl
	xor rdx, rdx
	dec rbx
	jmp .lp
.fin:
	add dl, '0'
	mov  byte[pidstr+rbx], dl
	ret

entry $
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
	jc wrong_argc
	cmp rax, MAX_NUMBER_OF_FORK
	ja wrong_argc
	test rax, rax
	jz wrong_argc
	mov [repnum], rax

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


	
	call pid_to_str
	syscall sys_write, STDOUT, fork_str, fork_str_sz
	syscall sys_waitid, 0, 0, 0, 0x4

	pop rcx
	loop fork

	jmp normal_exit

;---------------------------------------
exec:         
	syscall sys_execve, [program_name_ptr], [child_argv], 0
	; if reached here, could not perform exec
	jmp exec_err


wrong_argc: 
	syscall sys_write, STDOUT, usage_str, usage_sz
	jmp error_exit       

error_fork:
	syscall sys_write, STDOUT, err_str, err_sz
	jmp error_exit

exec_err:
	syscall sys_write, STDOUT, exec_err_str, exec_err_str_sz
	jmp error_exit

error_exit:  syscall sys_exit, 1
normal_exit: syscall sys_exit, 0

segment readable writable

	usage_str db "Usage: ./main <executable> <number:1-255> [args]", 0xA
	usage_sz = $-usage_str

	err_str db "Error occured while starting new process", 0xA
	err_sz = $-err_str

	exec_err_str db "Unable to execute given process", 0xA
	exec_err_str_sz = $-exec_err_str


	fork_str db 0x1B, "[30;42m", "Spawned child process with PID : " 
	pidstr db PID_MAX_CHAR dup 0 
	db 0x1B, "[0m", 0xA
	fork_str_sz = $ - fork_str


	argc dq ? 

	program_name_ptr dq ?
	repnum dq ?
	child_argv dq ?

	_status dq ?




