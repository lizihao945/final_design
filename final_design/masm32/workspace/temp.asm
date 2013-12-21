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
; PROCMARK	start_f1	1	
start_f1	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 16
; temps allocated between ebp-8 and ebp-16
; *** subroutine body ***
; ADD+	fpa	1	
	mov eax, dword ptr [ebp+12]
	mov eax, dword ptr [eax]
	add eax, 1
	mov dword ptr [ebp-8], eax
; ASSIGN	vd	t0	
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-4], eax
; ASSIGN	start_f1	vd	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-16], eax
; SUB-	0	vb	
	mov eax, 0
	mov eax, dword ptr [ebp+8]
	sub eax, dword ptr [eax-8]
	mov dword ptr [ebp-12], eax
; ADD+	t1	vc	
	mov eax, dword ptr [ebp-12]
	mov eax, dword ptr [ebp+8]
	add eax, dword ptr [eax-12]
	mov dword ptr [ebp-16], eax
; ASSIGN	va	t2	
	mov eax, dword ptr [ebp-16]
	mov ebx, dword ptr [ebp+8]
	mov dword ptr [ebx-4], eax
; PROCEND	start_f1	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_f1	ENDP
; PROCMARK	start_p1	1	
start_p1	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 4
; temps allocated between ebp-8 and ebp-4
; *** subroutine body ***
; PROCEND	start_p1	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	ret
start_p1	ENDP
; PROCMARK	start	5	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 20
; temps allocated between ebp-24 and ebp-20
; *** subroutine body ***
; READ	va	N/A	
	lea eax, dword ptr [ebp-4]
	push eax
	push offset OneInt
	call crt_scanf
; PARAMREF	va	N/A	
	lea eax, dword ptr [ebp-4]
	push eax
; CALL	start_f1	2	
	push ebp
	call start_f1
; ASSIGN	vb	start_f1	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-8], eax
; WRITE	vb	N/A	
	push dword ptr [ebp-8]
	push offset OneInt
	call crt_printf
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset Writeline
	call crt_printf
	call ExitProcess
start	ENDP
END
