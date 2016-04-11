#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"
#define NOTE_TIMER_PERIOD  1500

void FullScreen_IPI_SetCurState_Ipp(void)
{	
	if(ID_DLG_ChnInfo == GUI_ENC_API_GetCurState ())
	{
		GUI_ENC_API_SetFocus(ID_STA_ipp_price_ChnInfo);	
	}
	FullScreen_IPI_SetCurState (IPP_STATE);
}

void FullScreen_IPI_SetCurState_InfoBar(void)
{
	if(ID_DLG_ChnInfo == GUI_ENC_API_GetCurState ())
	{		
		GUI_ENC_API_SetFocus(ID_DLG_ChnInfo);
		FullScreen_IPI_ClearScreen();	
	}
	FullScreen_IPI_SetChnInfo();
	FullScreen_IPI_SetCurState (INFOBAR_STATE);
}

void FullScreen_IPI_SetCurState_IppNotify(void)
{
	GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, CDSTBCA_API_IppBuyRetID (), 0);
	GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);
	GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
	
	if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
	{
		FullScreen_IPI_ClearScreen();
	}
	FullScreen_IPI_SetCurState(NO_SERVICE);
}

