vga_size = 80*25
vga_seg16 = 0b800h
vga_ptr32 = 0b8000h

format binary
org 7C00h                       
use16
boot:                           
        cli                     
        lgdt fword[cs:gdt.size] 
        mov eax,cr0             
        or al,1                 
        mov cr0,eax             
        jmp gdt.code:pmode     

back_rmode:
				mov ax, vga_seg16
				mov ds, ax
				mov word[ds:0h], 0x2F52	 ;R
				mov word[ds:2h], 0x2F4D  ;M
				jmp $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
						use32            ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;7c7c
pmode:                         
        mov ax,gdt.data         
        mov ds,ax               

        call main           

				;jmp to segment assuming 16bit addresses
				jmp gdt.code_before_rm:@f
@@:
				cli
        mov eax,cr0             
        and al, 11111110b                 
        mov cr0,eax             
        jmp 00h:back_rmode                   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gdt:    dw 0      ; entry 0 (pseudo segment for lgdt)
.size   dw gdt_end-gdt-1   
.linear dd gdt           

.code=$-gdt            
dw 0FFFFH,0       ; entry 1 (data in PM)
db 0,9ah,0cfh,0  

.data=$-gdt       ; entry 2 (code in PM)
dw 0FFFFH,0          
db 0,92h,0cfh,0  

.code_before_rm=$-gdt    ; entry 3 (before go back in RM)
dw 0FFFFH,0
db 0,9ah,000h,0  
gdt_end:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

main:                             
				mov esi, vga_ptr32
				mov ecx, vga_size
	@@:
				mov word [ds:esi], 0
				add si, 2
				loop @b

				mov ecx, msglen
				mov esi, 0b8000h + msgcenter
				mov edi, msg
	@@:
				mov al, [edi]
				mov [esi], al
				mov byte [esi+1], 0x0F
				add esi, 2
				add edi, 1
				loop @b
        ret  

msg: 
	db 'Hello from protected mode!'
	msglen = $ - msg
	msgcenter = (12*80 + 40)*2 - (msglen*2)/2;
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
free =  510-(padding-$$)        
padding rb free                 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        dw 0aa55h               
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
