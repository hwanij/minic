	.text
	.global	qsort
	.type	qsort,function
qsort:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #72
	stw	r17, [r27+], #-16
	stw	r18, [r27+], #-20
	stw	r19, [r27+], #-24
	stw	r0, [r27+], #-24
	mov	r17, r1
	mov	r6, r2
.L1:
	add	r4, r17, #1
	cmpsub.a	r6, r4
	bsg	.L3
.L2:
	b	.L9
.L3:
	add	r4, r17, #1
	mov	r5, r4
	add	r4, r17, #1
.L8:
	cmpsub.a	r4, r6
	beg	.L5
.L4:
	ldw	r18, [r27+], #-48
	ldw	r1, [r27+], #-16
	add	r3, r1, r4<<#2
	stw	r3, [r27+], #-36
	ldw	r1, [r27+], #-36
	ldw	r3, [r1+], #0
	stw	r3, [r27+], #-40
	ldw	r1, [r27+], #-16
	add	r19, r1, r17<<#2
	ldw	r19, [r19+], #0
	ldw	r1, [r27+], #-40
	cmpsub.a	r1, r19
	beg	.L7
.L6:
	ldw	r1, [r27+], #-16
	add	r3, r1, r5<<#2
	stw	r3, [r27+], #-44
	ldw	r1, [r27+], #-44
	ldw	r18, [r1+], #0
	ldw	r1, [r27+], #-16
	add	r3, r1, r5<<#2
	stw	r3, [r27+], #-52
	ldw	r1, [r27+], #-16
	add	r19, r1, r4<<#2
	ldw	r19, [r19+], #0
	ldw	r1, [r27+], #-52
	stw	r19, [r1+], #0
	ldw	r1, [r27+], #-16
	add	r3, r1, r4<<#2
	stw	r3, [r27+], #-56
	ldw	r1, [r27+], #-56
	stw	r18, [r1+], #0
	add	r5, r5, #1
.L7:
	add	r4, r4, #1
	b	.L8
.L5:
	ldw	r1, [r27+], #-16
	add	r3, r1, r17<<#2
	stw	r3, [r27+], #-60
	ldw	r1, [r27+], #-60
	ldw	r18, [r1+], #0
	ldw	r1, [r27+], #-16
	add	r3, r1, r17<<#2
	stw	r3, [r27+], #-64
	sub	r4, r5, #1
	ldw	r4, [r27+], #-32
	ldw	r1, [r27+], #-16
	add	r4, r1, r4<<#2
	ldw	r4, [r27+], #-32
	ldw	r4, [r4+], #0
	ldw	r4, [r27+], #-32
	ldw	r1, [r27+], #-64
	stw	r4, [r1+], #0
	sub	r3, r5, #1
	stw	r3, [r27+], #-68
	ldw	r1, [r27+], #-16
	ldw	r2, [r27+], #-68
	add	r4, r1, r2<<#2
	ldw	r4, [r27+], #-32
	stw	r18, [r4+], #0
	sub	r19, r5, #1
	stw	r4, [r27+], #-32
	stw	r5, [r27+], #-28
	stw	r6, [r27+], #-24
	ldw	r28, [r27+], #-16
	mov	r0, r28
	mov	r1, r17
	mov	r2, r19
	b.l	qsort
	ldw	r6, [r27+], #-24
	ldw	r5, [r27+], #-28
	ldw	r4, [r27+], #-32
	ldw	r28, [r27+], #-16
	mov	r17, r5
	b	.L1
.L9:
	ldw	r17, [r27+], #-16
	ldw	r18, [r27+], #-20
	ldw	r19, [r27+], #-24
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L10:
	.text
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #536
	stw	r17, [r27+], #-16
	add	r3, r27, #-528
	stw	r3, [r27+], #-532
.L11:
	ldw	r4, [r27+], #-532
	mov	r5, #0
.L14:
	cmpsub.a	r5, #128
	beg	.L13
.L12:
	add	r17, r4, r5<<#2
	rsub	r6, r5, #128
	stw	r6, [r17+], #0
	add	r5, r5, #1
	b	.L14
.L13:
	mov	r5, #0
.L17:
	cmpsub.a	r5, #128
	beg	.L16
.L15:
	add	r6, r4, r5<<#2
	ldw	r6, [r6+], #0
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, r6
	b.l	print_int
	ldw	r5, [r27+], #-16
	ldw	r4, [r27+], #-532
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, #32
	b.l	print_char
	ldw	r5, [r27+], #-16
	ldw	r4, [r27+], #-532
	add	r5, r5, #1
	b	.L17
.L16:
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, #10
	b.l	print_char
	ldw	r5, [r27+], #-16
	ldw	r4, [r27+], #-532
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, #10
	b.l	print_char
	ldw	r5, [r27+], #-16
	ldw	r4, [r27+], #-532
	stw	r4, [r27+], #-532
	mov	r0, r4
	mov	r1, #0
	mov	r2, #5
	b.l	qsort
	ldw	r4, [r27+], #-532
	mov	r5, #0
.L20:
	cmpsub.a	r5, #128
	beg	.L19
.L18:
	add	r6, r4, r5<<#2
	ldw	r6, [r6+], #0
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, r6
	b.l	print_int
	ldw	r5, [r27+], #-16
	ldw	r4, [r27+], #-532
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, #32
	b.l	print_char
	ldw	r5, [r27+], #-16
	ldw	r4, [r27+], #-532
	add	r5, r5, #1
	b	.L20
.L19:
	mov	r0, #10
	b.l	print_char
	mov	r0, #10
	b.l	print_char
	mov	r0, #0
.L21:
	ldw	r17, [r27+], #-16
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L22:
