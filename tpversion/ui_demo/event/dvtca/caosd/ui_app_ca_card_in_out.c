#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

extern FYF_CA_RealTimeIpp_s DVTCAS_RealTimeipp;
void UI_APP_CA_CardOut(void)
{

	BU32 curState;
	APP_Com_API_Stop_RequestFeeding();
	curState = GUI_ENC_API_GetCurState();
	switch(curState)
	{
	case ID_DLG_EntitleInfo:
	case ID_DLG_GetServicer:
	case ID_DLG_ICInfo:
	case ID_DLG_PinSet:
	case ID_DLG_Rating:
	case ID_DLG_WorkTime:
	case ID_DLG_ePurse:
	case ID_DLG_BookUnbook:
	case ID_DLG_ViewedIPP:
	case ID_DLG_VerifyPin:
		GUI_ENC_API_SwitchState(ID_DLG_CardOut, 1);
		break;
	case ID_DLG_RealtimeBook:
		FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,DVTCAS_RealTimeipp.ecmPid,0);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case ID_DLG_BookUnbookInfo:
		if(GUI_ENC_API_GetPrevState() == ID_DLG_BookUnbook)
		{
			GUI_ENC_API_SwitchState(ID_DLG_CardOut, 1);
		}
		else if(GUI_ENC_API_GetPrevState() == ID_DLG_RealtimeBook)
		{
			FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,DVTCAS_RealTimeipp.ecmPid,0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		break;
	default:
		break;

	}

}

void UI_APP_CA_CardIn(void)
{
	BU32 curState=0;
	BU32 preState=0;

#if 0
	curState = GUI_ENC_API_GetCurState();
	if(curState != ID_DLG_CardState)return;
	preState = GUI_ENC_API_GetPrevState();
	switch(preState)
	{
	case ID_DLG_BookUnbook:
		//GUI_ENC_API_SetValue(ID_STA_CardState, WDT_STA_SV_DATAID, RS_ReadIPPInfo, 0);	
	case ID_DLG_EntitleInfo:
	case ID_DLG_Servicer:
	case ID_DLG_ICInfo:
	case ID_DLG_PinSet:
	case ID_DLG_Rating:
	case ID_DLG_WorkTime:
	case ID_DLG_ePurse:
		GUI_ENC_API_SwitchState(preState, 1);
		break;
	default:
		break;

	}
#endif
}


BU08 g_ca_cardout = 0;
void UI_APP_CA_CardOut_Enter(void)
{
	g_ca_cardout = 0;
}
void UI_APP_CA_CardOut_OnPaint(BU32 hdc,BU32 lParam)
{
	BU32 lang,len;
	BU08 *str;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		str = DicDataEnglish[RS_InsertCard];
	}
	else
	{
		str = DicData[RS_InsertCard];
	}
	
	if(lParam == 2)
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_SmallDialog3_W370_H120);
		len = GUI_API_Strlen(str);
		GUI_API_TextOut(hdc, 80+100-len*11/2, 50, str, len, COLOR_BLACK, COLOR_BLACK, 1);
	}
}
void UI_APP_CA_CardOut_Timer(void)
{
	g_ca_cardout ++;
	if(g_ca_cardout == 1)
	{
		GUI_ENC_API_Update(ID_ODC_CardOut, 2);
	}
	else if(g_ca_cardout == 2)
	{
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	}
}

