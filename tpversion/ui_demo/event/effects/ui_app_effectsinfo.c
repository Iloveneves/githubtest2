#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

static BU08 g_usb_event = 0xff;
static BU08 effectsinfo_curfilepath[FILE_PATH_LEN];
//特效提示框
void UsbList_IPI_SetPageParam(void);

void UI_APP_API_SetUSBEventType(APP_EVENT_Notify_TYPE_E event)
{
	g_usb_event = event;
}

#ifdef ENABLE_PVR
void UI_APP_RecDiskFull_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args)
{

	APP_PRINT_DEBUG("UI_APP_RecDiskFull_CallBack\n");
	APP_PRINT_DEBUG("u32ChnID = %d,EventType = %d,s32EventValue = %x,*arg = %d\n",u32ChnID,EventType,s32EventValue,*((BU32*)args));

	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_REC_DISK_Full, 0, 0);
	/*UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo, WDT_STA_SV_STRING,(BU32)"磁盘已满", 0);
	GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
	*/
}

void UI_APP_RecError_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args)
{

	APP_PRINT_DEBUG("UI_APP_RecError_CallBack\n");
	APP_PRINT_DEBUG("u32ChnID = %d,EventType = %d,s32EventValue = %x,*arg = %d\n",u32ChnID,EventType,s32EventValue,*((BU32*)args));
	if (GUI_ENC_API_GetCurState() == ID_DLG_TimeShift)
		APP_PVR_API_StopRePlay(1);
	APP_PVR_API_StopRec(1);
}

void UI_APP_PlayEndOfFile_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args)
{

	APP_PRINT_DEBUG("UI_APP_PlayEndOfFile_CallBack\n");
	APP_PRINT_DEBUG("u32ChnID = %d,EventType = %d,s32EventValue = %x,*arg = %d\n",u32ChnID,EventType,s32EventValue,*((BU32*)args));
	if (GUI_ENC_API_GetCurState() != ID_DLG_PvrFullScreen && GUI_ENC_API_GetCurState() != ID_DLG_PvrList)
	{
		//GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo, WDT_STA_SV_STRING,(BU32)"播放到文件尾", 0);
		//GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
		APP_PVR_API_StartRePlay(1);
	}
}

void UI_APP_PlayStartOfFile_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args)
{

	APP_PRINT_DEBUG("UI_APP_PlayStartOfFile_CallBack\n");
	APP_PRINT_DEBUG("u32ChnID = %d,EventType = %d,s32EventValue = %x,*arg = %d\n",u32ChnID,EventType,s32EventValue,*((BU32*)args));
	if (GUI_ENC_API_GetCurState() != ID_DLG_PvrFullScreen && GUI_ENC_API_GetCurState() != ID_DLG_PvrList)
	{
	//	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo, WDT_STA_SV_STRING,(BU32)"回退到文件头", 0);
	//	GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
	    /*回到正常播放速度*/
		if (APP_PVR_API_GetPvrCurSpeedIndex() < APP_PLAYER_SPEED_NORMAL)
		{
		    FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			UI_APP_API_SetTimeShiftCurState(TIMESHIFT_NULL_STATE);
			APP_PVR_API_SetNormalSpeed();

		}
	}
	else if (GUI_ENC_API_GetCurState() == ID_DLG_PvrFullScreen)
	{
		APP_Player_API_Play(APP_Player_API_GetPlayIndex());
		Pvr_IPI_SetSpeedStyle(APP_PLAYER_SPEED_NORMAL);
	}
}

void UI_APP_PlayError_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args)
{
	APP_PRINT_DEBUG("UI_APP_PlayError_CallBack\n");
	APP_PRINT_DEBUG("u32ChnID = %d,EventType = %d,s32EventValue = %x,*arg = %d\n",u32ChnID,EventType,s32EventValue,*((BU32*)args));
//	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo, WDT_STA_SV_STRING,(BU32)"播放内部错误", 0);
	if (GUI_ENC_API_GetCurState() == ID_DLG_PvrFullScreen || GUI_ENC_API_GetCurState() == ID_DLG_PvrList)
	{
		APP_Player_API_Stop();
	}
	else
	{
		APP_PVR_API_StopRePlay(1);
	}

	//GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
}

void UI_APP_PlayReachRec_CallBack(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args)
{
    BU32 rec_time;
	APP_PRINT_DEBUG("UI_APP_PlayReachRec_CallBack\n");
	APP_PRINT_DEBUG("u32ChnID = %d,EventType = %d,s32EventValue = %x,*arg = %d\n",u32ChnID,EventType,s32EventValue,*((BU32*)args));
	if (GUI_ENC_API_GetCurState() != ID_DLG_PvrFullScreen && GUI_ENC_API_GetCurState() != ID_DLG_PvrList)
	{
	    /*回到正常播放速度并跳转到当前录制时间*/
		if (APP_PVR_API_GetPvrCurSpeedIndex() > APP_PLAYER_SPEED_NORMAL)
		{
		    FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			UI_APP_API_SetTimeShiftCurState(TIMESHIFT_NULL_STATE);
            if (APP_FAIL ==APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_RECTIME, &rec_time))
            {   
                APP_PRINT_ERROR("UI_APP_PlayReachRec_CallBack===GetPVRAttribute error\n");
                return;
            }
			APP_PVR_API_SetNormalSpeed();
			APP_PVR_API_PlaySeek(u32ChnID, 1, rec_time);
		}
	}
}

#endif

void UI_APP_EffectsInfo_Enter(void)
{
	/*if(UI_APP_API_GetDelFileNo() == 0)
	{
		GUI_ENC_API_SetValue(ID_TIM_EffectsInfo, WDT_TIM_SV_PLAY,0,0);
	}
	*/
	
	//删除文件操作，则把定时器关掉

	if (g_usb_event != APP_EVENT_NOTIFY_FILE_DELETE)
	{
		GUI_ENC_API_SetValue(ID_TIM_EffectsInfo, WDT_TIM_SV_PLAY,0,0);
	}
}
void UI_APP_EffectsInfo_End(void)
{

}
void UI_APP_EffectsInfo_Key(BU32 key)
{
#ifdef ENABLE_USB
	BU32 file_no;
	BU32 count;
	BU32 index;
	APP_File_Info_S file_info;
	BU32 len;
	BU08 file_path[FILE_PATH_LEN];
	if(g_usb_event == APP_EVENT_NOTIFY_FILE_DELETE)
	{
		switch(key)
		{
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
			break;
		case GUI_VK_SELECT:
			APP_USB_API_GetCurSubFileInfo(UI_APP_API_GetDelFileNo() - 1,&file_info);
			if(file_info.FileType == 0)
			{
				if(APP_USB_API_DeleteDir(file_info.FilePath))
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"文件目录删除成功",0);
					APP_Printf("Delete Dir Success \n");
				}
				else
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"目录不是空目录，无法删除",0);
					APP_PRINT_DEBUG("Delete Dir Fail \n");
					break;
				}
			}
			else
			{
					APP_Printf("Delete File\n");
				if(APP_USB_API_DeleteFile(file_info.FilePath))
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"文件删除成功",0);
					APP_Printf("Delete File Success \n");
				}
				else
				{
					GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"文件删除失败",0);
					APP_PRINT_DEBUG("Delete File Fail \n");
					break;
				}
			}
	
			APP_USB_API_SaveCurDirInfo();
			count = APP_USB_API_GetCurFileCount();
			if(count == 0)//当删除该目录下的所有东西时，跳到上一个目录
			{
				if(APP_USB_API_GetCurParentDirPath(file_path) == APP_FAIL)
				{
					return;
				}
				APP_USB_API_SetCurPath(file_path);
				APP_USB_API_SaveCurDirInfo();
				UsbManagerList_IPI_ShowCurPage();	
				len = strlen(file_path);
				if(len == 4)
				{
					sprintf(effectsinfo_curfilepath,"/");
				}
				else if(len > 4)
				{
					strncpy(effectsinfo_curfilepath,file_path+4,sizeof(file_path)-4);
				}
				else
				{
					return;
				}
			//	GUI_ENC_API_Update(ID_STA_UsbListBk_Usb, 1);
			//	GUI_ENC_API_SetValue(ID_STA_Usb, WDT_STA_SV_STRING,(BU32)effectsinfo_curfilepath, 0);
			//	GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
			}
			else
			{   
			    file_no = APP_USB_API_GetCurDirCurFileNo();
				
                if(file_no == (count+2))
                {
                    
                    file_no = file_no - 1;
					APP_USB_API_SetCurDirCurFileNo(file_no);
					APP_PRINT_DEBUG("APP_USB_API_GetCurDirCurFileNo():file_no = %d===== count = %d ========\n",file_no, count);
                }
			}
			break;
		default:
			break;
		}
		g_usb_event = APP_EVENT_NOTIFY_OTHER;
		UI_APP_API_SetDelFileNo(0);
		GUI_ENC_API_SetValue(ID_TIM_EffectsInfo, WDT_TIM_SV_PLAY,0,0);
	}
#endif
}
void UI_APP_EffectsInfo_Timer(void)
{
	BU32 pre_state = GUI_ENC_API_GetPrevState();
	APP_PRINT_DEBUG("pre_state =%d==ID_DLG_EffectsInfo=%d=",pre_state,ID_DLG_EffectsInfo);
	GUI_ENC_API_SetValue(ID_TIM_EffectsInfo, WDT_TIM_SV_STOP,0,0);
	if (g_usb_event == APP_EVENT_NOTIFY_USB_INSERT)
	{
		GUI_ENC_API_SwitchState(pre_state, 1);
		return;
	}
	else if (g_usb_event == APP_EVENT_NOTIFY_USB_EVULSION)
	{
		switch(pre_state)
		{
#ifdef ENABLE_PVR
		case ID_DLG_Player:
			GUI_ENC_API_SwitchState(ID_DLG_Effects,1);
			APP_Com_API_StopProg(1);
			APP_player_API_AttachAV();
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			//UI_APP_API_SetVolume();
			break;
#endif
#ifdef ENABLE_USB
		case ID_DLG_UsbManager:
			GUI_ENC_API_SwitchState(ID_DLG_Effects,1);
			APP_Com_API_StopProg(1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			//UI_APP_API_SetVolume();
			break;
#endif
#ifdef ENABLE_JPEG
		case ID_DLG_PhotoFrame:
		case ID_DLG_PictureShow:
			GUI_ENC_API_SwitchState(ID_DLG_Effects,1);
			APP_Com_API_StopProg(1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			//UI_APP_API_SetVolume();
			break;
#endif
#ifdef ENABLE_PVR
		case ID_DLG_PvrList:
		case ID_DLG_PvrFullScreen:
			GUI_ENC_API_SwitchState(ID_DLG_Effects,1);
			APP_player_API_AttachAV();
			APP_Com_API_StopProg(1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			//UI_APP_API_SetVolume();
			break;
		case ID_DLG_TimeShift:
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
			APP_Com_API_StopProg(1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			//UI_APP_API_SetVolume();
			break;
#endif
		default:
			GUI_ENC_API_SwitchState(pre_state, 1);
			break;
		}
	
	}
	else if (g_usb_event == APP_EVENT_NOTIFY_PLAY_RESERVE)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
	else if (g_usb_event == APP_EVENT_NOTIFY_SYS_UPDATE)
	{
		if (APP_NIT_API_GetOTACheck())
		{
			APP_SYS_API_WirteUpdateFlag();
		}
		else 
		{
			GUI_ENC_API_SwitchState(ID_DLG_SysUpdate, 1);
		}
			
	}
#ifdef ENABLE_PVR
	else if (g_usb_event == APP_EVENT_NOTIFY_REC_STOP)
	{
		switch(pre_state)
		{
		case ID_DLG_TimeShift:
			GUI_ENC_API_SetValue(ID_TIM_TimeShift, WDT_TIM_SV_STOP, 0, 0);
			APP_PVR_API_StopRePlay(1);
			APP_PVR_API_StopRec(1);
			APP_PVR_API_SetDemux(0);
			APP_PVR_API_AttachAV();
			APP_Com_API_StopProg(0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			break;
		default:
			APP_PVR_API_StopRec(1);
			GUI_ENC_API_SwitchState(pre_state, 1);
			break;
		}

	}
	else if (g_usb_event == APP_EVENT_NOTIFY_DISK_FULL)
	{
		switch(pre_state)
		{
		case ID_DLG_TimeShift:
			GUI_ENC_API_SetValue(ID_TIM_TimeShift, WDT_TIM_SV_STOP, 0, 0);
			APP_PVR_API_StopRePlay(1);
			APP_PVR_API_StopRec(1);
			APP_PVR_API_SetDemux(0);
			APP_PVR_API_AttachAV();
			APP_Com_API_StopProg(0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			break;
		default:
			APP_PVR_API_StopRec(1);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			break;
		}
	}
#endif
	else if (g_usb_event == APP_EVENT_NOTIFY_OTHER)
	{
	    APP_PRINT_DEBUG("g_usb_event == APP_EVENT_NOTIFY_OTHER==pre_state =%d==ID_DLG_FullScreen=%d=",pre_state,ID_DLG_EffectsInfo);
		GUI_ENC_API_SwitchState(pre_state, 1);
	}
}

