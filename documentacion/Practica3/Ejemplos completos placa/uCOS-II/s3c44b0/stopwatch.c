void updateTimeTsk(void)

void updateTimeTsk(void)
{

struct watchTime
{
int hms;
int secs;
int mins;
};

watchTime ETime;

int msCntr = 0;

ETime.hms = 0;
ETime.secs = 0;
ETime.mins =0;

OSSemPend(UART_sem, 0, &err);
uHALr_printf("  Task%c Called.\n\n", *(char *)Id);
OSSemPost(UART_sem);


while(1)
{
	OSTimeDly(1);
	if(TimeMode == 1)
	{
		msCntr++;
		if(msCntr == 100)
		{
			msCntr = 0;
			ETime.hms++;
			if(ETime.hms == 10)
			{
				ETime.secs++;
				ETime.hms = 0;
				if(ETime.secs == 60)
				{
					ETime.mins++;
					ETime.secs = 0;
					if(ETime.mins == 999)
					{
						ETime.mins =0;
					}
				}
			}
			
			
			OSSemPend(UART_sem, 0, &err);
          	Uart_Printf(" %03x:%02x:%02x\r",ETime.mins,ETime.secs,ETime.hms);
          	OSSemPost(UART_sem);
			
		}
				
	}
}

}