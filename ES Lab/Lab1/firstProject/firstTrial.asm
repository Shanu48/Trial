	AREA RESET,DATA,READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	ALIGN
	AREA mycoode,CODE,READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	MOV R0,#10
	MOV R1,#0x10
	MOV R2,#2_1010
	MOV R3,#4_23
	MOV R4,#-1
STOP B STOP
	END