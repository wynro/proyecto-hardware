/*********************************************************************************************
* File£º	flash.c
* Author:	embest
* Desc£º	SST39VF160 Flash program code
* History:	
*********************************************************************************************/

/*--- include files ---*/

#include "flash.h"

/*--- global variables ---*/

/*--- function declare ---*/

int Identify(int base_addr);
int erase_flash(int base_addr);
int erase_sector(int base_addr);
int wait_flash_ready(int address, pB0SIZE data);
extern void DelayTime(int num);

/*--- function code ---*/

/*********************************************************************************************
* name:		Test_Flash
* func:		test flash program
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void Test_Flash(void)
{
	char data[256];
	int  i;
	
	Uart_Printf("\n  SST39VF160-90 (U12) \n");
	Uart_Printf("\nWrite 0x000-0xff to flash address 0x30000 ...\n");
	
	for( i = 0 ; i < 256 ; i++ )
	{
		data[i] = i;
	}
	
	if( Program(0x30000,data,256) == TRUE )
		Uart_Printf("\nFlash Write and Check Success!\n");
}

/*********************************************************************************************
* name:		Program
* func:		program data to flash
* para:		addr_base -- target flash start address
*			pData     -- source data pointer
*			data_size -- source data size
* ret:		none
* modify:
* comment:		
********************************************************************************************/
int Program(int addr_base, char* pData, int data_size)
{
	char sec_data[4096];					// sector data
    int base_addr,sector_addr,length;
    int i,j,sect_num;
    short *pSrc;
    short *pDest;
	
	base_addr = ((int)addr_base& ~FLASH_MASK);

	/*--- check flash type ---*/
	if( Identify(base_addr) == FALSE )
		return;
	
	/* count sector number */
	sect_num = (((addr_base+data_size)&SECTOR_MASK) - (addr_base&SECTOR_MASK))/0x1000+1;
	
	/* erase each sector */
	sector_addr = addr_base & SECTOR_MASK;
	for( i = 0 ; i < sect_num ; i++ )
	{
		sector_addr = sector_addr + i*0x1000;
		/* read data from sector */
		for( j = 0 ; j < 1024 ; j++ )
			((long *)sec_data)[j] = ((long *)sector_addr)[j];
		if( i == 0 )
		{
			/* copy length */
			length = 4096-(addr_base-sector_addr);
			length = (length < data_size)? length : data_size;
			/* copy target data to temp data */
			
			for( j = 0 ; j < length ; j++ )
			{
				sec_data[addr_base-sector_addr+j] = pData[j];
			}
		}
		else if( i == (sect_num - 1) )
		{
			length = (addr_base+data_size) - sector_addr;
			for( j = 0 ; j < length ; j++ )
			{
				sec_data[j] = pData[data_size-length+j];
			}
		}
		else
		{
			for( j = 0 ; j < 4096 ; j++ )
			{
				sec_data[j] = pData[sector_addr-addr_base+j];
			}
		}
		/* erase sector */
		erase_sector(sector_addr);
		/* write data to sector */
		pDest = (short *)sector_addr;
		pSrc = (short *)sec_data;
		for( j = 0 ; j < 2048 ; j++ )
		{
		    *((volatile short *)(base_addr + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
		    *((volatile short *)(base_addr + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
		    *((volatile short *)(base_addr + FLASH_SEQ_ADD_1)) = WRITE_WORD_COM;
		    pDest[j] = pSrc[j];
		    DelayTime(10);
		    /* check data */
		    if( pDest[j] != pSrc[j] )
		    {
		    	Uart_Printf("\nwrite address %x error!\n",sector_addr+j*2);
		    	return FALSE;
		    }
		}
	}
	
	return TRUE;
}

/*********************************************************************************************
* name:		Identify
* func:		identify flash type
* para:		base_addr -- flash start address
* ret:		none
* modify:
* comment:		
********************************************************************************************/
int Identify(int base_addr)
{
    int manuf_code, device_code;

    /* Enter Software Product Identification Mode  */
    *((short *)(base_addr + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
    *((short *)(base_addr + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
    *((short *)(base_addr + FLASH_SEQ_ADD_1)) = ID_IN_WORD_COM;
    /* Wait the device to change state */
    DelayTime(50);

    /* Read Manufacturer and device code from the device */
    manuf_code  = (*((short *)(base_addr + 0))) & 0x00FF;
    device_code = (*((short *)(base_addr + 2))) & 0xFFFF;

    /* Exit Software Product Identification Mode  */
    *((short *)(base_addr + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
    *((short *)(base_addr + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
    *((short *)(base_addr + FLASH_SEQ_ADD_1)) = ID_OUT_WORD_COM;
    /* Wait */
    DelayTime(50);

    /* Check the Manufacturer and Flash ID */
    if( manuf_code != MANU_ID )
    {
    	Uart_Printf("\nFlash Manufacturer ID Error!\n");
        return FALSE;
    }
    if( device_code != FLASH_ID )
    {
    	Uart_Printf("\nFlash Device ID Error!\n");
        return FALSE;
    }

	return TRUE;
}

/*********************************************************************************************
* name:		erase_sector
* func:		erase a sector on flash
* para:		base_addr -- flash start address
* ret:		none
* modify:
* comment:		
********************************************************************************************/
int erase_sector(int base_addr)
{
	int addr_base;
	
	/* erase flash chip */
	addr_base = ((int)base_addr& ~FLASH_MASK);
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
	*((short *)(addr_base + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = ERASE_SECTOR_CODE1;
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
	*((short *)(addr_base + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
	*((short *)(base_addr)) = ERASE_SECTOR_CODE2;

	DelayTime(30);
	//* Wait for Flash Ready after Erase, if timeout
	if(wait_flash_ready(base_addr,0xFFFF ) == FALSE)
	{
		Uart_Printf("\nErase sector %x Error!\n",base_addr);
		return FALSE;
	}
    return TRUE;
}

/*********************************************************************************************
* name:		erase_flash
* func:		erash all flash data
* para:		base_addr -- flash start address
* ret:		none
* modify:
* comment:		
********************************************************************************************/
int erase_flash(int base_addr)
{
	int addr_base;
	
	/* erase flash chip */
	addr_base = ((int)base_addr& ~FLASH_MASK);
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
	*((short *)(addr_base + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = ERASE_SECTOR_CODE1;
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = FLASH_WORD_COM_1;
	*((short *)(addr_base + FLASH_SEQ_ADD_2)) = FLASH_WORD_COM_2;
	*((short *)(addr_base + FLASH_SEQ_ADD_1)) = CHIP_SECTOR_CODE1;

	DelayTime(30);
	//* Wait for Flash Ready after Erase, if timeout
	if(wait_flash_ready(addr_base,0xFFFF ) == FALSE)
	{
		Uart_Printf("\nErase Flash Error!\n");
		return FALSE;
	}
    return TRUE;
}

/*********************************************************************************************
* name:		wait_flash_ready
* func:		wait flash operate until success or timeout
* para:		address -- compare address
*			data    -- compare data
* ret:		none
* modify:
* comment:		
********************************************************************************************/
int wait_flash_ready(int address, pB0SIZE data)
{
    pB0SIZE ulTemp;
	int i = 0 ;

    /* While two consecutive read don't give same value or timeout */
	do
	{
		ulTemp = *((pB0SIZE *)address);
	}while( (ulTemp != data) && (i++ < TIME_OUT) );

    if( i < TIME_OUT )
		return TRUE;
    else
    {
        return FALSE;
    }
}