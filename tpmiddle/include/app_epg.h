#ifndef _APP_EPG_H
#define _APP_EPG_H

/*һ��������EPG����*/
#define APP_EPG_MAX_DAY_COUNT 200
/**************************************************

***************************************************/
//#define  APP_GetCurEventNumFail  0  //��õ�ǰ�¼���ʧ��

#define GetHour(time) ((time) / 60)
#define GetMinute(time) ((time) % 60)
#define GetTime(hour, minute) (BU32)((hour)*60 + (minute))

#define APP_EPG_NAME_LEN     41//�¼���Ϣ��󳤶�
#define MAX_RESERVE_NUM      25		//���ԤԼ����



/**************************************************
					�ṹ�嶨��

***************************************************/
typedef struct _Epg_Info_S
{
	BU32 serviceId;
	BU32 eventId;

	BU08 chnName[APP_CHN_NAME_LEN];
	BU08 epgName[APP_EPG_NAME_LEN];
	BU08 week;
	BU32 day;
	BU08 startHour;
	BU08 startMin;
	BU08 endHour;
	BU08 endMin;

	BU08 playState;
	BU08 reserveState;
	BU08 detailState; 
	BU08 style;
}Epg_Info_S;
//�ֶ�ѡʱ¼�Ʋ����ṹ��
typedef struct _APP_ManualRecData_S
{
	BU32 serviceId;
	BU32 day;
	BU08 week;
	BU08 startHour;
	BU08 startMin;
	BU08 endHour;
	BU08 endMin;
	BU08 reserveState;
	BU08 chnName[APP_CHN_NAME_LEN];
}APP_ManualRecData_S;
//NvodԤԼ�ṹ��
typedef struct _APP_NvodReserve_S
{
	BU32 serviceId;
	BU32 eventId;
	BU32 day;
	BU08 week;
	BU08 startHour;
	BU08 startMin;
	BU08 endHour;
	BU08 endMin;
	BU08 reserveState;
	BU08 eventName[APP_EPG_NAME_LEN];
}APP_NvodReserve_S;

/**************************************************
				ö�����Ͷ���

***************************************************/
typedef enum
{
	APP_EPG_PLAYSTATE_PLAYED_e,          //�¼��Ѳ���
	APP_EPG_PLAYSTATE_PLAYING_e,         //�¼����ڲ���
	APP_EPG_PLAYSTATE_UNPLAY_e      //�¼���û��
	
}APP_EpgPlayState_e;

typedef enum
{
	APP_ReserveType_All_e = 0,    //��������
	APP_ReserveType_Epg_e = 1,    //EpgԤԼ��Ԥ¼
	APP_ReserveType_Rec_e = 2,    //EpgԤ¼��ѡʱԤ¼
	APP_ReserveType_EpgReserve_e = 3, //EpgԤԼ
	APP_ReserveType_Nvod_e = 4,           //NvodԤԼ
	APP_ReserveType_EpgRec_e = 5,     //EpgԤ¼
	APP_ReserveType_ManualRec_e = 6,   //ѡʱԤ¼
}APP_ReserveType_e;

/*
typedef enum
{
	APP_EPG_RESERVESTATE_UNRESERVED_e,       //�¼�û�б�ԤԼ
	APP_EPG_RESERVESTATE_RESERVED_e          //�¼�ԤԼ״̬	
}APP_EpgReserveState_e;
*/
typedef void(* ReserveCallBackFun)(BU32 callback_type, Epg_Info_S*);

typedef struct _ReservedList
{
	int serviceId;
	int eventId;
	BU08 reserveState;
}ReservedList;
/*
�����Ƿ�ֹͣԤԼ��ʾ
*/
void APP_EPG_API_SetRemindReserve(BU08 state);
/*
ֹͣ����EPG��Ϣ,���øú��������ʵ�������Զ���ʼ����EPG
*/
void APP_EPG_API_StopGetEpg(void);
/*
��ͣ����EPG��Ϣ,Ҫ�ٴο�������EPG ��Ϣ(void APP_EPG_API_PauseGetEpg(void)).
��ͣ�����˲�����Ҳ����ͣ,�����ͣ���޷�������̨�������˲�����.
*/
void APP_EPG_API_PauseGetEpg(void);
/*
����¼���ϸ��Ϣ
*/
BU32 APP_EPG_API_GetEventDetailText(BU32 EventNum, BU08 *DestStr, BU32 textLength);
/*
����EPG��Ϣ�б�,ͬʱ�õ���Ϣ����,�͵�ǰ�¼���λ��
*/
BU32 APP_EPG_API_GetEpgCountAndIndex(BU32 ChnNo, BU08 DayOffset, BU32 *Count, BU32 *Index);
/*
�õ�EPG��Ϣ,��Ҫ������Ϣ�б�(APP_EPG_API_GetEpgCountAndIndex)
*/
BU32 APP_EPG_API_GetEpgInfo(BU08 DayOffset, BU32 Index, BU32 Num, Epg_Info_S *EpgInfo);
/*
ȡ�ò�ͬԤ¼/ԤԼ�����¼�����
*/
BU32 APP_EPG_API_GetReserveTypeAmount(APP_ReserveType_e type);
/*
���ԤԼ��Ŀ����
*/
BU32 APP_EPG_API_GetReserveAmount(void);
/*
��õ�ǰ��һ�¼���Ϣ
*/
BU32 APP_EPG_API_GetCurAndNextEvent(BU32 ChnNo, Epg_Info_S *CurEpgInfo, Epg_Info_S *NextEpgInfo);
/*
����NvodԤԼ
*/
void APP_EPG_API_SetNvodReserveState(APP_NvodReserve_S *NvodReserve,BU08 ReserveState);
/*
�����ֶ�ѡʱ¼��
*/
void APP_EPG_API_SetManualRecState(APP_ManualRecData_S *RecData);
/*
�����¼�ԤԼ/Ԥ¼
*/
void APP_EPG_API_SetEvntReserveState(BU32 EventNo, BU08 dayOffset, BU08 ReserveState);
/*
ȡ�ò�ͬԤ¼/ԤԼ�����¼�����
*/
void APP_EPG_API_GetReserveTypeInfo(BU32 Index, BU32 Num, APP_ReserveType_e type,Epg_Info_S* EventInfo);
/*
ȡ��ԤԼ�¼�����
*/
void APP_EPG_API_GetReservedInfo(BU32 Index, BU32 Num, Epg_Info_S *EventInfo);
/*
ȡ����ͬԤ¼/ԤԼ����
*/
BU32 APP_EPG_API_CancelReserveType(BU32 CancelIndex,APP_ReserveType_e type);
/*
ȡ��ԤԼ
*/
BU32 APP_EPG_API_CancelReserve(BU32 CancelIndex);
/*
����ԤԼ����
*/
BU32 APP_EPG_API_UpDataToFlash(void);
/*
����ԤԼ��ʱ�ص�
*/
void APP_EPG_API_SetReserveCallBack(ReserveCallBackFun CallBack);
/*
Check whether this event has been reserved
*/
BU32 APP_EPG_API_ifReserved(Epg_Info_S* EventInfo);

/********************************************************
	����:���ԤԼ�б����Ƿ����뵱ǰnvodԤԼ��ͻ��
		 �еĻ�����ԤԼ�б��е����
*********************************************************/
BU32 APP_EPG_API_GetNvodConflictIndex(BU32 *conflictIndex,APP_NvodReserve_S* NvodReserve);

/*-------------------------------------------------------------------------------

	Description:ȡ��ԤԼ�¼���serviceId eventId,���ڱȽ��Ƿ�ԤԼ
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_IPI_GetReservedShortInfo(void);



#endif















