#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"
	
void UI_APP_CA_GotNewMail(void)
{	

	UI_APP_SetMailFullState(0);
//	GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STRING,(BU32)DicData[RS_NewMail],0);
	GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STYLE,STYLE_Email_W51_H34,0);
//	GUI_ENC_API_SetVisible(ID_STA_Mail_ChInfo,WS_VISIBLE); 
//	GUI_ENC_API_SetVisible(ID_STA_HideMail_ChInfo,WS_DISABLE); 

	GUI_ENC_API_Update(ID_STA_Mail_ChInfo ,1);	
//	APP_CA_API_SaveNewMail();
}
void UI_APP_CA_GotNewMailNoRoom(void)
{	
//	GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STRING,(BU32)"” œ‰“—¬˙",0);

    UI_APP_SetMailFullState(1);
//	GUI_ENC_API_SetVisible(ID_STA_Mail_ChInfo,WS_VISIBLE); 
//	GUI_ENC_API_SetVisible(ID_STA_HideMail_ChInfo,WS_VISIBLE); 
//	GUI_ENC_API_Update(ID_STA_Mail_ChInfo ,1);	
}
void UI_APP_CA_GotNewMailNone(void)
{	
	UI_APP_SetMailFullState(0);
	GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STYLE,STYLE_NULL,0);
//	GUI_ENC_API_SetVisible(ID_STA_Mail_ChInfo,WS_DISABLE); 
	GUI_ENC_API_Update(ID_STA_HideMail_ChInfo ,1);	
}