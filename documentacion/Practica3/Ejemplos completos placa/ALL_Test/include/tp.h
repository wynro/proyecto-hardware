/*********************************************************************************************
* File£º	tp.H
* Author:	embest	
* Desc£º	Touch Screen define file
* History:	
*********************************************************************************************/

#ifndef __TP_H__
#define __TP_H__

extern void TS_init(void);
extern void TS_close(void);
extern void user_irq1();

#endif /*__TP_H__*/

/*--- global  variables ---*/
volatile unsigned int  UserTouch=0;
volatile char CheckTSP;
char oneTouch;
unsigned int  Vx;
unsigned int  Vy;
unsigned int  Xmax;
unsigned int  Ymax;
unsigned int  Xmin;
unsigned int  Ymin;

void TS_Test(void);
void TS_init(void);
void TSInt(void);
void TS_close(void);
void Lcd_TC(void);
void DesignREC(ULONG tx, ULONG ty);
void Check_Sel(void);
