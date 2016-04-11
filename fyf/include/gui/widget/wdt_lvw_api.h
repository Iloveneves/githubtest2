/*===============================================================================

 File Name:wdt_lvw_api.h

 Author:szhy

 Create Time:2007-7-17
 
 Description:listview control

 Class Name:"LVW"

 History:

===============================================================================*/
#ifndef __WDT_LVW_API_H_
#define __WDT_LVW_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_LVW(lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle,unSelectStyle,styleWidth,styleHeight,numInPage,scrollbarID,interval) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle,selectAndUnFocusStyle, \
	unSelectStyle,styleWidth,styleHeight,numInPage,scrollbarID,interval,0,
#define DEF_LVW_EVE(ID,lpOnKey) ID,0,0,0,lpOnKey,0,
/*-------------------------------------------------------------------------------
	Struct Define
-------------------------------------------------------------------------------*/
typedef struct _tagGUI_ListviewAttribute_s
{
	BU32 selectStyle;
	BU32 unSelectStyle;
	BU32 styleX;
	BU32 styleY;
	BU32 keyValue;
	PU08 itemAttrib;
} GUI_ListviewAttribute_s;
typedef GUI_ListviewAttribute_s * GUI_ListviewAttribute_sp;
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define WDT_LVW_SV_ITEMATTRIB			1
#define WDT_LVW_SV_DATA					2
#define WDT_LVW_SV_ATTRIB			3
#define WDT_LVW_SV_INDEX				4
#define WDT_LVW_SV_MOVEATTRIB			5
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_LVW_API_Register(void);
PU08 WDT_LVW_API_GetVersion(void);
#endif

