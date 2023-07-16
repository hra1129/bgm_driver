// --------------------------------------------------------------------
//	bgm_driver を z88dk から利用するためのサンプル
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#ifndef __BGMDRIVER_H__
#define __BGMDRIVER_H__

// --------------------------------------------------------------------
//	BGMDRV.BIN を使える状態にする
//	これを呼んだ場合、DOSへ戻る前に bgmdrv_term() を呼ばねばならない
// --------------------------------------------------------------------
int bgmdrv_init( void );

// --------------------------------------------------------------------
//	BGMDRV.BIN の後始末をする
// --------------------------------------------------------------------
void bgmdrv_term( void );

// --------------------------------------------------------------------
//	演奏を開始する
//	input)
//		p_data .... 曲データへのポインタ
//	output)
//		None
// --------------------------------------------------------------------
extern void (* bgmdrv_play)( void *p_data );

// --------------------------------------------------------------------
//	演奏を停止する
//	input)
//		None
//	output)
//		None
// --------------------------------------------------------------------
extern void (* bgmdrv_stop)( void );

// --------------------------------------------------------------------
//	演奏中か確認する
//	input)
//		None
//	output)
//		0 ... 停止中
//		1 ... 演奏中
// --------------------------------------------------------------------
extern int (* bgmdrv_check_play)( void );

// --------------------------------------------------------------------
//	曲をフェードアウトする
//	input)
//		speed ... 1〜255 でフェードアウトスピードを指定する。1が最低速。255が最高速。0だとフェードしない。
//	output)
//		0 ... 停止中
//		1 ... 演奏中
// --------------------------------------------------------------------
extern void (* bgmdrv_fade_out)( unsigned int speed );

// --------------------------------------------------------------------
//	効果音を再生する
//	input)
//		p_data .... 効果音データへのポインタ
//	output)
//		None
// --------------------------------------------------------------------
extern void (* bgmdrv_play_se)( void *p_data );

// --------------------------------------------------------------------
//	メモリへファイルを読み込む
//	input)
//		p_memory ...... 読み込み先メモリのアドレス
//		p_file_name ... ファイル名
//		file_size ..... 読み込むサイズ ( 0 にすると、ファイル全体を読む )
//	output)
//		0 ... 失敗
//		1 ... 成功
// --------------------------------------------------------------------
int bgmdrv_load_file( void *p_memory, const char *p_file_name, size_t file_size );

// --------------------------------------------------------------------
//	効果音データ作成用
#define BGM_SE_FREQ			0
#define BGM_SE_VOL			1
#define BGM_SE_NOISE_FREQ	2
#define BGM_SE_WAIT			3
#define BGM_SE_END			4

#endif
