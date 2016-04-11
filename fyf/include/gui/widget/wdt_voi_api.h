/*===============================================================================

 File Name:wdt_voi_api.h

 Author:QJX

 Create Time:2007-6-19
 
 Description:voice control

 Class Name:"VOI"

 History:

===============================================================================*/
#ifndef __WDT_VOI_API_H_
#define __WDT_VOI_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_VOI(lpClassName,x,y,nWidth,nHeight,ID,parentID,AddStyle, \
	ReduceStyle,BKStyle,BlockWidth,VoiceMax,BlockStart_X,BlockStart_Y,Text_X,Text_Y) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,AddStyle,ReduceStyle, \
	BKStyle,BlockWidth,VoiceMax,BlockStart_X,BlockStart_Y,Text_X,Text_Y,
#define DEF_VOI_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_VOI_SV_LEFT        	1//音量减小
#define WDT_VOI_SV_RIGHT        2//音量增大
#define WDT_VOI_SV_COLOR        3//音量文本背景颜色
#define WDT_VOI_SV_INDEX        4//获得音量索引值
#define WDT_VOI_SV_VISIBLE		5//
#define WDT_VOI_SV_KEY          6//
#define WDT_VOI_SV_FONTCOLOR    7//字体前景色

#define WDT_VOI_SV_BK_STYLE		8//设置音量文本的背景样式
#define WDT_VOI_SV_BK_STYLE_POS 9//设置音量文本的背景样式的位置
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_VOI_API_Register(void);
PU08 WDT_VOI_API_GetVersion(void);
#endif
 
