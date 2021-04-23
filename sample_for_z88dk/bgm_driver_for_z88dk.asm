; --------------------------------------------------------------------
;	bgm_driver �� z88dk ���痘�p���邽�߂̃T���v��
; ====================================================================
;	2021/04/23	HRA!
; --------------------------------------------------------------------

		include "msx.asm"

		org		0x4000					; BGMDRV.BIN �� 0x4000�Ԓn�` �ɓǂݍ��܂�邱�Ƃ�O��
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

		; �V���� H.TIMI���Z�b�g����
		ld		a, 0xC3					; JP����
		ld		[ H_TIMI ], a
		ld		hl, htimi_handler
		ld		[ H_TIMI + 1 ], hl
		ei
		ret
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
		ld		hl, 0x6000
		call	bgmdriver_play
		ret
		endscope

; --------------------------------------------------------------------
		scope	htimi_handler
htimi_handler::
		call	bgmdriver_interrupt_handler
H_TIMI_BACKUP::
		ret
		ret
		ret
		ret
		ret
		endscope
