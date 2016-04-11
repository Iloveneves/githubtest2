/*===============================================================================

 File Name:wdt_edt_api.h

 Author:szhy

 Create Time:2007-8-27
 
 Description:edit control

 Class Name:"EDT"

 History:

===============================================================================*/
#ifndef __WDT_EDT_API_H_
#define __WDT_EDT_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_EDT(lpClassName,x,y,nWidth,nHeight,ID,parentID,maxLength) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,0,0,0,0,0,maxLength,0,0,0,
#define DEF_EDT_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_EDT_SCROLL_STATE	/* 滚动状态 */
{
	WDT_EDT_SCROLL_STATE_PLAY,	/* 开始滚动 */
	WDT_EDT_SCROLL_STATE_STOP	/* 停止滚动 */
};
enum WDT_EDT_SV_TYPE
{
	WDT_EDT_SV_VALUE,			/* 设置文本内容 */
	WDT_EDT_SV_TEXT_COLOR,		/* 设置文本颜色 */
	WDT_EDT_SV_V_SCROLL_POS,    /* 设置垂直滚动位置 */
	WDT_EDT_SV_SCROLL_TYPE,		/* 设置滚动类型 */
	WDT_EDT_SV_H_SCROLL_POS,	/* 设置水平滚动位置 */
	WDT_EDT_SV_SCROLL_STATE,	/* 设置滚动状态 */
	WDT_EDT_SV_SCROLL_CYCLE,	/* 设置滚动周期 */
	WDT_EDT_SV_SCROLL_BGS,		/* 设置滚动的背景色 */
	WDT_EDT_SV_PAGE_INDEX,		/* 设置滚动控件的当前页 */
	WDT_EDT_SV_PAGE_NUM,		/* 获得滚动控件的页数 */
	WDT_EDT_SV_CREATE_TIMER,	/* 创建定时器 */
	WDT_EDT_SV_TEXT_BKCOLOR		/* 设置滚动文本的背景色 */
};
enum WDT_EDT_SCROLL_TYPE
{
	WDT_EDT_SCROLL_TYPE_LEFT_RIGHT,		/* 设置滚动类型为左右 */
	WDT_EDT_SCROLL_TYPE_UP_DOWN			/* 设置滚动类型为上下 */
};
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_EDT_API_Register(void);
PU08 WDT_EDT_API_GetVersion(void);
#endif

