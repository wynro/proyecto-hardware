/*********************************************************************************************
* File£º	Flash.H
* Author:	embest	
* Desc£º	SST39VF160 header file
* History:	
*********************************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

/* Define the specific for the flash memory system */
#define MANU_ID           	0xBF
#define TARGET_ID           "S3CEV40"
#define FLASH_TYPE          "SST39VF160"
#define FLASH_ID            0x2782
#define SECTOR_SIZE         4096

/* FLASH Mask */
#define FLASH_MASK	0x00FFFFFF
#define SECTOR_MASK	0xFFFFF000

/* Local routine defines for the flashwrt.c module */
#define MIN_NO_OF_ARGS   	3
#define FILL_PATTERN     	0x5a

/* Timeout loop count */
#define TIME_OUT            100000

/* command word */
#define FLASH_SEQ_ADD_1  	(0x5555 << 1)
#define FLASH_SEQ_ADD_2  	(0x2AAA << 1)
#define FLASH_WORD_COM_1 	((short)0xAAAA)
#define FLASH_WORD_COM_2 	((short)0x5555)
#define WRITE_WORD_COM    	((short)0xA0A0)
#define ID_IN_WORD_COM   	((short)0x9090)
#define ID_OUT_WORD_COM  	((short)0xF0F0)
#define ERASE_SECTOR_CODE1  ((short)0x8080)
#define CHIP_SECTOR_CODE1   ((short)0x1010)
#define ERASE_SECTOR_CODE2  ((short)0x3030)

/********************************/
/* DEFINE BANK 0 DATA BUS WIDTH */
/********************************/
#define B0SIZE_BYTE			0x1
#define B0SIZE_SHORT		0x2
#define B0SIZE_WORD			0x3

#define _B0SIZE   			B0SIZE_SHORT  // Select Bank0 bus width

#if  _B0SIZE == B0SIZE_BYTE
typedef unsigned char pB0SIZE;
#elif _B0SIZE == B0SIZE_SHORT
typedef unsigned short pB0SIZE;
#elif _B0SIZE == B0SIZE_WORD
typedef unsigned long pB0SIZE;
#endif

#ifndef FALSE
#define FALSE		0
#endif
#ifndef TRUE
#define TRUE		1
#endif

/*****************************************/
/* Functions for SST SST39VF160D	     */
/*****************************************/
extern int Program(int addr_base, char* pData, int data_size);
extern int erase_flash(int base_addr);

#endif /* __FLASH_H__ */