// --------------------------------------------------------------------
//	bgm_driver �� z88dk ���痘�p���邽�߂̃T���v��
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include "bgmdriver.h"

static const unsigned int bgm_driver_base_address = 0x4000;
static const unsigned int bgm_data_address = 0x6000;

// zma.sym ����l���R�s�y
#define BGMDRV_SETUP_HTIMI			0x4693
#define BGMDRV_RESTORE_HTIMI		0x46af
#define BGMDRIVER_PLAY				0x46bd
#define BGMDRIVER_STOP				0x404b

// --------------------------------------------------------------------
int bgmdrv_init( void ) {
	FILE *p_file;
	char *p;
	int i;

	// BGMDRV.BIN �� 0x4000�Ԓn�` �ɓǂݍ���
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

	// ���������[�`�����Ă�
	#asm
		call BGMDRV_SETUP_HTIMI;
	#endasm
	return 1;
}

// --------------------------------------------------------------------
void bgmdrv_term( void ) {

	// ��n�����[�`�����Ă�
	#asm
		call BGMDRV_RESTORE_HTIMI;
	#endasm
}

// --------------------------------------------------------------------
int bgmdrv_play( const char *p_music_file_name ) {
	FILE *p_file;
	char *p;
	int i;

	// ���t��~
	#asm
		call BGMDRIVER_STOP;
	#endasm

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

	// ���t�J�n
	#asm
		call BGMDRIVER_PLAY;
	#endasm
	return 1;
}

// --------------------------------------------------------------------
void bgmdrv_stop( void ) {

	// ���t��~
	#asm
		call BGMDRIVER_STOP;
	#endasm
}
