#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


#define PAGE_PLAYER_NUM 7
#define FILE_NAME_LEN 40

#define PAGE_DISPLAY_NUM 3
#define DISPLAY_NAME_LEN 30

static BU32 player_bplay = 0;
static BU08 player_filename[20] ={0};
static BU08 player_times[40];
static BU08 player_curtime[40];
static BU08 player_mp3_info[100] = {0};

BU08 temp[100];

static BU32 player_curpage = 0;
static BU32 player_pagenum = 0;
static BU32 player_rest = 0;
static BU08 player_filenames[PAGE_PLAYER_NUM][FILE_NAME_LEN];
static BU08 *player_filenames_str[PAGE_PLAYER_NUM] =
{
	player_filenames[0],
	player_filenames[1],
	player_filenames[2],
	player_filenames[3],
	player_filenames[4],
	player_filenames[5],
	player_filenames[6]
};

typedef struct _PLAYER_DISPLAY_OPIONT_S
{
    BU08 *itemName;
	BU08 select;
}PLAYER_DISPLAY_OPIONT_S;

static BU32 g_player_displayCurIndex = 0;
static BU08 g_player_displaySelect[PAGE_DISPLAY_NUM] = {1, 1, 1}; 
static BU08 g_player_displayitemNames[PAGE_DISPLAY_NUM][DISPLAY_NAME_LEN];


static PLAYER_DISPLAY_OPIONT_S g_display_OptionItem_s[PAGE_DISPLAY_NUM] =
{
    {g_player_displayitemNames[0], 0},
    {g_player_displayitemNames[1], 0},
    {g_player_displayitemNames[2], 0}
};


static BU08 g_player_displaySetShowFlag = 0;/*显示设置窗口隐藏与否标志*/
static BU08 g_player_fileListShowFlag = 1;  /*文件列表隐藏与否标志*/
static BU08 g_player_playerBarShowFlag = 1; /*PlayerBar隐藏与否标志*/
#if 1
void Player_ShowChangeToNew(void)
{
	Player_Time_S times;
	Player_Time_S curtime;
	BU08 mp3name[32], mp3singer[32], mp3special[32];
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_ARTIST_e, (void*)mp3singer);
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_SONGNAME_e, (void*)mp3name);
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_SPECIAL_e, (void*)mp3special);
	APP_Printf("singer = %s, mp3name = %s\n", mp3singer, mp3name);
	if (mp3singer[0] == 0 && mp3name[0] == 0)
	{
		sprintf(player_mp3_info," 无法获取歌曲信息");
	}
	else
	{
		sprintf(player_mp3_info," %s-%s", mp3singer, mp3name);
	}		
	GUI_ENC_API_SetValue(ID_STA_NameAndArtist_Player, WDT_STA_SV_STRING, (BU32)player_mp3_info, 0);
	GUI_ENC_API_SetValue(ID_PBR_Player,WDT_PBR_SV_CHANGE,0,0);
}
#endif

void Player_UpdateProgress()
{

	Player_Time_S curtime, times;
	BU32 cursec, totalsec;
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_TIMES_e, (void*)&times);
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURTIME_e,(void*)&curtime);
	cursec = curtime.hour * 3600 + curtime.min * 60 + curtime.sec;
	totalsec = times.hour * 3600 + times.min * 60 + times.sec;
	if (totalsec != 0)
		GUI_ENC_API_SetValue(ID_PBR_Player,WDT_PBR_SV_CHANGE,(BU32)(cursec * 400 / totalsec),0);
}

/*显示或隐藏文件列表设置*/
void Player_IPI_ShowFileList(BU08 showFlag)
{
    BU32 showState = 0;
	BU32 i = 0;

	GUI_WinID_Demo_e fileList[4] = {ID_STA_PlayerListBk_Player,ID_STA_Title_Player,ID_LBP_Player,
		                            ID_SBR_Player};

	GUI_ENC_API_SetVisible(ID_STA_PlayerFileListShowBK_Player, WS_VISIBLE); 	
	GUI_ENC_API_Update(ID_STA_PlayerFileListShowBK_Player, 1);
	if(showFlag)
		{showState = WS_VISIBLE;}
	else
		{showState = WS_DISABLE;}
	for(i = 0; i < 4; i++)
	{
		GUI_ENC_API_SetVisible(fileList[i], showState);
		if(showFlag)
			{GUI_ENC_API_Update(fileList[i] ,1);}
	}
}

/*显示或隐藏显示设置窗口设置*/
void Player_IPI_ShowDisplaySet(BU08 showFlag)
{
    BU32 showState = 0;
	BU32 i = 0;

	GUI_WinID_Demo_e DisplaySet[3] = {ID_STA_DisplaySet_Player,ID_STA_DisplaySetTitle_Player,ID_LBP_DisplaySet_Player};

	GUI_ENC_API_SetVisible(ID_STA_PlayerDisplayShowBk_Player, WS_VISIBLE); 	
	GUI_ENC_API_Update(ID_STA_PlayerDisplayShowBk_Player, 1);
	if(showFlag)
		{showState = WS_VISIBLE;}
	else
		{showState = WS_DISABLE;}
	for(i = 0; i < 3; i++)
	{
		GUI_ENC_API_SetVisible(DisplaySet[i], showState);
		if(showFlag)
			{GUI_ENC_API_Update(DisplaySet[i] ,1);}
	}
}

/*显示或隐藏PlayerBar设置*/
void Player_IPI_ShowPlayerBar(BU08 showFlag)
{
    BU32 showState = 0;
	BU32 i = 0;

	GUI_WinID_Demo_e PlayerBar[8] = {ID_STA_PlayerBk_Player, ID_STA_Player,ID_PBR_Volume_Player,
		                             ID_PBR_Player, ID_STA_CTL_Player, ID_STA_NameAndArtist_Player, 
		                             ID_STA_DisplaySetIcon_Player, ID_STA_DisplaySetText_Player};

	GUI_ENC_API_SetVisible(ID_STA_PlayerBk_Player, WS_VISIBLE);	
	GUI_ENC_API_Update(ID_STA_PlayerBk_Player, 1);
	if(showFlag)
		{showState = WS_VISIBLE;}
	else
		{showState = WS_DISABLE;}
	for(i = 0; i < 8; i++)
	{
		GUI_ENC_API_SetVisible(PlayerBar[i], showState);
		if(showFlag)
			{GUI_ENC_API_Update(PlayerBar[i] ,1);}
	}
}


void PlayerList_IPI_SetPageParam(void)
{
	BU32  count,curfile;
	count = APP_Player_API_GetCurTypeCount();
	APP_Printf("count = %d\n",count);
	player_pagenum = (count +  PAGE_PLAYER_NUM -1) / PAGE_PLAYER_NUM;	
	player_rest = count % PAGE_PLAYER_NUM;
	player_curpage = 0;
	if(player_rest == 0)
	{
		player_rest = PAGE_PLAYER_NUM;
	}

	if(count == 0)
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_Player, WDT_SBR_SV_MAX, 0, 0);}
	else
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_Player, WDT_SBR_SV_MAX, (BU32)count-1, 0);}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_Player, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_Player, WDT_SBR_SV_VALUE, 0, 0);
	GUI_ENC_API_SetValue(ID_STA_NameAndArtist_Player, WDT_STA_SV_STRING, " ", 0);
	GUI_ENC_API_SetValue(ID_PBR_Player, WDT_PBR_SV_CHANGE, 0, 0);
	
	
}

void PlayerList_IPI_ShowCurPage(void)
{
	Player_Mp3File_S mp3info;
	Player_Film_Info_S film_info;
	BU32 index,i;

	BU32 pagenum;
	APP_PLAYER_TYPE_E PlayerType;
	APP_PLAYER_GROUP_TYPE_E PlayerGroupType = APP_PLAYER_GROUP_TYPE_ALL_e;
	PlayerType = APP_Player_API_GetCurPlayerType();

	if(player_pagenum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Player, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(player_curpage >= player_pagenum)
	{
		player_curpage = 0;
	}
	
	if(player_curpage+1 == player_pagenum)
	{	
		pagenum = player_rest;
	}
	else
	{
		pagenum = PAGE_PLAYER_NUM;
	}
	
	for(i=0; i<pagenum; i++)
	{	
		index = i + player_curpage*PAGE_PLAYER_NUM;

        if(APP_FAIL == APP_Player_API_GetPlayerFile(PlayerType,PlayerGroupType,index,&mp3info))
		{
			GUI_ENC_API_SetValue(ID_LBP_Player, WDT_LBP_SV_DATA, (BU32)player_filenames_str, i);
			return;
		}
		APP_PLAYER_API_GetRelativePath(mp3info.filename, player_filenames_str[i]);
		player_filenames[i][18]='\0';
	}
	GUI_ENC_API_SetValue(ID_LBP_Player, WDT_LBP_SV_DATA, (BU32)player_filenames_str, pagenum);
}

void Player_IPI_ShowDisplayOptionPage(void)
{
     
	 char **pDic = DicData;
	 BU32 lang;
	 BU08 index;
 
	 APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	 if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	 {
		 pDic = DicDataEnglish;
	 }
	 else
	 {
		 pDic = DicData;
	 }

	 sprintf(g_player_displayitemNames[0], pDic[RS_LRCLyric]);
	 sprintf(g_player_displayitemNames[1], pDic[RS_FileList]);
	 sprintf(g_player_displayitemNames[2], pDic[RS_PlayerBar]);

	 for(index = 0; index < PAGE_DISPLAY_NUM; index++)
	 {
	     g_display_OptionItem_s[index].select = g_player_displaySelect[index];
	 }
	 GUI_ENC_API_SetValue(ID_LBP_DisplaySet_Player, WDT_LBP_SV_DATA, (BU32)g_display_OptionItem_s, PAGE_DISPLAY_NUM);
}
void UI_APP_Player_Enter(void)
{
	BU32 i;
	BU32 count = 2;
	BU32 volume;
	APP_PLAYER_TYPE_E play_type = APP_PLAYER_TYPE_MP3_e;
	player_bplay = 0;
	APP_EPG_API_StopGetEpg();
	APP_Com_API_SetFullScreen();
	GUI_ENC_API_SetFocus(ID_LBP_Player);


	GUI_ENC_API_SetValue(ID_TIM_Player, WDT_TIM_SV_PLAY, 0, 0);
	volume = APP_Player_API_GetVolume();
	APP_Audio_API_SetVolume(volume);
	GUI_ENC_API_SetValue(ID_PBR_Volume_Player, WDT_PBR_SV_CHANGE, volume*5, 0);
	
	count = APP_Player_API_GetCurTypeCount();
	APP_Printf("APP_Player_API_GetCurTypeCount = %d\n", count);
	if(count == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Player, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_Player, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_Player, WDT_SBR_SV_MIN, 0, 0); 
		GUI_ENC_API_SetValue(ID_SBR_Player, WDT_SBR_SV_VALUE, 0, 0);
		
		GUI_ENC_API_SetValue(ID_STA_NameAndArtist_Player, WDT_STA_SV_STRING, " ", 0);
		GUI_ENC_API_SetValue(ID_PBR_Player, WDT_PBR_SV_CHANGE, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK1_W113_H113, 0);
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint(ID_LBP_Player, WDT_LBP_SV_INDEX, 0, 0);
		PlayerList_IPI_SetPageParam();
		PlayerList_IPI_ShowCurPage();
		Player_IPI_ShowDisplayOptionPage();
	}
}

void UI_APP_Player_End(void)
{
}
void UI_APP_Player_Key(BU32 key)
{
	BU32 index;
	BU32 fileno;
	BU32 volume;
	BU32 keyState;
	switch(key)
	{
	case GUI_VK_SELECT:
		
		GUI_ENC_API_GetValue(ID_LBP_Player,WDT_LBP_SV_INDEX,&index,0);
		fileno = (index + player_curpage*PAGE_PLAYER_NUM);
		APP_Printf("index = %d, fileno = %d\n", index, fileno);
		
		if(player_bplay == 0)
		{
			player_bplay = 1;
			GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK2_W113_H113, 0);
			APP_Player_API_Resume();
		}
		else
		{
			player_bplay = 0;
			GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK1_W113_H113, 0);
			APP_Player_API_Pause();
		}
		break;
	case GUI_VK_UP:
		GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK3_W113_H113, 0);
		GUI_ENC_API_PostMessage(ID_LBP_Player, WM_KEY, GUI_VK_UP, 0);
		keyState = GUI_ENC_API_GetKeyState();
		if (!keyState)
		{
			APP_Player_API_PlayPre();
			Player_ShowChangeToNew();
		//	GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK2_W113_H113, 0);
		}
		break;
	case GUI_VK_DOWN:
		GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK5_W113_H113, 0);
		GUI_ENC_API_PostMessage(ID_LBP_Player, WM_KEY, GUI_VK_DOWN, 0);
		keyState = GUI_ENC_API_GetKeyState();
		if (!keyState)
		{
			APP_Player_API_PlayNext();
			Player_ShowChangeToNew();
		//	GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK2_W113_H113, 0);
		}
		break;
	case GUI_VK_LEFT:
		GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK6_W113_H113, 0);
		keyState = GUI_ENC_API_GetKeyState();
		if(!keyState)
		{
		//    GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK2_W113_H113, 0);
		}
	    APP_Player_API_Rewind();
		if(player_bplay == 0)
		{
		   APP_Player_API_Resume();
		}
		break;
	case GUI_VK_RIGHT:
		GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK4_W113_H113, 0);
		keyState = GUI_ENC_API_GetKeyState();
		if(!keyState)
		{
		 //   GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK2_W113_H113, 0);
		}
	    APP_Player_API_Speed();
		if(player_bplay == 0)
		{
		   APP_Player_API_Resume();
		}
		break;
	case GUI_VK_V_UP:
		APP_Player_API_AddVolume();
		volume = APP_Player_API_GetVolume();
		GUI_ENC_API_SetValue(ID_PBR_Volume_Player, WDT_PBR_SV_CHANGE, volume*5, 0);
		break;
	case GUI_VK_V_DOWN:
		APP_Player_API_ReduceVolume();
		volume = APP_Player_API_GetVolume();
		GUI_ENC_API_SetValue(ID_PBR_Volume_Player, WDT_PBR_SV_CHANGE, volume*5, 0);
		break;
	case GUI_VK_MUTE:
		if(APP_PLAYER_MUTE_STATE == APP_Player_API_GetMuteState())
		{
			APP_Player_API_SetMuteState(APP_PLAYER_UNMUTE_STATE);
			UI_APP_API_SetVolume();
		}
		else
		{
			APP_Player_API_SetMuteState(APP_PLAYER_MUTE_STATE);
		}
		break;
	case GUI_VK_GREEN:
		if(g_player_displaySetShowFlag)
		{g_player_displaySetShowFlag = 0;}
		else
		{
			g_player_displaySetShowFlag = 1;
			GUI_ENC_API_SetFocus(ID_LBP_DisplaySet_Player);
		}
		Player_IPI_ShowDisplaySet(g_player_displaySetShowFlag);
		break;
	case GUI_VK_EXIT:
	case GUI_VK_RECALL:
		APP_Player_API_Stop();
		APP_player_API_AttachAV();
		APP_Com_API_StopProg(1);//黑屏
		UI_APP_API_SetVolume();
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		break;
	default:
		break;
	}
}


void UI_APP_PlayerFileList_OnKey(BU32 key)
{

	BU32 index;
	BU32 fileno;
	switch(key)
	{
	case GUI_VK_UP:

		GUI_ENC_API_GetValue(ID_LBP_Player,WDT_LBP_SV_INDEX,&index,0);
		fileno = (index + player_curpage*PAGE_PLAYER_NUM);
		GUI_ENC_API_SetValue(ID_SBR_Player, WDT_SBR_SV_VALUE,fileno, 0);

		break;
	case GUI_VK_DOWN:

		GUI_ENC_API_GetValue(ID_LBP_Player,WDT_LBP_SV_INDEX,&index,0);
		fileno = (index + player_curpage*PAGE_PLAYER_NUM);
		GUI_ENC_API_SetValue(ID_SBR_Player, WDT_SBR_SV_VALUE,fileno, 0);

		break;
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		GUI_ENC_API_GetValue(ID_LBP_Player,WDT_LBP_SV_INDEX,&index,0);
		fileno = (index + player_curpage*PAGE_PLAYER_NUM);
		GUI_ENC_API_SetValue(ID_SBR_Player, WDT_SBR_SV_VALUE,fileno, 0);
		
		break;
	case GUI_VK_SELECT:
		if (APP_Player_API_GetCurTypeCount() == 0)
		{
			break;
		}
		GUI_ENC_API_GetValue(ID_LBP_Player,WDT_LBP_SV_INDEX,&index,0);
		fileno = (index + player_curpage*PAGE_PLAYER_NUM);
	
		GUI_ENC_API_SetValue(ID_STA_CTL_Player, WDT_STA_SV_STYLE, (BU32)STYLE_Player_CTLBK2_W113_H113, 0);
		APP_Player_API_Play(fileno);
		player_bplay = 1;
		GUI_ENC_API_SetFocus(ID_DLG_Player);
		Player_ShowChangeToNew();
		break;
	case GUI_VK_GREEN:
	if(g_player_displaySetShowFlag)
		{g_player_displaySetShowFlag = 0;}
		else
		{
			g_player_displaySetShowFlag = 1;
			GUI_ENC_API_SetFocus(ID_LBP_DisplaySet_Player);
		}
		Player_IPI_ShowDisplaySet(g_player_displaySetShowFlag);
		break;
	case GUI_VK_EXIT:
	case GUI_VK_RECALL:
		APP_Player_API_Stop();
		APP_player_API_AttachAV();

		APP_Com_API_StopProg(1);//黑屏
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		UI_APP_API_SetVolume();
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		break;
	default:
		break;
	}

}


void UI_APP_PlayerFileList_Change()
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_Player, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			player_curpage++;
			break;
		case -1:
			if(player_curpage == 0)
				player_curpage = player_pagenum -1;
			else
				player_curpage--;
			if(player_curpage == player_pagenum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Player, WDT_LBP_SV_INDEX, PAGE_PLAYER_NUM-1, 0);
			}
			break;
		default:
			break;
	}
	PlayerList_IPI_ShowCurPage();//显示当前页
}

void UI_APP_PlayerDisplay_OnKey(BU32 key)
{
    BU32 volume;
    switch(key)
    {
    case GUI_VK_SELECT:
		if(g_player_displaySelect[g_player_displayCurIndex] == 0)
		{
		    g_player_displaySelect[g_player_displayCurIndex] = 1;
		}
		else
		{
		    g_player_displaySelect[g_player_displayCurIndex] = 0;
		}

		/*显示文件列表*/
		if(g_player_displayCurIndex == 1)
		{
		     g_player_fileListShowFlag = g_player_displaySelect[g_player_displayCurIndex];
			 Player_IPI_ShowFileList(g_player_fileListShowFlag);
		}
		else if(g_player_displayCurIndex == 2)/*显示PlayerBar*/
		{
		     g_player_playerBarShowFlag = g_player_displaySelect[g_player_displayCurIndex];
			 Player_IPI_ShowPlayerBar(g_player_playerBarShowFlag);
		}
		break;
	case GUI_VK_DOWN:
		if(g_player_displayCurIndex >= PAGE_DISPLAY_NUM - 1)
		{
		    g_player_displayCurIndex = 0;
		}
		else
		{
		    g_player_displayCurIndex++;
		}
		break;
	case GUI_VK_UP:
		if(g_player_displayCurIndex <= 0)
		{
		    g_player_displayCurIndex = PAGE_DISPLAY_NUM - 1;
		}
		else
		{
		    g_player_displayCurIndex--;
		}
		break;
	case GUI_VK_P_DOWN:
		g_player_displayCurIndex = 0;
		break;
	case GUI_VK_P_UP:
		g_player_displayCurIndex = PAGE_DISPLAY_NUM - 1;
		break;
	case GUI_VK_V_UP:
		APP_Player_API_AddVolume();
		volume = APP_Player_API_GetVolume();
		GUI_ENC_API_SetValue(ID_PBR_Volume_Player, WDT_PBR_SV_CHANGE, volume*5, 0);
		break;
	case GUI_VK_V_DOWN:
		APP_Player_API_ReduceVolume();
		volume = APP_Player_API_GetVolume();
		GUI_ENC_API_SetValue(ID_PBR_Volume_Player, WDT_PBR_SV_CHANGE, volume*5, 0);
		break;
	case GUI_VK_GREEN:
		g_player_displaySetShowFlag = 0;
		GUI_ENC_API_SetFocus(ID_LBP_Player);
		Player_IPI_ShowDisplaySet(g_player_displaySetShowFlag);
		break;
	case GUI_VK_EXIT:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
	    APP_Player_API_Stop();
		APP_player_API_AttachAV();
		APP_Com_API_StopProg(1);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		UI_APP_API_SetVolume();
		break;
	default:
		break;
    }
    Player_IPI_ShowDisplayOptionPage();

}

void UI_APP_PlayerDisplay_Change(void)
{
#if 0
    BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_DisplaySet_Player, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			GUI_ENC_API_SetValueUnPaint(ID_LBP_DisplaySet_Player, WDT_LBP_SV_INDEX, 0, 0);
			break;
		case -1:
			GUI_ENC_API_SetValueUnPaint(ID_LBP_DisplaySet_Player, WDT_LBP_SV_INDEX, PAGE_DISPLAY_NUM - 1, 0);
			break;
		default:
			break;
	}
	Player_IPI_ShowDisplayOptionPage();//显示当前页
#endif
}

void UI_APP_CurTime_Player_OnPaint(BU32 hdc, BU32 lParam)
{
    Player_Time_S curtime;
	Player_Time_S totaltimes;
	BU08 player_time[80];
#if 0
	if (APP_SUCCESS == APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURTIME_e,(void*)&curtime))
	{
		sprintf(player_curtime,"%02d:%02d",curtime.min,curtime.sec);
		GUI_API_TextOut(hdc, 0, 8, player_curtime, strlen(player_curtime), COLOR_BLACK, COLOR_BLACK, 1);
		//GUI_ENC_API_SetValue(ID_STA_CurTime_Player, WDT_STA_SV_STRING, (BU32)player_curtime, 0);
	}
#endif
	if (APP_SUCCESS == APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_TIMES_e,(void*)&totaltimes))
	{
		sprintf(player_times,"%02d:%02d",totaltimes.min,totaltimes.sec);
		//GUI_ENC_API_SetValue(ID_STA_Times_Player, WDT_STA_SV_STRING, (BU32)player_times, 0);
	}

	curtime.min = (lParam % 3600)/60;
	curtime.sec = (lParam % 3600)%60;
	memset(player_time, 0, sizeof(player_time));
	sprintf(player_time,"%02d:%02d/%02d:%02d",curtime.min,curtime.sec,totaltimes.min,totaltimes.sec);
    GUI_API_DrawStyle(hdc,0,0,STYLE_PlayerTimeBk_W110_H24);
	GUI_API_TextOut(hdc, 0, 0, player_time, strlen(player_time), COLOR_BLACK, COLOR_BLACK, 1);
}


void UI_APP_Player_Timer(void)
{
	Player_Time_S times;
	Player_Time_S curtime;
	BU32 cursec = 0, totalsec = 0;
	static BU32 start_time = 0;
	BU08 curstatus;
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURSTATUS_e, &curstatus);
	if (curstatus == 1)
	{
	
		if (APP_FAIL == APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_TIMES_e, (void*)&times))
		{
			return;
		}
		if (APP_FAIL == APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURTIME_e,(void*)&curtime))
		{
			return;

		}
		cursec = curtime.hour * 3600 + curtime.min * 60 + curtime.sec;
		totalsec = times.hour * 3600 + times.min * 60 + times.sec;
		if (totalsec != 0)
			GUI_ENC_API_SetValue(ID_PBR_Player,WDT_PBR_SV_CHANGE,(BU32)(cursec * 400 / totalsec),0);
		if (cursec == totalsec)
		{
			APP_Player_API_PlayNext();
			Player_ShowChangeToNew();
			GUI_ENC_API_PostMessage(ID_LBP_Player, WM_KEY, GUI_VK_DOWN, 0);
		}
		GUI_ENC_API_Update(ID_ODC_CurTime_Player, cursec);
	}

}


#if 0
void UI_APP_Player_Timer(void)
{
	Player_Time_S times;
	Player_Time_S curtime;
	BU32 cursec, totalsec;
	static BU32 start_time = 0;
	if (player_bplay == 1)
	{
		GUI_LRC_API_LoadFile(accdahai);
		player_bplay = 2;

	}
	else if (player_bplay == 2)
	{
		if(!GUI_LRC_API_IdTagIsNull())
		{
			sprintf(temp,"[%s:%s]\n", GUI_LRC_API_GetIdTagKey(),GUI_LRC_API_GetIdTagValue());
			GUI_API_FillRect(GUI_API_GetDesktopWindow(),0,50,720,24,1000);
			GUI_API_TextOut(GUI_API_GetDesktopWindow(),50,50,temp,GUI_API_Strlen(temp),0,1000,0);	
			GUI_LRC_API_GotoNextIdTag();
		}
		else
		{
			GUI_LRC_API_StartTime();
			player_bplay = 3;
			start_time = GUI_LRC_API_GetTimeTagStartTime();
		}
	}
	else  if (player_bplay == 3)
	{
		Player_UpdateCurTime();

		APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_TIMES_e, (void*)&times);
		APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURTIME_e,(void*)&curtime);
		cursec = curtime.hour * 3600 + curtime.min * 60 + curtime.sec;
		totalsec = times.hour * 3600 + times.min * 60 + times.sec;
		GUI_ENC_API_SetValue(ID_PBR_Player,WDT_PBR_SV_CHANGE,(BU32)(cursec * 193 / totalsec),0);

		if ((!GUI_LRC_API_TimeTagIsNull()) && start_time / 100 - 2 < cursec)
		{
			sprintf(temp,"[%d:%02d.%02d] %s\n", start_time/6000, (start_time%6000)/100, (start_time%6000)%100,
				GUI_LRC_API_GetTimeTagLrc());
			GUI_API_FillRect(GUI_API_GetDesktopWindow(),0,50,720,24,1000);
			GUI_API_TextOut(GUI_API_GetDesktopWindow(),50,50,temp,GUI_API_Strlen(temp),0,1000,0);	
			GUI_LRC_API_GotoNextTimeTag();
			if (!GUI_LRC_API_TimeTagIsNull())start_time = GUI_LRC_API_GetTimeTagStartTime();
			
		}
			
		

		if (cursec == totalsec)
		{
			APP_Player_API_PlayNext();
			Player_ShowChangeToNew();
		}
	}
	else
	{
		
	}
}
#endif
