#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


#define PAGE_PVR_NUM 7
#define FILE_NAME_LEN 40


static BU08 g_pvrList_fileName[PAGE_PVR_NUM][FILE_NAME_LEN];
static BU08 *g_pvr_list[PAGE_PVR_NUM] =
{
	g_pvrList_fileName[0],
	g_pvrList_fileName[1],
	g_pvrList_fileName[2],
	g_pvrList_fileName[3],
	g_pvrList_fileName[4],
	g_pvrList_fileName[5],
	g_pvrList_fileName[6]
};

static BU32 g_pvrList_curPage = 0;
static BU32 g_pvrList_pageNum = 0;
static BU32 g_pvrList_rest= 0;

void Pvr_IPI_ShowFileInfoByFileName(BS08 *filename)
{
    APP_File_EventInfo_S fileinfo;
	BU32 file_size;
	static BU08 file_size_str[11];
	static BU08 usb_date[60];
	if(APP_FAIL == APP_USB_API_GetFileInfoByFileName(filename, &fileinfo))
	{
	    GUI_ENC_API_SetValue(ID_STA_Line1_PvrList, WDT_STA_SV_STRING,"", 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_PvrList, WDT_STA_SV_STRING,"", 0);
		return;
	}
    file_size = fileinfo.FileSize;
	if(file_size < 1024 && file_size >= 0)
	{
		sprintf(file_size_str,"%d B",file_size);
	}
	else if(file_size >= 1024 && file_size < 1048576)
	{
		file_size = file_size >> 10;
		sprintf(file_size_str,"%d KB",file_size);
	}
	else if(file_size >= 1048576 && file_size < 1073741824)
	{
		file_size = file_size >> 20;
		sprintf(file_size_str,"%d MB",file_size);
	}
	else
	{
		file_size = file_size >> 30;
		sprintf(file_size_str,"%d GB",file_size);
	}
	GUI_ENC_API_SetValue(ID_STA_Line1_PvrList, WDT_STA_SV_STRING,(BU32)file_size_str, 0);
	//显示文件修改时间
	sprintf(usb_date,"%04d-%02d-%02d  %02d:%02d",fileinfo.FileTime.tm_year,fileinfo.FileTime.tm_mon,fileinfo.FileTime.tm_mday,fileinfo.FileTime.tm_hour,fileinfo.FileTime.tm_min);	
	GUI_ENC_API_SetValue(ID_STA_Line2_PvrList,WDT_STA_SV_STRING,(BU32)usb_date,0);
}
/*----------------------------------------------------------------------------
描述：每次创建列表成功后调用
-----------------------------------------------------------------------------*/
void PvrList_IPI_SetPageParam(void)
{
	BU16 pvrNum,curPvrNo;
	pvrNum = APP_Player_API_GetCurTypeCount();
	curPvrNo = APP_Player_API_GetPlayIndex();
	g_pvrList_pageNum = (pvrNum +  PAGE_PVR_NUM -1) / PAGE_PVR_NUM;	
	g_pvrList_rest = pvrNum % PAGE_PVR_NUM;
	g_pvrList_curPage = curPvrNo / PAGE_PVR_NUM;
	
	if(g_pvrList_rest == 0)
	{
		g_pvrList_rest = PAGE_PVR_NUM;
	}

	if(pvrNum == 0)
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_PvrList, WDT_SBR_SV_MAX, (BU32)pvrNum, 0);}
	else
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_PvrList, WDT_SBR_SV_MAX, (BU32)pvrNum-1, 0);}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_PvrList, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_FileList_PvrList, WDT_SBR_SV_VALUE, curPvrNo, 0);
	
}

void PvrList_IPI_ShowCurPage(void)
{
	Player_Film_Info_S film_Info;
	BU16 PvrNo,i;
	BU16 pageNum;
	APP_PLAYER_TYPE_E playerType;
	APP_PLAYER_GROUP_TYPE_E playerGroup;
	playerType = APP_Player_API_GetCurPlayerType();
	playerGroup = APP_PLAYER_GROUP_TYPE_ALL_e;
	
	if(g_pvrList_pageNum == 0)
	{
	    GUI_ENC_API_SetValue(ID_LBP_FileList_PvrList, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_pvrList_curPage >= g_pvrList_pageNum)
	{
		g_pvrList_curPage = 0;
	}
	
	if(g_pvrList_curPage+1 == g_pvrList_pageNum)
	{	
		pageNum = g_pvrList_rest;
	}
	else
	{
		pageNum = PAGE_PVR_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		PvrNo = i+g_pvrList_curPage*PAGE_PVR_NUM;
		if(APP_FAIL == APP_Player_API_GetPlayerFile(playerType, playerGroup, PvrNo, &film_Info))
		{
			GUI_ENC_API_SetValue(ID_LBP_FileList_PvrList, WDT_LBP_SV_DATA, (BU32)g_pvr_list, i);
			return;
		}
		APP_PLAYER_API_GetRelativePath(film_Info.FileName, g_pvrList_fileName[i]);
		g_pvrList_fileName[i][30] = '\0';
	}
	GUI_ENC_API_SetValue(ID_LBP_FileList_PvrList, WDT_LBP_SV_DATA, (BU32)g_pvr_list, pageNum);
}

void UI_APP_PvrList_Enter(void)
{
    BU16 pvrCount;
    BU16 pvrNum;
	BU16 index;
	BU32 hwnd;
	GUI_Rect_s rect;
	Player_Film_Info_S film_Info;
    APP_PLAYER_TYPE_E playerType;
	APP_PLAYER_GROUP_TYPE_E playerGroup;
	playerType = APP_Player_API_GetCurPlayerType();
	playerGroup = APP_PLAYER_GROUP_TYPE_ALL_e;

	pvrCount = APP_Player_API_GetCurTypeCount();

	if(pvrCount == 0)
	{
	    GUI_ENC_API_SetValue(ID_LBP_FileList_PvrList, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_PvrList, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_PvrList, WDT_SBR_SV_MIN, 0, 0); 
		GUI_ENC_API_SetValue(ID_SBR_FileList_PvrList, WDT_SBR_SV_VALUE, 0, 0);
	}
    else
    {
        
		PvrList_IPI_SetPageParam();
		PvrList_IPI_ShowCurPage();
		hwnd = GUI_API_GetDlgItem(ID_VFM_PvrList);
		GUI_API_GetWindowRect(hwnd,&rect);
		APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
		pvrNum = APP_Player_API_GetPlayIndex();
		GUI_ENC_API_SetValueUnPaint(ID_LBP_FileList_PvrList, WDT_LBP_SV_INDEX, pvrNum % PAGE_PVR_NUM, 0);
		APP_Player_API_Play(pvrNum);
		
		if(APP_SUCCESS == APP_Player_API_GetPlayerFile(playerType, playerGroup, pvrNum, &film_Info))
		{
		    Pvr_IPI_ShowFileInfoByFileName((BS08 *)film_Info.FileName);/*显示文件信息*/
        }
	    else
	    {
	        GUI_ENC_API_SetValue(ID_STA_Line1_PvrList, WDT_STA_SV_STRING,"", 0);
		    GUI_ENC_API_SetValue(ID_STA_Line2_PvrList, WDT_STA_SV_STRING,"", 0);
	    }
    }
	
}

void UI_APP_PvrList_End(void)
{

}

void UI_APP_PvrList_LbpKey(BU32 key)
{
    BU32 index;
	BU32 PvrNo;
	BU32 keyState;
	Player_Film_Info_S film_Info;
	APP_PLAYER_TYPE_E playerType;
	APP_PLAYER_GROUP_TYPE_E playerGroup;
	playerType = APP_Player_API_GetCurPlayerType();
	playerGroup = APP_PLAYER_GROUP_TYPE_ALL_e;
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		GUI_ENC_API_GetValue(ID_LBP_FileList_PvrList,WDT_LBP_SV_INDEX,&index,0);
		PvrNo = (index + g_pvrList_curPage*PAGE_PVR_NUM);
		GUI_ENC_API_SetValue(ID_SBR_FileList_PvrList, WDT_SBR_SV_VALUE,PvrNo, 0);
        APP_Player_API_SetPlayIndex(PvrNo);//设置播放的索引
        if(APP_SUCCESS == APP_Player_API_GetPlayerFile(playerType, playerGroup, PvrNo, &film_Info))
		{
		    Pvr_IPI_ShowFileInfoByFileName((BS08 *)film_Info.FileName);/*显示文件信息*/
        }
	    else
	    {
	        GUI_ENC_API_SetValue(ID_STA_Line1_PvrList, WDT_STA_SV_STRING,"", 0);
		    GUI_ENC_API_SetValue(ID_STA_Line2_PvrList, WDT_STA_SV_STRING,"", 0);
	    }
		keyState = GUI_ENC_API_GetKeyState();
		if (!keyState)
		{
			APP_Player_API_Play(PvrNo);
		}
		
		break;
	case GUI_VK_SELECT:
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &index);
		GUI_ENC_API_SetValue(ID_VOI_PvrFullScreen, WDT_VOI_SV_RIGHT, index, 0);
		GUI_ENC_API_SwitchState(ID_DLG_PvrFullScreen, 1);
		break;
	case GUI_VK_EXIT:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		APP_Player_API_Stop();
		APP_player_API_AttachAV();
		
		APP_Com_API_StopProg(0);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		FYF_API_thread_sleep(100);
	//	UI_APP_API_SetVolume();
		break;
	default:
		break;

    }
}
void UI_APP_PvrList_Change(void)
{
    BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_FileList_PvrList, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_pvrList_curPage++;
			break;
		case -1:
			if(g_pvrList_curPage == 0)
				g_pvrList_curPage = g_pvrList_pageNum -1;
			else
				g_pvrList_curPage--;
			
			if(g_pvrList_curPage == g_pvrList_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_FileList_PvrList, WDT_LBP_SV_INDEX, PAGE_PVR_NUM-1, 0);
			}
			break;
		default:
			break;
	}
	PvrList_IPI_ShowCurPage();//显示当前页
}