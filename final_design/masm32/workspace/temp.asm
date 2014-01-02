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
; PROCMARK	start_mod	0	
start_mod	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 12
; temps allocated between ebp-4 and ebp-12
; *** subroutine body ***
; DIV/	x	y	
	mov edx, 0
	mov eax, dword ptr [ebp+20]
	mov ebx, dword ptr [ebp+16]
	idiv ebx
	mov dword ptr [ebp-4], eax
; MULT*	t0	y	
	mov eax, dword ptr [ebp-4]
	imul eax, dword ptr [ebp+16]
	mov dword ptr [ebp-8], eax
; SUB-	x	t1	
	mov eax, dword ptr [ebp+20]
	sub eax, dword ptr [ebp-8]
	mov dword ptr [ebp-12], eax
; ASSIGN	start_mod	t2	
	mov eax, dword ptr [ebp-12]
	mov ebx, dword ptr [ebp+12]
	mov dword ptr [ebx], eax
; PROCEND	start_mod	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_mod	ENDP
; PROCMARK	start	4	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 24
; temps allocated between ebp-20 and ebp-24
; *** subroutine body ***
; ASSIGN	i	100	
	mov dword ptr [ebp-4], 100
; LABEL	0	N/A	
label0:
; LEQ<=	i	999	
	mov eax, dword ptr [ebp-4]
	mov ebx, 999
	cmp eax, ebx
	jg label2
; JMPF	2	t0	
; DIV/	i	100	
	mov edx, 0
	mov eax, dword ptr [ebp-4]
	mov ebx, 100
	idiv ebx
	mov dword ptr [ebp-20], eax
; ASSIGN	a	t0	
	mov eax, dword ptr [ebp-20]
	mov dword ptr [ebp-8], eax
; PARAMVAL	i	N/A	
	push dword ptr [ebp-4]
; PARAMVAL	10	N/A	
	push 10
; CALL	start_mod	t1	
	lea eax, dword ptr [ebp-24]
	push eax
	push ebp
	call start_mod
; ASSIGN	b	t1	
	mov eax, dword ptr [ebp-24]
	mov dword ptr [ebp-12], eax
; EQL=	a	b	
	mov eax, dword ptr [ebp-8]
	mov ebx, dword ptr [ebp-12]
	cmp eax, ebx
	jne label1
; JMPF	1	t0	
; WRITE	 	i	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-4]
	push offset OneInt
	call crt_printf
; LABEL	1	N/A	
label1:
; INC	i	N/A	
	inc dword ptr [ebp-4]
; JMP	0	N/A	
	jmp label0
; LABEL	2	N/A	
label2:
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset WriteLine
	call crt_printf
	call ExitProcess
start	ENDP
END
