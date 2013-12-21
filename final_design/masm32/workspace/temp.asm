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
; PROCMARK	start	1	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 4
; temps allocated between ebp-8 and ebp-4
; *** subroutine body ***
; ASSIGN	i	1	
	mov dword ptr [ebp-4], 1
; LABEL	0	N/A	
label0:
; LEQ<=	i	10	
	mov eax, dword ptr [ebp-4]
	mov ebx, 10
	cmp eax, ebx
	jg label1
; JMPF	1	t0	
; WRITE	i	N/A	
	push dword ptr [ebp-4]
	push offset OneInt
	call crt_printf
; INC	i	N/A	
	inc dword ptr [ebp-4]
; JMP	0	N/A	
	jmp label0
; LABEL	1	N/A	
label1:
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
