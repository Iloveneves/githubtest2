#include <stdio.h>
#include "app_common.h"

//#define SANMING_VERSION

struct ChnInfoList_s
{
	BU32 count;//可以存储节目总数
	BU32 progCount;//存储的节目总数
	BU08 isNeedReCreate;
	APP_CHN_GROUP_TYPE_E groupType;
	ChnDataBase_ChnInfo_S *pProgInfoList;
};


typedef struct _ChnDataBase_ChnList_S
{
	BU32 u32Count;
	BU32 u32ProgCount;
	APP_CHN_GROUP_TYPE_E GroupType;
	ChnDataBase_ChnInfo_S *pChnInfo;
}ChnDataBase_ChnList_S;


struct _CHN_BACK_S
{
	APP_CHN_AV_TYPE_E avtype;
	BU16 chn;
};
struct _CHN_BACK_S g_fullscreen_back_s[2] =
{	
	APP_CHN_AV_TYPE_TV_e,0,
	APP_CHN_AV_TYPE_TV_e,1,
};

extern BU32 stopParserTable;
extern BS08	g_StrSQL[APP_STR_SQL_LENGTH];
extern BU32	semaphoreChnDatabase;



BU32 g_AllTypeChnNo[(BU08)APP_CHN_AV_TYPE_RD_e + 1][(BU08)APP_CHN_GROUP_TYPE_EDIT_e + 1];
struct sortInfo_s g_SortInfo[CHN_MAX_SORT_COUNT];

static BU32 sortCount = 0;
//ChnDataBase_ChnInfo_S*	g_pProgInfoList[APP_CHN_AV_TYPE_RD_e + 1][(BU08)APP_CHN_GROUP_TYPE_EDIT_e + 1] = {0};//当前节目列表
ChnDataBase_ChnList_S*  g_pChnInfoList[APP_CHN_AV_TYPE_RD_e + 1][(BU08)APP_CHN_GROUP_TYPE_EDIT_e + 1] = {0};
BS08 g_sortname[][APP_CHN_NAME_LEN] = 
{
	"所有节目",
	"喜爱节目",
	"新闻时事",
	"影视戏剧",
	"综艺表演",
	"体育竞赛",
	"少年儿童",
	"音乐舞蹈",
	"文化艺术",
	"社会经济",
	"教育科学",
	"休闲娱乐",
	"其他节目",
};

BU08	isNeedReCreateChnList = 1;

#ifdef  APP_CONFIG_ZERO_CHANNEL
static BU16 g_search_ch0Flag=0;
#else
static BU16 g_search_ch0Flag=1;
#endif


/*-------------------------------------------------------------------------------

	Description:Sem for Chn List

	Parameters:

-------------------------------------------------------------------------------*/
void APP_ChnDatabase_API_Protect(void)
{
	if(semaphoreChnDatabase)
	{
		FYF_API_semaphore_wait (semaphoreChnDatabase,FYF_WAIT_FOREVER);
	}
}
void APP_ChnDatabase_API_UnProtect(void)
{	
	if(semaphoreChnDatabase)
	{		
		FYF_API_semaphore_release (semaphoreChnDatabase);		
	}
}


/*-------------------------------------------------------------------------------

	Description:设置当前的节目类型

	Parameters:当不同类型的节目进行切换时，要调用该函数，比如从电视到广播或者从广播到电视
	Note:当该列表个数为0时，则返回错误，节目类型还是为当前的节目类型

-------------------------------------------------------------------------------*/

BU32 APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType)
{
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnType error avtype = %d\n", AvType);
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnType error GroupType = %d\n", GroupType);
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[AvType][GroupType];
#if 1
	if (pChnInfoList == NULL)
	{
		return APP_FAIL;
	}
#else
	if (pChnInfoList == NULL || pChnInfoList->u32ProgCount == 0)
	{
		return APP_FAIL;
	}
#endif

	app_system_data.g_AvType = (BU08)AvType;
	app_system_data.g_GroupType = (BU08)GroupType;

	return APP_SUCCESS;
}

APP_CHN_AV_TYPE_E APP_ChnDatabase_API_GetCurChnType()
{
	return app_system_data.g_AvType;
}


APP_CHN_GROUP_TYPE_E APP_ChnDatabase_API_GetCurGroupType()
{
	return app_system_data.g_GroupType;
}


/*-------------------------------------------------------------------------------

	Description:调换2个节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_SwapChn(BU16 ProNo1, BU16 ProNo2)
{
	BU32					service1;
	BU32					service2;
	BU32					index1;
	BU32					index2;
	ChnDataBase_ChnInfo_S	tempChnInfo;

	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[app_system_data.g_AvType][app_system_data.g_GroupType];

	if(ProNo1 == ProNo2)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SwapChn ProNo1 == ProNo2\n");	
		return APP_FAIL;
	}

	if (pChnInfoList == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SwapChn pChnInfoList = null!\n");
		return APP_FAIL;
	}

	APP_ChnDatabase_API_Protect();

	if(ProNo1 >= pChnInfoList->u32ProgCount)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SwapChn ProNo1 == ProNo2\n");
		APP_ChnDatabase_API_UnProtect();
		return APP_FAIL;		
	}
	if(ProNo2 >= pChnInfoList->u32ProgCount)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SwapChn ProNo1 == ProNo2\n");
		APP_ChnDatabase_API_UnProtect();
		return APP_FAIL;		
	}
	if(pChnInfoList->pChnInfo == 0)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SwapChn g_pProgInfoList == 0\n");	
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}

	service1 = pChnInfoList->pChnInfo[ProNo1].m_ProgId;
	service2 = pChnInfoList->pChnInfo[ProNo2].m_ProgId;
	
	memcpy(&tempChnInfo, &pChnInfoList->pChnInfo[ProNo1], sizeof(ChnDataBase_ChnInfo_S));	
	memcpy(&pChnInfoList->pChnInfo[ProNo1], &pChnInfoList->pChnInfo[ProNo2], sizeof(ChnDataBase_ChnInfo_S));
	memcpy(&pChnInfoList->pChnInfo[ProNo2], &tempChnInfo, sizeof(ChnDataBase_ChnInfo_S));
	APP_ChnDatabase_API_UnProtect();
	
	SQL_API_Semaphore_Wait();
	
	sprintf(g_StrSQL, "SELECT 编号 FROM program WHERE service_id = %d;", service1);
	SQL_API_Select(g_StrSQL);
	index1 = SQL_API_GetData(0, 0, 0, APP_DATA_TYPE_NUM);

	sprintf(g_StrSQL, "SELECT 编号 FROM program WHERE service_id = %d;", service2);
	SQL_API_Select(g_StrSQL);
	index2 = SQL_API_GetData(0, 0, 0, APP_DATA_TYPE_NUM);
	
	sprintf(g_StrSQL, "UPDATE program SET 编号 = %d WHERE service_id = %d;", index2, service1);
	SQL_API_Exec(g_StrSQL);
	sprintf(g_StrSQL, "UPDATE program SET 编号 = %d WHERE service_id = %d;", index1, service2);
	SQL_API_Exec(g_StrSQL);

	SQL_API_Semaphore_Release();
	

	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:根据标志删除节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_DelChnByTag(void)
{
	BU32	i = 0;

	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[app_system_data.g_AvType][app_system_data.g_GroupType];
	APP_ChnDatabase_API_Protect();
	if(pChnInfoList == NULL || pChnInfoList->pChnInfo == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_DelChnByTag g_pProgInfoList == 0\n");	
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}
	
	SQL_API_Semaphore_Wait();
	for (i = 0; i < pChnInfoList->u32ProgCount; i++)
	{
		if (pChnInfoList->pChnInfo[i].m_Del == 1)
		{
			sprintf(g_StrSQL, "DELETE FROM program WHERE service_id = %d;", pChnInfoList->pChnInfo[i].m_ProgId);
			SQL_API_Exec(g_StrSQL);
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 编号 = %d;", pChnInfoList->pChnInfo[i].m_ProgId);
			SQL_API_Exec(g_StrSQL);
		}
	}
	SQL_API_Semaphore_Release();
	APP_ChnDatabase_API_UnProtect();
	
}

/*-------------------------------------------------------------------------------

	Description:获得当前节目类型的节目数

	Parameters:

-------------------------------------------------------------------------------*/
BU16 APP_ChnDatabase_API_GetCurTypeChnNum(void)
{
	
	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[app_system_data.g_AvType][app_system_data.g_GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetCurTypeChnNum g_pProgInfoList == 0\n");
		return APP_FAIL;		
	}
	return pChnInfoList->u32ProgCount;
}

/*-------------------------------------------------------------------------------

	Description:获得某种节目类型的节目数

	Parameters:
	比如在电视节目类型的状态下想获取广播节目的节目数

-------------------------------------------------------------------------------*/
BU16 APP_ChnDatabase_API_GetTypeChnNum(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType)
{
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetTypeChnNum error avtype = %d\n", AvType);
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetTypeChnNum error GroupType = %d\n", GroupType);
		return APP_FAIL;
	}
	
	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetTypeChnNum g_pProgInfoList == 0\n");
		return APP_FAIL;		
	}
	return pChnInfoList->u32ProgCount;
}


/*-------------------------------------------------------------------------------

	Description:得到频道的基本信息

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_GetChnInfo(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 ChnNo, ChnDataBase_ChnInfo_S* ChnInfo)
{
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	APP_ChnDatabase_API_Protect ();
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo error avtype = %d\n", AvType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}
	if(ChnNo >= pChnInfoList->u32ProgCount)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo chnNo >= g_ChnNum\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (ChnInfo == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo chnInfo == 0\n");	
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	memcpy(ChnInfo, &pChnInfoList->pChnInfo[ChnNo], sizeof(ChnDataBase_ChnInfo_S));
	APP_ChnDatabase_API_UnProtect ();
	return APP_SUCCESS;
}


/*-------------------------------------------------------------------------------

	Description:通过serviceId获取节目信息

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_GetChnInfoByService(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 ServiceId, ChnDataBase_ChnInfo_S* ChnInfo)
{
	BU32	i = 0;
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	
	if(ChnInfo == 0)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfoByService chnInfo == 0\n");
		return APP_FAIL;
	}

	APP_ChnDatabase_API_Protect ();
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfoByService error avtype = %d\n", AvType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfoByService error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfoByService g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}
	
	for (i = 0; i < pChnInfoList->u32ProgCount; i++)
	{
		if (ServiceId == pChnInfoList->pChnInfo[i].m_ProgId)
		{
			memcpy(ChnInfo, &pChnInfoList->pChnInfo[i], sizeof(ChnDataBase_ChnInfo_S));
//			chnInfo->m_ChnId = i;
			if(pChnInfoList->pChnInfo[i].m_ChnIndex != i)
			{
				FYF_API_printf("!!!!!!!!!!!!!!!!!!!!APP_ChnDatabase_API_GetChnInfoByService\n");
			}
			ChnInfo->m_ChnIndex = pChnInfoList->pChnInfo[i].m_ChnIndex;
			APP_ChnDatabase_API_UnProtect();
			return APP_SUCCESS;
		}
	}
	APP_ChnDatabase_API_UnProtect();
	memset(ChnInfo, 0, sizeof(ChnDataBase_ChnInfo_S));
	return APP_FAIL;	
}

/*-------------------------------------------------------------------------------

	Description:设置节目基本信息

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_SetChnInfo(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 ChnNo,const ChnDataBase_ChnInfo_S* ChnInfo)
{
	BU32	no;
	BU08	audioOffset = 0;
	
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	
	if(ChnInfo == 0)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfo chnInfo == 0\n");
		return APP_FAIL;
	}

	APP_ChnDatabase_API_Protect ();
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfo error avtype = %d\n", AvType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfo error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfo g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}
	if(ChnNo >= pChnInfoList->u32ProgCount)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfo chnNo >= g_ChnNum\n");
		APP_ChnDatabase_API_UnProtect ();		
		return APP_FAIL;
	}

	if (ChnNo == 0xffff)
	{
		no = ChnInfo->m_ChnIndex;
	}
	else
	{
		no = pChnInfoList->pChnInfo[ChnNo].m_ProgId;
	}

	pChnInfoList->pChnInfo[ChnNo].m_Fav 	= ChnInfo->m_Fav;
	pChnInfoList->pChnInfo[ChnNo].m_Lock 	= ChnInfo->m_Lock;
	pChnInfoList->pChnInfo[ChnNo].m_Ca 		= ChnInfo->m_Ca;
	pChnInfoList->pChnInfo[ChnNo].m_Skip 	= ChnInfo->m_Skip;
	pChnInfoList->pChnInfo[ChnNo].m_Del 	= ChnInfo->m_Del;
	pChnInfoList->pChnInfo[ChnNo].m_AudioChn = ChnInfo->m_AudioChn;
	pChnInfoList->pChnInfo[ChnNo].m_AudioOffset = ChnInfo->m_AudioOffset;
	pChnInfoList->pChnInfo[ChnNo].m_AudioVolume = ChnInfo->m_AudioVolume;

	audioOffset = (BU08) ChnInfo->m_AudioOffset;
	APP_ChnDatabase_API_UnProtect ();
	
	SQL_API_Semaphore_Wait();	
	sprintf(g_StrSQL,
		"UPDATE program SET 喜爱 = %d, 加锁 = %d, 加密 = %d, 跳过 = %d, 删除 = %d, 声道 = %d, 音量补偿 = %d, 音量大小 = %d WHERE service_id = %d;",
		ChnInfo->m_Fav, ChnInfo->m_Lock, ChnInfo->m_Ca, ChnInfo->m_Skip, ChnInfo->m_Del,
		ChnInfo->m_AudioChn, audioOffset, ChnInfo->m_AudioVolume,no);
	SQL_API_Exec(g_StrSQL);
	SQL_API_Semaphore_Release();
	return APP_SUCCESS;
}

BU32 APP_ChnDatabase_API_SetChnInfoByServiceid(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 serviceId,const ChnDataBase_ChnInfo_S* ChnInfo)
{
	BU32 i = 0;

	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	
	if(ChnInfo == 0)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfoByServiceid chnInfo == 0\n");
		return APP_FAIL;
	}

	APP_ChnDatabase_API_Protect ();
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfoByServiceid error avtype = %d\n", AvType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfoByServiceid error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnInfoByServiceid g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}

	for (i = 0; i < pChnInfoList->u32ProgCount; i++)
	{
		if (serviceId == pChnInfoList->pChnInfo[i].m_ProgId)
		{
			memcpy(&pChnInfoList->pChnInfo[i], ChnInfo, sizeof(ChnDataBase_ChnInfo_S));

			APP_ChnDatabase_API_UnProtect();
			return APP_SUCCESS;
		}
	}
	APP_ChnDatabase_API_UnProtect();
	return APP_FAIL;

	
}


BU32 APP_ChnDatabase_API_SetChnNameByServiceid(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU32 serviceId, BU08 *ChnName)
{
	BU32 i = 0;
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	
	if (ChnName == 0)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnNameByServiceid chnInfo == 0\n");
		return APP_FAIL;
	}

	APP_ChnDatabase_API_Protect ();
	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnNameByServiceid error avtype = %d\n", AvType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnNameByServiceid error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetChnNameByServiceid g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}


	for (i = 0; i < pChnInfoList->u32ProgCount; i++)
	{
		if (serviceId == pChnInfoList->pChnInfo[i].m_ProgId)
		{
			//memcpy(&g_pProgInfoList[i], chnInfo, sizeof(ChnDataBase_ChnInfo_S));
			strcpy(pChnInfoList->pChnInfo[i].m_ChnName, ChnName);
			APP_ChnDatabase_API_UnProtect();
			return APP_SUCCESS;
		}
	}
	APP_ChnDatabase_API_UnProtect();
	return APP_FAIL;

	
}
/*-------------------------------------------------------------------------------

	Description:获得当前频道NO

	Parameters:

-------------------------------------------------------------------------------*/
BU16 APP_ChnDatabase_API_GetCurChnNO(void)
{
	app_system_data.g_CurChnNo = g_AllTypeChnNo[app_system_data.g_AvType ][app_system_data.g_GroupType];
	return (BU16) app_system_data.g_CurChnNo;
}


BU16 APP_ChnDatabase_API_GetNextChnNO()
{
	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[app_system_data.g_AvType][app_system_data.g_GroupType];
	BU32 u32ChnNum = pChnInfoList->u32ProgCount;
	BU08 flag = 0;
	BS32 i;
	BU32 u32CurNum = APP_ChnDatabase_API_GetCurChnNO();
	BU16 u16ChnNO = 0;
	if(pChnInfoList == NULL || pChnInfoList->pChnInfo == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnNO g_pProgInfoList == 0\n");
		return APP_FAIL;		
	}

	for (i = u32CurNum + 1; i < u32ChnNum; i++)
	{
		if (pChnInfoList->pChnInfo[i].m_Skip == 0)
		{
			flag = 1;
			u16ChnNO = i;
			break;
		}
	}
	
	if (flag == 0)
	{
		for (i = 0; i < u32CurNum; i++)	
		{
			if (pChnInfoList->pChnInfo[i].m_Skip == 0)
			{
				flag = 1;
				u16ChnNO = i;
				break;
			}
		}
	}

	if (flag)
	{
		return u16ChnNO;
	}
	else
	{
		return u32CurNum;
	}
		
}

BU16 APP_ChnDatabase_API_GetPrevChnNO()
{
	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[app_system_data.g_AvType][app_system_data.g_GroupType];
	BU32 u32ChnNum = pChnInfoList->u32ProgCount;
	BU08 flag = 0;
	BS32 i;
	BU32 u32CurNum = APP_ChnDatabase_API_GetCurChnNO();
	BU16 u16ChnNO = 0;
	if(pChnInfoList == NULL || pChnInfoList->pChnInfo == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnNO g_pProgInfoList == 0\n");
		return APP_FAIL;		
	}

	for (i = u32CurNum - 1; i >= 0; i--)
	{
		if (pChnInfoList->pChnInfo[i].m_Skip == 0)
		{
			flag = 1;
			u16ChnNO = i;
			break;
		}
	}
	
	if (flag == 0)
	{
		for (i = u32ChnNum - 1; i > u32CurNum; i--)	
		{
			if (pChnInfoList->pChnInfo[i].m_Skip == 0)
			{
				flag = 1;
				u16ChnNO = i;
				break;
			}
		}
	}

	if (flag)
	{
		return u16ChnNO;
	}
	else
	{
		return u32CurNum;
	}

}


/*-------------------------------------------------------------------------------

	Description:设置当前节目号

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_SetCurChnNO(BU32 curChnNO)
{
	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[app_system_data.g_AvType][app_system_data.g_GroupType];

	if(pChnInfoList == NULL || pChnInfoList->pChnInfo == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnNO g_pProgInfoList == 0\n");
		return APP_FAIL;		
	}
	
	app_system_data.g_CurChnNo = curChnNO;

	if(app_system_data.g_CurChnNo >= pChnInfoList->u32ProgCount && pChnInfoList->u32ProgCount != 0)
	{
		app_system_data.g_CurChnNo = pChnInfoList->u32ProgCount - 1;
	}

	if(app_system_data.g_AvType > (BU08)APP_CHN_AV_TYPE_RD_e || app_system_data.g_GroupType > (BU08)APP_CHN_GROUP_TYPE_EDIT_e)
	{
		return APP_FAIL;
	}
	
	g_AllTypeChnNo[app_system_data.g_AvType ][app_system_data.g_GroupType] = app_system_data.g_CurChnNo;
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:设置所有电视节目类型的当前节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_SetTypeAllCurChnNO(BU32 curChnNO)
{
	ChnDataBase_ChnList_S *pChnInfoList = g_pChnInfoList[APP_CHN_AV_TYPE_TV_e][APP_CHN_GROUP_TYPE_ALL_e];

	if(pChnInfoList == NULL || pChnInfoList->pChnInfo == NULL)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnNO g_pProgInfoList == 0\n");
		return APP_FAIL;		
	}

	if(curChnNO >= pChnInfoList->u32ProgCount && pChnInfoList->u32ProgCount != 0)
	{
		curChnNO = pChnInfoList->u32ProgCount - 1;
	}
	
	g_AllTypeChnNo[app_system_data.g_AvType][APP_CHN_GROUP_TYPE_ALL_e] = curChnNO;
	return APP_SUCCESS;
}
BU32 APP_ChnDatabase_API_ClearAllTypeChnNO(void)
{
	BU32 type=0,group=0;

	for(type=0;type<=APP_CHN_AV_TYPE_RD_e;type++)
	{
		for(group=0;group<=APP_CHN_GROUP_TYPE_EDIT_e;group++)
		{
			g_AllTypeChnNo[type][group]=0;
		}
	}
		
}

/*-------------------------------------------------------------------------------

	Description:将节目库的改动保存到Flash中

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_UpDataToFlash(BU32 bat)
{
	stopParserTable = 1;
	SQL_API_SaveToFlash("flash.db");
	if(bat == 1)
	{
		APP_BAT_IPI_GetBatInfoFromDbs();
	}
	stopParserTable = 0;
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_LoadFromFlash(void)
{
	SQL_API_LoadFromFlash("flash.db");
	//APP_IPI_SetTagNeedReCreateChnList();

	APP_EPG_IPI_InitReserve();
	APP_BAT_IPI_GetBatInfoFromDbs();
	return APP_SUCCESS;
}


BU32 APP_ChnDatabase_API_GetSortTypeFlag(BU16 chnNO, APP_CHN_GROUP_TYPE_E GroupType, BU08 *flag)
{
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	APP_ChnDatabase_API_Protect ();

	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[APP_CHN_AV_TYPE_TV_e][APP_CHN_GROUP_TYPE_ALL_e];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}
	
	if (chnNO >= pChnInfoList->u32ProgCount)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo chnNo >= g_ChnNum\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	
	*flag = (pChnInfoList->pChnInfo[chnNO].m_SortFlag >> GroupType) & 0x1;
	printf("*flag = %d\n", *flag);

	APP_ChnDatabase_API_UnProtect ();
	return APP_SUCCESS;
}

BU32 APP_ChnDatabase_API_SetSortTypeFlag(BU16 chnNO, APP_CHN_GROUP_TYPE_E GroupType, BU08 flag)
{
	ChnDataBase_ChnList_S *pChnInfoList = NULL;
	BU32 serviceId;
	BU08 strAllSortId1[8 * CHN_MAX_SORT_COUNT + 1];
	BU08 strAllSortId2[8 * CHN_MAX_SORT_COUNT + 1];
	APP_ChnDatabase_API_Protect ();
	BU08 *p = NULL;

	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo error GroupType = %d\n", GroupType);
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}

	pChnInfoList = g_pChnInfoList[APP_CHN_AV_TYPE_TV_e][APP_CHN_GROUP_TYPE_ALL_e];
	if(pChnInfoList == NULL || (pChnInfoList->u32ProgCount > 0 && pChnInfoList->pChnInfo == NULL))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo g_pProgInfoList == 0\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;		
	}
	if(chnNO >= pChnInfoList->u32ProgCount)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo chnNo >= g_ChnNum\n");
		APP_ChnDatabase_API_UnProtect ();
		return APP_FAIL;
	}
	if (flag)
		pChnInfoList->pChnInfo[chnNO].m_SortFlag |= (flag << GroupType);
	else
		pChnInfoList->pChnInfo[chnNO].m_SortFlag &= (~(1 << GroupType));
	serviceId = pChnInfoList->pChnInfo[chnNO].m_ProgId;
	APP_ChnDatabase_API_UnProtect ();

	SQL_API_Semaphore_Wait();
	sprintf(g_StrSQL, "SELECT 分类ID FROM program WHERE service_id = %d;", serviceId);
	SQL_API_Select(g_StrSQL);
	SQL_API_GetData(0,strAllSortId1,8 * CHN_MAX_SORT_COUNT,APP_DATA_TYPE_STRING);
	APP_PRINT_DEBUG("strAllSortId1 = %s, len = %d\n", strAllSortId1, strlen(strAllSortId1));
	if (flag == 1)
	{
		if(strlen(strAllSortId1) < (8 * (CHN_MAX_SORT_COUNT - 1)))
		{
			if (strstr(strAllSortId1, g_sortname[GroupType]) == NULL)
			{
				sprintf(strAllSortId2, "%s %s ", strAllSortId1, g_sortname[GroupType]);
				APP_PRINT_DEBUG("strAllSortId2 = %s, len = %d\n", strAllSortId2, strlen(strAllSortId2));
				sprintf(g_StrSQL, "UPDATE program SET 分类ID = '%s' WHERE service_id = %d;",
					strAllSortId2, serviceId);
				SQL_API_Exec(g_StrSQL);
			}
		}
	}
	else
	{
		if(strlen(strAllSortId1) < (8 * (CHN_MAX_SORT_COUNT - 1)))
		{
			p = strstr(strAllSortId1, g_sortname[GroupType]);
			if ( p != NULL)
			{
				APP_PRINT_DEBUG("p = %s, len = %d\n", p, strlen(p));
				*(p - 1) = 0;
				sprintf(strAllSortId2, "%s%s", strAllSortId1, p + strlen(g_sortname[GroupType]) + 1);
				APP_PRINT_DEBUG("strAllSortId2 = %s, len = %d\n", strAllSortId2, strlen(strAllSortId2));
				sprintf(g_StrSQL, "UPDATE program SET 分类ID = '%s' WHERE service_id = %d;",
					strAllSortId2, serviceId);
				SQL_API_Exec(g_StrSQL);
			}
		}

	}
	SQL_API_Semaphore_Release();
	
	return APP_SUCCESS;
}
/*-------------------------------------------------------------------------------

	Description:重新创建节目列表

	Parameters:当刚开机，进行节目删除，或者恢复出厂设置，或者重新搜台的时候都要调用该函数
		每次都是重新创建，所以不要频繁地调用，而是适当的时候调用
-------------------------------------------------------------------------------*/

BU32 APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType)
{
	static  ChnDataBase_ChnList_S ListTv 		= {0, 0, APP_CHN_GROUP_TYPE_ALL_e, 0};
	static  ChnDataBase_ChnList_S ListTvFav     = {0, 0, APP_CHN_GROUP_TYPE_FAV_e, 0};
	static  ChnDataBase_ChnList_S ListRd 		= {0, 0, APP_CHN_GROUP_TYPE_ALL_e, 0};
	static  ChnDataBase_ChnList_S ListRdFav 	= {0, 0, APP_CHN_GROUP_TYPE_FAV_e, 0};
	static  ChnDataBase_ChnList_S ListSort[APP_CHN_GROUP_TYPE_EDIT_e];
	static BU08 u8ChnListInit = APP_FALSE;
	ChnDataBase_ChnList_S *pChnInfoList;
	BU08 isFoundProg0 = 0;
	BU32 i = 0, j;
	BU08 strAllSortId[8 * CHN_MAX_SORT_COUNT + 1];
	if (!u8ChnListInit)
	{
		g_pChnInfoList[APP_CHN_AV_TYPE_TV_e][APP_CHN_GROUP_TYPE_ALL_e] = &ListTv;
		g_pChnInfoList[APP_CHN_AV_TYPE_TV_e][APP_CHN_GROUP_TYPE_FAV_e] = &ListTvFav;
		g_pChnInfoList[APP_CHN_AV_TYPE_RD_e][APP_CHN_GROUP_TYPE_ALL_e] = &ListRd;
		g_pChnInfoList[APP_CHN_AV_TYPE_RD_e][APP_CHN_GROUP_TYPE_FAV_e] = &ListRdFav;
		for (i = APP_CHN_GROUP_TYPE_1_e; i < APP_CHN_GROUP_TYPE_26_e; i++)
		{
			ListSort[i].u32Count = 0;
			ListSort[i].u32ProgCount = 0;
			ListSort[i].GroupType = i;
			ListSort[i].pChnInfo = NULL;
			g_pChnInfoList[APP_CHN_AV_TYPE_TV_e][i] = &ListSort[i];
		}
		u8ChnListInit = APP_TRUE;
	}

	if (AvType == APP_CHN_AV_TYPE_NULL || AvType > APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_ReCreatChnList error avtype = %d\n", AvType);
		return APP_FAIL;
	}
	if (GroupType > APP_CHN_GROUP_TYPE_EDIT_e)
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_ReCreatChnList error GroupType = %d\n", GroupType);
		return APP_FAIL;
	}

	APP_PRINT_DEBUG("avType %d, Group %d \n", AvType, GroupType);
	pChnInfoList = g_pChnInfoList[AvType][GroupType];
	
	
	APP_ChnDatabase_API_Protect ();

	SQL_API_Semaphore_Wait();
	switch (GroupType)
	{
	case APP_CHN_GROUP_TYPE_FAV_e:
		sprintf(g_StrSQL, "select * from program where 喜爱 = 1 and 跳过 = 0 and 服务类型 = %d ORDER BY 编号 ASC, service_id ASC;", AvType);
		break;
	case APP_CHN_GROUP_TYPE_ALL_e:
		sprintf(g_StrSQL, "select * from program where 服务类型 = %d ORDER BY 编号 ASC, service_id ASC;", AvType);
		break;
	case APP_CHN_GROUP_TYPE_EDIT_e:
		sprintf(g_StrSQL, "select * from program where 跳过 = 0 and 服务类型 = %d ORDER BY 编号 ASC, service_id ASC;", AvType);
		break;
	default:
		if (GroupType >= APP_CHN_GROUP_TYPE_1_e && GroupType < APP_CHN_GROUP_TYPE_1_e + sortCount)
		{
			sprintf(g_StrSQL, "SELECT * FROM program WHERE 分类ID LIKE '%% %s %%' AND 服务类型 = %d ORDER BY 编号 ASC;", g_SortInfo[GroupType - APP_CHN_GROUP_TYPE_1_e].sortName, APP_CHN_AV_TYPE_TV_e);
		}
		else
		{
			pChnInfoList->u32ProgCount = 0; 
			if (g_AllTypeChnNo[AvType][GroupType] >= pChnInfoList->u32ProgCount &&  pChnInfoList->u32ProgCount != 0)
			{
				g_AllTypeChnNo[AvType][GroupType] = pChnInfoList->u32ProgCount -1;
			}
			SQL_API_Semaphore_Release();
			APP_ChnDatabase_API_UnProtect();
			return APP_SUCCESS;
			
		}
		break;
	}

	SQL_API_Select(g_StrSQL);
//	APP_PRINT_DEBUG("SQL_API_GetTotalNum()1 = %d \n", SQL_API_GetTotalNum());
	if(pChnInfoList->u32Count < SQL_API_GetTotalNum())
	{
//		APP_PRINT_DEBUG("SQL_API_GetTotalNum()2 = %d \n", SQL_API_GetTotalNum());
		if(pChnInfoList->pChnInfo != 0)
		{
		//	APP_PRINT_DEBUG("SQL_API_GetTotalNum()3 = %d \n", SQL_API_GetTotalNum());
			FYF_API_free(pChnInfoList->pChnInfo);
			pChnInfoList->pChnInfo= 0;
		}
//		APP_PRINT_DEBUG("SQL_API_GetTotalNum()4 = %d \n", SQL_API_GetTotalNum());
		pChnInfoList->pChnInfo = FYF_API_malloc((BS32)((SQL_API_GetTotalNum() + 21) * sizeof(ChnDataBase_ChnInfo_S)));		
		if(pChnInfoList->pChnInfo == 0)
		{
//			APP_PRINT_DEBUG("SQL_API_GetTotalNum()5 = %d \n", SQL_API_GetTotalNum());
			pChnInfoList->u32Count = 0;
			pChnInfoList->u32ProgCount = 0;
			SQL_API_Semaphore_Release();
			APP_ChnDatabase_API_UnProtect();
			return APP_FAIL;
		}
		pChnInfoList->u32Count = SQL_API_GetTotalNum() + 20;
	}
//	APP_PRINT_DEBUG("SQL_API_GetTotalNum()2 = %d \n", SQL_API_GetTotalNum());
#ifdef APP_CONFIG_ZERO_CHANNEL
	for (i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		BU32 iSave = 0;
		SQL_API_GotoRow(i);

		pChnInfoList->pChnInfo[i].m_ProgId 	= SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		/*发现0频道，所有频道向后移动一位*/
		if(SeparateSevId(pChnInfoList->pChnInfo[i].m_ProgId) == CHN_ZERO_PROG_SERVICE_ID && isFoundProg0 == 0)
		{
			BS32 j = 0;
			for(j = i - 1; j >= 0; j--)
			{
				memcpy(&pChnInfoList->pChnInfo[j + 1], &pChnInfoList->pChnInfo[j], sizeof(ChnDataBase_ChnInfo_S));
				pChnInfoList->pChnInfo[j + 1].m_ChnIndex = pChnInfoList->pChnInfo[j + 1].m_ChnIndex + 1;
			}
			iSave = i;
			i = 0;
			isFoundProg0 = 1;
		}
		pChnInfoList->pChnInfo[i].m_ProgId 	= SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_ChnIndex 		= i;
		pChnInfoList->pChnInfo[i].m_ProgLCN = (BU32)SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData((BU32)PROGRAM_CHN_NAME, pChnInfoList->pChnInfo[i].m_ChnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
		pChnInfoList->pChnInfo[i].m_Ca 		= (BU08) SQL_API_GetData((BU32)PROGRAM_CA, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_Lock 		= (BU08) SQL_API_GetData((BU32)PROGRAM_LOCK, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_Fav 		= (BU08) SQL_API_GetData((BU32)PROGRAM_FAV, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_Del 		= (BU08) SQL_API_GetData((BU32)PROGRAM_DEL, 0, 0, APP_DATA_TYPE_NUM);		
		pChnInfoList->pChnInfo[i].m_Skip 		= (BU08) SQL_API_GetData((BU32)PROGRAM_SKIP, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioChn 	= (BU08) SQL_API_GetData((BU32)PROGRAM_TRACK, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioOffset = (BS08) SQL_API_GetData((BU32)PROGRAM_AUDIO_OFFSET, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_FreqMHz 	= (BU16) SQL_API_GetData((BU32)PROGRAM_FREQ, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_QAM 		= (BU08) SQL_API_GetData((BU32)PROGRAM_QAM, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_SymKbps 	= (BU16) SQL_API_GetData((BU32)PROGRAM_SYMKBPS, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_style 		= (BU08) SQL_API_GetData((BU32)PROGRAM_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_VideoPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_PID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_PID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_PcrPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_PCRPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_EcmPID 	= (BU16) SQL_API_GetData((BU32)PROGRAM_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_audioEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_videoEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_CAT_Sys_ID = (BU16) SQL_API_GetData((BU32)PROGRAM_SCAT_SYS_ID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_TsId       = (BU32) SQL_API_GetData((BU32)PROGRAM_TSID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioVolume = (BS08) SQL_API_GetData((BU32)PROGRAM_AUDIO_VOLUME, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData(PROGRAM_SORT_ID, strAllSortId, 8 * CHN_MAX_SORT_COUNT, APP_DATA_TYPE_STRING);
		pChnInfoList->pChnInfo[i].m_SortFlag = 0x01;
		
		for (j = 0; j < sortCount; j++)
		{
			if (strstr(strAllSortId, g_SortInfo[j].sortName) != NULL)
			{
				pChnInfoList->pChnInfo[i].m_SortFlag |= (1 << j);
			}
		}
		pChnInfoList->pChnInfo[i].m_pmtpid     = (BU16) SQL_API_GetData((BU32)PROGRAM_PID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_OriNetId   = (BU16) SQL_API_GetData((BU32)PROGRAM_ORI_NET_ID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_VideoType  = (BU08) SQL_API_GetData((BU32)PROGRAM_VIDEO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioType  = (BU08) SQL_API_GetData((BU32)PROGRAM_AUDIO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		if(iSave != 0)
		{
			i = iSave;
		}
	}
#else
	for (i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		SQL_API_GotoRow(i);

		pChnInfoList->pChnInfo[i].m_ProgId 	= (BU32) SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_ChnIndex 		=  i;//(BU32) SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_ProgLCN = (BU32)SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData((BU32)PROGRAM_CHN_NAME, pChnInfoList->pChnInfo[i].m_ChnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
		pChnInfoList->pChnInfo[i].m_Ca 		= (BU08) SQL_API_GetData((BU32)PROGRAM_CA, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_Lock 		= (BU08) SQL_API_GetData((BU32)PROGRAM_LOCK, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_Fav 		= (BU08) SQL_API_GetData((BU32)PROGRAM_FAV, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_Del 		= (BU08) SQL_API_GetData((BU32)PROGRAM_DEL, 0, 0, APP_DATA_TYPE_NUM);		
		pChnInfoList->pChnInfo[i].m_Skip 		= (BU08) SQL_API_GetData((BU32)PROGRAM_SKIP, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioChn 	= (BU08) SQL_API_GetData((BU32)PROGRAM_TRACK, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioOffset = (BS08) SQL_API_GetData((BU32)PROGRAM_AUDIO_OFFSET, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_FreqMHz 	= (BU16) SQL_API_GetData((BU32)PROGRAM_FREQ, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_QAM 		= (BU08) SQL_API_GetData((BU32)PROGRAM_QAM, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_SymKbps 	= (BU16) SQL_API_GetData((BU32)PROGRAM_SYMKBPS, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_style 		= (BU08) SQL_API_GetData((BU32)PROGRAM_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_VideoPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_PID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_PID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_PcrPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_PCRPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_EcmPID 	= (BU16) SQL_API_GetData((BU32)PROGRAM_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_audioEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_videoEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_CAT_Sys_ID = (BU16) SQL_API_GetData((BU32)PROGRAM_SCAT_SYS_ID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_TsId       = (BU32) SQL_API_GetData((BU32)PROGRAM_TSID, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData(PROGRAM_SORT_ID, strAllSortId, 8 * CHN_MAX_SORT_COUNT, APP_DATA_TYPE_STRING);
		pChnInfoList->pChnInfo[i].m_AudioVolume = (BS08) SQL_API_GetData((BU32)PROGRAM_AUDIO_VOLUME, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_SortFlag = 0x01;
		//pChnInfoList->pChnInfo[i].m_SortFlag = 0xffffffff;
		for (j = 0; j < sortCount; j++)
		{
			if (strstr(strAllSortId, g_SortInfo[j].sortName) != NULL)
			{
				pChnInfoList->pChnInfo[i].m_SortFlag |= (1 << j);
			}
		}
		pChnInfoList->pChnInfo[i].m_pmtpid     = (BU16) SQL_API_GetData((BU32)PROGRAM_PID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_OriNetId   = (BU16) SQL_API_GetData((BU32)PROGRAM_ORI_NET_ID, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_VideoType  = (BU08) SQL_API_GetData((BU32)PROGRAM_VIDEO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		pChnInfoList->pChnInfo[i].m_AudioType  = (BU08) SQL_API_GetData((BU32)PROGRAM_AUDIO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
	}

#endif
	pChnInfoList->u32ProgCount = SQL_API_GetTotalNum(); 
	if (g_AllTypeChnNo[AvType][GroupType] >= pChnInfoList->u32ProgCount &&  pChnInfoList->u32ProgCount != 0)
	{
		g_AllTypeChnNo[AvType][GroupType] = pChnInfoList->u32ProgCount -1;
	}
//	printf("g_AllTypeChnNo[AvType][GroupType] = %d\n", g_AllTypeChnNo[AvType][GroupType]);

	SQL_API_Semaphore_Release();
	APP_ChnDatabase_API_UnProtect();
	
	return APP_SUCCESS;
}







#if 0

/*-------------------------------------------------------------------------------

	Description:Set flag for recreating list

	Parameters:

-------------------------------------------------------------------------------*/
void APP_IPI_SetTagNeedReCreateChnList(void)
{
	isNeedReCreateChnList = 1;
}

/*-------------------------------------------------------------------------------

	Description:创建节目列表

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType)
{
	static struct ChnInfoList_s ListTv 		= {0, 0, 1, APP_CHN_GROUP_TYPE_ALL_e, 0};
	static struct ChnInfoList_s ListTvFav   = {0, 0, 1, APP_CHN_GROUP_TYPE_FAV_e, 0};
	static struct ChnInfoList_s ListRd 		= {0, 0, 1, APP_CHN_GROUP_TYPE_ALL_e, 0};
	static struct ChnInfoList_s ListRdFav 	= {0, 0, 1, APP_CHN_GROUP_TYPE_FAV_e, 0};
	struct ChnInfoList_s *pChnInfoList;
	
	BU32	i = 0;
	static BU08 					oldAvType = 99;
	static BU08 					oldGroupType = 99;

	BU08 isFoundProg0 = 0;

	APP_EPG_API_StopGetEpg();

	app_system_data.g_AvType = (BU08)AvType;
	app_system_data.g_GroupType = (BU08)GroupType;
	
	if(oldAvType != AvType || oldGroupType != GroupType)
	{
		APP_SYS_API_SaveSysInfo ();
		oldAvType = AvType;
		oldGroupType = GroupType;
	}
	if(isNeedReCreateChnList == 1)
	{
		ListTv.isNeedReCreate = 1;
		ListTvFav.isNeedReCreate = 1; 
		ListRd.isNeedReCreate = 1; 	
		ListRdFav.isNeedReCreate = 1; 
		isNeedReCreateChnList = 0;
	}

	if(AvType == APP_CHN_AV_TYPE_TV_e)
	{
		if(GroupType == APP_CHN_GROUP_TYPE_FAV_e)
			pChnInfoList = &ListTvFav;
		else
			pChnInfoList = &ListTv;
	}
	else
	{
		if(GroupType == APP_CHN_GROUP_TYPE_FAV_e)
			pChnInfoList = &ListRdFav;
		else
			pChnInfoList = &ListRd;
	}
	APP_ChnDatabase_API_Protect ();
	if(pChnInfoList->isNeedReCreate == 0
		&& pChnInfoList->groupType == GroupType)
	{
		g_pProgInfoList = pChnInfoList->pProgInfoList;
		g_ChnNum = pChnInfoList->progCount;
		
		APP_ChnDatabase_API_UnProtect();
		
		if(pChnInfoList->progCount != 0)
		{
			return APP_SUCCESS;
		}
		else
		{

			return APP_FAIL;
		}
	}
	else
	{
		pChnInfoList->isNeedReCreate = 0;
		pChnInfoList->groupType = GroupType;		
	}

	APP_PRINT_DEBUG("avType %d, Group %d \n", AvType, GroupType);

	SQL_API_Semaphore_Wait();
	switch (app_system_data.g_GroupType)
	{
	case APP_CHN_GROUP_TYPE_FAV_e:
		sprintf(g_StrSQL, "select * from program where 喜爱 = 1 and 跳过 = 0 and 服务类型 = %d ORDER BY 编号 ASC, service_id ASC;",app_system_data.g_AvType);
		break;
	case APP_CHN_GROUP_TYPE_ALL_e:
		sprintf(g_StrSQL, "select * from program where 服务类型 = %d ORDER BY 编号 ASC, service_id ASC;", app_system_data.g_AvType);
		break;
	case APP_CHN_GROUP_TYPE_EDIT_e:
		sprintf(g_StrSQL, "select * from program where 跳过 = 0 and 服务类型 = %d ORDER BY 编号 ASC, service_id ASC;", app_system_data.g_AvType);
		break;
	default:
		break;
	}

	SQL_API_Select(g_StrSQL);

	if(pChnInfoList->count < SQL_API_GetTotalNum())
	{
		if(pChnInfoList->pProgInfoList != 0)
		{
			FYF_API_free(pChnInfoList->pProgInfoList);
			pChnInfoList->pProgInfoList = 0;
		}
		pChnInfoList->pProgInfoList = FYF_API_malloc((BS32)((SQL_API_GetTotalNum() + 21) * sizeof(ChnDataBase_ChnInfo_S)));		
		if(pChnInfoList->pProgInfoList == 0)
		{
			pChnInfoList->count = 0;
			pChnInfoList->progCount = 0;
			g_pProgInfoList = 0;
			g_ChnNum = 0;
			SQL_API_Semaphore_Release();
			APP_ChnDatabase_API_UnProtect();
			return APP_FAIL;
		}
		pChnInfoList->count = SQL_API_GetTotalNum() + 20;
	}
	g_pProgInfoList = pChnInfoList->pProgInfoList;
	if(SQL_API_GetTotalNum() == 0)
	{
		g_pProgInfoList = 0;	
	}

#ifdef APP_CONFIG_ZERO_CHANNEL
	for (i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		BU32 iSave = 0;
		SQL_API_GotoRow(i);

		g_pProgInfoList[i].m_ProgId 	= SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		/*发现0频道，所有频道向后移动一位*/
		if(SeparateSevId(g_pProgInfoList[i].m_ProgId) == CHN_ZERO_PROG_SERVICE_ID && isFoundProg0 == 0)
		{
			BS32 j = 0;
			for(j = i - 1; j >= 0; j--)
			{
				memcpy(&g_pProgInfoList[j + 1], &g_pProgInfoList[j], sizeof(ChnDataBase_ChnInfo_S));
				g_pProgInfoList[j + 1].m_ChnIndex = g_pProgInfoList[j + 1].m_ChnIndex + 1;
			}
			iSave = i;
			i = 0;
			isFoundProg0 = 1;
		}
		g_pProgInfoList[i].m_ProgId 	= SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_ChnIndex 		= i;
		g_pProgInfoList[i].m_ProgLCN = (BU32)SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData((BU32)PROGRAM_CHN_NAME, g_pProgInfoList[i].m_ChnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
		g_pProgInfoList[i].m_Ca 		= (BU08) SQL_API_GetData((BU32)PROGRAM_CA, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_Lock 		= (BU08) SQL_API_GetData((BU32)PROGRAM_LOCK, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_Fav 		= (BU08) SQL_API_GetData((BU32)PROGRAM_FAV, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_Del 		= (BU08) SQL_API_GetData((BU32)PROGRAM_DEL, 0, 0, APP_DATA_TYPE_NUM);		
		g_pProgInfoList[i].m_Skip 		= (BU08) SQL_API_GetData((BU32)PROGRAM_SKIP, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioChn 	= (BU08) SQL_API_GetData((BU32)PROGRAM_TRACK, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioOffset = (BS08) SQL_API_GetData((BU32)PROGRAM_AUDIO_OFFSET, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_FreqMHz 	= (BU16) SQL_API_GetData((BU32)PROGRAM_FREQ, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_QAM 		= (BU08) SQL_API_GetData((BU32)PROGRAM_QAM, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_SymKbps 	= (BU16) SQL_API_GetData((BU32)PROGRAM_SYMKBPS, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_style 		= (BU08) SQL_API_GetData((BU32)PROGRAM_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_VideoPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_PID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_PID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_PcrPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_PCRPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_EcmPID 	= (BU16) SQL_API_GetData((BU32)PROGRAM_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_audioEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_videoEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_CAT_Sys_ID = (BU16) SQL_API_GetData((BU32)PROGRAM_SCAT_SYS_ID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_TsId       = (BU32) SQL_API_GetData((BU32)PROGRAM_TSID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_pmtpid     = (BU16) SQL_API_GetData((BU32)PROGRAM_PID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_OriNetId   = (BU16) SQL_API_GetData((BU32)PROGRAM_ORI_NET_ID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_VideoType  = (BU08) SQL_API_GetData((BU32)PROGRAM_VIDEO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioType  = (BU08) SQL_API_GetData((BU32)PROGRAM_AUDIO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		if(iSave != 0)
		{
			i = iSave;
		}
	}
#else
	for (i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		SQL_API_GotoRow(i);

		g_pProgInfoList[i].m_ProgId 	= (BU32) SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_ChnIndex 		=  i;//(BU32) SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_ProgLCN = (BU32)SQL_API_GetData((BU32)PROGRAM_INDEX, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData((BU32)PROGRAM_CHN_NAME, g_pProgInfoList[i].m_ChnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
		g_pProgInfoList[i].m_Ca 		= (BU08) SQL_API_GetData((BU32)PROGRAM_CA, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_Lock 		= (BU08) SQL_API_GetData((BU32)PROGRAM_LOCK, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_Fav 		= (BU08) SQL_API_GetData((BU32)PROGRAM_FAV, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_Del 		= (BU08) SQL_API_GetData((BU32)PROGRAM_DEL, 0, 0, APP_DATA_TYPE_NUM);		
		g_pProgInfoList[i].m_Skip 		= (BU08) SQL_API_GetData((BU32)PROGRAM_SKIP, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioChn 	= (BU08) SQL_API_GetData((BU32)PROGRAM_TRACK, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioOffset = (BS08) SQL_API_GetData((BU32)PROGRAM_AUDIO_OFFSET, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_FreqMHz 	= (BU16) SQL_API_GetData((BU32)PROGRAM_FREQ, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_QAM 		= (BU08) SQL_API_GetData((BU32)PROGRAM_QAM, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_SymKbps 	= (BU16) SQL_API_GetData((BU32)PROGRAM_SYMKBPS, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_style 		= (BU08) SQL_API_GetData((BU32)PROGRAM_TYPE, 0, 0, APP_DATA_TYPE_NUM);

		g_pProgInfoList[i].m_VideoPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_PID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_PID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_PcrPid 	= (BU16) SQL_API_GetData((BU32)PROGRAM_PCRPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_EcmPID 	= (BU16) SQL_API_GetData((BU32)PROGRAM_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_audioEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_AUDIO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_videoEcmPID= (BU16) SQL_API_GetData((BU32)PROGRAM_VIDEO_ECMPID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_CAT_Sys_ID = (BU16) SQL_API_GetData((BU32)PROGRAM_SCAT_SYS_ID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_TsId       = (BU32) SQL_API_GetData((BU32)PROGRAM_TSID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_pmtpid     = (BU16) SQL_API_GetData((BU32)PROGRAM_PID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_OriNetId   = (BU16) SQL_API_GetData((BU32)PROGRAM_ORI_NET_ID, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_VideoType  = (BU08) SQL_API_GetData((BU32)PROGRAM_VIDEO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
		g_pProgInfoList[i].m_AudioType  = (BU08) SQL_API_GetData((BU32)PROGRAM_AUDIO_TYPE, 0, 0, APP_DATA_TYPE_NUM);
	}

#endif
	pChnInfoList->progCount = SQL_API_GetTotalNum(); 
	g_ChnNum = SQL_API_GetTotalNum();
	if (app_system_data.g_CurChnNo >= g_ChnNum)
	{		
		app_system_data.g_CurChnNo =(g_ChnNum == 0) ? 0 : (g_ChnNum - 1);
		if (APP_FAIL == APP_ChnDatabase_API_SetCurChnNO(app_system_data.g_CurChnNo))
		{
			APP_PRINT_ERROR("APP_ChnDatabase_API_SetCurChnNO Failure");
		}
	}

	SQL_API_Semaphore_Release();
	if (g_ChnNum != 0)
	{
		APP_ChnDatabase_API_UnProtect();
		return APP_SUCCESS;
	}
	else
	{
		APP_ChnDatabase_API_UnProtect();
		return APP_FAIL;
	}
}

#endif





void APP_ChnDatabase_IPI_SetSortCount(BU32 count)
{
	sortCount = count;
}

/*-------------------------------------------------------------------------------

	Description:得到节目分类数量

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_GetSortCount(void)
{
#ifdef SANMING_VERSION
	return 13;
#else
	return sortCount;
#endif
}

/*-------------------------------------------------------------------------------

	Description:得到分类的名称

	Parameters:

-------------------------------------------------------------------------------*/
void APP_ChnDatabase_API_GetSortName(BU32 sortIndex, BU08 *strName)
{

#ifdef SANMING_VERSION
	if(sortIndex >= 13)
	{
		strcpy(strName, "");		
		return;
	}
	strcpy(strName, g_sortname[sortIndex]);

#else
	if(sortIndex >= sortCount)
	{
		strcpy(strName, "");		
		return;
	}
	strcpy(strName, g_SortInfo[sortIndex].sortName);

#endif
}

/*-------------------------------------------------------------------------------

	Description:得到某个分类中的节目数

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_ChnDatabase_API_GetSortChnCount(BU32 sortIndex)
{
	if(sortIndex >= sortCount)
	{
		return 0;
	}
	return g_SortInfo[sortIndex].dataSort.nResult;

}

/*-------------------------------------------------------------------------------

	Description:得到某个分类中的节目信息

	Parameters:

-------------------------------------------------------------------------------*/
BU08 APP_ChnDatabase_API_GetSortChnInfo(BU32 sortIndex, BU32 chnIndex, ChnDataBase_ChnInfo_S* chnInfo)
{
	chnInfo->m_ProgId = SQL_GetNumberData(&(g_SortInfo[sortIndex].dataSort),chnIndex,PROGRAM_SERVICE_ID);
	SQL_GetStringData(&(g_SortInfo[sortIndex].dataSort),chnIndex,PROGRAM_CHN_NAME,chnInfo->m_ChnName,APP_CHN_NAME_LEN);
	chnInfo->m_Lock = SQL_GetNumberData(&(g_SortInfo[sortIndex].dataSort),chnIndex,PROGRAM_LOCK);
	chnInfo->m_Fav = SQL_GetNumberData(&(g_SortInfo[sortIndex].dataSort),chnIndex,PROGRAM_FAV);
	chnInfo->m_Ca = SQL_GetNumberData(&(g_SortInfo[sortIndex].dataSort),chnIndex,PROGRAM_CA);
}
/*-------------------------------------------------------------------------------
	Description:    设置和得到0频道标志

	Parameters:
-------------------------------------------------------------------------------*/
BU16 APP_ChnDatabase_API_GetChn0Flag(void)
{
	BU32 serviceid =0;
	ChnDataBase_ChnInfo_S chnInfo;
	APP_CHN_GROUP_TYPE_E type;
	APP_CHN_AV_TYPE_E avtype;
#ifdef APP_CONFIG_ZERO_CHANNEL
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e, 0, &chnInfo))
	{
		return;
	}
	serviceid = SeparateSevId(chnInfo.m_ProgId);
	type = APP_ChnDatabase_API_GetCurGroupType();
	avtype = APP_ChnDatabase_API_GetCurChnType();
	if(serviceid == CHN_ZERO_PROG_SERVICE_ID && type == APP_CHN_GROUP_TYPE_ALL_e
		&& avtype == APP_CHN_AV_TYPE_TV_e)
	{
		g_search_ch0Flag = 0;
	}
	else 
	{
		g_search_ch0Flag = 1;
	}
#endif
	return g_search_ch0Flag;
}
void APP_ChnDatabase_API_SetChn0Flag(BU16 ch0Flag)
{
	g_search_ch0Flag = ch0Flag;
}
/*-------------------------------------------------------------------------------
	Description:    判断是否有0频道
				    0: 0频道存在
				    1: 0频道不存在

	Parameters:
-------------------------------------------------------------------------------*/
void APP_ChnDatabase_API_JudgeChn0(void)
{
	BU32 serviceid =0;
	ChnDataBase_ChnInfo_S chnInfo;
	APP_CHN_GROUP_TYPE_E type;

#if 0
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e, 0, &chnInfo))
	{
		return;
	}
	serviceid = SeparateSevId(chnInfo.m_ProgId);
	type = APP_ChnDatabase_API_GetCurGroupType();
	if(serviceid == CHN_ZERO_PROG_SERVICE_ID && type == APP_CHN_GROUP_TYPE_ALL_e)
	{
		g_search_ch0Flag = 0;
	}
	else 
	{
		g_search_ch0Flag = 1;
	}
#endif
}
/*-------------------------------------------------------------------------------
	Description:    设置最后一次观看的节目号

	Parameters:
-------------------------------------------------------------------------------*/
void APP_ChnDatabase_API_SetLastChnInfo(BU16 chn)
{
	if(g_fullscreen_back_s[0].avtype !=APP_ChnDatabase_API_GetCurChnType() || g_fullscreen_back_s[0].chn != chn)
	{
		g_fullscreen_back_s[1].avtype = g_fullscreen_back_s[0].avtype ;//上一个
		g_fullscreen_back_s[1].chn = 	g_fullscreen_back_s[0].chn;
		g_fullscreen_back_s[0].avtype = APP_ChnDatabase_API_GetCurChnType(); // 当前
		g_fullscreen_back_s[0].chn = chn;
	}
}
/*-------------------------------------------------------------------------------
	Description:    得到最后一次观看的节目信息

	Parameters:
-------------------------------------------------------------------------------*/
void APP_ChnDatabase_API_GetLastChnInfo(APP_CHN_AV_TYPE_E *avType, BU32 *chIndex)
{
	*avType = g_fullscreen_back_s[1].avtype;
	*chIndex = g_fullscreen_back_s[1].chn;
}

