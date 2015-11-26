
#include "def.h"

extern unsigned long server_ip;
extern unsigned long local_ip;

void Dhcp_Test()
{
	NicInit();
	NetInit();
	DhcpQuery();
	Uart_Printf("\n\rReceive DHCP Message from server %d.%d.%d.%d",(char)(server_ip>>0),(char)(server_ip>>8),(char)(server_ip>>16),(char)(server_ip>>24));
	Uart_Printf("\n\rConfig local ip address %d.%d.%d.%d\n",(char)(local_ip>>0),(char)(local_ip>>8),(char)(local_ip>>16),(char)(local_ip>>24));	
}
