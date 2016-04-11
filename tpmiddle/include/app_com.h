#ifndef _APP_COM_H
#define _APP_COM_H


#define APP_Com_Api_Sprintf sprintf

typedef enum
{
	APP_Queue_WriteE2_e = 0, //0
	APP_Queue_LockTuner_e,//1//1
	APP_Queue_Search_e,//2//2
	APP_Queue_PlayChn_e,//3//3
	APP_Queue_StopPlayChn_e,//4//4
	APP_Queue_ShowBmp_e,//fl 增加对logo显示的入栈操作
	APP_Queue_StartVediodecode_e,
	APP_Queue_StopVediodecode_e,
	APP_Queue_SetVideoOutRect_e,
	APP_Queue_ShowPhoto,//数码相框显示
	APP_Queue_PlayMp3,
	APP_Queue_StopMp3,
	APP_Queue_ResumeMp3,
	APP_Queue_PauseMp3,
	APP_Queue_PlayFilm,
	APP_Queue_StopFilm,
	APP_Queue_ResumeFilm,
	APP_Queue_PauseFilm,
	APP_Queue_AttachAV,//18
	APP_Queue_DetachAV,//19
	APP_Queue_StartRec,//1//20
	APP_Queue_StopRec,//21
	APP_Queue_StartRePlay,
	APP_Queue_StopRePlay,
	APP_Queue_ResumeRePlay,
	APP_Queue_PauseRePlay,
	APP_Queue_DetectOTA,
	APP_Queue_SeekRePlay,
	APP_Queue_PlayChn_NoPsiSi_e,	//播放没有PSI/SI的节目
	APP_Queue_PlayNvodChn_e,//播放nvod节目
	APP_Queue_Ipannel_Start,
	APP_Queue_SpeedMP3,
	APP_Queue_RewindMP3,
	APP_Queue_SetEcmNull_e,
	APP_Queue_PlayTimeShift,
	APP_Queue_PlayVod
}APP_Queue_E;

//add by cwd 2009-12-05
//事件弹框通知
typedef enum 
{
	APP_EVENT_NOTIFY_USB_INSERT = 0x0,
	APP_EVENT_NOTIFY_USB_EVULSION = 0x01,
	APP_EVENT_NOTIFY_FILE_DELETE = 0x02, //删除文件
	APP_EVENT_NOTIFY_FILE_NULL,			//文件夹是空目录
	APP_EVENT_NOTIFY_NO_USB_DEVICE,		//没有插入usb设备
	APP_EVENT_NOTIFY_ROOT_DIR,			//当前目录是根目录
	APP_EVENT_NOTIFY_PLAY_RESERVE,
	APP_EVENT_NOTIFY_SYS_UPDATE,
	APP_EVENT_NOTIFY_REC_STOP,         //预约录制结束
	APP_EVENT_NOTIFY_DISK_FULL,
	APP_EVENT_NOTIFY_OTHER
}APP_EVENT_Notify_TYPE_E;

typedef BU32	(*SIGNALNOTIFY)(BU32 para1,BU32 para2);


/*
根据节目号播放节目
*/
//BU32 APP_Com_API_PlayChn(BU16 chnNO);
BU32 APP_Com_API_PlayChn(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 u16ChnNO, BS32 s32ServiceID);
/*
显示前面板
*/
BU32 APP_Com_API_ShowLED(BU08 str[4]);
/*
停止播放节目
*/
BU32 APP_Com_API_StopProg(BU32 isNeedBlkScrn);
/*
缩小视频
*/
BU32 APP_Com_API_SetSmallScreen(BU32 x,BU32 y,BU16 w,BU32 h);
/*
视频全屏显示
*/
BU32 APP_Com_API_SetFullScreen(void);
/*-------------------------------------------------------------------------------

	Description:显示logo 

	Paramenters: 0为黑屏，1为radio，2为无信号

-------------------------------------------------------------------------------*/
void APP_Com_API_ShowBmp(BU32 bmp);
/*-------------------------------------------------------------------------------

	Description:  启动视频解码器

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_API_StartVedioDecode(void);
/*-------------------------------------------------------------------------------

	Description: 停止视频解码器

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_API_StopVedioDecode(void);
/*-------------------------------------------------------------------------------

	Description: 设置信号检测的回调函数

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_SetSignalNotifyFun(SIGNALNOTIFY Notify );
/*-------------------------------------------------------------------------------

	Description: 将CodeDate转换为年月日周

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_CodeDateToNormalDate(BU32 iCodedDate,BU16 *psYear, BU08 *pcMonth, BU08 *pcDate, BU08 *pWeekDay);
/*-------------------------------------------------------------------------------

	Description: 将年月日转换为CodeDate


	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_NormalDateToCodeDate(BU16 sYear, BU08 cMonth, BU08 cDate, BU32 *piCodedDate);
/*-------------------------------------------------------------------------------

	Description: 串口打印

	Parameters:

-------------------------------------------------------------------------------*/
#if 0
void APP_Com_API_Printf(const char* fmt, ...);
#endif

#endif




