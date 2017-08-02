
;------------------------------------------------------------------[ Start a new game
;------------------------------------------------------------------[  Clear scores
;------------------------------------------------------------------[  Set health
;------------------------------------------------------------------[  Switch to bitmap mode
;------------------------------------------------------------------[  Play the game
;------------------------------------------------------------------[  Disable bitmap mode
game_startnewgame
	lda	#00 ; #11
	sta	$D025
	lda	#10
	sta	$D026	; Set sprite multicolours

	jsr	score_resetp1

	jsr	charmap_clearscreen
	jsr	bitmap_enable
	lda	#$7B
	jsr	bitmap_clear

	ldy	#$00
	sty	game_level
game_newgame_drawline
	ldy	game_level
	ldx	#255
	lda	#200
	jsr	bitmap_plotpixel
	inc	game_level
	lda	game_level
	cmp	#200
	bne	game_newgame_drawline
	
	lda	#$00
	sta	bullet_current

	lda	#$04
	sta	player_health	; Set player health to 4, because each level adds 1 health

	lda	#$0A
	sta	game_level

	jsr	game_playroutine

	jsr	bitmap_clear
	jsr	bitmap_disable
	jsr	charmap_clearscreen

	lda	#$00
	sta	$D015	; Disable Sprites
	rts

;------------------------------------------------------------------[ Configure game to current level
;------------------------------------------------------------------[  Give player +1 health for new level (Caps at 8)
;------------------------------------------------------------------[  Copy sprite and health data
;------------------------------------------------------------------[  Set tiger start and target positions
game_configurelevel
	lda	player_health
	cmp	#$08
	bne	game_cfglvl_nohealthboost
	inc	player_health
game_cfglvl_nohealthboost

	ldx	game_level
	lda	game_cfglvl_0Cmul, x
	tax	; .X = Level Index
	ldy	#$00	; .Y = Store Index
game_cfglvl_loop
	lda	tiger_level0_sprites, x
	sta	tiger_sprites, y
	lda	tiger_level0_health, x
	sta	tiger_health, y
	lda	tiger_level0_patterns, x
	sta	tiger_pattern, y
	inx
	iny
	cpy	#$0C
	bne	game_cfglvl_loop
	
	ldx	#$00
	stx	tiger_target_i
game_cfglvl_tpos
	lda #152
	sta	tiger_target_x, x
	txa
	clc
	adc	#154 ; #152
	sta	tiger_curpos_x, x
	lda	game_cfglvl_sty, x
	sta	tiger_target_y, x
	sta	tiger_curpos_y, x
	inx
	cpx	#$04
	bne	game_cfglvl_tpos

	rts

game_cfglvl_0Cmul	.byte	$00, $0C, $18, $24, $30, $3C, $48, $54, $60, $6C, $78, $84, $90, $9C
game_cfglvl_sty	.byte	$18, $10, $08, $00

;------------------------------------------------------------------[ Pre-routine to move to next level
game_play_nextlevel
	lda	game_level
	cmp	#$0A
	beq	game_playroutine
	inc	game_level
;------------------------------------------------------------------[ Play the game
;------------------------------------------------------------------[  Configure current level
;------------------------------------------------------------------[  Check player is alive
;------------------------------------------------------------------[  Check TigerMoth pieces are alive
;------------------------------------------------------------------[  Raster Timings:
;------------------------------------------------------------------[    Configure TigerMoth sprites
;------------------------------------------------------------------[    Configure Player sprites
;------------------------------------------------------------------[    Configure Score sprites
;------------------------------------------------------------------[    Process Bullets
;------------------------------------------------------------------[  Once Per Bullet Pass:
;------------------------------------------------------------------[    Move TigerMoth
;------------------------------------------------------------------[    Move Player
game_playroutine
	jsr	game_configurelevel

game_playloop
	lda	#$00
	sta	$D020	; DEBUG : Black Frame

	jsr	tigermoth_check_health
	;cmp	#$00
	beq	game_play_nextlevel
	
	jsr	timing_getraster
	cmp	#$06
	bcs	game_raster_notmsprite
	jsr	tigermoth_configure_sprites
game_raster_notmsprite
;	cmp	#$38
;	bcc	game_raster_nopsprite
;	cmp	#$42
;	bcs	game_raster_nopsprite
;	jsr	player_configure_sprites
;game_raster_nopsprite
;	cmp	#$50
;	bcc	game_process_frame
;	cmp	#$60
;	bcc	game_loop_configscorea
;	cmp	#$68
;	bcc	game_loop_configscoreb

	cmp	#145
	bcs	game_play_skipframe
game_play_doframe
	jsr	game_process_frame
game_play_skipframe
	lda	player_health
	;cmp	#$00
	bne	game_playloop
game_play_gameover
	rts

game_process_frame
	ldy	#$08
	jsr	bullets_check
	
	lda	bullet_current
	bne	game_process_finish

	jsr	tigermoth_check_health
	jsr	tigermoth_process_movement
	jsr	tigermoth_shoot_pattern
	jsr	player_process_movement
	
game_process_finish
	rts

	.INCLUDE "game_config_player.inc"
	.INCLUDE "game_config_tiger.inc"
	.INCLUDE "game_bullets.inc"

game_level		.byte	$00
player_health	.byte	$00
player_pos_x	.byte	$00
player_pos_y	.byte	$00

tiger_curpos_x	.byte	$00, $00, $00, $00
tiger_curpos_y	.byte	$00, $00, $00, $00
tiger_target_x	.byte	$00, $00, $00, $00
tiger_target_y	.byte	$00, $00, $00, $00
tiger_target_i	.byte	$00

tiger_spriterows	.byte	$00, $00, $00, $00, $00, $00, $00, $00
tiger_spritexmod	.byte	$00, $00, $00, $00, $00, $00, $00, $00

tiger_sprites
	.byte	$00, $00, $00
	.byte	$00, $00, $00
	.byte	$00, $00, $00
	.byte	$00, $00, $00
tiger_health
	.byte	$00, $00, $00
	.byte	$00, $00, $00
	.byte	$00, $00, $00
	.byte	$00, $00, $00
tiger_pattern
	.byte	$00, $00, $00
	.byte	$00, $00, $00
	.byte	$00, $00, $00
	.byte	$00, $00, $00


	.INCLUDE "game_levels.inc"
