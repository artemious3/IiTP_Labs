 org $8000
 
 ldx #20
 ldy #30
 ldd #10 ; loaded sample data into stacks

 stx $00
 sty $02
 subd $00
 subd $02 ; loaded X and Y into memory and subtracted them from D

 psha
 pshb ; pushed D into stack

 subd #3 ; subtracted 3 from D4
 
 
  	
 