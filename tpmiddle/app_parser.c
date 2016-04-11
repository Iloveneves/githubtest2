#include "app_common.h"
#include "app_ipi_common.h"

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern BU32 semaphoreDemux;


static struct ParserFuncNode_s *pFirstPasFuncNode = 0;

void APP_IPI_Semaphore_Wait_Demux(void)
{
	BU32 rv = 0;


	rv = FYF_API_semaphore_wait(semaphoreDemux, FYF_WAIT_FOREVER);//FYF_WAIT_FOREVER);
	
	if(rv != FYF_OS_QUEUE_SEM_STATUS_AVAILABLE)
	{

	}	
}

void APP_IPI_Semaphore_Release_Demux(void)
{
	FYF_API_semaphore_release(semaphoreDemux);
}
/*-------------------------------------------------------------------------------

	Description:从表分析函数链中查找一个结点,找到则返回对应分析函数,否则返回空
	
	Parameters:
	
-------------------------------------------------------------------------------*/
APP_ParserFunc APP_IPI_GetParserFunc(BU32 channelId, BU32 filterId)
{
	struct ParserFuncNode_s *pPasFuncNode = 0;
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreParserFuncList(), FYF_WAIT_FOREVER);
	if(pFirstPasFuncNode == 0)
	{
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
		return 0;
	}
	pPasFuncNode = pFirstPasFuncNode->next;
	while(pPasFuncNode != 0 && pPasFuncNode->isInUse == 1)
	{
		if(pPasFuncNode->channelId == channelId && pPasFuncNode->filterId == filterId)
		{
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
			return pPasFuncNode->ParserFunc;
		}
		pPasFuncNode = pPasFuncNode->next;
	}
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
	return 0;
}

/*-------------------------------------------------------------------------------

	Description:从表分析函数链中释放一个结点,但不释放内存,而是复位结点为默认值,挂在链表最后.
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_IPI_DelPasFuncNode(BU32 channelId, BU32 filterId)
{
	struct ParserFuncNode_s *pPasFuncNode = 0;
	struct ParserFuncNode_s *pPasFuncNodeLast = 0;
	struct ParserFuncNode_s *pPasFuncNodeDel = 0;

	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreParserFuncList(), FYF_WAIT_FOREVER);
	if (pFirstPasFuncNode == 0)
	{
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
		return;
	}
	pPasFuncNode = pFirstPasFuncNode->next;
	pPasFuncNodeLast = pFirstPasFuncNode;


	while (pPasFuncNode != 0 && pPasFuncNode->isInUse == 1)
	{
		if (pPasFuncNode->channelId == channelId && pPasFuncNode->filterId == filterId)
		{
			break;
		}

		pPasFuncNodeLast = pPasFuncNode;
		pPasFuncNode = pPasFuncNode->next;
	}
	if (pPasFuncNode == 0)
	{
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
		FYF_API_printf("	<<<<<<<<<<<<<APP_IPI_DelPasFuncNode channelId = %d, filterId = %d\n", channelId, filterId);
		return;
	}
	pPasFuncNodeLast->next = pPasFuncNode->next;
	pPasFuncNodeDel = pPasFuncNode;
	pPasFuncNode = pPasFuncNodeLast;
	while(pPasFuncNode->next != 0)
	{
		pPasFuncNode = pPasFuncNode->next;
	}
	/*挂到链最后位置*/
	pPasFuncNode->next = pPasFuncNodeDel;
	pPasFuncNodeDel->ParserFunc = NULL;
	pPasFuncNodeDel->isInUse = 0;
	pPasFuncNodeDel->next = 0;
	
//	FYF_API_free(pPasFuncNode);
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
}

/*-------------------------------------------------------------------------------

	Description:在表分析函数链中添加一个分析函数
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_IPI_AddPasFuncNode(struct ParserFuncNode_s parserFuncNode)
{
	struct ParserFuncNode_s *pPasFuncNode = 0;
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreParserFuncList(), FYF_WAIT_FOREVER);

	/*第一次添加*/
	if (pFirstPasFuncNode == 0)
	{
		pFirstPasFuncNode = FYF_API_malloc(sizeof(struct ParserFuncNode_s));
		if (pFirstPasFuncNode == 0)
		{			
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
			return;
		}
		pFirstPasFuncNode->isInUse = 1;
		pFirstPasFuncNode->next = FYF_API_malloc(sizeof(struct ParserFuncNode_s));
		if(pFirstPasFuncNode->next == 0)
		{			
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
			return;
		}
		memcpy(pFirstPasFuncNode->next, &parserFuncNode, sizeof(struct ParserFuncNode_s));
		pFirstPasFuncNode->next->isInUse = 1;
		pFirstPasFuncNode->next->next = 0;
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
		return;
	}

	pPasFuncNode = pFirstPasFuncNode->next;
	while (pPasFuncNode != 0)
	{
		/*找到未使用的结点*/
		if(pPasFuncNode->isInUse == 0)
		{
			pPasFuncNode->channelId = parserFuncNode.channelId;
			pPasFuncNode->filterId = parserFuncNode.filterId;
			pPasFuncNode->ParserFunc = parserFuncNode.ParserFunc;
			pPasFuncNode->isInUse = 1;
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
			return;
		}
		/*相同结点,返回*/
		if (pPasFuncNode->channelId == parserFuncNode.channelId && pPasFuncNode->filterId == parserFuncNode.filterId)
		{			
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
			return;
		}		
		pPasFuncNode = pPasFuncNode->next;
	}

	/*添加新结点*/
	pPasFuncNode = pFirstPasFuncNode->next;
	pFirstPasFuncNode->next = FYF_API_malloc(sizeof(struct ParserFuncNode_s));
	if(pFirstPasFuncNode->next == 0)
	{
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
		return;
	}
	memcpy(pFirstPasFuncNode->next, &parserFuncNode, sizeof(struct ParserFuncNode_s));
	pFirstPasFuncNode->next->next = pPasFuncNode;
	pFirstPasFuncNode->next->isInUse = 1;

	FYF_API_semaphore_release(APP_IPI_GetSemaphoreParserFuncList());
}
/*-------------------------------------------------------------------------------

	Description:公用函数,初始化接收数据段链表头结点,此结点用于存放已经收到数据段的总数.
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_IPI_InitSectionNode(SectionNode_s **ppFirstSectionNode)
{
//	ASSERT(*ppFirstSectionNode == 0);
	if(*ppFirstSectionNode == 0)
	{
		*ppFirstSectionNode = FYF_API_malloc(sizeof(SectionNode_s));
		if (ppFirstSectionNode == 0)
		{
			//ASSERT(0);
			return;
		}
		(*ppFirstSectionNode)->next = 0;
		(*ppFirstSectionNode)->section = 0;		
		(*ppFirstSectionNode)->isUsed = 1;
	}
}

/*-------------------------------------------------------------------------------

	Description:返回已经收到数据段的总数,与last_section_number对比,看是否接收完毕.
	
	Parameters:
	
-------------------------------------------------------------------------------*/

BU32 APP_IPI_GetSectionCount(SectionNode_s *pFirstSectionNode)
{
	if (pFirstSectionNode == 0)
	{
		return 0;
	}
	return pFirstSectionNode->section;
}
/*-------------------------------------------------------------------------------

	Description:循环清零,并不释放内存
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_IPI_ClearSection(SectionNode_s **ppFirstSectionNode)
{
	SectionNode_s *pSectionNode = 0;
	pSectionNode = *ppFirstSectionNode;
	while (pSectionNode != 0)
	{
		pSectionNode->isUsed = 0;
		pSectionNode->section = 0;
		pSectionNode = pSectionNode->next;
	}
}

/*-------------------------------------------------------------------------------

	Description:记录已经接收的段
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU08 APP_IPI_AddSection(SectionNode_s *pFirstSectionNode, BU08 section)
{
	SectionNode_s *pSectionNode = 0;
	SectionNode_s *pSectionNodeTemp = 0;
	
	if(pFirstSectionNode == 0)
		return APP_FAIL;
		
	pSectionNode = pFirstSectionNode->next;
	
	while (pSectionNode != 0 && pSectionNode->isUsed == 1)
	{
		/*此段已经接收,返回*/
		if (pSectionNode->section == section)
		{
			return APP_FAIL;
		}
		pSectionNode = pSectionNode->next;
	}
	if(pSectionNode != 0)
	{
		pSectionNode->section = section;
		pSectionNode->isUsed = 1;
	}
	else
	{
		/*新建一个结点,挂在链表第二个位置*/
		pSectionNodeTemp = FYF_API_malloc(sizeof(SectionNode_s));
		if (pSectionNodeTemp == 0)
		{
			return APP_FAIL;
		}
		pSectionNodeTemp->section = section;
		pSectionNodeTemp->isUsed = 1;
		pSectionNodeTemp->next = pFirstSectionNode->next;
		pFirstSectionNode->next = pSectionNodeTemp;
	}
	pFirstSectionNode->section = pFirstSectionNode->section + 1;
	return APP_SUCCESS;
}

BU32 APP_Parser_IPI_StopDemux(BU32 *channelID, BU32 *filterID)
{
	BU32 returnValue = 0;
	
	
	APP_IPI_Semaphore_Wait_Demux();
	if (*channelID == 0 && *filterID == 0)
	{	
		APP_IPI_Semaphore_Release_Demux();
		return 1;
	}
	/*
	if (*channelID == 0 && *filterID == 0)
	{
		//APP_IPI_Semaphore_Release_Demux();
		return 1;
	}
	*/
	//APP_Printf("______StopDemux() channelID = 0x%X, filterID = 0x%X\n", *channelID, *filterID);

	APP_IPI_DelPasFuncNode(*channelID, *filterID);
	if((returnValue =  FYF_API_demux_disable_filter(*channelID, *filterID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	
	if((returnValue = FYF_API_demux_destroy_filter(*channelID, *filterID)) !=0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	
	if((returnValue = FYF_API_demux_stop_channel(*channelID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	
	if((returnValue = FYF_API_demux_destroy_channel(*channelID))!= 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	
	*channelID = 0;
	*filterID = 0;
	APP_IPI_Semaphore_Release_Demux();
	//APP_Printf("		StopDemux() channelID = %d, filterID = %d\n", *channelID, *filterID);
	return 1;
}

BU32 APP_Parser_IPI_StartDemux(BU32 wide,BU16 pID, BU08 coef[], BU08 mask[], BU08 excl[], BU32 *channelID, BU32 *filterID)
{

	BU32 returnValue = 0;
	//APP_Printf("	StartDemux pID =%d, coef = %d\n", pID, coef[0]);
	APP_IPI_Semaphore_Wait_Demux();
	FYF_API_demux_create_channel(64 * 1024, FYF_DEMUX_DATA_PSI_CHANNEL, channelID, pID);
	
	if((returnValue = FYF_API_demux_set_channel_pid(*channelID, pID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	
	FYF_API_demux_create_filter(*channelID, filterID);
	
	if((returnValue = FYF_API_demux_set_filter(*channelID, *filterID, wide, coef, mask, excl))!= 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	
	if((returnValue = FYF_API_demux_enable_filter(*channelID, *filterID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}

	if((returnValue = FYF_API_demux_start_channel(*channelID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return APP_FAIL;
	}
	

	//APP_Printf("------StartDemux channelID = 0x%X, filterID = 0x%X, pID = 0x%X, coef = 0x%X\n", *channelID, *filterID, pID, coef[0]);

	APP_IPI_Semaphore_Release_Demux();

	return 1;
}









