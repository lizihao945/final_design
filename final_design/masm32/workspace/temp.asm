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
string0 db " ", 0
string1 db "Enter n:", 0
string2 db "Enter a[i]:", 0
string3 db "before:\n", 0
string4 db "after:", 0
.CODE
; PROCMARK	start_printarray	1	
start_printarray	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 16
; temps allocated between ebp-8 and ebp-16
; *** subroutine body ***
; ASSIGN	i	1	
	mov dword ptr [ebp-4], 1
; LABEL	0	N/A	
label0:
; LEQ<=	i	n	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp+8]
	mov ecx, dword ptr [ebx-64]
	cmp eax, ecx
	jg label1
; JMPF	1	t0	
; GETARRAY=[]	a	i	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-12], eax
; WRITE	 	t1	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-12]
	push offset OneInt
	call crt_printf
; ADD+	i	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-16], eax
; ASSIGN	i	t2	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-4], eax
; JMP	0	N/A	
	jmp label0
; LABEL	1	N/A	
label1:
; PROCEND	start_printarray	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_printarray	ENDP
; PROCMARK	start_initialize	2	
start_initialize	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 16
; temps allocated between ebp-12 and ebp-16
; *** subroutine body ***
; ASSIGN	i	1	
	mov dword ptr [ebp-4], 1
; WRITE	Enter n:	N/A	
	push offset string1
	push offset String
	call crt_printf
; READ	n	N/A	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-64]
	push eax
	push offset OneInt
	call crt_scanf
; WRITE	Enter a[i]:	N/A	
	push offset string2
	push offset String
	call crt_printf
; LABEL	2	N/A	
label2:
; LEQ<=	i	n	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp+8]
	mov ecx, dword ptr [ebx-64]
	cmp eax, ecx
	jg label3
; JMPF	3	t0	
; READ	tmp	N/A	
	lea eax, dword ptr [ebp-8]
	push eax
	push offset OneInt
	call crt_scanf
; SETARRAY[]=	a	i	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	mov ebx, dword ptr [ebp-8]
	mov dword ptr [eax], ebx
; ADD+	i	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-16], eax
; ASSIGN	i	t1	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-4], eax
; JMP	2	N/A	
	jmp label2
; LABEL	3	N/A	
label3:
; PROCEND	start_initialize	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_initialize	ENDP
; PROCMARK	start_quicksort	5	
start_quicksort	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 112
; temps allocated between ebp-24 and ebp-112
; *** subroutine body ***
; ASSIGN	i	left	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-52]
	mov dword ptr [ebp-4], eax
; ASSIGN	j	right	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-56]
	mov dword ptr [ebp-8], eax
; ADD+	left	right	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-52]
	mov ebx, dword ptr [ebp+8]
	add eax, dword ptr [ebx-56]
	mov dword ptr [ebp-24], eax
; DIV/	t0	2	
	mov edx, 0
	mov eax, dword ptr [ebp-24]
	mov ebx, 2
	idiv ebx
	mov dword ptr [ebp-28], eax
; ASSIGN	middle	t1	
	mov eax, dword ptr [ebp-28]
	mov dword ptr [ebp-20], eax
; GETARRAY=[]	a	middle	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-20], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-32], eax
; ASSIGN	x	t2	
	mov eax, dword ptr [ebp-32]
	mov dword ptr [ebp-12], eax
; LABEL	4	N/A	
label4:
; LES<	i	j	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp-8]
	cmp eax, ebx
	jge label10
; JMPF	10	t3	
; LABEL	5	N/A	
label5:
; GETARRAY=[]	a	i	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-40], eax
; LES<	t4	x	
	mov ebx, dword ptr [ebp-40]
	mov ecx, dword ptr [ebp-12]
	cmp ebx, ecx
	jge label6
; JMPF	6	t5	
; ADD+	i	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-48], eax
; ASSIGN	i	t6	
	mov eax, dword ptr [ebp-48]
	mov dword ptr [ebp-4], eax
; JMP	5	N/A	
	jmp label5
; LABEL	6	N/A	
label6:
; LABEL	7	N/A	
label7:
; GETARRAY=[]	a	j	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-8], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-52], eax
; GTR>	t7	x	
	mov ebx, dword ptr [ebp-52]
	mov ecx, dword ptr [ebp-12]
	cmp ebx, ecx
	jle label8
; JMPF	8	t8	
; SUB-	j	1	
	mov eax, dword ptr [ebp-8]
	sub eax, 1
	mov dword ptr [ebp-60], eax
; ASSIGN	j	t9	
	mov eax, dword ptr [ebp-60]
	mov dword ptr [ebp-8], eax
; JMP	7	N/A	
	jmp label7
; LABEL	8	N/A	
label8:
; LEQ<=	i	j	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp-8]
	cmp eax, ebx
	jg label9
; JMPF	9	t10	
; GETARRAY=[]	a	i	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-68], eax
; ASSIGN	w	t11	
	mov eax, dword ptr [ebp-68]
	mov dword ptr [ebp-16], eax
; GETARRAY=[]	a	j	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-4]
	imul ecx, dword ptr [ebp-8], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-72], eax
; SETARRAY[]=	a	i	
	mov ebx, dword ptr [ebp+8]
	lea eax, dword ptr [ebx-4]
	imul ecx, dword ptr [ebp-4], 4
	sub eax, ecx
	mov ebx, dword ptr [ebp-72]
	mov dword ptr [eax], ebx
; SETARRAY[]=	a	j	
	mov ecx, dword ptr [ebp+8]
	lea eax, dword ptr [ecx-4]
	imul ecx, dword ptr [ebp-8], 4
	sub eax, ecx
	mov ebx, dword ptr [ebp-16]
	mov dword ptr [eax], ebx
; ADD+	i	1	
	mov eax, dword ptr [ebp-4]
	add eax, 1
	mov dword ptr [ebp-76], eax
; ASSIGN	i	t13	
	mov eax, dword ptr [ebp-76]
	mov dword ptr [ebp-4], eax
; SUB-	j	1	
	mov eax, dword ptr [ebp-8]
	sub eax, 1
	mov dword ptr [ebp-80], eax
; ASSIGN	j	t14	
	mov eax, dword ptr [ebp-80]
	mov dword ptr [ebp-8], eax
; LABEL	9	N/A	
label9:
; JMP	4	N/A	
	jmp label4
; LABEL	10	N/A	
label10:
; LES<	left	j	
	mov eax, dword ptr [ebp+8]
	mov ebx, dword ptr [eax-52]
	mov ecx, dword ptr [ebp-8]
	cmp ebx, ecx
	jge label11
; JMPF	11	t15	
; ADD+	sp	1	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-48]
	add eax, 1
	mov dword ptr [ebp-88], eax
; ASSIGN	sp	t16	
	mov eax, dword ptr [ebp-88]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-48], eax
; SETARRAY[]=	stack	sp	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-68]
	mov ebx, dword ptr [ebp+8]
	imul ecx, dword ptr [ebx-48], 4
	sub eax, ecx
	mov ecx, dword ptr [ebp+8]
	mov ebx, dword ptr [ecx-56]
	mov dword ptr [eax], ebx
; ASSIGN	right	j	
	mov eax, dword ptr [ebp-8]
	mov edx, dword ptr [ebp+8]
	mov dword ptr [edx-56], eax
; CALL	start_quicksort	0	
	push dword ptr [ebp+8]
	call start_quicksort
; GETARRAY=[]	stack	sp	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-68]
	mov ebx, dword ptr [ebp+8]
	imul ecx, dword ptr [ebx-48], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-92], eax
; ASSIGN	right	t17	
	mov eax, dword ptr [ebp-92]
	mov ecx, dword ptr [ebp+8]
	mov dword ptr [ecx-56], eax
; SUB-	sp	1	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-48]
	sub eax, 1
	mov dword ptr [ebp-96], eax
; ASSIGN	sp	t18	
	mov eax, dword ptr [ebp-96]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-48], eax
; LABEL	11	N/A	
label11:
; LES<	i	right	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp+8]
	mov ecx, dword ptr [ebx-56]
	cmp eax, ecx
	jge label12
; JMPF	12	t19	
; ADD+	sp	1	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-48]
	add eax, 1
	mov dword ptr [ebp-104], eax
; ASSIGN	sp	t20	
	mov eax, dword ptr [ebp-104]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-48], eax
; SETARRAY[]=	stack	sp	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-68]
	mov ebx, dword ptr [ebp+8]
	imul ecx, dword ptr [ebx-48], 4
	sub eax, ecx
	mov ecx, dword ptr [ebp+8]
	mov ebx, dword ptr [ecx-52]
	mov dword ptr [eax], ebx
; ASSIGN	left	i	
	mov eax, dword ptr [ebp-4]
	mov edx, dword ptr [ebp+8]
	mov dword ptr [edx-52], eax
; CALL	start_quicksort	0	
	push dword ptr [ebp+8]
	call start_quicksort
; GETARRAY=[]	stack	sp	
	mov eax, dword ptr [ebp+8]
	lea eax, dword ptr [eax-68]
	mov ebx, dword ptr [ebp+8]
	imul ecx, dword ptr [ebx-48], 4
	sub eax, ecx
	mov eax, dword ptr [eax]
	mov dword ptr [ebp-108], eax
; ASSIGN	left	t21	
	mov eax, dword ptr [ebp-108]
	mov ecx, dword ptr [ebp+8]
	mov dword ptr [ecx-52], eax
; SUB-	sp	1	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-48]
	sub eax, 1
	mov dword ptr [ebp-112], eax
; ASSIGN	sp	t22	
	mov eax, dword ptr [ebp-112]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-48], eax
; LABEL	12	N/A	
label12:
; PROCEND	start_quicksort	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_quicksort	ENDP
; PROCMARK	start	120	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 480
; temps allocated between ebp-484 and ebp-480
; *** subroutine body ***
; CALL	start_initialize	0	
	push ebp
	call start_initialize
; WRITE	before:\n	N/A	
	push offset string3
	push offset String
	call crt_printf
; CALL	start_printarray	0	
	push ebp
	call start_printarray
; ASSIGN	sp	0	
	mov dword ptr [ebp-48], 0
; ASSIGN	left	1	
	mov dword ptr [ebp-52], 1
; ASSIGN	right	n	
	mov eax, dword ptr [ebp-64]
	mov dword ptr [ebp-56], eax
; CALL	start_quicksort	0	
	push ebp
	call start_quicksort
; WRITE	after:	N/A	
	push offset string4
	push offset String
	call crt_printf
; CALL	start_printarray	0	
	push ebp
	call start_printarray
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
