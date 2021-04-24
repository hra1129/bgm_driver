// --------------------------------------------------------------------
//	bgm_driver �� z88dk ���痘�p���邽�߂̃T���v��
// ====================================================================
//	2021/04/23	HRA!
// --------------------------------------------------------------------

#include <stdio.h>
#include "bgmdriver.h"
#include "bgmdriver_param.h"

// --------------------------------------------------------------------
int bgmdrv_init( void ) {
	FILE *p_file;
	char *p;
	int i, s;

	// BGMDRV.BIN �� 0x4000�Ԓn�` �ɓǂݍ���
	p_file = fopen( "BGMDRV.BIN", "rb" );
	if( p_file == NULL ) {
		printf( "[ERROR] Cannot read BGMDRV.BIN.\n" );
		return 0;
	}
	p = (char*) p_start_address;
	for( i = driver_size; i > 0; i -= 128 ) {
		if( i > 128 ) {
			s = 128;
		}
		else {
			s = i;
		}
		fread( p, s, 1, p_file );
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
