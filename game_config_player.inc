
;------------------------------------------------------------------[ Configure sprite pointers, positions and visibility
player_configure_sprites
	lda	#$04
	sta	$D020	; DEBUG

	lda	#$00
	sta	$D01C	; Multicolour sprites

	ldy	#$00	; .Y = Sprite Slot
	ldx	#$00	; .X = TigerMoth Index
	stx	$D015	; Disable all sprites


	rts

;------------------------------------------------------------------[ Update player's position, and shoot bullets
player_process_movement
	lda	#$05
	sta	$D020	; DEBUG

	rts
