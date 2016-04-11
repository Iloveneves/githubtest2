/***************************************************************************************/
/*																					   */
/*                                                                                     */
/*                                                                                     */
/*                                                                                     */
/*  2009-08-19 cwd created                                                             */
/*	说明:播放器的通用接口实现，包括播放MP3和pvr录制的节目流							   */
/***************************************************************************************/

#include "app_common.h"
#include "app_ipi_common.h"
//#include "hi_stb_api.h"

static BU32 g_cur_mp3_no = 0xffff;
static BU32 g_cur_film_no = 0xffff;
extern APP_File_Group_Info_S g_file_group[FILE_GROUP_MAX];
static Player_MP3_Group_S g_mp3_group;
static Player_Film_Group_S g_film_group;
static BU32 g_cur_player_type = APP_PLAYER_TYPE_NULL_e;
static BU32 g_cur_player_status = APP_PLAYER_STATUS_STOP_e;

static BU32 g_mute_status = APP_PLAYER_UNMUTE_STATE;
static BU32 g_cur_player_playChn = 0xffff;
static BU32 g_cur_player_speedIndex = APP_PLAYER_SPEED_NORMAL;

//播放标志，用来表示
static BU08 g_player_curflag = 0;
static BU32 g_cur_playIndex;
static BU32 g_volume = 0xffff;

BS32 g_player_speed[] = {
	-32*APP_PLAY_SPEED_NORMAL  ,
    -16*APP_PLAY_SPEED_NORMAL ,
    -8*APP_PLAY_SPEED_NORMAL  ,
    -4*APP_PLAY_SPEED_NORMAL ,
    -2*APP_PLAY_SPEED_NORMAL ,
	APP_PLAY_SPEED_NORMAL  ,
    2*APP_PLAY_SPEED_NORMAL,
    4*APP_PLAY_SPEED_NORMAL,
    8*APP_PLAY_SPEED_NORMAL ,
    16*APP_PLAY_SPEED_NORMAL ,
    32*APP_PLAY_SPEED_NORMAL
   
};


void APP_Player_API_SetTrack(APP_PLAYER_TRACK_E track)
{
	BU32 u32track = track;
	
	FYF_API_adec_ioctl(NULL, FYF_ADEC_SET_CHANNEL_MODE, &u32track);
}

void APP_Player_API_SetVolume(BU08 volume)
{
	BU32 u32vol = volume;
	FYF_API_aout_ioctl(NULL, FYF_AOUT_SET_VOLUME, &u32vol);
}

BU32 MID_Player_IPI_PVRPlay(BU32 index)
{
	BU16 audio_pid , video_pid;
	UserData_S user_data;
	BU32 data_len;
	BS32 ret;
	/*
	if (g_cur_player_status == APP_PLAYER_STATUS_PAUSE_e)
	{
		APP_Printf("current status is pause, resume it first!\n");
		return APP_FAIL;
	}
	*/
		
	if (g_cur_player_status == APP_PLAYER_STATUS_PLAY_e || g_cur_player_status == APP_PLAYER_STATUS_PAUSE_e)
	{

		ret = FYF_API_PVR_PlayStop(g_cur_player_playChn);

		if (ret != FYF_OK)
		{
			APP_PRINT_ERROR("FYF_API_PVR_PlayStop error 0x%x", ret);
			return APP_FAIL;
		}

		ret = FYF_API_PVR_PlayFreeChn(g_cur_player_playChn);
		if (ret != FYF_OK)
		{
			APP_PRINT_ERROR("FYF_API_PVR_PlayFreeChn error 0x%x", ret);
			return APP_FAIL;
		}

		g_cur_player_status = APP_PLAYER_STATUS_STOP_e;
	}
	
	ret = FYF_API_PVR_GetUsrData(g_film_group.FilmInfo[index].FileName, 
		&user_data, sizeof(UserData_S), &data_len);
	if (ret == FYF_OK)
	{
		APP_Printf("data_len = %d, audio_pid = %d, video_pid = %d\n", data_len, user_data.ad_pid, user_data.vd_pid);
		audio_pid = user_data.ad_pid;
		video_pid = user_data.vd_pid;
	}
	else
	{
		APP_PRINT_ERROR("HI_UNF_PVR_GetUsrDataInfoByFileName err 0x%x\n", ret);
		return APP_FAIL;
	}

	//ret = HI_UNF_DMX_SetAttr(0, &stDmxAttr);

    /*申请新的播放通道*/
	ret = FYF_API_PVR_PlayNewChn(&g_cur_player_playChn, g_film_group.FilmInfo[index].FileName);
    if (FYF_OK == ret)
    {
        APP_Printf("new play chn%d ok\n", g_cur_player_playChn);

        /*开始播放通道*/
        ret = FYF_API_PVR_PlayStart(g_cur_player_playChn, audio_pid, video_pid);//HI_UNF_PVR_PlayStartChn(g_cur_player_playChn);
        if (FYF_OK != ret)
        {
            APP_PRINT_ERROR("FYF_API_PVR_PlayStart Error,s32Ret=0x%x\n", ret);
        }
    }
    else
    {
        APP_PRINT_ERROR("FYF_API_PVR_PlayNewChn Error,s32Ret=0x%x\n", ret);
        return ret;
    }
	g_player_curflag = 1;
	g_cur_player_speedIndex = APP_PLAYER_SPEED_NORMAL;
	g_cur_player_status = APP_PLAYER_STATUS_PLAY_e;
	APP_Player_API_SetTrack(1);
//	APP_Player_API_SetVolume(30);
	return ret;

}
BU32 MID_Player_IPI_PVRStop(BU32 index)
{
	BS32 ret;
	if (g_cur_player_status == APP_PLAYER_STATUS_STOP_e)
	{
		return APP_SUCCESS;
	}

	/*停止播放通道*/
    ret = FYF_API_PVR_PlayStop(g_cur_player_playChn);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_PVR_PlayStop error 0x%x", ret);
		return APP_FAIL;
	}
	
    /*释放播放通道*/
    ret = FYF_API_PVR_PlayFreeChn(g_cur_player_playChn);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_PVR_PlayFreeChn error 0x%x", ret);
		return APP_FAIL;
	}
	g_cur_player_status = APP_PLAYER_STATUS_STOP_e;
	return APP_SUCCESS;
}

BU32 MID_Player_IPI_PVRResume()
{
    BS32 ret;
	BU32 playChn;
	if (g_cur_player_status == APP_PLAYER_STATUS_PLAY_e)
		return APP_SUCCESS;
	if (g_cur_player_status == APP_PLAYER_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("current status is stop, can't resume!\n");
		return APP_FAIL;
	}
	
	if (FYF_OK != FYF_API_PVR_PlayResume(g_cur_player_playChn))
	{
		APP_PRINT_ERROR("FYF_API_PVR_PlayResume error, ret = 0x%x\n", ret);
		return APP_FAIL;
	}

	if (g_cur_player_speedIndex != APP_PLAYER_SPEED_NORMAL)
	{
		g_cur_player_speedIndex = APP_PLAYER_SPEED_NORMAL;
		APP_Printf("g_cur_player_speed = %d\n", g_player_speed[g_cur_player_speedIndex]);
	    /*PVR开始Trick模式播放*/

		ret = FYF_API_PVR_PlayTPlay(g_cur_player_playChn, g_player_speed[g_cur_player_speedIndex]);
		if (FYF_OK != ret)
		{
		    APP_PRINT_ERROR("FYF_API_PVR_PlayTPlay Error,s32Ret=0x%x\n", ret);
		    return APP_FAIL;
		}
	}

	g_cur_player_status = APP_PLAYER_STATUS_PLAY_e;
	return APP_SUCCESS;
	
}

BU32 MID_Player_IPI_PVRPause()
{
	BU32 playChn;
	BS32 ret;
	
	if (g_cur_player_status == APP_PLAYER_STATUS_PAUSE_e)
		return APP_SUCCESS;
	if (g_cur_player_status == APP_PLAYER_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("current status is stop, can't pause!\n");
		return APP_FAIL;
	}

	ret = FYF_API_PVR_PlayPause(g_cur_player_playChn);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_PVR_PlayPause error 0x%x", ret);
	}
	
	
	g_cur_player_status = APP_PLAYER_STATUS_PAUSE_e;
	return APP_SUCCESS;
}

BU32 MID_Player_IPI_MP3Play(BU32 index)
{
	BS32 ret;
	/*if (g_cur_player_status == APP_PLAYER_STATUS_PLAY_e && index == g_cur_mp3_no)
		return APP_SUCCESS;
	
	if (g_cur_player_status == APP_PLAYER_STATUS_PAUSE_e)
	{
		APP_Printf("current status is pause, resume it first!\n");
		return APP_FAIL;
	}
	*/
	
	if (g_cur_player_status == APP_PLAYER_STATUS_PLAY_e || g_cur_player_status == APP_PLAYER_STATUS_PAUSE_e)
	{
		ret = FYF_API_Player_Stop();
		ret |= FYF_API_Player_Close();
		if (ret != FYF_OK)
		{
			APP_PRINT_ERROR("FYF_API_Player_Close error 0x%x", ret);
		}
//		HI_Player_Stop();
//		HI_Player_Close();
//		APP_SYS_IPI_SetTVMode(app_system_data.tvMode);
		
	}
	
	APP_Printf("index = %d, g_cur_mp3_no = %d\n", index, g_cur_mp3_no);

	if (index >= g_mp3_group.MP3FileNum)
	{
		return APP_FAIL;
	}
		
	if (FYF_OK != FYF_API_Player_Open(g_mp3_group.MP3Info[index].filename))
	{
		APP_PRINT_ERROR("HI_Player_Open failed Ret=%x\n",ret);
		return APP_FAIL;
	}

	/*启动播放器*/
	if (FYF_OK != FYF_API_Player_Start())
	{
		APP_PRINT_ERROR("FYF_API_Player_Start failed Ret=0x%x\n",ret);
		FYF_API_Player_Close();
		g_cur_player_status = APP_PLAYER_STATUS_STOP_e;
		return APP_FAIL;
	}
	g_player_curflag = 1;
	g_cur_player_status = APP_PLAYER_STATUS_PLAY_e;
	return APP_SUCCESS;
	
}

BU32 MID_Player_IPI_MP3Stop()
{
	BS32 ret;
	if (g_cur_player_status == APP_PLAYER_STATUS_STOP_e)
	{
		return APP_SUCCESS;
	}
	
	ret = FYF_API_Player_Stop();
	ret |= FYF_API_Player_Close();
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_Player_Close error 0x%x", ret);
		return APP_FAIL;
	}
//	APP_SYS_IPI_SetTVMode(app_system_data.tvMode);	
	g_cur_player_status = APP_PLAYER_STATUS_STOP_e;
	return APP_SUCCESS;
	
}

BU32 MID_Player_IPI_MP3Pause()
{
	BS32 ret;
	if (g_cur_player_status == APP_PLAYER_STATUS_PAUSE_e)
		return APP_SUCCESS;
	if (g_cur_player_status == APP_PLAYER_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("current status is stop, can't pause!\n");
		return APP_FAIL;
	}
	ret = FYF_API_Player_Pause();
	if (FYF_OK != ret)
	{
		FYF_API_Player_Close();
//		APP_SYS_IPI_SetTVMode(app_system_data.tvMode);
        g_cur_player_status = APP_PLAYER_STATUS_STOP_e; 
        APP_PRINT_ERROR("s32Ret = 0x%x,FYF_API_Player_Pause:\n", ret);
        return APP_FAIL;  
	}
	g_cur_player_status = APP_PLAYER_STATUS_PAUSE_e; 
	return APP_SUCCESS;
}

BU32 MID_Player_IPI_MP3Resume()
{
	BS32 ret;
	if (g_cur_player_status == APP_PLAYER_STATUS_PLAY_e)
		return APP_SUCCESS;
	if (g_cur_player_status == APP_PLAYER_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("current status is stop, can't resume!\n");
		return APP_FAIL;
	}
	ret = FYF_API_Player_Resume();
	if (FYF_OK != ret)
	{
		FYF_API_Player_Close();
//		APP_SYS_IPI_SetTVMode(app_system_data.tvMode);
        g_cur_player_status = APP_PLAYER_STATUS_STOP_e; 
        APP_PRINT_ERROR("s32Ret = %d,HI_Player_Resume: line = \n", ret);
        return APP_FAIL;  
	}
	g_cur_player_status = APP_PLAYER_STATUS_PLAY_e; 
	return APP_SUCCESS;
}


BU32 MID_Player_IPI_MP3Speed()
{
	BS32 ret;
	BU32 totaltime, curtime;
	ret = FYF_API_Player_GetTotalTime(&totaltime);
	if (FYF_OK != ret)
	{
		APP_PRINT_ERROR("s32Ret =0x%x,FYF_API_Player_GetTotalTime:\n", ret);
		return APP_FAIL;
	}
	ret = FYF_API_Player_GetCurrentTime(&curtime);
	if (FYF_OK != ret)
	{
		APP_PRINT_ERROR("s32Ret =0x%x,HI_Player_GetDuration\n", ret);
		return APP_FAIL;
	}

	if (curtime + 5000 < totaltime)
	{
		FYF_API_Player_Seek(curtime + 5000);
	}
	else
	{
		FYF_API_Player_Seek(totaltime);
	}
	return APP_SUCCESS;
}

BU32 MID_Player_IPI_MP3Rewind()
{
	BS32 ret;
	BU32 totaltime, curtime;
	if (FYF_OK != FYF_API_Player_GetTotalTime(&totaltime))
	{
		APP_PRINT_ERROR("s32Ret =,HI_Player_GetDuration: line = %d\n", __LINE__);
		return APP_FAIL;
	}

	if (FYF_OK != FYF_API_Player_GetCurrentTime(&curtime))
	{
		APP_PRINT_ERROR("s32Ret =,HI_Player_GetDuration: line = %d\n", __LINE__);
		return APP_FAIL;
	}

	if (curtime  > 5000)
	{
		FYF_API_Player_Seek(curtime - 5000);
	}
	else
	{
		FYF_API_Player_Seek(0);
	}
	return APP_SUCCESS;
}
BU32 MID_Player_IPI_ConvertTime(BU32 TotalTime, Player_Time_S *PlayerTime)
{
//	APP_Printf("totaltime = %d\n", TotalTime);
	BU32 hour;
	BU32 min;
	BU32 sec;
	if (TotalTime == 0)
	{
		PlayerTime->hour = 0;
		PlayerTime->min = 0;
		PlayerTime->sec = 0;
		return APP_SUCCESS;
	}
	TotalTime /= 1000;
	hour = TotalTime / 3600;
	min = (TotalTime - hour * 3600 )/60;
	sec = (TotalTime - hour * 3600 - min * 60);
	PlayerTime->hour = hour;
	PlayerTime->min = min;
	PlayerTime->sec = sec;
	return APP_SUCCESS;
}

BS32 MID_Player_API_InitMp3file()
{
	g_mp3_group.MP3FileNum = 0;
	return APP_OK;
}


BS32 MID_Player_API_AddMp3File(Player_Mp3File_S *mp3file)
{
	
	if (mp3file == NULL)
	{
		return APP_ERR;
	}
	memcpy(&(g_mp3_group.MP3Info[g_mp3_group.MP3FileNum++]), mp3file, sizeof(Player_Mp3File_S));
}


BS32 MID_Player_API_InitFilmfile()
{
	g_film_group.FilmFileNum = 0;
	return APP_OK;
}


BS32 MID_Player_API_AddFilmFile(APP_File_Info_S *filmfile)
{
	Player_Film_Info_S film_info;
	FYF_PVR_FILE_ATTR_S fileAttr;
	BU32 play_chn;
	BS32 ret;
	UserData_S user_data;
	BU32 data_len;
	
	if (filmfile == NULL)
	{
		return APP_ERR;
	}

	ret = FYF_API_PVR_PlayNewChn(&play_chn, filmfile->FileName);

	if (ret == FYF_OK)
	{
		APP_Printf("new play chn%d ok\n", play_chn);
		ret = FYF_API_PVR_GetPVRFileAttr(play_chn, &fileAttr);
		if (ret == FYF_OK)
		{
			film_info.FileLen = filmfile->FileSize;
			film_info.TimeLen = fileAttr.u32PlayTimeInMs;
			film_info.FrameNum = fileAttr.u32FrameNum;
			strcpy(film_info.FileName, filmfile->FileName);
            APP_Printf("file attr: frame/PES:%u, time:%ums %d %d , size:%lluByte.\n",
                   fileAttr.u32FrameNum, fileAttr.u32PlayTimeInMs, fileAttr.u32StartTimeInMs, fileAttr.u32EndTimeInMs, fileAttr.u64ValidSizeInByte);
			if (film_info.TimeLen == 0)
			{
				ret = FYF_API_PVR_GetUsrData(filmfile->FileName, &user_data, sizeof(UserData_S), &data_len);
				if (ret == FYF_OK)
				{
					film_info.TimeLen = user_data.total_time;
				}
			}
			ret = FYF_API_PVR_PlayFreeChn(play_chn);
			if (film_info.FrameNum == 0)
			{
				APP_PRINT_ERROR("film_info.FrameNum ==0 errot!\n");
				return APP_ERR;
			}
			
		}
		 else
        {
            APP_PRINT_ERROR("FYF_API_PVR_GetPVRFileAttr Error,s32Ret=0x%x\n", ret);
			return APP_ERR;
        }
		
    }
	else
	{
		APP_PRINT_ERROR("MID_Player_API_AddFilmFile error 0x%x", ret);
		return APP_ERR;
	}

	memcpy(&(g_film_group.FilmInfo[g_film_group.FilmFileNum++]), &film_info, sizeof(Player_Film_Info_S));
	return APP_OK;
}
	



void APP_Player_API_SetMuteState(BU08 bMute)
{
	g_mute_status = bMute;
	FYF_API_adec_ioctl(NULL, FYF_ADEC_SET_MUTE, &g_mute_status);
}

BU08 APP_Player_API_GetMuteState(void)
{
	return g_mute_status;
}

BU32 APP_player_API_AttachAV()
{
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_AttachAV;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	}
	
	return APP_SUCCESS;
}

BU32 APP_player_API_DetachAV()
{
	FYF_QUEUE_MESSAGE msgSend;
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_DetachAV;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

/*
开始播放
*/
BU32 APP_Player_API_Play(BU32 player_no)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_NULL_e)
	{
		APP_Printf("current player type is invaild!\n");
		return APP_FAIL;
	}
	
	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayMp3;
		msgSend.q2ndWordOfMsg= player_no;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		g_player_curflag = 0;
		g_cur_mp3_no = player_no;
		//return MID_Player_IPI_MP3Play(player_no);
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayFilm;
		msgSend.q2ndWordOfMsg= player_no;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		g_player_curflag = 0;
		g_cur_film_no = player_no;
	}

	return APP_SUCCESS;
		
}

/*
停止播放
*/
BU32 APP_Player_API_Stop(void)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_NULL_e)
	{
		APP_PRINT_ERROR("current player type is invaild!\n");
		return APP_FAIL;
	}

	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StopMp3;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		g_player_curflag = 0;
		//return MID_Player_IPI_MP3Stop();
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StopFilm;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		g_player_curflag = 0;
	}
	return APP_SUCCESS;

}

/*
暂停播放
*/
BU32 APP_Player_API_Pause(void)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_NULL_e)
	{
		APP_PRINT_ERROR("current player type is invaild!\n");
		return APP_FAIL;
	}

	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PauseMp3;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		//return MID_Player_IPI_MP3Pause();
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PauseFilm;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	}
	return APP_SUCCESS;
	
}

/*
恢复播放
*/
BU32 APP_Player_API_Resume(void)
{   
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_NULL_e)
	{
		APP_PRINT_ERROR("current player type is invaild!\n");
		return APP_FAIL;
	}

	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ResumeMp3;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		//return MID_Player_IPI_MP3Resume();
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ResumeFilm;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	}
	return APP_SUCCESS;
}

/*
播放当前播放组类型，当前播放类型的下个文件
*/
BU32 APP_Player_API_PlayNext(void)
{
	BU32 play_num;
	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		play_num = g_cur_mp3_no;
		if (++play_num == g_mp3_group.MP3FileNum)
			play_num = 0;
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		play_num = g_cur_film_no;
		if (++play_num == g_film_group.FilmFileNum)
			play_num = 0;
	}
	return APP_Player_API_Play(play_num);
}

/*
播放当前播放组类型，当前播放类型的上个文件
*/
BU32 APP_Player_API_PlayPre(void)
{
	BU32 play_num;
	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		play_num = g_cur_mp3_no;
		if (play_num == 0)
		{
			play_num = g_mp3_group.MP3FileNum - 1;
		}
		else
		{
			play_num --;
		}
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		play_num = g_cur_film_no;
		if (play_num == 0)
		{
			play_num = g_film_group.FilmFileNum - 1;
		}
		else
		{
			play_num --;
		}
	}
	return APP_Player_API_Play(play_num);
}

/*
快进播放文件
*/
BU32 APP_Player_API_Speed(void)
{
	BS32 ret;
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_SpeedMP3;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{


		if (g_cur_player_speedIndex >= APP_PLAYER_SPEED_32X_FAST_FORWORD)
		{
			g_cur_player_speedIndex = APP_PLAYER_SPEED_32X_FAST_FORWORD;
			return APP_SUCCESS;
		}
		g_cur_player_speedIndex += 1;
		APP_Printf("g_cur_player_speed = %d\n", g_player_speed[g_cur_player_speedIndex]);
	    /*PVR开始Trick模式播放*/
	    ret = FYF_API_PVR_PlayTPlay(g_cur_player_playChn, g_player_speed[g_cur_player_speedIndex]);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("FYF_API_PVR_PlayTPlay Error,s32Ret=0x%x\n", ret);
	        return APP_FAIL;
	    }
	   
	}
	return APP_SUCCESS;
}

/*218.58.72.60:9300
快退播放文件
*/
BU32 APP_Player_API_Rewind(void)
{
	BS32 ret;
	FYF_QUEUE_MESSAGE msgSend;
	if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_RewindMP3;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	}
	else if (g_cur_player_type == APP_PLAYER_TYPE_PVR_e)
	{
		
		if (g_cur_player_speedIndex <= APP_PLAYER_SPEED_32X_FAST_BACKWORD)
		{
			g_cur_player_speedIndex = APP_PLAYER_SPEED_32X_FAST_BACKWORD;
			return APP_SUCCESS;
		}
		g_cur_player_speedIndex -= 1;
		APP_Printf("g_cur_player_speed = %d\n", g_player_speed[g_cur_player_speedIndex]);
	    /*PVR开始Trick模式播放*/
		
		ret = FYF_API_PVR_PlayTPlay(g_cur_player_playChn, g_player_speed[g_cur_player_speedIndex]);
		if (FYF_OK != ret)
		{
		    APP_PRINT_ERROR("FYF_API_PVR_PlayTPlay Error,s32Ret=0x%x\n", ret);
		    return APP_FAIL;
		}
	 
	}
	return APP_SUCCESS;
}


/*
获取播放文件相关属性
*/
BU32 APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_E file_attr_type, void *str)
{
	BU08 *filename;
	Player_Time_S *time;
	BU32 totaltime;
	BU08 *curstatus;
	BU32 ret;
	switch(file_attr_type)
	{
	case APP_PLAYER_FILEATTR_TYPE_FILENAME_e:
		filename = (BU08 *)str;
		if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
			APP_PLAYER_API_GetRelativePath(g_mp3_group.MP3Info[g_cur_mp3_no].filename, filename);
		else
			APP_PLAYER_API_GetRelativePath(g_film_group.FilmInfo[g_cur_film_no].FileName, filename);
		break;
	case APP_PLAYER_FILEATTR_TYPE_TIMES_e:
		time = (Player_Time_S *)str;
		if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
		{
			ret = FYF_API_Player_GetTotalTime(&totaltime);
			if (ret != FYF_OK)
			{
					
				return APP_FAIL;
			}
		}
		else
		{
			totaltime = g_film_group.FilmInfo[g_cur_film_no].TimeLen;
		}
		MID_Player_IPI_ConvertTime(totaltime, time);
		break;
	case APP_PLAYER_FILEATTR_TYPE_CURTIME_e:
		time = (Player_Time_S *)str;
		if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
		{
			ret = FYF_API_Player_GetCurrentTime(&totaltime);
			if (ret != FYF_OK)
				return APP_FAIL;
		}
		else
		{
			ret = FYF_API_PVR_PlayGetCurPlayTime(g_cur_player_playChn, &totaltime);
			if (ret != FYF_OK)
			{
				APP_PRINT_ERROR(" FYF_API_PVR_PlayGetCurPlayTime error 0x%x\n", ret);
				return APP_FAIL;
			}
#if 0
			ret = HI_UNF_PVR_PlayGetStatus(g_cur_player_playChn, &play_status);
			if (ret != HI_SUCCESS)
			{
				printf(" HI_UNF_PVR_PlayGetStatus error 0x%x\n", ret);
				return APP_FAIL;
			}
			totaltime = play_status.u32CurPlayTimeInMs;
			if (totaltime == 0)
			{
				totaltime = g_film_group.FilmInfo[g_cur_film_no].TimeLen;
				totaltime = totaltime * play_status.u32CurPlayFrame / g_film_group.FilmInfo[g_cur_film_no].FrameNum;
			}
#endif
			APP_Printf("totaltime = %d\n", totaltime);

		}
		MID_Player_IPI_ConvertTime(totaltime, time);
		break;
	case APP_PLAYER_FILEATTR_TYPE_ARTIST_e:
		filename = (BU08*)str;
		if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
			memcpy(filename, g_mp3_group.MP3Info[g_cur_mp3_no].mp3singer, sizeof(g_mp3_group.MP3Info[g_cur_mp3_no].mp3singer));
		break;
	case APP_PLAYER_FILEATTR_TYPE_SONGNAME_e:
		filename = (BU08*)str;
		if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
			memcpy(filename, g_mp3_group.MP3Info[g_cur_mp3_no].mp3name, sizeof(g_mp3_group.MP3Info[g_cur_mp3_no].mp3name));
		break;
	case APP_PLAYER_FILEATTR_TYPE_SPECIAL_e:
		filename = (BU08*)str;
		if (g_cur_player_type == APP_PLAYER_TYPE_MP3_e)
			memcpy(filename, g_mp3_group.MP3Info[g_cur_mp3_no].mp3special, sizeof(g_mp3_group.MP3Info[g_cur_mp3_no].mp3special));
		break;
	case APP_PLAYER_FILEATTR_TYPE_CURSTATUS_e:
		curstatus = (BU08*)str;
		*curstatus = g_player_curflag;
		break;
	default:
		break;
	}

	return APP_SUCCESS;
}

BU32 APP_PLAYER_API_GetRelativePath(BU08 *abspath, BU08* relpath)
{
	BU32 i, len, cplen;
	if (abspath == NULL || relpath == NULL)
	{
		return APP_FAIL;
	}
	
	len = strlen(abspath);
	for (i = len - 1; i >= 0; i--)
	{
		if (abspath[i] == '/' && i != len-1)break;
	}

	memcpy(relpath, &(abspath[i + 1]), len - i);
	return APP_SUCCESS;
	
}

BU32 APP_Player_API_GetPlayerFile(APP_PLAYER_TYPE_E player_type, APP_PLAYER_GROUP_TYPE_E group_type, BU32 index, void *str)
{
	Player_Mp3File_S *fileinfo ;
	Player_Film_Info_S *filminfo;
/*	if (index >= g_mp3_group.MP3FileNum)
	{
		return APP_FAIL;
	}
	*/
	switch(player_type)
	{
	case APP_PLAYER_TYPE_MP3_e:
		fileinfo = (Player_Mp3File_S*)str;
		memcpy(fileinfo, &(g_mp3_group.MP3Info[index]), sizeof(Player_Mp3File_S));
		break;
	case APP_PLAYER_TYPE_PVR_e:
		filminfo = (Player_Film_Info_S*)str;
		memcpy(filminfo, &(g_film_group.FilmInfo[index]), sizeof(Player_Film_Info_S));
		break;
	default:
		break;
	}
	return APP_SUCCESS;
}


APP_PLAYER_TYPE_E APP_Player_API_GetCurPlayerType(void)
{
	return g_cur_player_type;
}

BS32 APP_Player_API_SetCurPlayerType(APP_PLAYER_TYPE_E PlayType)
{
	if (PlayType >= APP_PLAYER_TYPE_BUTT_e)
	{
		APP_PRINT_ERROR("playertype is invaild!\n");
		return APP_ERR;
	}
	g_cur_player_type = PlayType;
	
}

BU32 APP_Player_API_GetCurTypeCount()
{
	BU32 totalnum;
	if (g_cur_player_type == APP_PLAYER_TYPE_NULL_e || g_cur_player_type >= APP_PLAYER_TYPE_BUTT_e)
	{
		return 0;
	}
	
	switch(g_cur_player_type)
	{
	case APP_PLAYER_TYPE_MP3_e:
		totalnum = g_mp3_group.MP3FileNum;
		break;
	case APP_PLAYER_TYPE_PVR_e:
		totalnum = g_film_group.FilmFileNum;
		break;
	case APP_PLAYER_TYPE_AVI_e:
		break;
	}
	return totalnum;
}

APP_PLAYER_GROUP_TYPE_E APP_Player_API_GetCurPlayerGroupType(void)
{
	return APP_PLAYER_GROUP_TYPE_ALL_e;
}

BS32 APP_Player_API_AddVolume()
{
	if (g_volume < 30)g_volume++;
	else return APP_OK;
	APP_Audio_API_SetVolume(g_volume);
	return APP_OK;
}

BS32 APP_Player_API_ReduceVolume()
{
	if (g_volume > 0)g_volume--;
	else return APP_OK;
	APP_Audio_API_SetVolume(g_volume);
	return APP_OK;
}

BU32 APP_Player_API_GetVolume()
{
	if (g_volume == 0xffff)
	{
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &g_volume);
		APP_Audio_API_SetVolume(g_volume);
	}
	return g_volume;
}
BU32 APP_Player_API_GetPvrCurSpeedIndex()
{
     return g_cur_player_speedIndex;
}
void APP_Player_API_SetPvrCurSpeedIndex(BU32 speedIndex)
{
    if(speedIndex < APP_PLAYER_SPEED_32X_FAST_BACKWORD)
    {
        g_cur_player_speedIndex =  APP_PLAYER_SPEED_32X_FAST_BACKWORD;
    }
	else if(speedIndex > APP_PLAYER_SPEED_32X_FAST_FORWORD)
	{
	    g_cur_player_speedIndex =  APP_PLAYER_SPEED_32X_FAST_FORWORD;
	}
	else
	{
	    g_cur_player_speedIndex = speedIndex;
	}
}

BU32 APP_Player_API_GetPlayIndex(void)
{
    return g_cur_playIndex;
}
BU32 APP_Player_API_SetPlayIndex(BU32 index)
{
	g_cur_playIndex = index;
}
