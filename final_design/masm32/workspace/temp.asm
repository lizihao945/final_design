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
; PROCMARK	start	3	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 48
; temps allocated between ebp-16 and ebp-48
; *** subroutine body ***
; ASSIGN	x	6	
	mov dword ptr [ebp-4], 6
; ASSIGN	y	3	
	mov dword ptr [ebp-8], 3
; ADD+	x	y	
	mov eax, dword ptr [ebp-4]
	add eax, dword ptr [ebp-8]
	mov dword ptr [ebp-16], eax
; ADD+	x	y	
	mov eax, dword ptr [ebp-4]
	add eax, dword ptr [ebp-8]
	mov dword ptr [ebp-20], eax
; SUB-	x	y	
	mov eax, dword ptr [ebp-4]
	sub eax, dword ptr [ebp-8]
	mov dword ptr [ebp-24], eax
; MULT*	t1	t2	
	mov eax, dword ptr [ebp-20]
	imul eax, dword ptr [ebp-24]
	mov dword ptr [ebp-28], eax
; SUB-	t0	t3	
	mov eax, dword ptr [ebp-16]
	sub eax, dword ptr [ebp-28]
	mov dword ptr [ebp-32], eax
; ADD+	x	y	
	mov eax, dword ptr [ebp-4]
	add eax, dword ptr [ebp-8]
	mov dword ptr [ebp-36], eax
; SUB-	x	y	
	mov eax, dword ptr [ebp-4]
	sub eax, dword ptr [ebp-8]
	mov dword ptr [ebp-40], eax
; MULT*	t5	t6	
	mov eax, dword ptr [ebp-36]
	imul eax, dword ptr [ebp-40]
	mov dword ptr [ebp-44], eax
; ADD+	t4	t7	
	mov eax, dword ptr [ebp-32]
	add eax, dword ptr [ebp-44]
	mov dword ptr [ebp-48], eax
; ASSIGN	z	t8	
	mov eax, dword ptr [ebp-48]
	mov dword ptr [ebp-12], eax
; WRITE	z	N/A	
	push dword ptr [ebp-12]
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
