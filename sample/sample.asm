; =============================================================================
;	BGM Driver ����T���v��
; -----------------------------------------------------------------------------
;	2020/07/15	t.hara
; =============================================================================

	include		"msx.asm"

	BSAVE_HEADER	start_address, end_address, entry_point

	org			0xA000
start_address::
entry_point::
	call		interrupt_initializer		; ���荞�ݓo�^

play_bgm001::
	ld			hl, bgm001					; �Đ�����BGM�̒u����Ă�A�h���X
	call		bgmdriver_play				; BGM�Đ��J�n
	ret

stop_bgm001::
	call		bgmdriver_stop				; BGM��~
	ret

fadeout_bgm001::
	call		bgmdriver_fadeout			; BGM�����X�ɒ�~ (���ʂ̃t�F�[�h�A�E�g)
	ret

play_se001::
	ld			hl, sound_effect001			; �Đ�������ʉ��f�[�^�̒u����Ă�A�h���X
	call		bgmdriver_play_sound_effect	; ���ʉ��Đ��J�n
	ret

play_se002::
	ld			hl, sound_effect002			; �Đ�������ʉ��f�[�^�̒u����Ă�A�h���X
	call		bgmdriver_play_sound_effect	; ���ʉ��Đ��J�n
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
		db		32					; priority [�����������D��]
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
		db		128				; priority [�����������D��]
		db		BGM_SE_VOL
		db		12
		db		BGM_SE_FREQ
		dw		32768				; 32768 �ɂ���� TONE OFF
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
