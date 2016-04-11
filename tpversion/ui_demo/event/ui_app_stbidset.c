#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

extern BU32 serial_flag;
extern BU08 serial_num[30];
extern BU08 serial_detect;
BU08 tmpbuf[8][20];

void UI_APP_StbIDSetting_Enter(void)
{
	MAC_BOX_INFO_STRUCT mac_box_info;

	serial_detect = 1;
	APP_SYS_API_GetStbID(&mac_box_info);

	memcpy(tmpbuf[0], mac_box_info.AreaIndicatorCode, 4);
	tmpbuf[0][4] = 0;
	printf("tmpbuf = %s\n", tmpbuf[0]);
	GUI_ENC_API_SetValue(ID_STA_AreaIndicatorCode, WDT_STA_SV_STRING, (BU32)tmpbuf[0], 0);

	memcpy(tmpbuf[1], mac_box_info.ManufactureCode,2);
	tmpbuf[1][2] = 0;
	GUI_ENC_API_SetValue(ID_STA_ManufactureCode, WDT_STA_SV_STRING, (BU32)tmpbuf[1], 0);

	memcpy(tmpbuf[2], mac_box_info.StbFactoryIndicator,3);
	tmpbuf[2][3] = 0;
	GUI_ENC_API_SetValue(ID_STA_StbFactoryIndicator, WDT_STA_SV_STRING, (BU32)tmpbuf[2], 0);

	memcpy(tmpbuf[3], mac_box_info.StbType,2);
	tmpbuf[3][2] = 0;
	GUI_ENC_API_SetValue(ID_STA_StbType, WDT_STA_SV_STRING, (BU32)tmpbuf[3], 0);

	memcpy(tmpbuf[4], mac_box_info.HwVersion,2);
	tmpbuf[4][2] = 0;
	GUI_ENC_API_SetValue(ID_STA_HwVersion, WDT_STA_SV_STRING, (BU32)tmpbuf[4], 0);

	memcpy(tmpbuf[5], mac_box_info.StbIndicatorType,1);
	tmpbuf[5][1] = 0;
	GUI_ENC_API_SetValue(ID_STA_StbIndicatorType, WDT_STA_SV_STRING, (BU32)tmpbuf[5], 0);

	memcpy(tmpbuf[6], mac_box_info.ucMacAddress, 12);
	tmpbuf[6][12] = 0;
	GUI_ENC_API_SetValue(ID_STA_MacAddress, WDT_STA_SV_STRING, (BU32)tmpbuf[6], 0);
	
}

void UI_APP_StbIDSetting_End(void)
{
	//ipanel_update_startpagebuf();
	TPOWVOD_SetStbid();
	serial_detect = 0;
}

void UI_APP_StbIDSetting_Key(BU32 key)
{
	//BU08 tmpbuf[20];
	MAC_BOX_INFO_STRUCT mac_box_info;
	BS32 i;
//	memset(tmpbuf, 0, sizeof(tmpbuf));
	switch(key)
	{
	case GUI_VK_EXIT:
	//	for (i = 0; i < 7; i++)
		//	printf("tmpbuf[i] = %s\n", tmpbuf[i]);
		memcpy(mac_box_info.AreaIndicatorCode, tmpbuf[0], 4);
		
		memcpy(mac_box_info.ManufactureCode, tmpbuf[1], 2);

		memcpy(mac_box_info.StbFactoryIndicator, tmpbuf[2], 3);

		memcpy(mac_box_info.StbType, tmpbuf[3], 2);

		memcpy(mac_box_info.HwVersion, tmpbuf[4], 2);

		memcpy(mac_box_info.StbIndicatorType, tmpbuf[5], 1);
		memcpy(mac_box_info.ucMacAddress, tmpbuf[6], 12);
		mac_box_info.Reserved[0] = 0;
		mac_box_info.Reserved[1] = 0;
		APP_SYS_API_SetStbID(&mac_box_info);
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//²¥·Å½ÚÄ¿
		break;
	}

}

void UI_APP_StbIDSetting_Timer(void)
{
	printf("UI_APP_StbIDSetting_Timer(void)\n");
	BS32 i;
	BU32 len = GUI_API_Strlen(serial_num);
	if (serial_flag == 1)
	{
		if (len != 26)
		{
			printf("input stbid error\n");
		}
		else
		{
			memcpy(tmpbuf[0], serial_num, 4);
			tmpbuf[0][4] = 0;
			GUI_ENC_API_SetValue(ID_STA_AreaIndicatorCode, WDT_STA_SV_STRING, (BU32)tmpbuf[0], 0);

			memcpy(tmpbuf[1], serial_num + 4, 2);
			tmpbuf[1][2] = 0;
			GUI_ENC_API_SetValue(ID_STA_ManufactureCode, WDT_STA_SV_STRING, (BU32)tmpbuf[1], 0);

			memcpy(tmpbuf[2], serial_num + 6, 3);
			tmpbuf[2][3] = 0;
			GUI_ENC_API_SetValue(ID_STA_StbFactoryIndicator, WDT_STA_SV_STRING, (BU32)tmpbuf[2], 0);

			memcpy(tmpbuf[3], serial_num + 9, 2);
			tmpbuf[3][2] = 0;
			GUI_ENC_API_SetValue(ID_STA_StbType, WDT_STA_SV_STRING, (BU32)tmpbuf[3], 0);

			memcpy(tmpbuf[4], serial_num + 11, 2);
			tmpbuf[4][2] = 0;
			GUI_ENC_API_SetValue(ID_STA_HwVersion, WDT_STA_SV_STRING, (BU32)tmpbuf[4], 0);

			memcpy(tmpbuf[5], serial_num + 13, 1);
			tmpbuf[5][1] = 0;
			GUI_ENC_API_SetValue(ID_STA_StbIndicatorType, WDT_STA_SV_STRING, (BU32)tmpbuf[5], 0);

			memcpy(tmpbuf[6], serial_num + 14, 12);
			tmpbuf[6][12] = 0;
			GUI_ENC_API_SetValue(ID_STA_MacAddress, WDT_STA_SV_STRING, (BU32)tmpbuf[6], 0);
		}
		serial_flag = 0;
	}
}