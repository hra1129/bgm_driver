static const char *p_start_address				= 0x0c000;
static const int driver_size					= 0x0705;
void (* bgmdrv_setup_htimi)( void )				= 0x0c6a0;
void (* bgmdrv_restore_htimi)( void )			= 0x0c6c6;
void (* bgmdrv_play)( void *p_data )			= 0x0c6d4;
void (* bgmdrv_stop)( void )					= 0x0c04b;
int  (* bgmdrv_check_play)( void )				= 0x0c6f2;
void (* bgmdrv_play_se)( void *p_data )			= 0x0c6dc;
void (* bgmdrv_fade_out)( unsigned int speed )	= 0x0c6e4;