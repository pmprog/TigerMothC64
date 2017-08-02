
	; Clear character data
	jsr	clearcharscreen

	; Set black screen
	ldx	#00
	stx	$D020
	stx	$D021

	lda	#40
	jsr	waitframes

	; Load sprites
	ldx	#$00
pmlogo_loadsprites
	lda	pmlogo_sprites, x
	sta	$3C00, x
	inx
	bne	pmlogo_loadsprites
	
pmlogo_configuresprites
	; Set sprite pointers
	lda	#240
	sta	$07F8
	lda	#241
	sta	$07F9
	lda	#242
	sta	$07FA
	lda	#243
	sta	$07FB

	; Set sprite positions
	lda	#160
	sta	$D000
	lda	#100
	sta	$D001
	lda	#184
	sta	$D002
	lda	#100
	sta	$D003
	lda	#160
	sta	$D004
	lda	#121
	sta	$D005
	lda	#184
	sta	$D006
	lda	#121
	sta	$D007

	; Set sprites to black
	lda	#$01
	sta	$D027
	sta	$D028
	sta	$D029
	sta	$D02A
	
	; Enable Sprites
	lda	#$0F
	sta	$D015
	
	ldx	pmlogo_idx
pmlogo_fadein
	lda	pmlogo_col, x
	sta	$D027
	sta	$D028
	sta	$D029
	sta	$D02A
	; Delay between shades
	lda	#50
	jsr	waitframes
	; Advance shade
	ldx	pmlogo_idx
	inx
	stx	pmlogo_idx
	cpx	#5
	bne	pmlogo_fadein


	lda	#250
	jsr	waitframes


	ldx	pmlogo_idx
pmlogo_fadeout
	lda	pmlogo_col, x
	sta	$D027
	sta	$D028
	sta	$D029
	sta	$D02A
	; Delay between shades
	lda	#50
	jsr	waitframes
	; Advance shade
	ldx	pmlogo_idx
	dex
	stx	pmlogo_idx
	cpx	#$FF
	bne	pmlogo_fadeout
	
	jmp	pmlogo_finish


pmlogo_idx	.byte	$00
pmlogo_col	.byte	$00, $0B, $0C, $0F, $01, $01

pmlogo_sprites
pmlogo_1	.byte	$0f,$ff,$ff,$3f,$ff,$ff,$7f,$ff,$ff,$70,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$00,$e0,$00,$01,$00
pmlogo_2	.byte	$ff,$ff,$f0,$ff,$ff,$fc,$ff,$ff,$fe,$00,$00,$0e,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$0f,$7f,$ff,$ff,$80,$20,$0f,$00,$20,$07,$00
pmlogo_3	.byte	$e0,$00,$01,$e0,$00,$01,$e0,$00,$01,$e0,$00,$01,$e0,$00,$01,$e0,$00,$01,$e0,$00,$01,$f0,$00,$03,$ff,$ff,$ff,$f0,$07,$01,$e0,$82,$08,$e0,$82,$08,$e0,$02,$01,$e0,$02,$00,$e0,$06,$00,$e0,$7e,$00,$e0,$7e,$08,$70,$ff,$09,$7f,$ff,$ff,$3f,$ff,$ff,$0f,$ff,$ff,$00
pmlogo_4	.byte	$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$00,$00,$07,$04,$01,$07,$04,$01,$07,$84,$01,$0f,$ff,$ff,$ff,$c0,$70,$0f,$80,$20,$07,$84,$20,$07,$84,$20,$7f,$80,$20,$87,$80,$20,$87,$80,$20,$07,$80,$20,$07,$c0,$70,$0e,$ff,$ff,$fe,$ff,$ff,$fc,$ff,$ff,$f0,$00

pmlogo_finish
	; Enable Sprites
	lda	#$00
	sta	$D015
