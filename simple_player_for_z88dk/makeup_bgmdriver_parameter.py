#!/usr/bin/env python3
# -*- coding=utf-8 -*-

import re

# =============================================================================
def makeup():
	with open( 'zma.sym', 'r' ) as f:
		sym = f.readlines()

	symbol_list = {}
	for line in sym:
		match = re.match( '([A-Za-z_0-9]+) equ ([0-9a-f]+)h', line )
		if not match:
			continue
		item = match.groups(0)
		symbol_list[ item[0] ] = item[1]

	with open( 'bgmdriver_param.h', 'w' ) as f:
		f.write(   'static const char *p_start_address				= 0x%s;\n' % symbol_list[ 'START_ADDRESS' ] )
		f.write(   'static const int driver_size					= 0x%s;\n' % symbol_list[ 'DRIVER_SIZE' ] )
		f.write(   'void (* bgmdrv_setup_htimi)( void )				= 0x%s;\n' % symbol_list[ 'BGMDRV_SETUP_HTIMI' ] )
		f.write(   'void (* bgmdrv_restore_htimi)( void )			= 0x%s;\n' % symbol_list[ 'BGMDRV_RESTORE_HTIMI' ] )
		f.write(   'void (* bgmdrv_play)( void *p_data )			= 0x%s;\n' % symbol_list[ 'BGMDRV_PLAY' ] )
		f.write(   'void (* bgmdrv_stop)( void )					= 0x%s;\n' % symbol_list[ 'BGMDRIVER_STOP' ] )
		f.write(   'int  (* bgmdrv_check_play)( void )				= 0x%s;\n' % symbol_list[ 'BGMDRV_CHECK_PLAY' ] )
		f.write(   'void (* bgmdrv_play_se)( void *p_data )			= 0x%s;\n' % symbol_list[ 'BGMDRV_PLAY_SE' ] )
		f.write(   'void (* bgmdrv_fade_out)( unsigned int speed )	= 0x%s;\n' % symbol_list[ 'BGMDRV_FADE_OUT' ] )

# =============================================================================
if __name__ == '__main__':
	print( "Makeup bgmdriver_param.c from zma.sym\n" )
	makeup()
