#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

	
void UI_CA_IPI_ShowPrompt(BU32 messageNo)
{
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWCAPROMPT, messageNo, 0);
}
void UI_CA_IPI_HidePrompt(void)
{	
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_HIDECAPROMPT, 0, 0);	
}

void FYF_API_CA_notify(FYF_CA_NOTIFY_e type,BU32 *para1,BU32 *para2)
{	
    BU08 tmp[129];
	switch(type)
	{
	case FYF_CA_NOTIFY_CARD_IN:
		//soc_printf("\n------------FYF_CA_NOTIFY_CARD_IN-------------------\n");
		UI_APP_CA_CardIn();
		break;
	case FYF_CA_NOTIFY_CARD_OUT:
		//soc_printf("\n------------FYF_CA_NOTIFY_CARD_OUT-------------------\n");		
		UI_APP_CA_CardOut();
		break;
	case FYF_CA_NOTIFY_MAIL:
//		soc_printf("\n------------FYF_CA_NOTIFY_MAIL-------------------\n");	
		UI_APP_CA_GotNewMail();
		break;
	case FYF_CA_NOTIFY_MSG:
//		soc_printf("\n------------FYF_CA_NOTIFY_MSG-------------------\n");
	//	FYF_API_ca_get_value(FYF_CA_SV_MSG,(BU32 *)tmp,0);
	//	UI_APP_CA_GotMsg();
		break;
	case FYF_CA_NOTIFY_PPV:
//		soc_printf("\n------------FYF_CA_NOTIFY_PPV-------------------\n");
	//	UI_APP_CA_IppvProg();
	//	FYF_API_ca_set_value(FYF_CA_SV_PPV,FYF_TRUE,0);
		break;
	case FYF_CA_NOTIFY_APP_LOCKED:
//		soc_printf("\n------------FYF_CA_NOTIFY_APP_LOCKED-------------------\n");
		break;
	case FYF_CA_NOTIFY_CARD_LOCKED:
//		soc_printf("\n------------FYF_CA_NOTIFY_CARD_LOCKED-------------------\n");
		break;
	case FYF_CA_NOTIFY_NOT_AUTHORED:
	//	UI_APP_CA_NotAuthored();
//		soc_printf("\n------------FYF_CA_NOTIFY_NOT_AUTHORED-------------------\n");
		break;
	case FYF_CA_NOTIFY_CARD_NOTFOUND:
//		soc_printf("\n------------FYF_CA_NOTIFY_CARD_NOTFOUND-------------------\n");
		break;
	case FYF_CA_NOTIFY_NO_MONEY:
//		soc_printf("\n------------FYF_CA_NOTIFY_NO_MONEY-------------------\n");
		break;
	case FYF_CA_NOTIFY_EXTER_AUTHEN:
//		soc_printf("\n------------FYF_CA_NOTIFY_EXTER_AUTHEN-------------------\n");
		break;
	case FYF_CA_NOTIFY_PIN_AUTHEN:
//		soc_printf("\n------------FYF_CA_NOTIFY_PIN_AUTHEN-------------------\n");
		break;
	case FYF_CA_NOTIFY_NOREG_CARD:
//		soc_printf("\n------------FYF_CA_NOTIFY_NOREG_CARD-------------------\n");
		break;
	case FYF_CA_NOTIFY_ILLEGAL_CARD:
//		soc_printf("\n------------FYF_CA_NOTIFY_ILLEGAL_CARD-------------------\n");
		break;
	case FYF_CA_NOTIFY_SCARD_NOT_ACT:
//		soc_printf("\n------------FYF_CA_NOTIFY_SCARD_NOT_ACT-------------------\n");
		break;	
	case FYF_CA_NOTIFY_SCARD_DISABLE:
//		soc_printf("\n------------FYF_CA_NOTIFY_SCARD_DISABLE-------------------\n");
		break;
	case FYF_CA_NOTIFY_ZONE_CODE:
//		soc_printf("\n------------FYF_CA_NOTIFY_ZONE_CODE-------------------\n");
		break;
	case FYF_CA_NOTIFY_APP_STOP:
//		soc_printf("\n------------FYF_CA_NOTIFY_APP_STOP-------------------\n");
		break;
	case FYF_CA_NOTIFY_APP_RESUME:
//		soc_printf("\n------------FYF_CA_NOTIFY_APP_RESUME-------------------\n");
		break;
	case FYF_CA_NOTIFY_PROG_UNAUTH:
//		soc_printf("\n------------FYF_CA_NOTIFY_PROG_UNAUTH-------------------\n");
		break;
	case FYF_CA_NOTIFY_PROG_AUTH:
//		soc_printf("\n------------FYF_CA_NOTIFY_PROG_AUTH-------------------\n");
		break;
	case FYF_CA_NOTIFY_EN_PARTNER:
//		soc_printf("\n------------FYF_CA_NOTIFY_EN_PARTNER-------------------\n");
		break;
	case FYF_CA_NOTIFY_DIS_PARTNER:
//		soc_printf("\n------------FYF_CA_NOTIFY_DIS_PARTNER-------------------\n");
		break;
	case FYF_CA_NOTIFY_UPDATE_GNO:
//		soc_printf("\n------------FYF_CA_NOTIFY_UPDATE_GNO-------------------\n");
		break;
	case FYF_CA_NOTIFY_UPDATE_GK:
//		soc_printf("\n------------FYF_CA_NOTIFY_UPDATE_GK-------------------\n");
		break;
	case FYF_CA_NOTIFY_CHARGE:
//		soc_printf("\n------------FYF_CA_NOTIFY_CHARGE-------------------\n");
		break;
	case FYF_CA_NOTIFY_FIGEIDENT:
//		soc_printf("\n------------FYF_CA_NOTIFY_FIGEIDENT-------------------\n");		
 		UI_APP_CAOsd_ShowFingerPrinting(*para1,*para2);
		break;
	case FYF_CA_NOFITY_SONCARD:
		//UI_APP_CAosd_RequestFeeding(*para1,*para2);
		UI_APP_CAosd_RequestFeeding();
//		soc_printf("\n------------FYF_CA_NOFITY_SONCARD-------------------\n");
		break;
	case FYF_CA_NOFITY_URGENT_SERVICE:
//		soc_printf("\n------------FYF_CA_NOFITY_URGENT_SERVICE-------------------\n");
		break;
	case FYF_CA_NOTIFY_MODIFY_ZONECODE:
//		soc_printf("\n------------FYF_CA_NOTIFY_MODIFY_ZONECODE-------------------\n");
		break;
	case FYF_CA_NOTIFY_LOADER:
//		soc_printf("\n------------FYF_CA_NOTIFY_LOADER-------------------\n");
		break;
	case FYF_CA_EMAIL_NEW:
		UI_APP_CA_GotNewMail();
		FYF_API_printf("\n------------FYF_CA_EMAIL_NEW-------------------\n");
		break;
	case FYF_CA_EMAIL_NEW_NO_ROOM:
		UI_APP_CA_GotNewMailNoRoom();
		FYF_API_printf("\n------------FYF_CA_EMAIL_NEW_NO_ROOM-------------------\n");
		break;
	case FYF_CA_EMAIL_NONE:
		UI_APP_CA_GotNewMailNone();
		FYF_API_printf("\n------------FYF_CA_EMAIL_NONE-------------------\n");
		break;
	case FYF_CA_NOTIFY_SCROLL_MSG:
		FYF_API_printf("\n------------FYF_CA_NOTIFY_SCROLL_MSG-------------------\n");
		if(*(BU08 *)para2 == 1)
		{
			UI_APP_CAOsd_ShowOsdMsgTop((char *)para1);
		}
		else if(*(BU08 *)para2 == 2)
		{
			UI_APP_CAOsd_ShowOsdMsg((char *)para1);
		}

		break;
	case FYF_CA_NOTIFY_CLEAR_MSG:
		UI_APP_CAOsd_ClearOsdMsg(*(BU08 *)para2);
		break;
	case FYF_CA_NOTIFY_SHOW_PROMPT:  /* 显示提示信息*/
		FYF_API_printf("\n------------FYF_CA_NOTIFY_SHOW_PROMPT-------------------\n");
		UI_CA_IPI_ShowPrompt(*(BU08 *)para1);
		break;	
	case FYF_CA_NOTIFY_HIDE_PROMPT:   /* 隐藏提示信息*/
		FYF_API_printf("\n------------FYF_CA_NOTIFY_HIDE_PROMPT-------------------\n");
		UI_CA_IPI_HidePrompt();
		break;
	case FYF_CA_NOTIFY_SWITCH_CHANNEL:
		*para2 = UI_APP_AreaLock_SwitchChannel (*para1);
		break;
	case FYF_CA_NOTIFY_AREA_LOCK_OK:
		UI_APP_AreaLock_ok ();
		break;
/*
	case FYF_CA_NOTIFY_DETITLE_RECEIVED:
		UI_APP_API_DetitleReceived (*(BU16 *)para1, *(BU08 *)para2);
		break;
	case FYF_CA_NOTIFY_PROGRESS_STRIP:
		UI_APP_API_ShowProgressStrip (*(BU08 *)para1, *(BU08 *)para2);
		break;
	case FYF_CA_NOFITY_FEEDING_HIDE:
		UI_APP_CAosd_EndFeeding ();
		break;
*/
	default:
		break;
	}
}


