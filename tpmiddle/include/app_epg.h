#ifndef _APP_EPG_H
#define _APP_EPG_H

/*一天中最大的EPG数量*/
#define APP_EPG_MAX_DAY_COUNT 200
/**************************************************

***************************************************/
//#define  APP_GetCurEventNumFail  0  //获得当前事件号失败

#define GetHour(time) ((time) / 60)
#define GetMinute(time) ((time) % 60)
#define GetTime(hour, minute) (BU32)((hour)*60 + (minute))

#define APP_EPG_NAME_LEN     41//事件信息最大长度
#define MAX_RESERVE_NUM      25		//最大预约个数



/**************************************************
					结构体定义

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
//手动选时录制参数结构体
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
//Nvod预约结构体
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
				枚举类型定义

***************************************************/
typedef enum
{
	APP_EPG_PLAYSTATE_PLAYED_e,          //事件已播过
	APP_EPG_PLAYSTATE_PLAYING_e,         //事件正在播放
	APP_EPG_PLAYSTATE_UNPLAY_e      //事件还没播
	
}APP_EpgPlayState_e;

typedef enum
{
	APP_ReserveType_All_e = 0,    //所有类型
	APP_ReserveType_Epg_e = 1,    //Epg预约和预录
	APP_ReserveType_Rec_e = 2,    //Epg预录和选时预录
	APP_ReserveType_EpgReserve_e = 3, //Epg预约
	APP_ReserveType_Nvod_e = 4,           //Nvod预约
	APP_ReserveType_EpgRec_e = 5,     //Epg预录
	APP_ReserveType_ManualRec_e = 6,   //选时预录
}APP_ReserveType_e;

/*
typedef enum
{
	APP_EPG_RESERVESTATE_UNRESERVED_e,       //事件没有被预约
	APP_EPG_RESERVESTATE_RESERVED_e          //事件预约状态	
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
设置是否停止预约提示
*/
void APP_EPG_API_SetRemindReserve(BU08 state);
/*
停止接收EPG信息,调用该函数后在适当情况会自动开始接收EPG
*/
void APP_EPG_API_StopGetEpg(void);
/*
暂停接收EPG信息,要再次开启接收EPG 信息(void APP_EPG_API_PauseGetEpg(void)).
暂停后连滤波函数也会暂停,因此暂停后无法进行搜台等其他滤波操作.
*/
void APP_EPG_API_PauseGetEpg(void);
/*
获得事件详细信息
*/
BU32 APP_EPG_API_GetEventDetailText(BU32 EventNum, BU08 *DestStr, BU32 textLength);
/*
创建EPG信息列表,同时得到信息总数,和当前事件的位置
*/
BU32 APP_EPG_API_GetEpgCountAndIndex(BU32 ChnNo, BU08 DayOffset, BU32 *Count, BU32 *Index);
/*
得到EPG信息,先要创建信息列表(APP_EPG_API_GetEpgCountAndIndex)
*/
BU32 APP_EPG_API_GetEpgInfo(BU08 DayOffset, BU32 Index, BU32 Num, Epg_Info_S *EpgInfo);
/*
取得不同预录/预约类型事件总数
*/
BU32 APP_EPG_API_GetReserveTypeAmount(APP_ReserveType_e type);
/*
获得预约节目总数
*/
BU32 APP_EPG_API_GetReserveAmount(void);
/*
获得当前下一事件信息
*/
BU32 APP_EPG_API_GetCurAndNextEvent(BU32 ChnNo, Epg_Info_S *CurEpgInfo, Epg_Info_S *NextEpgInfo);
/*
设置Nvod预约
*/
void APP_EPG_API_SetNvodReserveState(APP_NvodReserve_S *NvodReserve,BU08 ReserveState);
/*
设置手动选时录制
*/
void APP_EPG_API_SetManualRecState(APP_ManualRecData_S *RecData);
/*
设置事件预约/预录
*/
void APP_EPG_API_SetEvntReserveState(BU32 EventNo, BU08 dayOffset, BU08 ReserveState);
/*
取得不同预录/预约类型事件内容
*/
void APP_EPG_API_GetReserveTypeInfo(BU32 Index, BU32 Num, APP_ReserveType_e type,Epg_Info_S* EventInfo);
/*
取得预约事件内容
*/
void APP_EPG_API_GetReservedInfo(BU32 Index, BU32 Num, Epg_Info_S *EventInfo);
/*
取消不同预录/预约类型
*/
BU32 APP_EPG_API_CancelReserveType(BU32 CancelIndex,APP_ReserveType_e type);
/*
取消预约
*/
BU32 APP_EPG_API_CancelReserve(BU32 CancelIndex);
/*
保存预约更改
*/
BU32 APP_EPG_API_UpDataToFlash(void);
/*
设置预约到时回调
*/
void APP_EPG_API_SetReserveCallBack(ReserveCallBackFun CallBack);
/*
Check whether this event has been reserved
*/
BU32 APP_EPG_API_ifReserved(Epg_Info_S* EventInfo);

/********************************************************
	描述:检测预约列表中是否有与当前nvod预约冲突，
		 有的话返回预约列表中的序号
*********************************************************/
BU32 APP_EPG_API_GetNvodConflictIndex(BU32 *conflictIndex,APP_NvodReserve_S* NvodReserve);

/*-------------------------------------------------------------------------------

	Description:取得预约事件的serviceId eventId,用于比较是否被预约
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_IPI_GetReservedShortInfo(void);



#endif















