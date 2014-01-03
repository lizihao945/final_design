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
string0 db "a=", 0
string1 db "b=", 0
string2 db "c=", 0
string3 db "e=", 0
.CODE
; PROCMARK	start	5	2	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 28
; temps allocated between ebp-24 and ebp-28
; *** subroutine body ***
; ASSIGN	a	3	N/A	
; ****	a	now in	eax
	mov eax, 3
; ASSIGN	b	11	N/A	
; ****	b	now in	ebx
	mov ebx, 11
; ASSIGN	c	2	N/A	
; ****	c	now in	ecx
	mov ecx, 2
; SUB-	a	b	t0	
	mov dword ptr [ebp-4], eax
	sub eax, ebx
; ADD+	t0	c	t1	
	mov dword ptr [ebp-24], eax
	add eax, ecx
; ASSIGN	e	t1	N/A	
; WRITE	a=	a	N/A	
; write back ****	t1	****
	mov dword ptr [ebp-28], eax
; write back ****	e	****
	mov dword ptr [ebp-20], eax
; write back ****	b	****
	mov dword ptr [ebp-8], ebx
; write back ****	c	****
	mov dword ptr [ebp-12], ecx
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-4]
	push offset OneInt
	call crt_printf
; WRITE	b=	b	N/A	
	push offset string1
	push offset String
	call crt_printf
	push dword ptr [ebp-8]
	push offset OneInt
	call crt_printf
; WRITE	c=	c	N/A	
	push offset string2
	push offset String
	call crt_printf
	push dword ptr [ebp-12]
	push offset OneInt
	call crt_printf
; WRITE	e=	e	N/A	
	push offset string3
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
