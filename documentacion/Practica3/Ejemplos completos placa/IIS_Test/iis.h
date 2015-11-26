/*********************************************************************************************
* File£º	IIS.H
* Author:	embest	
* Desc£º	IIS sound header file
* History:	
*********************************************************************************************/

#ifndef __IIS_H__
#define __IIS_H__

#define FS441KHZ	1

/*--- function decleare ---*/
void IISInit(void);
void IISClose(void);
void Playwave(int times);
void Record_Iis(void);
void BDMA0_Done(void) __attribute__ ((interrupt ("IRQ")));
void BDMA0_Rec_Done(void) __attribute__ ((interrupt ("IRQ")));

#endif /* __IIS_H__ */
