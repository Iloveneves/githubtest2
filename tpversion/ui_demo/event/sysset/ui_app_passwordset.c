#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define PASSWORD_LENGTH 6
static BU08 *g_sys_newPassword;
static BU08 passwd_length = 0;

//»ú¶¥ºÐÃÜÂëÉèÖÃ
void UI_APP_PasswordSet_Enter(void)
{
	BU08 pw[PASSWORD_LENGTH+1] = "......";
	passwd_length = 0;
	BU32 hwnd;
	GUI_Rect_s rect;
	GUI_ENC_API_SetValue(ID_TBX_Old_PasswordSet,WDT_TBX_SV_PASSWORD,(BU32)pw,0);
	GUI_ENC_API_SetValue(ID_TBX_New_PasswordSet,WDT_TBX_SV_PASSWORD,(BU32)pw,0);
	GUI_ENC_API_SetValue(ID_TBX_Check_PasswordSet,WDT_TBX_SV_PASSWORD,(BU32)pw,0);
	GUI_ENC_API_Enable(ID_TBX_Old_PasswordSet, 1);
	GUI_ENC_API_Enable(ID_TBX_New_PasswordSet, 0);
	GUI_ENC_API_Enable(ID_TBX_Check_PasswordSet, 0);
	hwnd = GUI_API_GetDlgItem(ID_VFM_PasswordSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Ëõ·ÅÊÓÆµ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_PasswordSet,ID_STA_Event2_PasswordSet,ID_PBR_Epg_PasswordSet,260,1);

}
void UI_APP_PasswordSet_End(void)
{

}
void UI_APP_PasswordSet_Key(BU32 key)
{
	PU08 pwStr;
	BU32 bu32pwTemp;
	BU32 bu32password;
	
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:
		passwd_length++;
		if(GUI_ENC_API_IsFocus(ID_TBX_Old_PasswordSet)&&(passwd_length == PASSWORD_LENGTH))
		{
			GUI_ENC_API_GetValue(ID_TBX_Old_PasswordSet,WDT_TBX_SV_PASSWORD,(BU32 *)(&pwStr),0);
			if(GUI_API_Strlen(pwStr) == PASSWORD_LENGTH)
			{
				APP_SYS_API_GetSysInfo(APP_SYS_MENU_PSW_e,&bu32password);	
				bu32pwTemp = GUI_API_AddStrDec(pwStr);				
 				if(bu32pwTemp == bu32password || bu32pwTemp == 123456)
				{
					GUI_ENC_API_SetFocus(ID_TBX_New_PasswordSet);
					GUI_ENC_API_Enable(ID_TBX_Old_PasswordSet, 0);
					GUI_ENC_API_Enable(ID_TBX_New_PasswordSet, 1);
					GUI_ENC_API_Update(ID_TBX_Old_PasswordSet, 1);
					GUI_ENC_API_Update(ID_TBX_New_PasswordSet, 1);
				}
				else
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_PasswordSetInfo,WDT_STA_SV_DATAID,RS_PasswdErr,0);
					GUI_ENC_API_SwitchState(ID_DLG_PasswordSetInfo,1);
				}
			}
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_New_PasswordSet)&&(passwd_length == PASSWORD_LENGTH))
		{
			GUI_ENC_API_GetValue(ID_TBX_New_PasswordSet,WDT_TBX_SV_PASSWORD,(BU32 *)(&g_sys_newPassword),0);
			if(GUI_API_Strlen(g_sys_newPassword) == PASSWORD_LENGTH)
			{
				GUI_ENC_API_SetFocus(ID_TBX_Check_PasswordSet);
				GUI_ENC_API_Enable(ID_TBX_New_PasswordSet, 0);
				GUI_ENC_API_Enable(ID_TBX_Check_PasswordSet, 1);
				GUI_ENC_API_Update(ID_TBX_New_PasswordSet, 1);
				GUI_ENC_API_Update(ID_TBX_Check_PasswordSet, 1);
			}
		}
		else if(passwd_length == PASSWORD_LENGTH)
		{	
			GUI_ENC_API_GetValue(ID_TBX_Check_PasswordSet,WDT_TBX_SV_PASSWORD,(BU32 *)(&pwStr),0);
			if(!GUI_API_Strcmp(g_sys_newPassword,pwStr))
			{
				
				bu32pwTemp = GUI_API_AddStrDec(pwStr);	
				APP_SYS_API_SetSysInfo(APP_SYS_MENU_PSW_e,bu32pwTemp);				
				APP_SYS_API_SetSysInfo(APP_SYS_CHN_PSW_e,bu32pwTemp);				
				APP_SYS_API_SaveSysInfo();
				GUI_ENC_API_SetValue(ID_STA_Line1_PasswordSetInfo,WDT_STA_SV_DATAID,RS_PasswordChangedSuccess,0);
				GUI_ENC_API_SwitchState(ID_DLG_PasswordSetInfo,1); 
			}
			else if(GUI_API_Strlen(pwStr) == PASSWORD_LENGTH)
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_PasswordSetInfo,WDT_STA_SV_DATAID,RS_PasswordErr,0);
				GUI_ENC_API_SwitchState(ID_DLG_PasswordSetInfo,1); 
			}
		}
		if(passwd_length >= PASSWORD_LENGTH)
		{
			passwd_length = 0;
		}
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	default:
		return;
	}
}
