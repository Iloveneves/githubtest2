/*===============================================================================

 File Name:gui_int_api.h

 Author:szhy

 Create Time:2007-7-27
 
 Description:interpreter

 History:

===============================================================================*/
#ifndef __GUI_INT_API_H_
#define __GUI_INT_API_H_
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
/*
	stateTable flag
*/
#define STF_NORMAL				0x00000001
#define STF_PRESTATE			0x00000002
#define STF_SAVEREGION			0x00000004
#define STF_RESET				0x00000008
#define STF_ORIGIN				0X00000010
/*
	stateTable controlID
*/
#define IDC_ALL				0
/*
	focusTable keyValue
*/
#define GUI_VK_NULL				-1
/*
	stateTable and focusTable info
*/
#define IDI_ALL         			-1
/*-------------------------------------------------------------------------------
	Struct Define
-------------------------------------------------------------------------------*/
/*
	OSDTable
*/
typedef struct _tagGUI_OSDTable_s
{
	PU08 lpClassName;
	BU32 x;
	BU32 y;
	BU32 nWidth;
	BU32 nHeight;
	BU32 ID;
	BU32 parentID;
	
	BU32 selectAndFocusStyle;
	BU32 selectAndUnFocusStyle;
	BU32 unSelectStyle;
	BU32 styleWidth;
	BU32 styleHeight;

	BU32 var1;
	BU32 var2;
	BU32 var3;
	BU32 var4;
} GUI_OSDTable_s;
typedef GUI_OSDTable_s * GUI_OSDTable_sp;
/*
	stateTable
*/
typedef struct _tagGUI_StateTable_s
{
	BU32 dialogIDSrc;
	BU32 controlID;
	BU32 info;
	BU32 keyValue;
	BU32 dialogIDDes;
	BU32 flag;
} GUI_StateTable_s;
typedef GUI_StateTable_s * GUI_StateTable_sp;
/*
	focusTable
*/
typedef struct _tagGUI_FocusTable_s
{
	BU32 ScrID;
	BU32 info;
	BU32 keyValue;
	BU32 DesID;
} GUI_FocusTable_s;
typedef GUI_FocusTable_s * GUI_FocusTable_sp;
/*
	eventTable
*/
typedef struct _tagGUI_EventTable_s
{
	BU32 ID;
	void (* lpEnter)(void);
	void (* lpEnd)(void);
	void (* lpOnPaint)(BU32 hdc,BU32 lParam);
	void (* lpOnKey)(BU32 keyValue);

	void (* lpFunc1)(void);
} GUI_EventTable_s;
typedef GUI_EventTable_s * GUI_EventTable_sp;
/*
	resource
*/
typedef struct _tagGUI_Resource_s
{
	void * styleDataAddr;
	void * styleTableAddr;
	void * dictionaryDataAddr;
	void * keyTable;
	BU32 keyTableNum;
	void * osdTable;
	BU32 osdTableNum;
	void * stateTable;
	BU32 stateTableNum;
	void * focusTable;
	BU32 focusTableNum;
	void * eventTable;
	BU32 eventTableNum;
} GUI_Resource_s;
typedef GUI_Resource_s * GUI_Resource_sp;

typedef struct _tagGUI_FocusWidget_s
{
	BU32 info;
	BU32 keyValue;
} GUI_FocusWidget_s;
typedef GUI_FocusWidget_s * GUI_FocusWidget_sp;
/*-------------------------------------------------------------------------------

	Function Declare
-------------------------------------------------------------------------------*/
void GUI_INT_API_RegisterResource(GUI_Resource_sp resource);
BU32 GUI_INT_API_ChangeFocus(GUI_FocusWidget_sp focusWidget,PU32 focusTable,BU32 focusTableNum);
BU32 GUI_INT_API_ChangeState(GUI_FocusWidget_sp focusWidget,PU32 stateTable,BU32 stateTableNum);
PU08 GUI_INT_API_GetVersion(void);
void GUI_INT_API_Encode(PU08 pu08Src, PU08 pu08Des1, PU08 pu08Des2);
void GUI_INT_API_Decode(PU08 pu08Src1, PU08 pu08Src2, PU08 pu08Des);
void GUI_INT_API_RegisterEvent(GUI_EventTable_sp pEventTable);
void GUI_INT_API_State(PU32 * stateAddr , PU32 stateMax, PU32 stateNum,void * para);
void GUI_INT_API_Focus(PU32 * focusAddr, PU32 focusMax, PU32 focusNum,void * para);
void GUI_INT_API_InitState(PU32 * stateAddr , PU32 stateMax, PU32 stateNum);
void GUI_INT_API_InitFocus(PU32 * focusAddr, PU32 focusMax, PU32 focusNum);
#endif

