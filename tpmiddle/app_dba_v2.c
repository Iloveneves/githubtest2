#include "app_common.h"
#include "app_ipi_common.h"

#include <stdlib.h>
#include <string.h>

static struct progInfoNode_s* g_pFirstProgInfoNode = 0;
static struct progInfoNode_s* g_pCurProgInfoNode = 0;
static struct epgInfoNode_s	  g_ArrEpgInfoNode[EPG_MEMORY_COUNT];//总的EPG结点数量,总的大小5000个
static BU32 epgInfoNodeRemain = EPG_MEMORY_COUNT;				//剩余可以使用的EPG节点数量
static struct epgInfoNode_s*  g_ArrpEpgInfoNode[EPG_MEMORY_COUNT];//记录未使用的EPG节点
static struct epgDetail_s 	  epgDetail[DETAIL_COUNT];
static BU32 nitFreqCount = 0;
static struct nitFreqInfo_s   arrNitFreqInfo[APP_NIT_FREQ_COUNT];
static BU32 pmtPIDCount = 0;
static struct pmtPIDInfo_s   arrPmtPIDInfo[APP_PMT_PID_COUNT];
static BU32 sdtInfoCount = 0;
static struct sdtInfo_s   arrSdtInfo[APP_SDT_INFO_COUNT];

BroadCast_ChnInfo_S BroadProg[BROADCAST_COUNT];
static BU32 broadcastcount = 0;

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];

extern BU32 semaphoreEPG;

extern BU08 g_Week;
extern BU32 g_DayOffset;//记录上次查询的星期

void _Assert(char* strFile, char* strFunction, unsigned uLine)
{
	BU32	lastPos = 0;
	BU32	i = 0;
	while (*(strFile + i) != '\0')
	{
		if (*(strFile + i) == '/')
		{
			lastPos = i;
		}
		++i;
	}
	FYF_API_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>Assertion: %s, %s, %u<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",
		strFile + lastPos, strFunction, uLine);
}

BU32 APP_EPG_IPI_Semaphore_Wait(BU32 time)
{
	BU32 rv = 0;
	
	rv = FYF_API_semaphore_wait(semaphoreEPG, time);//FYF_WAIT_FOREVER);
	if(rv == FYF_OS_QUEUE_SEM_STATUS_AVAILABLE)
	{
		return APP_SUCCESS;
	}
	else
	{
		_Assert(__FILE__, __FUNCTION__, __LINE__);
		return APP_FAIL;
	}
}

void APP_EPG_IPI_Semaphore_Release(void)
{
	FYF_API_semaphore_release(semaphoreEPG);
}

void DBA_IPI_InitEpgInfoNode(void)
{
	BU32 i = 0;

	memset(g_ArrEpgInfoNode, 0, sizeof(struct epgInfoNode_s) * EPG_MEMORY_COUNT);
	for(i = 0; i < EPG_MEMORY_COUNT; i++)
	{
		g_ArrpEpgInfoNode[i] = &g_ArrEpgInfoNode[i];
	}
	epgInfoNodeRemain = EPG_MEMORY_COUNT;
}
/*-------------------------------------------------------------------------------

	Description:回收EPG节点到EPG指针数据,修改资源剩余总数.不释放内存
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU08 DBA_IPI_FreeEpgInfoNode(struct epgInfoNode_s* pEpgInfoNode)
{
	if(pEpgInfoNode == 0)
	{
		return APP_FAIL;
	}
	else
	{
		memset(pEpgInfoNode, 0, sizeof(struct epgInfoNode_s));
		pEpgInfoNode->isUsed = 0;
		g_ArrpEpgInfoNode[epgInfoNodeRemain] = pEpgInfoNode;
		epgInfoNodeRemain++;
	}
	return APP_SUCCESS;
}
/*-------------------------------------------------------------------------------

	Description:删除一节目节点的一周内EPG事件
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 DBA_IPI_DelDayEpg(struct progInfoNode_s* pProgInfoNode)
{
	BU32					delEpgCount = 0;
	struct epgInfoNode_s*	pEpgInfoNode = 0;
	struct epgInfoNode_s*	pEpgInfoNodeNext = 0;
	int						i = 0;
	BU32 testI = 0;

	if (pProgInfoNode == 0)
	{
		return APP_FAIL;
	}

	for (i = 0; i < 7; i++)
	{
		pEpgInfoNode = pProgInfoNode->progInfo.arrEpgInfoNode[i];
		pProgInfoNode->progInfo.arrEpgInfoNode[i] = 0;
		testI = 0;
		while (pEpgInfoNode != 0)
		{
			pEpgInfoNodeNext = pEpgInfoNode->next;
			DBA_IPI_FreeEpgInfoNode(pEpgInfoNode);
			++delEpgCount;
			pEpgInfoNode = pEpgInfoNodeNext;
			if(++testI >= 1000)
				return APP_FAIL;
		}
	}
	return delEpgCount;
}
/*-------------------------------------------------------------------------------

	Description:删除一节目所有的EPG事件.
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 DBA_IPI_DelEpg(struct progInfoNode_s* pProgInfoNode)
{
	struct epgInfoNode_s*	pEpgInfoNode = 0;
	struct epgInfoNode_s*	pEpgInfoNodeNext = 0;
	int						i = 0;


	if (pProgInfoNode == 0)
	{
		return APP_FAIL;
	}

	/*下面所有的操作都只做指针置0处理,因为具体的EPG数据都存放于EPG节点源g_ArrEpgInfoNode[5000]中,
	  而DBA_IPI_InitEpgInfoNode()会将所有EPG数据清零*/
	
	if (pProgInfoNode->progInfo.pCurEpgNode != 0)
	{
		pProgInfoNode->progInfo.pCurEpgNode = 0;
	}
	if (pProgInfoNode->progInfo.pNextEpgNode != 0)
	{
		pProgInfoNode->progInfo.pNextEpgNode = 0;
	}
	
	for (i = 0; i < 7; i++)
	{
		pProgInfoNode->progInfo.arrEpgInfoNode[i] = 0;
	}
	return APP_SUCCESS;
}
/*-------------------------------------------------------------------------------

	Description:
	
	Parameters:
	
-------------------------------------------------------------------------------*/
struct epgInfoNode_s* DBA_IPI_GetOneEpgNode()
{

	if(epgInfoNodeRemain != 0)
	{
		epgInfoNodeRemain--;
		memset(g_ArrpEpgInfoNode[epgInfoNodeRemain], 0, sizeof(struct epgInfoNode_s));
		g_ArrpEpgInfoNode[epgInfoNodeRemain]->isUsed = 1;
		return (g_ArrpEpgInfoNode[epgInfoNodeRemain]);
	}
	return NULL;
}

/*-------------------------------------------------------------------------------

	Description:从EPG节点资源中分配EPG结点
	
	Parameters:
	
-------------------------------------------------------------------------------*/

struct epgInfoNode_s* DBA_IPI_MallocEpgInfoNode(struct progInfoNode_s* pProtectProg)
{
	static BU32				oldPosition = 0;
	struct progInfoNode_s*	pPorgInfoNode = 0;
	struct epgInfoNode_s*   pEpgNodeTmp = NULL;
	BU32					i = 0;
	BU08					direction = 1; //决定向前找还是向后找
	BU32 testI = 0;

	pEpgNodeTmp = DBA_IPI_GetOneEpgNode();
	if(pEpgNodeTmp)
	{
		return pEpgNodeTmp;
	}

	/*EPG资源无剩余,从当前节目开始,寻找一个有EPG的节目,删除其一周内的EPG*/
	for (direction = 0; direction != 2; direction++)
	{
		pPorgInfoNode = g_pCurProgInfoNode;		
		for (i = 0; i < 7; i++)
		{
			if (pPorgInfoNode != 0 && pPorgInfoNode->prior != 0 && pPorgInfoNode->isUsed == 1)
			{
				pPorgInfoNode = (direction == 1) ? pPorgInfoNode->next : pPorgInfoNode->prior;
			}
			else
			{
				break;
			}
		}

		while (pPorgInfoNode != 0 && pPorgInfoNode->prior != 0 && pPorgInfoNode->isUsed == 1)
		{
			if (pProtectProg != pPorgInfoNode && DBA_IPI_DelDayEpg(pPorgInfoNode) != APP_FAIL)
			{
				return DBA_IPI_GetOneEpgNode();
			}
			pPorgInfoNode = (direction == 1) ? pPorgInfoNode->next : pPorgInfoNode->prior;
			
			if(++testI >= 1000)
				return APP_FAIL;
		}
	}

	FYF_API_printf("epg memory not enough!!!!!!!!!!!!!!!!!!!!!\n");
	return APP_FAIL;
}


/*
根据serviceId得到节目节点
并根据标志判断是否改变全局当前节目位置
*/
struct progInfoNode_s* DBA_API_GetProgInfoNode(BU32 progId, BU32 isChangeProNodeP)
{
	BU08					isSearch = 0;
	struct progInfoNode_s*	p_ProgInfoNodeNext = 0;
	struct progInfoNode_s*	p_ProgInfoNodePrior = 0;
	struct progInfoNode_s*	p_ProgInfoNodeReturn = 0;
	BU32 testI = 0;

		
	p_ProgInfoNodeNext = g_pCurProgInfoNode;
	p_ProgInfoNodePrior = g_pCurProgInfoNode;
	while ((p_ProgInfoNodeNext != 0 && p_ProgInfoNodeNext->isUsed == 1)
		|| (p_ProgInfoNodePrior != 0 && p_ProgInfoNodePrior->isUsed == 1))
	{
		if (p_ProgInfoNodeNext != 0 && p_ProgInfoNodeNext->isUsed == 1)
		{
			if (p_ProgInfoNodeNext->progInfo.serviceId == progId)
			{
				p_ProgInfoNodeReturn = p_ProgInfoNodeNext;
				isSearch = 1;
				break;
			}
			p_ProgInfoNodeNext = p_ProgInfoNodeNext->next;
		}

		if (p_ProgInfoNodePrior != 0 && p_ProgInfoNodePrior->isUsed == 1)
		{
			if (p_ProgInfoNodePrior->progInfo.serviceId == progId)
			{
				p_ProgInfoNodeReturn = p_ProgInfoNodePrior;
				isSearch = 1;
				break;
			}
			p_ProgInfoNodePrior = p_ProgInfoNodePrior->prior;
		}

		if(++testI >= 1000)
			return APP_FAIL;
	}

	if (isSearch == 1)
	{
		if (isChangeProNodeP != 0)
		{
			g_pCurProgInfoNode = p_ProgInfoNodeReturn;
		}
		return (p_ProgInfoNodeReturn);
	}
	else
	{
		return APP_FAIL;
	}
}

BU32 DBA_IPI_GetProgJoinServiceId(BU32 serviceId)
{
	BU08					isSearch = 0;
	struct progInfoNode_s*	p_ProgInfoNodeNext = 0;
	struct progInfoNode_s*	p_ProgInfoNodePrior = 0;
	struct progInfoNode_s*	p_ProgInfoNodeReturn = 0;
	BU32 testI = 0;

		
	p_ProgInfoNodeNext = g_pCurProgInfoNode;
	p_ProgInfoNodePrior = g_pCurProgInfoNode;
	while ((p_ProgInfoNodeNext != 0 && p_ProgInfoNodeNext->isUsed == 1)
		|| (p_ProgInfoNodePrior != 0 && p_ProgInfoNodePrior->isUsed == 1))
	{
		if (p_ProgInfoNodeNext != 0 && p_ProgInfoNodeNext->isUsed == 1)
		{
			if (SeparateSevId(p_ProgInfoNodeNext->progInfo.serviceId) == serviceId)
			{
				p_ProgInfoNodeReturn = p_ProgInfoNodeNext;
				isSearch = 1;
				break;
			}
			p_ProgInfoNodeNext = p_ProgInfoNodeNext->next;
		}

		if (p_ProgInfoNodePrior != 0 && p_ProgInfoNodePrior->isUsed == 1)
		{
			if (SeparateSevId(p_ProgInfoNodePrior->progInfo.serviceId) == serviceId)
			{
				p_ProgInfoNodeReturn = p_ProgInfoNodePrior;
				isSearch = 1;
				break;
			}
			p_ProgInfoNodePrior = p_ProgInfoNodePrior->prior;
		}
//		ASSERT_WITH_RET((testI++) < 1000);
	}

	if (isSearch == 1)
	{
		g_pCurProgInfoNode = p_ProgInfoNodeReturn;
		return p_ProgInfoNodeReturn->progInfo.serviceId;
	}
	else
	{
		return APP_FAIL;
	}
}
/*-------------------------------------------------------------------------------

	Description:清空节目列表以及epg信息
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU08 DBA_API_ClearProg(void)
{
	struct progInfoNode_s*	pProgInfoNode = 0;
	BU32 i = 0;
	

	if(g_pFirstProgInfoNode != 0)
	{
		pProgInfoNode = g_pFirstProgInfoNode->next;
	}
	else
	{
		return APP_SUCCESS;
	}
	
	while (pProgInfoNode != 0 && pProgInfoNode->isUsed == 1)
	{
		if(APP_FAIL == DBA_IPI_DelEpg(pProgInfoNode))
			return APP_FAIL;
			
		pProgInfoNode->isUsed = 0;
		pProgInfoNode = pProgInfoNode->next;
	}

	g_pCurProgInfoNode = g_pFirstProgInfoNode;
	return APP_SUCCESS;
}

BU08 DBA_API_AddProg(struct progInfo_s progInfo)
{
	struct progInfoNode_s*	p_ProgInfoNode = 0;
	BU08					i = 0; 
	
	/*
	初始化
	*/
	if (g_pFirstProgInfoNode == 0)
	{
		g_pFirstProgInfoNode = (struct progInfoNode_s *)FYF_API_malloc(sizeof(struct progInfoNode_s));
		if (g_pFirstProgInfoNode == 0)
		{
			return APP_FAIL;
		}
		memset(g_pFirstProgInfoNode, 0, sizeof(struct progInfoNode_s));
		g_pFirstProgInfoNode->isUsed = 1;
		
		g_pCurProgInfoNode = g_pFirstProgInfoNode;
	}

	while (g_pCurProgInfoNode->next != 0 && g_pCurProgInfoNode->next->isUsed == 1)/*指向尾部*/
	{
		g_pCurProgInfoNode = g_pCurProgInfoNode->next;
	}

	/*
	加入一个节点
	*/
	if(g_pCurProgInfoNode->next != 0)
	{
		memcpy(&(g_pCurProgInfoNode->next->progInfo), &progInfo, sizeof(struct progInfo_s));	
		g_pCurProgInfoNode->next->isUsed = 1;
	}
	else
	{
		p_ProgInfoNode = (struct progInfoNode_s *) FYF_API_malloc(sizeof(struct progInfoNode_s));
		if (p_ProgInfoNode == 0)
		{
			return APP_FAIL;
		}
		memcpy(&(p_ProgInfoNode->progInfo), &progInfo, sizeof(struct progInfo_s));
		g_pCurProgInfoNode->next = p_ProgInfoNode;
		p_ProgInfoNode->next = 0;
		p_ProgInfoNode->prior = g_pCurProgInfoNode;
		p_ProgInfoNode->isUsed = 1;
		g_pCurProgInfoNode = p_ProgInfoNode;
	}
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:根据progid添加该节目当前/下一事件
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU08 DBA_API_AddCurNextEpg(BU32 progId, struct epgInfo_s *pEpgInfo, BU08 epgType)
{
	struct progInfoNode_s*	p_ProgInfoNode = 0;
	
	p_ProgInfoNode = DBA_API_GetProgInfoNode(progId, 0);
	if (p_ProgInfoNode == 0)
	{
		return APP_FAIL;
	}

	if( pEpgInfo == 0)
		return APP_FAIL;

	if (p_ProgInfoNode->progInfo.pCurEpgNode == 0)
	{
		p_ProgInfoNode->progInfo.pCurEpgNode = DBA_IPI_MallocEpgInfoNode(p_ProgInfoNode);		
		if (0 == (p_ProgInfoNode->progInfo.pCurEpgNode))
		{
			return APP_FAIL;
		}
	}
	if (p_ProgInfoNode->progInfo.pNextEpgNode == 0)
	{
		p_ProgInfoNode->progInfo.pNextEpgNode = DBA_IPI_MallocEpgInfoNode(p_ProgInfoNode);	
		if (0 == (p_ProgInfoNode->progInfo.pNextEpgNode))
		{
			return APP_FAIL;
		}
	}

	if(epgType == 0 && pEpgInfo->startTime != p_ProgInfoNode->progInfo.pCurEpgNode->epgInfo.startTime)
	{
		memcpy(&(p_ProgInfoNode->progInfo.pCurEpgNode->epgInfo), pEpgInfo, sizeof(struct epgInfo_s));
		return APP_SUCCESS;
	}
	else if(epgType == 1 && pEpgInfo->startTime != p_ProgInfoNode->progInfo.pNextEpgNode->epgInfo.startTime)
	{
		memcpy(&(p_ProgInfoNode->progInfo.pNextEpgNode->epgInfo), pEpgInfo, sizeof(struct epgInfo_s));
		return APP_SUCCESS;
	}
	return APP_FAIL;
}

BU08 DBA_API_AddEpg(BU32 progId, BU08 week, struct epgInfo_s *pEpgInfo)
{
	struct epgInfoNode_s*	p_EpgInfoNode = 0;
	struct epgInfoNode_s*	p_EpgInfoNodeNew = 0;
	struct progInfoNode_s*	p_ProgInfoNode = 0;
	BU32 testI = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	BU08 epgInfoTemp[APP_EPG_NAME_LEN];

	
	#ifndef APP_EPG_SAVE_PASSED_EPG
	if (pEpgInfo->day < APP_IPI_GetDay() - 1)
	{
		return APP_FAIL;
	}
	else if (pEpgInfo->day == APP_IPI_GetDay() - 1 && pEpgInfo->endTime > pEpgInfo->startTime)
	{
		return APP_FAIL;
	}		
	else if (pEpgInfo->day == APP_IPI_GetDay() - 1 && pEpgInfo->endTime < pEpgInfo->startTime && pEpgInfo->endTime < APP_IPI_GetTime())
	{
		return APP_FAIL;
	}
	else if(pEpgInfo->day == APP_IPI_GetDay() && pEpgInfo->startTime < pEpgInfo->endTime 
			 && pEpgInfo->endTime < APP_IPI_GetTime())
	{
		return APP_FAIL;
	}
	#endif

	week = week - 1;
	if (week > 6)
	{
		return APP_FAIL;
	}

	if(pEpgInfo == 0)
		return APP_FAIL;

	/*
	找到节目位置
	*/
	p_ProgInfoNode = DBA_API_GetProgInfoNode(progId, 0);
	if (p_ProgInfoNode == 0)
	{
		return APP_FAIL;
	}
	/*
	若节目存在，插入EPG
	*/
	if( pEpgInfo->day == APP_IPI_GetDay()-1 && pEpgInfo->endTime < pEpgInfo->startTime) /*今天第一条是跨天*/
	{
		strcpy(epgInfoTemp, pEpgInfo->strEpgInfo);
		epgInfoTemp[APP_EPG_NAME_LEN - 10] = '\0';
		sprintf(pEpgInfo->strEpgInfo, "%s (跨天)", epgInfoTemp);
		goto epg_to_next_day;
	}
    /*找到当天的epg事件链表*/
	p_EpgInfoNode = p_ProgInfoNode->progInfo.arrEpgInfoNode[week];
	if (p_EpgInfoNode == 0)
	{
		p_EpgInfoNode = DBA_IPI_MallocEpgInfoNode(p_ProgInfoNode);	
		if (p_EpgInfoNode == 0)
		{
			return APP_FAIL;
		}
		p_EpgInfoNode->next = 0;
		p_ProgInfoNode->progInfo.arrEpgInfoNode[week] = p_EpgInfoNode;
	}

	while (p_EpgInfoNode->next != 0)
	{
		if (p_EpgInfoNode->next->epgInfo.startTime == pEpgInfo->startTime 
			&& p_EpgInfoNode->next->epgInfo.endTime == pEpgInfo->endTime
			&& p_EpgInfoNode->next->epgInfo.day == pEpgInfo->day)/*相同EPG返回*/
		{
			return APP_FAIL;
		}
		if (p_EpgInfoNode->next->epgInfo.startTime > pEpgInfo->startTime
			&& p_EpgInfoNode->next->epgInfo.day == pEpgInfo->day)
		{
			break;
		}

		p_EpgInfoNode = p_EpgInfoNode->next;

		if(++testI >= 1000)
			return APP_FAIL;
	}

	p_EpgInfoNodeNew = DBA_IPI_MallocEpgInfoNode(p_ProgInfoNode);	
	if (p_EpgInfoNodeNew == 0)
	{
		return APP_FAIL;
	}

#if 1
	/*
	添加"(跨天)"在当天最后一条
	*/
	if(pEpgInfo->endTime < pEpgInfo->startTime && pEpgInfo->endTime != 0)
	{
		//BU08 epgInfoTemp[APP_EPG_NAME_LEN];
		strcpy(epgInfoTemp, pEpgInfo->strEpgInfo);
		epgInfoTemp[APP_EPG_NAME_LEN - 10] = '\0';
		sprintf(pEpgInfo->strEpgInfo, "%s (跨天)", epgInfoTemp);

	}
#endif
	memcpy(&(p_EpgInfoNodeNew->epgInfo), pEpgInfo, sizeof(struct epgInfo_s));
	p_EpgInfoNodeNew->next = p_EpgInfoNode->next;
	p_EpgInfoNode->next = p_EpgInfoNodeNew;
#if 1
	/*
	添加跨天EPG到第二天第一条
	*/
epg_to_next_day:
	if(pEpgInfo->endTime < pEpgInfo->startTime && pEpgInfo->endTime != 0)
	{
//		pEpgInfo->startTime = 0;
//		pEpgInfo->week = pEpgInfo->week + 1;
		week = week + 1;
		if(week > 6)
		{
			if(g_Week == 1)
			{
				return APP_SUCCESS;
			}
			else
			{
				week = 0;
			}
		}
		p_EpgInfoNode = p_ProgInfoNode->progInfo.arrEpgInfoNode[week];
		if (p_EpgInfoNode == 0)
		{
			p_EpgInfoNode = DBA_IPI_MallocEpgInfoNode(p_ProgInfoNode);	
			if (p_EpgInfoNode == 0)
			{
				return APP_SUCCESS;
			}
			p_EpgInfoNode->next = 0;
			p_ProgInfoNode->progInfo.arrEpgInfoNode[week] = p_EpgInfoNode;
		}
		else if(p_EpgInfoNode->next != 0)
		{
			if(p_EpgInfoNode->next->epgInfo.startTime == pEpgInfo->startTime 
				&& p_EpgInfoNode->next->epgInfo.endTime == pEpgInfo->endTime
				&& p_EpgInfoNode->next->epgInfo.eventId == pEpgInfo->eventId)
			{
				return APP_SUCCESS;
			}
		}
		p_EpgInfoNodeNew = DBA_IPI_MallocEpgInfoNode(p_ProgInfoNode);	
		if (p_EpgInfoNodeNew == 0)
		{
			return APP_SUCCESS;
		}
		memcpy(&(p_EpgInfoNodeNew->epgInfo), pEpgInfo, sizeof(struct epgInfo_s));
		p_EpgInfoNodeNew->next = p_EpgInfoNode->next;
		p_EpgInfoNode->next = p_EpgInfoNodeNew;

	}
#endif
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:根据progid获得该节目某天的epg事件链表
	
	Parameters:
	
-------------------------------------------------------------------------------*/
struct epgInfoNode_s* DBA_API_GetEpgNode(BU32 progId, BU08 week)
{
	struct progInfoNode_s*	p_ProgInfoNode = 0;

	week = week - 1;

	if (week > 6)
	{
		week = week % 7;
	}
	/*
	找到节目位置
	*/
	p_ProgInfoNode = DBA_API_GetProgInfoNode(progId, 1);
	if (p_ProgInfoNode == 0)
	{
		return APP_FAIL;
	}

	return (p_ProgInfoNode->progInfo.arrEpgInfoNode[week]);
}


/*-------------------------------------------------------------------------------

	Description:添加事件的详细信息
	
	Parameters:
	
-------------------------------------------------------------------------------*/

BU08 DBA_API_AddDetail(EIT_Content_s *pEitContent)
{
	BU32 i = 0;
	BU32 nowWeek = 0;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	static BU32 usedcount = 0;

	if(pEitContent == 0)
		return APP_FAIL;
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&ProgInfoListtemp))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo  over = %d\n",APP_ChnDatabase_API_GetCurChnNO());
		return APP_FAIL; 
	}	
	nowWeek = g_Week + g_DayOffset;//记录上次查询的星期
	//app_printf("g_week = %d, g_daypffset = %d\n", g_Week, g_DayOffset);
	if (nowWeek > 7)
	{
		nowWeek = nowWeek % 7;
	}

	//重复的事件描述就返回
	for (i = 0; i < usedcount; i++)
	{
		if (epgDetail[i].week == pEitContent->week && epgDetail[i].enventId == pEitContent->event_id)
		{
			return APP_FAIL;
		}
	}

	//没达到最大的描述事件的个数就直接添加
	if (usedcount < DETAIL_COUNT)
	{
		epgDetail[usedcount].progId = JoinSevIdFreq(pEitContent->service_id, APP_IPI_GetTunerFreq());
		epgDetail[usedcount].enventId = pEitContent->event_id;
		epgDetail[usedcount].week = pEitContent->week;
		memcpy(epgDetail[usedcount].strDetail, pEitContent->detailInfo, APP_DETAIL_LEN - 1);
		epgDetail[usedcount].strDetail[APP_DETAIL_LEN] = '\0';
		++usedcount;
		return APP_SUCCESS;
		
	}
    //资源不够时，找到一个非当前节目的epg描述事件覆盖
	for (i = 0; i < DETAIL_COUNT; i++)
	{
		if (epgDetail[i].progId != ProgInfoListtemp.m_ProgId)
			break;
	}

	if (i == DETAIL_COUNT)
	{	
		for (i = 0; i < DETAIL_COUNT; i++)
		{
			if (nowWeek != epgDetail[i].week)
				break;
		}
	}

	if (i != DETAIL_COUNT)
	{
		epgDetail[i].progId = JoinSevIdFreq(pEitContent->service_id, APP_IPI_GetTunerFreq());
		epgDetail[i].enventId = pEitContent->event_id;
		epgDetail[i].week = pEitContent->week;
		memcpy(epgDetail[i].strDetail, pEitContent->detailInfo, APP_DETAIL_LEN - 1);
		epgDetail[i].strDetail[APP_DETAIL_LEN] = '\0';
		return APP_SUCCESS;
	}
	return APP_FAIL;
}

/*-------------------------------------------------------------------------------

	Description:获得特定事件的详细信息
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU08 DBA_API_GetDetail(BU32 progId, BU32 eventId, BU08 *strDetail, BU32 strLen)
{	
	BU32 i = 0;

	
	strDetail[0] = '\0';
	for (i = 0; i < DETAIL_COUNT; i++)
	{
		if(epgDetail[i].progId == progId && epgDetail[i].enventId == eventId)
		{	
			if (strLen < APP_DETAIL_LEN)
			{
				memcpy(strDetail, epgDetail[i].strDetail, strLen);
				strDetail[strLen] = '\0';
			}
			else
			{
				memcpy(strDetail, epgDetail[i].strDetail, APP_DETAIL_LEN-1);
				strDetail[APP_DETAIL_LEN-1] = '\0';
			}
			
			return APP_SUCCESS;
		}
	}
	return APP_FAIL;
}

BU08 DBA_API_AddNitFreqInfo(BU32 freq, BU32 sym, BU08 qam)
{
	BU32 i = 0;

	if(nitFreqCount >= APP_NIT_FREQ_COUNT || freq==0)
	{
		return APP_FAIL;
	}
		
	for(i = 0; i < nitFreqCount && i < APP_NIT_FREQ_COUNT; i++)
	{
		if(freq == arrNitFreqInfo[i].freq
			&& sym == arrNitFreqInfo[i].sym
			&& qam == arrNitFreqInfo[i].qam)
		{
			return APP_FAIL;
		}
	}

	arrNitFreqInfo[nitFreqCount].freq = freq;
	arrNitFreqInfo[nitFreqCount].sym = sym;
	arrNitFreqInfo[nitFreqCount].qam = qam;
	nitFreqCount++;
	return APP_SUCCESS;
}

void DBA_API_SortNitFreqInfo(void)
{
	BU32 i = 0;
	BU32 top = 0;
	struct nitFreqInfo_s nitFITemp;

	top = nitFreqCount;
	for(; top > 0; top--)
	{
		for(i = 0; i < top - 1; i++)
		{
			if(arrNitFreqInfo[i + 1].freq < arrNitFreqInfo[i].freq)
			{
				memcpy(&nitFITemp, &arrNitFreqInfo[i + 1], sizeof(struct nitFreqInfo_s));
				memcpy(&arrNitFreqInfo[i + 1], &arrNitFreqInfo[i], sizeof(struct nitFreqInfo_s));
				memcpy(&arrNitFreqInfo[i], &nitFITemp, sizeof(struct nitFreqInfo_s));
			}
		}
	}
	
}

void DBA_API_GetNitFreqInfo(BU32 index, struct nitFreqInfo_s *pNitFreqInfo)
{
	if(index >= nitFreqCount)
	{
		memset(pNitFreqInfo, 0, sizeof(struct nitFreqInfo_s));
		return;
	}

	memcpy(pNitFreqInfo, &(arrNitFreqInfo[index]), sizeof(struct nitFreqInfo_s));
}

void DBA_API_ClearNitFreqInfo(void)
{
	nitFreqCount = 0;
}

BU32 DBA_API_GetNitFreqInfoCount(void)
{
	return nitFreqCount;
}



BU08 DBA_API_AddPmtPidInfo(BU32 SID, BU32 PID, BU32 TSID)
{
	BU32 i = 0;

	if(pmtPIDCount >= APP_PMT_PID_COUNT)
	{
		return APP_FAIL;
	}
		
	for(i = 0; i < pmtPIDCount && i < APP_PMT_PID_COUNT; i++)
	{
		if(SID == arrPmtPIDInfo[i].SID || PID == arrPmtPIDInfo[i].PID)
		{
			return APP_FAIL;
		}
	}

	arrPmtPIDInfo[pmtPIDCount].SID = SID;
	arrPmtPIDInfo[pmtPIDCount].PID = PID;
	arrPmtPIDInfo[pmtPIDCount].TSID = TSID;
	pmtPIDCount++;
	return APP_SUCCESS;
}



void DBA_API_ClearPmtPidInfo(void)
{
	pmtPIDCount = 0;
}

BU32 DBA_API_GetPmtPidInfoCount(void)
{
	return pmtPIDCount;
}

void DBA_API_GetPmtPidInfoBySID(BU32 SID, struct pmtPIDInfo_s *pPmtPidInfo)
{
	BU32 i = 0;

	for(i = 0; i < pmtPIDCount; i++)
	{
		if(SID == arrPmtPIDInfo[i].SID)
		{
			break;
		}
	}

	if(i != pmtPIDCount)
	{
		memcpy(pPmtPidInfo, &(arrPmtPIDInfo[i]), sizeof(struct pmtPIDInfo_s));
	}
	else
	{
		memset(pPmtPidInfo, 0, sizeof(struct pmtPIDInfo_s));
	}
}

void DBA_API_GetPmtPidInfo(BU32 index, struct pmtPIDInfo_s *pPmtPidInfo)
{
	if(index >= pmtPIDCount)
	{
		memset(pPmtPidInfo, 0, sizeof(struct pmtPIDInfo_s));
		return;
	}

	memcpy(pPmtPidInfo, &(arrPmtPIDInfo[index]), sizeof(struct pmtPIDInfo_s));
}


void DBA_API_GetSdtInfoBySerId(BU32 serId, struct sdtInfo_s *pSdtInfo)
{
	BU32 i = 0;

	for(i = 0; i < sdtInfoCount; i++)
	{
		if(serId == arrSdtInfo[i].serId)
		{
			break;
		}
	}

	if(i != sdtInfoCount)
	{
		memcpy(pSdtInfo, &(arrSdtInfo[i]), sizeof(struct sdtInfo_s));
	}
	else
	{
		memset(pSdtInfo, 0, sizeof(struct sdtInfo_s));
	}
}

BU08 DBA_API_AddSdtInfo(BU32 serId, BU08 *progName, BU08 serType, BU16 oriNetId)
{
	BU32 i = 0;

	if(sdtInfoCount >= APP_SDT_INFO_COUNT)
	{
		return APP_FAIL;
	}
		
	for(i = 0; i < sdtInfoCount && i < APP_SDT_INFO_COUNT; i++)
	{
		if(serId == arrSdtInfo[i].serId)
		{
			return APP_FAIL;
		}
	}

	arrSdtInfo[sdtInfoCount].serId= serId;
	strcpy(arrSdtInfo[sdtInfoCount].progName, progName);
	arrSdtInfo[sdtInfoCount].serType= serType;
	arrSdtInfo[sdtInfoCount].OriNetId = oriNetId;
	sdtInfoCount++;
	return 1;
}

void DBA_API_ClearSdtInfo(void)
{
	sdtInfoCount = 0;
}



void DBA_API_ADD_BroadCast(BroadCast_ChnInfo_S *chinfo)
{
	BU32 i;
	if (broadcastcount >= BROADCAST_COUNT)
	{
		return;
	}
	for (i = 0; i < broadcastcount; i++)
	{
		if (BroadProg[i].m_ProgId == chinfo->m_ProgId)
			return;
	}
	memcpy(&(BroadProg[broadcastcount++]), chinfo, sizeof(ChnDataBase_ChnInfo_S));
	//app_printf("freq = %d, sym = %d, qam = %d, serid = %d\n",BroadProg[broadcastcount - 1].m_FreqMHz, BroadProg[broadcastcount - 1].m_SymKbps, BroadProg[broadcastcount - 1].m_QAM,BroadProg[broadcastcount - 1].m_ProgId);
	//app_printf("-------------broadcastcount = %d--------------------\n", broadcastcount);
}


void DBA_API_CreateBroadCastList()
{
	BU32 i = 0;
	BroadCast_ChnInfo_S broad;
	broadcastcount = 0;
	sprintf(g_StrSQL, "select * from broadcast ORDER BY 编号 ASC, service_id ASC;");
	SQL_API_Select(g_StrSQL);
	for (i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		SQL_API_GotoRow(i);
		
		broad.m_ProgId 	= SQL_API_GetData((BU32)BROADCAST_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_ChnIndex 		= i;//(BU32) SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData((BU32)BROADCAST_CHN_NAME, broad.m_ChnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);

		broad.m_AudioChn 	= (BU08) SQL_API_GetData((BU32)BROADCAST_TRACK, 0, 0, APP_DATA_TYPE_NUM);
		
		broad.m_FreqMHz 	= (BU16) SQL_API_GetData((BU32)BROADCAST_FREQ, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_QAM 		= (BU08) SQL_API_GetData((BU32)BROADCAST_QAM, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_SymKbps 	= (BU16) SQL_API_GetData((BU32)BROADCAST_SYMKBPS, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_style 		= (BU08) SQL_API_GetData((BU32)BROADCAST_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_VideoPid 	= (BU16) SQL_API_GetData((BU32)BROADCAST_VIDEO_PID, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_AudioPid 	= (BU16) SQL_API_GetData((BU32)BROADCAST_AUDIO_PID, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_PcrPid 	= (BU16) SQL_API_GetData((BU32)BROADCAST_PCRPID, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_TsId       = (BU32) SQL_API_GetData((BU32)BROADCAST_TSID, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_pmtpid     = (BU16) SQL_API_GetData((BU32)BROADCAST_PID, 0, 0, APP_DATA_TYPE_NUM);
		broad.m_OriNetId   = (BU16) SQL_API_GetData((BU32)BROADCAST_ORI_NET_ID, 0, 0, APP_DATA_TYPE_NUM);
		DBA_API_ADD_BroadCast(&broad);
	}

	APP_PRINT_DEBUG("broadcastcount = %d\n", broadcastcount);


}



BS32 DBA_API_AV_GetFreqByServiceId(BroadCast_ChnInfo_S *chinfo, int network_id, int ts_id,  int service_id)
{

	BU32 i;
	if (chinfo == NULL)
	{
		APP_PRINT_ERROR("chinfo == NULL\n");
		return -1;
	}
	APP_PRINT_INFO("service_id %d ==============================broadcastcount = %d\n",service_id,broadcastcount);
	for (i = 0; i < broadcastcount; i++)
	{
		APP_Printf("netid = %d, tsid = %d, progid = %d\n", BroadProg[i].m_OriNetId, BroadProg[i].m_TsId, BroadProg[i].m_ProgId);
		if (/*network_id == BroadProg[i].m_OriNetId && ts_id == BroadProg[i].m_TsId &&*/ service_id == BroadProg[i].m_ProgId)
		{
			memcpy(chinfo, &(BroadProg[i]), sizeof(BroadCast_ChnInfo_S));
			break;
		}
	}
	if (i != broadcastcount)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

BS32 DBA_API_AV_GetFreqByAVID(BroadCast_ChnInfo_S *chinfo, BU32 video_pid, BU32 audio_pid, BU32 pcr_pid)
{

	BU32 i;
	if (chinfo == NULL)
	{
		return -1;
	}
	
	for (i = 0; i < broadcastcount; i++)
	{
		if (video_pid == BroadProg[i].m_VideoPid && audio_pid == BroadProg[i].m_AudioPid /*&& pcr_pid == BroadProg[i].m_PcrPid*/)
		{
			memcpy(chinfo, &(BroadProg[i]), sizeof(BroadCast_ChnInfo_S));
			break;
		}
	}
	if (i != broadcastcount)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

BS32 DBA_API_AV_GetTsidByServID(BroadCast_ChnInfo_S *chinfo, BU32 usSrvId)
{

	BU32 i;
	if (chinfo == NULL)
	{
		return -1;
	}
	
	for (i = 0; i < broadcastcount; i++)
	{
		if (usSrvId == BroadProg[i].m_ProgId)
		{
			memcpy(chinfo, &(BroadProg[i]), sizeof(BroadCast_ChnInfo_S));
			break;
		}
	}
	if (i != broadcastcount)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}
/*-------------------------------------------------------------------------------

	Description:获得数据广播的总数

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_DBA_API_GetBroadCastNum(void)
{
	BU32 i;
	BU32 brdcastnum = 0;
	BroadCast_ChnInfo_S brdcastInfo;
	for(i = 0;i < broadcastcount;i++)
	{
		if(!BroadProg[i].m_AudioPid)
			brdcastnum ++;
	}
	return brdcastnum;
}

/*-------------------------------------------------------------------------------

	Description:得到当前数据广播的信息

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_DBA_API_GetBroadcastInfo(BU32 brdcastNo, BroadCast_ChnInfo_S* brdcastInfo)
{
	BU32 i;
	BU32 index = 0;
	if(brdcastNo >= broadcastcount)
		return APP_FAIL;
	for(i = 0;i < broadcastcount;i++)
	{
		if(!BroadProg[i].m_AudioPid)
			index ++;
		if(index == (brdcastNo + 1))
		{
			memcpy(brdcastInfo, BroadProg+i, sizeof(BroadCast_ChnInfo_S));
			return APP_SUCCESS;
		}
	}
	return APP_FAIL;
}


