// --------------------------------------------------------------------
//	bgm_driver ‚ð z88dk ‚©‚ç—˜—p‚·‚é‚½‚ß‚ÌƒTƒ“ƒvƒ‹
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "bgmdriver.h"

static char *p_music_data = (char*)0x4000;

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	int i, j;
	char c;

	printf( "Simple Player.\n" );
	printf( "==================\n" );
	printf( "2023 HRA!\n" );

	if( argc < 2 ) {
		printf( "Usage> splay <bgm.bin>\n" );
		return 1;
	}

	if( !bgmdrv_init() ) {
		printf( "ERROR: Cannot read BGMDRV.BIN\n" );
		return 1;
	}

	if( !bgmdrv_load_file( p_music_data, argv[1], 0 ) ) {
		printf( "ERROR: Cannot read %s.\n", argv[1] );
		return 1;
	}
	printf( "%s\n", argv[1] );
	// ‹ÈÄ¶
	bgmdrv_play( p_music_data );
	for( c = 0; c != 'x' && c != 'X'; ) {
		puts( "[P] play" );
		puts( "[L] reload" );
		puts( "[S] stop" );
		puts( "[X] exit" );
		c = getchar();
		switch( c ) {
		case 'p':
		case 'P':
			bgmdrv_play( p_music_data );
			puts( "--> PLAY" );
			break;
		case 'l':
		case 'L':
			bgmdrv_stop();
			if( !bgmdrv_load_file( p_music_data, argv[1], 0 ) ) {
				printf( "ERROR: Cannot read %s.\n", argv[1] );
				return 1;
			}
			puts( "--> STOP & LOAD" );
			break;
		case 's':
		case 'S':
			bgmdrv_stop();
			puts( "--> STOP" );
			break;
		}
	}
	puts( "--> Exit." );
	bgmdrv_stop();
	bgmdrv_term();
	return 0;
}
