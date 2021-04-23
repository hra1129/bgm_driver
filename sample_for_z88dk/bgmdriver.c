// --------------------------------------------------------------------
//	bgm_driver を z88dk から利用するためのサンプル
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include "bgmdriver.h"

static const unsigned int bgm_driver_base_address = 0x4000;
static const unsigned int bgm_data_address = 0x6000;

// zma.sym から値をコピペ
void (* bgmdrv_setup_htimi)( void )		= 0x4693;
void (* bgmdrv_restore_htimi)( void )	= 0x46af;
void (* bgmdriver_play)( void )			= 0x46bd;
void (* bgmdrv_stop)( void )			= 0x404b;
int  (* bgmdrv_check_play)( void )		= 0x46c4;

// --------------------------------------------------------------------
int bgmdrv_init( void ) {
	FILE *p_file;
	char *p;
	volatile int i;

	// BGMDRV.BIN を 0x4000番地～ に読み込む
	p_file = fopen( "BGMDRV.BIN", "rb" );
	if( p_file == NULL ) {
		printf( "[ERROR] Cannot read BGMDRV.BIN.\n" );
		return 0;
	}
	p = (char*)bgm_driver_base_address;
	for( i = 0; i < 0x1000; i += 128 ) {
		fread( (char*) p, 128, 1, p_file );
		p += 128;
	}
	fclose( p_file );

	// 初期化ルーチンを呼ぶ
	bgmdrv_setup_htimi();
	return 1;
}

// --------------------------------------------------------------------
void bgmdrv_term( void ) {

	// 後始末ルーチンを呼ぶ
	bgmdrv_restore_htimi();
}

// --------------------------------------------------------------------
int bgmdrv_play( const char *p_music_file_name ) {
	FILE *p_file;
	char *p;
	int i;

	// 演奏停止
	bgmdrv_stop();

	p_file = fopen( p_music_file_name, "rb" );
	if( p_file == NULL ) {
		return 0;
	}
	p = (char*)bgm_data_address;
	for( i = 0; i < 0x2000; i += 128 ) {
		fread( (char*) p, 128, 1, p_file );
		p += 128;
	}
	fclose( p_file );

	// 演奏開始
	bgmdriver_play();
	return 1;
}
