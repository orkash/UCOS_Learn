/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*      This module contains several functions for use with the LCD
|*	on the Phytec KitCON 505L board.
|*
 ****************************************************************************/

#include "lcd.h"

_xdat unsigned char DAC0 _at (0x0F3DC);
_xdat unsigned char LCON _at (0x0F3DD);
_xdat unsigned char LCRL _at (0x0F3DE);
_xdat unsigned char LCRH _at (0x0F3DF);

_xdat unsigned int  LCD[LCD_WIDTH] _at (0x0F3E0);

_rom unsigned int CHR[128]  = {
    ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,
    ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,
    ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,
    ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,   ch_xx,
    ch_SP,   ch_EXCL, ch_xx,   ch_xx,   ch_DOL,  ch_xx,   ch_xx,   ch_AP,
    ch_OBRK, ch_CBRK, ch_AST,  ch_PLUS, ch_xx,   ch_MIN,  ch_DOT,  ch_SL,
    ch_0,    ch_1,    ch_2,    ch_3,    ch_4,    ch_5,    ch_6,    ch_7,
    ch_8,    ch_9,    ch_xx,   ch_xx,   ch_xx,   ch_EQU,  ch_xx,   ch_QUES,
    ch_xx,   ch_A,    ch_B,    ch_C,    ch_D,    ch_E,    ch_F,    ch_G,
    ch_H,    ch_I,    ch_J,    ch_K,    ch_L,    ch_M,    ch_N,    ch_O,
    ch_P,    ch_Q,    ch_R,    ch_S,    ch_T,    ch_U,    ch_V,    ch_W,
    ch_X,    ch_Y,    ch_Z,    ch_OBRS, ch_BSL,  ch_CBRS, ch_xx,   ch_UN,
    ch_xx,   ch_a,    ch_b,    ch_c,    ch_d,    ch_e,    ch_f,    ch_g,
    ch_h,    ch_i,    ch_j,    ch_k,    ch_l,    ch_m,    ch_n,    ch_o,
    ch_p,    ch_q,    ch_r,    ch_s,    ch_t,    ch_u,    ch_v,    ch_w,
    ch_x,    ch_y,    ch_z,    ch_xx,   ch_OR,   ch_xx,   ch_xx,   ch_xx
};


/*****************************************************************************
|*
|*  Function:           InitLcd
|*
|*  Description:
|*
|*      Initialize the LCD.
|*
 */
void InitLcd(void)
{
    SYSCON = 0x20;
    LCON   = 0xC1;
    DAC0   = 0xA0;
    LCRL   = 0x84;
    LCRH   = 0xC1;

    ClearLcd();
}

/*****************************************************************************
|*
|*  Function:           WriteLcd
|*
|*  Description:
|*
|*      Write a character to the LCD. This function assumes the LCD to be
|*      initialized (using InitLcd()). When the character to be printed
|*	is a '.' it will be patched into the previous character.
|*
 */
void WriteLcd( unsigned char position, unsigned char input )
{
    static unsigned char last_char = ' ';
	
    if ( position < LCD_WIDTH )
    {
	if ( input == '.' )
	{
	    LCD[position] = CHR[last_char] | ch_DOT;
	}
	else
	{
	    LCD[position] = CHR[input];
	}
	last_char = input;
    }
}

/*****************************************************************************
|*
|*  Function:           WriteLcdStr
|*
|*  Description:
|*
|*      Write a string to the LCD. This function assumes the LCD to be
|*      initialized (using InitLcd()). When the character to be printed
|*	is a '.' it will be patched into the previous character.
|*
 */
void WriteLcdStr( char * str )
{
	char	position = 0;
	char	ptr = 0;

	while ( position < LCD_WIDTH )
	{
		WriteLcd( position, str[ptr] );
		ptr++;

		/* If the next character is a '.' don't increment 'position'. *
		 * Because a seperate segment is present for the '.' it will  *
		 * be shown on the same position as the previous character    */
		if ( str[ptr] != '.' ) 
		{
			position++;
		}
	}
}

/*****************************************************************************
|*
|*  Function:           ClearLcd
|*
|*  Description:
|*
|*      Clear the entire LCD.
|*
 */
void ClearLcd( void )
{
    int i;

    for ( i=0; i < LCD_WIDTH; i++ )
    {
	LCD[i] = 0; /* clear all segments */
    }
}

/*****************************************************************************
|*
|*  Function:           SetContrast
|*
|*  Description:
|*
|*      Set the contrast level of the LCD.
|*
 */
void SetContrast( unsigned char Contrast )
{
    DAC0 = Contrast;
}
