/*===============================================================================

 File Name:wdt_mnu_api.h

 Author:szhy

 Create Time:2007-4-1
 
 Description:menu control

 Class Name:"MNU"

 History:

===============================================================================*/
#ifndef __WDT_MNU_API_H_
#define __WDT_MNU_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_MNU(lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle,unSelectStyle,styleWidth,styleHeight,menuItemX,menuItemY,titleX,titleY) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle,selectAndUnFocusStyle, \
	unSelectStyle,styleWidth,styleHeight,menuItemX,menuItemY,titleX,titleY,
#define DEF_MNU_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_MNU_SV_TYPE
{
	WDT_MNU_SV_CURDATAID,
	WDT_MNU_SV_DATA,
	WDT_MNU_SV_SELECT,
	WDT_MNU_SV_INDEX,
	WDT_MNU_SV_SELECT_TEXT_FGC,		/* 菜单控件当前项的文本前景色 */
	WDT_MNU_SV_UNSELECT_TEXT_FGC,	/* 菜单控件非当前项的文本前景色 */
	WDT_MNU_SV_TITLE_POSITION,		/* 设置菜单标题的坐标 */
	WDT_MNU_SV_TITLE_TEXT_FGC,		/* 设置菜单标题的文本前景色 */
	WDT_MNU_SV_ITEM_INTERVAL,		/* 设置菜单项的间隔 */
	WDT_MNU_SV_TITLE_BGS,			/* 设置菜单标题的背景样式 */
	WDT_MNU_SV_TITLE_BGS_POS,		/* 设置菜单标题的背景样式的坐标 */
	WDT_MNU_SV_BKSTYLE,				/* 设置菜单背景样式 */
	WDT_MNU_SV_BKSTYLE_POS,
	WDT_MNU_SV_ALIGNMENT,			/* 设置菜单对齐方式 */
	WDT_MNU_SV_OFFSET,				/* 设置菜单文本相对于菜单背景的偏移量 */
	WDT_MNU_SV_TITLE_RANGE,			/* 设置菜单标题文本显示范围 */
	WDT_MNU_SV_EXIT_OR_RECALL,		/* 退出键或返回键返回到上一级菜单 */
	WDT_MNU_SV_NUSELECT_STYLE_ARR  /* 设置非选择样式数据 */
};
enum WDT_MUN_ALIGNMENT
{
	WDT_MNU_ALIGNMENT_LEFT,			/* 左对齐 */
	WDT_MNU_ALIGNMENT_CENTER,		/* 居中 */
	WDT_MNU_ALIGNMENT_RIGHT			/* 右对齐 */
};
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_MNU_API_Register(void);
PU08 WDT_MNU_API_GetVersion(void);
#endif

