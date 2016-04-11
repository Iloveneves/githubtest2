#include "../tpmiddle/include/app_common.h"
#include "osd_version.h"
#if 0
#if OSD_VERSION == DEMO
#include "ui_demo/osd/include/resource.h"
#elif OSD_VERSION == LONGYAN
#include "longyan/osd/include/resource.h"
#endif
#endif
#include "ui_demo/osd/include/resource.h"
BU08 g_ui_app_str[13][80];

//显示，更新当前，下一个节目预告
static BU08 g_epg_EventName[2][60]=
{
	"","",
};

static BU32 g_chnList_preState = 0;
static BU32 g_lock_serviceIdTemp = 0;
static BU32 g_lock_curChnbPlay = 0;
static BU32 g_lock_bUnlock = 0;
static BU32 g_lock_curState = 0;
static BU32 g_progid_old = 0;
static BU08 g_sys_bSaveFlash = 0;
static BU32 g_fullscreen_curState = NULL_STATE;
static BU32 g_fullscreen_saveid = RS_Null;
static BU32 g_cur_prog_id = 0;
static BU08 g_epg_needSave=0;
static BU32 g_epg_preState = 0;
static BU32 g_reserve_preState = 0;
static BU32 g_sys_defaultToState=DLG_MAINMENU; 
static BU32 g_pinNote_preState = DLG_CA;
static BU32 g_brdcastlist_preState = DLG_MAINMENU;
static BU32 g_brdcastlist_curno = 0;
static BU08 g_ca_mail_full = 0;
static BU32 g_del_fileno = 0;
static BU32 g_mail_preState = ID_DLG_ChnInfo;
static BU32 g_SortProgram_preState = 0;
static BU32 g_chnEdit_preState = 0;
static BU32 g_doublehelp_preState = 0;
//nvod
static BU32 g_nvod_eventIndex = 0;
static BU32 g_conflictIndex = 0xffffffff;

//timeshift
static BU32 g_timeShift_curState = TIMESHIFT_NULL_STATE;

/*----------------------------------------------------------------------------
描述：显示当前、下一个节目预告
-----------------------------------------------------------------------------*/
void UI_APP_API_ShowCurAndNextEPG(BU32 EventID1,BU32 EventID2,BU32 PbrID, BU32 PbrLength, BU32 Param)
{
	BU32 ret = APP_FAIL;
	Epg_Info_S curEpgInfo;
	Epg_Info_S nextEpgInfo;
	APP_Date_S now;
	BU32 nowTime;//当前时间
	BU32 curEpgStartTime, curEpgEndTime;//当前EPG事件开始与结束时间
	BU32 pbrValue;

	if (APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		g_epg_EventName[0][0] = 0;
		g_epg_EventName[1][0] = 0;
		GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
		GUI_ENC_API_SetValue(EventID1, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
		GUI_ENC_API_SetValue(EventID2, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
		return;
	}
	
	ret = APP_EPG_API_GetCurAndNextEvent(APP_ChnDatabase_API_GetCurChnNO(), &curEpgInfo, &nextEpgInfo);
	
	//APP_Com_API_Printf("UI_APP_API_ShowCurAndNextEPG %s\n%s\n",curEpgInfo.epgName,nextEpgInfo.epgName);

	if(GUI_API_Strlen(curEpgInfo.epgName) != 0)
	{
		sprintf(g_epg_EventName[0], "%02d:%02d %s",curEpgInfo.startHour,curEpgInfo.startMin,curEpgInfo.epgName);
	}
	else 
	{
		g_epg_EventName[0][0] = 0;
	}
	
	if(GUI_API_Strlen(nextEpgInfo.epgName) != 0)
	{
		sprintf(g_epg_EventName[1], "%02d:%02d %s",nextEpgInfo.startHour,nextEpgInfo.startMin,nextEpgInfo.epgName);
	}
	else 
	{
		g_epg_EventName[1][0] = 0;
	}

	if(Param == 0)
	{
		g_epg_EventName[0][26]=0;
		g_epg_EventName[1][26]=0;
	}
	else if(Param == 1)
	{
		g_epg_EventName[0][44]=0;
		g_epg_EventName[1][44]=0;
	}	
	if((ret != APP_FAIL) && (APP_SUCCESS == APP_SYS_API_GetLocalDateAndTime(&now)))
	{
		nowTime = now.m_hour*60+now.m_min;
		curEpgStartTime = curEpgInfo.startHour*60+curEpgInfo.startMin;
		curEpgEndTime = curEpgInfo.endHour*60+curEpgInfo.endMin;
		if(nowTime < curEpgStartTime)
		{
		    nowTime = nowTime + 24*60;
		}
		if(curEpgEndTime < curEpgStartTime)
		{
		    curEpgEndTime = curEpgEndTime + 24*60;
		}
	//	APP_PRINT_DEBUG("curEpgEndTime = %d, curEpgStartTime = %d",curEpgEndTime, curEpgStartTime);
		if((curEpgEndTime > curEpgStartTime)&& curEpgEndTime > 0 && curEpgEndTime > 0)
		{
			pbrValue = (nowTime-curEpgStartTime)*PbrLength/(curEpgEndTime- curEpgStartTime);
		//	APP_PRINT_DEBUG("pbrValue = %d",pbrValue);
			GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, pbrValue, 0);
			//GUI_ENC_API_Update(PbrID, 1);
		}
		else
		{
		    GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
		}
	}
	else
	{
	     GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
	}
	GUI_ENC_API_SetValue(EventID1, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
	GUI_ENC_API_SetValue(EventID2, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
}

/*----------------------------------------------------------------------------
描述：显示当前、下一个节目预告
-----------------------------------------------------------------------------*/
void UI_APP_API_ShowCurAndNextEPGWithEndTime(BU32 EventID1,BU32 EventID2, BU32 PbrID, BU32 PbrLength, BU32 Param)
{
	BU32 ret = APP_FAIL;
	Epg_Info_S curEpgInfo;
	Epg_Info_S nextEpgInfo;
	APP_Date_S now;
	BU32 nowTime;//当前时间
	BU32 curEpgStartTime, curEpgEndTime;//当前EPG事件开始与结束时间
	BU32 pbrValue;

	if (APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		g_epg_EventName[0][0] = 0;
		g_epg_EventName[1][0] = 0;
	    GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
		GUI_ENC_API_SetValue(EventID1, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
		GUI_ENC_API_SetValue(EventID2, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
		return;
	}
	
	ret = APP_EPG_API_GetCurAndNextEvent(APP_ChnDatabase_API_GetCurChnNO(), &curEpgInfo, &nextEpgInfo);
	if(GUI_API_Strlen(curEpgInfo.epgName) != 0)
		sprintf(g_epg_EventName[0], "%02d:%02d-%02d:%02d %s",curEpgInfo.startHour,curEpgInfo.startMin,
			curEpgInfo.endHour,curEpgInfo.endMin,curEpgInfo.epgName);
	else 
		g_epg_EventName[0][0] = 0;

	if(GUI_API_Strlen(nextEpgInfo.epgName) != 0)
		sprintf(g_epg_EventName[1], "%02d:%02d-%02d:%02d %s",nextEpgInfo.startHour,nextEpgInfo.startMin,
			nextEpgInfo.endHour,nextEpgInfo.endMin,nextEpgInfo.epgName);
	else 
		g_epg_EventName[1][0] = 0;

	if(Param == 0)
	{
		g_epg_EventName[0][26]=0;
		g_epg_EventName[1][26]=0;
	}
	else if(Param == 1)
	{
		g_epg_EventName[0][44]=0;
		g_epg_EventName[1][44]=0;
	}	
	if((ret != APP_FAIL) && (APP_SUCCESS == APP_SYS_API_GetLocalDateAndTime(&now)))
	{
		nowTime = now.m_hour*60+now.m_min;
		curEpgStartTime = curEpgInfo.startHour*60+curEpgInfo.startMin;
		curEpgEndTime = curEpgInfo.endHour*60+curEpgInfo.endMin;
		if(nowTime < curEpgStartTime)
		{
		    nowTime = nowTime + 24*60;
		}
		if(curEpgEndTime < curEpgStartTime)
		{
		    curEpgEndTime = curEpgEndTime + 24*60;
		}
	//	APP_PRINT_DEBUG("curEpgEndTime = %d, curEpgStartTime = %d",curEpgEndTime, curEpgStartTime);
		if((curEpgEndTime > curEpgStartTime)&& curEpgEndTime > 0 && curEpgEndTime > 0)
		{
			pbrValue = (nowTime-curEpgStartTime)*PbrLength/(curEpgEndTime- curEpgStartTime);
		//	APP_PRINT_DEBUG("pbrValue = %d",pbrValue);
			GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, pbrValue, 0);
			//GUI_ENC_API_Update(PbrID, 1);
		}
		else
		{
		    GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
		}
	}
	else
	{
	     GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
	}
	GUI_ENC_API_SetValue(EventID1, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
	GUI_ENC_API_SetValue(EventID2, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
}
/*----------------------------------------------------------------------------
描述：更新当前、下一个节目预告
-----------------------------------------------------------------------------*/
void UI_APP_API_RenewEvent(BU32 curID,BU32 nextID, BU32 PbrID, BU32 PbrLength,BU32 bEndtime)
{

	//static BU32 curTime = 0,nextTime = 0;
	Epg_Info_S curEpgInfo;
	Epg_Info_S nextEpgInfo;
	BU32 ret = APP_FAIL;
	APP_Date_S now;
	BU32 nowTime;//当前时间
	BU32 curEpgStartTime, curEpgEndTime;//当前EPG事件开始与结束时间
	BU32 pbrValue;
	static BU08 eventName[2][60]=
	{
		"","",
	};
	if (APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		g_epg_EventName[0][0] = 0;
		g_epg_EventName[1][0] = 0;
		GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
		GUI_ENC_API_SetValue(curID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
		GUI_ENC_API_SetValue(nextID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
		return;
	}
	
	ret = APP_EPG_API_GetCurAndNextEvent(APP_ChnDatabase_API_GetCurChnNO(), &curEpgInfo, &nextEpgInfo);

	
	if(GUI_API_Strlen(curEpgInfo.epgName) != 0)
	{
		//if(curTime != curEpgInfo.startHour*60+curEpgInfo.startMin)
		{
			//curTime = curEpgInfo.startHour*60+curEpgInfo.startMin;
			if(bEndtime)
			{
				sprintf(g_epg_EventName[0], "%02d:%02d-%02d:%02d %s",curEpgInfo.startHour,curEpgInfo.startMin,
					curEpgInfo.endHour,curEpgInfo.endMin,curEpgInfo.epgName);

			}
			else
			{
				sprintf(g_epg_EventName[0], "%02d:%02d %s",curEpgInfo.startHour,curEpgInfo.startMin,curEpgInfo.epgName);

			}
			g_epg_EventName[0][44]=0;
			if(GUI_API_Strcmp(eventName[0], g_epg_EventName[0]))
			{
				GUI_API_Strcpy(eventName[0],g_epg_EventName[0]);
				GUI_ENC_API_SetValue(curID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
			}
		}
	}
	else
	{
		g_epg_EventName[0][0] = 0;
		if(GUI_API_Strcmp(eventName[0], g_epg_EventName[0]))
		{
			GUI_API_Strcpy(eventName[0],g_epg_EventName[0]);
			GUI_ENC_API_SetValue(curID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
		}
		//GUI_ENC_API_SetValue(curID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[0],0);
	}

	if(GUI_API_Strlen(nextEpgInfo.epgName) != 0)
	{
		//if(nextTime != nextEpgInfo.startHour*60+nextEpgInfo.startMin)
		{
			//nextTime = nextEpgInfo.startHour*60+nextEpgInfo.startMin;
			if(bEndtime)
			{
				sprintf(g_epg_EventName[1], "%02d:%02d-%02d:%02d %s",nextEpgInfo.startHour,nextEpgInfo.startMin,
					nextEpgInfo.endHour,nextEpgInfo.endMin,nextEpgInfo.epgName);
			}
			else
			{
				sprintf(g_epg_EventName[1], "%02d:%02d %s",nextEpgInfo.startHour,nextEpgInfo.startMin,nextEpgInfo.epgName);

			}
			g_epg_EventName[1][44]=0;
			if(GUI_API_Strcmp(eventName[1], g_epg_EventName[1]))
			{
				GUI_API_Strcpy(eventName[1],g_epg_EventName[1]);
				GUI_ENC_API_SetValue(nextID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
			}
		}
	}
	else
	{
		g_epg_EventName[1][0] = 0;
		//GUI_ENC_API_SetValue(nextID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
		if(GUI_API_Strcmp(eventName[1], g_epg_EventName[1]))
		{
			GUI_API_Strcpy(eventName[1],g_epg_EventName[1]);
			GUI_ENC_API_SetValue(nextID, WDT_STA_SV_STRING, (BU32)g_epg_EventName[1],0);
		}
	}

	/*更新当前epg事件播放时间进度*/
	if((ret != APP_FAIL) && (APP_SUCCESS == APP_SYS_API_GetLocalDateAndTime(&now)))
	{
		nowTime = now.m_hour*60+now.m_min;
		curEpgStartTime = curEpgInfo.startHour*60+curEpgInfo.startMin;
		curEpgEndTime = curEpgInfo.endHour*60+curEpgInfo.endMin;
		if(nowTime < curEpgStartTime)
		{
		    nowTime = nowTime + 24*60;
		}
		if(curEpgEndTime < curEpgStartTime)
		{
		    curEpgEndTime = curEpgEndTime + 24*60;
		}
		//APP_PRINT_DEBUG("curEpgEndTime = %d, curEpgStartTime = %d",curEpgEndTime, curEpgStartTime);
		if((curEpgEndTime > curEpgStartTime)&& curEpgEndTime > 0 && curEpgEndTime > 0)
		{
			pbrValue = (nowTime-curEpgStartTime)*PbrLength/(curEpgEndTime- curEpgStartTime);
			//APP_PRINT_DEBUG("pbrValue = %d",pbrValue);
			GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, pbrValue, 0);
			//GUI_ENC_API_Update(PbrID, 1);
		}
		else
		{
		    GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
		}
	}
	else
	{
	     GUI_ENC_API_SetValue(PbrID, WDT_PBR_SV_CHANGE, 0, 0);
	}
}
/*----------------------------------------------------------------------------
描述：设置，和获取节目列表dialog的前状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnListPreState(BU32 dlg_id)
{
	g_chnList_preState = dlg_id;
}
BU32 UI_APP_API_GetChnListPreState(void)
{
	return g_chnList_preState;
}

/*----------------------------------------------------------------------------
描述：设置，和获取节目编辑dialog的前状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnEditPreState(BU32 dlg_id)
{
	g_chnEdit_preState = dlg_id;
}
BU32 UI_APP_API_GetChnEditPreState(void)
{
	return g_chnEdit_preState;
}

/*----------------------------------------------------------------------------
描述：设置，和获取互动帮助dialog的前状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetDoubleHelpPreState(BU32 dlg_id)
{
	g_doublehelp_preState = dlg_id;
}
BU32 UI_APP_API_GetDoubleHelpPreState(void)
{
	return g_doublehelp_preState;
}

/*----------------------------------------------------------------------------
描述：设置音量大小
-----------------------------------------------------------------------------*/
void UI_APP_API_SetVolume(void)
{
	BS32 sys_volume=0;
	BU32 sys_volume_tmep=0;
	BS32 actual_volume=0;
	ChnDataBase_ChnInfo_S chnInfo;

	if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
	{
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&chnInfo))
		{
			actual_volume = sys_volume*2;
		}
		else
		{
			actual_volume = APP_Audio_API_GetActualVolume(chnInfo.m_AudioOffset, chnInfo.m_AudioVolume);
			APP_Audio_API_SetTrack((APP_CHN_TRACK_E)chnInfo.m_AudioChn);
		}
	
		APP_Audio_API_SetVolume((BU08)actual_volume);
	}
	
#if 0		
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e,&sys_volume_tmep);
		sys_volume = (BS32)sys_volume_tmep;

		if(sys_volume)
		{
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&chnInfo))
			{
				actual_volume = sys_volume*2;
			}
			else
			{
				if(sys_volume+chnInfo.m_AudioOffset>0)
				{
					
					if(sys_volume+chnInfo.m_AudioOffset > 32)
					{
						
						actual_volume = 32*2;
					}
					else
					{
						actual_volume = (sys_volume+chnInfo.m_AudioOffset)*2;
					}
				}
				else
				{
					
					actual_volume = 1;
				}
				APP_Audio_API_SetTrack((APP_CHN_TRACK_E)chnInfo.m_AudioChn);
			}
			
		}
		else
		{
			actual_volume = 0;
		}		

		APP_Audio_API_SetVolume((BU08)actual_volume);
	}
#endif
}
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetLockServiceId(BU32 serviceid)
{
	g_lock_serviceIdTemp = serviceid;
}
BU32 UI_APP_API_GetLockServiceId(void)
{
	return g_lock_serviceIdTemp;
}
/*----------------------------------------------------------------------------
描述：该变量表示节目是否被锁
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurChnbPlay(BU32 bplay)
{
	g_lock_curChnbPlay = bplay;
}
BU32 UI_APP_API_GetCurChnbPlay(void)
{
	return g_lock_curChnbPlay;
}
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnBUnlock(BU32 bunlock)
{
	g_lock_bUnlock = bunlock;
}
BU32 UI_APP_API_GetChnBUnlock(void)
{
	return g_lock_bUnlock;
}
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_API_SetLockCurState(BU32 curstate)
{
	g_lock_curState = curstate;
}
BU32 UI_APP_API_GetLockCurState(void)
{
	return 	g_lock_curState;
}


/*
播放电视广播的通用接口
u16CurChn:播放的节目号
u8CheckLock:是否检查节目锁，一般在节目列表或者全屏状态下切换节目是要检查节目锁的，
而在节目编辑切换节目不需要检查，传0
*/
BU32 UI_APP_API_PlayChn(BU16 u16CurChn, BU16 u16LedIndex, BU08 u8CheckLock)
{
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 prestate;
	BU08 str[5];
	BU08 AvType = APP_ChnDatabase_API_GetCurChnType();
	BU32 ServiceID = UI_APP_API_GetLockServiceId();
    BU32 chnLock;
	BU32 g_sys_volumesave = 0;
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(AvType, APP_ChnDatabase_API_GetCurGroupType(),u16CurChn, &chnInfo))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo error!\n");
		return APP_FAIL;
	}
	APP_SYS_API_GetSysInfo(APP_SYS_VolumeSave_e, &g_sys_volumesave);
	if (g_sys_volumesave == 0)
		GUI_ENC_API_SetValue(ID_VOI_ChnInfo, WDT_VOI_SV_RIGHT, chnInfo.m_AudioVolume, 0);
	sprintf((BS08*)str,"%c%03d",(AvType == APP_CHN_AV_TYPE_TV_e)?'C':'R', u16LedIndex + APP_ChnDatabase_API_GetChn0Flag());
	APP_Com_API_ShowLED(str);

	APP_PRINT_DEBUG("APP_Com_API_GetShowLogoFlag() = %d\n", APP_Com_API_GetShowLogoFlag());
	
	//进行检查节目锁
	APP_SYS_API_GetSysInfo(APP_SYS_CHN_LOCK_e, &chnLock);
	if(chnLock)
	{
		if (u8CheckLock && chnInfo.m_Lock && UI_APP_API_GetChnBUnlock() == 0)
		{
			if (UI_APP_API_GetCurChnbPlay() == 0 && ServiceID != chnInfo.m_ProgId) //不能进行播放，则先停止节目，并弹出密码框
			{
				if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
				{
					APP_Com_API_StopProg(1);//黑屏
				}
				else
				{
					APP_Com_API_StopProg(0);
				}
				APP_Com_API_SetEcmNULL();
				APP_Com_API_Stop_Signal_Dectect();
				prestate = GUI_ENC_API_GetCurState();
				UI_APP_API_SetLockCurState(prestate);
				if(prestate == DLG_TIMEOUT)
				{
					UI_APP_API_SetLockCurState(DLG_CHNINFO);
				}
				if(UI_APP_API_GetLockCurState() != DLG_CHNLOCK)
				{
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK, 0, 0);			
				}
				return APP_SUCCESS;
			}
			else if (UI_APP_API_GetCurChnbPlay() == 2)//按退出键
			{
				UI_APP_API_SetLockServiceId(0);
				UI_APP_API_SetCurChnbPlay(0);
				return APP_SUCCESS;
			}
			else if (UI_APP_API_GetCurChnbPlay() == 1)//输入密码正确
			{
				UI_APP_API_SetCurChnbPlay(0);
			}
		
		}
	}
	UI_APP_API_SetCurChnbPlay(0);
	APP_Com_API_Start_Signal_Dectect();
	UI_APP_API_SetLockServiceId(chnInfo.m_ProgId);
	if (ServiceID != chnInfo.m_ProgId || APP_IPI_GetIsPlayStop())
	{
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_SIGNAL_PLAYCHANGE, 0, 0);
	
	}
	if (ServiceID != chnInfo.m_ProgId || (ServiceID == chnInfo.m_ProgId && !UI_APP_API_bShowNoSignal()))
	{
			//当从无信号或者广播状态下跳到电视状态下要进行黑屏操作
		if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e && APP_Com_API_GetShowLogoFlag() != 0 )
		{
		//	if(!UI_APP_API_bShowNoSignal())
			{
				APP_PRINT_DEBUG("Show black screen");
				APP_Com_API_StopProg(1);
			}
		}

			// 进入广播状态下要显示广播logo
		if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_RD_e && APP_Com_API_GetShowLogoFlag() != 1)
		{
			//if (!UI_APP_API_bShowNoSignal())
			{
				APP_PRINT_DEBUG("showRadioLogo\n");
				//APP_Com_API_ShowRadioLogo();
				APP_Com_API_ShowOtherBmp(1);
			}
			
		}
	}

	APP_Com_API_PlayChn(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), u16CurChn, -1);

	return APP_SUCCESS;
	
}

/*
u32ServiceID:时移事件的serviceid
*/
extern BU16 nvodPlayServiceId;
BU32 UI_APP_API_PlayByServiceID(BU32 u32ServiceID, BU08 strled[])
{
	APP_Com_API_ShowLED(strled);
	
	if (nvodPlayServiceId != u32ServiceID && APP_Com_API_GetShowLogoFlag() != 0 )
	{
	//	if(!UI_APP_API_bShowNoSignal())
		{
			APP_PRINT_DEBUG("Show black screen");
			APP_Com_API_StopProg(1);
		}
	}

	APP_Com_API_PlayChn(APP_CHN_AV_TYPE_NVOD_REFERENCE_SERVICE, APP_CHN_GROUP_TYPE_ALL_e, 0xffff, u32ServiceID);
	
}



#if 0
/*----------------------------------------------------------------------------
描述：播放节目
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckPlay_PlayChn(BU16 curChn, BU08 bForce)
{
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 ca_sys_id;
	BS32 ret = 0;
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
	{
		return;
	}
#ifdef ENABLE_BROADCAST_IPANEL
	if(Ipanel_GetStatus() == 1)
	{
		APP_PRINT_DEBUG(" ipanel is start \n");
		return;
	}
#endif
	if(bForce || chnInfo.m_ProgId != UI_APP_API_GetLockServiceId())
	{
	//	APP_Printf("  UI_APP_API_CheckPlay_PlayChn  play chan %d \n",curChn);
		APP_Com_API_PlayChn(curChn);
		UI_APP_API_SetVolume();
		UI_APP_API_SetLockServiceId(chnInfo.m_ProgId);
	}
}
/*----------------------------------------------------------------------------
描述：停止播放节目
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_StopChn(BU32 isNeedBlkScrn)
{
	APP_Com_API_StopProg(isNeedBlkScrn);
	UI_APP_API_SetLockServiceId(0);
}
/*----------------------------------------------------------------------------
描述：播放节目并在面板上显示节目号
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckPlay_PlayChn_led(BU16 curChn, BU08 bForce)
{
	BU08 str[4];
	sprintf((BS08*)str,"%c%03d",(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)?'C':'R',curChn + APP_ChnDatabase_API_GetChn0Flag());
	APP_Com_API_ShowLED(str);
	UI_APP_API_CheckPlay_PlayChn (curChn, bForce);
}
/*----------------------------------------------------------------------------
描述：检测节目是否加锁，并根据情况选择是否播放节目
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_PlayChn(BU16 curChn,BU32 checkLock)
{
	BU08 sysVolume=0;
	BU32 volume=0;
	BU32 sysChnLock=0;
	ChnDataBase_ChnInfo_S chnInfo;
	APP_CHN_AV_TYPE_E avType=APP_CHN_AV_TYPE_TV_e;
	BU08 str[4];
	BU32 prestate;

	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
	{
		return;
	}

	//对于切换节目,隐藏节目暂停
	if(g_progid_old != chnInfo.m_ProgId)
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDE_PAUSE, 0, 0);
		g_progid_old = chnInfo.m_ProgId;
		UI_APP_API_SetCurChnbPlay(0);
	}
	APP_SYS_API_GetSysInfo(APP_SYS_CHN_LOCK_e, &sysChnLock);
	if(sysChnLock && UI_APP_API_GetChnBUnlock() == 0 && chnInfo.m_Lock)
	{
		if(UI_APP_API_GetCurChnbPlay())
		{
			UI_APP_API_CheckPlay_PlayChn (curChn, 1);
		}
		else
		{
			UI_APP_API_SetCurChnbPlay(0);
			avType = APP_ChnDatabase_API_GetCurChnType();
			if(UI_APP_API_GetLockServiceId() != chnInfo.m_ProgId)
			{
				UI_APP_API_SetLockServiceId(chnInfo.m_ProgId);
				APP_Com_API_StopProg(0);
				if(!UI_APP_API_bShowNoSignal() && avType != APP_CHN_AV_TYPE_RD_e)
				{
					APP_Com_API_ShowBmp(0);//黑屏
				}
				prestate = GUI_ENC_API_GetCurState();
				UI_APP_API_SetLockCurState(prestate);
				if(prestate == DLG_TIMEOUT)
				{
					UI_APP_API_SetLockCurState(DLG_CHNINFO);
				}
				if(UI_APP_API_GetLockCurState() != DLG_CHNLOCK && checkLock)
				{
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_CHECKLOCK, 0, 0);			
				}
			}
		}
	}
	else
	{
		UI_APP_API_SetCurChnbPlay(1);
		UI_APP_API_CheckPlay_PlayChn(curChn,1);
	}
}
/*----------------------------------------------------------------------------
描述：检测节目是否加锁，并根据情况选择是否播放节目,并在面板上显示节目号
-----------------------------------------------------------------------------*/
void UI_APP_API_CheckLock_PlayChn_led(BU16 curChn,BU32 ledIndex,BU32 checkLock)
{
	APP_CHN_AV_TYPE_E avType=APP_CHN_AV_TYPE_TV_e;
	BU08 str[5];
	
	avType = APP_ChnDatabase_API_GetCurChnType();
	sprintf((BS08*)str,"%c%03d",( avType == APP_CHN_AV_TYPE_TV_e)?'C':'R',ledIndex + APP_ChnDatabase_API_GetChn0Flag());
	APP_Com_API_ShowLED(str);
	APP_Printf("APP_Com_API_ShowLED !!!!\n");
	
	UI_APP_API_CheckLock_PlayChn (curChn,checkLock);
}
#endif
/*----------------------------------------------------------------------------
描述：设置、获得数据是否保存到Flash的标志
-----------------------------------------------------------------------------*/
void UI_APP_API_SetbSaveFlag(BU32 bsave)
{
	g_sys_bSaveFlash = bsave;
}
BU32 UI_APP_API_GetbSaveFlag(void)
{
	return g_sys_bSaveFlash;
}
/*----------------------------------------------------------------------------
描述：设置、获得在全屏播放节目时当前显示状态
		如:	显示节目信息状态,
			显示音量状态,
			声道设置状态,
			显示没有节目状态,
			显示节目号状态,
			显示没有服务状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnInfoCurState(BU32 curstate)
{
	g_fullscreen_curState = curstate;
}
BU32 UI_APP_API_GetChnInfoCurState(void)
{
	return g_fullscreen_curState;
}
/*----------------------------------------------------------------------------
描述：设置、获得在全屏播放节目时，在提示框显示的内容在数字字典中的ID
		如:"加密节目，请插卡"
			"节目暂停"
			"无节目"
-----------------------------------------------------------------------------*/
void UI_APP_API_SetChnInfoSaveID(BU32 rs_id)
{
	g_fullscreen_saveid = rs_id;
}
BU32 UI_APP_API_GetChnInfoSaveID(void)
{
	return g_fullscreen_saveid;
}
/*----------------------------------------------------------------------------
描述：保存和获得当前节目的progid
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurChnProgID(BU32 prog_id)
{
	g_cur_prog_id = prog_id;
}
BU32 UI_APP_API_GetCurChnProgID(void)
{
	return g_cur_prog_id;
}
/*----------------------------------------------------------------------------
描述：设置，获得epg是否保存，保存的就是epg中的预约节目
-----------------------------------------------------------------------------*/
void UI_APP_API_SetEpgSaveFlag(BU08 savefalg)
{
	g_epg_needSave = savefalg;
}
BU08 UI_APP_API_GetEpgSaveFlag(void)
{
	return g_epg_needSave;
}
/*----------------------------------------------------------------------------
描述：设置、获取Epg Dialog的前一个状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetEpgPreState(BU32 dlg_id)
{
	g_epg_preState = dlg_id;
}
BU32 UI_APP_API_GetEpgPreState(void)
{
	return g_epg_preState;
}
/*----------------------------------------------------------------------------
描述：设置、获取SortProg Dialog的前一个状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetSortProgPreState(BU32 dlg_id)
{
	g_SortProgram_preState = dlg_id;
}
BU32 UI_APP_API_GetSortProgPreState(void)
{
	return g_SortProgram_preState;
}
/*----------------------------------------------------------------------------
描述：设置、获取预约列表Dialog的前一个状态，如:从主菜单或从Epg进入预约列表对话框
-----------------------------------------------------------------------------*/
void UI_APP_API_SetReserveListPreState(BU32 id)
{
	g_reserve_preState = id;
}
BU32 UI_APP_API_GetReserveListPreState(void)
{
	return g_reserve_preState;
}
/*----------------------------------------------------------------------------
描述：
-----------------------------------------------------------------------------*/
void UI_APP_SetDefaultState(BU32 dlg_id)
{
	g_sys_defaultToState = dlg_id;
}
BU32 UI_APP_GetDefaultState(void)
{
	return g_sys_defaultToState;
}
/*----------------------------------------------------------------------------
描述：设置、获取邮件前状态
-----------------------------------------------------------------------------*/
void UI_APP_CA_SetMailPreState(BU32 id)
{
	g_mail_preState = id;
}
BU32 UI_APP_CA_getMailPreState(void)
{
	return g_mail_preState;
}
/*----------------------------------------------------------------------------
描述：设置、获取邮件是否已满状态
-----------------------------------------------------------------------------*/
void UI_APP_SetMailFullState(BU08 id)
{
	 g_ca_mail_full = id;
}
BU08 UI_APP_GetMailFullState(void)
{
	return g_ca_mail_full;
}
/*----------------------------------------------------------------------------
描述：设置或获取PIN检测提示框的前状态框，如:成人设置,工作时段设置等
-----------------------------------------------------------------------------*/
void UI_APP_API_SetPinNotePreState(BU32 pre_state)
{
	g_pinNote_preState = pre_state;
}
BU32 UI_APP_API_GetPinNotePreState(void)
{
	return g_pinNote_preState;
}
/*----------------------------------------------------------------------------
描述：设置，和获取数据广播列表dialog的前状态
-----------------------------------------------------------------------------*/
void UI_APP_API_SetBrdcastListPreState(BU32 dlg_id)
{
	g_brdcastlist_preState = dlg_id;
}
BU32 UI_APP_API_GetBrdcastListPreState(void)
{
	return g_brdcastlist_preState;
}
/*----------------------------------------------------------------------------
描述：设置，和获取数据广播列表中当前数据广播
-----------------------------------------------------------------------------*/
void UI_APP_API_SetCurBrdcastNo(BU32 curno)
{
	g_brdcastlist_curno = curno;
}
BU32 UI_APP_API_GetCurBrdcastNo(void)
{
	return g_brdcastlist_curno;
}
/*时间转换函数,来自CDCA整型转化为日期*/
BU08 UI_APP_GetDateStrFromValC_TF(unsigned short wInputDate,char * szOut)   
{
	int nYear,nMon,nDay,nTemp,nDate = wInputDate;
	char sz[30];
	
	nYear = 2000;
	do{/*year.*/
		if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0)
			nTemp = 366;
		else
			nTemp = 365;
		if(nDate - nTemp < 0)
			break;
		nDate -= nTemp;
		nYear ++;
	}while(true);
	nMon = 1;
	do{/*month.*/
		if(2 == nMon){/* dai:if is Feb.*/
			if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0) /*dai:if leap.*/
				nTemp = 29;
			else
				nTemp = 28;
		}
		else if(4 == nMon || 6 == nMon || 9 == nMon || 11 == nMon)
			nTemp = 30;
		else
			nTemp = 31;
		if(nDate - nTemp < 0)
			break;
		nDate -= nTemp;
		nMon ++;
	}while(true);
	if(1 > nMon || 12 < nMon)
		return false;
	nDay = 1;
	nDay += nDate; /*day.*/
	if(1 > nDay || 31 < nDay)
		return false;
	
	memset(sz,0,sizeof(sz));
	sprintf(sz,"%d-%d-%d",nYear,nMon,nDay);
	strcpy(szOut,sz);
	return true;
}

/*时间转换函数,来自CDCA整型转化为时间*/
BU08 UI_APP_GetTimeStrFromValC_TF(unsigned int wInputDate,char * szOut) 
{
	BU16 date;
	BU08 hour,minute,second;
	BU08 date_str[20];

	date =
(BU16)(wInputDate >> 16);
	hour = (BU08)((wInputDate & 0xf800)>>11);
	minute = (BU08)((wInputDate & 0x07e0)>>5);
	second = (BU08)(wInputDate & 0x001f);
	UI_APP_GetDateStrFromValC_TF(date, date_str);
	sprintf(szOut, "%s %02d:%02d",date_str,hour,minute);
}

/*时间转换函数,来自数码视讯整型转化为日期*/
/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22.*/
BU08 UI_APP_GetDateStrFromValC(unsigned short wInputDate,char * szOut)    /*dai:szOut memory is provided by outside,and its out format is like 2002-5-22.*/
{
	int nYear,nMon,nDay,nTemp,nDate = wInputDate;
	char sz[30];

	
	nYear = 1970;
	do{/*year.*/
		if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0)
		{
			nTemp = 366;
		}
		else
		{
			nTemp = 365;
		}
		if(nDate - nTemp < 0)
		{
			break;
		}
		nDate -= nTemp;
		nYear ++;
	}while(true);
	nMon = 1;
	do{/*month.*/
		if(2 == nMon)
		{/* dai:if is Feb.*/
			if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0) /*dai:if leap.*/
			{
				nTemp = 29;
			}
			else
			{
				nTemp = 28;
			}
		}
		else if(4 == nMon || 6 == nMon || 9 == nMon || 11 == nMon)
		{
			nTemp = 30;
		}
		else
		{
			nTemp = 31;
		}
		if(nDate - nTemp < 0)
		{
			break;
		}
		nDate -= nTemp;
		nMon ++;
	}while(true);
	if(1 > nMon || 12 < nMon)
	{
		return false;
	}
	nDay = 1;
	nDay += nDate; /*day.*/
	if(1 > nDay || 31 < nDay)
	{
		return false;
	}
	
	memset(sz,0,sizeof(sz));
	sprintf(sz,"%04d-%02d-%02d",nYear,nMon,nDay);
	GUI_API_Strcpy(szOut,sz);
	return true;
}

/*时间转换函数,来自数码视讯整型转化为日期兼时间*/
/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22 12:3:5.*/
BU08 UI_APP_GetTimeStrFromValC(int nInputTime,char * szOut)/*dai:szOut memory is provided by outside,and its out format is like 2002-5-22 12:3:5.*/
{	
	int nDate,nHour,nMin,nSec,nOff,nTime;
	char szTime[40];
	
	nDate = nInputTime / (24*3600);
	nTime = nInputTime % (24*3600);
	nHour = nTime / 3600;
	if(24 <= (nHour+8))	//dai:to local hour.
	{
		nDate ++;
	}
	if(!UI_APP_GetDateStrFromValC(nDate,szTime))
	//if(!__caInfo_GetDateStrFromValC_TF(nDate,szTime))
	{
		return false;
	}
	nOff = GUI_API_Strlen(szTime);
	if(8 > nOff)
	{
		return false;
	}
	nTime = nInputTime % 3600;
	nMin = nTime / 60;
	if(0 > nMin || 59 < nMin)
	{
		return false;
	}
	nSec = nTime % 60;
	if(0 > nSec || 59 < nSec)
	{
		return false;
	}
	sprintf(szTime + nOff," %02d:%02d:%02d",(nHour+8)%24,nMin,nSec);
	GUI_API_Strcpy(szOut,szTime);
	return true;

}
#if defined (DVT_CA)
void UI_APP_convertSecIntoDay (BU32 secInTotal, caInfo_time_s * outPut)
{
	BU32 days = 0;
	BU32 hours = 0;
	BU32 mins = 0;
	BU32 secs = 0;;
	BU32 day,len = 0;

	days = secInTotal / 86400;//24*60*60
	secInTotal %= 86400;
	hours  = (secInTotal / 3600);
	secInTotal %= 3600;
	mins = (secInTotal / 60);	
    secs = (secInTotal % 60);

	day = days;
	do
	{
		day = day / 10;
		len ++;
	}while(day);
	
	GUI_API_AddDecStr(days, len, outPut->m_days);
	GUI_API_AddDecStr(hours, 2, outPut->m_hrs);
	GUI_API_AddDecStr(mins, 2, outPut->m_mins);
	GUI_API_AddDecStr(secs, 2, outPut->m_secs);
	

}
void UI_APP_convertFengIntoYuan (BU32 fengInTotal, caInfo_currency_s * outPut)
{
	BU32 yuan = 0;
	BU32 fen = 0;
	BU32 jiao = 0;
	BU32 y,len = 0;

	yuan= fengInTotal / 100;//10*10*
	fengInTotal %= 100;
	jiao= fengInTotal / 10;	
    fen= fengInTotal % 10;

	y = yuan;
	do
	{
		y = y / 10;
		len ++;
	}while(y);
	
	GUI_API_AddDecStr(yuan, len, outPut->m_yuan);
	GUI_API_AddDecStr(jiao, 1, outPut->m_jiao);
	GUI_API_AddDecStr(fen,  1, outPut->m_fen);
	

}
#endif
/******************************
描述：设置，和获取删除当前目录下的文件的序号
*******************************/
void UI_APP_API_SetDelFileNo(BU32 FileNo)
{
	g_del_fileno = FileNo;
}
BU32 UI_APP_API_GetDelFileNo(void)
{
	return g_del_fileno;
}
/******************************
描述：设置，和获取Nvod事件序号
*******************************/
void UI_APP_API_SetNvodEventIndex(BU32 EventIndex)
{
	g_nvod_eventIndex = EventIndex;
}
BU32 UI_APP_API_GetNvodEventIndex(void)
{
	return g_nvod_eventIndex;
}
/******************************
描述：设置，和获取冲突序号
*******************************/
void UI_APP_API_SetConflictIndex(BU32 ConflictIndex)
{
	g_conflictIndex = ConflictIndex;
}
BU32 UI_APP_API_GetConflictIndex(void)
{
	return g_conflictIndex;
}
/*************************************
描述:设置和获取timeshift全屏下的各状态
*************************************/
void UI_APP_API_SetTimeShiftCurState(BU32 CurState)
{
    g_timeShift_curState = CurState;
}
BU32 UI_APP_API_GetTimeShiftCurState(void)
{
    return g_timeShift_curState;
}

/***************************************
描述:在菜单状态，控制音量大小
****************************************/
void UI_APP_API_VoiceControl(BU32 key)
{
	BU16 chnNum;
	BU32 index,i;
	BU08 str[3];
	
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	APP_PRINT_DEBUG("====UI_APP_API_VoiceControl====");
	if(chnNum == 0)
	{
		printf("UI_APP_API_VoiceControl: chnNum == 0 \n");
		return;
	}
#ifdef ENABLE_BROADCAST_ENREACH
	if (GUI_ENC_API_GetCurState () == ID_DLG_ChnInfo || GUI_ENC_API_GetCurState () == ID_DLG_FULLTOTVOD)
	{
		printf("status is no support\n");
		return;
	}
#else	
	if (GUI_ENC_API_GetCurState () == ID_DLG_ChnInfo || GUI_ENC_API_GetCurState () == ID_DLG_FULLTOTVOD || GUI_ENC_API_GetCurState () == ID_DLG_Null)
	{
		printf("status is no support\n");
		return;
	}
#endif
//	GUI_ENC_API_GetValue(ID_VOI_ChInfo,WDT_VOI_SV_INDEX,&index,0);
	if(GUI_VK_V_DOWN== key || GUI_VK_V_UP == key)
	{
		//将声音打开放到判断里面
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			if(GUI_VK_V_UP == key)
			{
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
		}
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e,&index);
		

		if(GUI_VK_V_DOWN == key)
		{
			if(index > 0)
			{
				index--;
				APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e,index);
				UI_APP_API_SetVolume(); 
			}
		}
		else if(GUI_VK_V_UP == key)
		{
			if(index < 32)
			{
				index++;
				APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e,index);
				UI_APP_API_SetVolume(); 					
			}
		}
	}
	else
	{
		if(key == GUI_VK_MUTE)
		{
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
			else
			{
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
		}
	}
}

