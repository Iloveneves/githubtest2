#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


void UI_APP_ParentControl_Enter(void)
{
    BU32 menulock;
	BU32 chnlock;
	BU32 startuplock;
    APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
    APP_SYS_API_GetSysInfo(APP_SYS_CHN_LOCK_e, &chnlock);
	APP_SYS_API_GetSysInfo(APP_SYS_STARTUP_LOCK_e, &startuplock);
	GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl, WDT_CBX_SV_INDEX, menulock, 0);
	GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl, WDT_CBX_SV_INDEX, chnlock, 0);
	GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl, WDT_CBX_SV_INDEX, startuplock, 0);

	GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_ParentControl,ID_STA_Event2_ParentControl,ID_PBR_Epg_ParentControl,260,1);
}

void UI_APP_ParentControl_End(void)
{
    
}

void UI_APP_ParentControl_Key(BU32 key)
{
    BU32 menulock;
	BU32 chnlock;
	BU32 startuplock;
    switch(key)
    {
    case GUI_VK_UP:
   	    if(GUI_ENC_API_IsFocus(ID_CBX_MenuLockSet_ParentControl))
   	    {
   	         GUI_ENC_API_SetFocus(ID_CBX_BootLockSet_ParentControl);
			 GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_BLACK, 0);
			 GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_WHITE, 0);
   	    }
		else if(GUI_ENC_API_IsFocus(ID_CBX_ChannelLockSet_ParentControl))
		{
		     GUI_ENC_API_SetFocus(ID_CBX_MenuLockSet_ParentControl);
			 GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_BLACK, 0);
			 GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_WHITE, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_BootLockSet_ParentControl))
		{
		     GUI_ENC_API_SetFocus(ID_CBX_ChannelLockSet_ParentControl);
			 GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_BLACK, 0);
			 GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_WHITE, 0);
		}
	    break;
    case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_CBX_MenuLockSet_ParentControl))
   	    {
   	         GUI_ENC_API_SetFocus(ID_CBX_ChannelLockSet_ParentControl);
			 GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_BLACK, 0);
			 GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_WHITE, 0);
   	    }
		else if(GUI_ENC_API_IsFocus(ID_CBX_ChannelLockSet_ParentControl))
		{
		     GUI_ENC_API_SetFocus(ID_CBX_BootLockSet_ParentControl);
			 GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_BLACK, 0);
			 GUI_ENC_API_SetValue(ID_CBX_ChannelLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_WHITE, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_BootLockSet_ParentControl))
		{
		     GUI_ENC_API_SetFocus(ID_CBX_MenuLockSet_ParentControl);
			 GUI_ENC_API_SetValue(ID_CBX_MenuLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_BLACK, 0);
			 GUI_ENC_API_SetValue(ID_CBX_BootLockSet_ParentControl,GUI_SV_TEXTCOLOR,COLOR_WHITE, 0);
		}
	    break;
    case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_CBX_MenuLockSet_ParentControl, WDT_CBX_SV_INDEX, &menulock, 0);
	    GUI_ENC_API_GetValue(ID_CBX_ChannelLockSet_ParentControl, WDT_CBX_SV_INDEX, &chnlock, 0);
	    GUI_ENC_API_GetValue(ID_CBX_BootLockSet_ParentControl, WDT_CBX_SV_INDEX, &startuplock, 0);
		APP_SYS_API_SetSysInfo(APP_SYS_MENU_LOCK_e, menulock);
        APP_SYS_API_SetSysInfo(APP_SYS_CHN_LOCK_e, chnlock);
	    APP_SYS_API_SetSysInfo(APP_SYS_STARTUP_LOCK_e, startuplock);
		APP_SYS_API_SaveSysInfo();//±£´æµ½e2p
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
   	    break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
		break;
    default:
   	    break;
    }
}