#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_PIN_LEN 			 8
extern CA_PIN_STATE_E g_pinState;
extern BU08 g_ca_oldPin[CA_PIN_LEN+1];
BU08 g_ca_newPin[CA_PIN_LEN+1]={0};

//PINÐÞ¸Ä
void UI_APP_CA_PinSet_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	BU08 pin[CA_PIN_LEN+1]="........";
	GUI_ENC_API_SetValue(ID_TBX_Old_PinSet,WDT_TBX_SV_PASSWORD,(BU32)pin,0);
	GUI_ENC_API_SetValue(ID_TBX_New_PinSet,WDT_TBX_SV_PASSWORD,(BU32)pin,0);
	GUI_ENC_API_SetValue(ID_TBX_Check_PinSet,WDT_TBX_SV_PASSWORD,(BU32)pin,0);
	GUI_ENC_API_Enable(ID_TBX_Old_PinSet, 1);
	GUI_ENC_API_Enable(ID_TBX_New_PinSet, 0);
	GUI_ENC_API_Enable(ID_TBX_Check_PinSet, 0);			
	hwnd = GUI_API_GetDlgItem(ID_VFM_PinSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Ëõ·ÅÊÓÆµ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_PinSet,ID_STA_Event2_PinSet,ID_PBR_Epg_PinSet,260,1);
}

void UI_APP_CA_PinSet_End(void)
{

}

void UI_APP_CA_PinSet_Key(BU32 key)
{
	BU32 ret;
	BU08 *pinTemp;
	BU32 i,value = CA_PIN_LEN;
	BU32 StrLen;
	BU32 tbxId;
	BU08 pin[CA_PIN_LEN+1]={0};
	BU08 TempStr[CA_PIN_LEN+1]={0};

	if(GUI_ENC_API_IsFocus(ID_TBX_Old_PinSet))
	{
		GUI_ENC_API_GetValue(ID_TBX_Old_PinSet,WDT_TBX_SV_PASSWORD,(BU32 *)(&pinTemp),0);
		if((FYF_API_Strlen(pinTemp) == CA_PIN_LEN)&&(GUI_API_Strcmp(pinTemp, "........")))
		{
			for(i=0;i<CA_PIN_LEN;i++)
			{
				g_ca_oldPin[i] = pinTemp[i] - 0x30;
			}
			GUI_ENC_API_SetFocus(ID_TBX_New_PinSet);
			GUI_ENC_API_Enable(ID_TBX_Old_PinSet, 0);
			GUI_ENC_API_Enable(ID_TBX_New_PinSet, 1);
			GUI_ENC_API_Update(ID_TBX_Old_PinSet, 1);
			GUI_ENC_API_Update(ID_TBX_New_PinSet, 1);
		}
	}
	else if(GUI_ENC_API_IsFocus(ID_TBX_New_PinSet))
	{
		GUI_ENC_API_GetValue(ID_TBX_New_PinSet,WDT_TBX_SV_PASSWORD,(BU32 *)(&pinTemp),0);
		if((FYF_API_Strlen(pinTemp) == CA_PIN_LEN)&&(GUI_API_Strcmp(pinTemp, "........")))
		{
			FYF_API_Memcpy(g_ca_newPin, pinTemp, CA_PIN_LEN);
			GUI_ENC_API_SetFocus(ID_TBX_Check_PinSet);
			GUI_ENC_API_Enable(ID_TBX_New_PinSet, 0);
			GUI_ENC_API_Enable(ID_TBX_Check_PinSet, 1);
			GUI_ENC_API_Update(ID_TBX_New_PinSet, 1);
			GUI_ENC_API_Update(ID_TBX_Check_PinSet, 1);
		}
	}
	switch(key)
	{
	case GUI_VK_SELECT:
		UI_APP_API_SetPinNotePreState(ID_DLG_PinSet);
		if(GUI_ENC_API_IsFocus(ID_TBX_Check_PinSet))
		{	
			GUI_ENC_API_GetValue(ID_TBX_Check_PinSet,WDT_TBX_SV_PASSWORD,(BU32 *)(&pinTemp),0);
			FYF_API_Memcpy(pin, pinTemp, CA_PIN_LEN);
			if(!FYF_API_Strcmp(g_ca_newPin,pin))
			{
				ret = FYF_API_ca_get_value(FYF_CA_SV_VERIFY_PIN,(BU32)g_ca_oldPin,0);
				if(FYF_OK == ret)
				{	
					for(i=0;i<CA_PIN_LEN;i++)
					{
						g_ca_newPin[i] = pin[i] - 0x30;
					}
					ret = FYF_API_ca_set_value(FYF_CA_SV_CHANGE_PIN,(BU32)g_ca_newPin,0);
					if(FYF_OK == ret)
					{
						g_pinState = CA_CHANGE_SUCESS_e;
						GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_ChangeSuccess,0);
						GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
					}
					else
					{
						g_pinState = CA_CHANGE_FAIL_e;
						if(FYF_STB_PIN_LOCKED == ret)
						{
							g_pinState = CA_PIN_LOCKED_e;
							GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_PinLocked,0);
						}
						else
							GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_ChangeFail,0);
						GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
					}
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
			else
			{
				g_pinState = CA_CHANGE_FAIL_e;
				GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_EnterDiff,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
		}
		break;
	case GUI_VK_LEFT:
		if(GUI_ENC_API_IsFocus(ID_TBX_Old_PinSet))
		{
			tbxId = ID_TBX_Old_PinSet;
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_New_PinSet))
		{
			tbxId = ID_TBX_New_PinSet;
		}
		else
		{
			tbxId = ID_TBX_Check_PinSet;
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

