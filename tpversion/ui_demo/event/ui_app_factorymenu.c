#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

BU32 serial_flag = 0;
BU08 serial_num[30];
BU08 serial_detect = 0;
void UI_APP_ShowSerial(char * sn)
{
	BU32 curState = GUI_ENC_API_GetCurState();
	memset(serial_num, 0, sizeof(serial_num));
	GUI_API_Strcpy(serial_num, sn);
	if (curState == ID_DLG_FactoryMenu ||  curState == ID_DLG_StbIDSetting)
		serial_flag = 1;
}

void UI_APP_FactoryMenu_Enter(void)
{
    BU32 sn;
	serial_detect = 2;
	GUI_ENC_API_SetFocus(ID_CBX_LedDetect_Factory);
	GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_CBX_E2p_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sn);
	printf("sn = %d\n", sn);
	GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory, WDT_TBX_SV_DATA, sn, 0);
}

void UI_APP_FactoryMenu_End(void)
{
	serial_detect = 0;
}

void UI_APP_FactoryMenu_CBXKey(BU32 key)
{
	BU32 chnNum;
	BU32 sn;
	switch(key)
	{
	case GUI_VK_UP:
		if (GUI_ENC_API_IsFocus(ID_CBX_LedDetect_Factory))
		{
			GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_TBX_InputSN_Factory);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_E2p_Factory))
		{
			GUI_ENC_API_SetValue(ID_CBX_E2p_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_LedDetect_Factory);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_ProResume_Factory))
		{
			GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_E2p_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_E2p_Factory);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_ProResume_Factory);
		}
		break;
	case GUI_VK_DOWN:
		if (GUI_ENC_API_IsFocus(ID_CBX_LedDetect_Factory))
		{
			GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_E2p_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_E2p_Factory);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_E2p_Factory))
		{
			GUI_ENC_API_SetValue(ID_CBX_E2p_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_ProResume_Factory);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_ProResume_Factory))
		{
			GUI_ENC_API_SetValue(ID_CBX_ProResume_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_TBX_InputSN_Factory);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_LedDetect_Factory,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_LedDetect_Factory);
		}
		break;
	case GUI_VK_SELECT:
		if (GUI_ENC_API_IsFocus(ID_CBX_LedDetect_Factory))
		{
			APP_Com_API_LedDetect();
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_ProResume_Factory))
		{
			UI_APP_FactoryDefault_SetPrevState(ID_DLG_FactoryMenu);
			GUI_ENC_API_SwitchState(ID_DLG_Default, 1);
		}
		break;
	case GUI_VK_EXIT:
	    GUI_ENC_API_GetValue(ID_TBX_InputSN_Factory,WDT_TBX_SV_DATA,&sn,0);
	    APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_SN,sn);
		APP_SYS_IPI_ReadStbInfo();
	    APP_SYS_API_SaveSysInfo();
		chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
		if(chnNum == 0)
		{
			APP_Com_API_ShowLED("C000");
		}
		else
		{
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//²¥·Å½ÚÄ¿
		}
		break;
	}
	
}

void UI_APP_SnInput_Timer(void)
{
	printf("UI_APP_SnInput_Timer(void)\n");
	BU32 sn = 0;
	BS32 i;
	BU32 len = GUI_API_Strlen(serial_num);
	if (serial_flag == 1)
	{
		for (i = 0; i < len; i++)
		{
			sn = sn * 10 + serial_num[i] - 48;
		}
		GUI_ENC_API_SetValue(ID_TBX_InputSN_Factory,WDT_TBX_SV_DATA ,sn, 0);
		serial_flag = 0;
	}
}