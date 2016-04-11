/*
author     :cwd
create date:2009-08-01
*/
#include <sys/time.h>
#include <time.h>

#include "fyf_def.h"
#define __FYF_SYSTEM_PORTING__
#ifdef  __FYF_SYSTEM_PORTING__
static BU32 g_bootstrap_time = 0;
#include "sys/fyf_system.h"
/******************************************************************************/
/*Alloc memory function.													  */
/*Input:  memory size														  */
/*Output&return: memory address pointer if success, other FYF_NULL			  */
/******************************************************************************/


void *FYF_CPI_malloc_porting(BS32 memsize)
{
	return malloc(memsize);
}
/******************************************************************************/
/*Free memory function. 													  */
/*Input:  memory address pointer											  */
/*Output&return: No 														  */
/******************************************************************************/
void FYF_CPI_free_porting(void *memptr)
{
	free(memptr);
}
/******************************************************************************/
/*Description: log output, description and format is same as ANSI			  */ 
/*			   ANSI FYF_API_printf											  */
/*Input 	 : ...															  */
/*Output	 : ...															  */
/*Return	 : ...															  */
/******************************************************************************/
BS32 FYF_CPI_printf_porting(const char* fmt, ...)
{
	printf(fmt);
	return FYF_OK;
}
/* ¼ì²âe2p */
extern BS32 APP_CPI_TestE2p_porting(void);
extern void FYF_CPI_power_on_process_porting(void);
extern void FYF_CPI_power_off_process_porting(void);
extern BU32 FYF_CPI_maincode_get_time_porting(BU08* time,BU32 len);
extern BS32	 FactoryCheck_CPI_Uart_porting(void);
/******************************************************************************/
/*Description: fyf Runtime Base time.										  */
/*			   and Unit is millisecond (1/1000 second). 					  */
/*Input 	 : No															  */
/*Output	 : No															  */
/*Return	 : counts of millisecond.										  */
/******************************************************************************/
static BU32 g_last_time = 0;
BU32 FYF_CPI_time_ms_porting(void)
{
	BU32 u32Time = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    u32Time = (tv.tv_sec - g_bootstrap_time) * 1000  + tv.tv_usec / 1000;  
	if (g_last_time <= u32Time)
	{
		g_last_time = u32Time;
	}
	else
	{
		printf("FYF_CPI_time_ms_porting error %d %d\n", g_last_time, u32Time);
	}
    return u32Time;
}

BU32 FYF_CPI_maincode_get_time_porting(BU08* time,BU32 len)
{
	BU08 year[5], month[4], day[3];
	BU08 date[20];
	BU08 m;
	strcpy(date, __DATE__);
	memcpy(month, date, 3);
	month[3] = 0;
	memcpy(day, date + 4, 2);
	day[2] = 0;
	if (day[0] == ' ')day[0] = '0';
	memcpy(year, date + 7, 4);
	year[4] = 0;
	printf("month = %s %s %s\n", month, day, year);
	if (strcmp(month, "Jan") == 0)
	{
		m = 1;
	}
	else if (strcmp(month, "Feb") == 0)
	{
		m = 2;
	}
	else if (strcmp(month, "Mar") == 0)
	{
		m = 3;
	}
	else if (strcmp(month, "Apr") == 0)
	{
		m = 4;
	}
	else if (strcmp(month, "May") == 0)
	{
		m = 5;
	}
	else if (strcmp(month, "Jun") == 0)
	{
		m = 6;
	}
	else if (strcmp(month, "Jul") == 0)
	{
		m = 7;
	}
	else if (strcmp(month, "Aug") == 0)
	{
		m = 8;
	}
	else if (strcmp(month, "Sep") == 0)
	{
		m = 9;
	}
	else if (strcmp(month, "Oct") == 0)
	{
		m = 10;
	}
	else if (strcmp(month, "Nov") == 0)
	{
		m = 11;
	}
	else if (strcmp(month, "Dec") == 0)
	{
		m = 12;
	}
	sprintf(time, "%s-%d-%s %s\n", year, m, day, __TIME__);
	
}
static FYF_SYSTEM_PORTING FYF_SYSTEM_porting=
{
	FYF_CPI_malloc_porting,
	FYF_CPI_free_porting,
	0,
	FYF_CPI_time_ms_porting,
	0,//APP_CPI_TestE2p_porting,
	0,//FYF_CPI_power_on_process_porting,
	0,//FYF_CPI_power_off_process_porting,
	FYF_CPI_maincode_get_time_porting,
	0,//FactoryCheck_CPI_Uart_porting
};

#endif

#define __FYF_SYS_PORTING__
#ifdef  __FYF_SYS_PORTING__
void FYF_API_sys_register(void)
{
#ifdef __FYF_SYSTEM_PORTING__
	FYF_API_system_register(&FYF_SYSTEM_porting);
    struct timeval tv;
    gettimeofday(&tv, NULL);
	g_bootstrap_time = tv.tv_sec;
	printf("fyf<<<>>>system register success!\n");
#else
	printf("fyf<<<>>>system register fail!\n");
#endif
}
#endif



