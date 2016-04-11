#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

typedef enum
{
	COM_SN_TIME_STATE_NULL,
	COM_SN_TIME_STATE_SMC,
	COM_SN_TIME_STATE_E2,
	COM_SN_TIME_STATE_UART
}COM_SN_TIME_STATE_e;

COM_SN_TIME_STATE_e com_sn_time_state;

BU32 cbx_inOrder[]={RS_E2InOrder};
BU32 cbx_OutOrder[]={RS_OutOrder};
BU32 cbx_NoCard[]={RS_NOInputCard};
BU32 cbx_NoDetect[]={RS_SIError};
BU32 cbx_Testing[]={RS_Testing};

BU32 serial_flag = 0;
BU08 serial_num[30];
BU08 serial_detect = 0;
BU08 serial_show[100];

static BU08 g_eth_index = 0;

void UI_APP_ShowSerial(char * sn)
{
	BU32 curState = GUI_ENC_API_GetCurState();
	memset(serial_num, 0, sizeof(serial_num));
	GUI_API_Strcpy(serial_num, sn);
	if (curState == ID_DLG_ComSequence||  curState == ID_DLG_StbIDSetting)
		serial_flag = 1;
}

void UI_APP_ComSn_Enter(void)
{
	BU32 cardState;

    BU32 hwnd;
	GUI_Rect_s rect;
	serial_detect = 2;
	hwnd = GUI_API_GetDlgItem(ID_VFM_ComSequence);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_ComSequence,ID_STA_Event2_ComSequence,ID_PBR_Epg_ComSequence,260,1);
	com_sn_time_state = COM_SN_TIME_STATE_NULL;

	if(APP_TRUE == APP_CA_API_GetCardIn())
	{
		GUI_ENC_API_SetValue(ID_CBX_DetectCard_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_inOrder, 1);
	}
	else
	{
    		GUI_ENC_API_SetValue(ID_CBX_DetectCard_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_NoCard, 1);
	}
  	GUI_ENC_API_SetValue(ID_CBX_DetectE2_ComSequence,  WDT_CBX_SV_DATA, (BU32)cbx_NoDetect, 1);
  	GUI_ENC_API_SetValue(ID_CBX_DetectCom_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_NoDetect, 1);
	 GUI_ENC_API_SetValue(ID_CBX_EthSet_ComSequence, WDT_CBX_SV_INDEX, g_eth_index, 0);
}

void UI_APP_ComSn_End(void)
{
    serial_detect = 0;
}

void UI_APP_ComSn_Smc_Key(BU32 key)
{
	if(GUI_VK_SELECT == key)
	{
		com_sn_time_state = COM_SN_TIME_STATE_SMC;
		GUI_ENC_API_SetValue(ID_CBX_DetectCard_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_Testing, 1);
	}
	else if(GUI_VK_MENU == key || GUI_VK_RECALL == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
	}
	else if(GUI_VK_EXIT == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}
void UI_APP_ComSn_E2_Key(BU32 key)
{
	if(GUI_VK_SELECT == key)
	{
		com_sn_time_state = COM_SN_TIME_STATE_E2;
		GUI_ENC_API_SetValue(ID_CBX_DetectE2_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_Testing, 1);
	}
	else if(GUI_VK_MENU == key || GUI_VK_RECALL == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
	}
	else if(GUI_VK_EXIT == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}
void UI_APP_ComSn_NetSet_Key(BU32 key)
{
	BU32 value = 0;
	if(GUI_VK_MENU == key || GUI_VK_RECALL == key)
	{
		GUI_ENC_API_GetValue(ID_CBX_EthSet_ComSequence, WDT_CBX_SV_INDEX, &value, 0);
		g_eth_index = value;
		UI_APP_SetEthAttr(value);
	    GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
	}
	else if(GUI_VK_EXIT == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}


void UI_APP_ComSn_Com_Key(BU32 key)
{
	if(GUI_VK_SELECT == key)
	{
		com_sn_time_state = COM_SN_TIME_STATE_UART;
		GUI_ENC_API_SetValue(ID_CBX_DetectCom_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_Testing, 1);
	}
	else if(GUI_VK_MENU == key || GUI_VK_RECALL == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
	}
	else if(GUI_VK_EXIT == key)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}


void UI_APP_ComSn_Timer(void)
{
	BU32 cardState;
	BU32 len;
	BU08 buf[100];
	BU32 factory_num, type, year, week, sn;
	BS32 i;
	BU32 iten[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	MAC_BOX_INFO_STRUCT mac_box_info;
	if(serial_flag)
	{
		len = GUI_API_Strlen (serial_num);
		if (len == 10)
		{
			serial_num[0] = '0';
			sprintf(serial_show,"序列号:%s", serial_num);
			GUI_ENC_API_SetValue(ID_STA_ComInfo_ComSequence, WDT_STA_SV_STRING, (BU32)serial_show, 0);
			factory_num = type = year = week = sn = 0;
	
			for (i = len - 1; i >= 1; i--)
			{
				if(serial_num[i] <= 0x39 && serial_num[i] >= 0x30)
				{
					sn += (serial_num[i] - 0x30) * iten[len - i -1];
					printf("sn = %d\n", sn);
				
				}
				else
					continue;
			}
		/*
			GUI_ENC_API_SetValue(ID_STA_ComInfo_ComSequence, WDT_STA_SV_STRING, (BU32)serial_num, 0);
			factory_num = type = year = week = sn = 0;
			iten = 1;
			for(i = len - 1; i >= 0; i--)
			{
				if(len - i < 8)
				{
					if(serial_num[i] <= 0x39 && serial_num[i] >= 0x30)
					{
						sn += (serial_num[i] - 0x30) * iten;
						iten *= 10;
						if( (i-1) == 7)
							iten = 1;
					}
					else
						break;
				}
				else if (len - i < 10)
				{
					if(serial_num[i] <= 0x39 && serial_num[i] >= 0x30)
					{
						week += (serial_num[i] - 0x30) * iten;
						iten *= 10;
						if( (i-1) == 5)
							iten = 1;
					}
					else
						break;
				}
				else if(len - i < 12)
				{
					if(serial_num[i] <= 0x39 && serial_num[i] >= 0x30)
					{
						year += (serial_num[i] - 0x30) * iten;
						iten *= 10;
						if( (i-1) == 3)
							iten = 1;
					}
					else
						break;
				}
				else if(len - i < 14)
				{
					if(serial_num[i] <= 0x39 && serial_num[i] >= 0x30)
					{
						type += (serial_num[i] - 0x30) * iten;
						iten *= 10;
						if( (i-1) == 1)
							iten = 1;
					}
					else
						break;
				}
				else
				{
					if(serial_num[i] <= 0x39 && serial_num[i] >= 0x30)
					{
						factory_num += (serial_num[i] - 0x30) * iten;
						iten *= 10;
					}
					else
						break;
				}
			}*/
			
			//sn = GUI_API_AddStrDec (serial_num);
			APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_NUM, factory_num);
			APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_STBTYPE, type);
			APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_YEAR, year);
			APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_WEEK, week);
			APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_SN, sn);
			APP_SYS_API_SaveSysInfo();
		}
		else if (len == 26)
		{
			sprintf(serial_show,"STBID:%s", serial_num);
			GUI_ENC_API_SetValue(ID_STA_ComInfo_ComSTBID, WDT_STA_SV_STRING, (BU32)serial_show, 0);
			memcpy(mac_box_info.AreaIndicatorCode, serial_num + 0, 4);
	
			memcpy(mac_box_info.ManufactureCode, serial_num + 4, 2);

			memcpy(mac_box_info.StbFactoryIndicator, serial_num + 6, 3);

			memcpy(mac_box_info.StbType, serial_num + 9, 2);

			memcpy(mac_box_info.HwVersion, serial_num + 11, 2);

			memcpy(mac_box_info.StbIndicatorType, serial_num + 13, 1);
			memcpy(mac_box_info.ucMacAddress, serial_num + 14, 12);
			mac_box_info.Reserved[0] = 0;
			mac_box_info.Reserved[1] = 0;
			APP_SYS_API_SetStbID(&mac_box_info);
			APP_SYS_API_SaveSysInfo();
		}
		else
		{
			sprintf (buf, "请输入10位序列号或者26位stbid", len);
			GUI_ENC_API_SetValue(ID_STA_ComInfo_ComSequence, WDT_STA_SV_STRING, (BU32)buf, 0);
		}
		serial_flag = 0;
	}
	switch(com_sn_time_state)
	{
	case COM_SN_TIME_STATE_NULL:
	//	fyf_uart_read(serial_num, 17, 50);
	//	serial_num[17]=0;
	//	GUI_ENC_API_SetValue(ID_STA_Info_ComSn, WDT_STA_SV_STRING, (BU32)serial_num, 0);
		break;
	case COM_SN_TIME_STATE_SMC:
		if(APP_TRUE == APP_CA_API_GetCardIn())
		{
	    		GUI_ENC_API_SetValue(ID_CBX_DetectCard_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_inOrder, 1);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_CBX_DetectCard_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_NoCard, 1);
		}
		com_sn_time_state = COM_SN_TIME_STATE_NULL;
		break;
	case COM_SN_TIME_STATE_E2:
		//if(FYF_OK == APP_API_TestE2p())
		{
	    	GUI_ENC_API_SetValue(ID_CBX_DetectE2_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_inOrder, 1);
		}
		/*else
		{
			GUI_ENC_API_SetValue(ID_CBX_DetectE2_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_OutOrder, 1);
		}
		*/
		com_sn_time_state = COM_SN_TIME_STATE_NULL;
		break;
	case COM_SN_TIME_STATE_UART:
	//	if(FYF_OK == FactoryCheck_Uart())
		{
	    	GUI_ENC_API_SetValue(ID_CBX_DetectCom_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_inOrder, 1);
		}
		/*else
		{
			GUI_ENC_API_SetValue(ID_CBX_DetectCom_ComSequence, WDT_CBX_SV_DATA, (BU32)cbx_OutOrder, 1);
		}
		*/
		com_sn_time_state = COM_SN_TIME_STATE_NULL;
		break;
	default:
		break;
	}
}


