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
.CODE
; PROCMARK	start	3	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 28
; temps allocated between ebp-16 and ebp-28
; *** subroutine body ***
; ASSIGN	a	0	
	mov dword ptr [ebp-4], 0
; ASSIGN	i	0	
	mov dword ptr [ebp-12], 0
; LABEL	0	N/A	
label0:
; LES<	i	10	
	mov eax, dword ptr [ebp-12]
	mov ebx, 10
	cmp eax, ebx
	jge label1
; JMPF	1	t0	
; ADD+	i	1	
	mov eax, dword ptr [ebp-12]
	add eax, 1
	mov dword ptr [ebp-20], eax
; MULT*	a	i	
	mov eax, dword ptr [ebp-4]
	imul eax, dword ptr [ebp-12]
	mov dword ptr [ebp-16], eax
; ASSIGN	a	a	
	mov eax, dword ptr [ebp-4]
	mov dword ptr [ebp-4], eax
; ASSIGN	i	i	
	mov eax, dword ptr [ebp-12]
	mov dword ptr [ebp-12], eax
; ASSIGN	t0	t0	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-16], eax
; ASSIGN	a	t0	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-4], eax
; ASSIGN	t1	t1	
	mov eax, dword ptr [ebp-20]
	mov dword ptr [ebp-20], eax
; ASSIGN	i	t1	
	mov eax, dword ptr [ebp-20]
	mov dword ptr [ebp-12], eax
; JMP	0	N/A	
	jmp label0
; LABEL	1	N/A	
label1:
; WRITE	a=	a	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-4]
	push offset OneInt
	call crt_printf
; ASSIGN	a	a	
	mov eax, dword ptr [ebp-4]
	mov dword ptr [ebp-4], eax
; ASSIGN	b	a	
	mov eax, dword ptr [ebp-4]
	mov dword ptr [ebp-8], eax
; ASSIGN	i	0	
	mov dword ptr [ebp-12], 0
; LABEL	2	N/A	
label2:
; LES<	i	100	
	mov eax, dword ptr [ebp-12]
	mov ebx, 100
	cmp eax, ebx
	jge label3
; JMPF	3	t0	
; ADD+	i	1	
	mov eax, dword ptr [ebp-12]
	add eax, 1
	mov dword ptr [ebp-28], eax
; ADD+	b	i	
	mov eax, dword ptr [ebp-8]
	add eax, dword ptr [ebp-12]
	mov dword ptr [ebp-24], eax
; ASSIGN	b	b	
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-8], eax
; ASSIGN	i	i	
	mov eax, dword ptr [ebp-12]
	mov dword ptr [ebp-12], eax
; ASSIGN	t2	t2	
	mov eax, dword ptr [ebp-24]
	mov dword ptr [ebp-24], eax
; ASSIGN	b	t2	
	mov eax, dword ptr [ebp-24]
	mov dword ptr [ebp-8], eax
; ASSIGN	t3	t3	
	mov eax, dword ptr [ebp-28]
	mov dword ptr [ebp-28], eax
; ASSIGN	i	t3	
	mov eax, dword ptr [ebp-28]
	mov dword ptr [ebp-12], eax
; JMP	2	N/A	
	jmp label2
; LABEL	3	N/A	
label3:
; WRITE	b=	b	
	push offset string1
	push offset String
	call crt_printf
	push dword ptr [ebp-8]
	push offset OneInt
	call crt_printf
; ASSIGN	b	b	
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-8], eax
; PROCEND	start	N/A	
; *** epilogue ***
	mov esp, ebp
	pop ebp
	push offset WriteLine
	call crt_printf
	call ExitProcess
start	ENDP
END
