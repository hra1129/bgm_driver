; --------------------------------------------------------------------
;	bgm_driver を z88dk から利用するためのサンプル
; ====================================================================
;	2021/04/23	HRA!
; --------------------------------------------------------------------

		include "msx.asm"

		org		0x4000					; BGMDRV.BIN は 0x4000番地～ に読み込まれることを前提
		include	"bgmdriver.asm"

; --------------------------------------------------------------------
		scope	bgmdrv_setup_htimi
bgmdrv_setup_htimi::
		call	bgmdriver_initialize
		; H.TIMI に書かれている内容をバックアップする
		di
		ld		hl, H_TIMI
		ld		de, H_TIMI_BACKUP
		ld		bc, 5
		ldir

		; 新しい H.TIMIをセットする
		ld		a, 0xC3					; JP命令
		ld		[ H_TIMI ], a
		ld		hl, htimi_handler
		ld		[ H_TIMI + 1 ], hl
		ei
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_restore_htimi
bgmdrv_restore_htimi::
		; H.TIMI に書かれていた内容を復元する
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
