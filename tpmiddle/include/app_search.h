/*===============================================================================

 File Name:app_search_api.h

 Description:

 History:
===============================================================================*/
#ifndef _APP_SEARCH_H_
#define _APP_SEARCH_H_

#define APP_MAIN_FREQ_MHZ 642
#define APP_NVOD_FREQ_MHZ 443
#define APP_AD_FREQ_MHZ 315
#define APP_DB_FREQ_MHZ 427
#define APP_STOCK_FREQ_MHZ 411

#define APP_MAIN_FREQ_MHZ_BCK 554

#define APP_OTA_FREQ_MHZ 642


/*表缓存大小*/
#define DEMUX_BUFFER_QUEUE_SIZE 100

/*等待频点锁定时间*/
#define TIME_WAIT_FOR_TUNER_LOCK 2000

/*
等待滤取各种表的时间
*/
#define TIME_WAIT_FOR_NIT 5000
#define TIME_WAIT_FOR_PAT 5000
#define TIME_WAIT_FOR_SDT 5000
#define TIME_WAIT_FOR_CAT 500
#define TIME_WAIT_FOR_PMT 5000

/*最大同时开始滤取PMT表的通道数*/
#define MAX_START_DEMUX_PMT 15

typedef struct
{
	BU08     	pSection[4096];               /* pointer to section               */
	BU32    	SectionLength;          /* length of section data           */
	BU32		chanelID;
	BU32		filterID;
}  EPGQUEUE_ELEMENTS;

/*------
搜索方式
--------*/
typedef enum
{
	APP_SEARCH_TYPE_ALL_e = 0, //全频段
	APP_SEARCH_TYPE_AUTO_e, //自动
	APP_SEARCH_TYPE_MANUAL_e //手动
}APP_SEARCH_TYPE_E;


typedef struct _tagAPP_Signal_BaseInfo_S
{
	BU32 m_BLock;//是否锁定
	BU32 m_SignalLever;//信号电平
	BU32 m_SNR;//信噪比
	BU32 m_BER;//误码率
}APP_Signal_BaseInfo_S;
typedef APP_Signal_BaseInfo_S* APP_Signal_BaseInfo_SP;

typedef struct _tagAPP_Tuner_FreqInfo_S
{
	BU32 m_FreqMHz      ;
	BU32 m_SymKbps      ;
	APP_CHN_QAM_E m_QAM;	
}APP_Tuner_FreqInfo_S;
typedef APP_Tuner_FreqInfo_S* APP_Tuner_FreqInfo_SP;


typedef void (*APP_Search_Callback_AddAChn)(ChnDataBase_ChnInfo_S *chnInfoP,APP_CHN_AV_TYPE_E avType);
typedef void (*APP_Search_Callback_UpdateSignalInfo)(APP_Signal_BaseInfo_SP signalBaseInfo);
typedef void (*APP_Search_Callback_UpdateFreqInfo)(APP_Tuner_FreqInfo_SP freqInfo,BU32 NO,BU32 Num);
typedef void (*APP_Search_Callback_UpdateProgress)(BU08 progress);
typedef void (*APP_Search_Callback_EndOfSave)(void);
/*----------
搜索节目时的参数
------------*/
typedef struct _tagAPP_Search_Param_S
{
	BS32 m_StartFreqMHz        ;
	BS32 m_StopFreqMHz         ;
	BS32 m_SymKbps             ;
	APP_CHN_QAM_E m_QamMode    ;
	BU08 m_bNitOption          ;
	APP_SEARCH_TYPE_E m_SrchType ;
	APP_Search_Callback_AddAChn          m_Callback_AddAChn;//搜到一个节目时，会调用该函数，上层可以用来显示搜到的节目
	APP_Search_Callback_UpdateSignalInfo m_Callback_UpdateSignalInfo;//用来更新信号状态的回调
	APP_Search_Callback_UpdateFreqInfo   m_Callback_UpdateFreqInfo;//用来更新频点信息的回调
	APP_Search_Callback_UpdateProgress   m_Callback_UpdateProgress;//用来更新搜索进度的回调，当进度到达100%的时候，界面要跳转到保存数据请等待。。。
	APP_Search_Callback_EndOfSave        m_Callback_EndOfSave;//用来通知上层，保存数据已经完毕。
}APP_Search_Param_S;
typedef APP_Search_Param_S* APP_Search_Param_SP;




/*
开始搜索节目
*/
BU32 APP_Search_API_StartSearch(APP_Search_Param_SP paramP);
/*
正常结束搜索
*/
void APP_Search_API_EndSearch(void);
/*
中断搜索
*/
void APP_Search_API_HaltSearch(void);
/*
开始保存
*/
void APP_Search_API_StartSave(void);
/*
搜索节目分类信息
*/
void APP_Search_API_SearchSortInfo(void);
/*
设置是否打印搜索回显
*/
void APP_Search_API_SetPrintf(BU08 isPrintf);


#endif











