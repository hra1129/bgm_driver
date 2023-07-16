; -----------------------------------------------------------------------------
;	PSG BGM DRIVER DEFINE HEADER
; -----------------------------------------------------------------------------
;	Copyright (c) 2020 Takayuki Hara
;	http://hraroom.s602.xrea.com/msx/software/index.html
;	
;	Permission is hereby granted, free of charge, to any person obtaining a 
;	copy of this software and associated documentation files (the 
;	"Software"), to deal in the Software without restriction, including 
;	without limitation the rights to use, copy, modify, merge, publish, 
;	distribute, sublicense, and/or sell copies of the Software, and to 
;	permit persons to whom the Software is furnished to do so, subject to 
;	the following conditions:
;	
;	The above copyright notice and this permission notice shall be 
;	included in all copies or substantial portions of the Software.
;	
;	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
;	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
;	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
;	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
;	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
;	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
;	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
; =============================================================================
;	2009/09/30	t.hara
; -----------------------------------------------------------------------------

BGM_TONE_C1			= 0
BGM_TONE_CS1		= 1
BGM_TONE_D1			= 2
BGM_TONE_DS1		= 3
BGM_TONE_E1			= 4
BGM_TONE_F1			= 5
BGM_TONE_FS1		= 6
BGM_TONE_G1			= 7
BGM_TONE_GS1		= 8
BGM_TONE_A1			= 9
BGM_TONE_AS1		= 10
BGM_TONE_B1			= 11

BGM_TONE_C2			= 0  + 12
BGM_TONE_CS2		= 1  + 12
BGM_TONE_D2			= 2  + 12
BGM_TONE_DS2		= 3  + 12
BGM_TONE_E2			= 4  + 12
BGM_TONE_F2			= 5  + 12
BGM_TONE_FS2		= 6  + 12
BGM_TONE_G2			= 7  + 12
BGM_TONE_GS2		= 8  + 12
BGM_TONE_A2			= 9  + 12
BGM_TONE_AS2		= 10 + 12
BGM_TONE_B2			= 11 + 12

BGM_TONE_C3			= 0  + 24
BGM_TONE_CS3		= 1  + 24
BGM_TONE_D3			= 2  + 24
BGM_TONE_DS3		= 3  + 24
BGM_TONE_E3			= 4  + 24
BGM_TONE_F3			= 5  + 24
BGM_TONE_FS3		= 6  + 24
BGM_TONE_G3			= 7  + 24
BGM_TONE_GS3		= 8  + 24
BGM_TONE_A3			= 9  + 24
BGM_TONE_AS3		= 10 + 24
BGM_TONE_B3			= 11 + 24

BGM_TONE_C4			= 0  + 36
BGM_TONE_CS4		= 1  + 36
BGM_TONE_D4			= 2  + 36
BGM_TONE_DS4		= 3  + 36
BGM_TONE_E4			= 4  + 36
BGM_TONE_F4			= 5  + 36
BGM_TONE_FS4		= 6  + 36
BGM_TONE_G4			= 7  + 36
BGM_TONE_GS4		= 8  + 36
BGM_TONE_A4			= 9  + 36
BGM_TONE_AS4		= 10 + 36
BGM_TONE_B4			= 11 + 36

BGM_TONE_C5			= 0  + 48
BGM_TONE_CS5		= 1  + 48
BGM_TONE_D5			= 2  + 48
BGM_TONE_DS5		= 3  + 48
BGM_TONE_E5			= 4  + 48
BGM_TONE_F5			= 5  + 48
BGM_TONE_FS5		= 6  + 48
BGM_TONE_G5			= 7  + 48
BGM_TONE_GS5		= 8  + 48
BGM_TONE_A5			= 9  + 48
BGM_TONE_AS5		= 10 + 48
BGM_TONE_B5			= 11 + 48

BGM_TONE_C6			= 0  + 60
BGM_TONE_CS6		= 1  + 60
BGM_TONE_D6			= 2  + 60
BGM_TONE_DS6		= 3  + 60
BGM_TONE_E6			= 4  + 60
BGM_TONE_F6			= 5  + 60
BGM_TONE_FS6		= 6  + 60
BGM_TONE_G6			= 7  + 60
BGM_TONE_GS6		= 8  + 60
BGM_TONE_A6			= 9  + 60
BGM_TONE_AS6		= 10 + 60
BGM_TONE_B6			= 11 + 60

BGM_TONE_C7			= 0  + 72
BGM_TONE_CS7		= 1  + 72
BGM_TONE_D7			= 2  + 72
BGM_TONE_DS7		= 3  + 72
BGM_TONE_E7			= 4  + 72
BGM_TONE_F7			= 5  + 72
BGM_TONE_FS7		= 6  + 72
BGM_TONE_G7			= 7  + 72
BGM_TONE_GS7		= 8  + 72
BGM_TONE_A7			= 9  + 72
BGM_TONE_AS7		= 10 + 72
BGM_TONE_B7			= 11 + 72

BGM_TONE_C8			= 0  + 84
BGM_TONE_CS8		= 1  + 84
BGM_TONE_D8			= 2  + 84
BGM_TONE_DS8		= 3  + 84
BGM_TONE_E8			= 4  + 84
BGM_TONE_F8			= 5  + 84
BGM_TONE_FS8		= 6  + 84
BGM_TONE_G8			= 7  + 84
BGM_TONE_GS8		= 8  + 84
BGM_TONE_A8			= 9  + 84
BGM_TONE_AS8		= 10 + 84
BGM_TONE_B8			= 11 + 84

BGM_DRUM_1			= 96
BGM_DRUM_2			= 97
BGM_DRUM_3			= 98
BGM_DRUM_4			= 99
BGM_DRUM_5			= 100

BGM_KEYOFF			= 101
BGM_REST			= 102
BGM_VOL				= 103
BGM_SOUND_FONT		= 104
BGM_JUMP			= 105
BGM_PLAY_END		= 106
BGM_DRUM1_FONT		= 107
BGM_DRUM2_FONT		= 108
BGM_DRUM3_FONT		= 109
BGM_DRUM4_FONT		= 110
BGM_DRUM5_FONT		= 111
; -----------------------------------------------------------------------------
BGM_SE_FREQ			= 0
BGM_SE_VOL			= 1
BGM_SE_NOISE_FREQ	= 2
BGM_SE_WAIT			= 3
BGM_SE_END			= 4
