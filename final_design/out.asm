.386
.MODEL FLAT
.DATA
	_a	DD ?
	_x	DD ?
	_y	DD ?
	_t0	DD ?
	_t1	DD ?
.CODE
PUBLIC _main
_main PROC
; *** standard subroutine prologue ***
	push	ebp
	mov ebp, esp
	sub esp, 4
	push	edi
	push	esi
; *** subroutine body ***
	mov eax, 100
	mov ebx, 5
	mul	ebx
	mov _t0, eax
	mov _x, _t0
	mov eax, _x
	mov ebx, 100
	div	ebx
	mov _t1, eax
	mov _y, _t1
; *** standard subroutine epilogue ***
	pop esi
	pop edi
	mov esp, ebp
_main	ENDP
END
