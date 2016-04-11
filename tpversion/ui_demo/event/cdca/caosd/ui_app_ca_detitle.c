#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"


ui_detitle_received_info_s ui_detitle_received_info = {0,0xff,610,130,0};

void UI_APP_API_DetitleReceived(BU16 wTvsID, BU08 bstatus)
{
	ui_detitle_received_info.wTvsID = wTvsID;
	ui_detitle_received_info.bstatus = bstatus;
	ui_detitle_received_info.show = 1;
}

void _CA_OSD_ShowDetitleReceived(BU32 hdc)
{
	BU32 lang;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);

	switch(ui_detitle_received_info.bstatus)
	{
	case FYF_CDCA_Detitle_All_Readed:
		GUI_API_FillRect(hdc, ui_detitle_received_info.xPos, ui_detitle_received_info.yPos, 51, 34, COLOR_TRANS);	
		break;
	case FYF_CDCA_Detitle_Received:
		GUI_API_DrawStyle (hdc, ui_detitle_received_info.xPos, ui_detitle_received_info.yPos, STYLE_TransRect_W51_H34);
		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
		{
			GUI_API_TextOut(hdc, ui_detitle_received_info.xPos+4, ui_detitle_received_info.yPos+6, "Detitle", 7, COLOR_BLACK, COLOR_INFOBK, 0);	
		}
		else
		{
			GUI_API_TextOut(hdc, ui_detitle_received_info.xPos+4, ui_detitle_received_info.yPos+6, "反授权", 6, COLOR_BLACK, COLOR_INFOBK, 0);	
		}
		break;
	case FYF_CDCA_Detitle_Space_Small:
		GUI_API_DrawStyle (hdc, ui_detitle_received_info.xPos, ui_detitle_received_info.yPos, STYLE_TransRect_W51_H34);
		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
		{
			GUI_API_TextOut(hdc, ui_detitle_received_info.xPos+4, ui_detitle_received_info.yPos+6, "Detitle", 7, COLOR_WHITE, COLOR_INFOBK, 0);	
		}
		else
		{
			GUI_API_TextOut(hdc, ui_detitle_received_info.xPos+4, ui_detitle_received_info.yPos+6, "反授权", 6, COLOR_WHITE, COLOR_INFOBK, 0);	
		}
		break;
	case FYF_CDCA_Detitle_Ignore:
		break;
	}	
}

