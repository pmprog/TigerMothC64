
;------------------------------------------------------------------[ Configure sprite pointers, positions and visibility
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
tigermoth_configure_sprites
	lda	#$01
	sta	$D020	; DEBUG
	
	lda	#$FF
	sta	$D01C	; Multicolour sprites

	ldy	#$00	; .Y = Sprite Slot
	ldx	#$00	; .X = TigerMoth Index
	stx	$D015	; Disable all sprites
tigermoth_sprite_unitconfig
	lda	tiger_sprites, x
	beq	tigermoth_sprite_unitnext
	
	sta	$07F8, y	; Sprite Pointer
	lda	#$09
	sta	$D027, y	; Colour

	lda	tigermoth_cfg_sprflg, y
	ora	$D015
	sta	$D015	; Enable sprite
	
	lda	tigermoth_cfg_xmod, x
	sta	tiger_spritexmod, y
	lda	tigermoth_cfg_rowlist, x
	sta	tiger_spriterows, y
	
	sty	tigermoth_cfg_curslot

	; Sprite(.Y).X = tiger_curpos_x(.A) + tigermoth_cfg_xmod(.X)
	tay
	lda	tiger_curpos_x, y	; Tiger(Row).X
	clc
	adc	tigermoth_cfg_xmod, x	; .A = Tiger(Row).X + Tiger(Index).XMod
	ldy	tigermoth_cfg_curslot
	sta	tigermoth_cfg_temp
	tya
	asl	A
	tay
	lda	tigermoth_cfg_temp
	sta	$D000, y
	
	; Sprite(.Y).Y = tiger_curpos_y(.A)
	ldy	tigermoth_cfg_curslot
	lda	tiger_spriterows, y
	tay
	lda	tiger_curpos_y, y	; .A = Tiget(Row).Y
	ldy	tigermoth_cfg_curslot
	sta	tigermoth_cfg_temp
	tya
	asl	A
	tay
	lda	tigermoth_cfg_temp
	sta	$D001, y
	
	ldy	tigermoth_cfg_curslot
	iny

tigermoth_sprite_unitnext
	inx
	cpx	#$0C
	bne	tigermoth_sprite_unitconfig

	rts

tigermoth_cfg_temp		.byte	$00
tigermoth_cfg_curslot	.byte	$00
tigermoth_cfg_rowlist	.byte	$00, $00, $00, $01, $01, $01, $02, $02, $02, $03, $03, $03
tigermoth_cfg_xmod		.byte	$E8, $00, $18, $E8, $00, $18, $E8, $00, $18, $E8, $00, $18
tigermoth_cfg_sprflg	.byte	$01, $02, $04, $08, $10, $20, $40, $80
tigermoth_shot_counter	.byte	$00
tigermoth_shot_wavepos	.byte	$00
tigermoth_shot_wave		.byte	28, 31, 34, 37, 40, 37, 31

;------------------------------------------------------------------[ Update TigerMoth position, and check for new targets
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
tigermoth_process_movement
	lda	#$02
	sta	$D020	; DEBUG

	
	ldx	#$00
tigermoth_move_loop

	lda	tiger_target_x, x
	cmp	tiger_curpos_x, x
	bne	tigermoth_move_gomove
	lda	tiger_target_y, x
	cmp	tiger_curpos_y, x
	bne	tigermoth_move_gomove
	
	txa
	cmp	#$00
	bne	tigermoth_move_histexist
	
	ldy	tiger_target_i
	lda	tiger_level0_path_x, y
	sta	tiger_target_x, x
	lda	tiger_level0_path_y, y
	sta	tiger_target_y, x
	
	iny
	cpy	#$08
	bne tigermoth_move_cleantarget
	ldy	#$00
tigermoth_move_cleantarget
	sty	tiger_target_i
	
	jmp	tigermoth_move_finishedpiece
	
tigermoth_move_histexist
	tay
	dey
	lda	tiger_target_x, y
	sta	tiger_target_x, x
	lda	tiger_target_y, y
	sta	tiger_target_y, x
	jmp	tigermoth_move_finishedpiece

tigermoth_move_gomove	
	lda	tiger_target_x, x
	cmp	tiger_curpos_x, x
	beq	tigermoth_move_noxmove
	bcc	tigermoth_move_xneg
	inc	tiger_curpos_x, x
	jmp	tigermoth_move_noxmove
tigermoth_move_xneg
	dec	tiger_curpos_x, x
tigermoth_move_noxmove

	lda	tiger_target_y, x
	cmp	tiger_curpos_y, x
	beq	tigermoth_move_noymove
	bcc	tigermoth_move_yneg
	inc	tiger_curpos_y, x
	jmp	tigermoth_move_noymove
tigermoth_move_yneg
	dec	tiger_curpos_y, x
tigermoth_move_noymove

tigermoth_move_finishedpiece
	inx
	cpx	#$04
	bne	tigermoth_move_loop
	rts
	
;------------------------------------------------------------------[ Process TigerMoth bullet patterns
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : N/A
tigermoth_shoot_pattern
	inc	tigermoth_shot_counter
	ldy	#$00
tigermoth_move_shooting_loop
	lda	tiger_pattern, y
	beq	tigermoth_move_shooting_next
	
	cmp	#$01
	bne	tigermoth_move_shooting_c2

	lda	tigermoth_shot_counter	; Pattern 1 - New bullet south every 16 frames
	and	#$07
	bne	tigermoth_move_shooting_next
	lda	#34
	jsr	tigermoth_shoot_bullet
	jmp	tigermoth_move_shooting_next
tigermoth_move_shooting_c2
	
	
	cmp	#$02
	bne	tigermoth_move_shooting_c3

	lda	tigermoth_shot_counter	; Pattern 2 - New quarter arc south every 64 frames
	and	#$1F
	bne	tigermoth_move_shooting_next
	lda	#28
tigermoth_move_shooting_c2go
	pha
	jsr	tigermoth_shoot_bullet
	pla
	clc
	adc	#$01
	cmp	#42
	bne	tigermoth_move_shooting_c2go
	jmp	tigermoth_move_shooting_next
tigermoth_move_shooting_c3
	
	cmp	#$03
	bne	tigermoth_move_shooting_c4

	lda	tigermoth_shot_counter	; Pattern 3 - Wave backwards and forwards
	and	#$03
	bne	tigermoth_move_shooting_next

	ldx	tigermoth_shot_wavepos
	inx
	cpx	#08
	bne	tigermoth_move_c4_clear
	ldx #$00
	sta	tigermoth_shot_wavepos
tigermoth_move_c4_clear
	lda	tigermoth_shot_wave, x
	jsr	tigermoth_shoot_bullet

	jmp	tigermoth_move_shooting_next
tigermoth_move_shooting_c4
	
tigermoth_move_shooting_next
	iny
	cpy	#$0C
	bne	tigermoth_move_shooting_loop

	rts

;------------------------------------------------------------------[ Shoot bullets
;------------------------------------------------------------------[ In  : .Y = Piece Index, .A = Direction
;------------------------------------------------------------------[ Out : N/A
tigermoth_shoot_bullet
	jsr	bullets_findcpuempty
	cpx	#$00
	beq	tigermoth_shoot_bullet_fin
	sta	bullet_direction, x
	
	stx	tigermoth_cfg_temp	; tigermoth_cfg_temp = Bullet Index

	lda	tiger_block_rows, y
	tax	; .X = Row index
	lda	tiger_curpos_x, x	; .A = X of TigerMoth row
	clc
	adc	tiger_block_columns, y	; .A  = X of TigerMoth piece
	ldx	tigermoth_cfg_temp
	sta	bullet_x, x
	
	lda	tiger_block_rows, y
	tax	; .X = Row index
	lda	tiger_curpos_y, x	; .A = Y of TigerMoth row
	sec
	sbc	#28
	ldx	tigermoth_cfg_temp
	sta	bullet_y, x
	
tigermoth_shoot_bullet_fin
	rts

;------------------------------------------------------------------[ Check TigerMoth Health
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : .A == 0, All TM pieces dead
tigermoth_check_health
	lda	#$03
	sta	$D020	; DEBUG

	ldx	#$00
tigermoth_health_unitcheck
	lda	tiger_health, x
	;cmp	#$00
	bne	tigermoth_health_unitnext
	
	; TODO: Maybe in the future remove TigerMoth piece, and spawn an explosion
	;lda	#$00
	sta	tiger_sprites, x
	sta	tiger_pattern, x

tigermoth_health_unitnext
	inx
	cpx	#$0C
	bne	tigermoth_health_unitcheck

	lda	tiger_health+1
	rts
