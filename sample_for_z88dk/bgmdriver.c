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
	bgmdrv_setup_htimi();
	return 1;
}

// --------------------------------------------------------------------
void bgmdrv_term( void ) {

	// ��n�����[�`�����Ă�
	bgmdrv_restore_htimi();
}

// --------------------------------------------------------------------
int bgmdrv_play( const char *p_music_file_name ) {
	FILE *p_file;
	char *p;
	int i;

	// ���t��~
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

	// ���t�J�n
	bgmdriver_play();
	return 1;
}
