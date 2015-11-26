//////////// RAM ·ÃÎÊÊ¾Àý
#define RWram		(*(unsigned long *)0x0c010200)	
void cRWramtest(void)
{
    unsigned long  * ptr  = 0x0c010200;//RWram;
    unsigned short * ptrh = 0x0c010200;//RWram;
    unsigned char  * ptrb = 0x0c010200;//RWram;

	char i;
	unsigned char  tmpb;
	unsigned short tmph;
	unsigned long  tmpw;
		
	*ptr = 0xAA55AA55; 
	
	tmpw = *ptr;    // ×Ö³¤¶Á
	*ptr = tmpw+1;  // ×Ö³¤Ð´
	
	tmph = *ptrh;   // °ë×Ö
	*ptrh = tmph+1; // °ë×Ö
	  
	tmpb = *ptrb;  	// ×Ö½Ú
	*ptrb = tmpb+1; // ×Ö½Ú
	
}
