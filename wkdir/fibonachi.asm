.data
	dword		input
	dword		output

.code
	call 		main

main:
	assign		a2, input
	load		r0, a2
	push		r0
	call		fibo
	assign		a3, output
	store		r0, a3
	pop			r0
	ret

fibo:
	assign		r4, 04h
	mov 		r0, a0
	add			r0, r4
	mov			a2, r0
	load		r0, a2
	assign		r4, 00h
	cmp			r0, r4
	je			return
	assign		r4, 01h
	cmp			r0, r4
	je			return
	push 		r0
	dec			r0
	push 		r0
	call		fibo
	pop 		r4
	push		r0
	mov			r0, r4
	dec			r0
	push 		r0
	call		fibo
	pop			r4
	pop			r4
	add			r0, r4
	pop			r4
	ret
return:
	assign		r0, 01h
	ret