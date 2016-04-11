#ifndef _FYF_PVR_H_
#define _FYF_PVR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _FYF_PVR_FILE_ATTR_S
{
    BU32		u32FrameNum;  /* when index for audio, frame number means pes number */
    BU32        u32PlayTimeInMs;
    BU32        u32StartTimeInMs;
    BU32        u32EndTimeInMs;
    BU64        u64ValidSizeInByte;
} FYF_PVR_FILE_ATTR_S;


typedef enum  _PVR_PLAY_SPEED_E
{
	FYF_PVR_PLAY_SPEED_X2_FAST_BACKWORD = -2,
    FYF_PVR_PLAY_SPEED_X4_FAST_BACKWORD = -4,
    FYF_PVR_PLAY_SPEED_X8_FAST_BACKWORD = -8,
    FYF_PVR_PLAY_SPEED_X16_FAST_BACKWORD = -16,
    FYF_PVR_PLAY_SPEED_X32_FAST_BACKWORD = -32,
	
	FYF_PVR_PLAY_SPEED_NORMAL_SPEED = 1,
    FYF_PVR_PLAY_SPEED_X2_FAST_FORWORD = 2,
    FYF_PVR_PLAY_SPEED_X4_FAST_FORWORD = 4,
    FYF_PVR_PLAY_SPEED_X8_FAST_FORWORD = 8,
    FYF_PVR_PLAY_SPEED_X16_FAST_FORWORD = 16,
    FYF_PVR_PLAY_SPEED_X32_FAST_FORWORD = 32,

	FYF_PVR_PLAY_SPEED_X2_SLOW_FORWORD = 2*32,
    FYF_PVR_PLAY_SPEED_X4_SLOW_FORWORD = 4*32,
    FYF_PVR_PLAY_SPEED_X8_SLOW_FORWORD = 8*32,
   	FYF_PVR_PLAY_SPEED_X16_SLOW_FORWORD = 16*32,
    FYF_PVR_PLAY_SPEED_X32_SLOW_FORWORD = 32*32,
    
    FYF_PVR_PLAY_SPEED_BUTT
}FYF_PVR_PLAY_SPEED_E;


typedef enum _FYF_PVR_EVENT_E
{
    FYF_PVR_EVENT_PLAY_EOF        = 0x001,                         /* end fo file */
    FYF_PVR_EVENT_PLAY_SOF        = 0x002,                         /* start of file */
    FYF_PVR_EVENT_PLAY_ERROR      = 0x003,                         /* play error */
    FYF_PVR_EVENT_PLAY_REACH_REC  = 0x004,                         /* timeshift play reach to record end */
    FYF_PVR_EVENT_PLAY_RESV       = 0x00f,                         /* reserved for play */
    FYF_PVR_EVENT_REC_DISKFULL    = 0x010,                         /* disk full */
    FYF_PVR_EVENT_REC_ERROR       = 0x011,                         /* record error */
    FYF_PVR_EVENT_REC_OVER_FIX    = 0x012,                         /* record size will over fix size */
    FYF_PVR_EVENT_REC_REACH_PLAY  = 0x013,                         /* timeshift record reach to play positon */
    FYF_PVR_EVENT_REC_RESV        = 0x01f,                         /* reserved for record */
    FYF_PVR_EVENT_BUTT            = 0x020
}FYF_PVR_EVENT_E;

typedef void (*EventCallBack)(BU32 u32ChnID, FYF_PVR_EVENT_E EventType, BS32 s32EventValue, void *args);
typedef struct _FYF_PVR_EventCallBack_S
{
	EventCallBack CallBack_RecDiskFull;		//磁盘满
	EventCallBack CallBack_RecError;		//录制内部错误
	EventCallBack CallBack_PlayEndOfFile;	//播放到文件尾
	EventCallBack CallBack_PlayStartOfFile;	//回退到文件头
	EventCallBack CallBack_PlayError;		//播放内部错误
	EventCallBack CallBack_PlayReachRec;	//时移的时候播放追上录制
	EventCallBack CallBack_RecOverFixSize;	//录制长度达到指定的长度
	EventCallBack CallBack_RecReachPlay;	//时移的时候录制追上播放

}FYF_PVR_EventCallBack_S;


//PVR录制、播放初始化
BS32 FYF_API_PVR_Init(void);

//PVR录制、播放去初始化
BS32 FYF_API_PVR_DeInit(void);

BS32 FYF_API_PVR_demux_init(BU32 u32DmxId, BU32 source, BU16 pid, FYF_DEMUX_CHANNEL_TYPE_e chnType);

/*PVR关闭通道*/
BS32 FYF_API_PVR_demux_deInit(BU32 u32DmxId);

BS32 FYF_API_PVR_RecNewChn(BU32 *pPvrRecChn,BU08 * file_name, BU32 aud_pid, BU32 vid_pid, BU32 scrambled);

BS32 FYF_API_PVR_RecStart(BU32 PvrRecChn);

BS32 FYF_API_PVR_RecStop(BU32 PvrRecChn);

BS32 FYF_API_PVR_RecFreeChn(BU32 PvrRecChn);


BS32 FYF_API_PVR_SetUsrData(BU08 *pFileName, BU08 *pInfo, BU32 UsrDataLen);

BS32 FYF_API_PVR_GetUsrData(BU08 *pFileName, BU08 *pInfo, BU32 BufLen, BU32 *pUsrDataLen);

BS32 FYF_API_PVR_GetRecTime(BU32 PvrRecChn, BU32 *pRecTime);


BS32 FYF_API_PVR_PlayNewChn(BU32 *pPlayChn, BU08* file_name);

BS32 FYF_API_PVR_PlayFreeChn(BU32 PlayChn);

BS32 FYF_API_PVR_PlayGetPlayTime(BU32 PlayChn, BU32 *pPlayTime);

BS32 FYF_API_PVR_PlayGetCurPlayTime(BU32 PlayChn, BU32 *pCurPlayTime);

BS32 FYF_API_PVR_PlayStart(BU32 PlayChn, BU16 AudioPid, BU16 VideoPid);

BS32 FYF_API_PVR_PlayStop(BU32 PlayChn);


BS32 FYF_API_PVR_PlayStopTimeShift(BU32 PlayChn);

BS32 FYF_API_PVR_PlayPause(BU32 PlayChn);


BS32 FYF_API_PVR_PlayStartTimeShift(BU32 *pPlayChn, BU32 demuxID, BU32 RecChnID, BU16 ad_pid, BU16 vd_pid);

BS32 FYF_API_PVR_PlayResume(BU32 PlayChn);


BS32 FYF_API_PVR_PlayTPlay(BU32 PlayChn, BS32 speed);

BS32 FYF_API_PVR_PlaySeekChn(BU32 PlayChn, BU32 SeekType,BU32 position);



#ifdef __cplusplus
}
#endif

#endif

