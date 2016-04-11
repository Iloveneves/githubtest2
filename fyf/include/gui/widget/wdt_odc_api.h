/*===============================================================================

 File Name:wdt_odc_api.h

 Author:QJX

 Create Time:2007-9-6
 
 Description:on draw control

 Class Name:"ODC"

 History:

===============================================================================*/
#ifndef __WDT_ODC_API_H_
#define __WDT_ODC_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_ODC(lpClassName,x,y,nWidth,nHeight,ID,parentID) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,0,0,0,0,
#define DEF_ODC_EVE(ID,lpOnPaint,lpOnKey) ID,0,0,lpOnPaint,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_ODC_API_Register(void);
PU08 WDT_ODC_API_GetVersion(void);
#endif
 
