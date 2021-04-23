/* ----------------------------------------------------- */
/*	MML Compiler										 */
/* ===================================================== */
/*	2009/10/03	t.hara									 */
/* ----------------------------------------------------- */

#include <stdio.h>
#include <ctype.h>
#include <malloc.h>

#define MEM_SIZE			32768
#define SOUND_FONT_SIZE		41

typedef struct {
	const char	*p_text;
	int			index;
	int			line_no;
} TEXT_T;

typedef struct {
	int			tempo;
	double		default_length;
	double		length_error;		/* 累積誤差 */
	double		count;
	char		*p_music_data;
	int			index;
	int			sound_font_count;
	int			octave;
	int			ratio;
	int			size;
	int			loop_index;
} MUSIC_T;

enum {
	BGM_HEADER_SIZE		= 6,

	BGM_DRUM_1			= 96,
	BGM_DRUM_2			= 97,
	BGM_DRUM_3			= 98,
	BGM_DRUM_4			= 99,
	BGM_DRUM_5			= 100,

	BGM_KEYOFF			= 101,
	BGM_REST			= 102,
	BGM_VOL				= 103,
	BGM_SOUND_FONT		= 104,
	BGM_JUMP			= 105,
	BGM_PLAY_END		= 106,
	BGM_DRUM1_FONT		= 107,
	BGM_DRUM2_FONT		= 108,
	BGM_DRUM3_FONT		= 109,
	BGM_DRUM4_FONT		= 110,
	BGM_DRUM5_FONT		= 111
};

/* ----------------------------------------------------- */
static char *load_file_image( const char *p_in_name ) {
	FILE *p_in;
	long in_size;
	char *p_image;

	p_in = fopen( p_in_name, "r" );
	if( p_in == NULL ) {
		return NULL;
	}
	fseek( p_in, 0, SEEK_END );
	in_size = ftell( p_in );
	fseek( p_in, 0, SEEK_SET );

	p_image = (char*) calloc( in_size + 1, 1 );		/* +1 は、端末文字 '\0' の分 */
	if( p_image == NULL ) {
		fclose( p_in );
		return NULL;
	}
	fread( p_image, 1, in_size, p_in );
	fclose( p_in );
	return p_image;
}

/* ----------------------------------------------------- */
static void skip_white_space( TEXT_T *p_text_info ) {
	char c;
	int is_comment = 0;

	while( (c = p_text_info->p_text[ p_text_info->index ]) != '\0' ) {
		if( is_comment ) {
			if( c == '\n' ) {
				is_comment = 0;
				p_text_info->line_no++;
			}
		}
		else if( c == ';' ) {
			is_comment = 1;
		}
		else if( c == '\n' ) {
			p_text_info->line_no++;
		}
		else if( !isspace( c & 255 ) ) {
			break;
		}
		p_text_info->index++;
	}
}

/* ----------------------------------------------------- */
static int get_number( TEXT_T *p_text_info ) {
	char c;
	int number, sign = 1;

	c = p_text_info->p_text[ p_text_info->index ];
	if( c == '-' ) {
		sign = -1;
		p_text_info->index++;
	}
	number = 0;
	for(;;) {
		c = p_text_info->p_text[ p_text_info->index ];
		if( !isdigit( c & 255 ) ) break;
		number = number * 10 + (c - '0');
		p_text_info->index++;
	}
	return number * sign;
}

/* ----------------------------------------------------- */
static int compile_sound_font( char *p_sound_font, TEXT_T *p_text_info ) {
	char c;
	int i, d, sound_font_count;

	sound_font_count = 0;
	skip_white_space( p_text_info );
	c = p_text_info->p_text[ p_text_info->index ];
	if( c != '{' ) {
		fprintf( stderr, "ERROR: 音色設定が見つかりません(%d)\n", p_text_info->line_no );
		return 0;
	}
	p_text_info->index++;

	do {
		for( i = 0; i < SOUND_FONT_SIZE; i++ ) {
			skip_white_space( p_text_info );
			d = get_number( p_text_info );
			if( i == 39 ) {
				p_sound_font[ sound_font_count * SOUND_FONT_SIZE + i ] = (char)(d & 255);
				i++;
				p_sound_font[ sound_font_count * SOUND_FONT_SIZE + i ] = (char)(d >> 8);
			}
			else {
				p_sound_font[ sound_font_count * SOUND_FONT_SIZE + i ] = (char)d;
			}
		}
		sound_font_count++;
		skip_white_space( p_text_info );
		c = p_text_info->p_text[ p_text_info->index ];
	} while( c != '}' && c != '\0' );
	if( c == '}' ) {
		p_text_info->index++;
	}
	return sound_font_count;
}

/* ----------------------------------------------------- */
static double get_length( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {
	char c;
	double length = 0;

	c = p_text_info->p_text[ p_text_info->index ];
	if( isdigit( c & 255 ) ) {
		/* 数値指定があれば読み取る */
		for(;;) {
			c = p_text_info->p_text[ p_text_info->index ];
			if( !isdigit( c & 255 ) ) break;
			length = length * 10. + (c - '0');
			p_text_info->index++;
		}
	}
	else {
		/* 数値指定がなければデフォルト値 */
		length = p_music_info->default_length;
	}
	/* 付点の確認 */
	for(;;) {
		c = p_text_info->p_text[ p_text_info->index ];
		if( c != '.' ) {
			break;
		}
		length = length / 1.5;
		p_text_info->index++;
	}
	return length;
}

/* ----------------------------------------------------- */
static void put_length( MUSIC_T *p_music_info, double length, int ratio ) {
	double length_time;
	int wait_count;

	/* tempo											 */
	/*		一分間の四分音符の数						 */
	/* tempo/4											 */
	/*		一分間の全音符の数							 */
	/* 60*60											 */
	/*		一分間のカウント数							 */
	/* 60*60/(tempo/4)									 */
	/*		全音符のカウント数							 */
	/* length											 */
	/*		希望の音符の長さ length分音符				 */
	/* (60*60/(tempo/4))/length							 */
	/*		希望の音符のカウント数						 */
	/* 14400/(tempo*length)								 */
	/*		上の式を整理								 */

	length_time = (14400. * ratio / (p_music_info->tempo* length * 8)) + p_music_info->length_error;
	wait_count = (int)length_time;
	p_music_info->length_error = length_time - (double)wait_count;	/* 誤差を覚えておく */
	/* 音長を出力 */
	wait_count--;
	while( wait_count > 255 ) {
		p_music_info->p_music_data[ p_music_info->index++ ] = (char)255;
		p_music_info->size++;
		wait_count -= 255;
	}
	p_music_info->p_music_data[ p_music_info->index++ ] = (char)wait_count;
	p_music_info->size++;
}

/* ----------------------------------------------------- */
static void process_drum_data( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {
	char c;
	int tone_id;
	double length;

	c = toupper(p_text_info->p_text[ p_text_info->index ]);
	tone_id = (c - 'H') + BGM_DRUM_1;
	if( c == 'M' ) {
		tone_id--;
	}
	p_text_info->index++;
	length = get_length( p_music_info, p_text_info );

	/* 集めた情報に基づいてデータを書き出す */
	p_music_info->p_music_data[ p_music_info->index++ ] = tone_id;
	p_music_info->size++;
	if( p_music_info->ratio != 8 ) {
		put_length( p_music_info, length, p_music_info->ratio );
		p_music_info->p_music_data[ p_music_info->index++ ] = BGM_KEYOFF;
		p_music_info->size++;
		put_length( p_music_info, length, 8 - p_music_info->ratio );
	}
	else {
		put_length( p_music_info, length, p_music_info->ratio );
	}
}

/* ----------------------------------------------------- */
static void process_tone_data( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {
	char c;
	int tone_id;
	double length;
	int with_tai;

	c = toupper(p_text_info->p_text[ p_text_info->index ]);
	if( c >= 'C' ) {
		tone_id = (c - 'C') * 2;
	}
	else {
		tone_id = (c - 'A' + 5) * 2;
	}
	if( tone_id > 4 ) {
		tone_id--;
	}
	p_text_info->index++;
	c = toupper(p_text_info->p_text[ p_text_info->index ]);
	if( c == '+' || c == '#' ) {
		tone_id++;
		p_text_info->index++;
	}
	else if( c == '-' ) {
		tone_id--;
		p_text_info->index++;
	}
	tone_id += p_music_info->octave * 12;
	if( tone_id < 0 || tone_id >= 96 ) {
		fprintf( stderr, "ERROR: 音程指定が範囲外です(%d)\n", p_text_info->line_no );
	}
	length = get_length( p_music_info, p_text_info );

	c = toupper( p_text_info->p_text[ p_text_info->index ] );
	if( c == '&' ){
		with_tai = 1;
		p_text_info->index++;
	}
	else{
		with_tai = 0;
	}

	/* 集めた情報に基づいてデータを書き出す */
	p_music_info->p_music_data[ p_music_info->index++ ] = tone_id;
	p_music_info->size++;
	if( with_tai == 0 && p_music_info->ratio != 8 ) {
		put_length( p_music_info, length, p_music_info->ratio );
		p_music_info->p_music_data[ p_music_info->index++ ] = BGM_KEYOFF;
		p_music_info->size++;
		put_length( p_music_info, length, 8 - p_music_info->ratio );
	}
	else {
		put_length( p_music_info, length, p_music_info->ratio );
	}
}

/* ----------------------------------------------------- */
static void process_length( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	p_music_info->default_length = 4.;
	p_music_info->default_length = get_length( p_music_info, p_text_info );
}

/* ----------------------------------------------------- */
static void process_tempo( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	p_music_info->tempo = get_number( p_text_info );
	if( p_music_info->tempo < 20 || p_music_info->tempo > 240 ) {
		fprintf( stderr, "ERROR: T の指定が範囲外です(%d)\n", p_text_info->line_no );
		p_music_info->tempo = 120;
	}
}

/* ----------------------------------------------------- */
static void process_ratio( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	p_music_info->ratio = get_number( p_text_info );
	if( p_music_info->ratio < 1 || p_music_info->ratio > 8 ) {
		fprintf( stderr, "ERROR: Q の指定が範囲外です(%d)\n", p_text_info->line_no );
		p_music_info->tempo = 120;
	}
}

/* ----------------------------------------------------- */
static void process_sound_font( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {
	char c, id;
	int sound_font;

	p_text_info->index++;
	c = toupper(p_text_info->p_text[ p_text_info->index ]);
	if( (c >= 'H' && c < 'L') || c == 'M' ) {
		id = BGM_DRUM1_FONT + (c - 'H');
		if( c == 'M' ) {
			id--;
		}
		p_text_info->index++;
	}
	else {
		id = BGM_SOUND_FONT;
	}
	sound_font = get_number( p_text_info );
	if( sound_font >= p_music_info->sound_font_count ) {
		fprintf( stderr, "ERROR: @ の指定が範囲外です(%d)\n", p_text_info->line_no );
		sound_font = 0;
	}
	p_music_info->p_music_data[ p_music_info->index++ ] = id;
	p_music_info->size++;
	/* 現段階では音色データのアドレスが不確定なので、代わりに音色番号を記録しておく */
	p_music_info->p_music_data[ p_music_info->index++ ] = (char)sound_font;
	p_music_info->size++;
	p_music_info->p_music_data[ p_music_info->index++ ] = 0;
	p_music_info->size++;
}

/* ----------------------------------------------------- */
static void process_volume( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {
	int volume;

	p_text_info->index++;
	volume = get_number( p_text_info );
	if( volume > 15 ) {
		fprintf( stderr, "ERROR: V の指定が範囲外です(%d)\n", p_text_info->line_no );
		volume = 15;
	}
	p_music_info->p_music_data[ p_music_info->index++ ] = BGM_VOL;
	p_music_info->size++;
	p_music_info->p_music_data[ p_music_info->index++ ] = (char)volume;
	p_music_info->size++;
}

/* ----------------------------------------------------- */
static void process_octave( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	p_music_info->octave = get_number( p_text_info ) - 1;
	if( p_music_info->octave < 0 || p_music_info->octave > 7 ) {
		fprintf( stderr, "ERROR: O の指定が範囲外です(%d)\n", p_text_info->line_no );
		p_music_info->octave = 4;
	}
}

/* ----------------------------------------------------- */
static void process_octave_inc( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	p_music_info->octave++;
	if( p_music_info->octave < 0 || p_music_info->octave > 7 ) {
		fprintf( stderr, "ERROR: > 指定により範囲外オクターブになりました(%d)\n", p_text_info->line_no );
		p_music_info->octave = 7;
	}
}

/* ----------------------------------------------------- */
static void process_octave_dec( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	p_music_info->octave--;
	if( p_music_info->octave < 0 || p_music_info->octave > 7 ) {
		fprintf( stderr, "ERROR: < 指定により範囲外オクターブになりました(%d)\n", p_text_info->line_no );
		p_music_info->octave = 0;
	}
}

/* ----------------------------------------------------- */
static void process_rest( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {
	double length;

	p_text_info->index++;
	length = get_length( p_music_info, p_text_info );
	p_music_info->p_music_data[ p_music_info->index++ ] = BGM_REST;
	p_music_info->size++;
	put_length( p_music_info, length, 8 );
}

/* ----------------------------------------------------- */
static void process_loop_start( MUSIC_T *p_music_info, TEXT_T *p_text_info ) {

	p_text_info->index++;
	if( p_music_info->loop_index != -1 ) {
		fprintf( stderr, "WARNNING: 同一チャンネル内に $指定が複数存在します(%d)\n", p_text_info->line_no );
	}
	p_music_info->loop_index = p_music_info->index;
}

/* ----------------------------------------------------- */
static int compile_music_data( char *p_music_data, TEXT_T *p_text_info, int sound_font_count, int offset ) {
	char c;
	MUSIC_T music_info;

	music_info.tempo = 120;
	music_info.count = 0.;
	music_info.default_length = 4;
	music_info.p_music_data = p_music_data;
	music_info.index = offset;
	music_info.sound_font_count = sound_font_count;
	music_info.ratio = 8;
	music_info.octave = 4;
	music_info.length_error = 0.;
	music_info.size = 0;
	music_info.loop_index = -1;

	skip_white_space( p_text_info );
	c = p_text_info->p_text[ p_text_info->index ];
	if( c != '{' ) {
		fprintf( stderr, "ERROR: 曲データが見つかりません(%d)\n", p_text_info->line_no );
		return 0;
	}
	p_text_info->index++;

	for(;;) {
		skip_white_space( p_text_info );
		c = p_text_info->p_text[ p_text_info->index ];
		if( c == '}' ) {
			p_text_info->index++;
			break;
		}
		switch( toupper(c) ) {
		case '\0':	
			break;
		case 'C':	case 'D':	case 'E':	case 'F':	case 'G':	case 'A':	case 'B':
			process_tone_data( &music_info, p_text_info );
			break;
		case 'T':
			process_tempo( &music_info, p_text_info );
			break;
		case 'L':
			process_length( &music_info, p_text_info );
			break;
		case '@':
			process_sound_font( &music_info, p_text_info );
			break;
		case 'V':
			process_volume( &music_info, p_text_info );
			break;
		case 'O':
			process_octave( &music_info, p_text_info );
			break;
		case '>':
			process_octave_inc( &music_info, p_text_info );
			break;
		case '<':
			process_octave_dec( &music_info, p_text_info );
			break;
		case 'R':
			process_rest( &music_info, p_text_info );
			break;
		case 'Q':
			process_ratio( &music_info, p_text_info );
			break;
		case '$':
			process_loop_start( &music_info, p_text_info );
			break;
		case 'H':	case 'I':	case 'J':	case 'K':	case 'M':
			process_drum_data( &music_info, p_text_info );
			break;
		default:
			fprintf( stderr, "ERROR: 解釈できない記号があります(%d)\n", p_text_info->line_no );
			p_text_info->index++;
			break;
		}
	}
	if( music_info.loop_index == -1 ) {
		music_info.p_music_data[ music_info.index++ ] = BGM_PLAY_END;
		music_info.size++;
	}
	else {
		music_info.p_music_data[ music_info.index++ ] = BGM_JUMP;
		music_info.size++;
		music_info.p_music_data[ music_info.index++ ] = (char)(music_info.loop_index & 255);
		music_info.size++;
		music_info.p_music_data[ music_info.index++ ] = (char)(music_info.loop_index >> 8);
		music_info.size++;
	}
	return music_info.size;
}

/* ----------------------------------------------------- */
static void fwrite_text( const char *p_buffer, int size, FILE *p_out ) {
	int i, mod16;

	for( i = 0; i < size; i++ ) {
		mod16 = i & 15;
		if( mod16 == 0 ) {
			fprintf( p_out, "\t\tdb\t\t" );
		}
		if( mod16 == 15 || i == (size - 1) ) {
			fprintf( p_out, "0x%02X\n", (int)p_buffer[i] & 255 );
		}
		else {
			fprintf( p_out, "0x%02X, ", (int)p_buffer[i] & 255 );
		}
	}
	fprintf( p_out, "\n" );
}

/* ----------------------------------------------------- */
static void link_music_data( const char *p_out_name, char *p_sound_font_memory, int sound_font_count, char *p_music_data_memory, int *p_music_data_size ) {
	int offset1, offset2, offset3, i, sound_font;
	FILE *p_out;

	offset1 = p_music_data_size[0] + BGM_HEADER_SIZE;
	offset2 = p_music_data_size[1] + offset1;
	offset3 = p_music_data_size[2] + offset2;

	/* ヘッダを更新する */
	p_music_data_memory[0] = BGM_HEADER_SIZE;
	p_music_data_memory[1] = 0;
	p_music_data_memory[2] = (char)(offset1 & 255);
	p_music_data_memory[3] = (char)(offset1 >> 8);
	p_music_data_memory[4] = (char)(offset2 & 255);
	p_music_data_memory[5] = (char)(offset2 >> 8);

	/* 音色データ番号を音色データアドレスへ変換 */
	for( i = 6; i < offset3; i++ ) {
		if( p_music_data_memory[i] <= BGM_REST ) {
			do {
				i++;
			} while( p_music_data_memory[i] == 255 );
		}
		else if( p_music_data_memory[i] == BGM_VOL ) {
			i++;
		}
		else if( p_music_data_memory[i] == BGM_JUMP ) {
			i+=2;
		}
		else if( p_music_data_memory[i] == BGM_PLAY_END ) {
		}
		else {
			i++;
			sound_font = p_music_data_memory[i] * SOUND_FONT_SIZE + offset3;
			p_music_data_memory[i] = (char)(sound_font & 255);
			i++;
			p_music_data_memory[i] = (char)(sound_font >> 8);
		}
	}

	/* ファイルへ書き出す */
	p_out = fopen( p_out_name, "w" );
	if( p_out == NULL ) {
		fprintf( stderr, "ERROR: %s を書き出せません\n", p_out_name );
		return;
	}
	fprintf( p_out, "\t\t; 曲データ\n" );
	fwrite_text( p_music_data_memory, offset3, p_out );
	fprintf( p_out, "\t\t; 音色データ\n" );
	fwrite_text( p_sound_font_memory, SOUND_FONT_SIZE * sound_font_count, p_out );
	fclose( p_out );
}

/* ----------------------------------------------------- */
static void mml_compile( const char *p_out_name, const char *p_text ) {
	static char sound_font_memory[ MEM_SIZE ];
	static char music_data_memory[ MEM_SIZE ];
	int sound_font_count, music_data_size[3];
	TEXT_T text_info;

	text_info.p_text = p_text;
	text_info.index = 0;
	text_info.line_no = 1;
	sound_font_count = compile_sound_font( sound_font_memory, &text_info );
	if( sound_font_count == 0 ) {
		fprintf( stderr, "ERROR: 音色データが１つもありません(%d)\n", text_info.line_no );
	}
	else {
		printf( "音色データ %d 個\n", sound_font_count );
	}
	music_data_size[0] = compile_music_data( music_data_memory, &text_info, sound_font_count, BGM_HEADER_SIZE );
	printf( "ch.0 データサイズ %d[byte]\n", music_data_size[0] );
	music_data_size[1] = compile_music_data( music_data_memory, &text_info, sound_font_count, BGM_HEADER_SIZE + music_data_size[0] );
	printf( "ch.1 データサイズ %d[byte]\n", music_data_size[1] );
	music_data_size[2] = compile_music_data( music_data_memory, &text_info, sound_font_count, BGM_HEADER_SIZE + music_data_size[0] + music_data_size[1] );
	printf( "ch.2 データサイズ %d[byte]\n", music_data_size[2] );
	link_music_data( p_out_name, sound_font_memory, sound_font_count, music_data_memory, music_data_size );
}

/* ----------------------------------------------------- */
static void usage( const char *p_name ) {

	fprintf( stderr, "Usage> %s <in.mml> <out.asm>\n", p_name );
}

/* ----------------------------------------------------- */
int main( int argc, char *argv[] ) {
	char *p_text;

	printf( "MML Compiler\n" );
	printf( "===========================================\n" );
	printf( "2009/10/03 t.hara\n" );

	if( argc < 3 ) {
		usage( argv[0] );
		return 1;
	}

	p_text = load_file_image( argv[1] );
	if( p_text == NULL ) {
		fprintf( stderr, "ERROR: \"%s\" を読み込めません\n", argv[1] );
		return 2;
	}
	mml_compile( argv[2], p_text );
	free( p_text );
	printf( "Completed.\n" );
	return 0;
}
