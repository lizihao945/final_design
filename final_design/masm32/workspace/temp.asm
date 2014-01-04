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
.CODE
; PROCMARK	start_qs_swap	1	
start_qs_swap	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 4
; temps allocated between ebp-8 and ebp-4
; *** subroutine body ***
; ASSIGN	tmp	x	
	mov eax, dword ptr [ebp+24]
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-4], eax
; ASSIGN	x	y	
	mov eax, dword ptr [ebp+20]
	mov eax, dword ptr [eax]
	mov ebx, dword ptr [ebp+24]
	mov dword ptr [ebx], eax
; ASSIGN	y	tmp	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp+20]
	mov dword ptr [ebx], eax
; PROCEND	start_qs_swap	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_qs_swap	ENDP
; PROCMARK	start_qs	5	
start_qs	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 76
; temps allocated between ebp-24 and ebp-76
; *** subroutine body ***
; ASSIGN	sum	5	
	mov eax, dword ptr [ebp+24]
	mov dword ptr [eax], 5
; ADD+	r	l	
	mov eax, dword ptr [ebp+16]
	add eax, dword ptr [ebp+20]
	mov dword ptr [ebp-24], eax
; DIV/	t0	2	
	mov edx, 0
	mov eax, dword ptr [ebp-24]
	mov ebx, 2
	idiv ebx
	mov dword ptr [ebp-28], eax
; GETARRAY=[]	a	t1	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-48]
	imul ecx, dword ptr [ebp-28], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-32], eax
; ASSIGN	x	t2	
	mov eax, dword ptr [ebp-32]
	mov dword ptr [ebp-16], eax
; ASSIGN	i	l	
	mov eax, dword ptr [ebp+20]
	mov dword ptr [ebp-4], eax
; ASSIGN	j	r	
	mov eax, dword ptr [ebp+16]
	mov dword ptr [ebp-8], eax
; LABEL	0	N/A	
label0:
; LEQ<=	i	j	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp-8]
	cmp eax, ebx
	jg label6
; JMPF	6	t0	
; LABEL	1	N/A	
label1:
; GETARRAY=[]	a	i	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-48]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-36], eax
; LES<	t3	x	
	mov ebx, dword ptr [ebp-36]
	mov ecx, dword ptr [ebp-16]
	cmp ebx, ecx
	jge label2
; JMPF	2	t0	
; ADD+	i	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-40], eax
; ASSIGN	i	t4	
	mov eax, dword ptr [ebp-40]
	mov dword ptr [ebp-4], eax
; JMP	1	N/A	
	jmp label1
; LABEL	2	N/A	
label2:
; LABEL	3	N/A	
label3:
; GETARRAY=[]	a	j	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-48]
	imul ecx, dword ptr [ebp-8], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-44], eax
; GTR>	t5	x	
	mov ebx, dword ptr [ebp-44]
	mov ecx, dword ptr [ebp-16]
	cmp ebx, ecx
	jle label4
; JMPF	4	t0	
; SUB-	j	1	
	mov eax, dword ptr [ebp-8]
	sub eax, 1
	mov dword ptr [ebp-48], eax
; ASSIGN	j	t6	
	mov eax, dword ptr [ebp-48]
	mov dword ptr [ebp-8], eax
; JMP	3	N/A	
	jmp label3
; LABEL	4	N/A	
label4:
; LEQ<=	i	j	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp-8]
	cmp eax, ebx
	jg label5
; JMPF	5	t0	
; GETARRAY=[]	a	i	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-48]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	push eax
; GETARRAY=[]	a	j	
	mov ebx, dword ptr [ebp+8]
	lea eax, dword ptr [ebx-48]
	imul ecx, dword ptr [ebp-8], 4
	sub eax, ecx
	push eax
; CALL	start_qs_swap	t9	
	lea eax, dword ptr [ebp-60]
	push eax
	push dword ptr [ebp+8]
	push ebp
	call start_qs_swap
; ADD+	i	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-64], eax
; ASSIGN	i	t10	
	mov eax, dword ptr [ebp-64]
	mov dword ptr [ebp-4], eax
; SUB-	j	1	
	mov eax, dword ptr [ebp-8]
	sub eax, 1
	mov dword ptr [ebp-68], eax
; ASSIGN	j	t11	
	mov eax, dword ptr [ebp-68]
	mov dword ptr [ebp-8], eax
; LABEL	5	N/A	
label5:
; JMP	0	N/A	
	jmp label0
; LABEL	6	N/A	
label6:
; LES<	i	r	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp+16]
	cmp eax, ebx
	jge label7
; JMPF	7	t0	
; PARAMREF	sum	N/A	
	mov eax, dword ptr [ebp+24]
	lea eax, dword ptr [eax]
	push eax
; PARAMVAL	i	N/A	
	push dword ptr [ebp-4]
; PARAMVAL	r	N/A	
	push dword ptr [ebp+16]
; CALL	start_qs	t12	
	lea eax, dword ptr [ebp-72]
	push eax
	push dword ptr [ebp+8]
	call start_qs
; LABEL	7	N/A	
label7:
; GTR>	j	l	
	mov eax, dword ptr [ebp-8]
	mov ebx, dword ptr [ebp+20]
	cmp eax, ebx
	jle label8
; JMPF	8	t0	
; PARAMREF	sum	N/A	
	mov eax, dword ptr [ebp+24]
	lea eax, dword ptr [eax]
	push eax
; PARAMVAL	l	N/A	
	push dword ptr [ebp+20]
; PARAMVAL	j	N/A	
	push dword ptr [ebp-8]
; CALL	start_qs	t13	
	lea eax, dword ptr [ebp-76]
	push eax
	push dword ptr [ebp+8]
	call start_qs
; LABEL	8	N/A	
label8:
; PROCEND	start_qs	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_qs	ENDP
; PROCMARK	start	26	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 148
; temps allocated between ebp-108 and ebp-148
; *** subroutine body ***
; ASSIGN	n	9	
	mov dword ptr [ebp-96], 9
; SETARRAY[]=	a	1	
	lea eax, dword ptr [ebp-48]
	sub eax, 4
	mov dword ptr [eax], 2
; SETARRAY[]=	a	2	
	lea eax, dword ptr [ebp-48]
	sub eax, 8
	mov dword ptr [eax], 3
; SETARRAY[]=	a	3	
	lea eax, dword ptr [ebp-48]
	sub eax, 12
	mov dword ptr [eax], 1
; SETARRAY[]=	a	4	
	lea eax, dword ptr [ebp-48]
	sub eax, 16
	mov dword ptr [eax], 8
; SETARRAY[]=	a	5	
	lea eax, dword ptr [ebp-48]
	sub eax, 20
	mov dword ptr [eax], 6
; SETARRAY[]=	a	6	
	lea eax, dword ptr [ebp-48]
	sub eax, 24
	mov dword ptr [eax], 5
; SETARRAY[]=	a	7	
	lea eax, dword ptr [ebp-48]
	sub eax, 28
	mov dword ptr [eax], 0
; SETARRAY[]=	a	8	
	lea eax, dword ptr [ebp-48]
	sub eax, 32
	mov dword ptr [eax], 9
; SETARRAY[]=	a	9	
	lea eax, dword ptr [ebp-48]
	sub eax, 36
	mov dword ptr [eax], 4
; PARAMREF	sum	N/A	
	lea eax, dword ptr [ebp-100]
	push eax
; PARAMVAL	1	N/A	
	push 1
; PARAMVAL	n	N/A	
	push dword ptr [ebp-96]
; CALL	start_qs	t0	
	lea eax, dword ptr [ebp-108]
	push eax
	push ebp
	call start_qs
; ASSIGN	i	1	
	mov dword ptr [ebp-92], 1
; LABEL	9	N/A	
label9:
; LEQ<=	i	n	
	mov eax, dword ptr [ebp-92]
	mov ebx, dword ptr [ebp-96]
	cmp eax, ebx
	jg label10
; JMPF	10	t0	
; SETARRAY[]=	b	i	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-92], 4
	sub eax, ecx
	mov dword ptr [eax], 1
; INC	i	N/A	
	inc dword ptr [ebp-92]
; JMP	9	N/A	
	jmp label9
; LABEL	10	N/A	
label10:
; ASSIGN	i	1	
	mov dword ptr [ebp-92], 1
; LABEL	11	N/A	
label11:
; SUB-	n	1	
	mov eax, dword ptr [ebp-96]
	sub eax, 1
	mov dword ptr [ebp-112], eax
; LEQ<=	i	t1	
	mov eax, dword ptr [ebp-92]
	mov ebx, dword ptr [ebp-112]
	cmp eax, ebx
	jg label13
; JMPF	13	t0	
; GETARRAY=[]	a	i	
	lea eax, dword ptr [ebp-48]
	imul ecx, dword ptr [ebp-92], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-116], eax
; ADD+	i	1	
	mov eax, dword ptr [ebp-92]
	add eax, 1
	mov dword ptr [ebp-120], eax
; GETARRAY=[]	a	t3	
	lea eax, dword ptr [ebp-48]
	imul ecx, dword ptr [ebp-120], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-124], eax
; EQL=	t2	t4	
	mov eax, dword ptr [ebp-116]
	mov ebx, dword ptr [ebp-124]
	cmp eax, ebx
	jne label12
; JMPF	12	t0	
; ADD+	i	1	
	mov eax, dword ptr [ebp-92]
	add eax, 1
	mov dword ptr [ebp-128], eax
; GETARRAY=[]	b	i	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-92], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-132], eax
; ADD+	i	1	
	mov eax, dword ptr [ebp-92]
	add eax, 1
	mov dword ptr [ebp-136], eax
; GETARRAY=[]	b	t7	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-136], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-140], eax
; ADD+	t6	t8	
	mov eax, dword ptr [ebp-132]
	add eax, dword ptr [ebp-140]
	mov dword ptr [ebp-144], eax
; SETARRAY[]=	b	t5	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-128], 4
	sub eax, ecx
	mov ebx, dword ptr [ebp-144]
	mov dword ptr [eax], ebx
; SETARRAY[]=	b	i	
	lea eax, dword ptr [ebp-4]
	imul ecx, dword ptr [ebp-92], 4
	sub eax, ecx
	mov dword ptr [eax], 0
; LABEL	12	N/A	
label12:
; INC	i	N/A	
	inc dword ptr [ebp-92]
; JMP	11	N/A	
	jmp label11
; LABEL	13	N/A	
label13:
; ASSIGN	i	1	
	mov dword ptr [ebp-92], 1
; LABEL	14	N/A	
label14:
; LEQ<=	i	n	
	mov eax, dword ptr [ebp-92]
	mov ebx, dword ptr [ebp-96]
	cmp eax, ebx
	jg label15
; JMPF	15	t0	
; GETARRAY=[]	a	i	
	lea eax, dword ptr [ebp-48]
	imul ecx, dword ptr [ebp-92], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-148], eax
; WRITE	t10	N/A	
	push dword ptr [ebp-148]
	push offset OneInt
	call crt_printf
; INC	i	N/A	
	inc dword ptr [ebp-92]
; JMP	14	N/A	
	jmp label14
; LABEL	15	N/A	
label15:
; WRITE	sum	N/A	
	push dword ptr [ebp-100]
	push offset OneInt
	call crt_printf
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset WriteLine
	call crt_printf
	call ExitProcess
start	ENDP
END
