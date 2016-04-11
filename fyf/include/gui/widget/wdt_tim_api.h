/*===============================================================================

 File Name:wdt_tim_api.h

 Author:szhy

 Create Time:2007-8-14
 
 Description:timer control

 Class Name:"TIM"

 History:

===============================================================================*/
#ifndef __WDT_TIM_API_H_
#define __WDT_TIM_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_TIM(lpClassName,ID,parentID,period,bPlay) \
	lpClassName,0,0,0,0,ID,parentID,STYLE_NULL,STYLE_NULL,STYLE_NULL,0,0,period,bPlay,0,0,
#define DEF_TIM_EVE(ID,lpOnTimer) ID,0,0,0,0,lpOnTimer,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_TIM_SV_PLAY				1
#define WDT_TIM_SV_STOP				2
#define WDT_TIM_SV_PERIOD			3
#define WDT_TIM_SV_REPLAY			4
#define WDT_TIM_SV_CYCLE			5
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_TIM_API_Register(void);
PU08 WDT_TIM_API_GetVersion(void);
#endif

