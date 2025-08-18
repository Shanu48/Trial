        AREA RESET, DATA, READONLY
        EXPORT __Vectors

__Vectors
        DCD 0x10001000          ; Initial Stack Pointer
        DCD Reset_Handler       ; Reset Handler Address

        ALIGN

        AREA mycode, CODE, READONLY
        ENTRY
        EXPORT Reset_Handler

Reset_Handler
        LDR R0, =N1             ; R0 -> first 128-bit number (in data section)
        LDR R1, =N2             ; R1 -> second 128-bit number (in data section)
        LDR R2, =Result         ; R2 -> result array (in data section)

		LDR R5,[R0],#4
		LDR R6,[R1],#4
		ADDS R7,R5,R6
		STR R7,[R2],#4
        MOV R3, #3              ; Loop count (4 words for 128-bit)
Loop
        LDR R5, [R0], #4        ; Load word from N1
        LDR R6, [R1], #4        ; Load word from N2
        ADCS R7, R5, R6         ; Add with carry
        STR R7, [R2], #4        ; Store result
        SUBS R3, R3, #1         ; Decrement count
        BNE Loop                ; Repeat if not zero

STOP
        B STOP                  ; Infinite loop
N1      DCD 0xFFFFFFFF, 0x2, 0x3, 0x4
N2      DCD 0x5, 0x6, 0x7, 0x8
        ; ?? Move N1, N2, Result here into DATA section
        AREA mydata, DATA, READWRITE


Result  DCD 0, 0, 0, 0          ; To store 128-bit result (LSB to MSB)

        END
