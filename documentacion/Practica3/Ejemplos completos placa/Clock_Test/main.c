
int Main()
{
	char input_char;				/* user input char 			*/

    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
	
    while(1)
     {
	   Uart_Printf("\n > Embest S3CEV40 board. < ");
       Uart_Printf("\n <<PLL configuration>> Test. Y/y to continue,any key skip it.\n");
       input_char = Uart_Getch();
       if(input_char == 'Y' || input_char == 'y')
          Test_PLL();
     }

    return 1;
}