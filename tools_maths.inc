
;------------------------------------------------------------------[ Multiplies a 16 bit number by an 8 bit number
;------------------------------------------------------------------[ In  : .Y:.X * .A
;------------------------------------------------------------------[ Out : .Y:.X / .Y:.A
; TODO: This doesn't work properly (Multiply by Zero does not return Zero)
maths_sixteenbitmultiply
	sta	maths_work_val_c
	stx	maths_work_val_b
	sty	maths_work_val_a
	lda #$00
	tay
	beq maths_sbm_enterloop
maths_sbm_doadd
	clc
	adc	maths_work_val_b
	tax
	tya
	adc	maths_work_val_a
	tay
	txa
maths_sbm_loop
	asl	maths_work_val_b
	rol	maths_work_val_a
maths_sbm_enterloop
	lsr	maths_work_val_c
	bcs	maths_sbm_doadd
	bne	maths_sbm_loop
	rts

;------------------------------------------------------------------[ Multiplies two 8 bit numbers
;------------------------------------------------------------------[ In  : .X * .Y
;------------------------------------------------------------------[ Out : .A
maths_eightbitmultiply
	stx	maths_work_val_a
	sty	maths_work_val_b
	lda	#$00
	cmp	maths_work_val_a
	beq	maths_ebm_leave
	cmp	maths_work_val_b
	beq	maths_ebm_leave
maths_ebm_loop
	adc	maths_work_val_a
	dec	maths_work_val_b
	bne	maths_ebm_loop
maths_ebm_leave
	rts

;------------------------------------------------------------------[ Div / Mod
;------------------------------------------------------------------[ In  : .X / .Y
;------------------------------------------------------------------[ Out : .Y = Divisor, .X = Mod
divmod
	sty	maths_work_val_a
	ldy	#$00
divmod_check
	cpx	maths_work_val_a
	bcc	divmod_return
	iny
	txa
	sbc	maths_work_val_a
	tax
	jmp	divmod_check
divmod_return
	rts

maths_work_val_a	.byte	$00
maths_work_val_b	.byte	$00
maths_work_val_c	.byte	$00
maths_work_val_d	.byte	$00

