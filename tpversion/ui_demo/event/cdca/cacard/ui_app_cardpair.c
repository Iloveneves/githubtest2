#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

BU08 g_ui_app_str[13][80];
//»ú¿¨Åä¶Ô
void UI_APP_CA_CardPair_Enter(void)
{
	BU08 paired_stb_num = 5;
	BU08 stb_id[30] = {0};
	char **pDic = DicData;
	BU32 i=0;
	BU32 ret;
	BU32 lang;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
    for(i=0;i<paired_stb_num;i++)
    {
		sprintf(g_ui_app_str[i],"     ");
    }
	ret = APP_CA_API_GetValue(FYF_CA_SV_Card_Pair,(BU32 *)&paired_stb_num,(BU32 *)stb_id);
//APP_Printf("paired_stb_num = %d\n",paired_stb_num);
    if(ret == FYF_OK)
    {
		GUI_ENC_API_SetValue(ID_STA_Line1_CardPair, WDT_STA_SV_DATAID, RS_CardPairedAlready, 0);
    }  
    else if(ret == FYF_CA_CARD_INVALID)
    {
		GUI_ENC_API_SetValue(ID_STA_Line1_CardPair, WDT_STA_SV_DATAID, RS_CardPair_Invalid, 0);
    }     
    else if(ret == FYF_CA_CARD_NOPAIR)
    {
		GUI_ENC_API_SetValue(ID_STA_Line1_CardPair, WDT_STA_SV_DATAID, RS_CardPairedNoSTB, 0);
    }        
    else if(ret == FYF_CA_CARD_PAIROTHER)
    {
		GUI_ENC_API_SetValue(ID_STA_Line1_CardPair, WDT_STA_SV_DATAID, RS_CardPairedAnother, 0);
    }

	if(ret == FYF_OK || ret == FYF_CA_CARD_PAIROTHER)
	{
		if(paired_stb_num>5)
			paired_stb_num=5;
			
	    for(i=0;i<paired_stb_num;i++)
	    {
			sprintf(g_ui_app_str[i],"%s%d:0x%02x%02x%02x%02x%02x%02x",pDic[RS_PairedSTB],i+1,stb_id[6*i],stb_id[6*i+1],stb_id[6*i+2],stb_id[6*i+3],stb_id[6*i+4],stb_id[6*i+5]);
		}
	}
	GUI_ENC_API_SetValue(ID_STA_Line1_StbID, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_StbID, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_StbID, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_StbID, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_StbID, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);


    APP_Com_API_SetFullScreen();


}

void UI_APP_CA_CardPair_Key(BU32 key)
{
	if(key == GUI_VK_RECALL)
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);	
	else if(key == GUI_VK_EXIT)
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
}


