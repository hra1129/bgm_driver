// --------------------------------------------------------------------
//	bgm_driver を z88dk から利用するためのサンプル
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "bgmdriver.h"

static const char se_data[] = {
	128,
	BGM_SE_VOL,         12,
	BGM_SE_FREQ,        0x00, 0x80,
	BGM_SE_NOISE_FREQ,  20 + 0x80,
	BGM_SE_WAIT,        5,
	BGM_SE_NOISE_FREQ,  8 + 0x80,
	BGM_SE_WAIT,        5,
	BGM_SE_NOISE_FREQ,  28 + 0x80,
	BGM_SE_WAIT,        5,
	BGM_SE_NOISE_FREQ,  12 + 0x80,
	BGM_SE_WAIT,        5,
	BGM_SE_NOISE_FREQ,  31 + 0x80,
	BGM_SE_WAIT,        10,
	BGM_SE_NOISE_FREQ,  15 + 0x80,
	BGM_SE_WAIT,        10,
	BGM_SE_END
};

static const char music_data[] = {
	// 曲データ
	0x06, 0x00, 0x18, 0x00, 0x28, 0x00, 0x67, 0x0F, 0x68, 0x29, 0x00, 0x30, 0x07, 0x32, 0x07, 0x34,
	0x08, 0x35, 0x07, 0x37, 0x07, 0x37, 0x08, 0x6A, 0x67, 0x0F, 0x68, 0x29, 0x00, 0x34, 0x07, 0x35,
	0x07, 0x37, 0x08, 0x39, 0x07, 0x3B, 0x07, 0x6A, 0x6A,
	// 音色データ
	0x00, 0x01, 0x02, 0x02, 0x02, 0x00, 0xFE, 0xFD, 0xFD, 0xFD, 0xFE, 0x00, 0x02, 0x02, 0x02, 0x01,
	0x00, 0xFE, 0xFD, 0xFD, 0xFD, 0xFF, 0x01, 0x02, 0x03, 0x02, 0x01, 0xFF, 0xFD, 0xFD, 0xFD, 0xFE,
	0xFF, 0x02, 0xE0, 0x01, 0x1E, 0x14, 0x00, 0x00, 0x00,
};

static char *p_music_data = (char*)0x6000;
static char *p_se_data = (char*)(0x6000 + sizeof(music_data));

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	int i, j;

	printf( "BGM Driver Sample Program.\n" );
	if( !bgmdrv_init() ) {
		//	BGMDRV.BIN 読めなかった
		return 1;
	}
	puts( "Success Load BGMDRV.BIN.\n" );

	// 曲データ・効果音データは 4000h以降になければならないのでコピーする
	memcpy( p_music_data, music_data, sizeof(music_data) );
	memcpy( p_se_data, se_data, sizeof(se_data) );

	// 曲再生
	bgmdrv_play( p_music_data );
	while( bgmdrv_check_play() ) {
		printf( "[playing]" );
	}
	puts( "[stop]" );

	// 効果音再生
	for( j = 0; j < 4; j++ ) {
		bgmdrv_play_se( p_se_data );
		for( i = 0; i < 800; i++ ) {
			printf( "[*]" );
		}
	}

	puts( "Finish.\n" );
	bgmdrv_term();
	return 0;
}
