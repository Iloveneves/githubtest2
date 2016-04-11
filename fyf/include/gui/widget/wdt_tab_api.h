/*===============================================================================

 File Name:wdt_tab_api.h

 Author:szhy

 Create Time:2007-6-21
 
 Description:tab control

 Class Name:"TAB"

 History:

===============================================================================*/
#ifndef __WDT_TAB_API_H_
#define __WDT_TAB_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_TAB(lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle, unSelectStyle,styleWidth,styleHeight,interval) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle,unSelectStyle,styleWidth,styleHeight,interval,0,0,0,
#define DEF_TAB_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_TAB_SV_DATA			1
#define WDT_TAB_SV_INDEX			2
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_TAB_API_Register(void);
PU08 WDT_TAB_API_GetVersion(void);
#endif

