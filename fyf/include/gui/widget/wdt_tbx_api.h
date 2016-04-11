/*===============================================================================

 File Name:wdt_tbx_api.h

 Author:szhy

 Create Time:2007-6-19
 
 Description:textbox control

 Class Name:"TBX"

 History:

===============================================================================*/
#ifndef __WDT_TBX_API_H_
#define __WDT_TBX_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_TBX(lpClassName,x,y,nWidth,nHeight,ID,parentID,focusStyle,unFocusStyle, \
	styleWidth,styleHeight,minValue,maxValue,bPassword,maxLength) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,focusStyle,0,unFocusStyle, \
	styleWidth,styleHeight,minValue,maxValue,bPassword,maxLength,
#define DEF_TBX_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_TBX_SV_TYPE
{
	WDT_TBX_SV_PASSWORD,
	WDT_TBX_SV_KEYVALUE,
	WDT_TBX_SV_DATA,
	WDT_TBX_SV_BSTAR,
	WDT_TBX_SV_LABEL,
	WDT_TBX_SV_LABEL_X,
	WDT_TBX_SV_SHOWZERO,	/* 设置数据显示格式,第一个参数传1表示显示0,否则不显示 */
	WDT_TBX_SV_RECALL,		/* 设置数据编辑形式,第一个参数传1代表响应返回键,否则不响应 */
	WDT_TBX_SV_LABEL_FGC,	/* 设置TBX控件label前景色 */
	WDT_TBX_SV_TEXT_FGC		/* 设置TBX控件文本前景色 */
};

typedef struct tagWDT_TBX_Attrib_s
{
	BU32 textColor_Focus;
	BU32 textColor;
	BU32 labelColor_Focus;
	BU32 labelColor;
}WDT_TBX_Attrib_s;

/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_TBX_API_Register(void);
PU08 WDT_TBX_API_GetVersion(void);
void WDT_TBX_API_SetAttrib(WDT_TBX_Attrib_s *pAttrib);

#endif
 
