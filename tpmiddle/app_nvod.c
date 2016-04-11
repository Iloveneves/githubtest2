#include "app_common.h"
#include "app_ipi_common.h"

extern BU32	semaphoreNvod ;
extern int g_RerservedCount;
extern BU08 bReserveChange;
extern ReservedList epg_reservedlist[25];
extern BU32 oldFreq;

static BU32 nvod_eitChannelID = 0;
static BU32 nvod_eitFilterID = 0;
static BU32 g_nvod_shift_service_need_update = 0;
static BU32	iNvodProgNoTemp = 0;   /*标记当前EIT正在分析的参考节目段所属的节目号*/
static BU32	NvodProgNum = 0;  
static BU32 NvodEventNum = 0;

/* 时移业务对应于serviceid，事件对应于eventid*/
static BU32 shiftServiceIndexNum = 0;
static BU08 shiftServiceIndex[NVOD_MAX_NO_OF_PROGRAMS];
static BU32 refEventIndexNum = 0;
static BU32 refEventIndexIndex[NVOD_MAX_NO_OF_EVENTS];

APP_Nvod_Program_List_S     astNvodProgInfoTable[NVOD_MAX_NO_OF_PROGRAMS];
APP_Nvod_Prog_TimeInfo_S    asNvodProgEventTable[NVOD_MAX_NO_OF_EVENTS];   
BU16 nvodPlayServiceId = 0;

/**********************************************************************************
Desc:    设置和获取时移业务数据库更新的标志
**********************************************************************************/
void APP_Nvod_SetShiftServiceUpdateFlag(const BU32 shift_service_update_flag)
{
	g_nvod_shift_service_need_update = shift_service_update_flag;
}
BU32 APP_Nvod_GetShiftServiceUpdateFlag(void)
{
	return g_nvod_shift_service_need_update;
}
/**********************************************************************************
Desc:   参考event有新数据时，更新时移业务数据库中链接事件表的标志
**********************************************************************************/
void APP_Nvod_UpdateAllRefEventUpdateStatus(void)
{
	BU32 index;
	for(index = 0;index<NVOD_MAX_NO_OF_PROGRAMS;index++)
	{
		if(astNvodProgInfoTable[index].uProgSlotStatus == ELEMENT_OCCUPIED)
		{
			astNvodProgInfoTable[index].update = 1;
		}
	}
}
/**********************************************************************************
Desc:   参考event有新数据时，更新时移业务数据库中链接事件表的标志
**********************************************************************************/
void APP_Nvod_SetRefEventUpdateStatusByIndex(BU32 index,BU32 event_update_flag)
{
	astNvodProgInfoTable[shiftServiceIndex[index]].update = event_update_flag;
}
/**********************************************************************************
Desc:   设置参考event数据库中运行标志run_status
**********************************************************************************/
void APP_Nvod_SetRefEventRunStatusByIndex(BU32 index,BU32 EventRunStatus)
{
	asNvodProgEventTable[refEventIndexIndex[index]].uEventRunStatus = EventRunStatus;
}
void APP_Nvod_SetRefEventRunStatusByReserveIndex(BU32 ReserveIndex,BU32 EventRunStatus)
{
	BU32 i;
	BU32 reserveCount;
	BU32 serviceId;
	BU32 eventId;
	BU32 day;
	BU08 startHour;
	BU08 startMin;
	SQL_API_Semaphore_Wait();
	SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 4 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
	reserveCount = SQL_API_GetTotalNum();
	if (reserveCount < ReserveIndex)
	{
		SQL_API_Semaphore_Release();
		APP_PRINT_ERROR("Invalid Cancle Index\n");
		return APP_FAIL;
	}
	SQL_API_GotoRow(ReserveIndex);
	serviceId = SQL_API_GetData(EVENTRESERVED_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
	eventId = SQL_API_GetData(EVENTRESERVED_EVENT_ID, 0, 0, APP_DATA_TYPE_NUM);
	startHour = (BU08)SQL_API_GetData(EVENTRESERVED_START_HOUR, 0, 0, APP_DATA_TYPE_NUM);
	startMin = (BU08)SQL_API_GetData(EVENTRESERVED_START_MIN, 0, 0, APP_DATA_TYPE_NUM);
	day = SQL_API_GetData(EVENTRESERVED_DAY, 0, 0, APP_DATA_TYPE_NUM);
	SQL_API_Semaphore_Release();

	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	for(i = 0;i<NVOD_MAX_NO_OF_EVENTS ;i++)
	{
		if((BU32)(asNvodProgEventTable[i].uProgNvodSeviceID) == serviceId 
			   && (BU32)(asNvodProgEventTable[i].uProgNvodEventID) == eventId 
			   && (BU08)(asNvodProgEventTable[i].uStartHour) == startHour
			   && (BU08)(asNvodProgEventTable[i].uStartMinute) == startMin)
		{
			asNvodProgEventTable[i].uEventRunStatus = EventRunStatus;
			FYF_API_semaphore_release(semaphoreNvod);
			return;
		}
	}
	FYF_API_semaphore_release(semaphoreNvod);
}
/**********************************************************************************
Desc:    事件时间与当前系统时间比较,传入时间应为标准时间；
Return:  TRUE: 事件是旧的；    FALSE: 事件是有效的；
**********************************************************************************/
BU32 APP_NVOD_CmpToSystemTime(const BU32 iCodeDate, const BU08 uStartHour, 
                               const BU08 uStartMinute, const BU16 sDuring)
{
	BU32 iCurHour;
	BU32 iCurMinute;
	BU32 iSysCodeDate;
	BU32 bOldEvent = FALSE;
	APP_Date_S now;
	
	APP_SYS_API_GetLocalDateAndTime(&now);

	iCurHour = (BU32)now.m_hour;
	iCurMinute = (BU32)now.m_min;
	iSysCodeDate = APP_IPI_GetDay();//APP_TDT_IPI_GetSystemCodedate();
	//app_printf("iCurHour = %d,iCurMinute = %d,iSysCodeDate = %d\n",iCurHour,iCurMinute,iSysCodeDate);
	//app_printf("uStartHour = %d,uStartMinute = %d,iCodeDate = %d\n",uStartHour,uStartMinute,iCodeDate);

	if ((iCodeDate == iSysCodeDate) &&\
		(uStartHour*60+uStartMinute+sDuring <= iCurHour * 60 + iCurMinute))
	{
		bOldEvent = TRUE;
	}
	else if((iCodeDate < iSysCodeDate))
	{
	    if(uStartHour*60+uStartMinute+sDuring <= (iCurHour+(iSysCodeDate-iCodeDate)*24) * 60 + iCurMinute)
	    {
			bOldEvent = TRUE;
		}
	}

	return bOldEvent;
}

/**********************************************************************************
					获取该事件在事件库中的萦引号
*********************************************************************************/  
BS16 APP_NVOD_GetEventIndex(BU08 ucTableId, BU16 sServideID, BU16 uEventId, BU32 uCodeDate,
                                     BU08 ucHour, BU08 ucMinute, BS32  iEndNum)
{
	BS16	sEventTempIndex = 0;
	if(ucTableId > 0x4F)  /*时移事件的EventId无效*/
	{
		for (sEventTempIndex = 0; sEventTempIndex < iEndNum; sEventTempIndex++)
		{
			if((asNvodProgEventTable[sEventTempIndex].uEventStatus == ELEMENT_OCCUPIED) &&\
				(asNvodProgEventTable[sEventTempIndex].uProgNvodSeviceID == sServideID) &&\
				asNvodProgEventTable[sEventTempIndex].uEventDateCode == uCodeDate &&\
				asNvodProgEventTable[sEventTempIndex].uStartHour == ucHour &&\
				asNvodProgEventTable[sEventTempIndex].uStartMinute == ucMinute)
			{
				return sEventTempIndex;
			}
		}
	}
	else/*参考事件的时间信息无效*/
	{
		for (sEventTempIndex = 0; sEventTempIndex < iEndNum; sEventTempIndex++)
		{
			if((asNvodProgEventTable[sEventTempIndex].uEventStatus == ELEMENT_OCCUPIED) &&\
				(asNvodProgEventTable[sEventTempIndex].uProgNvodEventID == uEventId) &&\
				(asNvodProgEventTable[sEventTempIndex].uProgNvodSeviceID == sServideID))
			{
				return sEventTempIndex ;
			}      
		}	
	}
	
	return INVALID_LINK;
}

/**********************************************************************************
通过事件的索引值清除nvod事件的信息，即释放一个事件空间
**********************************************************************************/
void APP_NVOD_FreeEventNodeByIndex(BS16 sIndex)
{
	if (sIndex >= 0 && sIndex < NVOD_MAX_NO_OF_EVENTS)  
	{
		asNvodProgEventTable[sIndex].uEventStatus 		= ELEMENT_FREE;
		asNvodProgEventTable[sIndex].uProgNvodEventID   = 0xFFFF;  
		asNvodProgEventTable[sIndex].uProgNvodSeviceID  = 0xFFFF; 
		asNvodProgEventTable[sIndex].uProgRefEventID	= 0xFFFF;
		asNvodProgEventTable[sIndex].uProgRefSeviceID	= 0xFFFF;
		
		asNvodProgEventTable[sIndex].uEventDateCode		= 0xFFFF;
		asNvodProgEventTable[sIndex].uStartHour 		= 0;
		asNvodProgEventTable[sIndex].uStartMinute 	    = 0;
		asNvodProgEventTable[sIndex].sDuringTime		= 0;
		asNvodProgEventTable[sIndex].uStartSecond       = 0;
		asNvodProgEventTable[sIndex].uDuringSecond      = 0;
		
		asNvodProgEventTable[sIndex].uEventRunStatus    = RUNNING_NONE;
		asNvodProgEventTable[sIndex].ubank 				= 0;
		asNvodProgEventTable[sIndex].next 				= NULL;
	}
}

/**********************************************************************************
				获取事件库中一个空节点
*********************************************************************************/  
BS16  APP_NVOD_GetFreeEventNode(void)
{
	BS16	Index = 0;
	BS16	sEventIndex = INVALID_LINK;

	for (Index = 0; Index < NVOD_MAX_NO_OF_EVENTS; Index++)
	{
		if (asNvodProgEventTable[Index].uEventStatus == ELEMENT_FREE)
		{
		    sEventIndex = Index;
			return sEventIndex ;
		}
	}
	if(sEventIndex == INVALID_LINK)
	{
		for (Index = 0; Index < NVOD_MAX_NO_OF_EVENTS; Index++)
		{
			if (asNvodProgEventTable[Index].uEventDateCode == 0xFFFF)
			{
			    APP_NVOD_FreeEventNodeByIndex(Index);
				NvodEventNum --;
			    sEventIndex = Index;
				return sEventIndex ;		
			}
		}
	}
	return INVALID_LINK;
}

void APP_Nvod_AddToProgList(APP_Nvod_Prog_TimeInfo_S * pTimeInfo)
{
	BU32 index;
	APP_Nvod_Prog_TimeInfo_S * tmp,*tmp1;

	for(index = 0;index<NVOD_MAX_NO_OF_PROGRAMS;index++)
	{
		if(astNvodProgInfoTable[index].uProgNvodEventID == pTimeInfo->uProgRefEventID && astNvodProgInfoTable[index].uProgSlotStatus == ELEMENT_OCCUPIED)
		{
			break;
		}
	}
	if(NVOD_MAX_NO_OF_PROGRAMS == index)
	{
		//APP_Printf("APP_Nvod_AddToProgList error\n");
		return;
	}
	//APP_Printf("APP_Nvod_AddToProgList success\n");
	tmp = astNvodProgInfoTable[index].pTimeInfo;
	if(NULL == tmp)
	{
		astNvodProgInfoTable[index].pTimeInfo = &(astNvodProgInfoTable[index].Head);
		astNvodProgInfoTable[index].pTimeInfo->next = pTimeInfo;
		astNvodProgInfoTable[index].update = 1;
		astNvodProgInfoTable[index].num=1;
	}
	else
	{
		tmp1 = tmp;
		tmp = tmp->next;
		while(tmp)
		{	
			if(tmp->uEventDateCode == pTimeInfo->uEventDateCode && tmp->uStartHour == pTimeInfo->uStartHour && tmp->uStartMinute == pTimeInfo->uStartMinute)
			{
				return;
			}
			if((tmp->uEventDateCode > pTimeInfo->uEventDateCode) ||
				(tmp->uEventDateCode == pTimeInfo->uEventDateCode && tmp->uStartHour > pTimeInfo->uStartHour) ||
				(tmp->uEventDateCode == pTimeInfo->uEventDateCode && tmp->uStartHour == pTimeInfo->uStartHour && tmp->uStartMinute > pTimeInfo->uStartMinute))
			{
				break;
			}
			tmp1 = tmp;
			tmp = tmp->next;
		}
		astNvodProgInfoTable[index].update = 1;
		(astNvodProgInfoTable[index].num)++;
		pTimeInfo->next = tmp;
		tmp1->next = pTimeInfo;
	}
}

/**********************************************************************************
					分析NVOD  的描述值
				1.分析事件名称及详细信息
**********************************************************************************/ 
BU32 APP_NVOD_ParseEpgDescriptor (BU08 *pucSectionData, BS16 sEventIndex, 
                        BU16 service_id, BU16 uEventID, BU16 sDurningTime)
{

	int	i,iProgTemp;
	int	iDescriptorLength;
	int	iNoOfBytesParsed;

	int  	name_length;
	int  	text_length;
	int  	length;
	int		iCurNvodNo = INVALID_LINK;
//	BU08 	ucTempData8;   
	BU16 	ref_service_id;
	BU32		bSaveEvent = false;
	BU32     bExisted = FALSE;
	//unsigned char * service_provider_name;
	//unsigned char * service_name;

	iNoOfBytesParsed 	= 0;
	iDescriptorLength 	= pucSectionData[1];

	switch (pucSectionData[0])
	{

	    /*只有参考业务才有此描述符，提供参考EVENT的名称及详细信息;
		即此时EIT中的service_id是节目段的uProgNvodReferenceSeviceID*/
		case SHORT_EVENT_DESC:	/*0x4D*/       /*短信息分析表*/ 
	//app_printf("2.service_id = %x\n",service_id);
			name_length 	= pucSectionData[5] ;
			text_length 	= pucSectionData[6 + name_length];
            for(i = 0; i < FYF_API_nvod_num(); i++)
            {
				FYF_API_nvod_ref_service(i, &ref_service_id, 0, 0);
                if(service_id == ref_service_id)
                {				
					for(iProgTemp = 0; iProgTemp < NVOD_MAX_NO_OF_PROGRAMS; iProgTemp++)
				    {  
				        if(service_id == astNvodProgInfoTable[iProgTemp].uProgNvodRefSeviceID &&\
                           astNvodProgInfoTable[iProgTemp].uProgNvodEventID == uEventID && astNvodProgInfoTable[iProgTemp].uProgSlotStatus == ELEMENT_OCCUPIED)
						{
							iCurNvodNo = iProgTemp;
							iNvodProgNoTemp = iCurNvodNo;
							bExisted = TRUE;
							bSaveEvent = TRUE;
							break;
						}
					}
					if(!bExisted)
					{
					    for(iProgTemp = 0; iProgTemp < NVOD_MAX_NO_OF_PROGRAMS; iProgTemp ++)
					    {
					    	//app_printf("astNvodProgInfoTable[iProgTemp].uProgSlotStatus = %d,",astNvodProgInfoTable[iProgTemp].uProgSlotStatus);
					    	//app_printf("astNvodProgInfoTable[iProgTemp].uProgNvodRefSeviceID = %d,",astNvodProgInfoTable[iCurNvodNo].uProgNvodRefSeviceID);
					    	//app_printf("astNvodProgInfoTable[iProgTemp].uProgNvodEventID = %d,",astNvodProgInfoTable[iProgTemp].uProgNvodEventID);
							if(astNvodProgInfoTable[iProgTemp].uProgSlotStatus == ELEMENT_FREE)
					        {
					            iCurNvodNo = iProgTemp;
								iNvodProgNoTemp = iCurNvodNo;
								break;
					        }
						}
						
						if(iCurNvodNo != INVALID_LINK)
						{
							NvodProgNum ++;
							bSaveEvent = TRUE;
							break;
						}
					}
				}
			}
			if(bSaveEvent)	
			{
				bSaveEvent = FALSE;
				/*判断事件信息是否溢出*/
				if(NvodProgNum >= NVOD_MAX_NO_OF_PROGRAMS )
				{
					NvodProgNum = NVOD_MAX_NO_OF_PROGRAMS-1;
					return ( iDescriptorLength + 2 );
				}
				/*事件名称*/
				memset((char *)astNvodProgInfoTable[iCurNvodNo].acProgNvodName, 0, NVOD_MAX_EVENT_NAME_LENGTH + 1);
				length = name_length > NVOD_MAX_EVENT_NAME_LENGTH ? NVOD_MAX_EVENT_NAME_LENGTH : name_length;
				if (length > 0)     
				{
					memcpy((char *)astNvodProgInfoTable[iCurNvodNo].acProgNvodName, (char *)(pucSectionData + 6), length);
				}	
				
				/*事件详细信息*/	 
				memset((char *)astNvodProgInfoTable[iCurNvodNo].acProgNvodText, 0, NVOD_MAX_EVENT_TEXT_LENGTH + 1);
				length = text_length > NVOD_MAX_EVENT_TEXT_LENGTH ? NVOD_MAX_EVENT_TEXT_LENGTH : text_length;
				if (length > 10)
				{
					memcpy((char *)astNvodProgInfoTable[iCurNvodNo].acProgNvodText, (char *)(pucSectionData + 7 + name_length), length);
				}
				if(astNvodProgInfoTable[iCurNvodNo].uProgSlotStatus == ELEMENT_FREE)
				{
					APP_Nvod_SetShiftServiceUpdateFlag(1);
				}
				/*事件信息的event_id*/     
				astNvodProgInfoTable[iCurNvodNo].uProgSlotStatus = ELEMENT_OCCUPIED;
				astNvodProgInfoTable[iCurNvodNo].uProgNvodRefSeviceID = service_id;
				astNvodProgInfoTable[iCurNvodNo].uProgNvodEventID	= uEventID;
				astNvodProgInfoTable[iCurNvodNo].sDuringTime = sDurningTime;
				//app_printf("uProgSlotStatus = %d,uProgNvodRefSeviceID = %d,uProgNvodEventID = %d\n",astNvodProgInfoTable[iCurNvodNo].uProgSlotStatus,astNvodProgInfoTable[iCurNvodNo].uProgNvodRefSeviceID,astNvodProgInfoTable[iCurNvodNo].uProgNvodEventID);
			}		
			break;

        /*只有TIME_SHIFT_SERVICE_SERVICE的事件才有此描述符
		  通过此描述符，取得事件的uProgReferenceEventID和uProgReferenceSeviceID
		  用于判断此时间事件属于哪个NVOD业务下的哪个节目段*/
		case TIME_SHIFTED_EVENT_DESC:	/* 0x4F,获取Reference Even ID*/
				//app_printf("3.service_id = %x\n",service_id);

			{
				asNvodProgEventTable[sEventIndex].uProgRefEventID = (pucSectionData[4]<< 8) | pucSectionData[5];
				asNvodProgEventTable[sEventIndex].uProgRefSeviceID = (pucSectionData[2]<< 8) | pucSectionData[3];
			}
			
			break;

		case EXTENDED_EVENT_DESC:    /*0x4E，未完成Items的分析*/
			//app_printf("4.service_id = %x\n",service_id);

		{
			BYTE   descriptor_number = 0;
			BYTE   last_descriptor_number = 0;
			BYTE   length_of_items = 0;
			BYTE   item_description_length = 0;
			BYTE   ucParsedSoFar = 0;

			if(FYF_API_nvod_num() <= 0)
			{
				return  (iDescriptorLength + 2);
			}
			
//			ucTempData8 = pucSectionData[2];
			descriptor_number = pucSectionData[2] & 0xF0;
			if(descriptor_number != 0)
			{

			}
			last_descriptor_number = pucSectionData[2] & 0x0F;
			if(last_descriptor_number != 0)
			{	

			}
			
			length_of_items = pucSectionData[6];

			while (ucParsedSoFar < length_of_items)
			{
				item_description_length = pucSectionData[7+ucParsedSoFar];
				length = item_description_length > NVOD_MAX_EVENT_TEXT_LENGTH?NVOD_MAX_EVENT_TEXT_LENGTH : item_description_length;

				ucParsedSoFar += item_description_length;
				break;
			}
			
			text_length = pucSectionData[7 + length_of_items];     /*注意字节的定位*/
			length = text_length > NVOD_MAX_EVENT_TEXT_LENGTH ? NVOD_MAX_EVENT_TEXT_LENGTH : text_length;

			if(length > 4)  
			{
				if(descriptor_number == 0)  /*只要一个Section即可提供所有详细信息的情况*/
				{	
					if(iNvodProgNoTemp >=0)
					{
						memset((char *)astNvodProgInfoTable[iNvodProgNoTemp].acProgNvodText, 0, NVOD_MAX_EVENT_TEXT_LENGTH +1);					
						memcpy((char *)astNvodProgInfoTable[iNvodProgNoTemp].acProgNvodText,  (char *)(pucSectionData + 8 + length_of_items), length);
					}
				}
			}
		}
			break;

		default:
			break;
	}

	return ( iDescriptorLength + 2 );
}

/**********************************************************************************
					分析NVOD 事件信息
**********************************************************************************/ 
void APP_NVOD_StartParserEit(BU08 *cEpgData, BU32 channelId, BU32 filterId)
{
	BU08			*pucSectionData;
	BU08			*pucdescriptorData;
	int				iNoOfDataBytes;
	int				iInfoLength;
	int				iTsInfoLength;
	int				iNoOfBytesParsedSoFar; 
	BU08        	stTempData8;
	BU16	service_id;							//NVOD  当前事件的service_id
	BU16	stTempData16;
	
	/* EIT TABLE ELEMENT */
	BU08        table_id;
	int			section_length;
	int			current_next_indicator;
	BU16	transport_stream_id;
	int 		runing_status;
	BU08       	star_time[5];						//NVOD  当前事件的开始时间 
	BU08      	duration[3];					    //NVOD  当前事件的持续时间	
	BU16		event_id=0;							//NVOD  当前事件的event_id
	/* EIT EVENT ELEMENT */
	int     descriptors_loop_length;

	BS16   sEventIndex		= 0;
	int				iTemp = 0;
	BU32  uEventCodeDate = 0;
	
	BU08    ucStartHour;
	BU08	ucStartMinute;
	BU16	sDurningTime; 
	BU16  ucStartSecond = 0;
	BU16  ucDuringSecond = 0;
	BU32  time_zone_type;
	
	table_id = cEpgData [0];

	if (table_id < 0x4E || table_id > 0x5f)
	{
		return;
	}
	/****** read section_length******************************************************/
	section_length = ((cEpgData [ 1 ] & 0x0f)<<8 ) | cEpgData [ 2 ];

	stTempData16 = (cEpgData [ 8 ]<<8) | cEpgData [ 9 ];
	transport_stream_id = stTempData16;

	/****** read service_id (16) *****************************************************/
	stTempData16 = (cEpgData [ 3 ]<<8) | cEpgData [ 4 ];
	service_id = stTempData16;	
	/****** read version_number (5)  , current_next_indicator (1) **************************/
	stTempData8 		= cEpgData [5];
	current_next_indicator	= stTempData8&0x01;
	
	iTsInfoLength = section_length - 15; 
	pucSectionData = &cEpgData [14];

	if (current_next_indicator != 1)
	{
		return FALSE;
	}

	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	while( iTsInfoLength > 0 )
	{
		event_id		= (pucSectionData [0] << 8) | pucSectionData [1];
		
		star_time[0] 	= pucSectionData [2];
		star_time[1] 	= pucSectionData [3];
		star_time[2] 	= pucSectionData [4];
		star_time[3] 	= pucSectionData [5];
		star_time[4] 	= pucSectionData [6];

		duration[0] 		= pucSectionData [7];
		duration[1] 		= pucSectionData [8];
		duration[2] 		= pucSectionData [9];		   

		stTempData8 		= pucSectionData [10];
		
		/*取高三位的运行状态*/
		runing_status		= (stTempData8 >> 5) & 0x07;

		descriptors_loop_length = ( (stTempData8&0x0f)<<8) | (pucSectionData [11]);		
		iInfoLength 			= descriptors_loop_length;
		
		/*计算时间*/
		uEventCodeDate  = ((star_time[0] << 8) | star_time[1]);
		ucStartHour 	= ((star_time[2]>>4)&0x0f) *10 +(star_time[2]&0x0f);/*BCD码*/
		ucStartMinute 	= ((star_time[3]>>4)&0x0f) *10 +(star_time[3]&0x0f);
		ucStartSecond	= ((star_time[4]>>4)&0x0f) *10 +(star_time[4]&0x0f);
		sDurningTime 	= (((duration[0]>>4)&0x0f) *10 +(duration[0]&0x0f))*60+(((duration[1]>>4)&0x0f) *10 +(duration[1]&0x0f));
		ucDuringSecond  = ((duration[2]>>4)&0x0f) * 10 + (duration[2]&0x0f);
		//把标准时间日期转化为程序此时所设置的时区的时间
		APP_SYS_API_GetSysInfo(APP_SYS_TimeZone_e, &time_zone_type);
		if (time_zone_type == 0)
		{
			ucStartHour = (ucStartHour + APP_CONFIG_TIME_ZONE) % 24;
		}
		uEventCodeDate = APP_EIT_SIE_ConventDay(ucStartHour, uEventCodeDate);
        if(APP_NVOD_CmpToSystemTime(uEventCodeDate, ucStartHour, ucStartMinute, sDurningTime)) 
        {
    		pucSectionData 	= pucSectionData + 12 + iInfoLength;
			iTsInfoLength 	= iTsInfoLength - 12 - iInfoLength;
            continue;  
		}

		sEventIndex = APP_NVOD_GetEventIndex(table_id, service_id, event_id, uEventCodeDate,ucStartHour,
		                                      ucStartMinute, NVOD_MAX_NO_OF_EVENTS); 		
		if(sEventIndex == INVALID_LINK)  
		{
			sEventIndex = APP_NVOD_GetFreeEventNode();
			if(sEventIndex == INVALID_LINK )
			{
				FYF_API_semaphore_release (semaphoreNvod);
				return false;
			}
			else
			{
				NvodEventNum ++;  /*有效的NVOD事件数目*/
			}
		}/*由于开机时载入的事件没有ucDurningTime，需更新信息，所以此处添加这个判断*/
		//APP_Printf("NvodEventNum = %d", NvodEventNum);
		if(sEventIndex >= NVOD_MAX_NO_OF_EVENTS)  /*akju:0528*/
		{
			NvodEventNum = NVOD_MAX_NO_OF_EVENTS-1;
			FYF_API_semaphore_release (semaphoreNvod);
			return 	false;
		} 

		pucdescriptorData = pucSectionData + 12;
		iNoOfBytesParsedSoFar 	= 0;

        /*由于广西NVOD的片花的EVENT都有事件名，故对iNvodProgNoTemp初始化
		避免其他节目段的名称被改变*/
        iNvodProgNoTemp = INVALID_LINK; 
		
		asNvodProgEventTable[sEventIndex].uProgRefSeviceID = 0xffff;
		while ( iNoOfBytesParsedSoFar  < iInfoLength  )
		{
			iNoOfDataBytes = APP_NVOD_ParseEpgDescriptor(pucdescriptorData , sEventIndex, 
				                             service_id, event_id, sDurningTime);
			
			pucdescriptorData 		+= iNoOfDataBytes;
			iNoOfBytesParsedSoFar 	+= iNoOfDataBytes;
		}
		
		asNvodProgEventTable[sEventIndex].uEventStatus		= ELEMENT_OCCUPIED;
		asNvodProgEventTable[sEventIndex].uProgNvodEventID  = event_id;
		asNvodProgEventTable[sEventIndex].uProgNvodSeviceID	= service_id;
		asNvodProgEventTable[sEventIndex].uEventDateCode    = uEventCodeDate;
		asNvodProgEventTable[sEventIndex].uStartHour 		= ucStartHour;
		asNvodProgEventTable[sEventIndex].uStartMinute 		= ucStartMinute;
		asNvodProgEventTable[sEventIndex].sDuringTime		= sDurningTime;
		asNvodProgEventTable[sEventIndex].uStartSecond      = ucStartSecond;
		asNvodProgEventTable[sEventIndex].uDuringSecond    	= ucDuringSecond;
		asNvodProgEventTable[sEventIndex].index     		= sEventIndex;
		APP_Nvod_AddToProgList(&asNvodProgEventTable[sEventIndex]);
		pucSectionData 	= pucSectionData + 12 + iInfoLength;
		iTsInfoLength 	= iTsInfoLength - 12 - iInfoLength;
	}
	FYF_API_semaphore_release (semaphoreNvod);

	return true;
}
/**********************************************************************************
Desc:	开启滤取和分析nvod节目信息		
**********************************************************************************/ 
void APP_Nvod_API_GetProgList(void)
{
	BS16 eitPID = 0x0012;
	BU08 eitCoef[1] = {0x4e};
	BU08 eitMask[1] = {0xFF};
	BU08 eitExcl[1] = {0x00};
	struct ParserFuncNode_s parserFuncNode;

	APP_Parser_IPI_StopDemux (&nvod_eitChannelID, &nvod_eitFilterID);
	APP_Parser_IPI_StartDemux(1, eitPID, eitCoef, eitMask, eitExcl, &nvod_eitChannelID, &nvod_eitFilterID);
	parserFuncNode.channelId = nvod_eitChannelID;
	parserFuncNode.filterId = nvod_eitFilterID;
	parserFuncNode.ParserFunc = APP_NVOD_StartParserEit;
	APP_IPI_AddPasFuncNode(parserFuncNode);
}
/**********************************************************************************
Desc:	开启滤取和分析nvod事件信息				
**********************************************************************************/ 
void APP_Nvod_API_GetEventTimeList(void)
{
	BS16 eitPID = 0x0012;
	BU08 eitCoef[1] = {0x50};
	BU08 eitMask[1] = {0xF0};
	BU08 eitExcl[1] = {0x00};
	struct ParserFuncNode_s parserFuncNode;
	APP_Parser_IPI_StopDemux (&nvod_eitChannelID, &nvod_eitFilterID);
	APP_Parser_IPI_StartDemux(1, eitPID, eitCoef, eitMask, eitExcl, &nvod_eitChannelID, &nvod_eitFilterID);
	parserFuncNode.channelId = nvod_eitChannelID;
	parserFuncNode.filterId = nvod_eitFilterID;
	parserFuncNode.ParserFunc = APP_NVOD_StartParserEit;
	APP_IPI_AddPasFuncNode(parserFuncNode);
}
/**********************************************************************************
Desc:	开启滤取和分析nvod节目和事件信息				
**********************************************************************************/ 

void APP_Nvod_API_StartGetNvodProg(void)
{
	BS16 eitPID = 0x0012;
	struct ParserFuncNode_s parserFuncNode;
	APP_Parser_IPI_StopDemux (&nvod_eitChannelID, &nvod_eitFilterID);
	APP_Parser_IPI_StartDemux(0, eitPID, 0, 0, 0, &nvod_eitChannelID, &nvod_eitFilterID);
	parserFuncNode.channelId = nvod_eitChannelID;
	parserFuncNode.filterId = nvod_eitFilterID;
	parserFuncNode.ParserFunc = APP_NVOD_StartParserEit;
	APP_IPI_AddPasFuncNode(parserFuncNode);
}
/**********************************************************************************
Desc:	停止滤取和分析nvod节目和事件信息				
**********************************************************************************/ 

void APP_Nvod_API_StopGetNvodProg(void)
{
	APP_Parser_IPI_StopDemux(&nvod_eitChannelID, &nvod_eitFilterID);
}

void APP_Nvod_IPI_ClearNvodInfo(void)
{
	g_nvod_shift_service_need_update = 0;
	iNvodProgNoTemp = 0;   /*标记当前EIT正在分析的参考节目段所属的节目号*/
	NvodProgNum = 0;  
	NvodEventNum = 0;

	/* 时移业务对应于实际事件id,参考事件对应于参考事件id*/
	shiftServiceIndexNum = 0;
	memset(shiftServiceIndex,0,NVOD_MAX_NO_OF_PROGRAMS);
	refEventIndexNum = 0;
	memset(refEventIndexIndex,0,NVOD_MAX_NO_OF_EVENTS);
	memset(astNvodProgInfoTable,0,sizeof(APP_Nvod_Program_List_S)*NVOD_MAX_NO_OF_PROGRAMS);
	memset(asNvodProgEventTable,0,sizeof(APP_Nvod_Prog_TimeInfo_S)*NVOD_MAX_NO_OF_EVENTS);
	FYF_API_InitNvodInfo();
}

//shiftserviceidindex表示界面上节目列表的顺序
BU32 APP_Nvod_GetAllShiftServiceNumAndIndex(void)
{
	BU32 index;
	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	memset(shiftServiceIndex,0,NVOD_MAX_NO_OF_PROGRAMS);
	shiftServiceIndexNum = 0;
	for(index = 0;index < NVOD_MAX_NO_OF_PROGRAMS;index++)
	{
		if(ELEMENT_OCCUPIED == astNvodProgInfoTable[index].uProgSlotStatus)
		{
			shiftServiceIndex[shiftServiceIndexNum] = index;
			shiftServiceIndexNum++;
		}
	}
	FYF_API_semaphore_release(semaphoreNvod);
	return shiftServiceIndexNum;
}

//获取该索引值的参考事件内容
BU32 APP_Nvod_GetShiftServiceNodeByIndex(BU32 index,APP_Nvod_Program_List_S* shift_time_service)
{
	APP_Nvod_Prog_TimeInfo_S* tmp;
	BU32 systemCodeDate;
	BU32 hour = 0;
	BU32 minute = 0;
	APP_Date_S now;
	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	APP_Printf("shiftServiceIndexNum = %d\n", shiftServiceIndexNum);
	if(index >= shiftServiceIndexNum || NULL == shift_time_service)
	{
		APP_PRINT_ERROR("APP_Nvod_GetShiftServiceNodeByIndex--Error\n");
		APP_PRINT_ERROR("APP_Nvod_GetShiftServiceNodeByIndex--index = %d,shiftServiceIndexNum = %d,shift_time_service = 0x%x\n",index,shiftServiceIndexNum,shift_time_service);
		FYF_API_semaphore_release(semaphoreNvod);
		return APP_FAIL;
	}
	if (astNvodProgInfoTable[shiftServiceIndex[index]].pTimeInfo != NULL)
	{
		tmp = astNvodProgInfoTable[shiftServiceIndex[index]].pTimeInfo->next;
		if(NULL == tmp)
		{
			astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUN_SOON;
		}
		else
		{
			APP_SYS_API_GetLocalDateAndTime(&now);
			hour = (BU32)now.m_hour;
			minute = (BU32)now.m_min;
			systemCodeDate = APP_IPI_GetDay();//APP_TDT_IPI_GetSystemCodedate();

			if(systemCodeDate > tmp->uEventDateCode ||
				(systemCodeDate == tmp->uEventDateCode && hour > tmp->uStartHour) ||
				(systemCodeDate == tmp->uEventDateCode && hour == tmp->uStartHour && minute >= tmp->uStartMinute))
			{
				astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUNNING;
			}
			else
			{
				astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUN_SOON;
			}
		}
		
	}
	memcpy(shift_time_service,&astNvodProgInfoTable[shiftServiceIndex[index]],sizeof(APP_Nvod_Program_List_S));
/*	else
	{
		FYF_API_semaphore_release(semaphoreNvod);
		return APP_FAIL;
	}
	*/
	FYF_API_semaphore_release(semaphoreNvod);
	return APP_SUCCESS;
}


//检查参考事件的状态，是不是播放
void APP_Nvod_CheckShiftServiceStatus(void)
{
	APP_Nvod_Prog_TimeInfo_S* tmp;
	BU32 index;
	BU32 systemCodeDate;
	BU32 hour = 0;
	BU32 minute = 0;
	APP_Date_S now;
	BU32 oldRunState;
//	APP_Printf("APP_Nvod_CheckShiftServiceStatus shiftServiceIndexNum = %d\n",shiftServiceIndexNum);
	APP_SYS_API_GetLocalDateAndTime(&now);
	hour = (BU32)now.m_hour;
	minute = (BU32)now.m_min;
	systemCodeDate = APP_IPI_GetDay();//APP_TDT_IPI_GetSystemCodedate();
	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	for(index=0;index < shiftServiceIndexNum;index++)
	{
		oldRunState = astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus;
		if (astNvodProgInfoTable[shiftServiceIndex[index]].pTimeInfo != NULL)
		{
			tmp = astNvodProgInfoTable[shiftServiceIndex[index]].pTimeInfo->next;
			if(tmp == NULL)
			{
				astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUN_SOON;
			}
			else
			{
				if(systemCodeDate > tmp->uEventDateCode ||
					(systemCodeDate == tmp->uEventDateCode && hour > tmp->uStartHour) ||
					(systemCodeDate == tmp->uEventDateCode && hour == tmp->uStartHour && minute >= tmp->uStartMinute))
				{
					astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUNNING;
				}
				else
				{
					astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUN_SOON;
				}
			}
			if(astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus != oldRunState)
			{
				APP_Nvod_SetShiftServiceUpdateFlag(1);
			}
		}
		else
		{
			astNvodProgInfoTable[shiftServiceIndex[index]].uProgRunStatus = STATUS_RUN_SOON;
		}
	}
	FYF_API_semaphore_release(semaphoreNvod);
}


//获得参考事件下的所有时移事件
BU32 APP_Nvod_GetAllRefEventNumAndIndexByEventId(BU16 event_id)
{
	BU32 index;
	APP_Nvod_Prog_TimeInfo_S* ref_event_node_tmp = NULL;
	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	for(index=0;index<NVOD_MAX_NO_OF_PROGRAMS;index++)
	{
		if(astNvodProgInfoTable[index].uProgNvodEventID == event_id && astNvodProgInfoTable[index].uProgSlotStatus == ELEMENT_OCCUPIED)
		{
			break;
		}
	}
	if(NVOD_MAX_NO_OF_EVENTS == index)
	{
		FYF_API_semaphore_release(semaphoreNvod);
		return;
	}

	memset(refEventIndexIndex,0,NVOD_MAX_NO_OF_EVENTS*4);
	refEventIndexNum = 0;
	if (astNvodProgInfoTable[index].pTimeInfo != NULL)
	{
		ref_event_node_tmp = astNvodProgInfoTable[index].pTimeInfo->next;
		while(ref_event_node_tmp != NULL)
		{
			refEventIndexIndex[refEventIndexNum]=ref_event_node_tmp->index;
			refEventIndexNum++;
			ref_event_node_tmp = ref_event_node_tmp->next;
		}
	}
	FYF_API_semaphore_release(semaphoreNvod);
	return refEventIndexNum;
}

//通过索引获取时移事件的内容
BU32 APP_Nvod_GetRefEventNodeByIndex(BU32 index,APP_Nvod_Prog_TimeInfo_S* ref_event)
{
	BU32 systemCodeDate;
	BU32 hour = 0;
	BU32 minute = 0;
	BU32 i;
	APP_Date_S now;
	APP_Nvod_Prog_TimeInfo_S *ref_event_node_tmp;
		FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	//app_printf("APP_Nvod_GetRefEventNodeByIndex----index = %d,refEventIndexNum = %d\n",index,refEventIndexNum);
	if(index >= refEventIndexNum || NULL == ref_event)
	{
		APP_PRINT_ERROR("APP_Nvod_GetRefEventNodeByIndex--Error");
		FYF_API_semaphore_release(semaphoreNvod);
		return APP_FAIL;
	}
	if(bReserveChange)
	{
		APP_PRINT_DEBUG("APP_Nvod_GetRefEventNodeByIndex--bReserveChange = %d\n",bReserveChange);
		APP_EPG_IPI_GetReservedShortInfo();
		bReserveChange = false;
	}


	APP_SYS_API_GetLocalDateAndTime(&now);
	hour = (BU32)now.m_hour;
	minute = (BU32)now.m_min;
	systemCodeDate = APP_IPI_GetDay() ;//APP_TDT_IPI_GetSystemCodedate();
	ref_event_node_tmp = &(asNvodProgEventTable[refEventIndexIndex[index]]);
	if(systemCodeDate > ref_event_node_tmp->uEventDateCode ||
		(systemCodeDate == ref_event_node_tmp->uEventDateCode && hour > ref_event_node_tmp->uStartHour) ||
		(systemCodeDate == ref_event_node_tmp->uEventDateCode && hour == ref_event_node_tmp->uStartHour && minute >= ref_event_node_tmp->uStartMinute))
	{
		asNvodProgEventTable[refEventIndexIndex[index]].uEventRunStatus = RUNNING_PLAY;
	}
	else
	{
		if(g_RerservedCount != 0)
		{
			for (i = 0; i < g_RerservedCount; i++)
			{
				if((epg_reservedlist[i].reserveState == 4)
					&&((BU32)(ref_event_node_tmp->uProgNvodSeviceID) == (BU32)(epg_reservedlist[i].serviceId)) 
					&&((BU32)(ref_event_node_tmp->uProgNvodEventID) == (BU32)(epg_reservedlist[i].eventId)))
				{
					 asNvodProgEventTable[refEventIndexIndex[index]].uEventRunStatus = RUNNING_RESERVED;
					 break;
				}
			}
			if(i == g_RerservedCount)
			{
				asNvodProgEventTable[refEventIndexIndex[index]].uEventRunStatus = RUNNING_NONE;
			}
		}
		else
		{
			asNvodProgEventTable[refEventIndexIndex[index]].uEventRunStatus = RUNNING_NONE;
		}
	}
	memcpy(ref_event,&asNvodProgEventTable[refEventIndexIndex[index]],sizeof(APP_Nvod_Prog_TimeInfo_S));
	FYF_API_semaphore_release(semaphoreNvod);
	return APP_SUCCESS;
}

//检测时移事件是否改变
void APP_Nvod_CheckRefEventStatus(void)
{
	BU32 index;
	APP_Nvod_Prog_TimeInfo_S* tmp;
	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	for(index = 0;index<NVOD_MAX_NO_OF_PROGRAMS;index++)
	{
		if(astNvodProgInfoTable[index].uProgSlotStatus == ELEMENT_OCCUPIED && astNvodProgInfoTable[index].pTimeInfo != NULL)
		{
			tmp = astNvodProgInfoTable[index].pTimeInfo->next;
			if(tmp)
			{
				if(APP_NVOD_CmpToSystemTime(tmp->uEventDateCode, tmp->uStartHour, tmp->uStartMinute, tmp->sDuringTime))
				{					
					astNvodProgInfoTable[index].pTimeInfo->next = tmp->next;
					APP_NVOD_FreeEventNodeByIndex(tmp->index);
					astNvodProgInfoTable[index].update = 1;
				}
			}
		}
	}
	FYF_API_semaphore_release(semaphoreNvod);
}

void APP_Nvod_InitProgList(void)
{
	BU32 index;
	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	g_nvod_shift_service_need_update = 0;
	iNvodProgNoTemp = 0;   /*标记当前EIT正在分析的参考节目段所属的节目号*/
	NvodProgNum = 0;  
	NvodEventNum = 0;

	/* 时移业务对应于实际事件id,参考事件对应于参考事件id*/
	shiftServiceIndexNum = 0;
	memset(shiftServiceIndex,0,NVOD_MAX_NO_OF_PROGRAMS);
	refEventIndexNum = 0;
	memset(refEventIndexIndex,0,NVOD_MAX_NO_OF_EVENTS);
	memset(astNvodProgInfoTable,0,sizeof(APP_Nvod_Program_List_S)*NVOD_MAX_NO_OF_PROGRAMS);
	memset(asNvodProgEventTable,0,sizeof(APP_Nvod_Prog_TimeInfo_S)*NVOD_MAX_NO_OF_EVENTS);
	FYF_API_semaphore_release(semaphoreNvod);
	/*
	for(index = 0;index<NVOD_MAX_NO_OF_PROGRAMS;index++)
	{
		astNvodProgInfoTable[index].pTimeInfo = NULL;//&(astNvodProgInfoTable[index].Head);
		astNvodProgInfoTable[index].uProgRunStatus = STATUS_NODEFINED;
		astNvodProgInfoTable[index].num = 0;
	}
	*/
}
/******************************************
Desc:	进入nvod时相关初始化操作
*******************************************/
void APP_Nvod_API_Init(void)
{
	BU32 novd_freq;
	BU32 tunerState = FYF_TUNER_STATUS_FAIL;
	BU32 i;
	APP_Com_API_ShowLED("nuod");
	APP_SYS_API_GetSysInfo(APP_SYS_NVOD_FREQ_e, &novd_freq);
//	novd_freq = APP_NVOD_FREQ_MHZ;

	APP_Signal_API_SendMsgToTune(novd_freq, 6875, 2);
	/*
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	APP_IPI_SaveTunerInfo(novd_freq, 6875, 2);
	FYF_API_tuner_lock_delivery(0, novd_freq * 1000, 6875, 2, novd_freq);
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
	tunerState = FYF_TUNER_STATUS_FAIL; 	
	for(i =0 ;i < 40 ;i++)
	{
		if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
		{
			tunerState = FYF_TUNER_STATUS_SUCCESS;
			break;
		}				
		FYF_API_thread_sleep(50);
	}
	*/
	FYF_API_nvod_test();
	//每次进入nvod都把上次搜到的信息清空
	//也可以就第一次进入nvod搜，再次进入就不搜了
	APP_Nvod_InitProgList();
	APP_Nvod_SetShiftServiceUpdateFlag(1);
	APP_Nvod_UpdateAllRefEventUpdateStatus();
	APP_EPG_API_StopGetEpg();
	APP_TDT_IPI_StartDemuxTdt();
	APP_Nvod_API_StartGetNvodProg();
}

