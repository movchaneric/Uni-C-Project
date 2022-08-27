;file test.as

.entry LOOP
.entry LENGTH
.extern L3
.extern W
 MAIN:	mov S1.1, W
        macro m1
        prn #-5
        sub r1,r4
        endmacro
 		add r2,STR
 LOOP:	jmp W
 		m1
 		inc K
macro macro2
mov r1,r5
endmacro
 		mov S1.2, r3
 		bne L3
END:    hlt
STR: 	.string "abcdef"
LENGTH: .data 6,-9,15
K: 	.data 22
 S1: 	.struct 8,"ab"
macro2

 ;end file