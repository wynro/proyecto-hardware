
#include "def.h"

extern unsigned long server_ip;
extern unsigned long local_ip;

void Dhcp_Test()
{
	NicInit();
	NetInit();
	Uart_Printf("\n Waiting DHCP server to Respond.\n");
	Uart_Printf("\n Press any key to exit ...\n");
	DhcpQuery();
	Uart_Printf("\nReceive DHCP Message from server %d.%d.%d.%d",
	            (char)(server_ip>>0),
	            (char)(server_ip>>8),
	            (char)(server_ip>>16),
	            (char)(server_ip>>24));
	Uart_Printf("\nConfig local ip address %d.%d.%d.%d\n",
	            (char)(local_ip>>0),
	            (char)(local_ip>>8),
	            (char)(local_ip>>16),
	            (char)(local_ip>>24));	
}

void Tftp_Test()
{
	char* pData;
	unsigned long write_addr;
	char input_string[64];
	char tmp_ip[4] = {0,0,0,0};	
	int  tmp,len,i,j,num=0;
	int  b10 =0; int b100 =0; int flag=0;
	
	NicInit();
	NetInit();
	
	Uart_Printf("\n Do you want to configure local IP ?\n");
	Uart_Printf(" Y/y to configure local IP addr; D/d to use Default IP addr(192.168.0.200).\n");	
	Uart_Printf(" Press any key to continue ...\n");	
	Uart_Printf(" ( %c )",i = Uart_Getch());
	if( i == 'Y' || i == 'y') {
		Uart_Printf(" Please input IP address(xxx.xxx.xxx.xxx) then press ENTER:\n");	


		for( i = 16; i != 0; i--)
	   	   input_string[i] = 0xaa;	   	   
		Uart_GetString(&input_string);
		for( i = 0;((i <16)&(input_string[i] != 0xAA)); i++)
		   if(input_string[i] == '.') num +=1;
		   
		if(num != 3) flag = 1; 
		else
		{
		   num = i - 2; j =0;
		   for( i = num; i >= 0; i--)
		    {
				if(input_string[i] != '.' )
				  {
			         if((input_string[i] < '0' | input_string[i] > '9')) flag = 1;     
				     else
				     {
				       
				       tmp = (input_string[i] - 0x30);
				       if (b100) { tmp *=100; b10 =0; }
				       if (b10)  { tmp *= 10; b100 =1;}
				           
				       b10 = 1;
		       		   if(tmp < 256) tmp_ip[j] += tmp; else local_ip = 0x4dc0c0c0;
		       		  }
		       		   
		       	 }else { j++; b10 =0; b100 =0;}
			}
		 }

		if(!flag)
		 {
	    	Uart_Printf("\nManual Set local ip %d.%d.%d.%d\n",
		            tmp_ip[3],tmp_ip[2],tmp_ip[1],tmp_ip[0]
		            );
		    local_ip = ((tmp_ip[0]<<24))+((tmp_ip[1]<<16))\
		              +((tmp_ip[2]<<8))+tmp_ip[3];

		 }else
		   Uart_Printf("\nIP address error (xxx.xxx.xxx.xxx)!\n");
		   
	}// yes
	else if(i == 'D' || i == 'd') { 
	local_ip = 0xc800a8c0;		// config local ip 192.168.0.200
	
	Uart_Printf("\nDefault Set local ip %d.%d.%d.%d\n",
	            local_ip&0x000000FF,(local_ip&0x0000FF00)>>8,
	            (local_ip&0x00FF0000)>>16,(local_ip&0xFF000000)>>24
	            );
	}
	Uart_Printf("\nPress any key to exit ...\n");	
		
	for( ; ; )
	{
		if( Uart_GetKey() )
			return;
		
		pData = (char *)TftpRecv(&len);
		if( (pData == 0) || (len <= 0) )
			continue;
		
		write_addr = (pData[0])+(pData[1]<<8)+(pData[2]<<16)+(pData[3]<<24);
		pData = pData + sizeof(long);
		
		if( Program(write_addr,pData,len-4) == FALSE )
		{
			continue;
		}
		MakeAnswer();
	}
}