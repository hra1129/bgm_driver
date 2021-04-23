; =============================================================================
;	BGM Driver 動作サンプル
; -----------------------------------------------------------------------------
;	2020/07/15	t.hara
; =============================================================================

	include		"msx.asm"

	bsave_header	start_address, end_address, entry_point

	org			0xA000
start_address::
entry_point::
	call		interrupt_initializer		; 割り込み登録

play_bgm001::
	ld			hl, bgm001					; 再生するBGMの置かれてるアドレス
	call		bgmdriver_play				; BGM再生開始
	ret

stop_bgm001::
	call		bgmdriver_stop				; BGM停止
	ret

fadeout_bgm001::
	call		bgmdriver_fadeout			; BGMを徐々に停止 (音量のフェードアウト)
	ret

play_se001::
	ld			hl, sound_effect001			; 再生する効果音データの置かれてるアドレス
	call		bgmdriver_play_sound_effect	; 効果音再生開始
	ret

play_se002::
	ld			hl, sound_effect002			; 再生する効果音データの置かれてるアドレス
	call		bgmdriver_play_sound_effect	; 効果音再生開始
	ret

; =============================================================================
;	initialize for interrupt
; =============================================================================
	scope		interrupt_initializer
interrupt_initializer::
	; initialize interrupt hooks
	di
	;	h_timi
	ld			hl, h_timi							; Source address
	ld			de, h_timi_next						; Destination address
	ld			bc, 5								; Transfer length
	ldir											; Block transfer

	ld			a, 0xC3								; 'jp xxxx' code
	ld			[h_timi], a							; hook update
	ld			hl, h_timi_interrupt_handler		; set interrupt handler
	ld			[h_timi + 1], hl
	ei
	ret
	endscope

; =============================================================================
;	interrupt handler
; =============================================================================
	scope		h_timi_interrupt_handler
h_timi_interrupt_handler::
	call		bgmdriver_interrupt_handler
h_timi_next::
	ret
	ret
	ret
	ret
	ret
	endscope

; =============================================================================
;	BGM driver
; =============================================================================
	include		"bgmdriver.asm"
bgm001::
	include		"bgm.asm"
sound_effect001:
		db		32					; priority [小さい方が優先]
		db		BGM_SE_VOL
		db		12
		db		BGM_SE_FREQ
		dw		30
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		25
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		20
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		10
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		30
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		25
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		20
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_FREQ
		dw		10
		db		BGM_SE_WAIT
		db		1
		db		BGM_SE_END
sound_effect002:
		db		128				; priority [小さい方が優先]
		db		BGM_SE_VOL
		db		12
		db		BGM_SE_FREQ
		dw		32768				; 32768 にすると TONE OFF
		db		BGM_SE_NOISE_FREQ
		db		20 + 0x80
		db		BGM_SE_WAIT
		db		5
		db		BGM_SE_NOISE_FREQ
		db		8 + 0x80
		db		BGM_SE_WAIT
		db		5
		db		BGM_SE_NOISE_FREQ
		db		28 + 0x80
		db		BGM_SE_WAIT
		db		5
		db		BGM_SE_NOISE_FREQ
		db		12 + 0x80
		db		BGM_SE_WAIT
		db		5
		db		BGM_SE_NOISE_FREQ
		db		31 + 0x80
		db		BGM_SE_WAIT
		db		10
		db		BGM_SE_NOISE_FREQ
		db		15 + 0x80
		db		BGM_SE_WAIT
		db		10
		db		BGM_SE_END
end_address::
