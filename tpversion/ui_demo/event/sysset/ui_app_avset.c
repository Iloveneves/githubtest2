#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//输出设置
void UI_APP_OutPutSet_Enter(void)
{
    GUI_Rect_s rect;
	BU32 hwnd;
    BU32 g_sys_tvmode=APP_SYS_TVMODE_PAL_e;
	BU32 g_sys_videoout =APP_SYS_VIDEO_OUT_YUV_e;
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	hwnd = GUI_API_GetDlgItem(ID_VFM_OutPutSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);
	APP_EPG_API_StopGetEpg();

	APP_SYS_API_GetSysInfo(APP_SYS_TV_MODE_e,&g_sys_tvmode);
	APP_SYS_API_GetSysInfo(APP_SYS_VIDEOOUT_e,&g_sys_videoout);
	GUI_ENC_API_SetValue(ID_CBX_TVMode_OutPutSet,WDT_CBX_SV_INDEX,(BU32)g_sys_tvmode,0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_OutPutSet,WDT_CBX_SV_INDEX,(BU32)g_sys_videoout,0);

	GUI_ENC_API_SetValue(ID_CBX_TVMode_OutPutSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_OutPutSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_OutPutSet,ID_STA_Event2_OutPutSet,ID_PBR_Epg_OutPutSet,260,1);
}

void UI_APP_OutPutSet_End(void)
{

}

void UI_APP_OutPutSet_Key(BU32 key)
{
    BU32 index;
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_OutPutSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_TVMode_OutPutSet,WDT_CBX_SV_INDEX,&index,0);
			APP_PRINT_DEBUG("index = %d", index);
			APP_SYS_API_SetSysInfo(APP_SYS_TV_MODE_e,index);
		}
		else
        {
            GUI_ENC_API_GetValue(ID_CBX_VideoOut_OutPutSet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_VIDEOOUT_e,index);
        }
		break;
	case GUI_VK_UP:
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_OutPutSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VideoOut_OutPutSet);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_OutPutSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_OutPutSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else
		{
		    GUI_ENC_API_SetFocus(ID_CBX_TVMode_OutPutSet);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_OutPutSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_OutPutSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	case GUI_VK_SELECT:
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}
}

//显示设置
void UI_APP_DisplaySet_Enter(void)
{
    GUI_Rect_s rect;
	BU32 hwnd;
    BU32 g_sys_aspect=0;
	BU32 g_sys_transparence=0;
	BU32 g_sys_timeout=0;
	BU32 g_sys_tvmode=APP_SYS_TVMODE_PAL_e;
	BU32 g_sys_videoout =APP_SYS_VIDEO_OUT_YUV_e;
	BU32 g_sys_volumesave = 0;
	BU32 index;
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	hwnd = GUI_API_GetDlgItem(ID_VFM_DisplaySet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);
	APP_EPG_API_StopGetEpg();

	APP_SYS_API_GetSysInfo(APP_SYS_TRANSPARENCE_e,&g_sys_transparence);
	APP_SYS_API_GetSysInfo(APP_SYS_ASPECT_e,&g_sys_aspect);
	APP_SYS_API_GetSysInfo(APP_SYS_TIMEOUT_e,&g_sys_timeout);
	APP_SYS_API_GetSysInfo(APP_SYS_VolumeSave_e, &g_sys_volumesave);
	
	GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet,WDT_CBX_SV_INDEX,(BU32)g_sys_transparence,0);
	GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet,WDT_CBX_SV_INDEX,(BU32)g_sys_aspect,0);
	GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet,WDT_CBX_SV_INDEX,(BU32)g_sys_timeout - 3,0);
	GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet,WDT_CBX_SV_INDEX,(BU32)g_sys_volumesave,0);
	
	GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	
	APP_SYS_API_GetSysInfo(APP_SYS_TV_MODE_e,&g_sys_tvmode);
	APP_SYS_API_GetSysInfo(APP_SYS_VIDEOOUT_e,&g_sys_videoout);
	GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet,WDT_CBX_SV_INDEX,(BU32)g_sys_tvmode,0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet,WDT_CBX_SV_INDEX,(BU32)g_sys_videoout,0);

	GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);

	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_DisplaySet,ID_STA_Event2_DisplaySet,ID_PBR_Epg_DisplaySet,260,1);
}

void UI_APP_DisplaySet_End(void)
{
    
}

void UI_APP_DisplaySet_Key(BU32 key)
{
    BU32 index;
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_DisplaySet))
		{
			GUI_ENC_API_GetValue(ID_CBX_TVMode_DisplaySet,WDT_CBX_SV_INDEX,&index,0);
			APP_PRINT_DEBUG("index = %d", index);
			APP_SYS_API_SetSysInfo(APP_SYS_TV_MODE_e,index);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_VideoOut_DisplaySet))
        {
            GUI_ENC_API_GetValue(ID_CBX_VideoOut_DisplaySet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_VIDEOOUT_e,index);
        }
		else if(GUI_ENC_API_IsFocus(ID_CBX_Trans_DisplaySet))
		{
			GUI_ENC_API_GetValue(ID_CBX_Trans_DisplaySet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_TRANSPARENCE_e,index);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_DisplayScale_DisplaySet))
		{
			GUI_ENC_API_GetValue(ID_CBX_DisplayScale_DisplaySet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_ASPECT_e,index);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_InfoTime_DisplaySet))
		{
			GUI_ENC_API_GetValue(ID_CBX_InfoTime_DisplaySet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_TIMEOUT_e,index + 3);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_VolumeSave_DisplaySet))
		{
			GUI_ENC_API_GetValue(ID_CBX_VolumeSave_DisplaySet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_VolumeSave_e,index);
		}
		break;
	case GUI_VK_UP:
		if (GUI_ENC_API_IsFocus(ID_CBX_TVMode_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VolumeSave_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_VideoOut_DisplaySet))
		{
		    GUI_ENC_API_SetFocus(ID_CBX_TVMode_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_Trans_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VideoOut_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_DisplayScale_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Trans_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_InfoTime_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_DisplayScale_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_VolumeSave_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_InfoTime_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_DOWN:

		if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VideoOut_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_VideoOut_DisplaySet))
		{
		    GUI_ENC_API_SetFocus(ID_CBX_Trans_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_Trans_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_DisplayScale_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_Trans_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_DisplayScale_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_InfoTime_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_DisplayScale_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_InfoTime_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VolumeSave_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_InfoTime_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if (GUI_ENC_API_IsFocus(ID_CBX_VolumeSave_DisplaySet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_TVMode_DisplaySet);
			GUI_ENC_API_SetValue(ID_CBX_VolumeSave_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_DisplaySet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	case GUI_VK_SELECT:
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	case GUI_VK_EXIT:
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}
}

//频道设置
static BU08 g_channelset_lastTrack;
static BS08 g_channelSet_lastOffSet;
static BS08 g_channelSet_lastSkipFlag;
static BS08 g_channelSet_lastFavFlag;
void UI_APP_ChannelSet_Enter(void)
{
    GUI_Rect_s rect;
	BU32 hwnd;
    BU08 g_sys_chnTrack =0;
	BS08 g_sys_chnOffSet = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	hwnd = GUI_API_GetDlgItem(ID_VFM_DisplaySet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);
	APP_EPG_API_StopGetEpg();

	if(APP_ChnDatabase_API_GetCurTypeChnNum())
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
		g_sys_chnTrack = chnInfo.m_AudioChn;
		g_sys_chnOffSet = chnInfo.m_AudioOffset;
		if(g_sys_chnOffSet+15 > 30)
		{
			g_sys_chnOffSet = 15;
		}
		else if(g_sys_chnOffSet+15 < 0)
		{
			g_sys_chnOffSet = -15;
		}
		
		GUI_ENC_API_SetValue(ID_CBX_Track_ChannelSet,WDT_CBX_SV_INDEX,(BU32)g_sys_chnTrack,0);
		GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet, WDT_CBX_SV_INDEX, (BU32)(g_sys_chnOffSet+15), 0);
		GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet,WDT_CBX_SV_INDEX,(BU32)chnInfo.m_Skip,0);
		GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet,WDT_CBX_SV_INDEX,(BU32)chnInfo.m_Fav,0);
  
		g_channelset_lastTrack = g_sys_chnTrack;
		g_channelSet_lastOffSet = g_sys_chnOffSet;
		g_channelSet_lastSkipFlag = chnInfo.m_Skip;
		g_channelSet_lastFavFlag = chnInfo.m_Fav;
		UI_APP_API_SetbSaveFlag(0);
	}

    GUI_ENC_API_SetValue(ID_CBX_Track_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
    
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_ChannelSet,ID_STA_Event2_ChannelSet,ID_PBR_Epg_ChannelSet,260,1);
	
}

void UI_APP_ChannelSet_End(void)
{
    
}

void UI_APP_ChannelSet_Key(BU32 key)
{
    BU32 index;
	ChnDataBase_ChnInfo_S chnInfo;
    switch(key)
    {
    case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
		UI_APP_API_SetbSaveFlag(1);
		if(GUI_ENC_API_IsFocus(ID_CBX_Track_ChannelSet))
		{
		    GUI_ENC_API_GetValue(ID_CBX_Track_ChannelSet,WDT_CBX_SV_INDEX,&index,0);
			chnInfo.m_AudioChn = index;
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_OffSet_ChannelSet))
		{
		    GUI_ENC_API_GetValue(ID_CBX_OffSet_ChannelSet,WDT_CBX_SV_INDEX,&index,0);
			chnInfo.m_AudioOffset= (BS08)(index- 15);
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_ChannelSkip_ChannelSet))
		{
		    GUI_ENC_API_GetValue(ID_CBX_ChannelSkip_ChannelSet,WDT_CBX_SV_INDEX,&index,0);
			chnInfo.m_Skip= index;
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_ChannelFav_ChannelSet))
		{
		    GUI_ENC_API_GetValue(ID_CBX_ChannelFav_ChannelSet,WDT_CBX_SV_INDEX,&index,0);
			chnInfo.m_Fav= index;
		}
		APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo);
		break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_CBX_Track_ChannelSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_ChannelFav_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_Track_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_OffSet_ChannelSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Track_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_Track_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_ChannelSkip_ChannelSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_OffSet_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else
		{
			GUI_ENC_API_SetFocus(ID_CBX_ChannelSkip_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_CBX_Track_ChannelSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_OffSet_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_Track_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_OffSet_ChannelSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_ChannelSkip_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_ChannelSkip_ChannelSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_ChannelFav_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_ChannelSkip_ChannelSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else
		{
			GUI_ENC_API_SetFocus(ID_CBX_Track_ChannelSet);
			GUI_ENC_API_SetValue(ID_CBX_ChannelFav_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_OffSet_ChannelSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_RECALL:
	case GUI_VK_SELECT:
		if(UI_APP_API_GetbSaveFlag())
		{
		    GUI_ENC_API_SwitchState(ID_DLG_ChannelSetInfo, 1);
		}
		else
		{
		    GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		}
		break;
	case GUI_VK_EXIT:
		if(UI_APP_API_GetbSaveFlag())
		{
		    GUI_ENC_API_SwitchState(ID_DLG_ChannelSetInfo, 1);
		}
		else
		{
		    GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		break;

	default:
		break;
    }
}


//频道设置保存提示框
void UI_APP_ChannelSetInfo_Enter(void)
{
	GUI_ENC_API_SetValue(ID_STA_Content_ChannelSetInfo, WDT_STA_SV_DATAID, RS_ChnSetSave, 0);
	GUI_ENC_API_SetValue(ID_TIM_ChannelSetInfo, WDT_TIM_SV_STOP, 0, 0);
}
void UI_APP_ChannelSetInfo_Key(BU32 key)
{
    BU32 index;
	ChnDataBase_ChnInfo_S chnInfo;
    switch(key)
    {
    case GUI_VK_SELECT:
		GUI_ENC_API_SetValue(ID_STA_Content_ChannelSetInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
		APP_ChnDatabase_API_UpDataToFlash(0);
		GUI_ENC_API_SetValue(ID_TIM_ChannelSetInfo, WDT_TIM_SV_PLAY, 0, 0);
	    break;
	case GUI_VK_RECALL:
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
		chnInfo.m_AudioChn = g_channelset_lastTrack;
		chnInfo.m_AudioOffset = g_channelSet_lastOffSet;
		chnInfo.m_Skip = g_channelSet_lastSkipFlag;
		chnInfo.m_Fav = g_channelSet_lastFavFlag;
		APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo);
        GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	case GUI_VK_EXIT:
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
		chnInfo.m_AudioChn = g_channelset_lastTrack;
		chnInfo.m_AudioOffset = g_channelSet_lastOffSet;
		chnInfo.m_Skip = g_channelSet_lastSkipFlag;
		chnInfo.m_Fav = g_channelSet_lastFavFlag;
		APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo);
        GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
    }
}
void UI_APP_ChannelSetInfo_Timer(void)
{
    GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
}
#if 1
//音视频设置
void UI_APP_AvSet_Enter(void)
{
	BU32 g_sys_aspect=0;
	BU32 g_sys_tvmode=APP_SYS_TVMODE_PAL_e;
	BU08 g_sys_chnTrack =0;
	BU32 g_sys_videoout =APP_SYS_VIDEO_OUT_YUV_e;
	BU32 g_sys_transparence=0;
	BU32 index;
	ChnDataBase_ChnInfo_S chnInfo;
//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//播放节目
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	APP_Com_API_SetFullScreen();

	APP_EPG_API_StopGetEpg();
	if(APP_ChnDatabase_API_GetCurTypeChnNum())
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
		g_sys_chnTrack = chnInfo.m_AudioChn;
		GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet,WDT_CBX_SV_INDEX,(BU32)g_sys_chnTrack,0);
	}
	APP_SYS_API_GetSysInfo(APP_SYS_TRANSPARENCE_e,&g_sys_transparence);
	APP_SYS_API_GetSysInfo(APP_SYS_ASPECT_e,&g_sys_aspect);
	APP_SYS_API_GetSysInfo(APP_SYS_TV_MODE_e,&g_sys_tvmode);
	APP_SYS_API_GetSysInfo(APP_SYS_VIDEOOUT_e,&g_sys_videoout);

	GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet,WDT_CBX_SV_INDEX,(BU32)g_sys_transparence,0);
	GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet,WDT_CBX_SV_INDEX,(BU32)g_sys_aspect,0);
	GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet,WDT_CBX_SV_INDEX,(BU32)g_sys_tvmode,0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet,WDT_CBX_SV_INDEX,(BU32)g_sys_videoout,0);
	
	GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
 }
void UI_APP_AvSet_End(void)
{

}
void UI_APP_AvSet_Key(BU32 key)
{
	BU32 index;
	ChnDataBase_ChnInfo_S chnInfo;
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_CBX_Transparence_AvSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_Transparence_AvSet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_TRANSPARENCE_e,index);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_DisplayFormat_AVSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_DisplayFormat_AVSet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_ASPECT_e,index);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_AVSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_TVMode_AVSet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_TV_MODE_e,index);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_TrackSet_AVSet))
		{
			if(APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				GUI_ENC_API_GetValue(ID_CBX_TrackSet_AVSet,WDT_CBX_SV_INDEX,&index,0);
				if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
				{
					return;
				}
				chnInfo.m_AudioChn = index;
				APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo);
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
				}
				UI_APP_API_SetbSaveFlag(1);

			}
		}
		else
		{
			GUI_ENC_API_GetValue(ID_CBX_VideoOut_AVSet,WDT_CBX_SV_INDEX,&index,0);
			APP_SYS_API_SetSysInfo(APP_SYS_VIDEOOUT_e,index);
		}
		break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_CBX_Transparence_AvSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VideoOut_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_DisplayFormat_AVSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_TrackSet_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_AVSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_DisplayFormat_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_TrackSet_AVSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_Transparence_AvSet);
			GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else
		{
			GUI_ENC_API_SetFocus(ID_CBX_TVMode_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_CBX_Transparence_AvSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_TrackSet_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_DisplayFormat_AVSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_TVMode_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_TVMode_AVSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_VideoOut_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_TVMode_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_TrackSet_AVSet))
		{
			GUI_ENC_API_SetFocus(ID_CBX_DisplayFormat_AVSet);
			GUI_ENC_API_SetValue(ID_CBX_TrackSet_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_DisplayFormat_AVSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		else
		{
			GUI_ENC_API_SetFocus(ID_CBX_Transparence_AvSet);
			GUI_ENC_API_SetValue(ID_CBX_VideoOut_AVSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_Transparence_AvSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
		}
		
		break;
	case GUI_VK_RECALL:
		APP_SYS_API_SaveSysInfo();
		if(UI_APP_API_GetbSaveFlag())
		{
			GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_SaveWait,0);
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		break;
	case GUI_VK_EXIT:
		APP_SYS_API_SaveSysInfo();
		if(UI_APP_API_GetbSaveFlag())
		{
			GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_SaveWait,0);
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		}
		break;
	default:
		break;
	}
}
#endif
