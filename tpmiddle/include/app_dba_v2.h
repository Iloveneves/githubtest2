#ifndef _APP_DBA_V2_H_
#define _APP_DBA_V2_H_


/*数据库存放EPG数量*/
#define EPG_MEMORY_COUNT 5000
/*保存EPG详细信息的数量*/
#define DETAIL_COUNT 150

/*保存过期EPG*/
//#define APP_EPG_SAVE_PASSED_EPG

#define APP_NIT_FREQ_COUNT 100
struct nitFreqInfo_s
{
	BU32 freq;
	BU32 sym;
	BU08 qam;
};

#define APP_PMT_PID_COUNT 32
struct pmtPIDInfo_s
{
	BU32 SID;
	BU32 PID;
	BU32 TSID;
};

#define APP_SDT_INFO_COUNT 32
struct sdtInfo_s
{
	BU32 serId;
	BU08 progName[APP_CHN_NAME_LEN];
	BU32 serType;
	BU16 OriNetId;
	BU16 reserver;
};
#define BROADCAST_COUNT 30
#define APP_DETAIL_LEN 201
struct epgDetail_s
{
	BU32 progId;
	BU32 enventId;
	BU08 week;
	BU08 strDetail[APP_DETAIL_LEN];
};

struct EpgReserveNode_s
{
	BU08 isUsed;
	Epg_Info_S epg_Info;
	struct EpgReserveNode_s *next;
};

struct epgInfo_s 
{
	BU32 eventId;
	BU08 strEpgInfo[APP_EPG_NAME_LEN];
	BU08 week;
	BU32 day;
	BU32 startTime;
	BU32 endTime;
	BU08 reserve;
	BU08 hasDetail;
	//......
};

struct epgInfoNode_s 
{
	BU08 isUsed;
	struct epgInfo_s epgInfo;

	struct epgInfoNode_s *next;
};

struct progInfo_s 
{
	BU32 serviceId;
	struct epgInfoNode_s *pCurEpgNode;
	struct epgInfoNode_s *pNextEpgNode;
	//......

	struct epgInfoNode_s *arrEpgInfoNode[7];
};

struct progInfoNode_s 
{
	BU08 isUsed;
	struct progInfo_s progInfo;
	struct progInfoNode_s *next;
	struct progInfoNode_s *prior;
};


BU08 DBA_API_AddProg(struct progInfo_s progInfo);
BU08 DBA_API_AddEpg(BU32 progId, BU08 week, struct epgInfo_s *pEpgInfo);
struct epgInfoNode_s* DBA_API_GetEpgNode(BU32 progId, BU08 week);
struct progInfoNode_s *DBA_API_GetProgInfoNode(BU32 progId, BU32 isChangeProNodeP);
BU08 DBA_API_ClearProg(void);

BU08 DBA_API_AddDetail(EIT_Content_s *pEitContent);
BU08 DBA_API_GetDetail(BU32 progId, BU32 eventId, BU08 *strDetail, BU32 strLen);

BU08 DBA_API_AddNitFreqInfo(BU32 freq, BU32 sym, BU08 qam);
void DBA_API_SortNitFreqInfo(void);
void DBA_API_GetNitFreqInfo(BU32 index, struct nitFreqInfo_s *pNitFreqInfo);
void DBA_API_ClearNitFreqInfo(void);
BU32 DBA_API_GetNitFreqInfoCount(void);

void DBA_API_CreateBroadCastList();
BU08 DBA_API_AddPmtPidInfo(BU32 SID, BU32 PID, BU32 TSID);
void DBA_API_ClearPmtPidInfo(void);
BU32 DBA_API_GetPmtPidInfoCount(void);
void DBA_API_GetPmtPidInfoBySID(BU32 SID, struct pmtPIDInfo_s *pPmtPidInfo);
void DBA_API_GetPmtPidInfo(BU32 index, struct pmtPIDInfo_s *pPmtPidInfo);

void DBA_API_GetSdtInfoBySerId(BU32 serId, struct sdtInfo_s *pSdtInfo);
BU08 DBA_API_AddSdtInfo(BU32 serId, BU08 *progName, BU08 serType, BU16 oriNetId);
void DBA_API_ClearSdtInfo(void);
/*-------------------------------------------------------------------------------
	Description:获得数据广播的总数
	Parameters:
-------------------------------------------------------------------------------*/
BU32 APP_DBA_API_GetBroadCastNum(void);
/*-------------------------------------------------------------------------------
	Description:得到当前数据广播的信息
	Parameters:
-------------------------------------------------------------------------------*/
BU32 APP_DBA_API_GetBroadcastInfo(BU32 brdcastNo, BroadCast_ChnInfo_S* brdcastInfo);

#endif

