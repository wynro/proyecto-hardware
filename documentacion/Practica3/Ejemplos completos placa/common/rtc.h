#ifndef __RTC_H__
#define __RTC_H__

int  year;
int  month,day,weekday,hour,min,sec;

int Test_Rtc_Alarm(void);
void Rtc_Init(void);
void Read_Rtc(void);
void Display_Rtc(void);
void Test_Rtc_Tick(void);

void Rtc_Int(void)  __attribute__ ((interrupt ("IRQ")));
void Rtc_Tick(void) __attribute__ ((interrupt ("IRQ")));

#define TESTYEAR 	(0x02)
#define TESTMONTH 	(0x12)
#define TESTDAY		(0x31)
#define TESTDATE        (0x03)  // SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7   
#define TESTHOUR	(0x23)
#define TESTMIN		(0x59)
#define TESTSEC		(0x59)

#define TESTYEAR2	(0x03)
#define TESTMONTH2 	(0x01)
#define TESTDAY2	(0x01)
#define TESTHOUR2	(0x00)
#define TESTMIN2	(0x00)
#define TESTSEC2	(0x00)

#endif //__RTC_H__

