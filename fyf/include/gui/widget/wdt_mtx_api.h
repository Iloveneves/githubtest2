/*===============================================================================

 File Name:wdt_mtx_api.h

 Author:QJX

 Create Time:2007-12-15
 
 Description:multi text control

 Class Name:"MTX"

 History:

===============================================================================*/
#ifndef __WDT_MTX_API_H_
#define __WDT_MTX_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_MTX(lpClassName,x,y,nWidth,nHeight,ID,parentID,vspace,linecharacternum,lineamount,backcolor) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,vspace,linecharacternum,lineamount,backcolor,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/		
#define WDT_MTX_SV_DATA      1	//…Ë÷√ ˝æ›
#define WDT_MTX_SV_PAGE_INDEX      2	//“≥

/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_MTX_API_Register(void);
PU08 WDT_MTX_API_GetVersion(void);
#endif
 
