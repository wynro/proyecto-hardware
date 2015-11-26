/*********************************************************************************************
*File£º		Thumb.c
*Author£º		Gf.XU	2002.5.15
*Description£º		C program of Thumb instruction
*Composing£º		
*Function£º		
*Modify:		
*********************************************************************************************/

extern void arm_function(void);

char arm[22];
char thumb[22];
unsigned long ulTemp = 0;

/* Delay function of Thumb instruction */
static void delayt(int time)
{
	int i, j, k;
	
	k = 0;
	for(i=0; i<time; i++)
	{
		for(j=0; j<10; j++)
			k++;
	}
}

/* Thumb instruction set function example program */
int Thumb_function(void)
{
	int i;
	char * p = "Hello from Thumb World";
	
	ulTemp ++;
	arm_function();
    
    delayt(2);
	
	for(i=0; i<22; i++)
		thumb[i] = (*p++);

}
