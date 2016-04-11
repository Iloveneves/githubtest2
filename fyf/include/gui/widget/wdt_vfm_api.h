/*===============================================================================

 File Name:wdt_vfm_api.h

 Author:QJX

 Create Time:2007-8-27
 
 Description:video frame control

 Class Name:"VFM"

 History:

===============================================================================*/
#ifndef __WDT_VFM_API_H_
#define __WDT_VFM_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_VFM(lpClassName,x,y,nWidth,nHeight,ID,parentID,color) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,color,0,0,0,
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_VFM_API_Register(void);
PU08 WDT_VFM_API_GetVersion(void);
#endif
 
