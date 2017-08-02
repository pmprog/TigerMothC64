
;------------------------------------------------------------------[ Clears the character mode screen
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
charmap_clearscreen
	ldx	#$00
charmap_ccs_clear
	lda	#$20
	sta	$0400, x
	sta	$0500, x
	sta	$0600, x
	sta	$06e8, x
	inx
	bne	charmap_ccs_clear
	rts
