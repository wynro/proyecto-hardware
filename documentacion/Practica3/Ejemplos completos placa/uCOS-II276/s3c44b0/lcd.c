/*********************************************************************************************
* File£º	lcd.c
* Author:	embest	
* Desc£º	LCD control and display functions
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "def.h"
#include "44b.h"
#include "44blib.h"
#include "lcd.h"


/*--- macro define ---*/
#define DMA_Byte  (0)
#define DMA_HW    (1)
#define DMA_Word  (2)
#define DW 		  DMA_Byte		//set ZDMA0 as half-word
	
/*--- extern variables ---*/
//extern INT8U g_auc_Ascii6x8[];
extern INT8U g_auc_Ascii8x16[];


/*--- function code ---*/
/*********************************************************************************************
* name:		Lcd_Init()
* func:		Initialize LCD Controller
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Init(void)
{       
	rDITHMODE=0x1223a;
	rDP1_2 =0x5a5a;      
	rDP4_7 =0x366cd9b;
	rDP3_5 =0xda5a7;
	rDP2_3 =0xad7;
	rDP5_7 =0xfeda5b7;
	rDP3_4 =0xebd7;
	rDP4_5 =0xebfd7;
	rDP6_7 =0x7efdfbf;

	rLCDCON1=(0)|(1<<5)|(MVAL_USED<<7)|(0x0<<8)|(0x0<<10)|(CLKVAL_GREY16<<12);
	rLCDCON2=(LINEVAL)|(HOZVAL<<10)|(10<<21); 
	rLCDSADDR1= (0x2<<27) | ( ((LCD_ACTIVE_BUFFER>>22)<<21 ) | M5D(LCD_ACTIVE_BUFFER>>1));
 	rLCDSADDR2= M5D(((LCD_ACTIVE_BUFFER+(SCR_XSIZE*LCD_YSIZE/2))>>1)) | (MVAL<<21);
	rLCDSADDR3= (LCD_XSIZE/4) | ( ((SCR_XSIZE-LCD_XSIZE)/4)<<9 );
	// enable,4B_SNGL_SCAN,WDLY=8clk,WLH=8clk,
	rLCDCON1=(1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_GREY16<<12);
	rBLUELUT=0xfa40;
	//Enable LCD Logic and EL back-light.
	rPDATE=rPDATE&0x0e;
}

/*********************************************************************************************
* name:		Lcd_Active_Clr()
* func:		clear LCD screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Active_Clr(void)
{
	INT32U i;
	INT32U *pDisp = (INT32U *)LCD_ACTIVE_BUFFER;
	
	for( i = 0; i < (SCR_XSIZE*SCR_YSIZE/2/4); i++ )
	{
		*pDisp++ = WHITE;
	}
}

/*********************************************************************************************
* name:		Lcd_GetPixel()
* func:		Get appointed point's color value
* para:		usX,usY -- pot's X-Y coordinate 
* ret:		pot's color value
* modify:
* comment:		
*********************************************************************************************/
INT8U LCD_GetPixel(INT16U usX, INT16U usY)
{
	INT8U ucColor;

	ucColor = *((INT8U*)(LCD_VIRTUAL_BUFFER + usY*SCR_XSIZE/2 + usX/8*4 + 3 - (usX%8)/2));
	ucColor = (ucColor >> ((1-(usX%2))*4)) & 0x0f;
	return ucColor;
}

/*********************************************************************************************
* name:		Lcd_Active_Clr()
* func:		clear virtual screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Clr(void)
{
	INT32U i;
	INT32U *pDisp = (INT32U *)LCD_VIRTUAL_BUFFER;
	
	for( i = 0; i < (SCR_XSIZE*SCR_YSIZE/2/4); i++ )
	{
		*pDisp++ = WHITE;
	}
}

/*********************************************************************************************
* name:		LcdClrRect()
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void LcdClrRect(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor)
{
	INT16 i,k,l,m;
	
	INT32U ulColor = (ucColor << 28) | (ucColor << 24) | (ucColor << 20) | (ucColor << 16) | 
				     (ucColor << 12) | (ucColor << 8) | (ucColor << 4) | ucColor;

	i = k = l = m = 0;	
	if( (usRight-usLeft) <= 8 )
	{
		for( i=usTop; i<=usBottom; i++)
		{
			for( m=usLeft; m<=usRight; m++)
			{
				LCD_PutPixel(m, i, ucColor);
			}
		}	
		return;
	}

	/* check borderline */
	if( 0 == (usLeft%8) )
		k=usLeft;
	else
	{
		k=(usLeft/8)*8+8;
	}
	if( 0 == (usRight%8) )
		l= usRight;
	else
	{
		l=(usRight/8)*8;
	}

	for( i=usTop; i<=usBottom; i++ )
	{
		for( m=usLeft; m<=(k-1); m++ )
		{
			LCD_PutPixel(m, i, ucColor);
		}
		for( m=k; m<l; m+=8 )
		{
			(*(INT32U*)(LCD_VIRTUAL_BUFFER + i * SCR_XSIZE / 2 + m / 2)) = ulColor;
		}
		for( m=l; m<=usRight; m++ )
		{
			LCD_PutPixel(m, i, ucColor);
		}
	}
}

/*********************************************************************************************
* name:		Lcd_DspAscII8x16()
* func:		display 8x16 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U * s)
{
	INT16 i,j,k,x,y,xx;
	INT8U qm;
	INT32U ulOffset;
	INT8 ywbuf[16],temp[2];
    
	for( i = 0; i < strlen((const char*)s); i++ )
	{
		if( (INT8U)*(s+i) >= 161 )
		{
			temp[0] = *(s + i);
			temp[1] = '\0';
			return;
		}
		else
		{
			qm = *(s+i);
			ulOffset = (INT32U)(qm) * 16;		//Here to be changed tomorrow
			for( j = 0; j < 16; j ++ )
			{
				ywbuf[j] = g_auc_Ascii8x16[ulOffset + j];
            }

            for( y = 0; y < 16; y++ )
            {
            	for( x = 0; x < 8; x++ ) 
               	{
                	k = x % 8;
			    	if( ywbuf[y]  & (0x80 >> k) )
			       	{
			       		xx = x0 + x + i*8;
			       		LCD_PutPixel(xx, y + y0, (INT8U)ForeColor);
			       	}
			   	}
            }
		}
	}
}




/*********************************************************************************************
* name:		ReverseLine()
* func:		Reverse display some lines 
* para:		ulHeight -- line's height
*			ulY -- line's Y-coordinate
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void ReverseLine(INT32U ulHeight, INT32U ulY)
{
	INT32U i, j, temp;
	
	for( i = 0; i < ulHeight; i++ )
	{
		for( j = 0; j < (SCR_XSIZE/4/2) ; j++ )
		{
			temp = *(INT32U*)(LCD_VIRTUAL_BUFFER + (ulY+i)*SCR_XSIZE/2 + j*4);
			temp ^= 0xFFFFFFFF;
			*(INT32U*)(LCD_VIRTUAL_BUFFER + (ulY+i)*SCR_XSIZE/2 + j*4) = temp;
		}
	}
}

/*********************************************************************************************
* name:		Zdma0Done()
* func:		LCD dma interrupt handle function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static INT8U ucZdma0Done=1;	//When DMA is finish,ucZdma0Done is cleared to Zero
void Zdma0Done(void)
{
	rI_ISPC=BIT_ZDMA0;	    //clear pending
	ucZdma0Done=0;
}

/*********************************************************************************************
* name:		Lcd_Dma_Trans()
* func:		dma transport virtual LCD screen to LCD actual screen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Dma_Trans(void)
{
	INT8U err;
	
	ucZdma0Done=1;
	//#define LCD_VIRTUAL_BUFFER	(0xc400000)
	//#define LCD_ACTIVE_BUFFER	(LCD_VIRTUAL_BUFFER+(SCR_XSIZE*SCR_YSIZE/2))	//DMA ON
	//#define LCD_ACTIVE_BUFFER	LCD_VIRTUAL_BUFFER								//DMA OFF
	//#define LCD_BUF_SIZE		(SCR_XSIZE*SCR_YSIZE/2)
	//So the Lcd Buffer Low area is from LCD_VIRTUAL_BUFFER to (LCD_ACTIVE_BUFFER+(SCR_XSIZE*SCR_YSIZE/2))
	rNCACHBE1=(((unsigned)(LCD_ACTIVE_BUFFER)>>12) <<16 )|((unsigned)(LCD_VIRTUAL_BUFFER)>>12);
  	rZDISRC0=(DW<<30)|(1<<28)|LCD_VIRTUAL_BUFFER; // inc
  	rZDIDES0=( 2<<30)  |(1<<28)|LCD_ACTIVE_BUFFER; // inc
        rZDICNT0=( 2<<28)|(1<<26)|(3<<22)|(0<<20)|(LCD_BUF_SIZE);
        //                      |            |            |             |            |---->0 = Disable DMA
        //                      |            |            |             |------------>Int. whenever transferred
        //                      |            |            |-------------------->Write time on the fly
        //                      |            |---------------------------->Block(4-word) transfer mode
        //                      |------------------------------------>whole service
	//reEnable ZDMA transfer
  	rZDICNT0 |= (1<<20);		//after ES3
    rZDCON0=0x1; // start!!!  

	Delay(500);
	//while(ucZdma0Done);		//wait for DMA finish
}


/*********************************************************************************************
* name:		LCD_view
* func:		print message to LCD
* para:		col   -- y value
			row   -- x value
			color -- color
			info  -- deplay message pointer 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void LCD_view(INT16 col, INT16 row, INT8U color, INT8U * info)
{
//void LcdClrRect(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor)
	LcdClrRect(col,row,316,row+8,WHITE);
	LcdClrRect(col,row+8,316,row+16,WHITE);
	Lcd_Dma_Trans();
	
	Lcd_DspAscII8x16(col,row,BLACK,info); // 
	Lcd_Dma_Trans();
}

void Lcd_Display(INT8U *s)
{

	
	
	Lcd_Clr();
	Lcd_Active_Clr();
	
	Lcd_DspAscII8x16(5,0,BLACK,"*=====================================*");
	Lcd_DspAscII8x16(5,10,BLACK,"*= ENGG*4420 Realtime Systems Design =*");
	Lcd_DspAscII8x16(5,20,BLACK,"*======== School of Engineering ======*");
	Lcd_DspAscII8x16(5,30,BLACK,"*======== University of Guelph =======*");
	Lcd_DspAscII8x16(5,40,BLACK,"*=====================================*");
	Lcd_DspAscII8x16(5,50,BLACK,"*====== LAB2: Start-stop Watch =======*");
	Lcd_DspAscII8x16(5,60,BLACK,"*=======       July, 2005       ======*");
	Lcd_DspAscII8x16(5,70,BLACK,"*=====================================*");
			
	Lcd_DspAscII8x16(125,125,BLACK, s);

	Lcd_Dma_Trans();

}