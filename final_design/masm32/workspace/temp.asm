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
; PROCMARK	start_f1	0	
start_f1	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 0
; temps allocated between ebp-4 and ebp-0
; *** subroutine body ***
; WRITE	a	N/A	
	push dword ptr [ebp+20]
	push offset OneInt
	call crt_printf
; WRITE	b	N/A	
	mov eax, dword ptr [ebp+16]
	push dword ptr [eax]
	push offset OneInt
	call crt_printf
; PROCEND	start_f1	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_f1	ENDP
; PROCMARK	start_f2	0	
start_f2	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 0
; temps allocated between ebp-4 and ebp-0
; *** subroutine body ***
; WRITE	c	N/A	
	push dword ptr [ebp+20]
	push offset OneInt
	call crt_printf
; WRITE	d	N/A	
	mov eax, dword ptr [ebp+16]
	push dword ptr [eax]
	push offset OneInt
	call crt_printf
; PROCEND	start_f2	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_f2	ENDP
; PROCMARK	start	4	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 24
; temps allocated between ebp-20 and ebp-24
; *** subroutine body ***
; ASSIGN	a	10	
	mov dword ptr [ebp-4], 10
; ASSIGN	b	1	
	mov dword ptr [ebp-8], 1
; PARAMVAL	a	N/A	
	push dword ptr [ebp-4]
; PARAMREF	b	N/A	
	lea eax, dword ptr [ebp-8]
	push eax
; CALL	start_f1	t0	
	lea eax, dword ptr [ebp-20]
	push eax
	push ebp
	call start_f1
; PARAMVAL	a	N/A	
	push dword ptr [ebp-4]
; PARAMREF	b	N/A	
	lea eax, dword ptr [ebp-8]
	push eax
; CALL	start_f2	t1	
	lea eax, dword ptr [ebp-24]
	push eax
	push ebp
	call start_f2
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
