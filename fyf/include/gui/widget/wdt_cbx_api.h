/*===============================================================================

 File Name:wdt_cbx_api.h

 Author:szhy

 Create Time:2007-5-10
 
 Description:combobox control

 Class Name:"CBX"

 History:

===============================================================================*/
#ifndef __WDT_CBX_API_H_
#define __WDT_CBX_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_CBX(lpClassName,x,y,nWidth,nHeight,ID,parentID,focusStyle,unFocusStyle,styleWidth,styleHeight,exdata_x) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,focusStyle,unFocusStyle,0,styleWidth,styleHeight,exdata_x,0,0,0,
#define DEF_CBX_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/

#define	WDT_CBX_ALIGN_LEFT 1
#define	WDT_CBX_ALIGN_RIGHT 2
#define	WDT_CBX_ALIGN_CENTER 3

#define	WDT_CBX_SV_INDEX 1	//设置或获得索引值
#define	WDT_CBX_SV_DATA 2	//设值
#define	WDT_CBX_SV_EXDATA 3 //扩展参数，暂时不用
#define	WDT_CBX_SV_ALIGN 4	//设置文字的显示位置
#define	WDT_CBX_SV_LABEL 5	//设置控件上显示的文本
#define	WDT_CBX_SV_LABEL_X 6//设置控件上文本的位置
#define	WDT_CBX_SV_FGC 7			/* 设置组合框文本的前景色 */
#define	WDT_CBX_SV_BGC 8			/* 设置组合框文本的背景色 */
#define WDT_CBX_SV_STRING 9	//设置显示的文字，若要取消设为0
#define WDT_CBX_SV_TEXTRECT 10 //设置文字显示区域 :参数1:文字的X位置;参数2:文字宽度
#define WDT_CBX_SV_SAVESTRINGLENGTH 11 //字符串长度
#define WDT_CBX_SV_CYCLE			12
#define WDT_CBX_SV_LABEL_FGC		13//设置CBX控件label的前景色
#define WDT_CBX_SV_TEXT_FGC			14//设置CBX控件文本的前景色

typedef struct tagWDT_CBX_Attrib_s
{
	BU32 textColor_Focus;
	BU32 textColor;
	BU32 labelColor_Focus;
	BU32 labelColor;
}WDT_CBX_Attrib_s;
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_CBX_API_Register(void);
PU08 WDT_CBX_API_GetVersion(void);
void WDT_CBX_API_SetAttrib(WDT_CBX_Attrib_s *pAttrib);
#endif
 
