*******	History, 2005.09.20 R.X.Huang(Huangrx@embedinfo.com) ********

1. add the version 2.76, higher stabilization			
2. modify the includes.h of 2.00, remove the const-defination to Sems.h
3. modify the main.c, print version info to uart

This start-stop watch example use the uC/OS-II V2.76 kernel.The program is a simple
 one-button stopwatch that displays minutes,seconts, and tenths of seconds in 
 the following format:99:59.9

The stop watch has a single button that cycles the watch through three modes:
 CLEAR->COUNT->STOP->CLEAR..

It creates 4 tasks :
(1) LED lights flashing 
(2) Keyboard inputting and 8-SEG LED displaying
(3) LCD displaying
(4) RTC counting and the key"0" control three modes: CLEAR->COUNT->STOP->CLEAR..

