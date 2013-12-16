.386
.model flat, stdcall

include \masm32\include\kernel32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\msvcrt.lib
includelib \masm32\lib\kernel32.lib

.data
OneInt db "%d", 0ah, 0
.CODE
; PROCMARK	start_foo	0	
start_foo	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 4
; var prev ebp located at ebp-4
	mov [ebp-4], eax
; *** subroutine body ***
; MULT*	x	10	
	mov eax, dword ptr [ebp+4]
	mov ebx, 10
	mul	ebx
	mov dword ptr [ebp-4], eax
; ASSIGN	x	t0	
	mov eax, dword ptr [ebp-4]
	mov dword ptr [ebp+4], eax
; WRITE	x	N/A	
	push dword ptr [ebp+4]
	push offset OneInt
	call crt_printf
; PROCEND	start_foo	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_foo	ENDP
; PROCMARK	start	2	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 12
; var prev ebp located at ebp-4
	mov [ebp-4], eax
; *** subroutine body ***
; ASSIGN	a	10	
	mov dword ptr [ebp-8], 10
; PARAMREF	a	N/A	
; CALL	start_foo	1	
	mov eax, ebp
	call start_foo
; WRITE	a	N/A	
	push dword ptr [ebp-8]
	push offset OneInt
	call crt_printf
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	call ExitProcess
start	ENDP
END
