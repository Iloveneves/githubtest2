#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

BU08 g_show_flag = 0;
BU08 PictureShow_API_GetShowFlag()
{
	return g_show_flag;
}
void PictureShow_API_SetShowFlag(BU08 flag)
{
	g_show_flag = flag;
}
void UI_APP_PictureShow_Enter(void)
{
	BU32 seconds;
	seconds = APP_PictureDecoder_API_GetEffectSeconds();
	GUI_API_FillRect(GUI_API_GetDesktopWindow(),0,0,720,576,RGB(0,0,0));
	APP_Printf("seconds = %d\n", seconds);
	if (g_show_flag == 2)
	{
		GUI_ENC_API_SetValue(ID_TIM_PictureShow, WDT_TIM_SV_PLAY, 0, 0);
		GUI_ENC_API_SetValue(ID_TIM_PictureShow, WDT_TIM_SV_PERIOD, seconds * 1000, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_TIM_PictureShow, WDT_TIM_SV_STOP, 0, 0);
	}
}
void UI_APP_PictureShow_End(void)
{

	
}
void UI_APP_PictureShow_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_SELECT:
		//APP_PictureDecoder_API_NextPicture();
		break;
	case GUI_VK_UP:
		if (g_show_flag == 1)
		{
			APP_PictureDecoder_API_PrePicture();
		}
		break;
	case GUI_VK_DOWN:
		if (g_show_flag == 1)
		{
			APP_PictureDecoder_API_NextPicture();
		}
		break;
	case GUI_VK_LEFT:
		
		break;
	case GUI_VK_RIGHT:

		break;
	case GUI_VK_EXIT:
		//GUI_ENC_API_SetValue(ID_TIM_PictureShow, WDT_TIM_SV_STOP, 0, 0);
		g_show_flag = 0;
		APP_Com_API_Wait();
		APP_PictureDecoder_API_Stop();
		GUI_ENC_API_SwitchState(ID_DLG_PhotoFrame, 1);
		break;
	default:
		break;
	}
}

void UI_APP_PictureShow_Timer(void)
{
//	APP_Printf("UI_APP_PictureShow_Timer\n");
	APP_PictureDecoder_API_NextPicture();
}


