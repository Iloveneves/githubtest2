/*===============================================================================

 File Name:wdt_pbr_api.h

 Author:QJX

 Create Time:2007-8-21
 
 Description:progress bar control

 Class Name:"PBR"

 History:

===============================================================================*/
#ifndef __WDT_PBR_API_H_
#define __WDT_PBR_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_PBR(lpClassName,x,y,nWidth,nHeight,ID,parentID,BarColor,BkColor,BkWidth,BkHeight) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,BarColor,BkColor,0,BkWidth,BkHeight,0,0,0,0,
#define DEF_PBR_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_PBR_SV_CHANGE			1
#define WDT_PBR_SV_COLOR            2
#define WDT_PBR_SV_DATESTR          3
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_PBR_API_Register(void);
PU08 WDT_PBR_API_GetVersion(void);
#endif
 
