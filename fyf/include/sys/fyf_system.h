/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/07/04                                           	  */
/******************************************************************************/
#ifndef __FYF_SYSTEM_H_
#define __FYF_SYSTEM_H_
typedef struct
{
	void* (*FYF_CPI_malloc)(BS32 memsize);
	void (*FYF_CPI_free)(void *ptr);
	BS32 (*FYF_CPI_printf)(const char* fmt,...);
	BU32 (*FYF_CPI_time_ms)(void);
	BS32 (*APP_CPI_TestE2p)(void);
	void (*FYF_CPI_power_on_process)(void);
	void (*FYF_CPI_power_off_process)(void);
	BU32 (*FYF_CPI_maincode_get_time)(BU08* time,BU32 len);
	BS32 (*FactoryCheck_CPI_Uart)(void);
}FYF_SYSTEM_PORTING; 
typedef struct _TDT_Content_s
{
	BU32 week;
	BU32 hour;
	BU32 min;
    BU32 sec;
    BU32 year;
    BU32 month;
    BU32 date;
}TDT_Content_s;
typedef TDT_Content_s * TDT_Content_sp;
/******************************************************************************/
/*Description: parse tdt                                                      */
/*Input      : section,tdtContent											  */
/*Output     : no                                                             */
/*Return     : no					                                          */
/******************************************************************************/
BU32 FYF_API_parser_tdt(BU08 * section, TDT_Content_s *tdtContents);
/******************************************************************************/
/*Description: register sytem                                                 */
/*Input      : porting														  */
/*Output     : no                                                             */
/*Return     : no					                                          */
/******************************************************************************/
void FYF_API_system_register(FYF_SYSTEM_PORTING * porting);
/******************************************************************************/
/*Alloc memory function.                                                      */
/*Input:  memory size                                                         */
/*Output&return: memory address pointer if success, other FYF_NULL            */
/******************************************************************************/
void *FYF_API_malloc(BS32 memsize);
/******************************************************************************/
/*Free memory function.                                                       */
/*Input:  memory address pointer                                              */
/*Output&return: No                                                           */
/******************************************************************************/
void FYF_API_free(void *memptr);
#if 0
/******************************************************************************/
/*Description: log output, description and format is same as ANSI             */ 
/*			   ANSI FYF_API_printf  										  */
/*Input      : ...                                                            */
/*Output     : ...                                                            */
/*Return     : ...                                                            */
/******************************************************************************/
BS32 FYF_API_printf(const char* fmt, ...); 
#endif
/******************************************************************************/
/*Description: fyf Runtime Base time.			                              */
/*             and Unit is millisecond (1/1000 second).                       */
/*Input      : No                                                             */
/*Output     : No                                                             */
/*Return     : counts of millisecond.                                         */
/******************************************************************************/
BU32 FYF_API_time_ms(void);
/******************************************************************************/
/*Description: fyf Runtime Base time.			                              */
/*             and Unit is millisecond (1/1000 second).                       */
/*Input      : No                                                             */
/*Output     : No                                                             */
/*Return     : counts of millisecond.                                         */
/******************************************************************************/
BS32 APP_API_TestE2p(void);
/******************************************************************************/
/*Description: fyf Runtime Base time.			                              */
/*             and Unit is millisecond (1/1000 second).                       */
/*Input      : No                                                             */
/*Output     : No                                                             */
/*Return     : counts of millisecond.                                         */
/******************************************************************************/
void FYF_API_power_on_process(void);
/******************************************************************************/
/*Description: fyf Runtime Base time.			                              */
/*             and Unit is millisecond (1/1000 second).                       */
/*Input      : No                                                             */
/*Output     : No                                                             */
/*Return     : counts of millisecond.                                         */
/******************************************************************************/
void FYF_API_power_off_process(void);
/******************************************************************************/
/*Description: fyf Runtime Base time.			                              */
/*             and Unit is millisecond (1/1000 second).                       */
/*Input      : No                                                             */
/*Output     : No                                                             */
/*Return     : counts of millisecond.                                         */
/******************************************************************************/
BU32 FYF_API_maincode_get_time(BU08* time,BU32 len);
/******************************************************************************/
/*Description: fyf Runtime Base time.			                              */
/*             and Unit is millisecond (1/1000 second).                       */
/*Input      : No                                                             */
/*Output     : No                                                             */
/*Return     : counts of millisecond.                                         */
/******************************************************************************/
BS32	 FactoryCheck_Uart(void);
#endif

