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
	sub esp, 24
; temps allocated between ebp-16 and ebp-24
; *** subroutine body ***
; ADD+	x	y	
	mov eax, dword ptr [ebp-4]
	add eax, dword ptr [ebp-8]
	mov dword ptr [ebp-16], eax
; ADD+	x	y	
	mov eax, dword ptr [ebp-4]
	add eax, dword ptr [ebp-8]
	mov dword ptr [ebp-20], eax
; SUB-	t0	t1	
	mov eax, dword ptr [ebp-16]
	sub eax, dword ptr [ebp-20]
	mov dword ptr [ebp-24], eax
; ASSIGN	z	t2	
	mov eax, dword ptr [ebp-24]
	mov dword ptr [ebp-12], eax
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset WriteLine
	call crt_printf
	call ExitProcess
start	ENDP
END
