#ifndef _H_APP_PVR_H_
#define _H_APP_PVR_H_


typedef enum
{
	APP_REC_STATUS_STOP_e = 0,
	APP_REC_STATUS_PLAY_e,
	APP_REC_STATUS_BUTT_e
}APP_REC_STATUS_TYPE_E;

typedef enum
{
	APP_PVR_STATUS_STOP_e = 0,
	APP_PVR_STATUS_PLAY_e,
	APP_PVR_STATUS_PAUSE_e,
	APP_PVR_STATUS_BUTT_e
}APP_PVR_STATUS_TYPE_E;

typedef enum
{
	APP_PVR_ATTR_TYPE_FILENAME = 0,//��ȡ�ļ����ļ���
	APP_PVR_ATTR_TYPE_STARTTIME,       //��ȡ�ļ���¼�ƿ�ʼʱ��
	APP_PVR_ATTR_TYPE_ENDTIME,     //��ȡ�ļ���¼�ƽ���ʱ��
	APP_PVR_ATTR_TYPE_RECTIME,      //��ȡ�ļ���¼�Ƶ�ǰʱ��
	APP_PVR_ATTR_TYPE_PLAYTIME    //��ȡ�ļ��ĵ�ǰ����ʱ��
}APP_PVR_ATTR_TYPE_E;

typedef struct _UserData_S
{
	BU16 vd_pid;
	BU16 ad_pid;
	BU32 total_time;
}UserData_S;

typedef enum _APP_PVR_EVENT_E
{
    APP_PVR_EVENT_PLAY_EOF        = 0x001,                         /* end fo file */
    APP_PVR_EVENT_PLAY_SOF        = 0x002,                         /* start of file */
    APP_PVR_EVENT_PLAY_ERROR      = 0x003,                         /* play error */
    APP_PVR_EVENT_PLAY_REACH_REC  = 0x004,                         /* timeshift play reach to record end */
    APP_PVR_EVENT_PLAY_RESV       = 0x00f,                         /* reserved for play */
    APP_PVR_EVENT_REC_DISKFULL    = 0x010,                         /* disk full */
    APP_PVR_EVENT_REC_ERROR       = 0x011,                         /* record error */
    APP_PVR_EVENT_REC_OVER_FIX    = 0x012,                         /* record size will over fix size */
    APP_PVR_EVENT_REC_REACH_PLAY  = 0x013,                         /* timeshift record reach to play positon */
    APP_PVR_EVENT_REC_RESV        = 0x01f,                         /* reserved for record */
    APP_PVR_EVENT_BUTT            = 0x020
}APP_PVR_EVENT_E;
#if 0

typedef void (*EventCallBack)(BU32 u32ChnID,APP_PVR_EVENT_E EventType,BS32 s32EventValue,void *args);
typedef struct _APP_PVR_EventCallBack_S
{
	EventCallBack CallBack_RecDiskFull;		//������
	EventCallBack CallBack_RecError;		//¼���ڲ�����
	EventCallBack CallBack_PlayEndOfFile;	//���ŵ��ļ�β
	EventCallBack CallBack_PlayStartOfFile;	//���˵��ļ�ͷ
	EventCallBack CallBack_PlayError;		//�����ڲ�����
	EventCallBack CallBack_PlayReachRec;	//ʱ�Ƶ�ʱ�򲥷�׷��¼��
	EventCallBack CallBack_RecOverFixSize;	//¼�Ƴ��ȴﵽָ���ĳ���
	EventCallBack CallBack_RecReachPlay;	//ʱ�Ƶ�ʱ��¼��׷�ϲ���

}APP_PVR_EventCallBack_S;
#endif

/*
����:��ʼ¼��
*/
BU32 APP_PVR_API_StartRec(BU32 chnNo, BU32 demuxId);

/*
����:ֹͣ¼��
*/
BU32 APP_PVR_API_StopRec(BU32 demuxId);

BU32 APP_PVR_API_SetSpeed(BS32 speed);

/*
����:ע��PVR�����¼�����
*/
BU32 APP_PVR_API_RegisterEventCallBack(FYF_PVR_EventCallBack_S* PvrRegisterParam);

/*
����:ע��PVR�����¼�����
*/
BU32 APP_PVR_API_UnRegisterEventCallBack(FYF_PVR_EventCallBack_S* PvrRegisterParam);

#endif

