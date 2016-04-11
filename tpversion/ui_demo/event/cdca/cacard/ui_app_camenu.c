#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

/*PIN检查提示*/
#define CA_PIN_LEN 			 6
BU08 g_ca_oldPin[CA_PIN_LEN+1]={0};
CA_PIN_STATE_E g_pinState = CA_PIN_NULL_e;

//条件接收
void UI_APP_CaMenu_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Ca);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_Ca,ID_STA_Event2_Ca,ID_PBR_Epg_Ca,260,1);
}
void UI_APP_CaMenu_End(void)
{

}
void UI_APP_CaMenu_Key(BU32 key)
{
	BU32 curID;
	BU32 menulock;
	BU32 avType;
	BU32 tmp_cur_no;
	BU16 curChn,chnNum ;
	GUI_ENC_API_GetValue(ID_MNU_Ca,WDT_MNU_SV_CURDATAID,&curID,0);

	
 	if(key == GUI_VK_SELECT)
	{
		switch(curID)
		{
/*
		case RS_Email:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			UI_APP_CA_SetMailPreState(ID_DLG_Ca);
			GUI_ENC_API_SwitchState(ID_DLG_Mail, 1);
			break;
			*/
		case RS_CaCardInfo:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			GUI_ENC_API_SwitchState(ID_DLG_ICInfo, 1);
			break;
		case RS_EntitleInfo:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			GUI_ENC_API_SwitchState(ID_DLG_GetServicer,1);	
			break;
		case RS_SlotsInfo:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			GUI_ENC_API_SwitchState(ID_DLG_GetServicer,1);
			break;
		case RS_IPPInfo:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_GetServicer, 1);
			}
			break;
		case RS_Detitle_Ack_Code:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			GUI_ENC_API_SwitchState(ID_DLG_GetServicer, 1);
			break;
		case RS_Rating:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_Rating, 1);	
			}				
			break;
		case RS_CaPasswdSet:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_PinSet, 1);		
			}		
			break;
		case RS_WorkTime:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_WorkTime, 1);	
			}			
			break;
		case RS_CardPair:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			GUI_ENC_API_SwitchState(ID_DLG_CardPair, 1);					
			break;
		case RS_MasterSlave:
			if(!APP_CA_API_GetCardIn())
			{		
				GUI_ENC_API_SwitchState(ID_DLG_CardState,1);
				return;
			}
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_MasterSlaveManage, 1);		
			}				
			break;
		default:
			break;
		}
 	}
	else if(key == GUI_VK_RECALL)
	{
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	}
	else if(key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}

