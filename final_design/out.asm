.386
.model flat, stdcall

include \masm32\include\kernel32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\msvcrt.lib
includelib \masm32\lib\kernel32.lib

.data
OneInt db "%d", 0ah, 0
; PROCMARK	start	3	
.CODE
PUBLIC start
start	PROC
; *** standard subroutine prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 12
	push	edi
	push	esi
; *** subroutine body ***
; MULT*	100	5	
	mov eax, 100
	mov ebx, 5
	mul	ebx
	mov [ebp-16], eax
; ASSIGN	x	t0	
	mov eax, [ebp-16]
	mov [ebp-8], eax
; DIV/	x	100	
	mov eax, [ebp-8]
	mov ebx, 100
	div	ebx
	mov [ebp-20], eax
; ASSIGN	y	t1	
	mov eax, [ebp-20]
	mov [ebp-12], eax
; WRITE	y	N/A	
	push [ebp-12]
	push offset OneInt
	call crt_printf
; PROCEND	start	N/A	
; *** standard subroutine epilogue ***
	pop esi
	pop edi
	mov esp, ebp
	call ExitProcess
start	ENDP
END
