#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

//系统提示
void UI_APP_SysInfo_Enter(void)
{

}

void UI_APP_SysInfo_End(void)
{
	if(UI_APP_API_GetbSaveFlag())
	{
		UI_APP_API_SetbSaveFlag(0);
		APP_ChnDatabase_API_UpDataToFlash(1);
	}
}
void UI_APP_SysInfo_Timer(void)
{
	BU32 prev_state = GUI_ENC_API_GetPrevState();
	if (prev_state == ID_DLG_MenuLock)
	{
		GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevPrevState(), 1);
	}
	else if(prev_state == ID_DLG_OTA)
	{
	    APP_SYS_API_WirteUpdateFlag();
	}
	else
	{
		GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
	}
}
