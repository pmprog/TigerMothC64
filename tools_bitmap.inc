

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
	cpy	#200
	bcc	bitmap_plot_yaccepted
	rts
bitmap_plot_yaccepted
	sta	bitmap_plotisset
	stx	bitmap_plotx
	
	lda	bitmap_plotpx_yhi, y
	sta	bitmap_plot_load+2
	;lda	bitmap_plotpx_ylo, y
	;sta	bitmap_plot_load+1	; Save calculated address
	
	txa	; lda	bitmap_plotx
	and	#$07
	sta	bitmap_plotpc	; .A = InChar X (0-7)
	lda	bitmap_plotx
	and	#$F8	; Get Screen Character X (*8, as that's reserved for InChar Y)
	;adc	bitmap_plot_load+1	; Add to saved address
	adc	bitmap_plotpx_ylo, y
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
bitmap_plot_load	; Actually perform pixel plot
	lda	$2000	; Address is modified by above code
bitmap_plot_flag
	ora	#01	; Command is modified by above code
bitmap_plot_save
	sta	$2000	; Address is modified by above code
	rts


bitmap_plotx					.byte	00
bitmap_plotpr					.byte	00
bitmap_plotpc					.byte	00
bitmap_plotisset			.byte	00
bitmap_plotsetpixel		.byte	$80, $40, $20, $10, $08, $04, $02, $01
bitmap_plotclearpixel	.byte	$7F, $BF, $DF, $EF, $F7, $FB, $FD, $FE
bitmap_plotpx_pmhi		.byte	$00, $01, $02, $03, $05, $06, $07, $08, $0A, $0B, $0C, $0D, $0F, $10, $11, $12, $14, $15, $16, $17, $19, $1A, $1B, $1C, $1E, $1F
bitmap_plotpx_pmlo		.byte	$00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40, $80, $C0, $00, $40


bitmap_plotpx_yhi
	.byte	$20, $20, $20, $20, $20, $20, $20, $20, $21, $21, $21, $21, $21, $21, $21, $21, $22, $22, $22, $22, $22, $22, $22, $22, $23, $23, $23, $23, $23, $23, $23, $23
	.byte	$25, $25, $25, $25, $25, $25, $25, $25, $26, $26, $26, $26, $26, $26, $26, $26, $27, $27, $27, $27, $27, $27, $27, $27, $28, $28, $28, $28, $28, $28, $28, $28
	.byte	$2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2B, $2B, $2B, $2B, $2B, $2B, $2B, $2B, $2C, $2C, $2C, $2C, $2C, $2C, $2C, $2C, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D
	.byte	$2F, $2F, $2F, $2F, $2F, $2F, $2F, $2F, $30, $30, $30, $30, $30, $30, $30, $30, $31, $31, $31, $31, $31, $31, $31, $31, $32, $32, $32, $32, $32, $32, $32, $32
	.byte	$34, $34, $34, $34, $34, $34, $34, $34, $35, $35, $35, $35, $35, $35, $35, $35, $36, $36, $36, $36, $36, $36, $36, $36, $37, $37, $37, $37, $37, $37, $37, $37
	.byte	$39, $39, $39, $39, $39, $39, $39, $39, $3A, $3A, $3A, $3A, $3A, $3A, $3A, $3A, $3B, $3B, $3B, $3B, $3B, $3B, $3B, $3B, $3C, $3C, $3C, $3C, $3C, $3C, $3C, $3C
	.byte	$3E, $3E, $3E, $3E, $3E, $3E, $3E, $3E
bitmap_plotpx_ylo
	.byte	$00, $01, $02, $03, $04, $05, $06, $07, $40, $41, $42, $43, $44, $45, $46, $47, $80, $81, $82, $83, $84, $85, $86, $87, $C0, $C1, $C2, $C3, $C4, $C5, $C6, $C7
	.byte	$00, $01, $02, $03, $04, $05, $06, $07, $40, $41, $42, $43, $44, $45, $46, $47, $80, $81, $82, $83, $84, $85, $86, $87, $C0, $C1, $C2, $C3, $C4, $C5, $C6, $C7
	.byte	$00, $01, $02, $03, $04, $05, $06, $07, $40, $41, $42, $43, $44, $45, $46, $47, $80, $81, $82, $83, $84, $85, $86, $87, $C0, $C1, $C2, $C3, $C4, $C5, $C6, $C7
	.byte	$00, $01, $02, $03, $04, $05, $06, $07, $40, $41, $42, $43, $44, $45, $46, $47, $80, $81, $82, $83, $84, $85, $86, $87, $C0, $C1, $C2, $C3, $C4, $C5, $C6, $C7
	.byte	$00, $01, $02, $03, $04, $05, $06, $07, $40, $41, $42, $43, $44, $45, $46, $47, $80, $81, $82, $83, $84, $85, $86, $87, $C0, $C1, $C2, $C3, $C4, $C5, $C6, $C7
	.byte	$00, $01, $02, $03, $04, $05, $06, $07, $40, $41, $42, $43, $44, $45, $46, $47, $80, $81, $82, $83, $84, $85, $86, $87, $C0, $C1, $C2, $C3, $C4, $C5, $C6, $C7
	.byte	$00, $01, $02, $03, $04, $05, $06, $07

