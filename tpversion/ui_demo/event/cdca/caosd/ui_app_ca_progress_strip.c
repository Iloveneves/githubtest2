#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

static BS32 ui_app_progress_show = FYF_FALSE;
static BU32 ui_app_progress = 0;
static BU08 ui_app_showProgress_Buf[5];

BS32 UI_APP_API_IsProgressStripShow(void)
{
	return ui_app_progress_show;
}

BU32 UI_APP_API_GetProgress(void)
{
	return ui_app_progress;
}

void UI_APP_API_ShowProgressStrip(BU08 byProgress,BU08 byMark)
{
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_PROGRESS_STRIP, byProgress, byMark);
}

void UI_APP_API_ShowProgressStripEx(BU08 byProgress,BU08 byMark)
{
	BU32 curState;

	curState = GUI_ENC_API_GetCurState();
	ui_app_progress = byProgress;
	if(byProgress <= 100)
	{
		ui_app_progress_show = FYF_TRUE;
		sprintf(ui_app_showProgress_Buf,"%d%%",byProgress);
		if(byMark == 1)
		{
			GUI_ENC_API_SetValueUnPaint(ID_STA_Text1_ChInfo, WDT_STA_SV_DATAID, RS_Receive_Patch, 0);
		}
		else if(byMark == 2)
		{
			GUI_ENC_API_SetValueUnPaint(ID_STA_Text1_ChInfo, WDT_STA_SV_DATAID, RS_Patching, 0);
		}
		GUI_ENC_API_SetValueUnPaint(ID_STA_Text2_ChInfo, WDT_STA_SV_STRING, (BU32)ui_app_showProgress_Buf, 0);
		if((curState == ID_DLG_ChnInfo)&&(UI_APP_API_GetChnInfoCurState()!= TRACKSET_STATE)&&(CDSTBCA_API_IppDlgShow () == FYF_FALSE)&&(UI_APP_API_IsOsdShow () == 0))
		{
			GUI_ENC_API_SetVisible(ID_STA_Text1_ChInfo,WS_VISIBLE);	
			GUI_ENC_API_SetVisible(ID_STA_Text2_ChInfo,WS_VISIBLE);	
			GUI_ENC_API_SetVisible(ID_PBR_ChInfo,WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_Text1_ChInfo ,1);
			GUI_ENC_API_Update(ID_STA_Text2_ChInfo ,1);
			GUI_ENC_API_SetValueUnPaint(ID_PBR_ChInfo, WDT_PBR_SV_CHANGE, (320*byProgress)/100, 0);
			GUI_ENC_API_Update(ID_PBR_ChInfo ,1);	
		}
		
	}
	else
	{
		ui_app_progress_show = FYF_FALSE;
		GUI_ENC_API_SetVisible(ID_STA_Text1_ChInfo,WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text2_ChInfo,WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_PBR_ChInfo,WS_DISABLE);
		if((curState == ID_DLG_ChnInfo)&&(UI_APP_API_GetChnInfoCurState()!= TRACKSET_STATE)&&(CDSTBCA_API_IppDlgShow () == FYF_FALSE)&&(UI_APP_API_IsOsdShow () == 0))
		{
			GUI_API_FillRect(GUI_API_GetDesktopWindow(), 100, 340, 530, 24, COLOR_TRANS);
		}
	}
}

void UI_APP_API_HideProgressStripEx(void)
{
	BU32 curState;

	curState = GUI_ENC_API_GetCurState();
	ui_app_progress_show = FYF_FALSE;
	GUI_ENC_API_SetVisible(ID_STA_Text1_ChInfo,WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_Text2_ChInfo,WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_PBR_ChInfo,WS_DISABLE);
	if((curState == ID_DLG_ChnInfo)&&(UI_APP_API_GetChnInfoCurState()!= TRACKSET_STATE)&&(CDSTBCA_API_IppDlgShow () == FYF_FALSE)&&(UI_APP_API_IsOsdShow () == 0))
	{
		GUI_API_FillRect(GUI_API_GetDesktopWindow(), 100, 340, 530, 24, COLOR_TRANS);
	}
}

