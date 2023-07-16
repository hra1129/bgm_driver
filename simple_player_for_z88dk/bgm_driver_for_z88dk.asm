; --------------------------------------------------------------------
;	bgm_driver を z88dk から利用するためのサンプル
; ====================================================================
;	2021/04/23	HRA!
; --------------------------------------------------------------------

		include "msx.asm"
		include "config.txt"

		org		start_address					; BGMDRV.BIN は start_address〜 に読み込まれることを前提

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

		; 新しい H.TIMIをセットする
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
		pop		bc		; 戻りアドレス
		pop		hl		; 第1引数
		push	hl
		push	bc
		call	bgmdriver_play
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_play_se
bgmdrv_play_se::
		pop		bc		; 戻りアドレス
		pop		hl		; 第1引数
		push	hl
		push	bc
		call	bgmdriver_play_sound_effect
		ret
		endscope

; --------------------------------------------------------------------
		scope	bgmdrv_fade_out
bgmdrv_fade_out::
		pop		bc		; 戻りアドレス
		pop		hl		; 第1引数
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
		ret		z		; 停止中
		inc		hl
		ret				; 演奏中
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
