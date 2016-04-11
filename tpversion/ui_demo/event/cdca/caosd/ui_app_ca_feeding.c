#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

static BU32 ui_app_feeding_preState = 0;
void UI_APP_CAosd_RequestFeeding(BU32 para1, BU32 para2)
{
#if 0
	if(para1)
	{
		GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_DATAID, RS_RequestOK, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_DATAID, RS_RequestFail, 0);
	}
	UI_APP_CA_SetReqFeed(para1);
	GUI_ENC_API_SwitchState(ID_DLG_RequestFeed,1);
#endif
//	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_REQUEST_FEEDING, para1, 0);
	if(ID_DLG_MasterSlaveManage == GUI_ENC_API_GetCurState()||ID_DLG_MasterSlave == GUI_ENC_API_GetCurState())
	{		
		return;
	}
	if(para1)
	{
		GUI_ENC_API_SetValueUnPaint(ID_STA_Text_Feeding, WDT_STA_SV_DATAID, RS_InsertSonCard, 0);
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint(ID_STA_Text_Feeding, WDT_STA_SV_DATAID, RS_RequestFail, 0);		
	}
	ui_app_feeding_preState = GUI_ENC_API_GetCurState();
	GUI_ENC_API_SwitchState(ID_DLG_Feeding, 1);
}
void UI_APP_Feeding_Timer(void)
{

	GUI_ENC_API_SwitchState(ui_app_feeding_preState, 1);
}
void UI_APP_CAosd_RequestFeedingEx(BU32 para1, BU32 para2)
{
	if(para1)
	{
		UI_OSD_API_ShowPrompt (RS_InsertSonCard, 1);
	}
	else
	{
		UI_OSD_API_ShowPrompt (RS_RequestFail, 1);
	}
}

void UI_APP_CAosd_EndFeeding(void)
{
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_REQUEST_FEEDING_END, 0, 0);
}
void UI_APP_CAosd_EndFeedingEx(void)
{
	if(RS_InsertSonCard == UI_APP_API_GetChnInfoSaveID() || RS_RequestFail == UI_APP_API_GetChnInfoSaveID())
	{
		UI_OSD_API_ShowPrompt (0, 0);
	}
}


