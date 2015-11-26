/*********************************************************************************************
* File£º	8led.c
* Author:	embest	
* Desc£º	8-segment digit LED control functions
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44b.h"
#include "44blib.h"

/*--- macro defines ---*/
/* Bitmaps for 8-segment */
#define SEGMENT_A		0x80		
#define SEGMENT_B		0x40
#define SEGMENT_C		0x20
#define SEGMENT_D		0x08
#define SEGMENT_E		0x04
#define SEGMENT_F		0x02
#define SEGMENT_G		0x01
#define SEGMENT_P		0x10

#define DIGIT_F	(SEGMENT_A | SEGMENT_G | SEGMENT_E | SEGMENT_F)
#define DIGIT_E	(SEGMENT_A | SEGMENT_G | SEGMENT_E | SEGMENT_F | SEGMENT_D)
#define DIGIT_D	(SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_E)
#define DIGIT_C	(SEGMENT_A | SEGMENT_D | SEGMENT_E | SEGMENT_G)
#define DIGIT_B	(SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_E | SEGMENT_G)
#define DIGIT_A	(SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_E | SEGMENT_G)
#define DIGIT_9	(SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G)
#define DIGIT_8	(SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_E | SEGMENT_G)
#define DIGIT_7	(SEGMENT_A | SEGMENT_B | SEGMENT_C)
#define DIGIT_6	(SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_E | SEGMENT_G)
#define DIGIT_5	(SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G)
#define DIGIT_4	(SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G)
#define DIGIT_3	(SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F)
#define DIGIT_2	(SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_F)
#define DIGIT_1	(SEGMENT_B | SEGMENT_C)
#define DIGIT_0	(SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_G)

/* 8led control register address */
#define	LED8ADDR	(*(volatile unsigned char *)(0x2140000))

/*--- global variables ---*/
/* Digit Symbol table*/
int Symbol[] = { DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7,
    			 DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B, DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F};
/* LED segment table */
int Seg[] = { SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_P};
			  					  
/*--- functions declare ---*/
void Digit_Led_Test(void);
void Digit_Led_Symbol(int value);
void Digit_Led_Segment(int seg_num);

/*--- extern functions ---*/
extern void Delay(int time);

/*--- function code---*/
/*********************************************************************************************
* name:		Digit_Led_Test
* func:		8-segment digit LED test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Digit_Led_Test(void)
{
	int i;

	/* display all digit from 0 to F */
	for(i=0; i<16; i++)
	{
		Digit_Led_Symbol(i);
		Delay(4000);
	}
}

/*********************************************************************************************
* name:		Digit_Led_Symbol
* func:		display digit symbol on 8-segment digit LED's segment
* para:		value --  digit's hexadecimal value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Digit_Led_Symbol(int value)
{
	/* symbol display */
	if( (value >= 0) && (value < 16) )
		LED8ADDR = ~Symbol[value];	
}

/*********************************************************************************************
* name:		Digit_Led_Segment
* func:		8-segment digit LED's segment display control function
* para:		seg_num -- segment number
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Digit_Led_Segment(int seg_num)
{
	/* segment control */
	if( (seg_num >= 0) && (seg_num < 8) )
		LED8ADDR = ~Seg[seg_num];	
}
