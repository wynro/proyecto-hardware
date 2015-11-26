/*--- include files ---*/
#include "44blib.h"
#include "option.h"
#include "44b.h"

/* ----------------------------------------------------------------------- */
void Test_PLL(void)
{
    int P_div, M_div, S_val, S_div, mck;

    Uart_Printf("<---- Running change test of M/P/S value ---->\n");

    Uart_Printf("Input M vlaue\n");
    M_div=Uart_GetIntNum();        
    Uart_Printf("Input P vlaue\n");
    P_div=Uart_GetIntNum();    
    Uart_Printf("Input S vlaue\n");
    S_div=Uart_GetIntNum();
    S_val=pow(2,S_div);
    mck=(  (M_div+8)*FIN  )/(  (P_div+2)*S_val  );
    
    Uart_Printf("MCLK=%d,M=0x%x,P=0x%x,S=0x%x\n",mck,M_div,P_div,S_div);
    Uart_Printf("Now change PLL value\n");
    Uart_TxEmpty(0);
    
    ChangePllValue(M_div,P_div,S_div);
    Uart_Init(mck,115200);
    Uart_Printf("...I'm running in changed MCLK...\nPress any key to return!\n");

    while(!Uart_GetKey())
    {
	Delay(1000);
	Delay(1000);;
    }

    ChangePllValue(PLL_M,PLL_P,PLL_S);
    Uart_Init(0,115200);
    Uart_Printf("Returned original clock\n");
}
