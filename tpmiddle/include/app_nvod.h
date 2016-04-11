#ifndef _APP_NVOD_H_
#define _APP_NVOD_H_

#define		NVOD_MAX_EVENT_NAME_LENGTH  30           /*节目段名*/
#define		NVOD_MAX_EVENT_TEXT_LENGTH  300          /*节目段详细信息*/

#define     INVALID_LINK				-1

#define		NVOD_MAX_NO_OF_PROGRAMS     100 	//与FYF_NVOD_TIME_SHIFTED_SERVICE_NUM相等
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
	STATUS_RUNNING = 0x4,      /* 运行，播出中 */
	STATUS_RUN_SOON = 0x5      /* 即将播放 */      
}APP_NVOD_UNNING_STATUS_E;   /* 业务运行状态 */

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
	
	char	acProgNvodName[NVOD_MAX_EVENT_NAME_LENGTH+1];	     /*节目段名*/
	char   	acProgNvodText[NVOD_MAX_EVENT_TEXT_LENGTH+1];        /*节目段详细信息*/
	APP_Nvod_Prog_TimeInfo_S * pTimeInfo;
	APP_Nvod_Prog_TimeInfo_S Head;
	BU32 update;
	BU08 num;

}APP_Nvod_Program_List_S;

/**********************************************************************************
Desc:    设置和获取时移业务数据库更新的标志
**********************************************************************************/
void APP_Nvod_SetShiftServiceUpdateFlag(const BU32 update_flag);

BU32 APP_Nvod_GetShiftServiceUpdateFlag(void);

/**********************************************************************************
Desc:   参考event有新数据时，更新时移业务数据库中链接事件表的标志
**********************************************************************************/
void APP_Nvod_UpdateAllRefEventUpdateStatus(void);

/**********************************************************************************
Desc:   参考event有新数据时，更新时移业务数据库中链接事件表的标志
**********************************************************************************/
void APP_Nvod_SetRefEventUpdateStatusByIndex(BU32 index,BU32 event_update_flag);

/**********************************************************************************
Desc:   设置参考event数据库中运行标志run_status
**********************************************************************************/
void APP_Nvod_SetRefEventRunStatusByIndex(BU32 index,BU32 EventRunStatus);

void APP_Nvod_SetRefEventRunStatusByReserveIndex(BU32 ReserveIndex,BU32 EventRunStatus);

/**********************************************************************************
Desc:    事件时间与当前系统时间比较,传入时间应为标准时间；
Return:  TRUE: 事件是旧的；    FALSE: 事件是有效的；
**********************************************************************************/
BU32 APP_NVOD_CmpToSystemTime(const BU32 iCodeDate, const BU08 uStartHour, 
                               const BU08 uStartMinute, const BU16 sDuring);

/**********************************************************************************
					获取该事件在事件库中的萦引号
*********************************************************************************/  
BS16 APP_NVOD_GetEventIndex(BU08 ucTableId, BU16 sServideID, BU16 uEventId, BU32 uCodeDate,
                                     BU08 ucHour, BU08 ucMinute, BS32  iEndNum);

/**********************************************************************************
通过事件的索引值清除nvod事件的信息，即释放一个事件空间
**********************************************************************************/
void APP_NVOD_FreeEventNodeByIndex(BS16 sIndex);

/**********************************************************************************
				获取事件库中一个空节点
*********************************************************************************/  
BS16  APP_NVOD_GetFreeEventNode(void);


void APP_Nvod_AddToProgList(APP_Nvod_Prog_TimeInfo_S * pTimeInfo);


/**********************************************************************************
					分析NVOD  的描述值
				1.分析事件名称及详细信息
**********************************************************************************/ 
BU32 APP_NVOD_ParseEpgDescriptor (BU08 *pucSectionData, BS16 sEventIndex, 
                        BU16 service_id, BU16 uEventID, BU16 sDurningTime);

/**********************************************************************************
					分析NVOD 事件信息
**********************************************************************************/ 
void APP_NVOD_StartParserEit(BU08 *cEpgData, BU32 channelId, BU32 filterId);
/**********************************************************************************
Desc:	开启滤取和分析nvod节目信息		
**********************************************************************************/ 
void APP_Nvod_API_GetProgList(void);
/**********************************************************************************
Desc:	开启滤取和分析nvod事件信息				
**********************************************************************************/ 
void APP_Nvod_API_GetEventTimeList(void);
/**********************************************************************************
Desc:	开启滤取和分析nvod节目和事件信息				
**********************************************************************************/ 
void APP_Nvod_API_StartGetNvodProg(void);
/**********************************************************************************
Desc:	停止滤取和分析nvod节目和事件信息				
**********************************************************************************/ 
void APP_Nvod_API_StopGetNvodProg(void);

BU32 APP_Nvod_GetShiftServiceNodeByIndex(BU32 index,APP_Nvod_Program_List_S* shift_time_service);

void APP_Nvod_CheckShiftServiceStatus(void);

BU32 APP_Nvod_GetAllRefEventNumAndIndexByEventId(BU16 event_id);

BU32 APP_Nvod_GetRefEventNodeByIndex(BU32 index,APP_Nvod_Prog_TimeInfo_S* ref_event);

void APP_Nvod_CheckRefEventStatus(void);

void APP_Nvod_InitProgList(void);

/******************************************
Desc:	进入nvod时相关初始化操作
*******************************************/
void APP_Nvod_API_Init(void);

#endif

