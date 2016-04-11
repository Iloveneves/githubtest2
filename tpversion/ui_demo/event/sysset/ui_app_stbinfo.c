#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//本机信息指针数组
extern BU08 g_ui_app_str[13][80];
//本机信息指针数组
static BU08 *g_sys_stbInfo_p[9]=
{
	g_ui_app_str[0],
	g_ui_app_str[1],
	g_ui_app_str[2],
	g_ui_app_str[3],
	g_ui_app_str[4],
	g_ui_app_str[5],
	g_ui_app_str[6],
	g_ui_app_str[7],
	g_ui_app_str[8]
};


//本机信息
void UI_APP_STBInfo_Enter(void)
{
	BU32 softVer;
	BU32 hardVer;
	BU32 cas_ver;
	BU08 card_sn[17];
	BU08 time[20];
	BU32 time_len;
	BU32 lang;
	BU32 loaderVer;
	BS32 ret;
	softVer = APP_SYS_SOFT_VERSION;
	hardVer = APP_SYS_HARD_VERSION;
	loaderVer = APP_SYS_LOADER_VERSION;
	APP_Com_API_SetFullScreen();
	//sn = 1;
#if defined (CD_CA)
	ret = FYF_API_ca_get_value(FYF_CA_SV_CARD_INFO,&cas_ver,(BU32 *)card_sn);
	if(ret == FYF_OK)
	{
	    
	    sprintf(g_sys_stbInfo_p[2],"%s",card_sn);
	}
	else
	{
	    sprintf(g_sys_stbInfo_p[2],"");
	}
#elif defined (DVT_CA)
	FYF_CA_ManuInfo dvt_card;

	ret = FYF_API_ca_get_value(FYF_CA_SV_CARD_INFO,&dvt_card,0);
	if(ret == FYF_OK)
	{
		sprintf(g_sys_stbInfo_p[2],"%s",dvt_card.m_byszSerialNO);
	}
	else
	{
		 sprintf(g_sys_stbInfo_p[2],"");
	}
#endif
	sprintf(g_sys_stbInfo_p[0],"%x",softVer);   	
	sprintf(g_sys_stbInfo_p[1],"%x",hardVer);
	sprintf(g_sys_stbInfo_p[3],"%s",APP_SYS_STBTYPE);
	sprintf(g_sys_stbInfo_p[4],"%s",APP_SYS_STBCHIP);
	//sprintf(g_sys_stbInfo_p[5],"%s",APP_SYS_BROADCAST);
	sprintf(g_sys_stbInfo_p[6],"%s",APP_SYS_CAINFO);
	sprintf(g_sys_stbInfo_p[7],"%dMB",APP_SYS_FLASHSIZE);
	sprintf(g_sys_stbInfo_p[8],"%dMB",APP_SYS_SDRAMSIZE);

	
	GUI_ENC_API_SetValue(ID_STA_Line0_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[0],0);
	GUI_ENC_API_SetValue(ID_STA_Line1_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[1],0);
	GUI_ENC_API_SetValue(ID_STA_Line2_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[2],0);
	GUI_ENC_API_SetValue(ID_STA_Line3_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[3],0);
	GUI_ENC_API_SetValue(ID_STA_Line4_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[4],0);
	GUI_ENC_API_SetValue(ID_STA_Line5_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[5],0);
	GUI_ENC_API_SetValue(ID_STA_Line6_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[6],0);
	GUI_ENC_API_SetValue(ID_STA_Line7_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[7],0);
	GUI_ENC_API_SetValue(ID_STA_Line8_StbInfo,WDT_STA_SV_STRING,(BU32)g_sys_stbInfo_p[8],0);
}
void UI_APP_STBInfo_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_RECALL:
		//GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_EXIT:
		//GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	default:
		break;
	}
}
