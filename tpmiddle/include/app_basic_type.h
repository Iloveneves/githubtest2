/*===============================================================================

 File Name:app_base_type.h

 Description:

 History:
===============================================================================*/
#ifndef _APP_BASE_TYPE_H_
#define _APP_BASE_TYPE_H_

#define APP_ON      1//开
#define APP_OFF     0//关

#define APP_TRUE (1)
#define APP_FALSE (0)

#define APP_OK (0)
#define APP_ERR (-1)

#define APP_SUCCESS (1)
#define APP_FAIL (0)

#define APP_CHN_NAME_LEN  41

#define APP_INVAL_CHN_NO 0xffff//无效的节目号

#define APP_SIGNAL_UNLOCK 0
#define APP_SIGNAL_LOCK   1
#define APP_SIGNAL_STOP   2
#define APP_SIGNAL_PLAY   3
#define APP_SIGNAL_PLAYCHANGE   4
#define APP_MAIL_MAX_NUM  50

/*------
QAM调制类型
--------*/
typedef enum
{
	APP_CHN_QAM_16_e = 0, 
	APP_CHN_QAM_32_e, 
	APP_CHN_QAM_64_e, 
	APP_CHN_QAM_128_e, 
	APP_CHN_QAM_256_e
}APP_CHN_QAM_E;

/*------
声道类型
--------*/
typedef enum
{
	APP_CHN_TRACK_STEREO_e = 0, //立体声
	APP_CHN_TRACK_LEFT_e, //左声道
	APP_CHN_TRACK_RIGHT_e, //右声道
	APP_CHN_TRACK_SINGLE_e //单声道
}APP_CHN_TRACK_E;
/*------
频道类型
--------*/
typedef enum
{
	APP_CHN_AV_TYPE_NULL = 0,
	APP_CHN_AV_TYPE_TV_e=1, //电视
	APP_CHN_AV_TYPE_RD_e,//广播
	APP_CHN_AV_TYPE_NVOD_REFERENCE_SERVICE,
	APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE,
	APP_CHN_AV_TYPE_BROADCAST_e,
	APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE_DESC,
}APP_CHN_AV_TYPE_E;
/*------
频道组类型
--------*/
typedef enum
{
	APP_CHN_GROUP_TYPE_ALL_e = 0, //所有节目
	APP_CHN_GROUP_TYPE_FAV_e, //喜爱节目
	APP_CHN_GROUP_TYPE_1_e,//节目分类1
	APP_CHN_GROUP_TYPE_2_e,
	APP_CHN_GROUP_TYPE_3_e,
	APP_CHN_GROUP_TYPE_4_e,
	APP_CHN_GROUP_TYPE_5_e,
	APP_CHN_GROUP_TYPE_6_e,
	APP_CHN_GROUP_TYPE_7_e,
	APP_CHN_GROUP_TYPE_8_e,
	APP_CHN_GROUP_TYPE_9_e,
	APP_CHN_GROUP_TYPE_10_e,
	APP_CHN_GROUP_TYPE_11_e,
	APP_CHN_GROUP_TYPE_12_e,
	APP_CHN_GROUP_TYPE_13_e,
	APP_CHN_GROUP_TYPE_14_e,
	APP_CHN_GROUP_TYPE_15_e,
	APP_CHN_GROUP_TYPE_16_e,
	APP_CHN_GROUP_TYPE_17_e,
	APP_CHN_GROUP_TYPE_18_e,
	APP_CHN_GROUP_TYPE_19_e,
	APP_CHN_GROUP_TYPE_20_e,
	APP_CHN_GROUP_TYPE_21_e,
	APP_CHN_GROUP_TYPE_22_e,
	APP_CHN_GROUP_TYPE_23_e,
	APP_CHN_GROUP_TYPE_24_e,
	APP_CHN_GROUP_TYPE_25_e,
	APP_CHN_GROUP_TYPE_26_e,
	APP_CHN_GROUP_TYPE_EDIT_e//编辑节目
}APP_CHN_GROUP_TYPE_E;

/*------
频道信息
--------*/
typedef struct _ChnDataBase_ChnInfo_S
{
	BU32 m_ProgId;
	BU08 m_ChnName[APP_CHN_NAME_LEN];
	BU32 m_ChnIndex;
	BU08 m_Fav;
	BU08 m_Lock;
	BU08 m_Ca;
	BU08 m_Skip;
	BU08 m_Del;
	BU08 m_AudioChn;
	BS08 m_AudioOffset;
	BU32 m_TsId;
	BU16 m_pmtpid;
	BU16 m_AudioPid;
	BU16 m_VideoPid;
	BU16 m_PcrPid;
	BU08 m_style;
	BU16 m_FreqMHz;
	BU16 m_SymKbps;
	BU08 m_QAM;
	BU16 m_CAT_Sys_ID;
	BU16 m_EcmPID;
	BU16 m_audioEcmPID;
	BU16 m_videoEcmPID;
	BU16 m_OriNetId;
	BU08 m_AudioType;
	BU08 m_VideoType;
	BU32 m_ProgLCN;
	BU32 m_SortFlag;
	BS08 m_AudioVolume;//分台存储的音量
}ChnDataBase_ChnInfo_S;


typedef struct _BroadCast_ChnInfo_S
{
	BU32 m_ProgId;
	BU08 m_ChnName[APP_CHN_NAME_LEN];
	BU32 m_ChnIndex;
	BU08 m_AudioChn;
	BU32 m_TsId;
	BU16 m_pmtpid;
	BU16 m_AudioPid;
	BU16 m_VideoPid;
	BU16 m_PcrPid;
	BU08 m_style;
	BU16 m_FreqMHz;
	BU16 m_SymKbps;
	BU08 m_QAM;
	BU16 m_OriNetId;
}BroadCast_ChnInfo_S;

typedef struct _ChnDataBase_ChnInfo_NoPsiSi_s
{
	BU16 m_AudioPid;
	BU16 m_VideoPid;
	BU16 m_PcrPid;
	BU16 m_audioEcmPID;
	BU16 m_videoEcmPID;	
}ChnDataBase_ChnInfo_NoPsiSi_s;
#endif


