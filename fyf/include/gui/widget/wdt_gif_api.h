/*===============================================================================

 File Name:wdt_gif_api.h

 Author:QJX

 Create Time:2007-8-15
 
 Description:gif control

 Class Name:"gif"

 History:

===============================================================================*/
#ifndef __WDT_GIF_API_H_
#define __WDT_GIF_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_GIF(lpClassName,x,y,nWidth,nHeight,ID,parentID,TimeInterval) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,TimeInterval,0,0,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/		
#define WDT_GIF_SV_DATA      1

/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_GIF_API_Register(void);
PU08 WDT_GIF_API_GetVersion(void);
#endif
 
