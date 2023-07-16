// --------------------------------------------------------------------
//	bgm_driver �� z88dk ���痘�p���邽�߂̃T���v��
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

	p_file = fopen( p_file_name, "rb" );
	if( p_file == NULL ) {
		return 0;	//	ERROR
	}
	if( file_size == 0 ) {
		fseek( p_file, 0, SEEK_END );
		total_size = ftell( p_file );
		fseek( p_file, 0, SEEK_SET );
	}
	else {
		total_size = file_size;
	}
	p = (char*) p_memory;
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

	// BGMDRV.BIN �� 0x4000�Ԓn�` �ɓǂݍ���
	if( !bgmdrv_load_file( p_start_address, "BGMDRV.BIN", 0 ) ) {
		printf( "[ERROR] Cannot read BGMDRV.BIN.\n" );
		return 0;
	}

	// ���������[�`�����Ă�
	bgmdrv_setup_htimi();
	return 1;
}

// --------------------------------------------------------------------
void bgmdrv_term( void ) {

	// ��n�����[�`�����Ă�
	bgmdrv_restore_htimi();
}
