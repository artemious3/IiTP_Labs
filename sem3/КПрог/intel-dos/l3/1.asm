.model tiny
.code
    org 100h
start: mov ah, 9
       mov dx, offset msg
       int 21h
       ret
msg db "Hello, World!", 0Ah, 0Dh, "Hello, World, again!",  0Ah, 0Dh, "Hello, World last time.", 0Ah, 0Dh, '$'
end start


