#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_PIN_LEN 			 8
extern BU08 g_ca_oldPin[CA_PIN_LEN+1];
extern CA_PIN_STATE_E g_pinState;

#define WORKTIME_X 			75
#define WORKTIME_W 			 10
static BU08 g_ca_startHour=0;
static BU08 g_ca_startMin=0;
static BU08 g_ca_endHour=23;
static BU08 g_ca_endMin=59;
static BU08 g_ca_offset=0;
//工作时段设置
void UI_APP_CA_WorkTimeSet_Enter(void)
{
	FYF_CA_WorkTime_s workTime;
	BS32 ret;
	BU32 hwnd;
	GUI_Rect_s rect;
	BU08 pin[CA_PIN_LEN+1]="........";
	GUI_ENC_API_SetValue(ID_TBX_PIN_WorkTime,WDT_TBX_SV_PASSWORD,(BU32)pin,0);
	ret = APP_CA_API_GetValue(FYF_CA_SV_WORK_TIME,(BU32 *)&workTime,0);
	if(FYF_OK == ret)
	{
		g_ca_startHour = workTime.byStartHour;
		g_ca_startMin = workTime.byStartMinute;
		g_ca_endHour = workTime.byEndHour;
		g_ca_endMin = workTime.byEndMinute;
	}
	GUI_ENC_API_Enable(ID_TBX_PIN_WorkTime, 1);
	GUI_ENC_API_Update(ID_TBX_PIN_WorkTime, 1);
	hwnd = GUI_API_GetDlgItem(ID_VFM_WorkTime);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_WorkTime,ID_STA_Event2_WorkTime,ID_PBR_Epg_WorkTime,260,1);
	GUI_ENC_API_SetValue(ID_STA_Title_WorkTime,WDT_STA_SV_DATAID,RS_WorkTime,0);
}
void UI_APP_CA_WorkTimeSet_End(void)
{
}
void UI_APP_CA_WorkTimeSet_Start_OnPaint(BU32 hdc,BU32 lParam)
{
	BU08 showStr[6]={0};
	sprintf(showStr,"%02d:%02d",g_ca_startHour,g_ca_startMin);
	if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_WorkTime))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_StyleCBXOrangeBar_W200_H36);	
		if(g_ca_offset>=0&&g_ca_offset<2)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+g_ca_offset*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
		}
		else if(g_ca_offset>=2&&g_ca_offset<4)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+(g_ca_offset+1)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
		}

		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_BLACK, COLOR_BLACK, 1);

	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_StyleCBXBlueBar_W200_H36);
		//GUI_API_DrawHLine(hdc, WORKTIME_X, 30, WORKTIME_W, 1);			
		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_WHITE, COLOR_BLACK, 1);
	}
	
	
}
void UI_APP_CA_WorkTimeSet_End_OnPaint(BU32 hdc,BU32 lParam)
{
	BU08 showStr[6]={0};
	sprintf(showStr,"%02d:%02d",g_ca_endHour,g_ca_endMin);
	if(GUI_ENC_API_IsFocus(ID_ODC_EndTime_WorkTime))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_StyleCBXOrangeBar_W200_H36);	
		if(g_ca_offset>=0&&g_ca_offset<2)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+g_ca_offset*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
		}
		else if(g_ca_offset>=2&&g_ca_offset<4)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+(g_ca_offset+1)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
		}

		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_StyleCBXBlueBar_W200_H36);
		//GUI_API_DrawHLine(hdc, WORKTIME_X, 30, WORKTIME_W, 1);			
		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_WHITE, COLOR_BLACK, 1);
	}
	
	
}

void UI_APP_CA_WorkTimeSet_Key(BU32 key)
{
	BU08 *pinTemp;
	BU08 TempStr[CA_PIN_LEN+1]={0};
	BU32 value,i;
	BS32 ret;
	BU32 StrLen;
	FYF_CA_WorkTime_s workTime;
	
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:
		if(GUI_ENC_API_IsFocus(ID_TBX_PIN_WorkTime))
		{
			GUI_ENC_API_GetValue(ID_TBX_PIN_WorkTime,WDT_TBX_SV_PASSWORD,(BU32 *)(&pinTemp),0);
			if(FYF_API_Strlen(pinTemp) == CA_PIN_LEN)
			{
				for(i=0;i<CA_PIN_LEN;i++)
				{
					g_ca_oldPin[i] = pinTemp[i] - 0x30;
				}
				GUI_ENC_API_SetFocus(ID_ODC_StartTime_WorkTime);
				GUI_ENC_API_Enable(ID_TBX_PIN_WorkTime, 0);
				GUI_ENC_API_Update(ID_TBX_PIN_WorkTime,1);
			}
			return;
		}
		if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_WorkTime))
		{
			switch(g_ca_offset)
			{
			case 0:
				g_ca_startHour = (BU08)((BU16)g_ca_startHour%10 + key*10);
				break;
			case 1:
				g_ca_startHour = (BU08)((BU16)g_ca_startHour-g_ca_startHour%10 + key);
				break;
			case 2:
				g_ca_startMin = (BU08)((BU16)g_ca_startMin%10 + key*10);
				break;
			case 3:
				g_ca_startMin = (BU08)((BU16)g_ca_startMin-g_ca_startMin%10 + key);
				break;

			default:
				break;
			}
			if(g_ca_startHour>=24)g_ca_startHour = 0;
			if(g_ca_startMin>=60)g_ca_startMin = 0;
			g_ca_offset++;
			if(g_ca_offset>3)g_ca_offset=0;
			GUI_ENC_API_Update(ID_ODC_StartTime_WorkTime, 1);

		}
		else
		{
			switch(g_ca_offset)
			{
			case 0:
				g_ca_endHour = (BU08)((BU16)g_ca_endHour%10 + key*10);
				break;
			case 1:
				g_ca_endHour = (BU08)((BU16)g_ca_endHour-g_ca_endHour%10 + key);
				break;
			case 2:
				g_ca_endMin = (BU08)((BU16)g_ca_endMin%10 + key*10);
				break;
			case 3:
				g_ca_endMin = (BU08)((BU16)g_ca_endMin-g_ca_endMin%10 + key);
				break;

			default:
				break;
			}
			if(g_ca_endHour>=24)g_ca_endHour = 23;
			if(g_ca_endMin>=60)g_ca_endMin = 59;
			g_ca_offset++;
			if(g_ca_offset>3)g_ca_offset=0;
			GUI_ENC_API_Update(ID_ODC_EndTime_WorkTime, 1);
		}

		break;
	case GUI_VK_LEFT:
		if(GUI_ENC_API_IsFocus(ID_TBX_PIN_WorkTime))
		{
			GUI_ENC_API_GetValue(ID_TBX_PIN_WorkTime,WDT_TBX_SV_PASSWORD,(BU32 *)&pinTemp,0);
			StrLen = GUI_API_Strlen(pinTemp);
			if( (GUI_API_Strlen(pinTemp) != 0))
			{
				pinTemp[StrLen-1] = '\0';
				GUI_API_Strcpy(TempStr,pinTemp);
				GUI_ENC_API_SetValue(ID_TBX_PIN_WorkTime,WDT_TBX_SV_PASSWORD,(BU32)TempStr,0);		
			}
		}
		else
		{
			if(g_ca_offset==0)g_ca_offset=3;
			else g_ca_offset--;
			if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_WorkTime))
				GUI_ENC_API_Update(ID_ODC_StartTime_WorkTime, 1);
			else if(GUI_ENC_API_IsFocus(ID_ODC_EndTime_WorkTime))
				GUI_ENC_API_Update(ID_ODC_EndTime_WorkTime, 1);
		}
		break;
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_TBX_PIN_WorkTime))return;
		g_ca_offset++;
		if(g_ca_offset >3)g_ca_offset=0;
		if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_WorkTime))
			GUI_ENC_API_Update(ID_ODC_StartTime_WorkTime, 1);
		else 
			GUI_ENC_API_Update(ID_ODC_EndTime_WorkTime, 1);
		break;
	case GUI_VK_SELECT:
		if(GUI_ENC_API_IsFocus(ID_TBX_PIN_WorkTime))return;
		UI_APP_API_SetPinNotePreState(ID_DLG_WorkTime);
		ret = FYF_API_ca_get_value(FYF_CA_SV_VERIFY_PIN,(BU32)g_ca_oldPin,0);
		if(FYF_OK == ret)
		{	
			workTime.byStartHour = g_ca_startHour;
			workTime.byStartMinute = g_ca_startMin;
			workTime.byEndHour = g_ca_endHour;
			workTime.byEndMinute = g_ca_endMin;
			ret = FYF_API_ca_set_value(FYF_CA_SV_WORK_TIME,(BU32)&workTime,0);
			if(FYF_OK == ret)
			{
				g_pinState = CA_CHANGE_SUCESS_e;
				GUI_ENC_API_SetValue(ID_STA_PinNote,WDT_STA_SV_DATAID,RS_WorktimeChangeSuccess,0);
				GUI_ENC_API_SwitchState(ID_DLG_PinNote,1);
			}
			else
			{
				g_pinState = CA_CHANGE_FAIL_e;
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
		break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_TBX_PIN_WorkTime))
		{
			return;
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_WorkTime))
		{
			GUI_ENC_API_SetFocus(ID_TBX_PIN_WorkTime);
			GUI_ENC_API_Enable(ID_TBX_PIN_WorkTime, 1);
			GUI_ENC_API_Update(ID_TBX_PIN_WorkTime,1);
		}
		else
		{
			GUI_ENC_API_SetFocus(ID_ODC_StartTime_WorkTime);
		}
		GUI_ENC_API_Update(ID_ODC_StartTime_WorkTime, 1);
		GUI_ENC_API_Update(ID_ODC_EndTime_WorkTime, 1);
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_TBX_PIN_WorkTime))
		{
			return;
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_WorkTime))
		{
			GUI_ENC_API_SetFocus(ID_ODC_EndTime_WorkTime);
		}
		else
		{
			GUI_ENC_API_SetFocus(ID_TBX_PIN_WorkTime);
			GUI_ENC_API_Enable(ID_TBX_PIN_WorkTime, 1);
			GUI_ENC_API_Update(ID_TBX_PIN_WorkTime,1);
		}		
		GUI_ENC_API_Update(ID_ODC_StartTime_WorkTime, 1);
		GUI_ENC_API_Update(ID_ODC_EndTime_WorkTime, 1);
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

