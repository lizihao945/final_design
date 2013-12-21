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
string0 db "answer:", 0
string1 db "!", 0
.CODE
; PROCMARK	start_fab	0	
start_fab	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 20
; temps allocated between ebp-4 and ebp-20
; *** subroutine body ***
; EQL=	x	0	
	mov eax, dword ptr [ebp+12]
	mov ebx, 0
	cmp eax, ebx
	jne label0
; JMPF	0	t0	
; ASSIGN	start_fab	1	
	mov eax, dword ptr [ebp+8]
	mov dword ptr [eax-8], 1
; JMP	3	N/A	
	jmp label3
; LABEL	0	N/A	
label0:
; EQL=	x	1	
	mov ebx, dword ptr [ebp+12]
	mov ecx, 1
	cmp ebx, ecx
	jne label1
; JMPF	1	t0	
; ASSIGN	start_fab	1	
	mov eax, dword ptr [ebp+8]
	mov dword ptr [eax-8], 1
; JMP	2	N/A	
	jmp label2
; LABEL	1	N/A	
label1:
; SUB-	x	2	
	mov eax, dword ptr [ebp+12]
	sub eax, 2
	mov dword ptr [ebp-4], eax
; PARAMVAL	t0	N/A	
	push dword ptr [ebp-4]
; CALL	start_fab	t1	
	push dword ptr [ebp+8]
	call start_fab
; SUB-	x	1	
	mov eax, dword ptr [ebp+12]
	sub eax, 1
	mov dword ptr [ebp-12], eax
; PARAMVAL	t2	N/A	
	push dword ptr [ebp-12]
; CALL	start_fab	t3	
	push dword ptr [ebp+8]
	call start_fab
; ADD+	t1	t3	
	mov eax, dword ptr [ebp-8]
	add eax, dword ptr [ebp-16]
	mov dword ptr [ebp-20], eax
; ASSIGN	start_fab	t4	
	mov eax, dword ptr [ebp-20]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-8], eax
; LABEL	2	N/A	
label2:
; LABEL	3	N/A	
label3:
; PROCEND	start_fab	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_fab	ENDP
; PROCMARK	start	2	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 12
; temps allocated between ebp-12 and ebp-12
; *** subroutine body ***
; LABEL	4	N/A	
label4:
; EQL=	1	1	
	mov eax, 1
	mov ebx, 1
	cmp eax, ebx
	jne label5
; JMPF	5	t0	
; READ	n	N/A	
	lea eax, dword ptr [ebp-4]
	push eax
	push offset OneInt
	call crt_scanf
; PARAMVAL	n	N/A	
	push dword ptr [ebp-4]
; CALL	start_fab	t0	
	push ebp
	call start_fab
; WRITE	answer:	t0	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-12]
	push offset OneInt
	call crt_printf
; WRITE	!	N/A	
	push offset string1
	push offset String
	call crt_printf
; JMP	4	N/A	
	jmp label4
; LABEL	5	N/A	
label5:
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
