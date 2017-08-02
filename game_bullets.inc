

;------------------------------------------------------------------[ Process Bullets
;------------------------------------------------------------------[ In  : .Y = Number of bullets to process
;------------------------------------------------------------------[ Out : N/A
bullets_check
	lda	#$06
	sta	$D020	; DEBUG

	sty	bullet_remain
	ldx	bullet_current
bullets_check_loop
	lda	bullet_direction, x
	beq	bullets_check_next
	
	sta	bullet_workingval

	ldy	bullet_y, x
	lda	bullet_x, x
	tax
	lda	#$00
	jsr	bitmap_plotpixel

	dec	bullet_workingval
	lda	bullet_workingval
	
	cmp	#$F0
	beq	bullets_playerbullet	; Player bullets have direction of $F0, rather than any specific angle
	
	ldx	bullet_frame
	jsr	maths_sine
	ldy	bullet_current	; .Y = Bullet position map offset
	clc
	adc	bullet_x, y
	sta	bullet_x, y

	lda	bullet_workingval
	ldx	bullet_frame
	jsr	maths_cosine
	ldy	bullet_current	; .Y = Bullet position map offset
	clc
	adc	bullet_y, y
	sta	bullet_y, y

bullets_check_position

	ldx	bullet_current
	lda	bullet_x, x
	;cmp	#$00
	beq	bullets_check_kill
	cmp	#$FF
	beq	bullets_check_kill		; Bullet gone outside of X range

	lda	bullet_y, x
	;cmp	#$00
	beq	bullets_check_kill
	cmp	#199
	beq	bullets_check_kill		; Bullet gone outside of Y range
	
	;lda	bullet_y, x
	tay
	lda	bullet_x, x
	tax
	lda	#$01
	jsr	bitmap_plotpixel		; Draw bullet
	jmp	bullets_check_next

bullets_check_kill
	lda	#$00
	sta	bullet_direction, x	; Remove bullet

bullets_check_next	; Check next bullet, unless we've exhausting our count, or we've hit the end of the 256 bullet block
	inc	bullet_current
	dec	bullet_remain
	beq	bullets_check_finished
	ldx	bullet_current
	;cpx	#$00
	bne	bullets_check_loop

bullets_check_finished	; Finished processing. If we've done all 256 bullets, increase bullet frame
	ldx	bullet_current
	;cpx	#$00
	bne	bullets_check_advover

	inc	bullet_frame
	lda	bullet_frame
	cmp	#$08
	bne	bullets_check_advover
	lda	#$00
	sta	bullet_frame

bullets_check_advover
	rts

bullets_playerbullet	; Player bullets always move up the screen a 4px/frame
	lda	bullet_y, x
	cmp	#$05
	bcc	bullets_check_kill
	sec
	sbc	#$04
	sta	bullet_y, x
	jmp	bullets_check_position


;------------------------------------------------------------------[ Find an empty spot for the CPU to shoot
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : .X = Bullet Index ($00 if none free)
bullets_findcpuempty
	pha
	ldx	#$06
bullets_findcpuempty_loop
	lda	bullet_direction, x
	beq	bullets_findcpuempty_found
	inx
	bne	bullets_findcpuempty_loop
bullets_findcpuempty_found
	pla
	rts

;------------------------------------------------------------------[ Find an empty spot for the player to shoot
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : .X = Bullet Index ($FF if none free)
bullets_findplayerempty
	ldx	#$00
bullets_findplayerempty_loop
	lda	bullet_direction, x
	beq	bullets_findplayerempty_found
	inx
	cpx	#$06
	bne	bullets_findplayerempty_loop
	ldx	#$FF
bullets_findplayerempty_found
	rts


bullet_workingval	.byte	$00
bullet_remain		.byte	$00
bullet_current	.byte	$00
bullet_frame		.byte	$00

bullet_x	
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

bullet_y
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

; -------------------------------------------------------------------[ Bullet Presence/Direction
; -------------------------------------------------------------------[ 0 = No bullet, 1-44 is direction (sub 1 before mapping to bullet_direction_x|y table)
bullet_direction
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte	$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
