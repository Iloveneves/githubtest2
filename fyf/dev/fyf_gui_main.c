/*
author     :szhy
create date:2008-12-01
*/
//#include <pthread.h>
#include "fyf_def.h"
#include "os/fyf_thread.h"
#include "../../tpmiddle/include/app_common.h"

#define UART_SERIAL_NUM
#define KEY_DETECT_INTERVAL     100

#define FYF_THREAD_PRIORITY_MAINGUI 		(17)
#define FYF_THREAD_STACK_MAINGUI  			(120*1024)

#define FYF_THREAD_PRIORITY_TIMER 			(6)
#define FYF_THREAD_STACK_TIMER  			(10*1024)

#define FYF_THREAD_PRIORITY_SERIAL 			(6)
#define FYF_THREAD_STACK_SERIAL  			(10*1024)

static BU08 isPowerOff = 0;

void MainTask();
void GUI_TIMER(void *pvParam);
void GUI_COMMON(void *pvParam);
//void GUI_CACard(void *pvParam);
static BS32 fyf_uart_semaphore;
static BS32 fyf_power_on_semaphore;
BU08 bGuiTimerSuspend = FYF_FALSE;
void FYF_API_createGUIThread(void)
{
	BU32 TimerTaskid;
	
//	taskid = FYF_API_thread_create("GUI", (void *)&MainTask, FYF_NULL, FYF_THREAD_PRIORITY_MAINGUI, FYF_THREAD_STACK_MAINGUI);	
	TimerTaskid = FYF_API_thread_create("GUI TIMER", (void *)&GUI_TIMER, FYF_NULL, FYF_THREAD_PRIORITY_TIMER, FYF_THREAD_STACK_TIMER);	
#ifdef UART_SERIAL_NUM
	fyf_uart_semaphore = FYF_API_semaphore_create("urt", 1, FYF_THREAD_WAIT_FIFO);
	FYF_API_thread_create("GUI COMMON", GUI_COMMON, FYF_NULL, FYF_THREAD_PRIORITY_SERIAL, FYF_THREAD_STACK_SERIAL);
#endif

	MainTask();
}
BS32 FYF_API_GetPowerOnSemaphore(void)
{
	return fyf_power_on_semaphore;
}

#ifdef UART_SERIAL_NUM

#define FYF_UART_BUF_MAX 256
#define FYF_STBID_NUM 28
static BU32 fyf_uart_len = 0;
BU08 fyf_uart_buf[FYF_STBID_NUM + 3];
extern BU08 serial_detect;
void GUI_COMMON(void *pvParam)
{

	BS32 ret;
	BU32 curState;
	BS32 len;
	
	while(1)
	{
		if(0 == isPowerOff && serial_detect >= 1)
		{
			ret = FYF_API_Uart_Open();
			
			if (ret == FYF_ERR)
			{
				printf("FYF_API_Uart_Open error\n");
				continue;
			}
			memset(fyf_uart_buf, 0, sizeof(fyf_uart_buf));
			if (serial_detect == 2)
				ret = FYF_API_Uart_Read(fyf_uart_buf, FYF_STBID_NUM);
			else
				ret = FYF_API_Uart_Read(fyf_uart_buf, FYF_STBID_NUM);
			if (ret == FYF_ERR)
			{
				printf("FYF_API_Uart_Read error\n");
			}
			else if (ret == 0)
			{
				printf("FYF_API_Uart_Read nothing!\n");
				
			}
			else
			{
				len = strlen(fyf_uart_buf);
				if (len >= 2)
				{
					if (fyf_uart_buf[len - 2] == 0x0d && fyf_uart_buf[len - 1] == 0xa)
					{
						fyf_uart_buf[len - 2] = 0;
		
					}
					else if (fyf_uart_buf[len - 1] == 0x0d)
					{
						fyf_uart_buf[len - 1] = 0;
					}
					printf("fyf_uart_buf = %s %d %d\n", fyf_uart_buf, ret, strlen(fyf_uart_buf));
					UI_APP_ShowSerial(fyf_uart_buf);
				}
			

			}
			FYF_API_Uart_Close();
		}
		FYF_API_thread_sleep(100);

	}
}
#endif

void GUI_TIMER(void *pvParam)
{

	GUI_API_SetTimerTick(KEY_DETECT_INTERVAL);	//GUI定时器初始化

	while(1)
	{
		if(bGuiTimerSuspend == 0)
		{
			GUI_API_TimerProcess();//GUI定时器进程	
		}
		FYF_API_thread_sleep(KEY_DETECT_INTERVAL);	
	}
}



