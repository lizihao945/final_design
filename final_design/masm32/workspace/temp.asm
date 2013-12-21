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
string0 db " cocks: ", 0
string1 db " rabbits: ", 0
string2 db " cocks: ", 0
string3 db " rabbits: ", 0
.CODE
; PROCMARK	start_calculation	3	
start_calculation	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 32
; temps allocated between ebp-16 and ebp-32
; *** subroutine body ***
; ASSIGN	n	0	
	mov dword ptr [ebp-4], 0
; ASSIGN	cock	1	
	mov dword ptr [ebp-8], 1
; LABEL	0	N/A	
label0:
; LEQ<=	cock	head	
	mov eax, dword ptr [ebp-8]
	mov ebx, dword ptr [ebp+16]
	mov ecx, dword ptr [ebx]
	cmp eax, ecx
	jg label2
; JMPF	2	t0	
; SUB-	head	cock	
	mov eax, dword ptr [ebp+16]
	mov eax, dword ptr [eax]
	sub eax, dword ptr [ebp-8]
	mov dword ptr [ebp-16], eax
; ASSIGN	rabbit	t0	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-12], eax
; MULT*	cock	2	
	mov eax, dword ptr [ebp-8]
	imul eax, 2
	mov dword ptr [ebp-20], eax
; MULT*	rabbit	4	
	mov eax, dword ptr [ebp-12]
	imul eax, 4
	mov dword ptr [ebp-24], eax
; ADD+	t1	t2	
	mov eax, dword ptr [ebp-20]
	add eax, dword ptr [ebp-24]
	mov dword ptr [ebp-28], eax
; EQL=	t3	foot	
	mov eax, dword ptr [ebp-28]
	mov ebx, dword ptr [ebp+12]
	mov ecx, dword ptr [ebx]
	cmp eax, ecx
	jne label1
; JMPF	1	t0	
; WRITE	 cocks: 	cock	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-8]
	push offset OneInt
	call crt_printf
; WRITE	 rabbits: 	rabbit	
	push offset string1
	push offset String
	call crt_printf
	push dword ptr [ebp-12]
	push offset OneInt
	call crt_printf
; ADD+	n	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-32], eax
; ASSIGN	n	t4	
	mov eax, dword ptr [ebp-32]
	mov dword ptr [ebp-4], eax
; LABEL	1	N/A	
label1:
; INC	cock	N/A	
	inc dword ptr [ebp-8]
; JMP	0	N/A	
	jmp label0
; LABEL	2	N/A	
label2:
; EQL=	n	0	
	mov eax, dword ptr [ebp-4]
	mov ebx, 0
	cmp eax, ebx
	jne label3
; JMPF	3	t0	
; WRITE	 cocks: 	0	
	push offset string2
	push offset String
	call crt_printf
	push 0
	push offset OneInt
	call crt_printf
; WRITE	 rabbits: 	0	
	push offset string3
	push offset String
	call crt_printf
	push 0
	push offset OneInt
	call crt_printf
; LABEL	3	N/A	
label3:
; PROCEND	start_calculation	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_calculation	ENDP
; PROCMARK	start	12	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 56
; temps allocated between ebp-52 and ebp-56
; *** subroutine body ***
; ASSIGN	i	0	
	mov dword ptr [ebp-36], 0
; LABEL	4	N/A	
label4:
; LEQ<=	i	2	
	mov eax, dword ptr [ebp-36]
	mov ebx, 2
	cmp eax, ebx
	jg label8
; JMPF	8	t0	
; EQL=	i	0	
	mov eax, dword ptr [ebp-36]
	mov ebx, 0
	cmp eax, ebx
	jne label5
; JMPF	5	t0	
; ASSIGN	head	4	
	mov dword ptr [ebp-40], 4
; ASSIGN	foot	8	
	mov dword ptr [ebp-44], 8
; LABEL	5	N/A	
label5:
; EQL=	i	1	
	mov eax, dword ptr [ebp-36]
	mov ebx, 1
	cmp eax, ebx
	jne label6
; JMPF	6	t0	
; ASSIGN	head	8	
	mov dword ptr [ebp-40], 8
; ASSIGN	foot	4	
	mov dword ptr [ebp-44], 4
; LABEL	6	N/A	
label6:
; EQL=	i	2	
	mov eax, dword ptr [ebp-36]
	mov ebx, 2
	cmp eax, ebx
	jne label7
; JMPF	7	t0	
; ASSIGN	head	3	
	mov dword ptr [ebp-40], 3
; ASSIGN	foot	5	
	mov dword ptr [ebp-44], 5
; LABEL	7	N/A	
label7:
; SETARRAY[]=	headarray	i	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-36], 4
	sub eax, ecx
	mov ebx, dword ptr [ebp-40]
	mov dword ptr [eax], ebx
; SETARRAY[]=	footarray	i	
	lea eax, dword ptr [ebp-20]
	imul ecx, dword ptr [ebp-36], 4
	sub eax, ecx
	mov ebx, dword ptr [ebp-44]
	mov dword ptr [eax], ebx
; GETARRAY=[]	headarray	i	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-36], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-52], eax
; PARAMREF	t0	N/A	
	lea eax, dword ptr [ebp-52]
	push eax
; GETARRAY=[]	footarray	i	
	lea eax, dword ptr [ebp-20]
	imul ecx, dword ptr [ebp-36], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-56], eax
; PARAMREF	t1	N/A	
	lea eax, dword ptr [ebp-56]
	push eax
; CALL	start_calculation	3	
	push ebp
	call start_calculation
; INC	i	N/A	
	inc dword ptr [ebp-36]
; JMP	4	N/A	
	jmp label4
; LABEL	8	N/A	
label8:
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
