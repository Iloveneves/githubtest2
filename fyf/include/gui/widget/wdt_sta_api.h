/*===============================================================================

 File Name:wdt_sta_api.h

 Author:szhy

 Create Time:2007-8-22
 
 Description:static control

 Class Name:"STA"

 History:

===============================================================================*/
#ifndef __WDT_STA_API_H_
#define __WDT_STA_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_STA(lpClassName,x,y,nWidth,nHeight,ID,parentID,style,dataID,bSaveBK) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,style,dataID,bSaveBK,0,
#define DEF_STA_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/

#define	WDT_STA_ALIGN_LEFT 1	//左对齐
#define	WDT_STA_ALIGN_RIGHT 2	//右对齐
#define	WDT_STA_ALIGN_CENTER 3	//居中



#define	WDT_STA_SV_DATAID 1		//给静态控件设置字典中字的ID
#define	WDT_STA_SV_STYLE 2		//设置样式的ID
#define	WDT_STA_SV_STRING 3		//直接给静态控件设置字符串
#define	WDT_STA_SV_ALIGN 4		//设置文本对齐方式
#define	WDT_STA_SV_FGC 5		/* 设置静态控件文本的前景色 */
#define	WDT_STA_SV_BGC	6		/* 设置静态控件文本的背景色 */
#define WDT_STA_SV_SAVESTRINGLENGTH 7 //字符串长度  
#define WDT_STA_SV_BGS 8		/* 设置背景样式 */
#define WDT_STA_SV_YPOS 9			/* 设置Y坐标 */
 
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_STA_API_Register(void);
PU08 WDT_STA_API_GetVersion(void);
#endif

