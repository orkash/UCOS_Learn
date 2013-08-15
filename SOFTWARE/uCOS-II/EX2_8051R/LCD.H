/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*      Interface to the lcd.c module with functions for use with the LCD
|*	on the Phytec KitCON 505L board.
|*
 ****************************************************************************/


#ifndef _LCD_H
#define _LCD_H


#define	LCD_WIDTH	   8

extern _xdat unsigned char DAC0;
extern _xdat unsigned char LCON;
extern _xdat unsigned char LCRL;
extern _xdat unsigned char LCRH;
extern _xdat unsigned int  LCD[LCD_WIDTH];


#define ch_A	0x341E
#define ch_B	0x741E
#define ch_C	0x5402
#define ch_D	0x1421
#define ch_E	0x740A
#define ch_F	0x340A
#define ch_G	0x7412
#define ch_H	0x341C
#define ch_I	0x4182
#define ch_J	0x5014
#define ch_K	0x9C08
#define ch_L	0x5400
#define ch_M	0x1C15
#define ch_N	0x9415
#define ch_O	0x5416
#define ch_P	0x340E
#define ch_Q	0x9654
#define ch_R	0xB40E
#define ch_S	0x641A
#define	ch_T	0x0182
#define ch_U	0x5414
#define ch_V	0x1C20
#define ch_W	0x9434
#define ch_X	0x8821
#define ch_Y	0x0881
#define ch_Z	0x4822

#define ch_a	ch_A
#define ch_b	ch_B
#define ch_c	ch_C
#define ch_d	ch_D
#define ch_e	ch_E
#define ch_f	ch_F
#define ch_g	ch_G
#define ch_h	ch_H
#define ch_i	ch_I
#define ch_j	ch_J
#define ch_k	ch_K
#define ch_l	ch_L
#define ch_m	ch_M
#define ch_n	ch_N
#define ch_o	ch_O
#define ch_p	ch_P
#define ch_q	ch_Q
#define ch_r	ch_R
#define ch_s	ch_S
#define ch_t	ch_T
#define ch_u	ch_U
#define ch_v	ch_V
#define ch_w	ch_W
#define ch_x	ch_X
#define ch_y	ch_Y
#define ch_z	ch_Z

#define ch_1	0x0014
#define	ch_2	0x700E
#define ch_3	0x601E
#define ch_4	0x241C
#define ch_5	0x641A
#define ch_6	0x741A
#define ch_7	0x0016
#define ch_8	0x741E
#define ch_9	0x641E
#define ch_0	0x5C36

#define ch_SP	0x0000			// Space-bar
#define ch_UN	0x4000			// Underscore
#define	ch_DOT	0x0040			// Decimal point (dot)
#define ch_AP	0x0200			// Apostrophe
#define ch_DOL	0x659A			// Dollar
#define ch_AST	0xA9A9			// Asterisk
#define ch_PLUS	0x2188			// Plus
#define ch_MIN	0x2008			// Minus
#define ch_SL	0x0820			// Slash
#define ch_BSL	0x8001			// Back-slash
#define ch_OR	0x0180			// OR-sign       '|'
#define ch_EXCL 0x0180			// Exclamation mark
#define ch_QUES 0x0C82			// Question mark
#define ch_OBRK 0x8800			// Open bracket	 '('
#define ch_CBRK 0x8800			// Close bracket ')'
#define ch_OBRS 0x5402			// Open bracket	 '['
#define ch_CBRS 0x4006			// Close bracket ']'
#define ch_EQU	0x6008			// Equal-sign

#define ch_xx	0x0000			// Undefined character

extern _rom unsigned int CHR[128];

extern void InitLcd( void );
extern void ClearLcd( void );
extern void WriteLcd( unsigned char position, unsigned char input );
extern void WriteLcdStr( char * str );
extern void SetContrast( unsigned char Contrast );


#endif /* _LCD_H */
