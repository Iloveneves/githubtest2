/*===============================================================================

 File Name:wdt_lbx_api.h

 Author:szhy

 Create Time:2007-4-22
 
 Description:listbox control

 Class Name:"LBX"

 History:

===============================================================================*/
#ifndef __WDT_LBX_API_H_
#define __WDT_LBX_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_LBX(lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle,unSelectStyle,styleWidth,styleHeight,numInPage,scrollbarID,interval) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle,selectAndUnFocusStyle, \
	unSelectStyle,styleWidth,styleHeight,numInPage,scrollbarID,interval,0,
#define DEF_LBX_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_LBX_SV_ADDDATA			1
#define WDT_LBX_SV_INDEX				2
#define WDT_LBX_SV_ALLNUM			3
#define WDT_LBX_SV_NUMINPAGE			4
#define WDT_LBX_SV_GETDATA			5
#define WDT_LBX_SV_DATA				6
#define WDT_LBX_SV_BKSTYLE			7
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_LBX_API_Register(void);
PU08 WDT_LBX_API_GetVersion(void);
#endif

