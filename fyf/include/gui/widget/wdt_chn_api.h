/*===============================================================================

 File Name:wdt_chn_api.h

 Author:QJX

 Create Time:2007-8-15
 
 Description:channel num control

 Class Name:"CHN"

 History:

===============================================================================*/
#ifndef __WDT_CHN_API_H_
#define __WDT_CHN_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_CHN(lpClassName,x,y,nWidth,nHeight,ID,parentID,stylewidth,styleheight) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,stylewidth,styleheight,0,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/		
#define WDT_CHN_SV_DATA      1
#define WDT_CHN_SV_NUM       2
#define WDT_CHN_SV_KEY		 3
#define WDT_CHN_SV_CLR		 4
#define WDT_CHN_SV_DES		 5
#define WDT_CHN_SV_DESSHOW   6
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_CHN_API_Register(void);
PU08 WDT_CHN_API_GetVersion(void);
#endif
 
