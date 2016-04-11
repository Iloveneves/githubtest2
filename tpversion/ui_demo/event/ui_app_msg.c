#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

extern BU16 nvodPlayServiceId;
extern BU32 nvodPlayFlag;
extern BU08 bfirst;
extern BU08 bGuiTimerSuspend;
struct LAST_MESSAGE_S 
{
	BU32 message;
	BU32 wParam;
	BU32 lParam;
}g_SaveMessage[6];

enum
{
	MESSAGE_INDEX_PAUSE, 		//节目暂停
	MESSAGE_INDEX_CA,			//CA提示
	MESSAGE_INDEX_NOPROG,		//无节目
	MESSAGE_INDEX_NOSIGNAL,		//无信号
	MESSAGE_INDEX_PLAY,			//在播台
	MESSAGE_INDEX_BLACK			//黑屏
};
static BU08 g_sys_sysTime[60];
char **g_sys_ppStr = DicData;
void UI_APP_SetIcoToCn(void);
void UI_APP_SetIcoToEn(void);
void UI_OSD_API_ShowPrompt(BU32 rs_id, BU08 bShow);

/*
*  	当前显示情况:
*					
*  	显示CA		|	0000 0001	&	0011 0001 	0x31,保留 黑屏
*	节目暂停 	|	0000 0010	&	0010 0010 	0x22,保留 黑屏
*	无节目   	|	0000 0100	&	0000 1100 	0x0c,保留 无信号
*	无信号   	|	0000 1000	&	0000 1100 	0x0c,保留 无节目 
*	有信号   	|	0001 0000	&	0001 0011	0x13,保留 CA,暂停 
*	黑屏	   	|	0010 0000	&	0010 0111 	0x27,保留 CA,暂停,无节目
*/
BU32 bShowPrompt = 0;		/* 目前是否显示提示信息 */

BU32 bShowHDPrompt = 0;
#define SHOW_CA_PROMPT(_val_)	(_val_ = (_val_|0x01)&0x39)				/* 显示CA提示信息 */
#define HIDE_CA_PROMPT(_val_)	(_val_ &= 0xfe)		/* 隐藏CA提示信息 */
#define IS_CA_PROMPT(_val_)		((_val_ & 0x01) == 0x01)	/* 判断当前是否显示CA提示信息 */

#define SHOW_PROG_PAUSE(_val_)	(_val_ =(_val_|0x02)& 0x22)				/* 显示节目暂停信息 */
#define HIDE_PROG_PAUSE(_val_)  (_val_ &= 0xfd)		/* 隐藏节目暂停信息 */
#define IS_PROG_PAUSE(_val_)	((_val_ & 0x02) == 0x02)	/* 判断当前是否显示节目暂停信息 */

#define SHOW_NO_PROG(_val_)		(_val_ =(_val_|0x04)&0x0c)				/* 显示没有节目信息 */
#define HIDE_NO_PROG(_val_)		(_val_ &= 0xfb)				/* 隐藏没有节目信息 */
#define IS_NO_PROG(_val_)		((_val_ & 0x04) == 0x04)	/* 判断当前是否显示没有节目信息 */

#define SHOW_NO_SIGNAL(_val_)	(_val_ =(_val_|0x08)&0x0c)				/* 显示无信号信息 */
#define HIDE_NO_SIGNAL(_val_)	(_val_ &= 0xf7)		/* 不显示无信号信息 */
#define IS_NO_SIGNAL(_val_)		((_val_ & 0x08) == 0x08)	/* 判断当前是否显示无信号信息 */

#define PLAY_CHN(_val_)			(_val_ =(_val_|0x10)& 0x13)				/* 正在播台中 */
//#define NOT_PLAY_CHN(_val_)		(_val_ &= 0xef)		/* 当前没有在播台 */
#define IS_PLAY_CHN(_val_)		((_val_ & 0x10) == 0x10)	/* 判断当前是否在播台 */

#define SHOW_BLACK(_val_)		(_val_ =(_val_|0x20)& 0x27)		/* 显示黑屏 */
//#define NOT_BLACK(_val_)		(_val_ &= 0xdf)		/* 不显示黑屏 */
#define IS_SHOW_BLACK(_val_)	((_val_ & 0x20) == 0x20)	/* 判断当前是否黑屏 */


#define HIDE_PROMPT(_val_)    (_val_ &= 0xf8) /* 隐藏CA提示信息、节目暂停信息、没有节目信息 */


//隐藏所有提示信息
void UI_APP_API_HidePrompt(void)
{
	UI_OSD_API_ShowPrompt (RS_Null, 0);
	HIDE_PROMPT(bShowPrompt);
}

//隐藏所有提示信息
void UI_APP_API_HideNoSignal(void)
{
	HIDE_NO_SIGNAL(bShowPrompt);
}

//隐藏ca信息
void UI_APP_API_HideCaPrompt(void)
{
	UI_OSD_API_ShowPrompt(RS_Null, 0);
	HIDE_CA_PROMPT(bShowPrompt);
}
//隐藏无节目信息
void UI_APP_API_HideNoProg()
{
	UI_OSD_API_ShowPrompt(RS_Null, 0);
	HIDE_NO_PROG(bShowPrompt);
}

//隐藏节目暂停信息
void UI_APP_API_HideProgPause()
{
	UI_OSD_API_ShowPrompt(RS_Null, 0);
	HIDE_PROG_PAUSE(bShowPrompt);
}

//显示节目暂停信息

void UI_APP_API_ShowChannelPause(void)
{
	UI_OSD_API_ShowPrompt(RS_ProgramPause, 1);
	SHOW_PROG_PAUSE (bShowPrompt);
}

//显示无信号节目信息
void UI_APP_API_ShowNoProg(void)
{
	UI_OSD_API_ShowPrompt(RS_NoProg, 1);
	SHOW_NO_PROG (bShowPrompt);
}

//显示信号中断logo
void UI_APP_API_ShowNoSignal(void)
{
	APP_Com_API_ShowBmp(2);//显示信号中断
	SHOW_NO_SIGNAL(bShowPrompt);
}

//判断是否显示无节目
BU08 UI_APP_API_bShowNoProg(void)
{
	printf("bShowPrompt = %d\n", bShowPrompt);
	if (IS_NO_PROG(bShowPrompt))
		return 1;
	else return 0;
}

BU08 UI_APP_API_bShowPrompt(void)	//目前是否显示提示信息
{
	if(IS_CA_PROMPT (bShowPrompt)||IS_PROG_PAUSE (bShowPrompt)||IS_NO_PROG(bShowPrompt) || bShowHDPrompt)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
BU08 UI_APP_API_bShowNoSignal(void)	//目前是否显示 无信号
{
	return IS_NO_SIGNAL(bShowPrompt);
}

void UI_APP_API_ShowHDPrompt()
{
	ChnDataBase_ChnInfo_S chnInfo;

	if (bShowHDPrompt == 0)
	{
		APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &chnInfo);
		if (strstr(chnInfo.m_ChnName, "高清") != NULL || strstr(chnInfo.m_ChnName, "HD") != NULL)
		{
			bShowHDPrompt = 1;
			//APP_Com_API_ShowBmp(0);
			APP_Com_API_StopProg(1);
			UI_OSD_API_ShowPrompt(RS_HDProgram, 1);
		}
	}

}


void UI_APP_API_HideHDPrompt()
{
	ChnDataBase_ChnInfo_S chnInfo;

	if (bShowHDPrompt == 1)
	{
		bShowHDPrompt = 0;
		UI_OSD_API_ShowPrompt(RS_Null, 0);
	}

}
//显示CA信息
void UI_APP_API_ShowCaPrompt(BU32 messageNo)
{
	BU32 lang;

	SHOW_CA_PROMPT(bShowPrompt);

#if defined (DVT_CA)
	if(messageNo <= 21)
	{
		UI_OSD_API_ShowPrompt(RS_Rating_too_low+messageNo,1);
	}
#elif defined (CD_CA)
	if(messageNo <= 21)
	{
		UI_OSD_API_ShowPrompt(RS_CANCEL_TYPE+messageNo,1);
	}
#endif
}

void UI_APP_API_ShowChnLock(void)
{
	GUI_ENC_API_SwitchState(ID_DLG_ChnLock,1);
}
void UI_APP_API_ShowNoUsbRec(void)
{
	UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo, WDT_STA_SV_STRING,(BU32)"未插入USB,无法录制", 0);
	GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
}
void UI_APP_API_ShowReserveTimeOut(void)
{
	GUI_ENC_API_SwitchState(ID_DLG_TimeOut, 1);
	APP_Printf("bfirst = %d\n", bfirst);
	if (bfirst == 0)
	{
		bfirst = 1;
		bGuiTimerSuspend = FYF_FALSE;
		GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	}
}



void UI_APP_SaveMessage(BU32 message,BU32 wParam,BU32 lParam)
{
	BU32 index;

	index=0xffffffff;
	switch(message)
	{
	case WM_SIGNAL_UNLOCK:
		index = MESSAGE_INDEX_NOSIGNAL;
		break;
	case WM_SIGNAL_LOCK:
		index = MESSAGE_INDEX_PLAY;
		break;
	case WM_SIGNAL_STOP:
		index = MESSAGE_INDEX_PAUSE;
		break;
	case WM_SHOWCAPROMPT:
		index = MESSAGE_INDEX_CA;
		break;
	case WM_SHOWNOPROG:
		index = MESSAGE_INDEX_NOPROG;
		break;
	}

	if(0xffffffff != index)
	{
		g_SaveMessage[index].message =message;
		g_SaveMessage[index].wParam = wParam ;
		g_SaveMessage[index].lParam = lParam ;
	}
}

void UI_APP_ClearMessage(BU32 message,BU32 wParam,BU32 lParam)
{
	BU32 index;

	index=0xffffffff;
	switch(message)
	{
	case WM_SIGNAL_UNLOCK:
		index = MESSAGE_INDEX_NOSIGNAL;
		break;
	case WM_SIGNAL_LOCK:
		index = MESSAGE_INDEX_PLAY;
		break;
	case WM_SIGNAL_STOP:
		index = MESSAGE_INDEX_PAUSE;
		break;
	case WM_SHOWCAPROMPT:
		index = MESSAGE_INDEX_CA;
		break;
	case WM_SHOWNOPROG:
		index = MESSAGE_INDEX_NOPROG;
		break;
	}

	if(0xffffffff != index)
	{
		g_SaveMessage[index].message =0;
		g_SaveMessage[index].wParam = 0;
		g_SaveMessage[index].lParam = 0;
	}
}

void UI_APP_DispatchMessage(void)
{
	BU32 index;

	for(index = 0;index<6;index++)
	{
		if(g_SaveMessage[index].message)
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, g_SaveMessage[index].message, g_SaveMessage[index].wParam, g_SaveMessage[index].lParam);	
			UI_APP_ClearMessage (g_SaveMessage[index].message, 0, 0);
			break;
		}
	}
}

/*------------------------------------
显示系统时间
-------------------------------------*/
void SYS_IPI_ShowSysTime(BU32 STA_ID)
{
	APP_Date_S now;
	BU32 language;
	APP_SYS_API_GetLocalDateAndTime(&now);
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
	#if 0
	if(APP_ChnDatabase_API_GetCurTypeChnNum() != 0)
	{
		if(language == APP_SYS_LANGUAGE_CHINESE_e)
		{
			g_sys_ppStr = DicData;
			sprintf(g_sys_sysTime,"%04d年%02d月%02d日 %s %02d:%02d",now.m_year,now.m_month,now.m_day,g_sys_ppStr[RS_Monday+now.m_weekday-1],now.m_hour,now.m_min);	
		}
		else
		{
			g_sys_ppStr = DicDataEnglish;
			sprintf(g_sys_sysTime,"%04d-%02d-%02d %s %02d:%02d",now.m_year,now.m_month,now.m_day,g_sys_ppStr[RS_Monday+now.m_weekday-1],now.m_hour,now.m_min);	
		}
	}
	else
		g_sys_sysTime[0] = '\0';
	#endif
	#if 1
	if(language == APP_SYS_LANGUAGE_CHINESE_e)
	{
		g_sys_ppStr = DicData;
		sprintf(g_sys_sysTime,"%04d年%02d月%02d日 %s %02d:%02d",now.m_year,now.m_month,now.m_day,g_sys_ppStr[RS_Monday1+now.m_weekday-1],now.m_hour,now.m_min);	
	}
	else
	{
		g_sys_ppStr = DicDataEnglish;
		sprintf(g_sys_sysTime,"%04d-%02d-%02d %s %02d:%02d",now.m_year,now.m_month,now.m_day,g_sys_ppStr[RS_Monday1+now.m_weekday-1],now.m_hour,now.m_min);	
	}
	#endif
	GUI_ENC_API_SetValue(STA_ID,WDT_STA_SV_STRING,(BU32)g_sys_sysTime,0);
}

void UI_APP_SYS_ShowSysTime(void)
{
/*
ID_STA_Time_ProgManager
ID_STA_Time_SysSet
ID_STA_Time_Default
ID_STA_Time_StbInfo
ID_STA_Time_PasswordSet
ID_STA_Time_LanguageSet
ID_STA_Time_TimeZoneSet
ID_STA_Time_Russia
ID_STA_Time_Snake
ID_STA_Time_Box
ID_STA_Time_ProgList
ID_STA_Time_SortProgram
ID_STA_Time_SearchProg
ID_STA_Time_MainFreqSet
ID_STA_Time_ChannelEdit
ID_STA_Time_BroadcastList
ID_STA_Time_EPG
ID_STA_Time_ManualSearchSet
ID_STA_Time_Nvod
*/
	SYS_IPI_ShowSysTime(ID_STA_Time_ProgManager);	
	SYS_IPI_ShowSysTime(ID_STA_Time_SysSet );	
	SYS_IPI_ShowSysTime(ID_STA_Time_Default);	
	SYS_IPI_ShowSysTime(ID_STA_Time_StbInfo );	
	SYS_IPI_ShowSysTime(ID_STA_Time_PasswordSet);	
	SYS_IPI_ShowSysTime(ID_STA_Time_LanguageSet );
	SYS_IPI_ShowSysTime(ID_STA_Time_NetSet );
	SYS_IPI_ShowSysTime(ID_STA_Time_TimeZoneSet );	
	SYS_IPI_ShowSysTime(ID_STA_Time_Russia );	
	SYS_IPI_ShowSysTime(ID_STA_Time_Snake );	
	SYS_IPI_ShowSysTime(ID_STA_Time_Box );	
	SYS_IPI_ShowSysTime(ID_STA_Time_ProgList );	
	SYS_IPI_ShowSysTime(ID_STA_Time_SortProgram );	
	SYS_IPI_ShowSysTime(ID_STA_Time_Search );	
	SYS_IPI_ShowSysTime(ID_STA_Time_MainFreqSet );	
	SYS_IPI_ShowSysTime(ID_STA_Time_ChannelEdit );
	SYS_IPI_ShowSysTime(ID_STA_Time_BroadcastList );	
	SYS_IPI_ShowSysTime(ID_STA_Time_EPG );
	SYS_IPI_ShowSysTime(ID_STA_Time_Ca );
	SYS_IPI_ShowSysTime(ID_STA_Time_MainMenu);
	SYS_IPI_ShowSysTime(ID_STA_Time_GetServicer );
	SYS_IPI_ShowSysTime(ID_STA_Time_Rating );
	SYS_IPI_ShowSysTime(ID_STA_Time_PinSet );
	SYS_IPI_ShowSysTime(ID_STA_Time_WorkTime );
	SYS_IPI_ShowSysTime(ID_STA_Time_MasterSlaveManage );
	SYS_IPI_ShowSysTime(ID_STA_Time_ManualSearchSet );
	SYS_IPI_ShowSysTime(ID_STA_Time_Nvod );	
    SYS_IPI_ShowSysTime(ID_STA_Time_PvrList);
	SYS_IPI_ShowSysTime(ID_STA_Time_Game );
	SYS_IPI_ShowSysTime(ID_STA_Time_UsbManager );	
	SYS_IPI_ShowSysTime(ID_STA_Time_MasterSlave );	
	SYS_IPI_ShowSysTime(ID_STA_Time_ReadMail );	
	SYS_IPI_ShowSysTime(ID_STA_Time1_Mail );
	SYS_IPI_ShowSysTime(ID_STA_Time_ICInfo );
	SYS_IPI_ShowSysTime(ID_STA_Time_EntitleInfo );
	SYS_IPI_ShowSysTime(ID_STA_Time_ePurse );
	SYS_IPI_ShowSysTime(ID_STA_Time_IPPInfo );
	SYS_IPI_ShowSysTime(ID_STA_Time_ACList );
	SYS_IPI_ShowSysTime(ID_STA_Time_DetitleChkNums );
	SYS_IPI_ShowSysTime(ID_STA_Time_CardPair );
	SYS_IPI_ShowSysTime(ID_STA_Time_RegionSet );
	SYS_IPI_ShowSysTime(ID_STA_Time_OperationGuide);
	SYS_IPI_ShowSysTime(ID_STA_Time_OutPutSet);
	SYS_IPI_ShowSysTime(ID_STA_Time_DisplaySet);
	SYS_IPI_ShowSysTime(ID_STA_Time_ChannelSet);
	SYS_IPI_ShowSysTime(ID_STA_Time_ParentControl);
	SYS_IPI_ShowSysTime(ID_STA_Time_DoubleHelp);
}

BU32 UI_APP_SignalNotify(BU32 para1,BU32 para2)
{
	switch(para2)
	{
		case APP_SIGNAL_LOCK:
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_SIGNAL_LOCK, 0, 0);
			//libc_printf ("ssssssssssss-------UI_APP_SignalNotify----------------信号锁定\n");
			break;
		case APP_SIGNAL_UNLOCK:
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_SIGNAL_UNLOCK, 0, 0);
			//libc_printf ("ssssssssssss-------UI_APP_SignalNotify----------------信号未锁定\n");
			break;
		case APP_SIGNAL_STOP:
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_SIGNAL_STOP, 0, 0);
			//libc_printf ("ssssssssssss-------UI_APP_SignalNotify----------------信号中断\n");
			break;
		case APP_SIGNAL_PLAY:
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_HIDE_PAUSE, 0, 0);
			//printf ("ssssssssssss-------UI_APP_SignalNotify----------------信号锁定，节目暂停\n");
			break;
		case APP_SIGNAL_PLAYCHANGE:
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_SIGNAL_PLAYCHANGE, 0, 0);
			//libc_printf ("ssssssssssss-------UI_APP_SignalNotify----------------信号锁定，节目播放\n");			
			break;
		default:
			break;
		  
	}  
}

void UI_APP_SetIcoToCn(void)
{

/*
ID_STA_ExitIco_ProManager
ID_STA_ExitIco_SysSet
ID_STA_ExitIco_StbInfo
ID_STA_ExitIco_PasswordSet
ID_STA_ExitIco_LanguageSet
ID_STA_ExitIco_TimeZoneSet
ID_STA_ExitIco_SysUpdate
ID_STA_ExitIco_ChannelList
ID_STA_ExitIco_SortProgram
ID_STA_ExitIco_SearchProg
ID_STA_ExitIco_MainFreqSet
ID_STA_ExitIco_ChannelEdit
ID_STA_ExitIco_ManualSearchSet
ID_STAExit_Epg
ID_STA_ExitIco_BroadcastList

ID_STA_OkIco_ProgManager
ID_STA_OkIco_SysSet
ID_STA_OKIco_SysUpdate
ID_STA_OkIco_Game
ID_STA_OKIco_Snake
ID_STA_OkIco_ChannelList
ID_STA_OkIco_SortProgram
ID_STA_OkIco_ChannelEdit
ID_STA_OkIco_ManualSearchSet
ID_STAOK_Epg
ID_STA_OkIco_BroadcastList
*/
	g_sys_ppStr = DicData;
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ProManager,WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_SysSet,      WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_StbInfo,         WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_PasswordSet,    WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_LanguageSet,        WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_TimeZoneSet,       WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_SysUpdate, WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ProgList,    WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_SortProgram,           WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_Search,    WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_MainFreqSet,     WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ChannelEdit,   WDT_STA_SV_STYLE, STYLE_ExitCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ManualSearchSet,   WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_BroadcastList,    WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);

	GUI_ENC_API_SetValue(ID_STA_OkIco_ProgManager,       WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_SysSet,        WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OKIco_SysUpdate,   WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_Game,          WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OKIco_Snake,     WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ProgList,        WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_SortProgram,    WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ChannelEdit,    WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ManualSearchSet,    WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIcon_EPG,    WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_BroadcastList,    WDT_STA_SV_STYLE, STYLE_OkCn_W46_H28, 0);

}

void UI_APP_SetIcoToEn(void)
{
	g_sys_ppStr = DicDataEnglish;
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ProManager,WDT_STA_SV_STYLE, STYLE_ReturnEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_SysSet,      WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_StbInfo,         WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_PasswordSet,    WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_LanguageSet,        WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_TimeZoneSet,       WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_SysUpdate, WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ProgList,    WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_SortProgram,           WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_Search,    WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_MainFreqSet,     WDT_STA_SV_STYLE, STYLE_ReturnEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ChannelEdit,   WDT_STA_SV_STYLE, STYLE_ExitEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_ManualSearchSet,   WDT_STA_SV_STYLE, STYLE_ReturnEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_ExitIco_BroadcastList,    WDT_STA_SV_STYLE, STYLE_ReturnEn_W46_H28, 0);

	GUI_ENC_API_SetValue(ID_STA_OkIco_ProgManager,       WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_SysSet,        WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OKIco_SysUpdate,   WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_Game,          WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OKIco_Snake,     WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ProgList,        WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_SortProgram,    WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ChannelEdit,    WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_ManualSearchSet,    WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_OkIco_BroadcastList,        WDT_STA_SV_STYLE, STYLE_OkEn_W46_H28, 0);

}

void UI_OSD_API_ShowPrompt(BU32 rs_id, BU08 bShow)
{
	BU32 curState;
/*
ID_STA_CardState_MainMenu
ID_STA_CardState_ProgramManager
ID_STA_CardState_SysSet
ID_STA_CardState_StbInfo
ID_STA_CardState_PasswordSet
ID_STA_CardState_LanguageSet
ID_STA_CardState_TimeZoneSet
ID_STA_CardState_Game
ID_STA_CardState_Russia
ID_STA_CardState_Snake
ID_STA_CardState_Box
ID_STA_CardState_ProgList
ID_STA_CardState_SortProgram
ID_STA_CardState_MainFreqSet
ID_STA_CardState_ChannelEdit
ID_STA_CardState_BroadcastList
ID_STA_CardState_EPG
ID_STA_CardState_Ca
ID_STA_CardState_Rating
ID_STA_CardState_PinSet
ID_STA_CardState_WorkTime
*/
	UI_APP_API_SetChnInfoSaveID(rs_id);
	curState = GUI_ENC_API_GetCurState();
	if(bShow)
	{
		{
			GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, rs_id, 0);
			if(UI_APP_API_GetChnInfoCurState()!= TRACKSET_STATE)
			{
				GUI_ENC_API_SetVisible(ID_STA_TextBk_ChnInfo,WS_VISIBLE);	
				GUI_ENC_API_SetVisible(ID_STA_Text_ChnInfo,WS_VISIBLE);	
				GUI_ENC_API_SetVisible(ID_STA_Title_ChnInfo,WS_VISIBLE);	
				GUI_ENC_API_Update(ID_STA_TextBk_ChnInfo ,1);
				GUI_ENC_API_Update(ID_STA_Text_ChnInfo ,1);
				GUI_ENC_API_Update(ID_STA_Title_ChnInfo ,1);
				
			}
		}

		GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Game, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgList, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,      WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_EPG,      WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Russia,   WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Snake,    WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Box,      WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SysSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Ca, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Rating, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PinSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_NetSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Effects, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp, WDT_STA_SV_BGC, COLOR_INFOBK, 0);	
		GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Game, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgList, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,      WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_EPG,      WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Russia,   WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Snake,    WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Box,      WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SysSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Ca, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Rating, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PinSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_NetSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Effects, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl, WDT_STA_SV_DATAID, rs_id, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp, WDT_STA_SV_DATAID, rs_id, 0);
		if (nvodPlayFlag)
		{
			GUI_ENC_API_SetValue(ID_STA_CardState_Nvod, WDT_STA_SV_BGC, COLOR_INFOBK, 0);
			GUI_ENC_API_SetValue(ID_STA_CardState_Nvod, WDT_STA_SV_DATAID, rs_id, 0);
		}
	}
	else
	{

#if defined (CD_CA)
		if((curState == ID_DLG_ChnInfo)&&(UI_APP_API_GetChnInfoCurState()!= TRACKSET_STATE)&&(CDSTBCA_API_IppDlgShow () == FYF_FALSE))
#else
		if((curState == ID_DLG_ChnInfo)&&(UI_APP_API_GetChnInfoCurState()!= TRACKSET_STATE))
#endif
		{
			//GUI_API_FillRect(GUI_API_GetDesktopWindow(), 145, 172, 440, 140, COLOR_TRANS);
			GUI_ENC_API_Update(ID_STA_HideTextBk_ChnInfo ,1);	
		}

		GUI_ENC_API_SetVisible(ID_STA_TextBk_ChnInfo,WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_STA_Text_ChnInfo,WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_STA_Title_ChnInfo,WS_DISABLE);	
#if 0
		if (nvodPlayFlag)
		{
			NvodPlay_IPI_ClearScreen();
			GUI_ENC_API_SetVisible(ID_STA_TextBk_NvodPlay,WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_STA_Text_NvodPlay,WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_STA_Title_NvodPlay,WS_DISABLE);
		}
#endif

		GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Game, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgList, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,      WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_EPG,      WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Russia,   WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Snake,    WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Box,      WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SysSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Ca, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Rating, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PinSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_NetSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Effects, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl, WDT_STA_SV_BGC, COLOR_TRANS, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp, WDT_STA_SV_BGC, COLOR_TRANS, 0);	
		GUI_ENC_API_SetValue(ID_STA_CardState_MainMenu, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Game, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgList, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_BroadcastList,      WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_EPG,      WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Russia,   WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Snake,    WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Box,      WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SortProgram, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ProgramManager, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_SysSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PasswordSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_LanguageSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_TimeZoneSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_MainFreqSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelEdit, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Ca, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Rating, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_PinSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_WorkTime, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_NetSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_RegionSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_Effects, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_OutPutSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DisplaySet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ChannelSet, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_ParentControl, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SetValue(ID_STA_CardState_DoubleHelp, WDT_STA_SV_DATAID, RS_Null, 0);
		if (nvodPlayFlag)
		{
			GUI_ENC_API_SetValue(ID_STA_CardState_Nvod, WDT_STA_SV_BGC, COLOR_TRANS, 0);
			GUI_ENC_API_SetValue(ID_STA_CardState_Nvod, WDT_STA_SV_DATAID, RS_Null, 0);
		}
	}
}

BU32 gui_msg_handle(BU32 ID,BU32 message,BU32 wParam,BU32 lParam)
{
	static BU32 unlockCount = 0, pauseCount = 0;
	static BU32 bSaveFlash = 0, bCardCommunication = 0, bIPP = 0;
	BU32 curState;

	if(!bSaveFlash && !bCardCommunication )
	{

		if(message == WM_SIGNAL_LOCK || message == WM_SIGNAL_STOP || message == WM_HIDE_PAUSE)
		{
			unlockCount = 0;
		}

	    if(message == WM_HIDE_PAUSE)
		{
			pauseCount = 0;
		}

	}
	//APP_Printf("unlockCount = %d\n", unlockCount);
	

//	APP_PRINT_DEBUG("-------gui_msg_handle----------------get message : %d\n", message-WM_USER);

	switch(message)
	{
	case WM_SIGNAL_PLAYCHANGE:
	/*	curState =GUI_ENC_API_GetCurState();
		if(curState==ID_DLG_Search || curState == ID_DLG_ManualSearchSet || curState == ID_DLG_SearchInfo
			|| curState == ID_DLG_PictureShow || curState == ID_DLG_PhotoFrame || curState == ID_DLG_TimeShift)
			return;
	*/
		if (APP_Com_API_GetSignal_Dectect() == 0)
			return;
		pauseCount = 0;
		unlockCount = 0;
		if (IS_NO_SIGNAL(bShowPrompt))
		{
			HIDE_NO_SIGNAL(bShowPrompt);
		}

		if (IS_PROG_PAUSE(bShowPrompt))
		{
			UI_APP_API_HideProgPause();
		}
		break;
		
	case WM_SIGNAL_UNLOCK://信号中断
		curState =GUI_ENC_API_GetCurState();
		if(curState==ID_DLG_Search || curState == ID_DLG_ManualSearchSet || curState == ID_DLG_SearchInfo
			|| curState == ID_DLG_PictureShow || curState == ID_DLG_PictureShow || curState == ID_DLG_TimeShift)
			return;
		if (APP_Com_API_GetSignal_Dectect() == 0)
			return;
		if (++unlockCount == 30)
		{
			if (IS_CA_PROMPT(bShowPrompt))
			{
				UI_APP_API_HideCaPrompt();
				//HIDE_CA_PROMPT(bShowPrompt);
			}
			else if(IS_PROG_PAUSE (bShowPrompt))
			{
				UI_APP_API_HideProgPause();
				pauseCount = 0;
				//UI_APP_ClearMessage(WM_SHOWCAPROMPT,0,0);

			}
			UI_APP_API_ShowNoSignal();

		}		
		break;
	case WM_SIGNAL_LOCK://信号锁定
		if (APP_Com_API_GetSignal_Dectect() == 0)
			return;
		if (IS_NO_SIGNAL(bShowPrompt))
		{
			if(IS_NO_PROG(bShowPrompt))
			{
				APP_Com_API_ShowOtherBmp(0);//黑屏
			}
			HIDE_NO_SIGNAL(bShowPrompt);
		//	UI_APP_API_SetLockServiceId(0);

			if (UI_APP_API_GetCurChnbPlay() == 0)
			{
				APP_Com_API_SetEcmNULL();
				if(1 == nvodPlayFlag)
				{
					UI_APP_API_PlayByServiceID(nvodPlayServiceId, "nuod");
				}
				else
				{
					UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 0);
				}
				
			}
			else//处于节目锁定状态 黑屏操作
			{
				APP_Com_API_StopProg(1);
			}
		}
	
#if 0
		if(IS_NO_PROG(bShowPrompt))
		{
			APP_Com_API_ShowOtherBmp(0);//黑屏
			HIDE_NO_SIGNAL(bShowPrompt);
			return 0;
		}
		
		if(IS_PLAY_CHN(bShowPrompt))
		{

		}
		else
		{
			if(IS_NO_SIGNAL(bShowPrompt))
			{
				HIDE_NO_SIGNAL(bShowPrompt);
				if(APP_ChnDatabase_API_GetCurChnType()==APP_CHN_AV_TYPE_RD_e)
				{
					//libc_printf ("\ngui_msg_handle                  *****--显示广播LOGO-s\n");
					APP_Com_API_ShowBmp(1);//广播LOGO
				}
				else
				{
					//libc_printf ("gui_msg_handle---------------------****--显示-黑屏-s\n");
					APP_Com_API_ShowBmp(0);//黑屏
				}
			}
			if(UI_APP_API_GetCurChnbPlay())
			{
				//libc_printf ("\ngui_msg_handle------------WM_SIGNAL_LOCK*****--播台-s\n");
				APP_Com_API_StopProg(0);
				if(IS_CA_PROMPT(bShowPrompt))
				{
					//libc_printf ("gui_msg_handle              正显示---CA提示-s\n");
				}
				else
				{
					FYF_Ecm_Info_s serviceInfo;
					memset(&serviceInfo, 0, sizeof(FYF_Ecm_Info_s));
					FYF_API_SetCurEcmInfo(&serviceInfo);
				}
					if(1 == nvodPlayFlag)
					{
						APP_Com_API_PlayNvodChn(nvodPlayServiceId);
					}
					else
					{
						APP_Com_API_PlayChn(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO (), -1);
					}
				UI_APP_API_SetVolume();
				PLAY_CHN(bShowPrompt);
	
			}
			else if(APP_ChnDatabase_API_GetCurChnType()==APP_CHN_AV_TYPE_TV_e)
			{
				APP_Com_API_ShowBmp(0);//黑屏
			}

		}
#endif
		break;		
	case WM_SIGNAL_STOP://信号锁定，节目暂停
	
		if (APP_Com_API_GetSignal_Dectect() == 0)
		{
			return;
		}
		//已经有显示信息了
		if (UI_APP_API_bShowPrompt())
		{
			return 0;
		}
		curState =GUI_ENC_API_GetCurState();
		if(curState==ID_DLG_Search || curState == ID_DLG_ManualSearchSet || curState == ID_DLG_SearchInfo
		|| curState == ID_DLG_PictureShow || curState == ID_DLG_PictureShow)return 0;

		if (++pauseCount == 25)
		{

			if(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
			{
				APP_Printf("\ngui_msg_handle----------------------****--显示--广播LOGO-s\n, pauseCount = %d\n", pauseCount);
				APP_Com_API_ShowBmp(0);//黑屏;//广播LOGO
			}
	
			if (FYF_DVT_GetCasOther())
			{
				printf ("gui_msg_handle              正显示---CA提示,或是----其它CA台s\n");
			}
			else
			{
				#if defined (CD_CA)
				if(CDSTBCA_API_IppDlgShow () == FYF_TRUE)
				{
					CDSTBCA_API_ExitBuy ();
				}
				#endif
				UI_APP_API_ShowChannelPause ();
				//UI_APP_SaveMessage(message,wParam,lParam);
			}
		}
#if 0
		if(pauseCount<2 && lParam==1)/*加锁,预约时间到返回*/
		{
			UI_APP_API_ShowChannelPause ();
		}
		else if(pauseCount==30)/*30 次大约为6 秒*/
		{
			if(APP_ChnDatabase_API_GetCurChnType()==APP_CHN_AV_TYPE_RD_e)
			{
				//libc_printf ("\ngui_msg_handle----------------------****--显示--广播LOGO-s\n");
				APP_Com_API_ShowBmp(1);//广播LOGO
			}
			else
			{
				//libc_printf ("gui_msg_handle---------------------****--显示-黑屏-s\n");
				APP_Com_API_ShowBmp(0);//黑屏
			}
			
			if(FYF_DVT_GetCasOther()/* || UI_APP_IsLock()*/)
			{
				//libc_printf ("gui_msg_handle              正显示---CA提示,或是----其它CA台s\n");
			}
			else
			{
				if(IS_CA_PROMPT(bShowPrompt))
				{
				//	UI_APP_CA_HidePromptEx ();
				//	HIDE_CA_PROMPT(bShowPrompt);
				//	APP_Com_API_PlayChn (APP_ChnDatabase_API_GetCurChnNO ());
					return 0;
				}
				//libc_printf ("\ngui_msg_handle                 *****--显示节目暂停-s\n");
				#ifdef CD_CA
				if(CDSTBCA_API_IppDlgShow () == FYF_TRUE)
				{
					CDSTBCA_API_ExitBuy ();
				}
				#endif
				UI_APP_API_ShowChannelPause ();
				UI_APP_SaveMessage(message,wParam,lParam);

			}
		}
#endif
		break;
	case WM_HIDE_PAUSE://信号锁定，节目播放
		if (APP_Com_API_GetSignal_Dectect() == 0)
		return;
		if(IS_PROG_PAUSE (bShowPrompt))
		{
			UI_APP_API_HideProgPause();
			pauseCount = 0;
			unlockCount = 0;
		//	UI_APP_ClearMessage(WM_SHOWCAPROMPT,0,0);
		}
		break;	
	case WM_SHOWCURTIME:		
		//libc_printf("显示当前时间\n");	
		UI_APP_SYS_ShowSysTime();
		break;
	case WM_SHOWCAPROMPT:
		if(IS_CA_PROMPT(bShowPrompt))
		{
			//APP_Com_API_Printf("目前,显示CA提示信息,不能显示CA提示信息\n");
		}
		else
		{
		//	APP_Com_API_StopProg (0);
#ifdef ENABLE_BROADCAST_IPANEL
			if (Inthird_GetFlag()==FYF_FALSE)
#endif	
			{
			
				if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
					APP_Com_API_ShowBmp(0);//黑屏
			}		
		
			//libc_printf ("gui_msg_handle                  *****--显示--CA提示-s\n");
			if(wParam<=21)
			{
				UI_APP_API_ShowCaPrompt(wParam);
			//	UI_APP_SaveMessage(message,wParam,lParam);

			}
		}
		break;
	case WM_HIDECAPROMPT:
		//APP_Printf ("隐藏CA提示信息 bShowPrompt=%d\n",bShowPrompt);
		if(IS_CA_PROMPT (bShowPrompt))
		{
			//APP_Printf ("gui_msg_handle                  *****--隐藏--CA提示-s\n");
			UI_APP_API_HideCaPrompt();
	//		UI_APP_ClearMessage(WM_SHOWCAPROMPT,0,0);
	//		APP_Com_API_PlayChn (APP_ChnDatabase_API_GetCurChnNO ());
	//		APP_Printf("card in  notify paly time = %d\n",FYF_API_time_ms ());
		}
		else
		{
			//APP_Printf ("目前,没有显示CA提示信息,所以不能隐藏CA提示信息\n");
		}
		break;
	case WM_SHOWNOPROG:
		if(IS_NO_PROG (bShowPrompt))
		{
			//APP_Printf("目前,显示没有节目提示,所以不能显示没有节目提示\n");
		}
		else
		{
			UI_APP_API_HidePrompt();
			//libc_printf ("gui_msg_handle                  *****--显示--无节目-s\n");
			UI_APP_API_ShowNoProg ();
		}
		break;
	case WM_HIDENOPROG:
		if(IS_NO_PROG (bShowPrompt))
		{
			//libc_printf ("gui_msg_handle                  *****--隐藏--无节目-s\n");
			UI_APP_API_HideNoProg();
		}
		break;
	case WM_CHECKLOCK:
		if(UI_APP_API_bShowPrompt() && (GUI_ENC_API_GetCurState() == ID_DLG_ChnInfo))
		{
			UI_APP_API_HidePrompt();
		}
		else
		{
			//UI_APP_ClearMessage(WM_SHOWCAPROMPT,0,0);
		}
		
		UI_APP_API_ShowChnLock();

		break;
	case WM_RESERVEPROMPT:
		if(UI_APP_API_bShowPrompt() && (GUI_ENC_API_GetCurState() == ID_DLG_ChnInfo))
		{
			UI_APP_API_HidePrompt();
		}
		else
		{
	//		UI_APP_ClearMessage(WM_SHOWCAPROMPT,0,0);
		}

		UI_APP_API_ShowReserveTimeOut();
		break;		
	case WM_START_SAVEFLASH:
		bSaveFlash = 1;
		pauseCount = 0;
		unlockCount =0;
		break;
	case WM_END_SAVEFLASH:
		bSaveFlash = 0;
		break;
	case WM_SHOW_CUR_EPG:
		//EPG_IPI_ShowCurEventPageEx ();
		break;
	case WM_CHECKLOCK_END:
//		UI_APP_DispatchMessage ();
		break;
	case WM_RESERVEPROMPT_END:
		UI_APP_DispatchMessage ();
		break;
	case WM_FULLSCREENCHARGEPROG:
		FullScreen_IPI_ChangeProg();
		break;
	case WM_SHOWSYSTIME:
		UI_APP_SYS_ShowSysTime();
		break;
	case WM_ISRESERVEONTIME:
		APP_API_IsReserveOnTime();
		break;
	case WM_GETPATONTIME:		
		if (APP_Search_Get_OTA_Status() == 1)
		{
			APP_API_TimeGetPat();
		}
		break;
	case WM_GETPMTONTIME:
		if (APP_Search_Get_OTA_Status() == 1)
		{
			APP_API_TimeGetPmt();
		}
		break;
	case WM_GETSDTONTIME:
		if (APP_Search_Get_OTA_Status() == 1)
		{
			APP_API_TimeGetSdt();
		}
		break;
	case WM_GETNITOTAONTIME:
		if (APP_Search_Get_OTA_Status() == 1)
		{
			APP_API_TimeGetNitAndOta();
		}
		break;
	case WM_SIGNALDETECT:
		APP_Com_API_SignalDetect();
		break;
	case WM_START_CARD:
		bCardCommunication = 1;
		pauseCount = 0;
		unlockCount =0;
		break;
	case WM_END_CARD:
		bCardCommunication = 0;
		break;
	case WM_NIT_VER_UPDATED:
		#if defined (CD_CA)
		if(CDSTBCA_API_IppDlgShow () == FYF_TRUE)
		{
			CDSTBCA_API_ExitBuy ();
		}
		#endif
		APP_PRINT_DEBUG("WM_NIT_VER_UPDATED   ==== \n");
		UI_APP_SetNitVerChangedUI();
		break;
	case WM_OTA_DESC_DETECTED:
		#if defined (CD_CA)
		if(CDSTBCA_API_IppDlgShow () == FYF_TRUE)
		{
			CDSTBCA_API_ExitBuy ();
		}
		#endif
		APP_PRINT_DEBUG("UI_APP_SetOTAUI   ==== \n");
		UI_APP_SetOTAUI();
		break;
#if defined (CD_CA)
	case WM_IPP_SHOW:
		bIPP = 1;
		pauseCount = 0;
		unlockCount =0;
		FullScreen_IPI_SetCurState_Ipp ();	
		break;
	case WM_IPP_HIDE:
		bIPP = 0;
		FullScreen_IPI_SetCurState_InfoBar ();		
		break;
	case WM_IPP_NOTIFY:
		FullScreen_IPI_SetCurState_IppNotify ();
		break;
	case WM_PROGRESS_STRIP:
		UI_APP_API_ShowProgressStripEx ( wParam, lParam);
		break;
	case WM_REQUEST_FEEDING:
		UI_APP_CAosd_RequestFeedingEx ( wParam, lParam);
		break;
	case WM_REQUEST_FEEDING_END:
		UI_APP_CAosd_EndFeedingEx ();
		break;
#endif
	#if 0
	case WM_OSD_SHOW_START:
		UI_APP_API_ShowOsd (wParam);
		break;
	case WM_OSD_SHOW_END:
		UI_APP_API_HideOsd (wParam);
		break;
		#endif
#if defined(USE_FZVOD) || defined(USE_ENREACH)		
	case WM_SHOWRELOGO:
		printf("show===========\n");
		GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_Relogo ,1);
		break;

	case WM_HIDERELOGO:
		
		printf("hide===========\n");
		GUI_API_FillRect(GUI_API_GetDesktopWindow(), 40, 526, 30, 30, COLOR_TRANS);
		break;
#endif

#ifdef ENABLE_USB
	case WM_SHOWNOUSBREC:
		UI_APP_API_ShowNoUsbRec();
		break;
	case WM_DETECT_USB_INSERT:
		UI_APP_API_USBInsertDispose(wParam);
		break;
	case WM_DETECT_USB_EVULSION:
		UI_APP_API_USBEvulsionDispose(wParam);
		break;
	case WM_REC_DISK_Full:
		UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_DISK_FULL);
		GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo, WDT_STA_SV_STRING,(BU32)"磁盘已满", 0);
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
		break;
#endif
	case WM_CHN_CHANGE:
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		break;
	case WM_KEY_MESSAGE:
		APP_PRINT_DEBUG("======WM_KEY_MESSAGE=====");
		UI_APP_API_VoiceControl(wParam);
		break;
	case WM_Show_HDPROMPT:
		UI_APP_API_ShowHDPrompt();
		break;
	case WM_Hide_HDPROMPT:
		UI_APP_API_HideHDPrompt();
		break;
	case WM_Show_EnrichVodError:
		UI_APP_FULLTOTVOD_Dispose(wParam);
		break;
	default:
		break;
	}	
	
	
	return 1;
}

