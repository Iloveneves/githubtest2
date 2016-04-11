/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*  pvr的底层接口包括录制和时移接口      cwd 2009-09-18            			  */	
/******************************************************************************/ 

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hi_stb_api.h"
#include "fyf_def.h"
#include "dev/fyf_demux.h"
#include "pvr/fyf_pvr.h"


extern  HI_HANDLE s_AVPLAYHandle;
#define PVR_CIPHER
#define PVR_CIPHER_KEY "X5-PVR-KEY-123456789"

static BU32 fyf_video_channelhandle[3];
static BU32 fyf_audio_channelhandle[3];

//PVR录制、播放初始化
BS32 FYF_API_PVR_Init(void)
{

    BS32 s32Ret = HI_SUCCESS;

    /*PVR录制初始化*/
    s32Ret = HI_UNF_PVR_RecInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_RecInit Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*PVR播放初始化*/
    s32Ret = HI_UNF_PVR_PlayInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_PlayInit Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

//PVR录制、播放去初始化
BS32 FYF_API_PVR_DeInit(void)
{
    BS32 s32Ret = HI_SUCCESS;
    /*去初始化PVR录制*/
    s32Ret = HI_UNF_PVR_RecDeInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_RecDeInit  Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*去初始化PVR播放*/
    s32Ret = HI_UNF_PVR_PlayDeInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_PlayDeInit  Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }
    return s32Ret;
}


                        

BS32 FYF_API_PVR_demux_init(BU32 u32DmxId, BU32 source, BU16 pid, FYF_DEMUX_CHANNEL_TYPE_e chnType)
{
	HI_UNF_DMX_ATTR_S stDmxAttr;
    HI_U32 s32Ret = HI_SUCCESS;
	BU32 *channel_handle;
	DMX_CHAN_HANDLE hChannelHandle;

    if (0 == u32DmxId)
    {
    
    }
    else
    {

		s32Ret = HI_UNF_DMX_GetChannelHandle(u32DmxId, pid, &hChannelHandle);
	    if (s32Ret == HI_SUCCESS)
	    {
	        HI_UNF_DMX_CloseChannel(hChannelHandle);
	        HI_UNF_DMX_DeleteChannel(hChannelHandle);
	        printf("demuxId, = %dhChannelHandle = %x\n", u32DmxId, hChannelHandle);
	    }
		
        HI_UNF_DMX_CHAN_ATTR_S stChnAttr;

        /*demux设置录制类型*/
        s32Ret = HI_UNF_DMX_SetRecodeType(u32DmxId, HI_UNF_DMX_RECORD_DESCRAM_TS);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_SetRecodeType Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }

        /*demux获取通道默认属性*/
        s32Ret = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_GetChannelDefaultAttr Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
		if (chnType == FYF_DEMUX_VIDEO_CHANNEL)
        {
        	stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_VID;
			channel_handle = &fyf_video_channelhandle[u32DmxId];
		}
		else if (chnType == FYF_DEMUX_AUDIO_CHANNEL)
		{
			stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_AUD;
			channel_handle = &fyf_audio_channelhandle[u32DmxId];
		}
			
        stChnAttr.enCRCMode    = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
        stChnAttr.u32BufSize   = 32;
        stChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;
	#if 0
		  /*demux新建通道*/
		if(chnType == HI_UNF_DMX_CHAN_TYPE_AUD)
		{
			channel_handle = &fyf_audio_channelhandle[u32DmxId];
		}
		else if (chnType == HI_UNF_DMX_CHAN_TYPE_VID)
		{
			channel_handle = &fyf_video_channelhandle[u32DmxId];
		}
	#endif
		s32Ret = HI_UNF_DMX_NewChannel(u32DmxId, &stChnAttr, channel_handle);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_NewChannel Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }

        /*demux设置通道PID*/
        s32Ret = HI_UNF_DMX_SetChannelPID(*channel_handle, pid);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_SetChannelPID Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
		
	//把通道和密钥区进行绑定
		if (chnType == FYF_DEMUX_AUDIO_CHANNEL)
		{
			s32Ret = HI_UNF_DMX_AttachDescrambler(FYF_API_descr_GetAudioDescrambler(u32DmxId), *channel_handle);
			if (HI_SUCCESS != s32Ret)
	        {
	            printf("(%d):HI_UNF_DMX_AttachDescrambler Error,s32Ret=0x%x\n", __LINE__, s32Ret);
	           // return s32Ret;
	        }
		}
		else if (chnType == FYF_DEMUX_VIDEO_CHANNEL)
		{
			s32Ret = HI_UNF_DMX_AttachDescrambler(FYF_API_descr_GetVideoDescrambler(u32DmxId), *channel_handle);
			if (HI_SUCCESS != s32Ret)
	        {
	            printf("(%d):HI_UNF_DMX_AttachDescrambler Error,s32Ret=0x%x\n", __LINE__, s32Ret);
	           // return s32Ret;
	        }
		}
        /*demux打开通道*/
        s32Ret = HI_UNF_DMX_OpenChannel(*channel_handle);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_OpenChannel Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
    }

    printf("DMX:%d init OK, sourcetype:%d, pid:%d/%#x, chnTyp:%d\n", u32DmxId, source, pid, pid, chnType);
    return HI_SUCCESS;
}


/*PVR关闭通道*/
BS32 FYF_API_PVR_demux_deInit(BU32 u32DmxId)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (0 == u32DmxId)
    {
    }
    else
    {
        s32Ret = HI_UNF_DMX_CloseChannel(fyf_video_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_CloseChannel(fyf_audio_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DetachDescrambler(FYF_API_descr_GetAudioDescrambler(u32DmxId), fyf_audio_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DetachDescrambler(FYF_API_descr_GetVideoDescrambler(u32DmxId), fyf_video_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DeleteChannel(fyf_audio_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DeleteChannel(fyf_video_channelhandle[u32DmxId]);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_CloseChannel Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
    }
    return s32Ret;
}





BS32 FYF_API_PVR_RecNewChn(BU32 *pPvrRecChn,BU08 * file_name, BU32 aud_pid, BU32 vid_pid, BU32 scrambled)//BU08 video_type, BU08 audio_type)
{
	printf("FYF_API_PVR_RecNewChn %s %d %d\n", file_name, aud_pid, vid_pid);
	HI_S32 ret;
	PVR_REC_ATTR_S attr;

	memcpy(attr.szFileName, file_name, strlen(file_name) + 1);

	attr.u32FileNameLen = strlen(file_name);
	attr.u32DemuxID    = 1;
	attr.u32IndexPid   = vid_pid;
	attr.enIndexType   = 1;
	attr.u32ScdBufSize = PVR_STUB_SC_BUF_SZIE;
	attr.u32DavBufSize = PVR_STUB_TSDATA_SIZE;
	attr.enStreamType  = PVR_STREAM_TYPE_TS;
	attr.u32UsrDataInfoSize = 8;//sizeof(UserData_S);
	attr.u64MaxFileSize= (HI_U64)0;
	if (scrambled == 0)
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


	ret = HI_UNF_PVR_RecNewChn(pPvrRecChn, &attr);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_RecNewChn error 0x%x\n", ret);
		return FYF_ERR;
	}

	return FYF_OK;
	
}

BS32 FYF_API_PVR_RecStart(BU32 PvrRecChn)
{
	BS32 ret;
	ret = HI_UNF_PVR_RecStartChn(PvrRecChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_RecStart error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_RecStop(BU32 PvrRecChn)
{
	BS32 ret;
	ret = HI_UNF_PVR_RecStopChn(PvrRecChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_RecStop error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_RecFreeChn(BU32 PvrRecChn)
{
	BS32 ret;
	
	ret = HI_UNF_PVR_RecFreeChn(PvrRecChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_RecFreeChn error 0x%x\n", ret);
		return FYF_ERR;
	}
     
	return FYF_OK;
}

BS32 FYF_API_PVR_SetUsrData(BU08 *pFileName, BU08 *pInfo, BU32 UsrDataLen)
{
	BS32 ret;
	ret = HI_UNF_PVR_SetUsrDataInfoByFileName(pFileName, pInfo, UsrDataLen);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_SetUsrDataInfoByFileName error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_GetUsrData(BU08 *pFileName, BU08 *pInfo, BU32 BufLen, BU32 *pUsrDataLen)
{
	BS32 ret;
	ret = HI_UNF_PVR_GetUsrDataInfoByFileName(pFileName, pInfo, BufLen, pUsrDataLen);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_GetUsrDataInfoByFileName error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_GetRecTime(BU32 PvrRecChn, BU32 *pRecTime)
{
	BS32 ret;
	PVR_REC_STATUS_S status;
	ret = HI_UNF_PVR_RecGetStatus(PvrRecChn, &status);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_RecGetStatus error 0x%x\n", ret);
		return FYF_ERR;
	}
	//HI_UNF_PVR_RecGetStatus(g_cur_recChn, &status);
	printf("times = %d %d %d %d %d \n", status.u32CurTimeInMs, status.u32EndTimeInMs, status.u32StartTimeInMs, status.u32CurWriteFrame, status.u64CurWritePos);
	*pRecTime = status.u32EndTimeInMs;
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayNewChn(BU32 *pPlayChn, BU08* file_name)
{
	PVR_PLAY_ATTR_S attr;
	BS32 ret;
	
	PVR_FILE_ATTR_S fileAttr = {0};
	attr.u32DemuxID = 0;
	
	memcpy(attr.szFileName, file_name, strlen(file_name) + 1);
    attr.u32FileNameLen = strlen(file_name);
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
	ret = HI_UNF_PVR_PlayNewChn(pPlayChn, &attr);

	if (HI_SUCCESS == ret)
	{
		/*获取文件信息*/
		ret = HI_UNF_PVR_PlayGetFileAttr(*pPlayChn, &fileAttr);
		if (HI_SUCCESS == ret)
		{
			printf("file attr: frame/PES:%u, time:%ums, %ums, %ums, size:%lluByte.\n",
		       fileAttr.u32FrameNum, fileAttr.u32PlayTimeInMs, fileAttr.u32EndTimeInMs, fileAttr.u32StartTimeInMs, fileAttr.u64ValidSizeInByte);

		}
		else
		{
			printf("(%d):HI_UNF_PVR_PlayGetFileAttr Error,s32Ret=0x%x\n", __LINE__, ret);
		}
		return FYF_OK;
	}
/*
	HI_UNF_PVR_PLAY_ATTR_S     pvr_play_attr;
	HI_S32 ret;
	strcpy(pvr_play_attr.au8FileName, file_name);
	if (PvrRecChn == 0xffff)
	{
		pvr_play_attr.bTimeShift = HI_FALSE;
		pvr_play_attr.u32TimeShiftChn = -1;
	}
	else
	{
		pvr_play_attr.bTimeShift = HI_TRUE;
		pvr_play_attr.u32TimeShiftChn = PvrRecChn;
	}

	ret = HI_UNF_PVR_PlayNewChn(pPlayChn, &pvr_play_attr);

	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayNewChn error 0x%x\n", ret);
		return FYF_ERR;
	}
*/
	return FYF_ERR;

}

BS32 FYF_API_PVR_PlayFreeChn(BU32 PlayChn)
{
	BS32 ret;

	ret = HI_UNF_PVR_PlayFreeChn(PlayChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayFreeChn error 0x%x\n", ret);
		return FYF_ERR;
	}

	return FYF_OK;
}

BS32 FYF_API_PVR_PlayGetPlayTime(BU32 PlayChn, BU32 *pPlayTime)
{/*
	BS32 ret;
	ret = HI_UNF_PVR_PlayGetPlayTime(PlayChn, pPlayTime);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayGetPlayTime error 0x%x\n", ret);
		return FYF_ERR;
	}
	*/
	return FYF_OK;
	
}

BS32 FYF_API_PVR_PlayGetCurPlayTime(BU32 PlayChn, BU32 *pCurPlayTime)
{
	BS32 ret;
	PVR_PLAY_STATUS_S play_status;
	
	ret = HI_UNF_PVR_PlayGetStatus(PlayChn, &play_status);
	if (ret != HI_SUCCESS)
	{
		printf(" HI_UNF_PVR_PlayGetStatus error 0x%x\n", ret);
		return FYF_ERR;
	}
/*
	BS32 ret;
	ret = HI_UNF_PVR_PlayGetCurPlayTime(PlayChn, pCurPlayTime);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayGetCurPlayTime error 0x%x\n", ret);
		return FYF_ERR;
	}
	*/
	*pCurPlayTime = play_status.u32CurPlayTimeInMs;;
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayStart(BU32 PlayChn, BU16 AudioPid, BU16 VideoPid)
{
	BS32 ret;
	DMX_CHAN_HANDLE hChannelHandle;

	
	ret = HI_UNF_PVR_PlayStartChn(PlayChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayStart error 0x%x\n", ret);
		ret = HI_UNF_PVR_PlayFreeChn(PlayChn);
		return FYF_ERR;
	}
	
	ret = HI_UNF_DMX_GetAudioChannelHandle(0, &hChannelHandle);
	ret |= HI_UNF_DMX_SetChannelPID(hChannelHandle, AudioPid);
	if (HI_SUCCESS!= ret)
	{
		printf("FYF_API_PVR_PlayStart HI_UNF_DMX_SetChannelPID error 0x%x\n", ret);
	}
	
    ret = HI_UNF_DMX_OpenChannel(hChannelHandle);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStart HI_UNF_DMX_OpenChannel error 0x%x\n", ret);
	}

	ret = HI_UNF_DMX_GetVideoChannelHandle(0, &hChannelHandle);
	ret |= HI_UNF_DMX_SetChannelPID(hChannelHandle, VideoPid);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStart HI_UNF_DMX_SetChannelPIDerror 0x%x\n", ret);
	}
  	ret = HI_UNF_DMX_OpenChannel(hChannelHandle);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStart HI_UNF_DMX_OpenChannel error 0x%x\n", ret);
	} 
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayStop(BU32 PlayChn)
{
	BS32 ret;
	DMX_CHAN_HANDLE hChannelHandle;
	HI_UNF_AVPLAY_STOP_OPT_S StopAttr;
	ret = HI_UNF_PVR_PlayStopChn(PlayChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayStop error 0x%x\n", ret);
		return FYF_ERR;
	}

	ret = HI_UNF_AVPLAY_Stop(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
	StopAttr.s32Timeout = 0;
	StopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
	ret |=HI_UNF_AVPLAY_Stop(s_AVPLAYHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &StopAttr);
	HI_VID_FlushBuffer(VID_FLUSH_ALL);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStop HI_UNF_AVPLAY_Stop error 0x%x\n", ret);
	}

	/*
	ret = HI_UNF_DMX_GetAudioChannelHandle(0, &hChannelHandle);
	ret |= HI_UNF_DMX_CloseChannel(hChannelHandle);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStop HI_UNF_DMX_CloseChannel error 0x%x\n", ret);
	}
	ret = HI_UNF_DMX_GetVideoChannelHandle(0, &hChannelHandle);
	ret |= HI_UNF_DMX_CloseChannel(hChannelHandle);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStop HI_UNF_DMX_CloseChannel error 0x%x\n", ret);
	}
	*/
	return FYF_OK;
}


BS32 FYF_API_PVR_PlayStartTimeShift(BU32 *pPlayChn, BU32 demuxID, BU32 RecChnID, BU16 ad_pid, BU16 vd_pid)
{
	BS32 ret;
	HI_UNF_DMX_ATTR_S stDmxAttr;
	DMX_CHAN_HANDLE ChannelHandle;
    stDmxAttr.u32PortId = HI_UNF_DMX_PORT_MODE_RAM;
	ret = HI_UNF_DMX_SetAttr(demuxID, &stDmxAttr);
	
	ret = HI_UNF_DMX_GetAudioChannelHandle(demuxID, &ChannelHandle);
	ret |= HI_UNF_DMX_SetChannelPID(ChannelHandle, ad_pid);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStartTimeShift HI_UNF_DMX_SetChannelPID error 0x%x\n", ret);
	}
	
    ret = HI_UNF_DMX_OpenChannel(ChannelHandle);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStartTimeShift HI_UNF_DMX_OpenChannel error 0x%x\n", ret);
	}

	ret = HI_UNF_DMX_GetVideoChannelHandle(demuxID, &ChannelHandle);
	ret |= HI_UNF_DMX_SetChannelPID(ChannelHandle, vd_pid);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStartTimeShift HI_UNF_DMX_SetChannelPIDerror 0x%x\n", ret);
	}
  	ret = HI_UNF_DMX_OpenChannel(ChannelHandle);
	if (HI_SUCCESS != ret)
	{
		printf("FYF_API_PVR_PlayStartTimeShift HI_UNF_DMX_OpenChannel error 0x%x\n", ret);
	}

	ret = HI_UNF_PVR_PlayStartTimeShift(pPlayChn, demuxID, RecChnID);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayStartTimeShift error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayStopTimeShift(BU32 PlayChn)
{
	BS32 ret;
	DMX_CHAN_HANDLE ChannelHandle;
	ret = HI_UNF_PVR_PlayStopTimeShift(PlayChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayStopTimeShift error 0x%x\n", ret);
		return FYF_ERR;
	}

	ret = HI_UNF_DMX_GetAudioChannelHandle(0, &ChannelHandle);
	ret |= HI_UNF_DMX_CloseChannel(ChannelHandle);
	if (ret != HI_SUCCESS)
	{
		printf("FYF_API_PVR_PlayStopTimeShift HI_UNF_DMX_CloseChannel error 0x%x\n", ret);
	}
		
	ret = HI_UNF_DMX_GetVideoChannelHandle(0, &ChannelHandle);
	ret |= HI_UNF_DMX_CloseChannel(ChannelHandle);

	if (ret != HI_SUCCESS)
	{
		printf("FYF_API_PVR_PlayStopTimeShift HI_UNF_DMX_CloseChannel error 0x%x\n", ret);
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayPause(BU32 PlayChn)
{
	BS32 ret;
	ret = HI_UNF_PVR_PlayPauseChn(PlayChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayPauseChn error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayResume(BU32 PlayChn)
{
	BS32 ret;
	ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayResumeChn error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_PlayTPlay(BU32 PlayChn, BS32 speed)
{
	BS32 ret;
	PVR_PLAY_MODE_S mode;
	PVR_PLAY_STATUS_S status;
	ret = HI_UNF_PVR_PlayGetStatus(PlayChn, &status);
    if (HI_SUCCESS != ret)
    {
        printf("(%d):HI_UNF_PVR_PlayGetStatus Error,s32Ret=0x%x\n", __LINE__, ret);
        return FYF_ERR;
    }

	mode.enSpeed = speed;
	ret = HI_UNF_PVR_PlayTrickMode(PlayChn, &mode);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlayTrickMode error 0x%x\n", ret);
		return FYF_ERR;
	}
	return FYF_OK;
}

BS32 FYF_API_PVR_PlaySeekChn(BU32 PlayChn, BU32 SeekType,BU32 position)
{
	BS32 ret;
	PVR_PLAY_POSITION_S pvr_pos;
	pvr_pos.enPositionType = SeekType;
	pvr_pos.s64Offset = position * 1000;
	pvr_pos.s32Whence = SEEK_SET;
	ret = HI_UNF_PVR_PlaySeek(PlayChn, &pvr_pos);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_PVR_PlaySeek error 0x%x\n", ret);
		return FYF_ERR;
	}

	return FYF_OK;
	
}

BS32 FYF_API_PVR_GetPVRFileAttr(BU32 PlayChn, FYF_PVR_FILE_ATTR_S *attr)
{
	BS32 ret;
	ret = HI_UNF_PVR_PlayGetFileAttr(PlayChn, (PVR_FILE_ATTR_S*)attr);
    if (HI_SUCCESS == ret)
    {
      // printf("file attr: frame/PES:%u, time:%ums, size:%lluByte.\n",
         //          attr->u32FrameNum, attr->u32PlayTimeInMs, attr->u64ValidSizeInByte);
    }
	else
	{
		printf("HI_UNF_PVR_PlayGetFileAttr error 0x%x", ret);
	}
	return ret;
}

BS32 FYF_API_PVR_RegisterEvent(FYF_PVR_EVENT_E EventType, EventCallBack callback, void *arg)
{
	BS32 ret;
	ret = HI_UNF_PVR_RegisterEvent(EventType, callback, arg);
    if (HI_SUCCESS != ret)
    {
        printf("(%d):HI_UNF_PVR_RegisterEvent 2 Error,s32Ret=0x%x\n", __LINE__, ret);
		return FYF_ERR;
	}
	return FYF_OK;

}

BS32 FYF_API_PVR_UnRegisterEvent(FYF_PVR_EVENT_E EventType)
{
	BS32 ret;
	ret = HI_UNF_PVR_UnRegisterEvent(EventType);
    if (HI_SUCCESS != ret)
    {
        printf("(%d):HI_UNF_PVR_UnRegisterEvent 2 Error,s32Ret=0x%x\n", __LINE__, ret);
		return FYF_ERR;
	}
	return FYF_OK;
}









//下面是播放器的接口



BS32 FYF_API_Player_Open(BU08 *pFileName)
{
	return HI_Player_Open(pFileName);
}


BS32 FYF_API_Player_Close()
{
	return HI_Player_Close();
}
BS32 FYF_API_Player_Start()
{
	return HI_Player_Start();
}

BS32 FYF_API_Player_Stop()
{
	return HI_Player_Stop();
}

BS32 FYF_API_Player_Resume()
{
	return HI_Player_Resume();
}

BS32 FYF_API_Player_Pause()
{
	return HI_Player_Pause();
}

BS32 FYF_API_Player_Seek(BU32 MsTime)
{
	return HI_Player_Seek(MsTime);
}

BS32 FYF_API_Player_GetCurrentTime(BU32 *pTime)
{
	return HI_Player_GetCurrentTime(pTime);
}

BS32 FYF_API_Player_GetTotalTime(BU32 *pTime)
{
	return HI_Player_GetDuration(pTime);
}