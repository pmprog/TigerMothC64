
;------------------------------------------------------------------[ Waits for a number of frames
;------------------------------------------------------------------[ In  : .A = Frames to wait
;------------------------------------------------------------------[ Out : N/A
timing_waitframes
	tay
timing_wf_nodecrease
	ldx	$D012
	bne	timing_wf_nodecrease
	dey
	bne	timing_wf_nodecrease
timing_wf_end
	rts


;------------------------------------------------------------------[ Gets raster position
;------------------------------------------------------------------[ In  : N/A
;------------------------------------------------------------------[ Out : .A = Raster position (Upper 8 bits of 9 bit value)
timing_getraster
	lda	$D012
	lsr	A
	sta	timing_grl
	lda	$D011
	and	#$80
	clc
	adc	timing_grl	; .A = current raster
	rts
timing_grl	.byte	$00

