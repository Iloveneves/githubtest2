#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_PIN_LEN 			 6
static BU32 rating = 0;
extern CA_PIN_STATE_E g_pinState;
extern  BU08 g_ca_oldPin[CA_PIN_LEN+1];
//成人级设置
void UI_APP_CA_RateSet_Enter(void)
{
	BS32 ret;
	BU32 hwnd;
	GUI_Rect_s rect;
	BU08 pin[CA_PIN_LEN+1]="......";
	GUI_ENC_API_SetValue(ID_TBX_Rating,WDT_TBX_SV_PASSWORD,(BU32)pin,0);
	ret = APP_CA_API_GetValue(FYF_CA_SV_RATING,&rating,0);
	if(FYF_OK == ret)
	{
		GUI_ENC_API_SetValue(ID_TBX_Value_Rating,WDT_TBX_SV_DATA,rating,0);
	}
	GUI_ENC_API_Enable(ID_TBX_Rating, 1);
	GUI_ENC_API_Enable(ID_TBX_Value_Rating, 0);
	APP_EPG_API_StopGetEpg();
	GUI_ENC_API_SetValue(ID_STA_Title_Rating,WDT_STA_SV_DATAID,RS_Rating,0);
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_Rating,ID_STA_Event2_Rating,ID_PBR_Epg_Rating,260,1);
}
void UI_APP_CA_RateSet_End(void)
{

}

void UI_APP_CA_RateSet_Key(BU32 key)
{
	BU08 *pinTemp;
	BU08 TempStr[CA_PIN_LEN+1]={0};
	BU32 value,i;
	BS32 ret;
	BU32 index,StrLen;
	BU32 tbxId;

	if(GUI_ENC_API_IsFocus(ID_TBX_Rating))
	{
		GUI_ENC_API_GetValue(ID_TBX_Rating,WDT_TBX_SV_PASSWORD,(BU32 *)(&pinTemp),0);
		if(FYF_API_Strlen(pinTemp) == CA_PIN_LEN)
		{
			for(i=0;i<6;i++)
			{
				g_ca_oldPin[i] = pinTemp[i] - 0x30;
			}
			GUI_ENC_API_SetFocus(ID_TBX_Value_Rating);
			GUI_ENC_API_Enable(ID_TBX_Rating, 0);
			GUI_ENC_API_Enable(ID_TBX_Value_Rating, 1);
			GUI_ENC_API_Update(ID_TBX_Rating, 1);
			GUI_ENC_API_Update(ID_TBX_Value_Rating, 1);
		}
	}	
	switch(key)
	{
	case GUI_VK_UP:
		GUI_ENC_API_SetFocus(ID_TBX_Rating);
		break;
	case GUI_VK_DOWN:
		GUI_ENC_API_SetFocus(ID_TBX_Rating);
		break;
	case GUI_VK_SELECT:
		if(GUI_ENC_API_IsFocus(ID_TBX_Value_Rating))
		{
			UI_APP_API_SetPinNotePreState(ID_DLG_Rating);
			GUI_ENC_API_GetValue(ID_TBX_Value_Rating,WDT_TBX_SV_DATA,&index,0);
			ret = FYF_API_ca_set_value(FYF_CA_SV_RATING,index,(BU32)g_ca_oldPin);
			if(FYF_OK == ret)
			{
				g_pinState = CA_CHANGE_SUCESS_e;
				GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_RateChangeSuccess,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
			else
			{
				g_pinState = CA_CHANGE_FAIL_e;
				if(FYF_RC_WATCHRATING_INVALID == ret)
					GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_RateInvalid,0);
				else if(FYF_RC_PIN_INVALID == ret)
					GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_PinErr,0);
				else
					GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_ChangeFail,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
		}
		break;
	case GUI_VK_LEFT:
		if(GUI_ENC_API_IsFocus(ID_TBX_Rating))
		{
			tbxId = ID_TBX_Rating;
		}
		else
		{
			return;
		}
		GUI_ENC_API_GetValue(tbxId,WDT_TBX_SV_PASSWORD,(BU32 *)&pinTemp,0);
		StrLen = GUI_API_Strlen(pinTemp);
		if( (GUI_API_Strlen(pinTemp) != 0))
		{
			pinTemp[StrLen-1] = '\0';
			GUI_API_Strcpy(TempStr,pinTemp);
			GUI_ENC_API_SetValue(tbxId,WDT_TBX_SV_PASSWORD,(BU32)TempStr,0);		
		}
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	default:
		return;
		
	}

}

