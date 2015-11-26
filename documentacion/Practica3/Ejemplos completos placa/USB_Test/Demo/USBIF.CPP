//usbif.cpp
#include "stdafx.h"
#include "usbif.h"

//implement
int  hOpenDevice( HANDLE *hDeviceHandle )
{
	char	deviceName[ 15 ];
	int		deviceNumber = 0;
	int		hOpen=FALSE;
	int		hVerLength=8;
	int		hReadLength=8;
	char	hLic[]="hugehard\n";	

	while( !hOpen && deviceNumber < kMaxNumInstances )
	{
		//The name specified in the driver is "\\.\HW9911-nnn" where n is the number of the
		//device instance passed to the driver. Note that "\\" is an escape sequence.
        //wsprintf( deviceName, "%s%03d", "\\\\.\\HW9911-", deviceNumber );
        //wsprintf( deviceName, "%s%d", "\\\\.\\Ezusb-0", deviceNumber );
		strcpy(deviceName,"\\\\.\\HHusb-0");   //根据设备名来连接设备
		//Get a handle to the device:
		*hDeviceHandle = CreateFile( deviceName, 
					GENERIC_WRITE | GENERIC_READ, 
					FILE_SHARE_WRITE | FILE_SHARE_READ, 
					NULL,
					OPEN_EXISTING, 
					0, 
					NULL);

		if( *hDeviceHandle != INVALID_HANDLE_VALUE )
			hOpen = TRUE;

		++deviceNumber;
	}
  return	hOpen;
}
//************************************************************************
//function： hCloseDevice
//parameter：HANDLE *hDeviceHandle  ------hOpenDevice 打开的设备句柄
//purpose :  读写等操作完以后，要调用此函数来关闭
//           由hOpenDevice 函数打开的设备
//return：   关闭成功，返回 TRUE, 失败，返回FALSE
//************************************************************************
 int  hCloseDevice( HANDLE *hDeviceHandle )
{
	if( !CloseHandle( *hDeviceHandle ) )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	
}

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
			int hReadWrite)          //TRUE means Read
{	DWORD	count;
	int     yushu=0;
	int     trancounts=0;

	int i=0;

	USC *   ptr=NULL;

	int status;
	BULK_TRANSFER_CONTROL bulkControl;

	bulkControl.pipeNum= hPipeNumber;   //注意输入输出端口信息放入了这里

	if( !hReadWrite )	//if write
	{
		if(hDataLength>0)
		{
	       	//分成一次64字节 写。
			
			trancounts=hDataLength/64;         //有trancounts个 64字节块 
			yushu=hDataLength%64;              //余下 yushu 个字节
			
            
			// 先写，trancounts个64字节块
		  
			ptr=&hDataBuffer[0];
		    hDataLength=64;
            for(i=0;i<trancounts;i++)
			{
	     	   if( !(status=DeviceIoControl(*hDeviceHandle,
					                         IOCTL_EVK_BULK_WRITE,
                                             &bulkControl,
                                             sizeof(BULK_TRANSFER_CONTROL),
				                             ptr,
				                           	 hDataLength,
				                         	 &count,
				                         	 NULL )))
			   {		
                 
		      	return FALSE;
			   }
	
                ptr=&hDataBuffer[(i+1)*hDataLength];
			}// if 读取 trancounts个64字节块 结束

            //然后，写剩余的字节
			ptr=&hDataBuffer[i*64];
			hDataLength=yushu;
	        if(yushu==0)
			{
				return TRUE;
			}
			else
			{
				 if( !(status=DeviceIoControl(	*hDeviceHandle,
					                           IOCTL_EVK_BULK_READ,
					                           &bulkControl,
					                           sizeof(BULK_TRANSFER_CONTROL),
					                           ptr,
					                           hDataLength,
					                           &count,
					                           NULL )))
				 {
 
			       return FALSE;
				 }
			}

	

		}    //end of if(hDataLength>0)
	}        //end of write
	else    //read
	{    
		
		if(hDataLength>0)
		{   
			//分成一次64字节 读取。
			
			trancounts=hDataLength/64;         //有trancounts个 64字节块 
			yushu=hDataLength%64;              //余下 yushu 个字节
			
		  //首先，读取 trancounts个64字节块
		  ptr=&hDataBuffer[0];
		  hDataLength=64;

          for(i=0;i<trancounts;i++)
		  {
	     	if( !(status=DeviceIoControl(	*hDeviceHandle,
					IOCTL_EVK_BULK_READ,
                    &bulkControl,
                    sizeof(BULK_TRANSFER_CONTROL),
					ptr,
					hDataLength,
					&count,
					NULL )))
			{		

			return FALSE;
			}
	
            ptr=&hDataBuffer[(i+1)*hDataLength];
		  }// if 读取 trancounts个64字节块 结束

            //然后，读取yushu 个字节
			ptr=&hDataBuffer[i*64];
			hDataLength=yushu;
            
			if(yushu!=0)
			{
			     if( !(status=DeviceIoControl(	*hDeviceHandle,
					                          IOCTL_EVK_BULK_READ,
                                               &bulkControl,
                                               sizeof(BULK_TRANSFER_CONTROL),
					                           ptr,
					                            hDataLength,
					                            &count,
					                            NULL )))
				 {		

			         return FALSE;
				 }
			}

		  
		} //end of if(hDataLength>0)
	}     //end of if read

	return TRUE;

}
