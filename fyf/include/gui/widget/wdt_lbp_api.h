/*===============================================================================

 File Name:wdt_lbp_api.h

 Author:szhy

 Create Time:2007-10-22
 
 Description:listbox page control

 Class Name:"LBP"

 History:

===============================================================================*/
#ifndef __WDT_LBP_API_H_
#define __WDT_LBP_API_H_
/*-------------------------------------------------------------------------------
	Macro Define
-------------------------------------------------------------------------------*/
#define DEF_LBP(lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle, \
	selectAndUnFocusStyle,unSelectStyle,styleWidth,styleHeight,numInPage,scrollbarID,interval,bkStyle) \
	lpClassName,x,y,nWidth,nHeight,ID,parentID,selectAndFocusStyle,selectAndUnFocusStyle, \
	unSelectStyle,styleWidth,styleHeight,numInPage,scrollbarID,interval,bkStyle,
#define DEF_LBP_EVE(ID,lpOnKey,lpOnChange) ID,0,0,0,lpOnKey,lpOnChange,
/*-------------------------------------------------------------------------------
	Struct Define
-------------------------------------------------------------------------------*/
typedef struct _tagGUI_lbpAttribute_s
{
	BU32 selectStyle;
	BU32 unSelectStyle;
	BU32 styleX;
	BU32 styleY;
	BU32 keyValue;
} GUI_lbpAttribute_s;
typedef GUI_lbpAttribute_s * GUI_lbpAttribute_sp;
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
enum WDT_LBP_SV_TYPE
{
	WDT_LBP_SV_INDEX,
	WDT_LBP_SV_NUM,
 	WDT_LBP_SV_NUMINPAGE,
 	WDT_LBP_SV_DATA,
 	WDT_LBP_SV_STYLE,
 	WDT_LBP_SV_STATE,
 	WDT_LBP_SV_KEYVALUE,
 	WDT_LBP_SV_MOVESTYLE,
 	WDT_LBP_SV_DATAX,
 	WDT_LBP_SV_ITEMTYPE,
 	WDT_LBP_SV_COLOR1,
 	WDT_LBP_SV_COLOR2,
 	WDT_LBP_SV_PAINT,
 	WDT_LBP_SV_KEYSTATE,
 	WDT_LBP_SV_MOVE
};
enum WDT_LBP_ITEM_TYPE
{
	WDT_LBP_ITEM_2COLOR,
	WDT_LBP_ITEM_LINE
};
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
void WDT_LBP_API_Register(void);
PU08 WDT_LBP_API_GetVersion(void);
#endif

