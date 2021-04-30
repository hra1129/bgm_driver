// --------------------------------------------------------------------
//	bgm_driver を z88dk から利用するためのサンプル
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include "bgmdriver.h"
#include "bgmdriver_param.h"

// --------------------------------------------------------------------
int bgmdrv_load_file( void *p_memory, const char *p_file_name, size_t file_size ) {
	char *p;
	FILE *p_file;
	size_t i, s, total_size;

	p_file = fopen( "BGMDRV.BIN", "rb" );
	if( p_file == NULL ) {
		return 0;	//	ERROR
	}
	if( file_size == 0 ) {
		total_size = 65000;
	}
	else {
		total_size = file_size;
	}
	p = (char*) p_start_address;
	for( i = total_size; i > 0; i -= s ) {
		if( i > 128 ) {
			s = 128;
		}
		else {
			s = i;
		}
		if( fread( p, 1, s, p_file ) < s ) {
			break;
		}
		p += 128;
	}
	fclose( p_file );
	if( i > 0 && file_size != 0 ) {
		return 0;	//	ERROR
	}
	return 1;	//	SUCCESS
}

// --------------------------------------------------------------------
int bgmdrv_init( void ) {

	// BGMDRV.BIN を 0x4000番地～ に読み込む
	if( !bgmdrv_load_file( p_start_address, "BGMDRV.BIN", driver_size ) ) {
		printf( "[ERROR] Cannot read BGMDRV.BIN.\n" );
		return 0;
	}

	// 初期化ルーチンを呼ぶ
	bgmdrv_setup_htimi();
	return 1;
}

// --------------------------------------------------------------------
void bgmdrv_term( void ) {

	// 後始末ルーチンを呼ぶ
	bgmdrv_restore_htimi();
}
