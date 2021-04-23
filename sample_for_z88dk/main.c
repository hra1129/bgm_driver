// --------------------------------------------------------------------
//	bgm_driver を z88dk から利用するためのサンプル
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include "bgmdriver.h"

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {

	printf( "BGM Driver Sample Program.\n" );
	if( !bgmdrv_init() ) {
		//	BGMDRV.BIN 読めなかった
		return 1;
	}
	puts( "Success Load BGMDRV.BIN.\n" );

	bgmdrv_play( "SAMPLE.BIN" );

	printf( "Play SAMPLE.BIN.\n" );

	while( bgmdrv_check_play() );

	puts( "Finish.\n" );
	bgmdrv_stop();
	bgmdrv_term();
	return 0;
}
