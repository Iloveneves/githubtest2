#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

char *g_ca_masterCnStr[]=
{
	"请插入母卡",
	"请插入子卡",
	"请插入母卡",
	"请插入子卡",
	"请插入子卡",
	"配对成功!",
	"读取母卡数据失败!",
	"请注意",
	"喂养失败!",
	"喂养成功!",
};
char *g_ca_masterEnStr[]=
{
	"Insert Mother Card",
	"Insert Child Card",
	"Insert Mother Card",
	"Insert Child Card",
	"Please Confirm the Card",
	"Success!",
	"Reading Mother Card Fail!",
	"Attention",
	"Feeding Fail!",
	"Feeding Success!",
};
BU08 g_ui_app_str[13][80];
char **g_ca_master_ppStr;
static BU16 master_slave_tvs[4];
static FYF_CA_MotherCard_s master_slave_motherCard[4];
static FYF_CDCA_PairInfo_s __glb_cainfo_PairedInfo;
static BU08 g_ca_masterSlave = 0;
/*
子母卡配对

g_ca_masterSlave:
				0	插入母卡
				1	母卡成功，插入子卡

*/
static BU08 __glb_caosd_ReqFeed = 0;
void UI_APP_CA_SetReqFeed(BU08 value)
{
	__glb_caosd_ReqFeed = value;
}

void UI_APP_CA_MasterSlave_Enter(void)
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

	GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_DATAID, RS_Warning, 0);
	if(GUI_ENC_API_GetPrevState()!=ID_DLG_Feeding)
	{
		GUI_ENC_API_GetValue(ID_LBX_MasterSlaveManage, WDT_LBX_SV_INDEX, &index, 0);
		if(!master_slave_motherCard[index].m_isMotherCard)
		{
			__glb_cainfo_PairedInfo.m_PairedInfo_Len=0;
			memset(__glb_cainfo_PairedInfo.m_PairedInfo, 0, 256);
			ret = FYF_API_ca_get_value(FYF_CA_SV_CORRESPOND_INFO,(BU32 *)&master_slave_tvs[index],(BU32 *)&__glb_cainfo_PairedInfo);
			if(FYF_OK == ret)
			{
//				GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[7], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[1], 0);
				g_ca_masterSlave = 1;
			}
			else
			{
//				GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[6], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[6], 0);
			}
		}
		else
		{
			g_ca_masterSlave = 0;
			GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[0], 0);
		}
	}
	APP_Com_API_SetFullScreen();
}
void UI_APP_CA_MasterSlave_End(void)
{
	
}
/*--Paired Info--*/

void UI_APP_CA_MasterSlave_Key(BU32 key)
{
	BS32 ret;
	BU32 index = 0;
	if(key == GUI_VK_SELECT)
	{
		if(!APP_CA_API_GetCardIn())
			return;
		GUI_ENC_API_GetValue(ID_LBX_MasterSlaveManage, WDT_LBX_SV_INDEX, &index, 0);
		switch(g_ca_masterSlave)
		{
		case 0:	
			__glb_cainfo_PairedInfo.m_PairedInfo_Len=0;
			memset(__glb_cainfo_PairedInfo.m_PairedInfo, 0, 256);
			ret = FYF_API_ca_get_value(FYF_CA_SV_CORRESPOND_INFO,(BU32 *)&master_slave_tvs[index],(BU32 *)&__glb_cainfo_PairedInfo);
//			soc_printf("__glb_cainfo_PairedInfo_Len = %d\n",__glb_cainfo_PairedInfo_Len);
			if(FYF_OK == ret)
			{
//				GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[7], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[1], 0);
				g_ca_masterSlave = 1;
			}
			else
			{
//				GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[6], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[6], 0);
			}
			break;
		case 1:
			ret = FYF_API_ca_set_value(FYF_CA_SV_CORRESPOND_INFO, (BU32)&__glb_cainfo_PairedInfo, 0);
			if(FYF_OK == ret)
			{
//				GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[9], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[9], 0);
			}
			else
			{
//				GUI_ENC_API_SetValue(ID_STA_Msg_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[8], 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[8], 0);
			}
			break;
		default:
			break;
		}
	}
	else if(key == GUI_VK_RECALL)
		GUI_ENC_API_SwitchState(ID_DLG_MasterSlaveManage, 1);	
	else if(key == GUI_VK_EXIT)
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);	

}

void UI_APP_CA_RequestFeed_Timer(void)
{
	static BU08 time_cnt = 0;
	
	time_cnt++;
	if(time_cnt == 5)
	{
		time_cnt = 0;
		if(__glb_caosd_ReqFeed)
		{
			g_ca_masterSlave = 1;
			GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[1], 0);
		}
		else
		{
			g_ca_masterSlave = 0;
			GUI_ENC_API_SetValue(ID_STA_Line1_MasterSlave, WDT_STA_SV_STRING, (BU32)g_ca_master_ppStr[0], 0);
		}
		GUI_ENC_API_SwitchState(ID_DLG_MasterSlave, 1);
	}
	
}


static BU32 master_slave_tvs_num;
static BU08 *master_slave_p[5]=
{
	g_ui_app_str[9],
	g_ui_app_str[10],
	g_ui_app_str[11],
	g_ui_app_str[12]
};
static BU08 service_title[20];
void UI_APP_CA_MasterSlaveManage_Enter(void)
{
	BS32 ret;
	BU32 lang;
	char **pDic = DicData;
	BU32 i;
	BU08 strTemp[40];
	BU32 index;
	
	APP_Com_API_SetFullScreen();
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
	/*运营商 & 子母卡属性*/
	sprintf(g_ui_app_str[8],"%-9s %-4s %-12s %s",pDic[RS_TVS_ID],pDic[RS_Attr],pDic[RS_FeedTime],pDic[RS_LastFeedTime]);
	memset(master_slave_tvs, 0, sizeof(BU16)*4);
	memset(master_slave_motherCard, 0, sizeof(FYF_CA_MotherCard_s)*4);
	ret = FYF_API_ca_get_value(FYF_CA_SV_MOTHER_INFO,(BU32*)master_slave_tvs,(BU32*)master_slave_motherCard);
	master_slave_tvs_num = 0;
	if(ret == FYF_OK)
	{
		for(i=0;i<4;i++)
		{
			if(master_slave_tvs[i])
			{
				if(master_slave_motherCard[i].m_isMotherCard)
				{
					UI_APP_GetTimeStrFromValC_TF(master_slave_motherCard[i].m_LastFeedTime,strTemp);
					sprintf(g_ui_app_str[9+i],"%-8d %-4s %-14d %s",master_slave_tvs[i],pDic[RS_ChildCard],master_slave_motherCard[i].m_FeedCycle,strTemp);	
				}
				else
				{
					sprintf(g_ui_app_str[9+i],"%-8d %-4s",master_slave_tvs[i],pDic[RS_MotherCard]);	
				}
				master_slave_tvs_num++;
			}
			else
			{
				sprintf(g_ui_app_str[9+i],"    ");	
			}
		}
			
	}
	else
	{	
		sprintf(g_ui_app_str[9],"    ");	
		sprintf(g_ui_app_str[10],"    ");	
		sprintf(g_ui_app_str[11],"    ");	
		sprintf(g_ui_app_str[12],"    ");	
	}	
	GUI_ENC_API_SetValueUnPaint(ID_STA_MasterSlaveManage, WDT_STA_SV_STRING, (BU32)g_ui_app_str[8], 0);
	if(master_slave_tvs_num)
	{
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_MasterSlaveManage, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_MasterSlaveManage, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_MasterSlaveManage, WS_DISABLE);
		GUI_ENC_API_SetValueUnPaint(ID_LBX_MasterSlaveManage, WDT_LBX_SV_DATA, (BU32)master_slave_p, master_slave_tvs_num);
		GUI_ENC_API_GetValue(ID_LBX_MasterSlaveManage, WDT_LBX_SV_INDEX, &index, 0);
		if(index>=master_slave_tvs_num)index=0;
		GUI_ENC_API_SetValue(ID_LBX_MasterSlaveManage, WDT_LBX_SV_INDEX, index, 0);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_MasterSlaveManage, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_MasterSlaveManage, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_MasterSlaveManage, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint(ID_STA_Line1_MasterSlaveManage, WDT_STA_SV_DATAID, RS_GetFailed, 0);
		GUI_ENC_API_SetValueUnPaint (ID_LBX_MasterSlaveManage, WDT_LBX_SV_ALLNUM, 0, 0);
	}
}
void UI_APP_CA_MasterSlaveManage_End(void)
{

}
void UI_APP_CA_MasterSlaveManage_Key(BU32 key)
{
	BU32 i;
	BU32 lang;
	char **pDic = DicData;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	switch(key)
	{
	case GUI_VK_RED:
		if(master_slave_tvs_num)
		{
			GUI_ENC_API_GetValue(ID_LBX_MasterSlaveManage, WDT_LBX_SV_INDEX, &i, 0);
			sprintf(service_title,"%s:%d",pDic[RS_TVS_ID],master_slave_tvs[i]);
			if(!APP_CA_API_GetCardIn())
				return;
			GUI_ENC_API_SetValueUnPaint(ID_STA_MasterSlave, WDT_STA_SV_STRING, (BU32)service_title, 0);
			GUI_ENC_API_SwitchState(ID_DLG_MasterSlave, 1);
		} 
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}
}
