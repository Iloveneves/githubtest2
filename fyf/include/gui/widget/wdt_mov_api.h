/*===============================================================================

 File Name:wdt_mov_api.h

 Author:qjx

 Create Time:2007-5-10
 
 Description:moving control

 Class Name:"MOV"

 History:

===============================================================================*/
#ifndef __WDT_MOV_API_H_
#define __WDT_MOV_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_MOV(lpClassName,x,y,nWidth,nHeight,ID,parentID) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,0,0,0,0,
#define DEF_MOV_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_MOV_SV_INDEX			1
#define WDT_MOV_SV_DATA		        2
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_MOV_API_Register(void);
PU08 WDT_MOV_API_GetVersion(void);
#endif
 
