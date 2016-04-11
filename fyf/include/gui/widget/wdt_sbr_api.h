/*===============================================================================

 File Name:wdt_sbr_api.h

 Author:szhy

 Create Time:2007-6-30
 
 Description:scrollbar control

 Class Name:"SBR"

 History:

===============================================================================*/
#ifndef __WDT_SBR_API_H_
#define __WDT_SBR_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_SBR(lpClassName,x,y,nWidth,nHeight,ID,parentID,BKStyle,blockStyle,refurbishStyle, \
	BKStyleWidth,BKStyleHeight,blockWidth,blockHeight,blockStart,blockEnd) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,BKStyle,blockStyle,refurbishStyle, \
	BKStyleWidth,BKStyleHeight,blockWidth,blockHeight,blockStart,blockEnd,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_SBR_SV_TYPE
{
	WDT_SBR_SV_MAX,
	WDT_SBR_SV_MIN,
 	WDT_SBR_SV_VALUE
}; 
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_SBR_API_Register(void);
PU08 WDT_SBR_API_GetVersion(void);
#endif
 
