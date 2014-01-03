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
; PROCMARK	start_swap	1	
start_swap	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 4
; temps allocated between ebp-8 and ebp-4
; *** subroutine body ***
; ASSIGN	temp	x	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-4]
	mov dword ptr [ebp-4], eax
; ASSIGN	x	y	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-8]
	mov ecx, dword ptr [ebp+8]
	mov dword ptr [ecx-4], eax
; ASSIGN	y	temp	
	mov eax, dword ptr [ebp-4]
	mov ecx, dword ptr [ebp+8]
	mov dword ptr [ecx-8], eax
; PROCEND	start_swap	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_swap	ENDP
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
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-4]
	mov ecx, dword ptr [ebp+8]
	mov edx, dword ptr [ecx-8]
	idiv edx
	mov dword ptr [ebp-4], eax
; MULT*	t0	y	
	mov eax, dword ptr [ebp-4]
	mov ecx, dword ptr [ebp+8]
	imul eax, dword ptr [ecx-8]
	mov dword ptr [ebp-8], eax
; SUB-	x	t1	
	mov eax, dword ptr [ebp+8]
	mov eax, dword ptr [eax-4]
	sub eax, dword ptr [ebp-8]
	mov dword ptr [ebp-12], eax
; ASSIGN	x	t2	
	mov eax, dword ptr [ebp-12]
	mov ecx, dword ptr [ebp+8]
	mov dword ptr [ecx-4], eax
; PROCEND	start_mod	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_mod	ENDP
; PROCMARK	start	6	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 44
; temps allocated between ebp-28 and ebp-44
; *** subroutine body ***
; ASSIGN	x	a	
	mov eax, 45
	mov dword ptr [ebp-4], eax
; ASSIGN	y	b	
	mov eax, 27
	mov dword ptr [ebp-8], eax
; CALL	start_mod	t0	
	lea eax, dword ptr [ebp-28]
	push eax
	push ebp
	call start_mod
; LABEL	0	N/A	
label0:
; NEQL<>	x	0	
	mov eax, dword ptr [ebp-4]
	mov ecx, 0
	cmp eax, ecx
	je label1
; JMPF	1	t0	
; CALL	start_swap	t1	
	lea eax, dword ptr [ebp-32]
	push eax
	push ebp
	call start_swap
; CALL	start_mod	t2	
	lea eax, dword ptr [ebp-36]
	push eax
	push ebp
	call start_mod
; JMP	0	N/A	
	jmp label0
; LABEL	1	N/A	
label1:
; ASSIGN	g	y	
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-12], eax
; MULT*	a	b	
	mov eax, 45
	imul eax, 27
	mov dword ptr [ebp-40], eax
; DIV/	t3	g	
	mov edx, 0
	mov eax, dword ptr [ebp-40]
	mov ecx, dword ptr [ebp-12]
	idiv ecx
	mov dword ptr [ebp-44], eax
; ASSIGN	m	t4	
	mov eax, dword ptr [ebp-44]
	mov dword ptr [ebp-16], eax
; WRITE	g	N/A	
	push dword ptr [ebp-12]
	push offset OneInt
	call crt_printf
; WRITE	m	N/A	
	push dword ptr [ebp-16]
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
