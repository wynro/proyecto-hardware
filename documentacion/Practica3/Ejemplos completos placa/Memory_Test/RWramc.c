//////////// RAM ����ʾ��
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
	
	tmpw = *ptr;    // �ֳ���
	*ptr = tmpw+1;  // �ֳ�д
	
	tmph = *ptrh;   // ����
	*ptrh = tmph+1; // ����
	  
	tmpb = *ptrb;  	// �ֽ�
	*ptrb = tmpb+1; // �ֽ�
	
}
