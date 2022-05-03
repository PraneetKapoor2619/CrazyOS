global printf

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PRINTF
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printf:
	mov di, si
	xor ax, ax
	xor cx, cx
	dec cx
	cld
	repne scasw
.1:
	lodsb
	cmp al, 0x00
	je .exit
	cmp al, '%'
	je .percent
	cmp al, '\'
	je .backslash
	call putchar
	jmp .1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IF PERCENTAGE SIGN IS ENCOUNTERED
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.percent:
	lodsb
	cmp al, 'c'
	je .char
	cmp al, 'd'
	je .num
	cmp al, 'x'
	je .hex
.2p:
	add di, 0x02
	jmp .1
.char:
	mov ax, word [di]
	call putchar
	jmp .2p
.num:
	mov dx, word [di]
	call printdec
	jmp .2p
.hex:
	mov dx, word [di]
	push si
	push di
	call printhex
	pop di
	pop si
	jmp .2p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IF BACKSLASH IS ENCOUNTERED
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.backslash:
	lodsb
	cmp al, 'n'
	je .newline
	cmp al, 't'
	je .tab
.2b:
	jmp .1
.newline:
	mov al, 0x0a
	call putchar
	jmp .2b
.tab:
	mov al, 0x09
	call putchar
	jmp .2b
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IF NULL TERMINATOR IS ENCOUNTERED
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.exit:
	nop
	ret
