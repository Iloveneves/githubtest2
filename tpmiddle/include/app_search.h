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


/*�����С*/
#define DEMUX_BUFFER_QUEUE_SIZE 100

/*�ȴ�Ƶ������ʱ��*/
#define TIME_WAIT_FOR_TUNER_LOCK 2000

/*
�ȴ���ȡ���ֱ��ʱ��
*/
#define TIME_WAIT_FOR_NIT 5000
#define TIME_WAIT_FOR_PAT 5000
#define TIME_WAIT_FOR_SDT 5000
#define TIME_WAIT_FOR_CAT 500
#define TIME_WAIT_FOR_PMT 5000

/*���ͬʱ��ʼ��ȡPMT���ͨ����*/
#define MAX_START_DEMUX_PMT 15

typedef struct
{
	BU08     	pSection[4096];               /* pointer to section               */
	BU32    	SectionLength;          /* length of section data           */
	BU32		chanelID;
	BU32		filterID;
}  EPGQUEUE_ELEMENTS;

/*------
������ʽ
--------*/
typedef enum
{
	APP_SEARCH_TYPE_ALL_e = 0, //ȫƵ��
	APP_SEARCH_TYPE_AUTO_e, //�Զ�
	APP_SEARCH_TYPE_MANUAL_e //�ֶ�
}APP_SEARCH_TYPE_E;


typedef struct _tagAPP_Signal_BaseInfo_S
{
	BU32 m_BLock;//�Ƿ�����
	BU32 m_SignalLever;//�źŵ�ƽ
	BU32 m_SNR;//�����
	BU32 m_BER;//������
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
������Ŀʱ�Ĳ���
------------*/
typedef struct _tagAPP_Search_Param_S
{
	BS32 m_StartFreqMHz        ;
	BS32 m_StopFreqMHz         ;
	BS32 m_SymKbps             ;
	APP_CHN_QAM_E m_QamMode    ;
	BU08 m_bNitOption          ;
	APP_SEARCH_TYPE_E m_SrchType ;
	APP_Search_Callback_AddAChn          m_Callback_AddAChn;//�ѵ�һ����Ŀʱ������øú������ϲ����������ʾ�ѵ��Ľ�Ŀ
	APP_Search_Callback_UpdateSignalInfo m_Callback_UpdateSignalInfo;//���������ź�״̬�Ļص�
	APP_Search_Callback_UpdateFreqInfo   m_Callback_UpdateFreqInfo;//��������Ƶ����Ϣ�Ļص�
	APP_Search_Callback_UpdateProgress   m_Callback_UpdateProgress;//���������������ȵĻص��������ȵ���100%��ʱ�򣬽���Ҫ��ת������������ȴ�������
	APP_Search_Callback_EndOfSave        m_Callback_EndOfSave;//����֪ͨ�ϲ㣬���������Ѿ���ϡ�
}APP_Search_Param_S;
typedef APP_Search_Param_S* APP_Search_Param_SP;




/*
��ʼ������Ŀ
*/
BU32 APP_Search_API_StartSearch(APP_Search_Param_SP paramP);
/*
������������
*/
void APP_Search_API_EndSearch(void);
/*
�ж�����
*/
void APP_Search_API_HaltSearch(void);
/*
��ʼ����
*/
void APP_Search_API_StartSave(void);
/*
������Ŀ������Ϣ
*/
void APP_Search_API_SearchSortInfo(void);
/*
�����Ƿ��ӡ��������
*/
void APP_Search_API_SetPrintf(BU08 isPrintf);


#endif











