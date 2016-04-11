#include "app_common.h"
#include "app_ipi_common.h"



#define UPGRD_DESCRIPTION_LENGTH 80   
#define UPGRD_PARTITION_TABILEEXTID 0x00
#define DLPARTHEAD_TAG 0xEC
#define PARTMODE 0
#define FILEMODE 1

#define DLDOWNLOADINFO_TAG 0xEA
#define DLPARTINFO_TAG 0xEB
#define DLPARTHEAD_TAG 0xEC




typedef enum hiUPGRD_PARTVER_E
{
    app = 0x01,
    loader = 0x02,
    logo = 0x04,
    BootLoader = 0x08,
    LOADER = 0x10,
    LOGO = 0x20,
    BUTT = 0xff
} UPGRD_PARTVER_E;


/*表明解析段的类型*/
typedef enum hiUPGRD_ParSecType_E
{
    UPGRD_EN_TYPE_DLDesptor = 0,/*解析CodeDownLoad descriptor描述符*/
    UPGRD_EN_TYPE_CONTROLHEAD,/*解析升级包控制包头*/
    UPGRD_EN_TYPE_PARTHEAD,/*解析升级包分区信息头*/
    UPGRD_EN_TYPE_DATAGRAM, /*解析升级包datagram段*/
    UPGRD_EN_TYPE_BUTT    /*无解析类型*/
} UPGRD_ParSecType_E;

/*分区信息结构*/
typedef struct hiUPGRD_UpgrdPartInfo_S
{
    BU32   u32Download_mode;  /*升级形式，分区，文件*/
    BU32   u32Download_old_size; /*原来的大小*/
    BU32   u32Download_old_ver_start; /*原来的起始版本*/
    BU32   u32Download_old_ver_end; /*原来的结束版本*/
    BU32   u32Download_ver; /*待更新的版本*/
    BU32   u32Download_addr; /*待更新的地址*/
    BU32   u32Download_size;/*待更新的大小*/
    BU32   u32Download_CRC32; /*待更新的CRC校验*/
    BU32   u32Datagram_number; /*需收取datagram的个数*/
    BU32   u32Datagram_start_addr; /*Partition addr*/
    BU32   u32Download_string_len; /*待更新文件名长度*/
    BU08* pDownload_string; /*待更新文件名的长度指针*/
} UPGRD_UpgrdPartInfo_S;



/*升级包控制信息结构*/
typedef struct hiUPGRD_UpgrdControlHead_S
{
    BU08                  u8Download_table_id; /*升级数据包的table_id*/
    BU08                  u8Partiton_number; /*待升级的分区个数*/
    BU16                 u16FactoryId; /* 厂商ID号 */
    BU32                 u32Software_ver; /*软件版本号- 高层 */
    BU32                 u32HardwareVer; /* 硬件版本号 */
    BU32                 u32Download_data_totalsize; /*整个升级数据包的大小*/
    UPGRD_UpgrdPartInfo_S *pPartInfo; /*分区信息指针*/
    BU08                strUpgrdReason[UPGRD_DESCRIPTION_LENGTH]; /*升级描述信息*/
    BU32                 u32appver; /* 应用程序版本号 */
    BU32                 u32loaderver; /* loader 版本号 */
    BU32                 u32logover; /* 开机画面版本号 */
    BU32                 u32downloaddate; /* 下载日期和时间 */
    BU32                 u32startSn; /* 起始 序列号*/
    BU32                 u32endSn;
} UPGRD_UpgrdControlHead_S;


#define GETFOURBYTE(n, p) \
    do {\
        n  = 0 | (*p << 24) | (*(p + 1) << 16) | (*(p + 2) << 8) | (*(p + 3)); \
        p += 4; \
    } while (0)






extern BS32 e2h_newVersion;
extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern OTA_Info_S g_OtaInfo;

static BU32 otaChannelID = 0;
static BU32 otaFilterID = 0;
static BU32 queueParserOtaEnd = 0;
	
static SectionNode_s *pNitFirstSectionNode = 0;
static BU32	nitChannelID = 0;
static BU32 nitFilterID = 0;
static BU08	isStopParserNit = 0;
static BU32 queueParserNitEnd = 0;
static BU08 g_ota_nit_last_sec_num = 0;
static BU08 g_ota_nit_sec_num = 0;
static BU08 ifNitVerChewcked = 0;
static BU08 ifNitOTAChecked = 0;
/*
void ParserNit_printf (const char *fmt ,...)
{
	char	        report_buffer[4*1024];
	va_list      list;	
	va_start(list, fmt);
	vsprintf(report_buffer, fmt, list);
	va_end(list);
	printf(( report_buffer ));
}
*/
void APP_IPI_SetHasNewChn(BU08 version);


BU08 APP_NIT_API_SetNitVerCheck(BU08 flag)
{
	ifNitVerChewcked = 0;
}

BU08 APP_NIT_API_GetNitVerCheck()
{
	return ifNitVerChewcked;
}


BU08 APP_NIT_API_GetOTACheck()
{
	return ifNitOTAChecked;
}

static BU32 APP_NIT_SIE_SearchBCDToInt(BU08* ptr, BU08 digits, BS32 offset)
{
	BU32 ret=0;
	BS32 i;
	BU08 bcd;
	BU08 offt;
	for(i=0; i<digits; i++) {
		offt = (BU08)(i+offset);
		bcd = ptr[offt>>1];
		bcd = (offt&0x01)? (bcd&0x0f) : (bcd>>4);
		ret = ret*10 + bcd;
	}
	return ret;
}


BU32 APP_NIT_IPI_ParserNitGetVer(BU08 * section, void (*SaveNitInfo)(NIT_Content_s *))
{	
	NIT_Content_s nitContent;
	if(ifNitVerChewcked == 0)
	{
		ifNitVerChewcked = 1;
		nitContent.version = (section[5]&0x3E)>>1;
		nitContent.freqMHz = 0;
		nitContent.qam = 0;
		nitContent.symKbps = 0;
		(*SaveNitInfo)(&nitContent);
	}
	return 1;
}

BU32 APP_IPI_Pow(BU32 v, BU32 c)
{
	BU32 i = 0;
	BU32 rV = 1;
	if(v == 0)
		return 0;
	if(c == 0)
		return 1;
	for(i = 0; i < c; i++)
		rV *= v;
	return rV;
}

INT APP_TPOTA_ParseCodeDesc(BYTE * const pucSectionData, void (*SaveNitInfo)(NIT_Content_s *))
{
	NIT_Content_s nitContent;
	BYTE DlodParsed = 0;
	int iNoOfDataBytes;
	SHORT2BYTE ManuID;
	BYTE DSysDesLength = 0;
	BYTE Table_id;
	short int Pid;
	int iFreq;
	short int wSymrate;
	char cQAMMode;
	BYTE Dlod_Type;
	short int HardVer = 0;
	short int SoftVer = 0;
	BYTE DlodInfoLength = 0;
	BYTE Privatelength = 0;

	BYTE StartNo[2*TPOTA_SERIAL_NUM_MAX];
	BYTE EndNo[2*TPOTA_SERIAL_NUM_MAX];
	UINT StbUpgradeStartNo = 0;
	UINT StbUpgradeEndNo = 0;
	BYTE i;
	iNoOfDataBytes = pucSectionData[1] + 2;
	if (pucSectionData[0] == APP_NIT_PUCSECTIONDATA)//DVT_DL_DESC)
	{
		ManuID.byte.ucByte1 = pucSectionData[2];
		ManuID.byte.ucByte0 = pucSectionData[3];

		DlodInfoLength = pucSectionData[1] - 2;	/*exclude Manu ID */
		DlodParsed = 0;
		
		if (ManuID.sWord16 == APP_NIT_FJ_MANU_ID)//0x42F)//DVT_NL_ID)
		{
			while (DlodParsed < DlodInfoLength)
			{
				if (pucSectionData[4 + DlodParsed] == CABLE_DELIVERY_DESC)
				{
					DSysDesLength = pucSectionData[4 + DlodParsed + 1] + 2;

					Pid = pucSectionData[4 + DlodParsed + DSysDesLength];
					Pid = (Pid << 5) | ((pucSectionData[4 + DlodParsed + DSysDesLength + 1] & 0xf8) >> 3);
					Dlod_Type = (pucSectionData[4 + DlodParsed + DSysDesLength + 1] & 0x07);
					Privatelength = pucSectionData[4 + DlodParsed + DSysDesLength + 2];

					HardVer = pucSectionData[4 + DlodParsed + DSysDesLength + 3];
					HardVer = (HardVer << 8) | (pucSectionData[4 + DlodParsed + DSysDesLength + 4]);
					SoftVer = pucSectionData[4 + DlodParsed + DSysDesLength + 5];
					SoftVer = (SoftVer << 8) | (pucSectionData[4 + DlodParsed + DSysDesLength + 6]);
					Table_id = pucSectionData[4 + DlodParsed + DSysDesLength + 7];

					for (i = 0; i < TPOTA_SERIAL_NUM_MAX; i++)
					{
						StartNo[2 * i] = (pucSectionData[4 + DlodParsed + DSysDesLength + 8 + i] & 0xF0) >> 4;
						StartNo[2 * i + 1] = pucSectionData[4 + DlodParsed + DSysDesLength + 8 + i] & 0x0F;
						
						EndNo[2 * i] = (pucSectionData[4 + DlodParsed + DSysDesLength + 8 + TPOTA_SERIAL_NUM_MAX + i] & 0xF0) >> 4;
						EndNo[2 * i + 1] = pucSectionData[4 + DlodParsed + DSysDesLength + 8 + TPOTA_SERIAL_NUM_MAX + i] & 0x0F;
					}
					
					for (i = 0; i < 2 * TPOTA_SERIAL_NUM_MAX; i++)
					{
						StbUpgradeStartNo += StartNo[2 * TPOTA_SERIAL_NUM_MAX - 1 - i] * (UINT)(APP_IPI_Pow(10, i));
						StbUpgradeEndNo += EndNo[2 * TPOTA_SERIAL_NUM_MAX - 1 - i] * (UINT)(APP_IPI_Pow(10, i));
					}

					{
						int iTemp;

						iTemp =	 ((pucSectionData[4 + DlodParsed + 2] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 2] & 0xF);
						iTemp = iTemp * 100 + ((pucSectionData[4 + DlodParsed + 3] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 3] & 0xF);
						iTemp = iTemp * 100 + ((pucSectionData[4 + DlodParsed + 4] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 4] & 0xF);
						iTemp = iTemp * 100 + ((pucSectionData[4 + DlodParsed + 5] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 5] & 0xF);

						iFreq = (iTemp / 10) / 1000;

						iTemp = ((pucSectionData[4 + DlodParsed + 8] >> 4) & 0xF) * 16 + (pucSectionData[4 + DlodParsed + 8] & 0xF);
						cQAMMode = iTemp - 1;

						iTemp = ((pucSectionData[4 + DlodParsed + 9] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 9] & 0xF);
						iTemp = iTemp * 100 + ((pucSectionData[4 + DlodParsed + 10] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 10] & 0xF);
						iTemp = iTemp * 100 + ((pucSectionData[4 + DlodParsed + 11] >> 4) & 0xF) * 10 + (pucSectionData[4 + DlodParsed + 11] & 0xF);
						iTemp = iTemp * 10 + ((pucSectionData[4 + DlodParsed + 12] >> 4) & 0xF);

						wSymrate = iTemp / 10;
					}
					
					nitContent.downloadPid = Pid;
					nitContent.downloadType = Dlod_Type;
					nitContent.hardwareVersion = HardVer;
					nitContent.softwareVersion = SoftVer;
					nitContent.tableId = Table_id;
					nitContent.startSerialNo = StbUpgradeStartNo;
					nitContent.endSerialNo = StbUpgradeEndNo;
					nitContent.freqMHz = iFreq;
					nitContent.qam = cQAMMode;
					nitContent.symKbps = wSymrate;
					(*SaveNitInfo)(&nitContent);
				}

				DlodParsed = DlodParsed + DSysDesLength + 3 + Privatelength;
			}

		}
	}
	return iNoOfDataBytes;
}


static void APP_TFCA_ParseNitSection(BU08 *head)
{
    BU32  freq;
    BU08   modulation, otaType;
    BU32  dataPid;
    BU32  symbolRate;
    BU16  des_len, tempLen;
	BU32  tempCount;
	BU08 * loop1_data;
	BU08   loop1_len;
	BU08 *dataBuf = head;
	BU32 sn, s_start, s_end;
	
	if (ifNitOTAChecked != 0)
		return;
	/*first loop*/
	/*APP_Printf("Cur OUI 0x%x , DESC 0x%x, --- OTA OUI 0x%x, DESC 0x%x\n",\
		APP_NIT_FJ_MANU_ID, APP_NIT_PUCSECTIONDATA,\
		(((dataBuf[2]<<8)&0xFF00)|dataBuf[3]), dataBuf[0]);
	*/
		
#if 0
	//For test only
	ifNitOTAChecked = 1;
	g_OtaInfo.downloadPid = APP_SYS_UPDATE_PID;
	g_OtaInfo.downloadType = 1;
	g_OtaInfo.softwareVersion = APP_SYS_SOFT_VERSION+1;
	g_OtaInfo.hardwareVersion = APP_SYS_HARD_VERSION;			
	g_OtaInfo.startSerialNo = 0;
	g_OtaInfo.endSerialNo = 0xffffffff;
	
	g_OtaInfo.freqMHz = 602 * 1000;
	g_OtaInfo.qam = 2;
	g_OtaInfo.symKbps = 6875 * 1000;
	APP_Printf("downloadPid %d, downloadType %d, softwareVersion %d, startSerialNo 0x%x, endSerialNo 0x%x\n freqMHz %d, qam %d, symKbps %d\n",\
		g_OtaInfo.downloadPid, g_OtaInfo.downloadType, g_OtaInfo.softwareVersion, \
		g_OtaInfo.startSerialNo, g_OtaInfo.endSerialNo, \
		g_OtaInfo.freqMHz, g_OtaInfo.qam, g_OtaInfo.symKbps);

	APP_SYS_API_NotifyOTA_Detected();
	return;
#endif

	if(APP_NIT_PUCSECTIONDATA == dataBuf[0] && \
		(/*APP_NIT_FJ_MANU_ID*/ 0xaf5 == (((dataBuf[2]<<8)&0xFF00)|dataBuf[3])))
	{
		APP_Printf("====ent ota_desc_parse\n");

	    loop1_data = &(dataBuf[4]);

		APP_Printf("Cur SW 0x%x , HW Ver 0x%x, --- OTA SW 0x%x, HW 0x%x\n",\
			APP_SYS_SOFT_VERSION, APP_SYS_HARD_VERSION,\
		//	(((loop1_data[18]<<8)&0xFF00)|loop1_data[19]),(((loop1_data[16]<<8)&0xFF00)|loop1_data[17]));
			((loop1_data[20]<<24)|(loop1_data[21]<<16)|(loop1_data[22]<<8)|loop1_data[23])
			,((loop1_data[16]<<24)|(loop1_data[17]<<16)|(loop1_data[18]<<8)|loop1_data[19]));
		/*Check version info*/
		if ((((loop1_data[16]<<24)|(loop1_data[17]<<16)|(loop1_data[18]<<8)|loop1_data[19]) == APP_SYS_HARD_VERSION) &&\
			(((loop1_data[20]<<24)|(loop1_data[21]<<16)|(loop1_data[22]<<8)|loop1_data[23]) >  APP_SYS_SOFT_VERSION))
		{
			APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sn);
			
			s_start = (loop1_data[25]&0x0F)*1000000 + \
				((loop1_data[26]&0xF0)>>4)*100000 + (loop1_data[26]&0x0F)*10000 +\
				((loop1_data[27]&0xF0)>>4)*1000 + (loop1_data[27]&0x0F)*100 +\
				((loop1_data[28]&0xF0)>>4)*10 + (loop1_data[28]&0x0F);

			s_end = (loop1_data[29]&0x0F)*1000000 + \
				((loop1_data[30]&0xF0)>>4)*100000 + (loop1_data[30]&0x0F)*10000 +\
				((loop1_data[31]&0xF0)>>4)*1000 + (loop1_data[31]&0x0F)*100 +\
				((loop1_data[32]&0xF0)>>4)*10 + (loop1_data[32]&0x0F);
			
			APP_Printf("Cur SN %d, --- OTA SN FROM %d, TO %d\n", sn,s_start,s_end);

			/*Check SN info*/
			if(((sn>=s_start)&&(sn<=s_end))||((s_start == 0x00000000)&&(s_end == 0x00000000)))
			{
				/*Get OTA parameters, including freq, symb, pid, mode and ota type.*/
				tempCount = (loop1_data[2]&0xF0)>>4;
				tempCount *= 10;
				tempCount += loop1_data[2]&0x0F;
				tempCount *= 10;
				tempCount += (loop1_data[3]&0xF0)>>4;
				tempCount *= 10;
				tempCount +=loop1_data[3]&0x0F;
				freq = tempCount;//*100;
					
				modulation = loop1_data[8] - 1;// + 3;
				
				tempCount = (loop1_data[10]&0xF0)>>4;
				tempCount *= 10;
				tempCount += loop1_data[10]&0x0F;
				tempCount *= 10;
				tempCount += (loop1_data[11]&0xF0)>>4;
				tempCount *= 10;
				tempCount +=loop1_data[11]&0x0F;
				symbolRate = tempCount;
					
				dataPid = (loop1_data[13]<<5)|((loop1_data[14]>>3)&0x1f);

				otaType = loop1_data[14]&0x07;

				g_OtaInfo.downloadPid = dataPid;
				g_OtaInfo.downloadType = otaType;
				g_OtaInfo.softwareVersion = ((loop1_data[20]<<24)|(loop1_data[21]<<16)|(loop1_data[22]<<8)|loop1_data[23]);
				g_OtaInfo.hardwareVersion = APP_SYS_HARD_VERSION;			
				g_OtaInfo.startSerialNo = s_start;
				g_OtaInfo.endSerialNo = s_end;
				
				g_OtaInfo.freqMHz = freq*1000;
				g_OtaInfo.qam = modulation;
				g_OtaInfo.symKbps = symbolRate*1000;

				ifNitOTAChecked = 1;

				APP_PRINT_DEBUG("downloadPid %d, downloadType %d, softwareVersion %d, startSerialNo 0x%x, endSerialNo 0x%x\n freqMHz %d, qam %d, symKbps %d\n",\
					g_OtaInfo.downloadPid, g_OtaInfo.downloadType, g_OtaInfo.softwareVersion, \
					g_OtaInfo.startSerialNo, g_OtaInfo.endSerialNo, \
					g_OtaInfo.freqMHz, g_OtaInfo.qam, g_OtaInfo.symKbps);

				APP_SYS_API_NotifyOTA_Detected();
				return;

			}
		}
	}   
}
static BS32 APP_HISI_ParseNitSection(BU08 *head)
{
    	BU32  freq;
    	BU08  modulation;
    	BU32  symbolRate;
    	BU16  des_len;
	BU08 * loop1_data;
	BU08   loop1_len;
	BU08 *dataBuf = head;
	BU32 sn;
	BU32 i;

    	BU08 deptor_tag;
    	BU08 deptor_len;
    	BU08 download_Type;
    	BU08  table_Id;
    	BU16 factory_Id;
    	BU16 download_Pid;
    	BU16 network_Id;
    	BU32 hardware_Ver;
    	BU32 software_Ver;
    	BU32 serial_Number_Start;
    	BU32 serial_Number_End;
	
	if (ifNitOTAChecked != 0)
		return FYF_ERR;

	if(*dataBuf == APP_NIT_LY_MANU_ID)  /*网络名称描述符*/
	{
		dataBuf++;
		loop1_len = *dataBuf++;
/*		
		printf("NetName:");
		for(i = 0; i < loop1_len; i++)
		{
			printf("%c", *dataBuf++);
		}
		printf("\n");
*/		
	}
	else if(*dataBuf == APP_NIT_PUCSECTIONDATA)
	{
		dataBuf++;
		loop1_len = *dataBuf++;

		download_Type = *dataBuf++;
		table_Id = *dataBuf++;
		factory_Id = (((*dataBuf)<<8) | (*(dataBuf+1)));
		dataBuf +=2;
		download_Pid = (((*dataBuf)<<8) | (*(dataBuf+1)));
		dataBuf +=2;
		network_Id = (((*dataBuf)<<8) | (*(dataBuf+1)));
		dataBuf +=2;
		hardware_Ver = (((*dataBuf)<<24)|((*(dataBuf+1))<<16)|((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		//hardware_Ver = (((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		dataBuf +=4;
		software_Ver = (((*dataBuf)<<24)|((*(dataBuf+1))<<16)|((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		//software_Ver = (((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		dataBuf +=4;
		serial_Number_Start = (((*dataBuf)<<24)|((*(dataBuf+1))<<16)|((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		dataBuf +=4;
		serial_Number_End =(((*dataBuf)<<24)|((*(dataBuf+1))<<16)|((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		dataBuf +=4;

		if(dataBuf - head -2 != loop1_len)
		{
			printf(" APP_HISI_ParseNitSection check loop len %d now %d error tag = 0x%x\n",loop1_len,dataBuf - head,head[0]);
			return FYF_ERR;
		}

		if(factory_Id != APP_NIT_FJ_MANU_ID)
		{
			printf("APP_HISI_ParseNitSection  get factory id 0x%x  need 0x%x \n",factory_Id,APP_NIT_FJ_MANU_ID);
			return FYF_ERR;
		}

		if(hardware_Ver != APP_SYS_HARD_VERSION)
		{
			printf("APP_HISI_ParseNitSection  get hardware id 0x%x  need 0x%x \n",hardware_Ver,APP_SYS_HARD_VERSION);
			return FYF_ERR;
		}

		//自动升级
		if(download_Type == 1)
		{
			if(software_Ver <= APP_SYS_SOFT_VERSION)
			{
				printf("APP_HISI_ParseNitSection  get software id 0x%x  need biger than 0x%x \n",software_Ver,APP_SYS_SOFT_VERSION);
				return FYF_ERR;
			}
			g_OtaInfo.downloadType = 1;
		}
		//选择升级
		else if(download_Type == 2)
		{
			if(software_Ver <= APP_SYS_SOFT_VERSION)
			{
				printf("APP_HISI_ParseNitSection  get software id 0x%x  need biger than 0x%x \n",software_Ver,APP_SYS_SOFT_VERSION);
				return FYF_ERR;
			}
		
			APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sn);
			if(sn<serial_Number_Start || sn>serial_Number_End)
			{
				printf("APP_HISI_ParseNitSection  get sys sn 0x%x check error\n",sn);
				return FYF_ERR;
			}
			g_OtaInfo.downloadType = 1;
				
		}
		//强制升级
		else if(download_Type == 0)
		{
			g_OtaInfo.downloadType = 2;
		}
		else
		{
			g_OtaInfo.downloadType = 0;
			return FYF_ERR;
		}
		
			
		g_OtaInfo.tableId = table_Id;
		g_OtaInfo.downloadPid = download_Pid;
		
		g_OtaInfo.softwareVersion = software_Ver;
		g_OtaInfo.hardwareVersion = hardware_Ver;			
		g_OtaInfo.startSerialNo = serial_Number_Start;
		g_OtaInfo.endSerialNo = serial_Number_End;

		printf("***************************************\n");
		printf("Download_Pid = 0x%X\n", g_OtaInfo.downloadPid);
		printf("Download_Type = 0x%X\n", g_OtaInfo.downloadType);
		printf("HardwareVersion = 0x%X\n", g_OtaInfo.hardwareVersion);
		printf("SoftwareVersion = 0x%X\n", g_OtaInfo.softwareVersion);
		printf("Table_id = 0x%X\n", g_OtaInfo.tableId);
		printf("startSerialNo = %d\n", g_OtaInfo.startSerialNo);
		printf("endSerialNo = %d\n", g_OtaInfo.endSerialNo);
		printf("***************************************\n");
		
	}   
	else if(*dataBuf == APP_NIT_LY_CABLE_ID)
	{
		dataBuf++;
		loop1_len = *dataBuf++;
		freq =  (((*dataBuf)<<24)|((*(dataBuf+1))<<16)|((*(dataBuf+2))<<8)|(*(dataBuf+3)));
		dataBuf += 4;
		dataBuf++;
		dataBuf++;
		modulation = *dataBuf++;
		symbolRate = (((*dataBuf) << 20) |  ((*(dataBuf+1)) << 12) |  ((*(dataBuf+2)) << 4) |  (((*(dataBuf+3)) & 0xf0) >> 4)) ;
		dataBuf += 3;
		dataBuf++;
		if(dataBuf - head -2 != loop1_len)
		{
//			printf(" APP_HISI_ParseNitSection check loop len %d error  tag = 0x%x\n",loop1_len,head[0]);
			return FYF_ERR;
		}

		g_OtaInfo.freqMHz = freq;
		g_OtaInfo.qam = modulation;
		g_OtaInfo.symKbps = symbolRate;
		printf("freqMHz = %d\n", g_OtaInfo.freqMHz);
		printf("qam = %d\n", g_OtaInfo.qam);
		printf("symKbps = %d\n", g_OtaInfo.symKbps);

		//g_OtaInfo.downloadType = 1 选择升级
		//g_OtaInfo.downloadType = 2 强制升级
		if(g_OtaInfo.downloadType != 0)
		{
			ifNitOTAChecked = 1;
			APP_SYS_API_NotifyOTA_Detected();
		}

	}
	return FYF_OK;
}

void APP_DVTCA_ParseNitSection(BU08 * section, void (*SaveNitInfo)(NIT_Content_s *))
{
	BYTE *pucSectionData;

	int iNoOfDataBytes;
	int iInfoLength;
	int iNoOfBytesParsedSoFar;

	SHORT2BYTE stTempData16;
	BYTE ucCurNextIndicator;

	BYTE ucVersionNumber;

	ucVersionNumber = (section[5] & 0x3E) >> 1;
	ucCurNextIndicator = section[5] & 0x01;

	stTempData16.byte.ucByte1 = section[1] & 0xF;
	stTempData16.byte.ucByte0 = section[2];

	pucSectionData = section + 10;


	if (ucCurNextIndicator != 1)
	{
		return;
	}

	/*
	 * read the network_descriptors_length (12)
	 */
	stTempData16.byte.ucByte1 = section[8] & 0xF;
	stTempData16.byte.ucByte0 = section[9];

	iInfoLength = stTempData16.sWord16;

	if (iInfoLength)
	{
		iNoOfBytesParsedSoFar = 0;
		while (iNoOfBytesParsedSoFar < iInfoLength)
		{
			iNoOfDataBytes = APP_TPOTA_ParseCodeDesc(pucSectionData, SaveNitInfo);
			pucSectionData += iNoOfDataBytes;
			iNoOfBytesParsedSoFar += iNoOfDataBytes;
		}
	}
}

static void APP_NIT_IPI_ParserNitOta(BU08 *buff, BS16 buff_len)
{
	BU16 i = 0,j = 0;
	BU16 sec_len = 0;
	BU08 desc_tag = 0;
	BU08 *loop1_header = NULL;
	BU16 loop1_count = 0;
	BU08 *loop2_header = NULL;
	BU16 loop2_count = 0;
	BU08 *loop3_header = NULL;
	BU08 *byte_pos = NULL;
	BU16 loop3_count = 0;
	BU08  nitversion = 0xFF;
	BU16 network_des_len = 0;
	BS16 ts_loop_len = 0,ts_loop_leave = 0,ts_des_len,ts_des_leave;
	BU16 ts_id = 0, o_network_id = 0;
	BU16 trans_des_len = 0;
	BU16 desc_len = 0;
	struct nit_info *temp_header = NULL;
	struct nit_info *temp_com = NULL;
	BU08 ret_loop1 = 0;

	BU16 netDesLen,tsLoopLen,transportDesLen;
	BU08 *sectionPos;
	BU08 *sectionPos1;
	BU08 *sectionPos2;
	BU08 desLen,tag;
	BU08 *section = buff;
	BU08 DVTCASetAreaCode = 0;
//	APP_Printf("====enter ota_nit_parse, buff_len: %d, table id 0x%x, secNum: %d, last SecNum %d\n", buff_len, buff[0], buff[6], buff[7]);

	g_ota_nit_last_sec_num = buff[7];
	
	network_des_len = ((buff[8] &0x0F)<<8) |  buff[9];
	sec_len = ((buff[1] &0x03)<<8)|buff[2];
	ts_loop_leave = ts_loop_len = ((buff[network_des_len + 10] &0x0F)<<8)|buff[network_des_len + 11];

	if(ifNitVerChewcked == 0)
	{
		nitversion = ((buff[5]&0x3E)>>1);
		APP_PRINT_DEBUG("cur nit ver %d\n", nitversion);
		if ((nitversion != APP_SYS_IPI_GetNitVer())&&(0xFF != APP_SYS_IPI_GetNitVer()))
		{
			ifNitVerChewcked = 1;
			APP_SYS_API_NotifyNitUpdated();
		}
	}
	sectionPos1 = &section[10];
	if (network_des_len > 0)
	{
		i = 0;
		while (i < network_des_len)
		{
#ifdef DVT_CA
			//if(APP_IPI_GetTunerFreq() == APP_SYS_IPI_GetMainFreq())
			{
				if(DVTCA_SetAreaCodeInNIT(sectionPos1))
					DVTCASetAreaCode = 1;
			}
#endif
			desLen = sectionPos1[1];
			sectionPos1 = sectionPos1 + desLen + 2;
			i += (desLen + 2);
		}
	}

	netDesLen = network_des_len;//((section[8]&0x0f)<<8)|section[9];
	tsLoopLen = ts_loop_len;//((section[10+netDesLen]&0x0f)<<8)|section[11+netDesLen];
	
	sectionPos = &section[12+netDesLen];
	for(i = 0;i < tsLoopLen;i = i+6+transportDesLen)
	{
		transportDesLen = ((sectionPos[4]&0x0f)<<8)|sectionPos[5];
		sectionPos2 = &sectionPos[6];
		for(j = 0;j < transportDesLen;j = j+2+desLen)
		{
		#ifdef DVT_CA
			//if(APP_IPI_GetTunerFreq() == APP_SYS_IPI_GetMainFreq())
			{
				if(DVTCASetAreaCode == 0)
					DVTCA_SetAreaCodeInNIT(sectionPos2);
			}
		#endif
			tag    = sectionPos2[0];
			desLen = sectionPos2[1];
			sectionPos2 = sectionPos2+2+desLen;
		}
		
		sectionPos = sectionPos+6+transportDesLen;
	}
	
	return;

	/*get ota download info*/
	loop1_header = buff + 10;
	APP_Printf("network_des_len = %d\n", network_des_len);
	while (network_des_len - loop1_count)
	{
		desc_tag = loop1_header[0];
		desc_len = loop1_header[1];

		APP_TFCA_ParseNitSection(loop1_header);
			

		loop1_count += (desc_len + 2);
		loop1_header += (desc_len + 2);
	}

	i = network_des_len + 12;
	while(ts_loop_leave>=6)
	{
		ts_id = (buff[i]<<8)|buff[i+1];
		o_network_id= (buff[i+2]<<8)|buff[i+2+1];
		ts_des_leave = ts_des_len = ((buff[i+4]&0xf)<<8)|buff[i+5];
		j = i + 6;
		while(ts_des_leave>=2)
		{
			desc_tag = buff[j];
			desc_len = buff[j+1];
			APP_TFCA_ParseNitSection(&buff[j]);
			ts_des_leave -=2+desc_len;
			j += 2+desc_len;
		}
		ts_loop_leave -=6+ts_des_len;
		i += 6+ts_des_len;
	}

	#if 0
	//For test only
	ifNitOTAChecked = 1;
	g_OtaInfo.downloadPid = APP_SYS_UPDATE_PID;
	g_OtaInfo.downloadType = 1;
	g_OtaInfo.softwareVersion = 10001;
	g_OtaInfo.hardwareVersion = APP_SYS_HARD_VERSION;			
	g_OtaInfo.startSerialNo = 0;
	g_OtaInfo.endSerialNo = 0xffffffff;
	
	g_OtaInfo.freqMHz = 602 * 1000;
	g_OtaInfo.qam = 2;
	g_OtaInfo.symKbps = 6875 * 1000;
	APP_Printf("downloadPid %d, downloadType %d, softwareVersion %d, startSerialNo 0x%x, endSerialNo 0x%x\n freqMHz %d, qam %d, symKbps %d\n",\
		g_OtaInfo.downloadPid, g_OtaInfo.downloadType, g_OtaInfo.softwareVersion, \
		g_OtaInfo.startSerialNo, g_OtaInfo.endSerialNo, \
		g_OtaInfo.freqMHz, g_OtaInfo.qam, g_OtaInfo.symKbps);

	APP_SYS_API_NotifyOTA_Detected();
	return;
#endif
	//APP_Printf("====exit ota_nit_parse\n");

}

void APP_NIT_IPI_StartDemuxNitOta(APP_ParserFunc parserFunc)
{
	BU08 nitCoef[16] = {0x40,0x80,0x00,0x00,0x00,0x01,0x00};
	BU08 nitMask[16] ={0xFF,0x80,0x00,0x00,0x00,0x01,0xff};
	BU08 nitExcl[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	BU32 returnValue = 0;
	struct ParserFuncNode_s parFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	if(queueParserNitEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserNitEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserNitEnd = FYF_API_queue_create("qPNitE", 1, FYF_THREAD_WAIT_FIFO);		
	}
	

		
	APP_NIT_IPI_StopDemuxNit();
	
	SQL_API_Semaphore_Wait();
/*
	sprintf(g_StrSQL, "DROP TABLE IF EXISTS %s;", NIT_TABLE_NAME);
	SQL_API_Exec(g_StrSQL);
	sprintf(g_StrSQL,"CREATE TABLE %s (freqMHz INTEGER,symKbps INTEGER,QAM INTEGER);",NIT_TABLE_NAME);
	SQL_API_Exec(g_StrSQL);
*/

//	sprintf(g_StrSQL, "DELETE FROM %s;", NIT_TABLE_NAME);
//	SQL_API_Exec(g_StrSQL);
	DBA_API_ClearNitFreqInfo();
	
	SQL_API_Semaphore_Release();	

	APP_IPI_InitSectionNode(&pNitFirstSectionNode);
	isStopParserNit = 0;
	g_ota_nit_sec_num++;
	
	if (g_ota_nit_sec_num > g_ota_nit_last_sec_num)
		g_ota_nit_sec_num = 0;
	
	nitCoef[6] = g_ota_nit_sec_num;
	
	APP_Parser_IPI_StartDemux(1, 0x0010, nitCoef, nitMask, nitExcl, &nitChannelID, &nitFilterID);
	parFuncNode.channelId = nitChannelID;
	parFuncNode.filterId = nitFilterID;
	parFuncNode.ParserFunc = parserFunc;
	APP_IPI_AddPasFuncNode(parFuncNode);
}


BU32 APP_NIT_IPI_ParserNit(BU08 * section, void (*SaveNitInfo)(NIT_Content_s *))
{	
	NIT_Content_s nitContent;
	BU16 netDesLen,tsLoopLen,transportDesLen,i,j;
	BU08 *sectionPos;
	BU08 *sectionPos2;
	BU08 desLen,tag;
	BU32 freqMHz,symKbps;
	BU08 tempQAM;
	APP_CHN_QAM_E qam;
	
	BU32 nitContentCount = 0;


	nitContent.version = (section[5]&0x3E)>>1;
	nitContent.freqMHz = 0;
	nitContent.qam = 0;
	nitContent.symKbps = 0;
	(*SaveNitInfo)(&nitContent);
	
	netDesLen = ((section[8]&0x0f)<<8)|section[9];
	tsLoopLen = ((section[10+netDesLen]&0x0f)<<8)|section[11+netDesLen];
	
	sectionPos = &section[12+netDesLen];
	for(i = 0;i < tsLoopLen;i = i+6+transportDesLen)
	{
		transportDesLen = ((sectionPos[4]&0x0f)<<8)|sectionPos[5];
		sectionPos2 = &sectionPos[6];
		for(j = 0;j < transportDesLen;j = j+2+desLen)
		{

			tag    = sectionPos2[0];
			desLen = sectionPos2[1];
			switch(tag)
			{
			case 0x44://CABLE_DELIVERY_SYSTEM_DESCRIPTOR
				freqMHz = APP_NIT_SIE_SearchBCDToInt(&sectionPos2[2],8,0)/10000;
				symKbps = APP_NIT_SIE_SearchBCDToInt(&sectionPos2[9],7,0)/10;
				tempQAM = sectionPos2[8];
				switch(tempQAM)
				{
				case 1://16
					qam = APP_CHN_QAM_16_e;
					break;
				case 2://32
					qam = APP_CHN_QAM_32_e;
					break;
				case 3://64
					qam = APP_CHN_QAM_64_e;
					break;
				case 4://128
					qam = APP_CHN_QAM_128_e;
					break;
				case 5://256
					qam = APP_CHN_QAM_256_e;
					break;
				default:
					qam = APP_CHN_QAM_64_e;
					break;
				}
				nitContent.freqMHz = freqMHz;
				nitContent.symKbps = symKbps;
				nitContent.qam = qam;
				nitContentCount++;
				(*SaveNitInfo)(&nitContent);
				//nitContents++;
				break;
				default:
					break;
			}
			sectionPos2 = sectionPos2+2+desLen;
		}
		
		sectionPos = sectionPos+6+transportDesLen;
	}
	
	return nitContentCount;
}

void APP_NIT_IPI_SaveOtaInfo(NIT_Content_s *nitContents)//, BU32 nitContentCount)
{
	if (ifNitOTAChecked != 0)
		return;


	if (nitContents->hardwareVersion != APP_SYS_HARD_VERSION)
	{
		return;
	}
	

	g_OtaInfo.downloadPid = nitContents->downloadPid;
	g_OtaInfo.downloadType = nitContents->downloadType;
	g_OtaInfo.softwareVersion = nitContents->softwareVersion;
	g_OtaInfo.hardwareVersion = nitContents->hardwareVersion;
	g_OtaInfo.tableId = nitContents->tableId;
	g_OtaInfo.startSerialNo = nitContents->startSerialNo;
	g_OtaInfo.endSerialNo = nitContents->endSerialNo;
	g_OtaInfo.freqMHz = nitContents->freqMHz;
	g_OtaInfo.qam = nitContents->qam;
	g_OtaInfo.symKbps = nitContents->symKbps;
	FYF_API_printf("***************************************\n");
	FYF_API_printf("Download_Pid = 0x%X\n", nitContents->downloadPid);
	FYF_API_printf("Download_Type = 0x%X\n", nitContents->downloadType);
	FYF_API_printf("HardwareVersion = 0x%X\n", nitContents->hardwareVersion);
	FYF_API_printf("SoftwareVersion = 0x%X\n", nitContents->softwareVersion);
	FYF_API_printf("Table_id = 0x%X\n", nitContents->tableId);
	FYF_API_printf("startSerialNo = %d\n", nitContents->startSerialNo);
	FYF_API_printf("endSerialNo = %d\n", nitContents->endSerialNo);
	FYF_API_printf("freqMHz = %d\n", nitContents->freqMHz);
	FYF_API_printf("qam = %d\n", nitContents->qam);
	FYF_API_printf("symKbps = %d\n", nitContents->symKbps);
	FYF_API_printf("***************************************\n");
	ifNitOTAChecked = 1;	
	
	APP_SYS_API_NotifyOTA_Detected();
}

void APP_NIT_IPI_SaveNitInfo(NIT_Content_s *nitContents)//, BU32 nitContentCount)
{

	if(nitContents->freqMHz == 0
		&& nitContents->qam == 0
		&& nitContents->symKbps == 0)
	{
		BU08 version = 0;
		version = app_system_data.nitVersion;
		APP_Search_printf("APP_NIT_IPI_SaveNitInfo oldVer = 0x%X, nowVer = 0x%X\n", version, nitContents->version);
		if(version != nitContents->version)
		{
			version = nitContents->version;			
			APP_IPI_SetHasNewChn(version);
			
		}
		else
		{
			app_system_data.nitVersion = nitContents->version;
		}
		APP_Search_printf("isHasNewVersion = %d\n", APP_API_GetIsHasNewChn());
	}
	else
	{
//		sprintf(g_StrSQL,"insert into %s (freqMHz,symKbps,QAM) values(%d,%d,%d);",NIT_TABLE_NAME,nitContents->freqMHz,nitContents->symKbps,nitContents->qam);					
//		SQL_API_Exec(g_StrSQL);
		DBA_API_AddNitFreqInfo(nitContents->freqMHz,nitContents->symKbps,(BU08)(nitContents->qam));
		FYF_API_printf("NIT freqMhz = %d\n", nitContents->freqMHz);
	}
}

void APP_NIT_IPI_CheckNitInfo(NIT_Content_s *nitContents)//, BU32 nitContentCount)
{
	if(nitContents->freqMHz == 0
		&& nitContents->qam == 0
		&& nitContents->symKbps == 0)
	{
		BU08 version = 0;
		version = app_system_data.nitVersion;
		APP_Search_printf("APP_NIT_IPI_SaveNitInfo oldVer = 0x%X, nowVer = 0x%X\n", version, nitContents->version);
		if(version != nitContents->version)
		{
			APP_IPI_SetHasNewChn(nitContents->version);
		}
		APP_Search_printf("isHasNewVersion = %d\n", APP_API_GetIsHasNewChn());
	}	
}


BU32 APP_NIT_IPI_GetQueueParserNitEnd(void)
{
	return queueParserNitEnd;
}

void APP_NIT_IPI_StartDemuxNit(APP_ParserFunc parserFunc)
{
	BU08 nitCoef[1] = {0x40};
	BU08 nitMask[1] = {0xFF};
	BU08 nitExcl[1] = {0x00};
	BU32 returnValue = 0;
	struct ParserFuncNode_s parFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	if(queueParserNitEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserNitEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserNitEnd = FYF_API_queue_create("qPNitE", 1, FYF_THREAD_WAIT_FIFO);		
	}

		
	APP_NIT_IPI_StopDemuxNit();
	
	SQL_API_Semaphore_Wait();
/*
	sprintf(g_StrSQL, "DROP TABLE IF EXISTS %s;", NIT_TABLE_NAME);
	SQL_API_Exec(g_StrSQL);
	sprintf(g_StrSQL,"CREATE TABLE %s (freqMHz INTEGER,symKbps INTEGER,QAM INTEGER);",NIT_TABLE_NAME);
	SQL_API_Exec(g_StrSQL);
*/

//	sprintf(g_StrSQL, "DELETE FROM %s;", NIT_TABLE_NAME);
//	SQL_API_Exec(g_StrSQL);
	DBA_API_ClearNitFreqInfo();
	
	SQL_API_Semaphore_Release();	

	APP_IPI_InitSectionNode(&pNitFirstSectionNode);
	isStopParserNit = 0;
	APP_Parser_IPI_StartDemux(1, 0x0010, nitCoef, nitMask, nitExcl, &nitChannelID, &nitFilterID);
	parFuncNode.channelId = nitChannelID;
	parFuncNode.filterId = nitFilterID;
	parFuncNode.ParserFunc = parserFunc;
	APP_IPI_AddPasFuncNode(parFuncNode);
}



void APP_NIT_IPI_StopDemuxNit(void)
{
	isStopParserNit = 1;
	APP_Parser_IPI_StopDemux(&nitChannelID, &nitFilterID);
	APP_IPI_ClearSection(&pNitFirstSectionNode);
}

void APP_NIT_IPI_StartParserNit(BU08 *section, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;
	BU32 nitContentCount = 0;
	//NIT_Content_s nitContents[MAX_NIT_CONTENT_COUNT];
	if(0x40 != section[0])
	{
		APP_PRINT_ERROR("APP_NIT_IPI_StartParserNit table_id = 0x%x\n",section[0]);
		return;
	}
	if (isStopParserNit)
	{
		return;
	}
	
	nitContentCount = APP_NIT_IPI_ParserNit(section, APP_NIT_IPI_SaveNitInfo);
	//SaveNitInfo(nitContents, nitContentCount);
	
	if(pNitFirstSectionNode==0)
		return ;
	
	APP_IPI_AddSection(pNitFirstSectionNode, section[6]);
	if ((section[7] + 1) == (BU08)APP_IPI_GetSectionCount(pNitFirstSectionNode))
	{
		APP_NIT_IPI_StopDemuxNit();
		msgSend.qlstWordOfMsg = 1;
		FYF_API_queue_send(APP_NIT_IPI_GetQueueParserNitEnd(), &msgSend);
	}
}

void APP_NIT_IPI_StartParserNitGetVerAndOTA(BU08 *section, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;
	//NIT_Content_s nitContents[MAX_NIT_CONTENT_COUNT];
	if(0x40 != section[0])
	{
		APP_PRINT_ERROR("APP_NIT_IPI_StartParserNitGetVerAndOTA table_id = 0x%x\n",section[0]);
		return;
	}
	if (isStopParserNit)
	{
		return;
	}
	APP_Printf("%s\n", __FUNCTION__);
	/*得到NIT版本信息*/
//	APP_NIT_IPI_ParserNitGetVer(section, APP_NIT_IPI_CheckNitInfo);
	/*得到OTA信息*/
//	APP_NIT_IPI_ParserNit(section, APP_NIT_IPI_SaveOtaInfo);

	//SaveNitInfo(nitContents, nitContentCount);
	APP_NIT_IPI_ParserNitOta(section, 0);
	
	if(pNitFirstSectionNode == 0)
		return ;
		
	APP_IPI_AddSection(pNitFirstSectionNode, section[6]);
	if ((section[7] + 1) == (BU08)APP_IPI_GetSectionCount(pNitFirstSectionNode))
	{
		APP_NIT_IPI_StopDemuxNit();
		msgSend.qlstWordOfMsg = 1;
		FYF_API_queue_send(APP_NIT_IPI_GetQueueParserNitEnd(), &msgSend);
	}
}

void APP_NIT_IPI_StartParserNitGetVer(BU08 *section, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;
	//NIT_Content_s nitContents[MAX_NIT_CONTENT_COUNT];
	
	if (isStopParserNit)
	{
		return;
	}

	/*得到NIT版本信息*/
	APP_NIT_IPI_ParserNitGetVer(section, APP_NIT_IPI_CheckNitInfo);

	//SaveNitInfo(nitContents, nitContentCount);
	if(pNitFirstSectionNode == 0)
		return ;
		
	APP_IPI_AddSection(pNitFirstSectionNode, section[6]);
	if ((section[7] + 1) == (BU08)APP_IPI_GetSectionCount(pNitFirstSectionNode))
	{
		APP_NIT_IPI_StopDemuxNit();
		msgSend.qlstWordOfMsg = 1;
		FYF_API_queue_send(APP_NIT_IPI_GetQueueParserNitEnd(), &msgSend);
	}
}


//2010-03-12 cwd add



BU08 ParseControlHeadDesc(BU08* pu8DesData, BS32 s32DescLen,
                                    UPGRD_UpgrdControlHead_S* pstruUpdControlHead)
{
    BU08 u8Tag   = 0;
    BS32 s32Len = 0;
    const BU08 *u8Data = NULL;

    BU08 u8Temp   = 0;
    BU32 u32Temp = 0;
    BU08 u8Key_control;
    BU08 u8flag = 1;
    BU08 i = 0;
    UPGRD_UpgrdControlHead_S* pTmpControlHead = pstruUpdControlHead;
    UPGRD_UpgrdPartInfo_S* pstruUpgrdPartInfo = NULL;
    BU08* pstr = NULL;
    BS32 s32DataLen = 0;
	BU32 sn;

    APP_Printf("parse controlhead desc: --start--\n");

    if ((NULL == pu8DesData) || (NULL == pTmpControlHead))
    {
        APP_Printf("parse controlhead desc: -------------end-------------\n" );
        return APP_FALSE;
    }

    /* don't parse crc*/
    while (s32DescLen > 4)
    {
        /* get description tag */
        u8Tag = *pu8DesData++;
        printf("tag = %x\n", u8Tag );

        /* get description length*/
        s32Len = *pu8DesData++;
        printf("tag = %x\n", s32Len );

        if (s32Len == 0)
        {
            APP_Printf("parse controlhead desc: -------------end-------------\n" );
            return APP_FALSE;
        }

        u8Data = pu8DesData;
        pu8DesData += s32Len;

        printf("Parse: s32DescLen = %x", s32DescLen);
        s32DescLen -= (s32Len + 2);
        printf("Parse: s32Len = %x", s32Len);

        switch (u8Tag)
        {
            APP_Printf("parse controlhead desc loop: u8Tag is %d\n", u8Tag );
        case DLDOWNLOADINFO_TAG:
        {
            /*get STB_ManufactuerID*/
            GETFOURBYTE(u32Temp, u8Data);
            pTmpControlHead->u16FactoryId = u32Temp;
            APP_Printf("u16FactoryId Id is %d \n", pTmpControlHead->u16FactoryId);

            /*get hardware version*/
            GETFOURBYTE(u32Temp, u8Data);
            pTmpControlHead->u32HardwareVer = u32Temp;
            APP_Printf("u32HardwareVer ver is 0x%x \n", pTmpControlHead->u32HardwareVer);

            // UPGRD_Printf("hardver is %x", u32Temp);

            /*get software version*/
            GETFOURBYTE(u32Temp, u8Data);
            pTmpControlHead->u32Software_ver = u32Temp;
            APP_Printf("u8Software ver is 0x%x\n", pTmpControlHead->u32Software_ver);

            /*get download_table_id*/
            pTmpControlHead->u8Download_table_id = *u8Data++;      /*modify by l55561 5.14*/
   //         g_u8download_tableid = pTmpControlHead->u8Download_table_id;
            APP_Printf("table id  is 0x%x\n", pTmpControlHead->u8Download_table_id);

            /* AI7D01639 2007/10/12: delete verity table id code*/

            /*parse key_control key word*/
            u8Key_control = *u8Data++;

            APP_Printf("u8Key_control is %x\n", u8Key_control);

            /*get sn range*/
            GETFOURBYTE(u32Temp, u8Data);
            APP_Printf("seri begin is %x\n", u32Temp);
            pTmpControlHead->u32startSn = u32Temp;

            GETFOURBYTE(u32Temp, u8Data);
            APP_Printf("seri end is %x\n", u32Temp);
            pTmpControlHead->u32endSn = u32Temp;

            /*get upgrade date*/
            GETFOURBYTE(u32Temp, u8Data);
            APP_Printf("updata date is %x\n", u32Temp);
            pTmpControlHead->u32downloaddate = u32Temp;

            /*get other versions*/
            while (((u8Key_control != 0) && (u8Key_control != 0xff) && (u8flag)))
            {
                u8flag = u8Key_control & (1 << i);
                switch (u8flag)
                {
                case app:
                    GETFOURBYTE(u32Temp, u8Data);
                    APP_Printf("App is %x\n", u32Temp);
                    pTmpControlHead->u32appver = u32Temp;
                    break;
                case loader:
                    GETFOURBYTE(u32Temp, u8Data);
                    APP_Printf("Kernel is %x\n", u32Temp);
                    break;
                case logo:
                    GETFOURBYTE(u32Temp, u8Data);
                    APP_Printf("CA is %x\n", u32Temp);
                    break;
                case BootLoader:
                    GETFOURBYTE(u32Temp, u8Data);
                    APP_Printf("BootLoader is %x\n", u32Temp);
                    break;
                case LOADER:
                    GETFOURBYTE(u32Temp, u8Data);
                    APP_Printf("LOADER is %x\n", u32Temp);
                    break;
                case LOGO:
                    GETFOURBYTE(u32Temp, u8Data);
                    APP_Printf("LOGO is %x\n", u32Temp);
                    break;
                default:
                    break;
                }

                i++;
            }

            /*get product type*/
            u8Temp  = *u8Data++;
            u8Data += u8Temp;
            break;
        }
        case DLPARTINFO_TAG:
        {
            pTmpControlHead->u32Download_data_totalsize = u8Data[0] << 24 | u8Data[1] << 16 | u8Data[2] << 8
                                                          | u8Data[3];

            u8Data += 4;

            /*get partition number*/
            pTmpControlHead->u8Partiton_number = *u8Data;
            APP_Printf("controlhead: lesslength: pTmpControlHead->u8Partiton_number = %d\n",
                         pTmpControlHead->u8Partiton_number);

            u8Data += 2;

            s32DataLen = u8Data[0] << 8 | u8Data[1];
            u8Data += 2;    /*add by l55561 5.14*/

            if (s32DataLen >= s32Len)
            {
                APP_Printf("controlhead: lesslength: desc length = %d, data length = %d\n",
                             s32Len, s32DataLen);
                APP_Printf("parse controlhead desc: ==end==\n" );
                return APP_FALSE;
            }

            /*allocate memory*/
            u32Temp = (pTmpControlHead->u8Partiton_number) * sizeof(UPGRD_UpgrdPartInfo_S);
            pstruUpgrdPartInfo = (UPGRD_UpgrdPartInfo_S*)malloc(u32Temp);
            if (NULL == pstruUpgrdPartInfo)
            {
                APP_Printf("controlhead:malloc error");
                return APP_FALSE;
            }

            memset(pstruUpgrdPartInfo, 0, u32Temp);
            pTmpControlHead->pPartInfo = pstruUpgrdPartInfo;

            while (s32DataLen > 0)
            {
                /*download_mode*/
                u8Temp = *u8Data++;
                s32DataLen--;
                if ((PARTMODE != u8Temp) && (FILEMODE != u8Temp))
                {
                    return APP_FALSE;
                }

                pstruUpgrdPartInfo->u32Download_mode = u8Temp;
                APP_Printf("controlhead:pstruUpgrdPartInfo->u32Download_mode is %d\n",
                             pstruUpgrdPartInfo->u32Download_mode);

                /*download_mode_data_len*/
                u8Temp = *u8Data++;
                s32DataLen--;
                s32DataLen -= u8Temp;

                if (PARTMODE == pstruUpgrdPartInfo->u32Download_mode)
                {
                    /*get upgrade address*/
                    GETFOURBYTE(u32Temp, u8Data);
                    pstruUpgrdPartInfo->u32Download_addr = u32Temp;

                    /*get upgrade size*/
                    GETFOURBYTE(u32Temp, u8Data);
                    pstruUpgrdPartInfo->u32Download_size = u32Temp;
                    APP_Printf("controlhead:pstruUpgrdPartInfo->u32Download_size is %\n",
                                 pstruUpgrdPartInfo->u32Download_size);

                    APP_Printf("controlhead:u8Data is %x\n", u8Data[0]);
                    APP_Printf("controlhead:u8Data is %x\n", u8Data[1]);
                    APP_Printf("controlhead:u8Data is %x\n", u8Data[2]);
                    APP_Printf("controlhead:u8Data is %x\n", u8Data[3]);

                    /*get crc value*/
                    GETFOURBYTE(u32Temp, u8Data);

                    pstruUpgrdPartInfo->u32Download_CRC32 = u32Temp;
                    APP_Printf("controlhead:pstruUpgrdPartInfo->u32Download_CRC32 is %x\n",
                                 pstruUpgrdPartInfo->u32Download_CRC32);
                }
                else
                {
                    /*get length of file name*/
                    u8Temp = *u8Data++;
                    s32DataLen--;
                    pstruUpgrdPartInfo->u32Download_string_len = u8Temp;
                    u8Temp += 1;

                    /*get file name*/
                    pstr = (BU08*)malloc(u8Temp);
                    if (NULL == pstr)
                    {
                        return APP_FALSE;
                    }

                    pstruUpgrdPartInfo->pDownload_string = pstr;
                    memset(pstr, 0, u8Temp);
                    strncpy(pstr, (BU08*)u8Data, u8Temp - 1);
                    u8Data += pstruUpgrdPartInfo->u32Download_string_len;

                    /*get file size*/
                    GETFOURBYTE(u32Temp, u8Data);
                    pstruUpgrdPartInfo->u32Download_size = u32Temp;

                    /*get crc value*/
                    GETFOURBYTE(u32Temp, u8Data);
                    pstruUpgrdPartInfo->u32Download_CRC32 = u32Temp;
                }

                pstruUpgrdPartInfo += 1;
            }

            /*get file ugrade description*/
            u8Temp = *u8Data++;
            if (UPGRD_DESCRIPTION_LENGTH <= u8Temp)
            {
                u8Temp = UPGRD_DESCRIPTION_LENGTH - 1;
            }

            strncpy(pTmpControlHead->strUpgrdReason, (BU08*)u8Data,
                      u8Temp);
            pTmpControlHead->strUpgrdReason[u8Temp] = '\0';
            break;
        }
        default:
            break;
        }
    }


	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sn);
			

	if (pTmpControlHead->u16FactoryId == APP_NIT_FJ_MANU_ID && pTmpControlHead->u32HardwareVer == APP_SYS_HARD_VERSION
		&& pTmpControlHead->u32Software_ver > APP_SYS_SOFT_VERSION
		&& ((sn>=pTmpControlHead->u32startSn && sn<=pTmpControlHead->u32endSn )||(pTmpControlHead->u32startSn == 0x00000000 && pTmpControlHead->u32endSn == 0x00000000)))
	{
	
		g_OtaInfo.downloadPid = APP_SYS_UPDATE_PID;
		g_OtaInfo.downloadType = 1;
		g_OtaInfo.softwareVersion = pTmpControlHead->u32Software_ver;
		g_OtaInfo.hardwareVersion = APP_SYS_HARD_VERSION;			
		g_OtaInfo.startSerialNo = pTmpControlHead->u32startSn;
		g_OtaInfo.endSerialNo = pTmpControlHead->u32endSn;
			
		g_OtaInfo.freqMHz = APP_IPI_GetTunerFreq() * 1000; //APP_OTA_FREQ_MHZ*1000;
		g_OtaInfo.qam = 2;
		g_OtaInfo.symKbps = 6875*1000;
		
		ifNitOTAChecked = 1;

		APP_PRINT_DEBUG("downloadPid %d, downloadType %d, softwareVersion %d, startSerialNo 0x%x, endSerialNo 0x%x\n freqMHz %d, qam %d, symKbps %d\n",\
			g_OtaInfo.downloadPid, g_OtaInfo.downloadType, g_OtaInfo.softwareVersion, \
			g_OtaInfo.startSerialNo, g_OtaInfo.endSerialNo, \
			g_OtaInfo.freqMHz, g_OtaInfo.qam, g_OtaInfo.symKbps);

		APP_SYS_API_NotifyOTA_Detected();
	}

    return APP_TRUE;
}
    




void APP_NIT_IPI_StartParseOTAInfo(BU08 *section, BU32 channelId, BU32 filterId)
{
	BS32 s32DataLength = 0;
    BU16 u16tableextId = 0;
    BS32 s32Ret = 0;
	BS32 s32Length = 0;
	UPGRD_UpgrdControlHead_S pstruUpdControlHead;
	static FYF_QUEUE_MESSAGE msgSend;
	if (ifNitOTAChecked != 0)
		return;

    APP_Printf("==start parse control head==\n" );
    if ((NULL == section))
    {
        APP_Printf("==end parse control head: para error==\n" );
        return ;
    }
	if (section[0] != 0xfe)
	{
		APP_PRINT_ERROR("APP_NIT_IPI_StartParseOTAInfo table_id = 0x%x\n",section[0]);
		return;
	}
    /* get section length*/
    s32Length = (BS32)((section[1] << 8) | section[2]) & 0x0fff;
    APP_Printf("controlhead: section length = %d\n", s32Length );

    s32DataLength = s32Length + 3 - 4;  /*add by l55561 5.14*/

    section += 3;
    s32DataLength -= 3;

    /*parse table_id_extension*/
    u16tableextId = (BU16)((section[0] << 8) | section[1]);

   // g_GetedBiggerVersion = (section[2] << 8) | section[4];

    section += 5;
    s32DataLength -= 5;

    /* parse download_info description */
    APP_Printf("parse controlhead table:s32DataLength is %d\n", s32DataLength );
    if (!ParseControlHeadDesc( section, s32DataLength, &pstruUpdControlHead))
    {
        APP_Printf("\n[ERROR]==end parse download info: parse error ==\n" );
        return ;
    }
	msgSend.qlstWordOfMsg = 1;
	FYF_API_queue_send(APP_NIT_IPI_GetQueueParserOtaEnd(), &msgSend);
	APP_NIT_API_StopDemuxOTAInfo();
    return ;
}

BU32 APP_NIT_IPI_GetQueueParserOtaEnd(void)
{
	return queueParserOtaEnd;
}


void APP_NIT_API_StopDemuxOTAInfo()
{
	APP_Parser_IPI_StopDemux(&otaChannelID, &otaFilterID);
}
void APP_NIT_API_StartDemuxOTAInfo(APP_ParserFunc parserFunc)
{
	BU08 otaCoef[1] = {0xfe};
	BU08 otaMask[1] = {0xff};
	BU08 otaExcl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	if(queueParserOtaEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserOtaEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserOtaEnd = FYF_API_queue_create("qPOTAE", 1, FYF_THREAD_WAIT_FIFO);		
	}
	
		
	APP_NIT_API_StopDemuxOTAInfo();

	
	APP_Parser_IPI_StartDemux(1, APP_SYS_UPDATE_PID, otaCoef, otaMask, otaExcl, &otaChannelID, &otaFilterID);
	parFuncNode.channelId = otaChannelID;
	parFuncNode.filterId = otaFilterID;
	parFuncNode.ParserFunc = parserFunc;
	APP_IPI_AddPasFuncNode(parFuncNode);
}



