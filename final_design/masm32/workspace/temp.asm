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
	mov eax, dword ptr [ebp+16]
	mov ebx, 0
	cmp eax, ebx
	jne label0
; JMPF	0	t0	
; ASSIGN	start_fab	0	
	mov eax, dword ptr [ebp+12]
	mov dword ptr [eax], 0
; JMP	3	N/A	
	jmp label3
; LABEL	0	N/A	
label0:
; EQL=	x	1	
	mov ebx, dword ptr [ebp+16]
	mov ecx, 1
	cmp ebx, ecx
	jne label1
; JMPF	1	t0	
; ASSIGN	start_fab	1	
	mov eax, dword ptr [ebp+12]
	mov dword ptr [eax], 1
; JMP	2	N/A	
	jmp label2
; LABEL	1	N/A	
label1:
; SUB-	x	2	
	mov eax, dword ptr [ebp+16]
	sub eax, 2
	mov dword ptr [ebp-4], eax
; PARAMVAL	t0	N/A	
	push dword ptr [ebp-4]
; CALL	start_fab	t1	
	lea eax, dword ptr [ebp-8]
	push eax
	push dword ptr [ebp+8]
	call start_fab
; SUB-	x	1	
	mov eax, dword ptr [ebp+16]
	sub eax, 1
	mov dword ptr [ebp-12], eax
; PARAMVAL	t2	N/A	
	push dword ptr [ebp-12]
; CALL	start_fab	t3	
	lea eax, dword ptr [ebp-16]
	push eax
	push dword ptr [ebp+8]
	call start_fab
; ADD+	t1	t3	
	mov eax, dword ptr [ebp-8]
	add eax, dword ptr [ebp-16]
	mov dword ptr [ebp-20], eax
; ASSIGN	start_fab	t4	
	mov eax, dword ptr [ebp-20]
	mov ebx, dword ptr [ebp+12]
	mov dword ptr [ebx], eax
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
; ASSIGN	i	0	
	mov dword ptr [ebp-4], 0
; LABEL	4	N/A	
label4:
; LEQ<=	i	9	
	mov eax, dword ptr [ebp-4]
	mov ebx, 9
	cmp eax, ebx
	jg label5
; JMPF	5	t0	
; PARAMVAL	i	N/A	
	push dword ptr [ebp-4]
; CALL	start_fab	t0	
	lea eax, dword ptr [ebp-12]
	push eax
	push ebp
	call start_fab
; WRITE	t0	N/A	
	push dword ptr [ebp-12]
	push offset OneInt
	call crt_printf
; WRITE	 	N/A	
	push offset string0
	push offset String
	call crt_printf
; INC	i	N/A	
	inc dword ptr [ebp-4]
; JMP	4	N/A	
	jmp label4
; LABEL	5	N/A	
label5:
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset WriteLine
	call crt_printf
	call ExitProcess
start	ENDP
END
