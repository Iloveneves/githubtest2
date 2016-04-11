#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_PIN_LEN 			 8
extern CA_PIN_STATE_E g_pinState;

//PINÐÞ¸Ä
void UI_APP_CA_VerifyPin_Enter(void)
{
	BU08 pin[CA_PIN_LEN+1]="........";
	GUI_ENC_API_SetValue(ID_TBX_Pin_VerifyPin,WDT_TBX_SV_PASSWORD,(BU32)pin,0);
	APP_Com_API_SetFullScreen();
}

void UI_APP_CA_VerifyPin_End(void)
{

}

void UI_APP_CA_VerifyPin_Key(BU32 key)
{
	BU32 ret;
	BU08 *pinTemp;
	BU32 i;
	BU08 pin[CA_PIN_LEN+1]={0};
	BU32 StrLen;
	BU08 TempStr[CA_PIN_LEN+1]={0};

	switch(key)
	{
	case GUI_VK_SELECT:
		UI_APP_API_SetPinNotePreState(ID_DLG_VerifyPin);
		if(GUI_ENC_API_IsFocus(ID_TBX_Pin_VerifyPin))
		{	
			GUI_ENC_API_GetValue(ID_TBX_Pin_VerifyPin,WDT_TBX_SV_PASSWORD,(BU32 *)(&pinTemp),0);

			for(i=0;i<CA_PIN_LEN;i++)
			{
				pin[i] = pinTemp[i] - 0x30;
			}

			ret = FYF_API_ca_get_value(FYF_CA_SV_VERIFY_PIN,(BU32)pin,0);
			if(FYF_OK == ret)
			{	
				GUI_ENC_API_SwitchState(ID_DLG_BookUnbook,1);
			}
			else if(FYF_STB_DATA_LEN_ERROR == ret)
			{
				g_pinState = CA_PIN_ERROR_e;
				GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_LenErr,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
			else if(FYF_STB_PIN_LOCKED == ret)
			{
				g_pinState = CA_PIN_LOCKED_e;
				GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_PinLocked,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
			else if(FYF_STB_PIN_INVALID == ret)
			{
				g_pinState = CA_PIN_ERROR_e;
				GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_PinErr,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
		}
		break;
	case GUI_VK_LEFT:
		if(GUI_ENC_API_IsFocus(ID_TBX_Pin_VerifyPin))
		{
			GUI_ENC_API_GetValue(ID_TBX_Pin_VerifyPin,WDT_TBX_SV_PASSWORD,(BU32 *)&pinTemp,0);
			StrLen = GUI_API_Strlen(pinTemp);
			if( (GUI_API_Strlen(pinTemp) != 0))
			{
				pinTemp[StrLen-1] = '\0';
				GUI_API_Strcpy(TempStr,pinTemp);
				GUI_ENC_API_SetValue(ID_TBX_Pin_VerifyPin,WDT_TBX_SV_PASSWORD,(BU32)TempStr,0);		
			}
		}
		break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;	
	}
}

