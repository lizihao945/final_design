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
string0 db "e=", 0
.CODE
; PROCMARK	start	5	3	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 32
; temps allocated between ebp-24 and ebp-32
; *** subroutine body ***
; ASSIGN	a	3	N/A	
	mov eax, 3
; ASSIGN	b	11	N/A	
	mov ecx, 11
; ASSIGN	c	2	N/A	
	mov edx, 2
; ASSIGN	d	20	N/A	
	mov dword ptr [ebp-4], eax
	mov eax, 20
; SUB-	a	b	t0	
	mov dword ptr [ebp-8], ecx
	mov ecx, dword ptr [ebp-4]
	sub ecx, dword ptr [ebp-8]
; SUB-	c	d	t1	
	sub edx, eax
; ADD+	t0	t1	t2	
	mov dword ptr [ebp-12], edx
	mov edx, dword ptr [ebp-24]
	add edx, dword ptr [ebp-28]
	mov dword ptr [ebp-32], eax
; ASSIGN	e	t2	N/A	
	mov eax, dword ptr [ebp-32]
	mov dword ptr [ebp-20], eax
; WRITE	e=	e	N/A	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-20]
	push offset OneInt
	call crt_printf
; PROCEND	start	N/A	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset WriteLine
	call crt_printf
	call ExitProcess
start	ENDP
END
