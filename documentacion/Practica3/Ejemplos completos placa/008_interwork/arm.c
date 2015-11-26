/*********************************************************************************************
*File£º		arm.c
*Author£º		gf.XU	2002.5.15
*Description£º		C program of ARM instruction
*Composing£º		
*Function£º		
*Modify:		
*********************************************************************************************/
extern char arm[20];

/* Delay function of ARM instruction */
static void delaya(int time)
{
	int i, j, k;
	
	k = 0;
	for(i=0; i<time; i++)
	{
		for(j=0; j<10; j++)
			k++;
	}
}

/* ARM instruction set function example program */
void arm_function(void)
{
	int i;
	int loop;
	unsigned int random;
	char * p = "Hello from ARM world";
	
	for(i=0; i<20; i++)
		arm[i] = (*p++);
		
	delaya(2);

	for( loop = 0; loop < 10; loop++ )
	{ 
		random = randomnumber();
	}
}
