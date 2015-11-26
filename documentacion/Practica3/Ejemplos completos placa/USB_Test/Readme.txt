 -------------
| DIR & FILES |
 -------------
DEMO           DEMO software project in VC
Driver         USB driver
Model9603      IDE USB test program (click the *.ews to run) for GNU GCC compiler
Model9603s     IDE USB test project for SDT 2.51 compiler
Demo.exe       application
Readme.txt     this file

 -------------
|    USAGE   |
 -------------
1. connect the JTAG 20pin to Embest emulator then connect to PC PCI port;
2. connect the USB jack to PC USB device port;
3. power up the board, and check the board self-test (Flash LED1 and LED2, LCD print infomation...);
4. open USB_Test\Model9603\Model9603.ews in Embest IDE, or open USB_Test\Model9603s\Model9603.ews
   NOTE: please take attention to adding librarys settings: Project->Settings¡­->linker->Add Library Seaching Path.
5. run the application Demo.exe;
6. after 1min (install driver), then input the txt on the left windows, click translate, print on the right windows.

If translation failure, please reinstall the driver for USB.