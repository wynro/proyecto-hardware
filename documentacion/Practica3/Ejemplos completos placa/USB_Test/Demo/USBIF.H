//usbif.h
#ifndef _USBIF_H_
#define _USBIF_H_

#include "devioctl.h"

#define USC unsigned char
#define FALSE 0
#define TRUE 1

//定义都是针对主控制器的
#define hReadPipe1   0		//endpoint 1, IN
#define hWritePipe1  1		//endpoint 2, OUT

#define hReadPipe2   2		//endpoint 3, IN
#define hWritePipe2  3		//endpoint 4, OUT

#define hReadPipe3   4		//endpoint 5, IN
#define hWritePipe3  5		//endpoint 6, OUT

#define kMaxNumInstances 999

typedef struct _BULK_TRANSFER_CONTROL
{
   ULONG pipeNum;
} BULK_TRANSFER_CONTROL, *PBULK_TRANSFER_CONTROL;


#define Ezusb_IOCTL_INDEX  0x0800

//
// Perform an IN transfer over the specified bulk or interrupt pipe.
//
// lpInBuffer: BULK_TRANSFER_CONTROL stucture specifying the pipe number to read from
// nInBufferSize: sizeof(BULK_TRANSFER_CONTROL)
// lpOutBuffer: Buffer to hold data read from the device.  
// nOutputBufferSize: size of lpOutBuffer.  This parameter determines
//    the size of the USB transfer.
// lpBytesReturned: actual number of bytes read
// 
#define IOCTL_EVK_BULK_READ             CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Ezusb_IOCTL_INDEX+19,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

//
// Perform an OUT transfer over the specified bulk or interrupt pipe.
//
// lpInBuffer: BULK_TRANSFER_CONTROL stucture specifying the pipe number to write to
// nInBufferSize: sizeof(BULK_TRANSFER_CONTROL)
// lpOutBuffer: Buffer of data to write to the device
// nOutputBufferSize: size of lpOutBuffer.  This parameter determines
//    the size of the USB transfer.
// lpBytesReturned: actual number of bytes written
// 
#define IOCTL_EVK_BULK_WRITE            CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Ezusb_IOCTL_INDEX+20,\
                                                   METHOD_IN_DIRECT,  \
                                                   FILE_ANY_ACCESS)




//
// Returns driver version information
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PEZUSB_DRIVER_VERSION
// nOutputBufferSize: sizeof(EZUSB_DRIVER_VERSION)
// 
#define IOCTL_EZUSB_GET_DRIVER_VERSION   CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Ezusb_IOCTL_INDEX+29,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)




///


//底下3个函数是最最重要的，其他函数都建立在这个基础上的。
//******************************************************************
//function: hOpenDevice
//parameter: HANDLE *hDevcieHandle  ----保存打开的设备句柄
//purpose : 对设备任何的操作之前都要调用这个函数，来开启设备
//return  : 成功， 返回TRUE;  失败，返回FALSE         
//*******************************************************************  
int  hOpenDevice( HANDLE *hDeviceHandle );

//************************************************************************
//function： hCloseDevice
//parameter：HANDLE *hDeviceHandle  ------hOpenDevice 打开的设备句柄
//purpose :  读写等操作完以后，要调用此函数来关闭
//           由hOpenDevice 函数打开的设备
//return：   关闭成功，返回 TRUE, 失败，返回FALSE
//************************************************************************
 int  hCloseDevice( HANDLE *hDeviceHandle );

//*************************************************************
//function：hUSBIO
//purpose: 读取 或发送数据.这里采用的是这样一种策略，读写超过６４字节，
//         那么分成６４大小的块。
//return： 成功传输，返回TRUE; 失败， 返FALSE.
//**************************************************************
int  hUSBIO(HANDLE *hDeviceHandle,
			USC* hDataBuffer,
			int hDataLength,	
			int hPipeNumber,
			int hReadWrite);         //TRUE means Read


#endif _USBIF_H_  //endof 