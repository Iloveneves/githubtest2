

#include "app_common.h"
#include "app_ipi_common.h"

#define RECDIR1 "/mnt/C/Rec_Pvr"
#define RECDIR2 "/mnt/D/Rec_Pvr"
#define REC_MAX_NUM 100

static BU32 rec_demux[3] = {0};
static BU08 g_file_name[200];
UserData_S g_user_data;
static BU32 g_cur_recChn;
static BU32 g_cur_playChn = 0xffff;
static BU32 g_cur_pvr_speedIndex = APP_PLAYER_SPEED_NORMAL;
static BU32 g_cur_rePlayStatus = APP_PVR_STATUS_STOP_e;
static BU32 g_starttime, g_endtime;
extern BS32 g_player_speed[];


//第一路demux进行录制，第0路demux进行时移播放，第二路demux暂时不用

/*开始录制*/
BS32 MID_PVR_IPI_StartRec(BU32 demuxId, const BU08 *pszFileName, BU32 source, BU32 scrambled, BU16 vd_pid, BU16 ad_pid)
{
    BU32 pid;
    BS32 s32Ret;
	BS32 i = 0;

    if ((demuxId > 2) || (demuxId < 1))
    {
        demuxId = 1;
    }
	
	for(i=0;i<3;i++)
	{
		APP_Printf("rec_chan[%d] = %d\n",i, rec_demux[i]);
	}
	
#if 1
	 /*PVR初始化demux*/
    s32Ret = FYF_API_PVR_demux_init(demuxId, source, ad_pid, FYF_DEMUX_AUDIO_CHANNEL);
    if (FYF_OK != s32Ret)
    {
        APP_PRINT_ERROR("FYF_API_PVR_demux_init audio Error,s32Ret=0x%x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = FYF_API_PVR_demux_init(demuxId, source, vd_pid, FYF_DEMUX_VIDEO_CHANNEL);
    if (FYF_OK != s32Ret)
    {
        APP_PRINT_ERROR("FYF_API_PVR_demux_init video Error,s32Ret=0x%x\n", s32Ret);
        return s32Ret;
    }

	g_user_data.ad_pid = ad_pid;
	g_user_data.vd_pid = vd_pid;

	memcpy(g_file_name, pszFileName, strlen(pszFileName) + 1);
	s32Ret = FYF_API_PVR_RecNewChn(&g_cur_recChn, pszFileName, ad_pid, vd_pid, scrambled);

    if (FYF_OK == s32Ret)
    {
        APP_PRINT_INFO("Get chn OK:%d\n", g_cur_recChn);
    }
    else
    {
        APP_PRINT_ERROR("FYF_API_PVR_RecNewChn Error,s32Ret=0x%x\n", s32Ret);
        return s32Ret;
    }

    /*开始录制通道*/
    s32Ret = FYF_API_PVR_RecStart(g_cur_recChn);
    if (FYF_OK == s32Ret)
    {
		g_user_data.total_time = 0;
		s32Ret = FYF_API_PVR_SetUsrData(g_file_name, &g_user_data,sizeof(UserData_S));
	    APP_PRINT_INFO("start chn OK:%d\n", g_cur_recChn);
    }
    else
    {
        APP_PRINT_ERROR("HI_UNF_PVR_RecStartChn Error,s32Ret=0x%x\n", s32Ret);

        /*释放录制通道*/
        s32Ret = FYF_API_PVR_RecFreeChn(g_cur_recChn);
        if (FYF_OK != s32Ret)
        {
            APP_PRINT_ERROR("HI_UNF_PVR_RecFreeChn Error,s32Ret=0x%x\n", s32Ret);
            return s32Ret;
        }

        return s32Ret;
    }

#else
	//先判断该pid是否已分配通道，如果是就释放已分配的channel
/*    s32Ret = HI_UNF_DMX_GetChannelHandle(demuxId, ad_pid, &hChannelHandle);
    if (s32Ret == HI_SUCCESS)
    {
        HI_UNF_DMX_CloseChannel(hChannelHandle);
        HI_UNF_DMX_DeleteChannel(hChannelHandle);
        printf("demuxId, = %dhChannelHandle = %x\n", demuxId, hChannelHandle);
    }

    s32Ret = HI_UNF_DMX_GetChannelHandle(demuxId, vd_pid, &hChannelHandle);
    if (s32Ret == HI_SUCCESS)
    {
        HI_UNF_DMX_CloseChannel(hChannelHandle);
        HI_UNF_DMX_DeleteChannel(hChannelHandle);
        printf("demuxId, = %dhChannelHandle = %x\n", demuxId, hChannelHandle);
    }
    */
    /*PVR初始化demux*/
    s32Ret = FYF_API_demux_pvr_init(demuxId, source, ad_pid, HI_UNF_DMX_CHAN_TYPE_AUD);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):PVR_demux_Init Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    s32Ret = FYF_API_demux_pvr_init(demuxId, source, vd_pid, HI_UNF_DMX_CHAN_TYPE_VID);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):PVR_demux_Init Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

	g_user_data.ad_pid = ad_pid;
	g_user_data.vd_pid = vd_pid;

    memcpy(attr.szFileName, pszFileName, strlen(pszFileName) + 1);
	memcpy(g_file_name, pszFileName, strlen(pszFileName) + 1);
    attr.u32FileNameLen = strlen(pszFileName);
    attr.u32DemuxID    = demuxId;
    attr.u32IndexPid   = vd_pid;
    attr.enIndexType   = 1;
    attr.u32ScdBufSize = PVR_STUB_SC_BUF_SZIE;
    attr.u32DavBufSize = PVR_STUB_TSDATA_SIZE;
    attr.enStreamType  = PVR_STREAM_TYPE_TS;
    attr.u32UsrDataInfoSize = sizeof(UserData_S);
    attr.u64MaxFileSize= (HI_U64)0;
    
    if (0 == scrambled)
    {
        attr.bIsClearStream = HI_TRUE;
    }
    else
    {
        attr.bIsClearStream = HI_FALSE;
    }
#ifdef PVR_CIPHER
    	attr.stEncryptCfg.bDoCipher = HI_TRUE;
        attr.stEncryptCfg.enType = 0;
        attr.stEncryptCfg.u32KeyLen = 16;          /*strlen(PVR_CIPHER_KEY)*/

        memset(attr.stEncryptCfg.au8Key, 0, 16);/*128 = sizeof(PVR_CIPHER_KEY)*/
        memcpy(attr.stEncryptCfg.au8Key, PVR_CIPHER_KEY, strlen(PVR_CIPHER_KEY));
#else
     attr.stEncryptCfg.bDoCipher = HI_FALSE; 
#endif

    /*申请新的录制通道*/
    s32Ret = HI_UNF_PVR_RecNewChn(&g_cur_recChn, &attr);
    if (HI_SUCCESS == s32Ret)
    {
        printf("Get chn OK:%d\n", g_cur_recChn);
    }
    else
    {
        printf("(%d):HI_UNF_PVR_RecNewChn Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*开始录制通道*/
    s32Ret = HI_UNF_PVR_RecStartChn(g_cur_recChn);
    if (HI_SUCCESS == s32Ret)
    {
		g_user_data.total_time = 0;
		s32Ret = HI_UNF_PVR_SetUsrDataInfoByFileName(g_file_name, &g_user_data,sizeof(UserData_S));
	    printf("start chn OK:%d\n", g_cur_recChn);
    }
    else
    {
        printf("(%d):HI_UNF_PVR_RecStartChn Error,s32Ret=0x%x\n", __LINE__, s32Ret);

        /*释放录制通道*/
        s32Ret = HI_UNF_PVR_RecFreeChn(g_cur_recChn);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_PVR_RecFreeChn Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }

        return s32Ret;
    }
#endif
    rec_demux[demuxId] = APP_REC_STATUS_PLAY_e;
	for(i=0;i<3;i++)
	{
		APP_Printf("rec_chan[%d] = %d\n",i, rec_demux[i]);
	}

    return s32Ret;
}

BU32 MID_PVR_API_StartRec(BU32 chnNo, BU32 demuxId)
{
	BU08 rec_file_name[FILE_FULLNAME_LEN];
	BU08 rec_dir[FILE_FULLNAME_LEN];
	BU08 chn_name[FILE_FULLNAME_LEN];
	ChnDataBase_ChnInfo_S chnInfo;	
	BU32 j, len = 0;
	BU32 UsbNum;
	if (rec_demux[demuxId] == APP_REC_STATUS_PLAY_e)
	{
		return APP_SUCCESS;
	}
	UsbNum = APP_USB_API_DetectedWhichUSB();
	if (UsbNum > 0 )
	{
		strcpy(rec_dir,RECDIR1);
	}
	else
	{
		APP_PRINT_ERROR("Not Insert Usb\n");
		return APP_FAIL;
	}
#if 0
	if(UsbNum == 1)
	{
		strcpy(rec_dir,RECDIR1);
	}
	else if(UsbNum == 2 || UsbNum == 3)
	{
		strcpy(rec_dir,RECDIR2);
	}
#endif

	if (FYF_API_dir_judge(rec_dir) == FYF_ERR)
	{
		if (FYF_API_dir_create(rec_dir) == FYF_ERR)
		{
			APP_PRINT_ERROR("do not create Rec_Pvr folder\n");
			return APP_FAIL;
		}
	}
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),chnNo, &chnInfo))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo error\n");
		return APP_FAIL;
	}
	APP_Printf("APP_PVR_API_StartRec chnNo = %d\n", chnNo);
	//APP_Printf("chnInfo.m_ChnName =%s\n", chnInfo.m_ChnName);

	//add by cwd 
	//防止由于节目名中有怪异字符，导致创建文件错误
	len = strlen(chnInfo.m_ChnName);
	for (j = 0; j < len; j++)
	{
		APP_Printf("%d\n",chnInfo.m_ChnName[j]);
		if (chnInfo.m_ChnName[j] != 19)
		{
			sprintf(chn_name, "%s", &(chnInfo.m_ChnName[j]));
			break;
		}
	}
	for (j = 0; j < REC_MAX_NUM; j++)
	{
		sprintf(rec_file_name,"%s/%s_%02d.pvr.idx",rec_dir,chn_name,j);
		APP_Printf("%s\n", rec_file_name);
		if (FYF_API_file_isexist(rec_file_name) == 0)
		{
			sprintf(rec_file_name,"%s/%s_%02d.pvr",rec_dir,chn_name,j);
			break;
		}
	}
	if (j >= REC_MAX_NUM)
	{
		APP_PRINT_ERROR("rec_file_name is error\n");
		return APP_FAIL;
	}
	if (APP_ERR == MID_PVR_IPI_StartRec(demuxId, rec_file_name, 0, 0, chnInfo.m_VideoPid, chnInfo.m_AudioPid))
	{
		return APP_FAIL;
	}
	return APP_SUCCESS;
}

BU32 MID_PVR_API_StopRec(BU32 demuxId)
{

    BS32 s32Ret ;
	BU32 RecTime;

	if (rec_demux[demuxId] == APP_REC_STATUS_STOP_e)
	{
		APP_PRINT_DEBUG("MID_PVR_API_StopRec is stop!!!\n");
		return APP_SUCCESS;
	}


	s32Ret = FYF_API_PVR_GetRecTime(g_cur_recChn, &RecTime);// 
	
	g_user_data.total_time = RecTime;//status.u32EndTimeInMs;
	s32Ret = FYF_API_PVR_SetUsrData(g_file_name, &g_user_data,sizeof(UserData_S));
	if (s32Ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_PVR_SetUsrData error 0x%x", s32Ret);
	}
    /*停止录制通道*/
    s32Ret = FYF_API_PVR_RecStop(g_cur_recChn);//HI_UNF_PVR_RecStopChn(g_cur_recChn);
    if (FYF_OK == s32Ret)
    {
        APP_Printf("stop chn OK:%d\n", g_cur_recChn);
    }
    else
    {
        APP_PRINT_ERROR("FYF_API_PVR_RecStop Error,s32Ret=0x%x\n", s32Ret);

        /*释放录制通道*/
        s32Ret = FYF_API_PVR_RecFreeChn(g_cur_recChn);//HI_UNF_PVR_RecFreeChn(g_cur_recChn);
        if (FYF_OK != s32Ret)
        {
            APP_PRINT_ERROR("FYF_API_PVR_RecFreeChn Error,s32Ret=0x%x\n", s32Ret);
            return s32Ret;
        }

        return s32Ret;
    }

    /*释放录制通道*/
	s32Ret = FYF_API_PVR_RecFreeChn(g_cur_recChn);
	if (FYF_OK != s32Ret)
	{
	    APP_PRINT_ERROR("FYF_API_PVR_RecFreeChn Error,s32Ret=0x%x\n", s32Ret);
	    return s32Ret;
	}
	rec_demux[demuxId] = APP_REC_STATUS_STOP_e;
	APP_Printf("demuxid = %d\n", demuxId);
	//cwd 2009-09-04 add
	FYF_API_PVR_demux_deInit(demuxId);
	APP_USB_API_AddFile(FILE_TYPE_FILM, g_file_name);

    return s32Ret;
}

BU32 MID_PVR_API_StartRePlay(BU32 demuxId)
{

	BS32 ret;

	ret = FYF_API_PVR_PlayStartTimeShift(&g_cur_playChn, demuxId, g_cur_recChn, g_user_data.ad_pid, g_user_data.vd_pid);

	if (ret != FYF_OK)
    {
        APP_PRINT_ERROR("FYF_API_PVR_PlayStartTimeShift Error,s32Ret=0x%x\n", ret);
        return ret;
    }
	//默认设置左声道
	APP_Audio_API_SetTrack(APP_CHN_TRACK_LEFT_e);
/*
	ret = HI_UNF_DMX_SetChannelPID(APP_COM_IPI_GetAudioHandle(), g_user_data.ad_pid);
	if (FYF_OK != ret)
	{
		printf("HI_UNF_DMX_SetChannelPID error 0x%x\n", ret);
	}
	
    ret = HI_UNF_DMX_OpenChannel(APP_COM_IPI_GetAudioHandle());

	if (FYF_OK != ret)
	{
		printf("HI_UNF_DMX_OpenChannel error 0x%x\n", ret);
	}
	
	ret = HI_UNF_DMX_SetChannelPID(APP_COM_IPI_GetVideoHandle(), g_user_data.vd_pid);
	if (FYF_OK != ret)
	{
		printf("HI_UNF_DMX_SetChannelPIDerror 0x%x\n", ret);
	}

  	ret = HI_UNF_DMX_OpenChannel(APP_COM_IPI_GetVideoHandle());
	if (FYF_OK != ret)
	{
		printf("HI_UNF_DMX_OpenChannel error 0x%x\n", ret);
	}
	ret = HI_UNF_DMX_SetAttr(0, &stDmxAttr);
	attr.u32DemuxID = 0;

	//modify by cwd 2009-12-04 
	//使用时移接口
	ret = HI_UNF_PVR_PlayStartTimeShift(&g_cur_playChn, 0, g_cur_recChn);

	if (ret != HI_SUCCESS)
    {
        APP_PRINT_ERROR("(%d):HI_UNF_PVR_PlayStartTimeShift Error,s32Ret=0x%x\n", __LINE__, ret);
        return ret;
    }
	//默认设置左声道
	APP_Audio_API_SetTrack(APP_CHN_TRACK_LEFT_e);
*/
#if 0
	memcpy(attr.szFileName, g_file_name, strlen(g_file_name) + 1);
    attr.u32FileNameLen = strlen(g_file_name);
    attr.enStreamType = PVR_STREAM_TYPE_TS;
	attr.bIsClearStream = HI_TRUE;
#ifdef PVR_CIPHER
    attr.stDecryptCfg.bDoCipher = HI_TRUE;
    attr.stDecryptCfg.enType = 0;
    attr.stDecryptCfg.u32KeyLen = 16;          /*strlen(PVR_CIPHER_KEY)*/

    memset(attr.stDecryptCfg.au8Key, 0, 16);/*128 = sizeof(PVR_CIPHER_KEY)*/
    memcpy(attr.stDecryptCfg.au8Key, PVR_CIPHER_KEY, strlen(PVR_CIPHER_KEY));
#else
	attr.stDecryptCfg.bDoCipher = HI_FALSE;
#endif
    /*申请新的播放通道*/
    ret = HI_UNF_PVR_PlayNewChn(&g_cur_playChn, &attr);
    if (HI_SUCCESS == ret)
    {
        PVR_FILE_ATTR_S fileAttr = {0};
        printf("new play chn%d ok\n", g_cur_playChn);

        /*获取文件信息*/
        ret = HI_UNF_PVR_PlayGetFileAttr(g_cur_playChn, &fileAttr);
        if (HI_SUCCESS == ret)
        {
            printf("file attr: frame/PES:%u, time:%ums, %ums, %ums, size:%lluByte.\n",
                   fileAttr.u32FrameNum, fileAttr.u32PlayTimeInMs, fileAttr.u32EndTimeInMs, fileAttr.u32StartTimeInMs, fileAttr.u64ValidSizeInByte);
        }
        else
        {
            printf("(%d):HI_UNF_PVR_PlayGetFileAttr Error,s32Ret=0x%x\n", __LINE__, ret);
        }


        /*获取通道0文件信息*/
        ret = HI_UNF_PVR_PlayGetFileAttr(g_cur_playChn, &fileAttr);
        if (HI_SUCCESS != ret)
        {
            printf("(%d):HI_UNF_PVR_PlayGetFileAttr Error,s32Ret=0x%x\n", __LINE__, ret);
            return ret;
        }

        printf("play size:%llu \n", fileAttr.u64ValidSizeInByte);

        /*开始播放通道*/
        ret = HI_UNF_PVR_PlayStartChn(g_cur_playChn);
        if (HI_SUCCESS == ret)
        {
            printf("start play chn%d ok\n", g_cur_playChn);
            printf("attr: clr:%d, cipher:%d\n", attr.bIsClearStream, attr.stDecryptCfg.bDoCipher);
        }
        else
        {
            printf("(%d):HI_UNF_PVR_PlayStartChn Error,s32Ret=0x%x\n", __LINE__, ret);
        }
    }
    else
    {
        printf("(%d):HI_UNF_PVR_PlayGetFileAttr Error,s32Ret=0x%x\n", __LINE__, ret);
        return ret;
    }
#endif
	g_cur_pvr_speedIndex = APP_PLAYER_SPEED_NORMAL;
	g_cur_rePlayStatus = APP_PVR_STATUS_PLAY_e;
	return ret;
}

BU32 MID_PVR_API_StopRePlay()
{
	BS32 ret;
	if (g_cur_rePlayStatus == APP_PVR_STATUS_STOP_e)
	{
		return APP_SUCCESS;
	}
	ret = FYF_API_PVR_PlayStopTimeShift(g_cur_playChn);//HI_UNF_PVR_PlayStopTimeShift(g_cur_playChn);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("HI_UNF_PVR_PlayStopTimeShift error 0x%x", ret);
		return APP_FAIL;
	}
#if 0
	/*停止播放通道*/
    ret = HI_UNF_PVR_PlayStopChn(g_cur_playChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayStopChn error 0x%x", ret);
		return APP_FAIL;
	}
	
    /*释放播放通道*/
    ret = HI_UNF_PVR_PlayFreeChn(g_cur_playChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayFreeChn error 0x%x", ret);
		return APP_FAIL;
	}
#endif
	g_cur_rePlayStatus = APP_PVR_STATUS_STOP_e;
	g_cur_playChn = 0xffff;
	return APP_SUCCESS;
}



BU32 APP_PVR_API_SetNormalSpeed()
{
	BS32 ret;

	g_cur_pvr_speedIndex = APP_PLAYER_SPEED_NORMAL;
   

    /*PVR开始Trick模式播放*/
    ret = FYF_API_PVR_PlayTPlay(g_cur_playChn, g_player_speed[g_cur_pvr_speedIndex]);
    if (FYF_OK != ret)
    {
        APP_PRINT_ERROR("FYF_API_PVR_PlayTPlay Error,s32Ret=0x%x\n", ret);
        return APP_FAIL;
    }

}
BU32 MID_PVR_API_ResumeRePlay()
{
	BU32 playChn;
	if (g_cur_rePlayStatus == APP_PVR_STATUS_PLAY_e)
	{
		return APP_SUCCESS;
	}
	if (g_cur_rePlayStatus == 0)
	{
		APP_PRINT_ERROR("current status is stop, can't resume!\n");
		return APP_FAIL;
	}
	if (FYF_OK != FYF_API_PVR_PlayResume(g_cur_playChn))
	{
		APP_PRINT_ERROR("HI_UNF_PVR_PlayResumeChn error");
		return APP_FAIL;
	}
	g_cur_rePlayStatus = APP_PVR_STATUS_PLAY_e;
	if (g_cur_pvr_speedIndex != APP_PLAYER_SPEED_NORMAL)
	{
		g_cur_pvr_speedIndex = APP_PLAYER_SPEED_NORMAL;
		APP_PVR_API_SetNormalSpeed();
	}
	return APP_SUCCESS;
	
}

BU32 MID_PVR_API_PauseRePlay()
{
	BU32 playChn;
	BS32 ret;

	if (g_cur_rePlayStatus == 0)
	{
		APP_Printf("current status is stop, can't pause!\n");
		return APP_FAIL;
	}
	ret = FYF_API_PVR_PlayPause(g_cur_playChn);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_PVR_PlayPause error 0x%x", ret);
	}
	
	g_cur_rePlayStatus = 2;
	return APP_SUCCESS;
}


/*停止录制*/
BU32 APP_PVR_API_StopRec(BU32 demuxId)
{
	FYF_QUEUE_MESSAGE msgSend;
    if (rec_demux[demuxId] == APP_REC_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("demux %d is stop\n", demuxId);
		return APP_SUCCESS;
	}
	//add by cwd 2009-12-07
	//停止录制的时候把预录的结束时间清0
	APP_API_SetPVREndTime(0, 0, 0);
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StopRec;
	msgSend.q2ndWordOfMsg= demuxId;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	
	return APP_SUCCESS;
}




BU32 APP_PVR_API_StartRec(BU32 chnNo, BU32 demuxId)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (rec_demux[demuxId] == APP_REC_STATUS_PLAY_e)
	{
		APP_PRINT_ERROR("demux %d is recording, stop it first!\n", demuxId);
		return APP_FAIL;
		
	}


	if(APP_OK == APP_USB_API_DetectedUSB())
	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StartRec;
		msgSend.q2ndWordOfMsg= chnNo;
		msgSend.q3rdWordOfMsg = demuxId;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
		return APP_SUCCESS;
	}
	else
	{
		return APP_FAIL;
	//	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWNOUSBREC, 0, 0);
	}
}

BU32 APP_PVR_API_GetRecStatus(BU32 demuxId)
{
	return rec_demux[demuxId];
	
}

BU32 APP_PVR_API_StartRePlay(BU32 demuxId)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (rec_demux[demuxId] == APP_REC_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("demux %d is not recording, can't not replay!\n", demuxId);
		return APP_FAIL;
		
	}
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StartRePlay;
	msgSend.q2ndWordOfMsg= 0;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

BU32 APP_PVR_API_StopRePlay(BU32 demuxId)
{
	FYF_QUEUE_MESSAGE msgSend;
	if (rec_demux[demuxId] == APP_REC_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("demux %d is not recording, can't not replay!\n", demuxId);
		return APP_FAIL;
		
	}
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StopRePlay;
	msgSend.q2ndWordOfMsg= demuxId;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

BU32 APP_PVR_API_ResumeRePlay()
{
	FYF_QUEUE_MESSAGE msgSend;
	if (rec_demux[1] == APP_REC_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("demux %d is not recording, can't not Resume!\n", 1);
		return APP_FAIL;
		
	}
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ResumeRePlay;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

BU32 APP_PVR_API_PauseRePlay()
{
	FYF_QUEUE_MESSAGE msgSend;
	if (rec_demux[1] == APP_REC_STATUS_STOP_e)
	{
		APP_PRINT_ERROR("demux %d is not recording, can't not Pause!\n", 1);
		return APP_FAIL;
		
	}
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PauseRePlay;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

BU32 APP_PVR_API_GetRePlayStatus()
{
	return g_cur_rePlayStatus;
}

BU32 APP_PVR_API_AttachAV()
{
	FYF_QUEUE_MESSAGE msgSend;

	{
		msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_AttachAV;
		FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	}
	
	return APP_SUCCESS;
}

BU32 APP_PVR_API_DetachAV()
{
	FYF_QUEUE_MESSAGE msgSend;
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_DetachAV;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}


/*
注意:在正常播放与时移切换时要进行demux的设置，在正常播放时滤各种表都是从第
0路demux申请的通道，而进行时移时由于第0路demux的数据不是从cable线进来的
而是从录制的文件传进去的，所以这时候滤各种表包括emm和ecm数据的表都要从第
1路demux申请通道 by cwd
*/
BU32 APP_PVR_API_SetDemux(BU32 demuxId)
{
	if (demuxId >= 2)
	{
		APP_PRINT_ERROR("APP_PVR_API_SetDemux demuxId %d is over!\n", demuxId);
	}
	APP_Printf("FYF_API_PVR_GetDemux = %d\n", FYF_API_PVR_GetDemux());
	FYF_API_PVR_SetDemux(demuxId);
	return APP_SUCCESS;
}


/******************************************************************

	描述:注册PVR出错事件函数

*******************************************************************/
static	BU32 arg = 1;
BU32 APP_PVR_API_RegisterEventCallBack(FYF_PVR_EventCallBack_S *PvrRegisterParam)
{
	BU32 ret;
    /*PVR注册PVR_EVENT_REC_DISKFULL事件*/
	if(PvrRegisterParam->CallBack_RecDiskFull != NULL)
	{
	    ret = FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_REC_DISKFULL, PvrRegisterParam->CallBack_RecDiskFull, (void*)&arg);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_RegisterEvent 1 Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	else
	{
	        APP_PRINT_DEBUG("CallBack_RecDiskFull == NULL\n");
	}

    /*PVR注册PVR_EVENT_REC_ERROR事件*/
	if(PvrRegisterParam->CallBack_RecError != NULL)
	{
	    ret = FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_REC_ERROR, PvrRegisterParam->CallBack_RecError, (void*)&arg);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_RegisterEvent 2 Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	else
	{
	        APP_PRINT_DEBUG("CallBack_RecError == NULL\n");
	}

    /*PVR注册PVR_EVENT_PLAY_EOF事件*/
	if(PvrRegisterParam->CallBack_PlayEndOfFile != NULL)
	{
	    ret = FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_PLAY_EOF, PvrRegisterParam->CallBack_PlayEndOfFile, (void*)&arg);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_RegisterEvent 3 Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	else
	{
	        APP_PRINT_DEBUG("CallBack_PlayEndOfFile == NULL\n");
	}

    /*PVR注册PVR_EVENT_PLAY_SOF事件*/
	if(PvrRegisterParam->CallBack_PlayStartOfFile != NULL)
	{
	    ret = FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_PLAY_SOF, PvrRegisterParam->CallBack_PlayStartOfFile, (void*)&arg);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_RegisterEvent 4 Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	else
	{
	        APP_PRINT_DEBUG("CallBack_PlayStartOfFile == NULL\n");
	}

    /*PVR注册PVR_EVENT_PLAY_ERROR事件*/
	if(PvrRegisterParam->CallBack_PlayError != NULL)
	{
	    ret = FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_PLAY_ERROR, PvrRegisterParam->CallBack_PlayError, (void*)&arg);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_RegisterEvent 5 Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	else
	{
	        APP_PRINT_DEBUG("CallBack_PlayError == NULL\n");
	}

	   /*PVR注册PVR_EVENT_PLAY_ERROR事件*/
	if(PvrRegisterParam->CallBack_PlayReachRec != NULL)
	{
	    ret = FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_PLAY_REACH_REC, PvrRegisterParam->CallBack_PlayReachRec, (void*)&arg);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_RegisterEvent 5 Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	else
	{
	        APP_PRINT_DEBUG("CallBack_PlayReachRec == NULL\n");
	}


}

/******************************************************************

	描述:注销PVR出错事件函数

*******************************************************************/
BU32 APP_PVR_API_UnRegisterEventCallBack(FYF_PVR_EventCallBack_S *PvrRegisterParam)
{
	BU32 ret;
    /*PVR注销PVR_EVENT_REC_DISKFULL事件*/
	if(PvrRegisterParam->CallBack_RecDiskFull != NULL)
	{
	    ret = FYF_API_PVR_UnRegisterEvent(FYF_PVR_EVENT_REC_DISKFULL);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_UnRegisterEvent PVR_EVENT_REC_DISKFULL Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	
    /*PVR注销PVR_EVENT_REC_ERROR事件*/
	if(PvrRegisterParam->CallBack_RecError != NULL)
	{
	    ret = FYF_API_PVR_UnRegisterEvent(FYF_PVR_EVENT_REC_ERROR);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_UnRegisterEvent PVR_EVENT_REC_ERROR Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	
    /*PVR注销PVR_EVENT_PLAY_EOF事件*/
	if(PvrRegisterParam->CallBack_PlayEndOfFile != NULL)
	{
	    ret = FYF_API_PVR_UnRegisterEvent(FYF_PVR_EVENT_PLAY_EOF);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_UnRegisterEvent PVR_EVENT_PLAY_EOF Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	
    /*PVR注销PVR_EVENT_PLAY_SOF事件*/
	if(PvrRegisterParam->CallBack_PlayStartOfFile != NULL)
	{
	    ret = FYF_API_PVR_UnRegisterEvent(FYF_PVR_EVENT_PLAY_SOF);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_UnRegisterEvent PVR_EVENT_PLAY_SOF Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
	
    /*PVR注销PVR_EVENT_PLAY_ERROR事件*/
	if(PvrRegisterParam->CallBack_PlayError != NULL)
	{
	    ret = FYF_API_PVR_UnRegisterEvent(FYF_PVR_EVENT_PLAY_ERROR);
	    if (FYF_OK != ret)
	    {
	        APP_PRINT_ERROR("(%d):HI_UNF_PVR_UnRegisterEvent PVR_EVENT_PLAY_ERROR Error,s32Ret=0x%x\n", __LINE__, ret);
	        return ret;
	    }
	}
}




BU32 APP_PVR_API_SetRecTime(BU32 start_time, BU32 end_time)
{
	if (start_time > end_time)
	{
		APP_PRINT_ERROR("startime > endtime %d %d\n", start_time, end_time);
		return APP_FAIL;
	}
	g_starttime = start_time ;
	g_endtime = end_time - (end_time %60);
}

BU32 APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_E type, void * arg)
{
	BU32 *times;
	BS32 ret;
	switch(type)
	{
	case APP_PVR_ATTR_TYPE_FILENAME:
		break;
	case APP_PVR_ATTR_TYPE_STARTTIME:
		times = (BU32 *)arg;
		*times = g_starttime;
		break;
	case APP_PVR_ATTR_TYPE_ENDTIME:
		times = (BU32 *)arg;
		*times = g_endtime;
		break;
	case APP_PVR_ATTR_TYPE_RECTIME:
		times = (BU32 *)arg; 
		ret = FYF_API_PVR_GetRecTime(g_cur_recChn, times);	
		if (ret != FYF_OK)
		{
			APP_PRINT_ERROR(" FYF_API_PVR_GetRecTime error 0x%x\n", ret);
			return APP_FAIL;
		}
		//printf("file_attr.u32EndTimeInMs = %d %d\n", rec_status.u32CurTimeInMs, rec_status.u32EndTimeInMs);
		//*times = rec_status.u32EndTimeInMs;
		*times /= 1000;
		break;
	case APP_PVR_ATTR_TYPE_PLAYTIME:
		times = (BU32 *)arg; 

		ret = FYF_API_PVR_PlayGetCurPlayTime(g_cur_playChn, times);
		if (ret != FYF_OK)
		{
			APP_PRINT_ERROR(" FYF_API_PVR_PlayGetCurPlayTime error 0x%x\n", ret);
			return APP_FAIL;
		}
		
#if 0
		ret = HI_UNF_PVR_PlayGetStatus(g_cur_playChn, &play_status);
		if (ret != HI_SUCCESS)
		{
			printf(" HI_UNF_PVR_PlayGetStatus error 0x%x\n", ret);
			return APP_FAIL;
		}
		ret = HI_UNF_PVR_RecGetStatus(g_cur_recChn, &rec_status);		
		if (ret != HI_SUCCESS)
		{
			printf(" HI_UNF_PVR_RecGetStatus error 0x%x\n", ret);
			return APP_FAIL;
		}
		*times = play_status.u32CurPlayTimeInMs;
#endif
		APP_Printf("totaltime = %d\n", *times);
		if (*times == 0)
		{
			//*times = rec_status.u32EndTimeInMs;
			//*times = (*times) * play_status.u32CurPlayFrame / rec_status.u32CurWriteFrame;
		}
		*times /= 1000;
		APP_Printf("totaltime = %d\n", *times);
		break;
	default:
		break;
	}
	return APP_SUCCESS;
}

BU32 MID_PVR_API_PlaySeek(BU32 seekType, BU32 position)
{
	BS32 ret;
	BU32 play_time;
	BU32 Index = 0;
	APP_Printf("seekType = %d %d\=n", seekType, position);


	ret = FYF_API_PVR_PlaySeekChn(g_cur_playChn, seekType, position);
	if (ret != FYF_OK)
	{
		APP_PRINT_ERROR("FYF_API_PVR_PlaySeekChn error\n");
		return APP_FAIL;
	}
#if 0
	mode.enSpeed = 1024;
	g_cur_speed = 1024;
	printf("g_cur_player_speed = %d\n", g_cur_speed);
    /*PVR开始Trick模式播放*/
    ret = HI_UNF_PVR_PlayTrickMode(g_cur_playChn, &mode);
#endif
	return APP_SUCCESS;
}

BU32 APP_PVR_API_PlaySeek(BU32 chnId, BU32 seekType, BU32 position)
{
	FYF_QUEUE_MESSAGE msgSend;
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_SeekRePlay;
    msgSend.q2ndWordOfMsg= seekType;
	msgSend.q3rdWordOfMsg= position;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
	

}


//实现视频帧一帧一帧的进行播放
BS32 APP_PVR_API_PlayStep(BS08 position)
{
	BS32 ret;
	/*
	if (position < 0 && g_cur_playChn != 0xffff)
	{
		ret = HI_UNF_PVR_PlayStep(g_cur_playChn, -1);
		if (ret != FYF_OK)
		{
			APP_PRINT_ERROR("HI_UNF_PVR_PlayStep error 0x%x\n", ret);
		}
	}
	*/
	return ret;
	
}


/*
时移快进
*/
BU32 APP_PVR_API_Speed(void)
{
	BS32 ret;
	
	if (g_cur_pvr_speedIndex >= APP_PLAYER_SPEED_32X_FAST_FORWORD)
	{
		g_cur_pvr_speedIndex = APP_PLAYER_SPEED_32X_FAST_FORWORD;
		return APP_SUCCESS;
	}
	g_cur_pvr_speedIndex += 1;
	APP_Printf("g_cur_player_speed = %d\n", g_player_speed[g_cur_pvr_speedIndex]);
    /*PVR开始Trick模式播放*/
    ret = FYF_API_PVR_PlayTPlay(g_cur_playChn, g_player_speed[g_cur_pvr_speedIndex]);
    if (FYF_OK != ret)
    {
        APP_PRINT_ERROR("FYF_API_PVR_PlayTPlay Error,s32Ret=0x%x\n", ret);
        return APP_FAIL;
    }
	return APP_SUCCESS;
}

/*218.58.72.60:9300
时移快退
*/
BU32 APP_PVR_API_Rewind(void)
{
	BS32 ret;
	 
	if (g_cur_pvr_speedIndex <= APP_PLAYER_SPEED_32X_FAST_BACKWORD)
	{
		g_cur_pvr_speedIndex = APP_PLAYER_SPEED_32X_FAST_BACKWORD;
		return APP_SUCCESS;
	}
	
	g_cur_pvr_speedIndex -= 1;
	APP_Printf("g_cur_player_speed = %d\n", g_player_speed[g_cur_pvr_speedIndex]);
	/*PVR开始Trick模式播放*/
	ret = FYF_API_PVR_PlayTPlay(g_cur_playChn, g_player_speed[g_cur_pvr_speedIndex]);
	if (FYF_OK != ret)
	{
	    APP_PRINT_ERROR("FYF_API_PVR_PlayTPlay Error,s32Ret=0x%x\n", ret);
	    return APP_FAIL;
	}
	return APP_SUCCESS;
}

BU32 APP_PVR_API_GetPvrCurSpeedIndex(void)
{
    return g_cur_pvr_speedIndex;
}

void APP_PVR_API_SetPvrCurSpeedIndex(BU32 speedIndex)
{
    if(speedIndex < APP_PLAYER_SPEED_32X_FAST_BACKWORD)
    {
        g_cur_pvr_speedIndex =  APP_PLAYER_SPEED_32X_FAST_BACKWORD;
    }
	else if(speedIndex > APP_PLAYER_SPEED_32X_FAST_FORWORD)
	{
	    g_cur_pvr_speedIndex =  APP_PLAYER_SPEED_32X_FAST_FORWORD;
	}
	else
	{
	    g_cur_pvr_speedIndex = speedIndex;
	}
}



