
           .align      4
           .arm
           .section    .text
           .global     entry
entry:
		ldr     r0, =main
        blx     r0
        .byte	 0,  1,  2,  3
        .byte	 4,  5,  6,  7
