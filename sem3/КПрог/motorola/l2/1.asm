 org $8000

 ldx #%0000000011111111
 ldd #%0011110000111100 ; load D and X

 coma
 comb ; invert D, making a bitmask of cleared bits

 staa $00
 stab $01 ; load D into memory

 xgdx ; load X into D

 eora $00
 eorb $01 ; invert bits, that were zero at start

 xgdx ; load D into X

 wai ; end of program

 

 

