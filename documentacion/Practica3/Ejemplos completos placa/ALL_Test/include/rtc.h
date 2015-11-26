#ifndef __RTC_H__
#define __RTC_H__

char RTC_ok;
int  year;
int  month,day,weekday,hour,min,sec;
char *date[8];// = {"","SUN","MON","TUE","WED","THU","FRI","SAT"};

void Display_Rtc(void);
void Rtc_Init(void);
void Rtc_Tick(void);
void Rtc_Int(void);
void Read_Rtc(void);
int  Test_Rtc_Alarm(void);
void Test_Rtc_Tick(void);

#define TESTYEAR 	(0x03)
#define TESTMONTH 	(0x11)
#define TESTDAY		(0x20)
#define TESTDATE        (0x05)  // SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7   
#define TESTHOUR	(0x20)
#define TESTMIN		(0x08)
#define TESTSEC		(0x59)

#define TESTYEAR2	(0x00)
#define TESTMONTH2 	(0x01)
#define TESTDAY2	(0x01)
#define TESTDATE2	(0x07)  // SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7   
#define TESTHOUR2	(0x00)
#define TESTMIN2	(0x00)
#define TESTSEC2	(0x00)

#endif //__RTC_H__
