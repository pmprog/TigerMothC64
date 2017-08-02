
;------------------------------------------------------------------[ Clears Player 1's score
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
score_resetp1
	lda	#$00
	ldx	#$00
score_resetp1_loop
	sta	score_player1, x
	inx
	cpx	#$06
	bne	score_resetp1_loop
	rts

;------------------------------------------------------------------[ Adds value to Player 1's score
;------------------------------------------------------------------[ In  : .A = Score to Add, .Y = ^10
;------------------------------------------------------------------[ Out : N/A
score_addp1
	clc
	adc	score_player1, y
	sta	score_player1, y
	cmp	#$0A
	bcc	score_addp1_end
	clc
	sbc	#$09
	sta	score_player1, y
	cpy	#05
	beq	score_addp1_end
	lda	#$01
	iny
	jsr score_addp1
score_addp1_end
	rts

score_player1		.byte	$00, $00, $00, $00, $00, $00
;score_player2		.byte	$00, $00, $00, $00, $00, $00
