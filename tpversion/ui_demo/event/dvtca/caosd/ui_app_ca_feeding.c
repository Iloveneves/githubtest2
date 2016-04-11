#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

void UI_APP_CAosd_RequestFeeding()
{
	if(GUI_ENC_API_GetCurState()==ID_DLG_ChnInfo)
	{
		GUI_ENC_API_SwitchState(ID_DLG_RequestFeed,1);
	}
}

static char *g_ca_masterCnStr[]=
{
	"请插入要配对的母卡",
	"请插入要配对的子卡",
	"请插入要配对的母卡",
	"请插入其它要配对的子卡",
	"请确认插入的是要配对的子卡",
	"配对成功!",
	"读取母卡数据失败!",
	"请注意",
	"喂养失败!",
	"喂养成功!",
	"获取子母卡属性失败",
	"子母卡配对时间已到,拔出此卡,再插母卡",
};
static char *g_ca_masterEnStr[]=
{
	"Insert Mother Card",
	"Insert Child Card",
	"Insert Mother Card",
	"Insert Another Child Card",
	"Please Confirm the Card",
	"Success!",
	"Reading Mother Card Fail!",
	"Attention",
	"Feeding Fail!",
	"Feeding Success!",
	"Get data fail",
	"Insert Mother Card",
};
BU08 g_ui_app_str[13][80];
char **g_ca_master_ppStr;
static BU16 master_slave_tvs[4];
static BU32 master_slave_motherCard;
static FYF_DVTCA_PairInfo_s __glb_cainfo_PairedInfo;
static BU08 g_ca_masterSlave = 0;
/*
子母卡配对

g_ca_masterSlave:
				0	插入母卡
				1	母卡成功，插入子卡

*/

void UI_APP_CA_RequestFeed_Enter(void)
{
	BU32 lang;
	BU32 index = 0;
	BS32 ret;
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		g_ca_master_ppStr = g_ca_masterEnStr;
	}
	else
	{
		g_ca_master_ppStr = g_ca_masterCnStr;
	}

	GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_DATAID, RS_Warning, 0);
	ret = FYF_API_ca_get_value(FYF_CA_SV_MOTHER_INFO,(BU32 *)&master_slave_motherCard,0);

	if(ret == FYF_OK)
	{	
		if(GUI_ENC_API_GetPrevState()!=ID_DLG_MasterSlave)
		{
			if(master_slave_motherCard == 0)
			{
				__glb_cainfo_PairedInfo.m_PairedInfo_Len=250;
				memset(__glb_cainfo_PairedInfo.m_PairedInfo, 0, 256);
				ret = FYF_API_ca_get_value(FYF_CA_SV_CORRESPOND_INFO,(BU32 *)__glb_cainfo_PairedInfo.m_PairedInfo,(BU32 *)&__glb_cainfo_PairedInfo.m_PairedInfo_Len);
				if(FYF_OK == ret)
				{
					GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[7], 0);
					GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[1], 0);
					g_ca_masterSlave = 1;
				}
				else
				{
					GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[6], 0);
					GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[2], 0);
					g_ca_masterSlave = 0;
				}
			}
			else
			{
				g_ca_masterSlave = 0;
				GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[11], 0);
			}
		}	
		APP_Com_API_SetFullScreen();		
	}
	else
	{
		g_ca_masterSlave = 0xff;
		GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[10], 0);
	}

}
void UI_APP_CA_RequestFeed_End(void)
{
	
}
/*--Paired Info--*/

void UI_APP_CA_RequestFeed_Key(BU32 key)
{
	BS32 ret;
	BU32 index = 0;
	if(key == GUI_VK_SELECT)
	{
		if(APP_CA_API_GetCardIn() == 0 ||APP_CA_API_GetCardIn() == 1)
			return;

		switch(g_ca_masterSlave)
		{
		case 0:	
			__glb_cainfo_PairedInfo.m_PairedInfo_Len=250;
			memset(__glb_cainfo_PairedInfo.m_PairedInfo, 0, 256);
			ret = FYF_API_ca_get_value(FYF_CA_SV_CORRESPOND_INFO,(BU32 *)__glb_cainfo_PairedInfo.m_PairedInfo,(BU32 *)&__glb_cainfo_PairedInfo.m_PairedInfo_Len);
			if(FYF_OK == ret)
			{
				GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[7], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[1], 0);
				g_ca_masterSlave = 1;
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[6], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[2], 0);
			}
			break;
		case 1:
			ret = FYF_API_ca_set_value(FYF_CA_SV_CORRESPOND_INFO, (BU32)__glb_cainfo_PairedInfo.m_PairedInfo,(BU32)__glb_cainfo_PairedInfo.m_PairedInfo_Len);
			if(FYF_OK == ret)
			{
				GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[9], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[3], 0);
				APP_Com_API_Stop_RequestFeeding();
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[8], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_RequestFeed, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[4], 0);
			}
			break;
		default:
			break;
		}
	}
	else if(key == GUI_VK_DOWN || key == GUI_VK_UP)
	{
		APP_Com_API_Stop_RequestFeeding();
		GUI_ENC_API_PostMessage (ID_DLG_ChnInfo, WM_KEY, key, 0);
	}
	else if(key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);	
	}
	else if(key == GUI_VK_MENU)
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu,1);	
}


