// --------------------------------------------------------------------
//	bgm_driver ‚ð z88dk ‚©‚ç—˜—p‚·‚é‚½‚ß‚ÌƒTƒ“ƒvƒ‹
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include "bgmdriver.h"

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {

	printf( "BGM Driver Sample Program.\n" );
	if( !bgmdrv_init() ) {
		//	BGMDRV.BIN “Ç‚ß‚È‚©‚Á‚½
		return 1;
	}
	puts( "Success Load BGMDRV.BIN.\n" );

	bgmdrv_play( "SAMPLE.BIN" );

	printf( "Play SAMPLE.BIN.\n" );
	getchar();

	bgmdrv_stop();
	bgmdrv_term();
	return 0;
}
