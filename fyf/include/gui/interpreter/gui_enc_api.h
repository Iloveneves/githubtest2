/*===============================================================================

 File Name:gui_enc_api.h

 Author:szhy

 Create Time:2007-8-23
 
 Description:encapsulation

 History:

===============================================================================*/
#ifndef __GUI_ENC_API_H_
#define __GUI_ENC_API_H_
/*-------------------------------------------------------------------------------
	Constant Define
-------------------------------------------------------------------------------*/
#define GUI_SV_TEXTCOLOR				100
/*-------------------------------------------------------------------------------
	Function Declare
-------------------------------------------------------------------------------*/
/*
	dialog
*/
void GUI_ENC_API_SetState(BU32 dlgID, BU32 bRedraw);
void GUI_ENC_API_KillState(BU32 dlgID);
void GUI_ENC_API_SwitchState(BU32 dlgID, BU32 bRedraw);
BU32 GUI_ENC_API_GetCurState(void);
BU32 GUI_ENC_API_GetPrevState(void);
BU32 GUI_ENC_API_GetPrevPrevState(void);
/*
	widget
*/
void GUI_ENC_API_Enable(BU32 ctrlID, BU32 bEnable);
void GUI_ENC_API_SetVisible(BU32 ctrlID, BU32 state);
/*
	dialog and widget
*/
BU32 GUI_ENC_API_IsFocus(BU32 ID);
void GUI_ENC_API_SetFocus(BU32 ID);
void GUI_ENC_API_Update(BU32 ID, BU32 lParam);
void GUI_ENC_API_GetValue(BU32 ID, BU32 type, BU32 * value, BU32 * para);
void GUI_ENC_API_SetValue(BU32 ID, BU32 type, BU32 value, BU32 para);
void GUI_ENC_API_SetValueUnPaint(BU32 ID, BU32 type, BU32 value, BU32 para);
void GUI_ENC_API_SaveRect(BU32 ID);
void GUI_ENC_API_ReleaseRect(BU32 ID);

#define GUI_KEYSTATE_PRESS		    (1)
#define GUI_KEYSTATE_UP				(0)
BU32 GUI_ENC_API_GetKeyState(void);

#define GUI_CHECKKEYSTATE_PU		(1)
#define GUI_CHECKKEYSTATE_OTHER     (0)
BU32 GUI_ENC_API_CheckKeyState(void);

#define GUI_WND_ID_INVALID			(0xffffffff)
BU32 GUI_ENC_API_PostMessage(BU32 ID,BU32 Msg,BU32 wParam,BU32 lParam);/* 无效ID时,发消息给当前状态 */
typedef BU32 (*GUI_ENC_API_MsgHander)(BU32 ID,BU32 Msg,BU32 wParam,BU32 lParam);
BU32 GUI_ENC_API_RegisterMsgHander(GUI_ENC_API_MsgHander hander);
#endif

