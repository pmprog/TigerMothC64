

;------------------------------------------------------------------[ Enable Bitmap Mode
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
bitmap_enable
	lda	$D018
	ora	#8
	sta	$D018
	lda	$D011
	ora	#32
	sta	$D011
	rts

;------------------------------------------------------------------[ Disable Bitmap Mode
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
bitmap_disable
	lda	$D018
	and	#247
	sta	$D018
	lda	$D011
	and	#223
	sta	$D011
	rts

;------------------------------------------------------------------[ Clear Bitmap Screen
;------------------------------------------------------------------[ In  : .A = Colour Pair (Upper 4 bits = Set Colour, Lower 4 bits = Background Colour)
;------------------------------------------------------------------[ Out : N/A
bitmap_clear
	sta	bitmap_plotisset
	lda	#$1F
	sta	(bitmap_clearaddressline)+2
	ldx	#$00
	lda	#$00
	ldy	#32
bitmap_clearloop
	inc	(bitmap_clearaddressline)+2
bitmap_clearaddressline
	sta	$2000, x
	inx
	bne	bitmap_clearaddressline
	dey
	bne	bitmap_clearloop
	ldx	#$00
	lda	bitmap_plotisset ; #$30
bitmap_clearcolumn
	sta	$0400, x
	sta	$0500, x
	sta	$0600, x
	sta	$0700, x
	inx
	bne	bitmap_clearcolumn
	rts

;------------------------------------------------------------------[ Draw/Clear Pixel On Screen
;------------------------------------------------------------------[ In  : .X, .Y = Screen X/Y (X only up to 255); .A = Draw/Clear (Draw != 0, Clear == 0)
;------------------------------------------------------------------[ Out : N/A
bitmap_plotpixel
	sta	bitmap_plotisset
	stx	bitmap_plotx
	tya
	and	#$07
	sta	bitmap_plotpr
	tya
	lsr	A
	lsr	A
	lsr	A
	tax
	ldy	bitmap_plotpx_pmhi, x
	lda	bitmap_plotpx_pmlo, x
	tax
	clc
	adc	bitmap_plotpr
	sta	bitmap_plot_load+1
	tya
	adc	#$20
	sta	bitmap_plot_load+2
	lda	bitmap_plotx
	and	#$07
	sta	bitmap_plotpc
	lda	bitmap_plotx
	and	#$F8
	adc	bitmap_plot_load+1
	sta	bitmap_plot_load+1
	sta	bitmap_plot_save+1
	bcc	bitmap_plot_noinchi
	inc	bitmap_plot_load+2
bitmap_plot_noinchi
	lda	bitmap_plot_load+2
	sta	bitmap_plot_save+2	; Copy plotting address from load to save
	lda	bitmap_plotisset
	beq	bitmap_plot_wantclear
	ldx	bitmap_plotpc
	lda	bitmap_plotsetpixel,x
	ldx	#$09
	jmp	bitmap_plot_pointonscreen
bitmap_plot_wantclear
	ldx	bitmap_plotpc
	lda	bitmap_plotclearpixel,x
	ldx	#$29
bitmap_plot_pointonscreen
	stx	bitmap_plot_flag
	sta	bitmap_plot_flag+1
bitmap_plot_load
	lda	$2000
bitmap_plot_flag
	ora	#01
bitmap_plot_save
	sta	$2000
	rts


bitmap_plotx					.byte	00
bitmap_plotpr					.byte	00
bitmap_plotpc					.byte	00
bitmap_plotisset			.byte	00
bitmap_plotsetpixel		.byte	$80, $40, $20, $10, $08, $04, $02, $01
bitmap_plotclearpixel	.byte	$7F, $BF, $DF, $EF, $F7, $FB, $FD, $FE
bitmap_plotpx_pmhi		.byte	$00, $01, $02, $03, $05, $06, $07, $08, $0A, $0B, $0C, $0D, $0F, $10, $11, $12, $14, $15, $16, $17, $19, $1A, $1B, $1C, $1E, $1F
bitmap_plotpx_pmlo		.byte	$00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40

