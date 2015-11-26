#define RWNum  100
#define RWBase 0x0c030000
/*--- function declare ---*/
void Test_MEM(void);
void main(void);

/*--- extern function ---*/
extern void sRWramtest(void);
/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Main(void)
{
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */

	Test_MEM();
    Uart_Printf("\n Press any key to exit Memory Test.\n"); 
    Uart_Getch();

	__asm("mov pc,#0"); // return;  
}                       

void Test_MEM(void)
{    
    int i,step; 
    volatile char input_char;

    Uart_Printf(
   "\n   ================= Memory Read/Write Access Test. ================= \n");

    Uart_Printf("\n Memory Read/Write(ASM code) Test. \n"); 
    sRWramtest();
    Uart_Printf("\n Press any key to continue... \n"); 
    Uart_Getch();
    
    step=sizeof(int);            // Access by Word. 
    for(i=0;i<RWNum/step;i++)          
     {
     	(*(int *)(RWBase +i*step))         = 0xAA55AA55;  
    	(*(int *)(RWBase +RWNum+i*step))   = (*(int *)(RWBase +i*step));  
     }
    Uart_Printf(" Memory Read/Write(C code =>Word) Test. \n"); 
    Uart_Printf("  Base Address is: %x\n",RWBase); 
    Uart_Printf("  Memory Units is: %x\n",RWNum); 
    Uart_Printf("  Access Memory Times is: %d\n",i); 
    Uart_Printf("\n Press any key to continue... \n"); 
    Uart_Getch();

    step=sizeof(short);          // Access by half Word. 
    for(i=0;i<RWNum/step;i++)
     {
     	(*(short *)(RWBase +i*step)) = 0xFF00;      
    	(*(short *)(RWBase +RWNum+i*step))   = (*(short *)(RWBase +i*step));  
     }
    Uart_Printf(" Memory Read/Write(C code =>halfWord) Test. \n"); 
    Uart_Printf("  Base Address is: %x\n",RWBase); 
    Uart_Printf("  Memory Units is: %x\n",RWNum); 
    Uart_Printf("  Access Memory Times is: %d\n",i); 
    Uart_Printf("\n Press any key to continue... \n"); 
    Uart_Getch();

    step=sizeof(char);           // Access by Byte.
    for(i=0;i<RWNum/step;i++)  
     {
    	(*(char *)(RWBase +i*step))  = 0xBB;        
    	(*(char *)(RWBase +RWNum+i*step))   = (*(char *)(RWBase +i*step));  
     }
    Uart_Printf(" Memory Read/Write(C code =>Byte) Test. \n"); 
    Uart_Printf("  Base Address is: %x\n",RWBase); 
    Uart_Printf("  Memory Units is: %x\n",RWNum); 
    Uart_Printf("  Access Memory Times is: %d\n",i); 
    Uart_Printf("\n Press any key to continue... \n"); 
    Uart_Getch();
    
    Uart_Printf(" Memory Test Success! \n"); 

      Uart_Printf("\n << CACHE >> Test. Y/y to continue,any key skip it.\n");
      input_char = Uart_Getch();
      if(input_char == 'Y' || input_char == 'y')
         Test_CACHE();                                           
}                                              
                                               
