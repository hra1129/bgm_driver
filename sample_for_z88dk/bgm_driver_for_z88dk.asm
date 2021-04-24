; --------------------------------------------------------------------
;	bgm_driver �� z88dk ���痘�p���邽�߂̃T���v��
; ====================================================================
;	2021/04/23	HRA!
; --------------------------------------------------------------------

		include "msx.asm"
		include "config.txt"

		org		start_address					; BGMDRV.BIN �� start_address�` �ɓǂݍ��܂�邱�Ƃ�O��

		include	"bgmdriver.asm"

; --------------------------------------------------------------------
		scope	bgmdrv_setup_htimi
bgmdrv_setup_htimi::
		call	bgmdriver_initialize
		; H.TIMI �ɏ�����Ă�����e���o�b�N�A�b�v����
		di
		ld		hl, H_TIMI
		ld		de, H_TIMI_BACKUP
		ld		bc, 5
		ldir

		if htimi_handler < 0x4000
			ld		a, [ramad0]
		elseif htimi_handler < 0x8000
			ld		a, [ramad1]
		elseif htimi_handler < 0xC000
			ld		a, [ramad2]
		else
			ld		a, [ramad3]
		endif
		ld		[h_timi_new_slot], a

		; �V���� H.TIMI���Z�b�g����
		ld		hl, H_TIMI_NEW
		ld		de, H_TIMI
		ld		bc, 5
		ldir
		ei
		ret

H_TIMI_NEW:
		rst		0x30
h_timi_new_slot:
		db		0
		dw		htimi_handler
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_restore_htimi
bgmdrv_restore_htimi::
		; H.TIMI �ɏ�����Ă������e�𕜌�����
		di
		ld		hl, H_TIMI_BACKUP
		ld		de, H_TIMI
		ld		bc, 5
		ldir
		ei
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_play
bgmdrv_play::
		pop		bc		; �߂�A�h���X
		pop		hl		; ��1����
		push	hl
		push	bc
		call	bgmdriver_play
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_play_se
bgmdrv_play_se::
		pop		bc		; �߂�A�h���X
		pop		hl		; ��1����
		push	hl
		push	bc
		call	bgmdriver_play_sound_effect
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_fade_out
bgmdrv_fade_out::
		pop		bc		; �߂�A�h���X
		pop		hl		; ��1����
		push	hl
		push	bc
		ld		a, h
		ld		a, 255
		jr		nz, skip1
		ld		a, l
skip1:
		call	bgmdriver_fadeout
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_check_play
bgmdrv_check_play::
		call	bgmdriver_check_playing
		ld		hl, 0
		ret		z		; ��~��
		inc		hl
		ret				; ���t��
		endscope

; --------------------------------------------------------------------
		scope	htimi_handler
htimi_handler::
		push	af
		call	bgmdriver_interrupt_handler
		pop		af
H_TIMI_BACKUP::
		ret
		ret
		ret
		ret
		ret
		endscope
end_address::

driver_size := end_address - start_address
