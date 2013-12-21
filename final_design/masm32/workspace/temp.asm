.386
.model flat, stdcall

include \masm32\include\kernel32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\msvcrt.lib
includelib \masm32\lib\kernel32.lib

.data
String db "%s", 0
OneChar db "%c", 0
OneInt db "%d", 0
WriteLine db 0ah, 0
string0 db " The ASCII of c1 is: ", 0
string1 db " The letter you input is out of lowercase's range! ", 0
string2 db " The ASCII of c1 is: ", 0
string3 db " The letter you input is out of uppercase's range! ", 0
string4 db " The letter you input is invalid! ", 0
string5 db " The result is: ", 0
.CODE
; PROCMARK	start	7	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 84
; temps allocated between ebp-32 and ebp-84
; *** subroutine body ***
; ASSIGN	i	1	
	mov dword ptr [ebp-28], 1
; LABEL	0	N/A	
label0:
; LEQ<=	i	2	
	mov eax, dword ptr [ebp-28]
	mov ebx, 2
	cmp eax, ebx
	jg label13
; JMPF	13	t0	
; EQL=	i	1	
	mov eax, dword ptr [ebp-28]
	mov ebx, 1
	cmp eax, ebx
	jne label1
; JMPF	1	t0	
; ASSIGN	c1	w	
	mov eax, 119
	mov dword ptr [ebp-4], eax
; ASSIGN	c2	e	
	mov eax, 101
	mov dword ptr [ebp-8], eax
; ASSIGN	i1	1	
	mov dword ptr [ebp-12], 1
; ASSIGN	i2	2	
	mov dword ptr [ebp-16], 2
; LABEL	1	N/A	
label1:
; EQL=	i	2	
	mov eax, dword ptr [ebp-28]
	mov ebx, 2
	cmp eax, ebx
	jne label2
; JMPF	2	t0	
; ASSIGN	c1	F	
	mov eax, 70
	mov dword ptr [ebp-4], eax
; ASSIGN	c2	II	
	mov eax, 73
	mov dword ptr [ebp-8], eax
; ASSIGN	i1	3	
	mov dword ptr [ebp-12], 3
; ASSIGN	i2	5	
	mov dword ptr [ebp-16], 5
; LABEL	2	N/A	
label2:
; GEQ>=	c1	a	
	mov eax, dword ptr [ebp-4]
	mov ebx, 97
	cmp eax, ebx
	jl label5
; JMPF	5	t0	
; LEQ<=	c1	z	
	mov eax, dword ptr [ebp-4]
	mov ebx, 122
	cmp eax, ebx
	jg label3
; JMPF	3	t0	
; SUB-	c1	a	
	mov eax, dword ptr [ebp-4]
	sub eax, 97
	mov dword ptr [ebp-32], eax
; ADD+	t0	97	
	mov eax, dword ptr [ebp-32]
	add eax, 97
	mov dword ptr [ebp-36], eax
; ASSIGN	ordc1	t1	
	mov eax, dword ptr [ebp-36]
	mov dword ptr [ebp-24], eax
; WRITE	 The ASCII of c1 is: 	ordc1	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-24]
	push offset OneInt
	call crt_printf
; JMP	4	N/A	
	jmp label4
; LABEL	3	N/A	
label3:
; WRITE	 The letter you input is out of lowercase's range! 	N/A	
	push offset string1
	push offset String
	call crt_printf
; LABEL	4	N/A	
label4:
; JMP	10	N/A	
	jmp label10
; LABEL	5	N/A	
label5:
; LEQ<=	c1	Z	
	mov eax, dword ptr [ebp-4]
	mov ebx, 90
	cmp eax, ebx
	jg label8
; JMPF	8	t0	
; GEQ>=	c1	A	
	mov eax, dword ptr [ebp-4]
	mov ebx, 65
	cmp eax, ebx
	jl label6
; JMPF	6	t0	
; SUB-	c1	A	
	mov eax, dword ptr [ebp-4]
	sub eax, 65
	mov dword ptr [ebp-40], eax
; ADD+	t2	65	
	mov eax, dword ptr [ebp-40]
	add eax, 65
	mov dword ptr [ebp-44], eax
; ASSIGN	ordc1	t3	
	mov eax, dword ptr [ebp-44]
	mov dword ptr [ebp-24], eax
; WRITE	 The ASCII of c1 is: 	ordc1	
	push offset string2
	push offset String
	call crt_printf
	push dword ptr [ebp-24]
	push offset OneInt
	call crt_printf
; JMP	7	N/A	
	jmp label7
; LABEL	6	N/A	
label6:
; WRITE	 The letter you input is out of uppercase's range! 	N/A	
	push offset string3
	push offset String
	call crt_printf
; LABEL	7	N/A	
label7:
; JMP	9	N/A	
	jmp label9
; LABEL	8	N/A	
label8:
; WRITE	 The letter you input is invalid! 	N/A	
	push offset string4
	push offset String
	call crt_printf
; LABEL	9	N/A	
label9:
; LABEL	10	N/A	
label10:
; LEQ<=	c1	c2	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp-8]
	cmp eax, ebx
	jg label11
; JMPF	11	t0	
; MULT*	a	i1	
	mov eax, 97
	imul eax, dword ptr [ebp-12]
	mov dword ptr [ebp-48], eax
; MULT*	m	i2	
	mov eax, 109
	imul eax, dword ptr [ebp-16]
	mov dword ptr [ebp-52], eax
; ADD+	t4	t5	
	mov eax, dword ptr [ebp-48]
	add eax, dword ptr [ebp-52]
	mov dword ptr [ebp-56], eax
; ADD+	a	m	
	mov eax, 97
	add eax, 109
	mov dword ptr [ebp-60], eax
; DIV/	t6	t7	
	mov edx, 0
	mov eax, dword ptr [ebp-56]
	mov ebx, dword ptr [ebp-60]
	idiv ebx
	mov dword ptr [ebp-64], eax
; ASSIGN	i3	t8	
	mov eax, dword ptr [ebp-64]
	mov dword ptr [ebp-20], eax
; JMP	12	N/A	
	jmp label12
; LABEL	11	N/A	
label11:
; MULT*	a	i2	
	mov eax, 97
	imul eax, dword ptr [ebp-16]
	mov dword ptr [ebp-68], eax
; MULT*	m	i1	
	mov eax, 109
	imul eax, dword ptr [ebp-12]
	mov dword ptr [ebp-72], eax
; ADD+	t9	t10	
	mov eax, dword ptr [ebp-68]
	add eax, dword ptr [ebp-72]
	mov dword ptr [ebp-76], eax
; SUB-	a	m	
	mov eax, 97
	sub eax, 109
	mov dword ptr [ebp-80], eax
; DIV/	t11	t12	
	mov edx, 0
	mov eax, dword ptr [ebp-76]
	mov ebx, dword ptr [ebp-80]
	idiv ebx
	mov dword ptr [ebp-84], eax
; ASSIGN	i3	t13	
	mov eax, dword ptr [ebp-84]
	mov dword ptr [ebp-20], eax
; LABEL	12	N/A	
label12:
; WRITE	 The result is: 	i3	
	push offset string5
	push offset String
	call crt_printf
	push dword ptr [ebp-20]
	push offset OneInt
	call crt_printf
; INC	i	N/A	
	inc dword ptr [ebp-28]
; JMP	0	N/A	
	jmp label0
; LABEL	13	N/A	
label13:
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
