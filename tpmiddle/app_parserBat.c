#include "app_common.h"
#include "app_ipi_common.h"

//#define BAT_SANMING


/**************************************************************
分析BAT表的结果
**************************************************************/
typedef struct _BAT_Content_s
{
	BU32 bouquetId;
	BU08 bouquetName[APP_CHN_NAME_LEN];
	BU32 serviceId;
	BS08 audioOffset;
	BU08 audioTrack;
	BU32 progLCN;
}BAT_Content_s;

extern struct sortInfo_s g_SortInfo[CHN_MAX_SORT_COUNT];
extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];

static BU32 batChannelID = 0;
static BU32 batFilterID = 0;
static BU08 isStopParserBat = 0;
static BU32 queueParserBatEnd = 0;

#define TPDBASE_BOUQUETNAME_LENGTH_MAX			19
#define	TPDBASE_BOUQUET_NAME_DESC				0x47
#define TPDBASE_PROGSORT_MAX					32
#define	SERVICE_LIST_DESC				0x41



/*----------------------------------------------------------------------
Function Prototypes
----------------------------------------------------------------------*/
BU32 DBA_IPI_GetProgJoinServiceId(BU32 serviceId);
void APP_BAT_IPI_SaveBatInfo(BAT_Content_s *pBatContent, BU08 style);



static INT BATPARSER_ParseDescriptor(BYTE * pucSectionData, BAT_Content_s *pBatContent, 
	SHORT sTransportStreamId, SHORT sOrgNetworkId)
{
	INT iDescriptorLength;
	INT iNoOfBytesParsed;
	BYTE DescTag = pucSectionData[0];

	iNoOfBytesParsed = 0;
	iDescriptorLength = pucSectionData[1];

	switch(DescTag)
	{
		/*bouquet名称*/
		case TPDBASE_BOUQUET_NAME_DESC:
			pucSectionData += 2;

			iNoOfBytesParsed = iDescriptorLength > (APP_CHN_NAME_LEN - 2) ? (APP_CHN_NAME_LEN - 2) : iDescriptorLength;
			memcpy(pBatContent->bouquetName,(CHAR *)pucSectionData, iNoOfBytesParsed);
			pBatContent->bouquetName[iNoOfBytesParsed] = '\0';
			APP_BAT_IPI_SaveBatInfo(pBatContent, 0);			
			break;
		/*节目serviceId*/
		case SERVICE_LIST_DESC:
			pucSectionData += 2;
			iNoOfBytesParsed = 0;
			while (iNoOfBytesParsed < iDescriptorLength)
			{			
				pBatContent->serviceId = (pucSectionData[0] << 8) | pucSectionData[1];
				pBatContent->audioOffset = 0;
				pBatContent->audioTrack = 0;
				pBatContent->progLCN = 0;
				APP_BAT_IPI_SaveBatInfo(pBatContent, 1);

				pucSectionData += 3;
				iNoOfBytesParsed += 3;
			}
			break;
#ifdef BAT_SANMING
		case 0x82:
			{
				INT iDescLength;
				INT iAudtrack = 1;
				BS08 iLevel;
				USHORT usProgNo, usProgLCN;
				SHORT iTemp;

				iDescLength = pucSectionData[1];
				pucSectionData += 2;

				while (iDescLength > 0)
				{
					usProgNo = (pucSectionData[0] << 8) | pucSectionData[1];
					usProgLCN = (pucSectionData[2] << 8) | pucSectionData[3];
					pBatContent->bouquetId = 800;
					pBatContent->serviceId = usProgNo;
					pBatContent->progLCN = usProgLCN;

					if (iAudtrack == 0)
					{
						iAudtrack = APP_CHN_TRACK_STEREO_e;
					}
					else if (iAudtrack == 1)
					{
						iAudtrack = APP_CHN_TRACK_LEFT_e;
					}
					else if (iAudtrack == 2)
					{
						iAudtrack = APP_CHN_TRACK_RIGHT_e;
					}
					else
					{
						iAudtrack = APP_CHN_TRACK_LEFT_e;
					}
					
					pBatContent->audioTrack = iAudtrack;
					APP_BAT_IPI_SaveBatInfo(pBatContent, 2);
					iDescLength -= 4;
					pucSectionData += 4;
				}
			}	
			break;

#else
		/*音量补偿，声道，逻辑频道*/
		case 0x80:
			{
				INT iDescLength;
				INT iAudtrack = 1;
				BS08 iLevel;
				USHORT usProgNo, usProgLCN;
				SHORT iTemp;

				iDescLength = pucSectionData[1];
				pucSectionData += 2;

				while (iDescLength > 0)
				{
					usProgNo = (pucSectionData[0] << 8) | pucSectionData[1];
					usProgLCN = (pucSectionData[2] << 8) | pucSectionData[3];
					iAudtrack = pucSectionData[4];
					iLevel = pucSectionData[5];
					pBatContent->bouquetId = 800;
					pBatContent->serviceId = usProgNo;
					pBatContent->audioOffset = iLevel;
					pBatContent->progLCN = usProgLCN;

					if (iAudtrack == 0)
					{
						iAudtrack = APP_CHN_TRACK_STEREO_e;
					}
					else if (iAudtrack == 1)
					{
						iAudtrack = APP_CHN_TRACK_LEFT_e;
					}
					else if (iAudtrack == 2)
					{
						iAudtrack = APP_CHN_TRACK_RIGHT_e;
					}
					else
					{
						iAudtrack = APP_CHN_TRACK_LEFT_e;
					}
					
					pBatContent->audioTrack = iAudtrack;
					APP_BAT_IPI_SaveBatInfo(pBatContent, 2);
					iDescLength -= 6;
					pucSectionData += 6;
				}	
			}
			break;
#endif
			default:
				break;
	}

	return (iDescriptorLength + 2);
}

BU32 APP_BAT_IPI_ParserBat(BU08 * section, void (*SaveBatInfo)(BAT_Content_s *, BU08))
{
	BAT_Content_s batContent;
	BYTE *pucSectionData;
	INT iNoOfDataBytes;
	INT iInfoLength;
	INT iNoOfBytesParsedSoFar;
	INT iTsInfoLength;
	SHORT2BYTE stTempData16;
	WORD2SHORT stTempData32;
	SHORT sTransportStreamId;
	SHORT sOrgNetworkId;
	BYTE ucLastSectionNumber = 0;
	BYTE ucSectionNumber = 0;
	BYTE ucVersionNumber = 0;
	BYTE ucCurNextIndicator = 0;

	batContent.bouquetId = (section[3] << 8) | section[4];
	batContent.bouquetName[0] = '\0';
	batContent.serviceId = 0;
	batContent.audioOffset = 0;
	batContent.audioTrack = 1;
	
	ucVersionNumber = (section[5] & 0x3E) >> 1;
	ucCurNextIndicator = section[5] & 0x01;
	ucSectionNumber = section[6];
	ucLastSectionNumber = section[7];

	pucSectionData = section + 10;

	stTempData16.byte.ucByte1 = section[8] & 0xF;
	stTempData16.byte.ucByte0 = section[9];

	iInfoLength = stTempData16.sWord16;

	APP_SYS_IPI_SetBatVer(ucVersionNumber);
	
	if (iInfoLength)
	{
		iNoOfBytesParsedSoFar = 0;
		while (iNoOfBytesParsedSoFar < iInfoLength)
		{
			iNoOfDataBytes = BATPARSER_ParseDescriptor(pucSectionData, &batContent, -1, -1);
			pucSectionData += iNoOfDataBytes;
			iNoOfBytesParsedSoFar += iNoOfDataBytes;
		}
	}

	stTempData16.byte.ucByte1 = pucSectionData[0] & 0xF;
	stTempData16.byte.ucByte0 = pucSectionData[1];

	iTsInfoLength = stTempData16.sWord16;
	pucSectionData += 2;

	while (iTsInfoLength > 0)
	{
		stTempData16.byte.ucByte1 = pucSectionData[0];
		stTempData16.byte.ucByte0 = pucSectionData[1];
		stTempData32.unShort.sLo16 = stTempData16.sWord16;
		sTransportStreamId = stTempData16.sWord16;

		stTempData16.byte.ucByte1 = pucSectionData[2];
		stTempData16.byte.ucByte0 = pucSectionData[3];
		stTempData32.unShort.sHi16 = stTempData16.sWord16;
		sOrgNetworkId = stTempData32.unShort.sHi16;

		iInfoLength = (pucSectionData[4] << 8) & 0xF00 | pucSectionData[5];
		pucSectionData += 6;
/*
		if (TPDBASE_BAT_TRANSMIT_TYPE == TPDBASE_BATTRANSMIT_TYPE_HZG)
		{
			TPDBASE_BatParserSetProgBouquetTypeEx(sTransportStreamId, sOrgNetworkId, ProgSortIndex);
		}
*/
		if (iInfoLength)
		{
			iNoOfBytesParsedSoFar = 0;
			while (iNoOfBytesParsedSoFar < iInfoLength)
			{
				iNoOfDataBytes = BATPARSER_ParseDescriptor(pucSectionData, &batContent, sTransportStreamId, sOrgNetworkId);
				pucSectionData += iNoOfDataBytes;
				iNoOfBytesParsedSoFar += iNoOfDataBytes;
			}
		}
		iTsInfoLength = iTsInfoLength - iInfoLength - 6;
	}
}

/*
排序BAT信息
*/
void APP_BAT_IPI_SortBatInfo(struct batInfo_s *pBatInfo)
{
	struct batProgInfo_s *pBatProgInfo = 0;
	struct batProgInfo_s *pBatProgInfoCop = 0;
	struct batProgInfo_s *pBatProgInfoCg = 0;
	struct batProgInfo_s *pBatProgInfoL = 0;
	struct batProgInfo_s batProgInfo;
	struct batProgInfo_s *pBatProgInfoA = 0;
	struct batProgInfo_s *pBatProgInfoB = 0;
	BU32 i = 0;
	
	if(pBatInfo->pFirstBatProgInfo == 0)
		return;

	/*排序LCN*/
	pBatProgInfo = pBatInfo->pFirstBatProgInfo->next;
	while (pBatProgInfo != 0 && pBatProgInfo->isUsed != 0)
	{
		pBatProgInfoCop = pBatProgInfo;
		pBatProgInfoCg = pBatProgInfo;
		while (pBatProgInfoCop != 0 && pBatProgInfoCop->isUsed != 0)
		{
			if (pBatProgInfoCop->progLCN < pBatProgInfoCg->progLCN)
			{
				pBatProgInfoCg = pBatProgInfoCop;
			}

			pBatProgInfoCop = pBatProgInfoCop->next;
		}

		if (pBatProgInfoCg != pBatProgInfo)
		{
			pBatProgInfoA = pBatProgInfo->next;
			pBatProgInfoB = pBatProgInfoCg->next;
			memcpy(&batProgInfo, pBatProgInfo, sizeof(struct batProgInfo_s));
			memcpy(pBatProgInfo, pBatProgInfoCg, sizeof(struct batProgInfo_s));
			memcpy(pBatProgInfoCg, &batProgInfo, sizeof(struct batProgInfo_s));
			pBatProgInfo->next = pBatProgInfoA;
			pBatProgInfoCg->next = pBatProgInfoB;
		}

		pBatProgInfo = pBatProgInfo->next;
	}

	/*在相同LCG中排序serviceId*/
	pBatProgInfo = pBatInfo->pFirstBatProgInfo->next;
	while (pBatProgInfo != 0 && pBatProgInfo->isUsed != 0)
	{
		pBatProgInfoCop = pBatProgInfo;
		pBatProgInfoCg = pBatProgInfo;
		pBatProgInfoL = pBatProgInfo;

		while (pBatProgInfoL != 0 && pBatProgInfoL->isUsed != 0
			&& pBatProgInfoL->progLCN == pBatProgInfoCop->progLCN)
		{
			pBatProgInfoL = pBatProgInfoL->next;
		}

		while (pBatProgInfoCop != 0 && pBatProgInfoCop->isUsed != 0
			&& pBatProgInfoCop != pBatProgInfoL)
		{
			if (pBatProgInfoCop->serviceId < pBatProgInfoCg->serviceId)
			{
				pBatProgInfoCg = pBatProgInfoCop;
			}
			
			pBatProgInfoCop = pBatProgInfoCop->next;
		}
		
		if (pBatProgInfoCg != pBatProgInfo)
		{
			pBatProgInfoA = pBatProgInfo->next;
			pBatProgInfoB = pBatProgInfoCg->next;
			memcpy(&batProgInfo, pBatProgInfo, sizeof(struct batProgInfo_s));
			memcpy(pBatProgInfo, pBatProgInfoCg, sizeof(struct batProgInfo_s));
			memcpy(pBatProgInfoCg, &batProgInfo, sizeof(struct batProgInfo_s));
			pBatProgInfo->next = pBatProgInfoA;
			pBatProgInfoCg->next = pBatProgInfoB;
		}

		pBatProgInfo = pBatProgInfo->next;
	}
	
	/*消除相同逻辑号*/
	pBatProgInfo = pBatInfo->pFirstBatProgInfo->next;
	while(pBatProgInfo != 0 && pBatProgInfo->isUsed != 0)
	{
		pBatProgInfoCop = pBatProgInfo->next;
		i = 1;
		while(pBatProgInfoCop != 0 && pBatProgInfoCop->isUsed != 0)
		{
			if (pBatProgInfoCop->progLCN == pBatProgInfo->progLCN)
			{
				pBatProgInfoCop->progLCN = pBatProgInfoCop->progLCN + (i++);
			}
			else if (i != 1)
			{
				while (pBatProgInfoCop != 0 && pBatProgInfoCop->isUsed != 0)
				{
					pBatProgInfoCop->progLCN = pBatProgInfoCop->progLCN + i;
					pBatProgInfoCop = pBatProgInfoCop->next;
				}
				break;
			}
			pBatProgInfoCop = pBatProgInfoCop->next;
		}
		pBatProgInfo = pBatProgInfo->next;
	}
}


struct batInfo_s g_FirstBatInfo = {0, 0, {0}, 0, 0};

void APP_BAT_IPI_AddBatProgInfo(struct batInfo_s *pBatInfo, BAT_Content_s *pBatContent)
{
	struct batProgInfo_s *pBatProgInfo = 0;
	
	if(pBatInfo->pFirstBatProgInfo == 0)
	{
		pBatInfo->pFirstBatProgInfo = FYF_API_malloc(sizeof(struct batProgInfo_s));
		if(pBatInfo->pFirstBatProgInfo == 0)
			return;
		pBatInfo->pFirstBatProgInfo->isUsed = 1;
		pBatInfo->pFirstBatProgInfo->serviceId = 0;
		pBatInfo->pFirstBatProgInfo->audioOffset = 0;
		pBatInfo->pFirstBatProgInfo->next = 0;
	}

	pBatProgInfo = pBatInfo->pFirstBatProgInfo->next;
	while(pBatProgInfo != 0 && pBatProgInfo->isUsed != 0)
	{
		if(pBatProgInfo->serviceId == pBatContent->serviceId)
			break;
		pBatProgInfo = pBatProgInfo->next;
	}

	if(pBatProgInfo == 0)
	{
		pBatProgInfo = FYF_API_malloc(sizeof(struct batProgInfo_s));
		if(pBatProgInfo == 0)
			return;
		memset(pBatProgInfo, 0, sizeof(struct batProgInfo_s));
		pBatProgInfo->next = pBatInfo->pFirstBatProgInfo->next;
		pBatInfo->pFirstBatProgInfo->next = pBatProgInfo;
	}

	pBatProgInfo->isUsed = 1;
	pBatProgInfo->serviceId = pBatContent->serviceId;
	if(pBatContent->audioOffset != 0)
		pBatProgInfo->audioOffset = pBatContent->audioOffset;
	if(pBatContent->audioTrack != 0)
		pBatProgInfo->audioTrack = pBatContent->audioTrack;
	if(pBatContent->progLCN != 0)
		pBatProgInfo->progLCN = pBatContent->progLCN;
}

/*
style 0 : bqId bqName;
       1 : serviceId
       2 : audioOffset
*/
void APP_BAT_IPI_SaveBatInfo(BAT_Content_s *pBatContent, BU08 style)
{
	struct batInfo_s *pBatInfo = 0;

	pBatInfo = g_FirstBatInfo.next;

	if(style == 0)
	{
		while(pBatInfo != 0 && pBatInfo->isUsed == 1)
		{
			if(pBatInfo->bqId == pBatContent->bouquetId)
			{
				memcpy(pBatInfo->bqName, pBatContent->bouquetName, APP_CHN_NAME_LEN);
				return;
			}
			pBatInfo = pBatInfo->next;
		}
		if(pBatInfo == 0)
		{
			pBatInfo = FYF_API_malloc(sizeof(struct batInfo_s));
			if(pBatInfo == 0)
				return;
			pBatInfo->pFirstBatProgInfo = 0;	
			pBatInfo->next = g_FirstBatInfo.next;
			g_FirstBatInfo.next = pBatInfo;
		}
		pBatInfo->isUsed = 1;
		pBatInfo->bqId = pBatContent->bouquetId;
		memcpy(pBatInfo->bqName, pBatContent->bouquetName, APP_CHN_NAME_LEN);
	}
	else if(style == 1)
	{
		while(pBatInfo != 0 && pBatInfo->isUsed == 1)
		{
			if(pBatInfo->bqId == pBatContent->bouquetId)
			{
				APP_BAT_IPI_AddBatProgInfo(pBatInfo, pBatContent);
				return;
			}
			pBatInfo = pBatInfo->next;
		}	
		//if(pBatInfo == 0)
		{
			APP_BAT_IPI_SaveBatInfo(pBatContent, 0);
			APP_BAT_IPI_SaveBatInfo(pBatContent, 1);
		}
	}
	else if(style == 2)
	{
		while(pBatInfo != 0 && pBatInfo->isUsed == 1)
		{
			if(pBatInfo->bqId == pBatContent->bouquetId)
			{
				APP_BAT_IPI_AddBatProgInfo(pBatInfo, pBatContent);
				return;
			}
			pBatInfo = pBatInfo->next;
		}	
	//	if(pBatInfo == 0)
		{
			APP_BAT_IPI_SaveBatInfo(pBatContent, 0);
			APP_BAT_IPI_SaveBatInfo(pBatContent, 2);
		}
	}
}

void APP_BAT_IPI_ClearBatInfo(void)
{
	struct batInfo_s *pBatInfo = 0;
	struct batProgInfo_s *pBatProgInfo = 0;
	
	pBatInfo = g_FirstBatInfo.next;
	while(pBatInfo != 0)
	{
		pBatInfo->isUsed = 0;
		pBatInfo->bqId = 0;
		pBatInfo->bqName[0] = 0;
		pBatProgInfo = pBatInfo->pFirstBatProgInfo;
		while(pBatProgInfo != 0)
		{
			pBatProgInfo->isUsed = 0;
			pBatProgInfo = pBatProgInfo->next;
		}

		pBatInfo = pBatInfo->next;
	}
}

void APP_BAT_IPI_GetBatInfoFromDbs(void)
{
	BU32 i = 0;
	BU32 j = 0;
	BU32 bqId = 0;
	BU32 sortCount = 0;
	BU08 strProgSortId[10];
	SQL_API_Semaphore_Wait();

	SQL_API_Select("SELECT * FROM sortTable;");

	sortCount = SQL_API_GetTotalNum();
	APP_PRINT_INFO("SQL_API_GetTotalNum()-------------%d\n",sortCount);
	if(sortCount > CHN_MAX_SORT_COUNT)
		sortCount = CHN_MAX_SORT_COUNT;
	
	for(i = 0; i < sortCount; i++)
	{
		SQL_API_GotoRow(i);
		bqId = SQL_API_GetData(SORT_BOUQUET_ID, 0, 0,APP_DATA_TYPE_NUM);
		SQL_API_GetData(SORT_BOUQUET_NAME, g_SortInfo[j].sortName, APP_CHN_NAME_LEN,APP_DATA_TYPE_STRING);
		GUI_API_AddDecStr(bqId,10,strProgSortId);
		sprintf(g_StrSQL, "SELECT * FROM program WHERE 分类ID LIKE '%% %s %%' AND 服务类型 = %d ORDER BY 编号 ASC;",strProgSortId, APP_CHN_AV_TYPE_TV_e);
		SQL_Select(&(g_SortInfo[j].dataSort),g_StrSQL);
	//	if(g_SortInfo[j].dataSort.nResult != 0)
		{
			j++;
		}
	}
	APP_ChnDatabase_IPI_SetSortCount(j);	
	SQL_API_Semaphore_Release();
}

void APP_BAT_IPI_SaveBatInfoIntoDbs(void)
{
	BAT_Content_s batContent;
	struct batInfo_s *pBatInfo = 0;
	struct batProgInfo_s *pBatProgInfo = 0;
	BU32 serviceId = 0;
	BU32 bqId = 0;
	BU08 bqname[APP_CHN_NAME_LEN];
	BU32 i = 0;
	BU32 j = 0;	
	BU08 strAllSortId1[7 * CHN_MAX_SORT_COUNT + 1];
	BU08 strAllSortId2[7 * CHN_MAX_SORT_COUNT + 1];
	BU08 strProgSortId[10];
	BU32 count = 0;

	SQL_API_Semaphore_Wait();
	#ifndef BAT_SANMING
	SQL_API_Exec("DELETE FROM sortTable;");
	SQL_API_Exec("UPDATE program SET 分类ID = ' ';");
	#endif
	SQL_API_Semaphore_Release();
	
	pBatInfo = g_FirstBatInfo.next;
	while(pBatInfo != 0 && pBatInfo->isUsed == 1)
	{	
		if(pBatInfo->bqId != 800 && pBatInfo->bqName[0] != '\0')
		{
#ifndef BAT_SANMING
			for(i =0,j=0;i<APP_CHN_NAME_LEN;i++,j++)
			{
				if((pBatInfo->bqName)[i] == 0)
				{
					bqname[j] = (pBatInfo->bqName)[i];
					break;
				}
				else if((pBatInfo->bqName)[i] == '\'')
				{
					bqname[j] = '\'';
					j++;
					bqname[j] = '\'';
				}
				else
				{
					bqname[j] = (pBatInfo->bqName)[i];
				}
			}
			SQL_API_Semaphore_Wait();
			sprintf(g_StrSQL, "INSERT INTO sortTable(bouquetId, bouquetName) VALUES(%d, '%s');",
				pBatInfo->bqId, bqname);
			SQL_API_Exec(g_StrSQL);
			SQL_API_Semaphore_Release();
			
			if(pBatInfo->pFirstBatProgInfo == 0)
			{
				pBatInfo = pBatInfo->next;
				continue;
			}
			pBatProgInfo = pBatInfo->pFirstBatProgInfo->next;
			while(pBatProgInfo != 0 && pBatProgInfo->isUsed == 1)
			{
				serviceId = DBA_IPI_GetProgJoinServiceId(pBatProgInfo->serviceId);
				if(serviceId != 0)
				{
					SQL_API_Semaphore_Wait();
					sprintf(g_StrSQL, "SELECT 分类ID FROM program WHERE service_id = %d;", serviceId);
					SQL_API_Select(g_StrSQL);
					SQL_API_GetData(0,strAllSortId1,7 * CHN_MAX_SORT_COUNT + 1,APP_DATA_TYPE_STRING);
					if(strlen(strAllSortId1) < (7 * (CHN_MAX_SORT_COUNT - 1)))
					{
						GUI_API_AddDecStr(pBatInfo->bqId,10,strProgSortId);
						sprintf(strAllSortId2, "%s %s ", strAllSortId1, strProgSortId);
						sprintf(g_StrSQL, "UPDATE program SET 分类ID = '%s' WHERE service_id = %d;",
							strAllSortId2, serviceId);
						SQL_API_Exec(g_StrSQL);
					}
					SQL_API_Semaphore_Release();
				}
				else
				{
				}
				pBatProgInfo = pBatProgInfo->next;
			}
#endif
		}
		else if(pBatInfo->bqId == 800)
		{
			APP_BAT_IPI_SortBatInfo(pBatInfo);
			if(pBatInfo->pFirstBatProgInfo == 0)
			{
				pBatInfo = pBatInfo->next;
				continue;
			}
			pBatProgInfo = pBatInfo->pFirstBatProgInfo->next;
			while(pBatProgInfo != 0 && pBatProgInfo->isUsed == 1)
			{
				serviceId = DBA_IPI_GetProgJoinServiceId(pBatProgInfo->serviceId);
				count++;
				if(serviceId != 0)
				{
					SQL_API_Semaphore_Wait();
				#ifdef BAT_SANMING
					sprintf(g_StrSQL, "UPDATE program SET 编号 = %d WHERE service_id = %d;",pBatProgInfo->progLCN,
						serviceId);
				#else
					sprintf(g_StrSQL, "UPDATE program SET 音量补偿 = %d, 声道 = %d, 编号 = %d WHERE service_id = %d;",
						pBatProgInfo->audioOffset, pBatProgInfo->audioTrack, pBatProgInfo->progLCN,
						serviceId);
				#endif
					SQL_API_Exec(g_StrSQL);
					SQL_API_Semaphore_Release();
				}
				else
				{
				}
				pBatProgInfo = pBatProgInfo->next;
			}
		}
		pBatInfo = pBatInfo->next;
	}
	APP_PRINT_DEBUG("count2 = %d\n", count);
	//SQL_API_SaveToFlash("flash.db");
}

BU32 APP_BAT_IPI_GetQueueParserBatEnd(void)
{
	return queueParserBatEnd;
}

void APP_BAT_IPI_ClearSection(void);
void APP_BAT_IPI_StopDemuxBat(void)
{
	isStopParserBat = 1;
	APP_Parser_IPI_StopDemux(&batChannelID, &batFilterID);
	APP_BAT_IPI_ClearSection();
}

static BU32 startDemuxBatTime = 0;
void APP_BAT_IPI_InitSectionNode(void);
void APP_BAT_IPI_ClearBatInfo(void);
void APP_BAT_IPI_StartParserBatVersion(BU08 *section, BU32 channelId, BU32 filterId);


//分析bat版本信息 cwd add
void APP_BAT_IPI_StartDemuxBatVersion(void)
{
	BU08 batCoef[1] = {0x4A};
	BU08 batMask[1] = {0xFF};
	BU08 batExcl[1] = {0x00};

	struct ParserFuncNode_s parserFuncNode;
	BU32 returnValue = 0;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	if(queueParserBatEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserBatEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserBatEnd = FYF_API_queue_create("qPBatE", 1, FYF_THREAD_WAIT_FIFO);		
	}

	APP_BAT_IPI_StopDemuxBat();	
//	APP_BAT_IPI_InitSectionNode();
	//APP_BAT_IPI_ClearBatInfo();
	isStopParserBat = 0;
	//startDemuxBatTime = FYF_API_time_ms();
	APP_Parser_IPI_StartDemux(1, 0x0011, batCoef, batMask, batExcl, &batChannelID, &batFilterID);
	parserFuncNode.channelId = batChannelID;
	parserFuncNode.filterId = batFilterID;
	parserFuncNode.ParserFunc = APP_BAT_IPI_StartParserBatVersion;
	APP_IPI_AddPasFuncNode(parserFuncNode);
	
}
void APP_BAT_IPI_StartDemuxBat(void)
{
	BU08 batCoef[1] = {0x4A};
	BU08 batMask[1] = {0xFF};
	BU08 batExcl[1] = {0x00};
	struct ParserFuncNode_s parserFuncNode;
	BU32 returnValue = 0;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	if(queueParserBatEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserBatEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserBatEnd = FYF_API_queue_create("qPBatE", 1, FYF_THREAD_WAIT_FIFO);		
	}

	APP_BAT_IPI_StopDemuxBat();	
	APP_BAT_IPI_InitSectionNode();
	APP_BAT_IPI_ClearBatInfo();
	isStopParserBat = 0;
	startDemuxBatTime = FYF_API_time_ms();
	APP_Parser_IPI_StartDemux(1, 0x0011, batCoef, batMask, batExcl, &batChannelID, &batFilterID);
	parserFuncNode.channelId = batChannelID;
	parserFuncNode.filterId = batFilterID;
	parserFuncNode.ParserFunc = APP_BAT_IPI_StartParserBat;
	APP_IPI_AddPasFuncNode(parserFuncNode);
}

struct batBqSec_s
{
	BU08 isUsed;
	BU32 bqId;
	SectionNode_s *pBatFirstSectionNode;
	struct batBqSec_s *next;
};

struct batBqSec_s *g_pFirBatBqSec = 0;

void APP_BAT_IPI_ClearSection(void)
{
	struct batBqSec_s *pBatBqSec = 0;
	pBatBqSec = g_pFirBatBqSec;
	while(pBatBqSec != 0)
	{
		APP_IPI_ClearSection(&(pBatBqSec->pBatFirstSectionNode));
		pBatBqSec->isUsed = 0;
		pBatBqSec = pBatBqSec->next;
	}
}

void APP_BAT_IPI_InitSectionNode(void)
{
	struct batBqSec_s *pBatBqSec = 0;
	if(g_pFirBatBqSec == 0)
	{

		g_pFirBatBqSec = FYF_API_malloc(sizeof(struct batBqSec_s));

		if(g_pFirBatBqSec == 0)
			return;

		g_pFirBatBqSec->next = 0;
		g_pFirBatBqSec->isUsed = 0;
		g_pFirBatBqSec->pBatFirstSectionNode = 0;
		APP_IPI_InitSectionNode(&(g_pFirBatBqSec->pBatFirstSectionNode));

	}
	pBatBqSec = g_pFirBatBqSec->next;

	while(pBatBqSec != 0)
	{

		APP_IPI_InitSectionNode(&(pBatBqSec->pBatFirstSectionNode));
		pBatBqSec->isUsed = 0;
		pBatBqSec = pBatBqSec->next;
	}

}

BU08 APP_BAT_IPI_AddSectionNode(BU32 bqId, BU08 section)
{
	struct batBqSec_s *pBatBqSec = g_pFirBatBqSec->next;

	while(pBatBqSec != 0 && pBatBqSec->isUsed == 1)
	{
		if(pBatBqSec->bqId == bqId)
			break;
		pBatBqSec = pBatBqSec->next;
	}

	if(pBatBqSec != 0)
	{
		pBatBqSec->isUsed = 1;
		pBatBqSec->bqId = bqId;
		return APP_IPI_AddSection(pBatBqSec->pBatFirstSectionNode, section);
	}
	else
	{
		pBatBqSec = FYF_API_malloc(sizeof(struct batBqSec_s));
		if(pBatBqSec == 0)
			return 0;
		pBatBqSec->next = g_pFirBatBqSec->next;
		g_pFirBatBqSec->next = pBatBqSec;

		g_pFirBatBqSec->next->isUsed = 1;
		g_pFirBatBqSec->next->bqId = bqId;
		g_pFirBatBqSec->next->pBatFirstSectionNode = 0;
		APP_IPI_InitSectionNode(&(g_pFirBatBqSec->next->pBatFirstSectionNode));
		return APP_IPI_AddSection(g_pFirBatBqSec->next->pBatFirstSectionNode, section);
	}
}

BS32 APP_BAT_IPI_CheckBatVer(BU08 batVer)
{

	APP_PRINT_DEBUG("APP_BAT_IPI_CheckBatVer batver = %d %d\n", batVer, APP_SYS_IPI_GetBatVer());
	if (APP_SYS_IPI_GetBatVer() == batVer)
		return APP_SUCCESS;
	else
		return APP_FAIL;
}

void APP_BAT_IPI_StartParserBatVersion(BU08 *section, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;

	if (section[0] != 0x4A)
	{
		APP_PRINT_ERROR("APP_BAT_IPI_StartParserBat table_id = 0x%x\n", section[0]);
		return;
	}

	if (isStopParserBat)
	{
		return;
	}

	if (0 == APP_BAT_IPI_CheckBatVer((section[5] & 0x3E) >> 1))
	{
		APP_BAT_IPI_StopDemuxBat();
		msgSend.qlstWordOfMsg = 1;
		FYF_API_queue_send(APP_BAT_IPI_GetQueueParserBatEnd(), &msgSend);
		
	}
	else
	{
		APP_BAT_IPI_StopDemuxBat();
		msgSend.qlstWordOfMsg = 0;
		FYF_API_queue_send(APP_BAT_IPI_GetQueueParserBatEnd(), &msgSend);
	}
	
}

void APP_BAT_IPI_StartParserBat(BU08 *section, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;
	static BU32 costTimeCount = 0;
	static BU32 startTime = 0;
	BU32 parserStartTime = 0;

	if(startTime == 0)
		startTime = FYF_API_time_ms();

	if (section[0] != 0x4A)
	{
		APP_PRINT_ERROR("APP_BAT_IPI_StartParserBat table_id = 0x%x\n", section[0]);
		return;
	}

	if (isStopParserBat)
	{
		return;
	}
	APP_PRINT_DEBUG("bqid = %d, sectionid = %d\n", (section[3] << 8) | section[4], section[6]);
	if(1 == APP_BAT_IPI_AddSectionNode((section[3] << 8) | section[4], section[6]))
	{
		APP_PRINT_DEBUG("parse success bqid = %d, sectionid = %d\n", (section[3] << 8) | section[4], section[6]);
		parserStartTime = FYF_API_time_ms();
		APP_BAT_IPI_ParserBat(section, APP_BAT_IPI_SaveBatInfo);
		costTimeCount = costTimeCount + FYF_API_time_ms() - parserStartTime;
	}
	
	if(startDemuxBatTime != 0 && ((FYF_API_time_ms() - startDemuxBatTime) > 6000))
	{
		startDemuxBatTime = 0;
		APP_BAT_IPI_StopDemuxBat();
//		APP_BAT_IPI_HandleAllBatInfo();
		msgSend.qlstWordOfMsg = 1;
		FYF_API_queue_send(APP_BAT_IPI_GetQueueParserBatEnd(), &msgSend);
	}
}




void APP_BAT_IPI_BatBootupProc(void)
{
	FYF_QUEUE_MESSAGE msgRecv;
	msgRecv.qlstWordOfMsg = 0;
	
	APP_BAT_IPI_StartDemuxBat();
	FYF_API_queue_recv(APP_BAT_IPI_GetQueueParserBatEnd(), &msgRecv, 6000);
	if(msgRecv.qlstWordOfMsg == 0)
	{
		APP_BAT_IPI_StopDemuxBat();
	}
	APP_BAT_IPI_SaveBatInfoIntoDbs();
	SQL_API_SaveToFlash("flash.db");
	APP_BAT_IPI_GetBatInfoFromDbs();
	
}





