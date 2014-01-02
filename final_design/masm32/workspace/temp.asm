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
.CODE
; PROCMARK	start	6	
start	PROC
; *** prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 44
; temps allocated between ebp-28 and ebp-44
; *** subroutine body ***
; ASSIGN	x	a	
	mov eax, dword ptr [ebp-16]
	mov dword ptr [ebp-4], eax
; ASSIGN	y	b	
	mov eax, dword ptr [ebp-20]
	mov dword ptr [ebp-8], eax
; ASSIGN	i	0	
	mov dword ptr [ebp-24], 0
; LABEL	0	N/A	
label0:
; LES<	i	100	
	mov eax, dword ptr [ebp-24]
	mov ebx, 100
	cmp eax, ebx
	jge label2
; JMPF	2	t0	
; MULT*	a	10	
	mov eax, dword ptr [ebp-16]
	imul eax, 10
	mov dword ptr [ebp-28], eax
; ASSIGN	z	t0	
	mov eax, dword ptr [ebp-28]
	mov dword ptr [ebp-12], eax
; ADD+	x	y	
	mov eax, dword ptr [ebp-4]
	add eax, dword ptr [ebp-8]
	mov dword ptr [ebp-32], eax
; ASSIGN	x	t1	
	mov eax, dword ptr [ebp-32]
	mov dword ptr [ebp-4], eax
; LES<	x	z	
	mov eax, dword ptr [ebp-4]
	mov ebx, dword ptr [ebp-12]
	cmp eax, ebx
	jge label1
; JMPF	1	t0	
; SUB-	x	y	
	mov eax, dword ptr [ebp-4]
	sub eax, dword ptr [ebp-8]
	mov dword ptr [ebp-36], eax
; ASSIGN	x	t2	
	mov eax, dword ptr [ebp-36]
	mov dword ptr [ebp-4], eax
; LABEL	1	N/A	
label1:
; ADD+	y	1	
	mov eax, dword ptr [ebp-8]
	add eax, 1
	mov dword ptr [ebp-40], eax
; ASSIGN	y	t3	
	mov eax, dword ptr [ebp-40]
	mov dword ptr [ebp-8], eax
; ADD+	i	1	
	mov eax, dword ptr [ebp-24]
	add eax, 1
	mov dword ptr [ebp-44], eax
; ASSIGN	i	t4	
	mov eax, dword ptr [ebp-44]
	mov dword ptr [ebp-24], eax
; JMP	0	N/A	
	jmp label0
; LABEL	2	N/A	
label2:
; WRITE	a=	a	
	push offset string0
	push offset String
	call crt_printf
	push dword ptr [ebp-16]
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
