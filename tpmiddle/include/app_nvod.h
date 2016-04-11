#ifndef _APP_NVOD_H_
#define _APP_NVOD_H_

#define		NVOD_MAX_EVENT_NAME_LENGTH  30           /*��Ŀ����*/
#define		NVOD_MAX_EVENT_TEXT_LENGTH  300          /*��Ŀ����ϸ��Ϣ*/

#define     INVALID_LINK				-1

#define		NVOD_MAX_NO_OF_PROGRAMS     100 	//��FYF_NVOD_TIME_SHIFTED_SERVICE_NUM���
#define     NVOD_MAX_NO_OF_EVENTS	    1500

#define     SHORT_EVENT_DESC  		    0x4d
#define     EXTENDED_EVENT_DESC         0x4e
#define     TIME_SHIFTED_EVENT_DESC     0x4f

typedef enum
{
	ELEMENT_FREE = 0,
	ELEMENT_OCCUPIED,
	ELEMENT_DELETED,
	ELEMENT_RESERVED
}APP_NVOD_ELEMENT_STATUS_E;

typedef enum
{
	RUNNING_NONE = 0,
	RUNNING_PLAY,
	RUNNING_RESERVED
}APP_NVOD_EVENT_RUNNING_STATUS_E;

typedef enum
{
	STATUS_NODEFINED = 0x0,
	STATUS_RUNNING = 0x4,      /* ���У������� */
	STATUS_RUN_SOON = 0x5      /* �������� */      
}APP_NVOD_UNNING_STATUS_E;   /* ҵ������״̬ */

typedef	struct	_Nvod_Prog_TimeInfo_Table 
{
	unsigned    uProgNvodSeviceID	   : 16;
	unsigned    uProgNvodEventID	   : 16;
	unsigned    uProgRefEventID        : 16;
	unsigned    uProgRefSeviceID       : 16;

	BU32    	uEventDateCode 		   ;
	unsigned    uStartHour 		       : 8;
	unsigned    uStartMinute		   : 8;
	unsigned    uStartSecond		   : 8;
	unsigned	sDuringTime			   : 16;
	unsigned    uDuringSecond		   : 8;
	
	unsigned	uEventStatus           : 2;	
	unsigned 	uEventRunStatus	       : 2;
	unsigned	ubank                  : 2;	
	BS16 		index;
	struct		APP_Nvod_Prog_TimeInfo_S * next;
}APP_Nvod_Prog_TimeInfo_S;

typedef struct   _Nvod_Program_Table
{
    unsigned        uProgSlotStatus             : 4;
    unsigned        uProgRunStatus              : 4;
	//unsigned        uNvodXpdrIndex				: 16;
	unsigned        uProgNvodRefSeviceID		: 16;
	unsigned    	uProgNvodEventID			: 16;
	unsigned	    sDuringTime			        : 16;
	
	char	acProgNvodName[NVOD_MAX_EVENT_NAME_LENGTH+1];	     /*��Ŀ����*/
	char   	acProgNvodText[NVOD_MAX_EVENT_TEXT_LENGTH+1];        /*��Ŀ����ϸ��Ϣ*/
	APP_Nvod_Prog_TimeInfo_S * pTimeInfo;
	APP_Nvod_Prog_TimeInfo_S Head;
	BU32 update;
	BU08 num;

}APP_Nvod_Program_List_S;

/**********************************************************************************
Desc:    ���úͻ�ȡʱ��ҵ�����ݿ���µı�־
**********************************************************************************/
void APP_Nvod_SetShiftServiceUpdateFlag(const BU32 update_flag);

BU32 APP_Nvod_GetShiftServiceUpdateFlag(void);

/**********************************************************************************
Desc:   �ο�event��������ʱ������ʱ��ҵ�����ݿ��������¼���ı�־
**********************************************************************************/
void APP_Nvod_UpdateAllRefEventUpdateStatus(void);

/**********************************************************************************
Desc:   �ο�event��������ʱ������ʱ��ҵ�����ݿ��������¼���ı�־
**********************************************************************************/
void APP_Nvod_SetRefEventUpdateStatusByIndex(BU32 index,BU32 event_update_flag);

/**********************************************************************************
Desc:   ���òο�event���ݿ������б�־run_status
**********************************************************************************/
void APP_Nvod_SetRefEventRunStatusByIndex(BU32 index,BU32 EventRunStatus);

void APP_Nvod_SetRefEventRunStatusByReserveIndex(BU32 ReserveIndex,BU32 EventRunStatus);

/**********************************************************************************
Desc:    �¼�ʱ���뵱ǰϵͳʱ��Ƚ�,����ʱ��ӦΪ��׼ʱ�䣻
Return:  TRUE: �¼��Ǿɵģ�    FALSE: �¼�����Ч�ģ�
**********************************************************************************/
BU32 APP_NVOD_CmpToSystemTime(const BU32 iCodeDate, const BU08 uStartHour, 
                               const BU08 uStartMinute, const BU16 sDuring);

/**********************************************************************************
					��ȡ���¼����¼����е�������
*********************************************************************************/  
BS16 APP_NVOD_GetEventIndex(BU08 ucTableId, BU16 sServideID, BU16 uEventId, BU32 uCodeDate,
                                     BU08 ucHour, BU08 ucMinute, BS32  iEndNum);

/**********************************************************************************
ͨ���¼�������ֵ���nvod�¼�����Ϣ�����ͷ�һ���¼��ռ�
**********************************************************************************/
void APP_NVOD_FreeEventNodeByIndex(BS16 sIndex);

/**********************************************************************************
				��ȡ�¼�����һ���սڵ�
*********************************************************************************/  
BS16  APP_NVOD_GetFreeEventNode(void);


void APP_Nvod_AddToProgList(APP_Nvod_Prog_TimeInfo_S * pTimeInfo);


/**********************************************************************************
					����NVOD  ������ֵ
				1.�����¼����Ƽ���ϸ��Ϣ
**********************************************************************************/ 
BU32 APP_NVOD_ParseEpgDescriptor (BU08 *pucSectionData, BS16 sEventIndex, 
                        BU16 service_id, BU16 uEventID, BU16 sDurningTime);

/**********************************************************************************
					����NVOD �¼���Ϣ
**********************************************************************************/ 
void APP_NVOD_StartParserEit(BU08 *cEpgData, BU32 channelId, BU32 filterId);
/**********************************************************************************
Desc:	������ȡ�ͷ���nvod��Ŀ��Ϣ		
**********************************************************************************/ 
void APP_Nvod_API_GetProgList(void);
/**********************************************************************************
Desc:	������ȡ�ͷ���nvod�¼���Ϣ				
**********************************************************************************/ 
void APP_Nvod_API_GetEventTimeList(void);
/**********************************************************************************
Desc:	������ȡ�ͷ���nvod��Ŀ���¼���Ϣ				
**********************************************************************************/ 
void APP_Nvod_API_StartGetNvodProg(void);
/**********************************************************************************
Desc:	ֹͣ��ȡ�ͷ���nvod��Ŀ���¼���Ϣ				
**********************************************************************************/ 
void APP_Nvod_API_StopGetNvodProg(void);

BU32 APP_Nvod_GetShiftServiceNodeByIndex(BU32 index,APP_Nvod_Program_List_S* shift_time_service);

void APP_Nvod_CheckShiftServiceStatus(void);

BU32 APP_Nvod_GetAllRefEventNumAndIndexByEventId(BU16 event_id);

BU32 APP_Nvod_GetRefEventNodeByIndex(BU32 index,APP_Nvod_Prog_TimeInfo_S* ref_event);

void APP_Nvod_CheckRefEventStatus(void);

void APP_Nvod_InitProgList(void);

/******************************************
Desc:	����nvodʱ��س�ʼ������
*******************************************/
void APP_Nvod_API_Init(void);

#endif

