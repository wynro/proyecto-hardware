/*********************************************************************************************
* File£º	EtherNet.c
* Author:	embest	
* Desc£º	ethernet MAC protocol and control code
* History:	
*********************************************************************************************/

/*--- include files ---*/

#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "option.h"
#include "ethernet.h"

/*--- global variables ---*/

/* local MAC Address */
char mac_addr[6] = { 0x00,0x06,0x98,0x01,0x7E,0x8F };

/* data buffer and operate signal */
char  net_out_data[1536];
char  net_in_data[1536];
int	  iReceiveData;
int   iSendData;

/* Ethernet control address */
volatile UCHAR *ether_base = (UCHAR *)0x02180000;
int EtherOutput(UCHAR *dmac,char* pdata, USHORT len);
int EtherInput(USHORT tms);
int iLoopBackMode;

/*--- function decleare ---*/

int  NicReset(void);
void NicInit(void);
void NicClose(void);

/*--- function code ---*/

/*********************************************************************************************
* name:		NicInit
* func:		initialize 8019AS chip
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void NicInit(void)
{
    int i;
    char temp;

	if(NicReset())
	{
		Uart_Printf("\nReset 8019AS fail!\n");
        return;
	}
        
	nic_write(NIC_PG0_IMR, 0x0);		// disable 8019 interrupt
    nic_write(NIC_PG0_ISR, 0xff);		// clear interrupt status
    
    /* stop command,complete DMA,RTL8019 configuration(Page 3) */
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2 | NIC_CR_PS0 | NIC_CR_PS1);
    /* write register enable */
    nic_write(NIC_PG3_EECR, NIC_EECR_EEM0 | NIC_EECR_EEM1);	
    /* jumper mode, half-duplex */
    nic_write(NIC_PG3_CONFIG3, 0x0);
    /* 8019as */
    nic_write(NIC_PG3_CONFIG0, 0x0);
    /* Auto-detect, BROM disable */
    nic_write(NIC_PG3_CONFIG2, NIC_CONFIG2_BSELB);
    /* write register disable */
    nic_write(NIC_PG3_EECR, 0x0);
    DelayTime(100);						// delay some times
    /* Page 0 configure */
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2);
   	/* Normal Operate, FIFO threshold 1 */
    nic_write(NIC_PG0_DCR, NIC_DCR_LS | NIC_DCR_FT1);
    /* clear Remote byte count Register */
    nic_write(NIC_PG0_RBCR0, 0x0);
    nic_write(NIC_PG0_RBCR1, 0x0);
    /* receive monitor mode */
    nic_write(NIC_PG0_RCR, NIC_RCR_MON);
    /* Internel Loopback */
    nic_write(NIC_PG0_TCR, NIC_TCR_LB0);
    /* Transmit Page Start */
    nic_write(NIC_PG0_TPSR, NIC_FIRST_TX_PAGE);
    /* Boundary */
    nic_write(NIC_PG0_BNRY, NIC_STOP_PAGE-1);
    /* Receive Page Start */
    nic_write(NIC_PG0_PSTART, NIC_FIRST_RX_PAGE);
    /* Receive Page Stop */
    nic_write(NIC_PG0_PSTOP, NIC_STOP_PAGE);
    nic_write(NIC_PG0_ISR, 0xff);		// clear interrupt status
    /* Page 1 configure */
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2 | NIC_CR_PS0);
    /* set MAC address */
    for( i = 0; i < 6; i++ )
        nic_write(NIC_PG1_PAR0 + i, mac_addr[i]);
    /* broadcast address is 0x0 */
    for( i = 0; i < 8; i++ )
        nic_write(NIC_PG1_MAR0 + i, 0);
    /* Current Page */
    nic_write(NIC_PG1_CURR, NIC_FIRST_RX_PAGE);
    /* Page 0 Configure */
    nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2);
    /* only accept destination address message */
    nic_write(NIC_PG0_RCR, 0x0);
    nic_write(NIC_PG0_ISR, 0xff);		// clear interrupt status
    /* enable received interrupt */
    nic_write(NIC_PG0_IMR, NIC_IMR_PRXE);			
    /* Start command */
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
   	/* CRC enable, Normal Operate */
    nic_write(NIC_PG0_TCR, 0x0);
    DelayTime(100);						// delay some times
}

/*********************************************************************************************
* name:		NicClose
* func:		close 8019AS chip
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void NicClose(void)
{
	/* close ethernet */
	nic_write(NIC_PG0_IMR, 0);			// disable interrupt
    nic_write(NIC_PG0_ISR, 0xff);		// clear interrupt status
	nic_write(NIC_CR, NIC_CR_STP | NIC_CR_RD2);
}

/*********************************************************************************************
* name:		NicReset
* func:		Reset 8019AS chip
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
int NicReset(void)
{
    UCHAR i;
    UCHAR j;

    for( j = 0; j < 20; j++ )
    {
        i = nic_read(NIC_RESET);
        DelayTime(50);
        nic_write(NIC_RESET, i);
        for( i = 0; i < 20; i++ )
        {
            DelayTime(50);
            if(nic_read(NIC_PG0_ISR) & NIC_ISR_RST)
                return 0;
        }
    }
    return -1;
}

/*********************************************************************************************
* name:		EtherOutput
* func:		output data through ethernet 8019AS chip
* para:		pdata -- data pointer
*			len	  -- data length
* ret:		0 -- success 		-1 -- fail
* modify:
* comment:		
*********************************************************************************************/
int EtherOutput(UCHAR *dmac,char* pdata, USHORT len)
{
    USHORT i;
    USHORT send_len;
    UCHAR *cp;
	struct ETHERHDR *eh;
	
	/* set MAC address */
	eh = (struct ETHERHDR *)pdata;
    for(i = 0; i < 6; i++)
        eh->ether_shost[i] = mac_addr[i];
    for(i = 0; i < 6; i++)
        eh->ether_dhost[i] = dmac[i];
    
    send_len = len + sizeof(struct ETHERHDR);
    if( send_len > 1518 )
    {
    	Uart_Printf("\nCann't send data more than 1518 bytes!\n");
    	return -1;
    }
    /* at less send 60 byte */
    if( send_len < 60 )
        send_len = 60;

	/* Register Page 0 operate */
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
    /* set byte count */
    nic_write(NIC_PG0_RBCR0, (UCHAR)(send_len));
    /* set byte count */
    nic_write(NIC_PG0_RBCR1, (UCHAR)(send_len >> 8));
    /* Remote Start Address*/
    nic_write(NIC_PG0_RSAR0, 0);
    /* Remote Start Address */
    nic_write(NIC_PG0_RSAR1, NIC_FIRST_TX_PAGE);
    /* remote write */
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD1);
    
    /* Transfer data. */
    cp = pdata;
    for( i = 0; i < send_len; i++, cp++ )
        nic_write(NIC_IOPORT, *cp);
    for( i = send_len; i < 60; i++ )
        nic_write(NIC_IOPORT, 0);
    
    /** Complete remote dma. */
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
    /* if completed */
    for( i = 0; i <= 20; i++ )
        if(nic_read(NIC_PG0_ISR) & NIC_ISR_RDC)
            break;
    /* clear status */
    nic_write(NIC_PG0_ISR, NIC_ISR_RDC);
        
    /* Number of bytes to be transmitted. */
    nic_write(NIC_PG0_TBCR0, ((send_len) & 0xff));
    nic_write(NIC_PG0_TBCR1, ((send_len >> 8) & 0xff));
        
    /* First page of packet to be transmitted. */
    nic_write(NIC_PG0_TPSR, NIC_FIRST_TX_PAGE);
        
    /* Start transmission. */
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_TXP | NIC_CR_RD1 | NIC_CR_RD0);

    /* Wait until transmission is completed or aborted. */
    while(nic_read(NIC_CR) & NIC_CR_TXP);

	/* Complete DMA. */
    nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2 );

	iSendData = 0;
	
    return 0;
}

/*********************************************************************************************
* name:		EtherInput
* func:		get data from ethernet
* para:		tms -- timeout value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
int EtherInput(USHORT tms)
{
    int    rc = 0;
    UCHAR  isr = 0;
    struct NICPKYTHEADER hdr;
    UCHAR  *buf;
    UCHAR  nextpg;
    UCHAR  bnry;
    UCHAR  curr;
    USHORT i;
    UCHAR  wtc;

	tms = tms*50;
    while( (tms && rc) == 0 )
    {
        for( wtc = 1; tms; wtc++ )
        {
            isr = nic_read(NIC_PG0_ISR);
            /* Is error ? */
            if( isr & NIC_ISR_RXE )
            {
            	/* clear interrupt status register */
            	nic_write(NIC_PG0_ISR,isr);
                return -1;
            }
            /* get packet */
            if( isr & NIC_ISR_PRX )
                break;
            /* Is timeout */
            if( wtc == 0 )
                tms--;
            
            if(Uart_GetKey() != 0) 	Main();
        }
        
        /* Any frame received? */
        if( (isr & NIC_ISR_PRX) == 0 )
        {
        	/* clear interrupt status register */
            nic_write(NIC_PG0_ISR, isr);
            break;
        }
    
        /* Get the current page pointer. It points to the page where the NIC 
         * will start saving the next incoming packet. */
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2 | NIC_CR_PS0);
        curr = nic_read(NIC_PG1_CURR);
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
        
        /* Get the pointer to the last page we read from. The following page
         * is the one where we start reading. */
        if( (bnry = (nic_read(NIC_PG0_BNRY) + 1)) >= NIC_STOP_PAGE )
            bnry = NIC_FIRST_RX_PAGE;
            
        /* if bnry equal curr, mean not data ,nothing to read */
        if( bnry == curr )
        {
            nic_write(NIC_PG0_ISR, isr);
            continue;
        }
        
        /* Read the NIC specific packet header. */
        nic_write(NIC_PG0_RBCR0, sizeof(struct NICPKYTHEADER));
        nic_write(NIC_PG0_RBCR1, 0);
        nic_write(NIC_PG0_RSAR0, 0);
        nic_write(NIC_PG0_RSAR1, bnry);
        buf = (UCHAR *)&hdr;
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD0);
        for( i = 0; i < sizeof(struct NICPKYTHEADER); i++ )
            *buf++ = nic_read(NIC_IOPORT);
        
        /** Complete remote dma. */
        nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
        for( i = 0; i <= 20; i++ )
        {
            if(nic_read(NIC_PG0_ISR) & NIC_ISR_RDC)
                break;
        }
        nic_write(NIC_PG0_ISR, NIC_ISR_RDC);
        
        /* Check packet length. */
        if( hdr.ph_size < (60 + sizeof(struct NICPKYTHEADER)) ||
            hdr.ph_size > (1518 + sizeof(struct NICPKYTHEADER)) )
        {
        	nic_write(NIC_PG0_ISR, isr);
            return 0;
        }
        
        /* Calculate the page of the next packet. If it differs from the
         * pointer in the packet header, we discard the whole buffer
         * and return a null pointer. */
        nextpg = bnry + (hdr.ph_size >> 8) + ((hdr.ph_size & 0xFF) != 0);
        if( nextpg >= NIC_STOP_PAGE )
        {
            nextpg -= NIC_STOP_PAGE;
            nextpg += NIC_FIRST_RX_PAGE;
        }
        if( nextpg != hdr.ph_nextpg )
        {
            UCHAR nextpg1 = nextpg + 1;
            
            if( nextpg1 >= NIC_STOP_PAGE )
            {
                nextpg1 -= NIC_STOP_PAGE;
                nextpg1 += NIC_FIRST_RX_PAGE;
            }
            if( nextpg1 != hdr.ph_nextpg )
            {
                nic_write(NIC_PG0_ISR, isr);
                break;
            }
            nextpg = nextpg1;
        }
        
        /* Check packet status. */
        if( (hdr.ph_status & 0x0F) == 1 )
        {
            rc = hdr.ph_size - sizeof(struct NICPKYTHEADER);
            
            /* Set remote dma byte count and start address. Don't read the
             * header again. */
            nic_write(NIC_PG0_RBCR0, (UCHAR)rc);
            nic_write(NIC_PG0_RBCR1, (UCHAR)((USHORT)rc >> 8));
            nic_write(NIC_PG0_RSAR0, sizeof(struct NICPKYTHEADER));
            nic_write(NIC_PG0_RSAR1, bnry);
            
            /* Perform the read. */
            nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD0);
            buf = net_in_data;
            for( i = 0; i < rc; i++ )
                *buf++ = nic_read(NIC_IOPORT);
            
            /* Complete remote dma. */
            nic_write(NIC_CR, NIC_CR_STA | NIC_CR_RD2);
            
            for( i = 0; i <= 20; i++ )
            {
                if(nic_read(NIC_PG0_ISR) & NIC_ISR_RDC)
                    break;
            }
            nic_write(NIC_PG0_ISR, NIC_ISR_RDC);
            iReceiveData = 1;
        }
        
        /* Set boundary register to the last page we read. */
        if( --nextpg < NIC_FIRST_RX_PAGE )
            nextpg = NIC_STOP_PAGE - 1;
        nic_write(NIC_PG0_BNRY, nextpg);
    }
    
    return rc;
}
