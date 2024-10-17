  org $8000
 ldx #$8220
 ldab #$20

write stab 0,x   ; loading test data into memory
       dex
       decb
       bpl write ; if b is positive, go again


       ldx #$8220
      ldy #$0020

copy  brset 0,x,#%00000100 iter ; if bit 3 is set, skip
      ldaa 0,x
      staa 0,y
iter  dex
      dey
      bne copy ; while y is nonzero

      brset 0,x,#%00000100 end  ; process address 0
      ldaa 0,x
      staa 0,y ; copyhte
end   wai