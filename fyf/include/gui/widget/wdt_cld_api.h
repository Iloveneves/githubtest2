/*===============================================================================

 File Name:wdt_cld_api.h

 Author:QJX

 Create Time:2007-9-2
 
 Description:calendar control

 Class Name:"CLD"

 History:

===============================================================================*/
#ifndef __WDT_CLD_API_H_
#define __WDT_CLD_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_CLD(lpClassName,x,y,nWidth,nHeight,ID,parentID,bkcolor,bkselectcolor) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,bkcolor,bkselectcolor,0,0,
#define DEF_CLD_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_CLD_SV_CURYEAR         1
#define WDT_CLD_SV_CURMONTH        2
#define WDT_CLD_SV_CURDATE         3
#define WDT_CLD_SV_WEEK		 	   4
#define WDT_CLD_SV_MONTH		   5
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_CLD_API_Register(void);
PU08 WDT_CLD_API_GetVersion(void);
#endif
 
