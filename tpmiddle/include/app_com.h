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
	APP_Queue_ShowBmp_e,//fl ���Ӷ�logo��ʾ����ջ����
	APP_Queue_StartVediodecode_e,
	APP_Queue_StopVediodecode_e,
	APP_Queue_SetVideoOutRect_e,
	APP_Queue_ShowPhoto,//���������ʾ
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
	APP_Queue_PlayChn_NoPsiSi_e,	//����û��PSI/SI�Ľ�Ŀ
	APP_Queue_PlayNvodChn_e,//����nvod��Ŀ
	APP_Queue_Ipannel_Start,
	APP_Queue_SpeedMP3,
	APP_Queue_RewindMP3,
	APP_Queue_SetEcmNull_e,
	APP_Queue_PlayTimeShift,
	APP_Queue_PlayVod
}APP_Queue_E;

//add by cwd 2009-12-05
//�¼�����֪ͨ
typedef enum 
{
	APP_EVENT_NOTIFY_USB_INSERT = 0x0,
	APP_EVENT_NOTIFY_USB_EVULSION = 0x01,
	APP_EVENT_NOTIFY_FILE_DELETE = 0x02, //ɾ���ļ�
	APP_EVENT_NOTIFY_FILE_NULL,			//�ļ����ǿ�Ŀ¼
	APP_EVENT_NOTIFY_NO_USB_DEVICE,		//û�в���usb�豸
	APP_EVENT_NOTIFY_ROOT_DIR,			//��ǰĿ¼�Ǹ�Ŀ¼
	APP_EVENT_NOTIFY_PLAY_RESERVE,
	APP_EVENT_NOTIFY_SYS_UPDATE,
	APP_EVENT_NOTIFY_REC_STOP,         //ԤԼ¼�ƽ���
	APP_EVENT_NOTIFY_DISK_FULL,
	APP_EVENT_NOTIFY_OTHER
}APP_EVENT_Notify_TYPE_E;

typedef BU32	(*SIGNALNOTIFY)(BU32 para1,BU32 para2);


/*
���ݽ�Ŀ�Ų��Ž�Ŀ
*/
//BU32 APP_Com_API_PlayChn(BU16 chnNO);
BU32 APP_Com_API_PlayChn(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 u16ChnNO, BS32 s32ServiceID);
/*
��ʾǰ���
*/
BU32 APP_Com_API_ShowLED(BU08 str[4]);
/*
ֹͣ���Ž�Ŀ
*/
BU32 APP_Com_API_StopProg(BU32 isNeedBlkScrn);
/*
��С��Ƶ
*/
BU32 APP_Com_API_SetSmallScreen(BU32 x,BU32 y,BU16 w,BU32 h);
/*
��Ƶȫ����ʾ
*/
BU32 APP_Com_API_SetFullScreen(void);
/*-------------------------------------------------------------------------------

	Description:��ʾlogo 

	Paramenters: 0Ϊ������1Ϊradio��2Ϊ���ź�

-------------------------------------------------------------------------------*/
void APP_Com_API_ShowBmp(BU32 bmp);
/*-------------------------------------------------------------------------------

	Description:  ������Ƶ������

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_API_StartVedioDecode(void);
/*-------------------------------------------------------------------------------

	Description: ֹͣ��Ƶ������

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_API_StopVedioDecode(void);
/*-------------------------------------------------------------------------------

	Description: �����źż��Ļص�����

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_SetSignalNotifyFun(SIGNALNOTIFY Notify );
/*-------------------------------------------------------------------------------

	Description: ��CodeDateת��Ϊ��������

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_CodeDateToNormalDate(BU32 iCodedDate,BU16 *psYear, BU08 *pcMonth, BU08 *pcDate, BU08 *pWeekDay);
/*-------------------------------------------------------------------------------

	Description: ��������ת��ΪCodeDate


	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_NormalDateToCodeDate(BU16 sYear, BU08 cMonth, BU08 cDate, BU32 *piCodedDate);
/*-------------------------------------------------------------------------------

	Description: ���ڴ�ӡ

	Parameters:

-------------------------------------------------------------------------------*/
#if 0
void APP_Com_API_Printf(const char* fmt, ...);
#endif

#endif




