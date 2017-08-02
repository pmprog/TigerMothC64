VIC	=	$D000

	.org	$07FE
	.word	$0800			; Program Startup

; -------------------------------------------------------------------[ Boot loader
	.byte	$00, $0C, $08, $66, $19, $9E, $32, $30, $36, $34

	.org		$0810
	
	jmp	start

	.org		$0840

	.INCLUDE	"game_sprites.inc"

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Application
	.org    $4000

start
	.INCLUDE	"screen_polymath.inc"

menu_loop
	jsr	titlescreen_start

;	cmp	#$00
;	bne	game_notplaying
	jsr	game_startnewgame

	jmp	menu_loop



	.INCLUDE	"screen_title.inc"
	.INCLUDE	"screen_game.inc"


	.INCLUDE	"tools_bitmap.inc"
	.INCLUDE	"tools_charmap.inc"
	.INCLUDE	"tools_maths.inc"
	.INCLUDE	"tools_scoring.inc"
	.INCLUDE	"tools_timing.inc"

	.end
