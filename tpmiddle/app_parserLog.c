#include "app_common.h"
#include "app_ipi_common.h"

#define FYF_FLS_LOGO_DATA_SIZE      (1024*128)
#define LogoMaxTableIdExtenNum		10/*max number should be 256*/
#define LogoMaxSectionNum			256
#define LogoPID						7072
#define LogoTableID					202
#define LogoMaxDataK				256
#define LogoOneSectionLength		1152/*1024*/
#define LogoFILTER_DEPTH_SIZE       16

//#ifdef SYS_CONFIG_UPDATE_STARTLOGO
extern BU08 data_logo_buf[1024*64];
//#endif
static BU32	logChannelID = 0;
static BU32 logFilterID = 0;


BOOLEAN   bDrawLogoDisplayScollMenu=false;

BYTE  	 LogoReadRamFlash = 0;
BOOLEAN  LogoStopFilter = false;
BU08   	 LogoStartFlag = 0;	
BU08 	 LogoVersionNO = 0xff;
BU08 	 OldLogoVersionNO = 0xff;
BS32	 iLogoWaitTime=5;
BYTE 	 LogoVersionNum;
BU16 	 LogoTableIdExtension[LogoMaxTableIdExtenNum];
BU08 	 LogoSectionNum[LogoMaxSectionNum];

//LogoLengthTag_p= (SHORT*)malloc(sizeof(SHORT)*LogoMaxDataK);
BU16	 LogoLengthTag_p[LogoMaxDataK];
BS32	 LogoCacheSectionNum=0;
BU32 	 LogoFileTotalLength = 0;
BU32  	 OldLogoFileTotalLength = 0;
BU08     *data_bitmap_Logo = 0;

void APP_LOG_IPI_StartParserLog(BU08 *section, BU32 channelId, BU32 filterId);
void APP_LOG_IPI_StopDemuxLog(void);
static void LOGO_DefaultData(void);



BU08 TPSYSTEM_GetLogoVerNumValue(void)
{
	return LogoVersionNO;
}
/* ----------------------------------------------------------------------------
得到搜索LOGO数据的状态和长度
---------------------------------------------------------------------------- */
BYTE    bGetLogoRamFlashValue(void)
{
	return  LogoReadRamFlash;
}
ULONG   iGetLogoFileTotalLength(void)
{
	return  LogoFileTotalLength;
}


/* ----------------------------------------------------------------------------
组合LOGO的数据
---------------------------------------------------------------------------- */
static void LOGO_SaveData(BYTE* data_p, SHORT dataLength, SHORT blockNum, ULONG fileLength)
{
	BU32 i;
	APP_PRINT_INFO("LOGO_SaveData_Enter blockNum = %d,dataLength = %d\n",blockNum,dataLength);
	if(blockNum>=LogoMaxDataK ||dataLength>=1024)
		return;

	LogoLengthTag_p[blockNum] = dataLength;
	APP_PRINT_INFO("LOGO_SaveData_Enter LogoLengthTag_p[%d] = %d \n",blockNum,dataLength);
	for(i=0;i<dataLength;i++)
		data_bitmap_Logo[blockNum*1024 + i] = data_p[i];
}
/**********************************************************************************
**********************************************************************************/ 
static void LOGO_ArrangeData(void)
{
	int i,j;
	int temp=0;
	APP_PRINT_INFO("LOGO_ArrangeData_Enter, LogoLengthTag_p[0] = %d\n",LogoLengthTag_p[0]);
	if(LogoLengthTag_p[0]==0)
		return;
	
	for(i=1;i<LogoMaxDataK;i++)
	{
		temp = temp + LogoLengthTag_p[i-1];

		if(temp<=LogoFileTotalLength)
		{
			LogoLengthTag_p[i-1] = 0;
			for(j=0;j<LogoLengthTag_p[i];j++)
				data_bitmap_Logo[temp+j] = data_bitmap_Logo[i*1024+j];
		}
		else
		{
			break;
		}	
	}		
	
	LogoReadRamFlash = 3;
		
}

/****************************************************
将新的LOGO数据保存到FLASH中
*****************************************************/
void APP_LOG_IPI_SaveLogoData(BU08 * data,BU32 len,BU08 version)
{
	FYF_FLS_IOCTL_PARAM_s fyf_fls_ioctl_param;
	APP_PRINT_INFO("  APP_LOG_IPI_SaveLogoData  len = 0x%x version = 0x%x \n",len,version);
	fyf_fls_ioctl_param.buffer = data;
	fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-1] = (len&0x000000ff);
	fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-2] = (len&0x0000ff00)>>8;
	fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-3] = (len&0x00ff0000)>>16;
	fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-4] = (len&0xff000000)>>24;
	fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-5] = version;
	fyf_fls_ioctl_param.bufferSize = FYF_FLS_LOGO_DATA_SIZE;
	FYF_API_fls_ioctl(FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_ADVLOGO, (BU32)&fyf_fls_ioctl_param);
}
/****************************************************
获取FLASH中LOGO的长度和版本号
*****************************************************/
void APP_LOG_IPI_GetLogoData(BU08 * data,BU32 * len,BU08 * version)
{
	BU32 temp = 0;
	BU32 temp0 = 0;
	BU32 temp1 = 0;
	BU32 temp2 = 0;
	BU32 temp3 = 0;
	FYF_FLS_IOCTL_PARAM_s fyf_fls_ioctl_param;
	fyf_fls_ioctl_param.buffer = data;
	fyf_fls_ioctl_param.bufferSize = FYF_FLS_LOGO_DATA_SIZE;
	FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_ADVLOGO, (BU32)&fyf_fls_ioctl_param);

	temp  = 0;
	temp0 = 0;
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp 		= fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-4];
	if (temp > 0)
	{
		return;
	}
	temp3 		= (temp & 0xff)<<24;
	temp = 0;
	temp 		= fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-3];
	temp2 		= (temp & 0xff)<<16;
	temp = 0;
	temp 		= fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-2];
	temp1 		= (temp & 0xff)<<8;
	temp = 0;
	temp0 		= fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-1];
	memcpy(data_logo_buf,fyf_fls_ioctl_param.buffer,64*1024);
	*len 	= temp3 | temp2 | temp1 | temp0;
	if (*len > 64 * 1024)
		return;
	*version = fyf_fls_ioctl_param.buffer[FYF_FLS_LOGO_DATA_SIZE-5];
}

BOOLEAN   APP_LOG_IPI_IsSaveFlash(void)
{
	APP_PRINT_INFO(" APP_LOG_IPI_IsSaveFlash LogoReadRamFlash = %d LogoFileTotalLength = 0x%x OldLogoFileTotalLength = 0x%x",LogoReadRamFlash,LogoFileTotalLength,OldLogoFileTotalLength);
	if(LogoReadRamFlash == 3 /*&& iLogoWaitTime>5*/)
	{		
		if((LogoFileTotalLength < 64*1024) && (OldLogoFileTotalLength != LogoFileTotalLength))
		{
			return true;
		}
	}

	return false;
}

/* ----------------------------------------------------------------------------
分析LOGO的数据
---------------------------------------------------------------------------- */
static void  LOGO_ParseSection( BYTE *data_p )
{
	BYTE 		SectionSyntaxIndicator = 0;
	SHORT 		SectionLength = 0;
	SHORT 		TableID_Extension = 0;
	BYTE 		VersionNumber = 0;
	BYTE 		CurrentNextIndicator = 0;
	BYTE 		SectionNumber = 0;
	BYTE 		LastSectionNumber = 0;
	SHORT 		BlockNumber = 0;
	ULONG 		FileLength = 0;
	ULONG 		StopTime = 0;
	SHORT		SectionLogoLength = 0;

	BYTE		*SectionData_p;
	SHORT2BYTE	stTempData16;
	ULONG 		temp0 = 0;
	ULONG 		temp1 = 0;
	ULONG 		temp2 = 0;
	ULONG 		temp3 = 0;
	ULONG 		temp = 0;

	if (LogoStopFilter == true)
		return;
	
	SectionData_p = data_p;

	if(SectionData_p[0] != LogoTableID)
		return;

	SectionSyntaxIndicator = (SectionData_p[1]&0x80)>>8;
	
	stTempData16.byte.ucByte1	= SectionData_p[1] & 0xF;
	stTempData16.byte.ucByte0	= SectionData_p[2];
	SectionLength 			= stTempData16.sWord16;
	
	stTempData16.byte.ucByte1	= SectionData_p[3];
	stTempData16.byte.ucByte0	= SectionData_p[4];
	TableID_Extension 			= stTempData16.sWord16;
	
	VersionNumber 		= (SectionData_p[5] & 0x3E)>>1;
	CurrentNextIndicator 	= SectionData_p[5] & 0x01;

	SectionNumber 		= SectionData_p[6];
	LastSectionNumber 	= SectionData_p[7];

	LogoReadRamFlash = 1;
	#if 0
	if (VersionNumber == OldLogoVersionNO)
	{
		APP_Printf("VersionNumber = %d - %d equal\n", VersionNumber,OldLogoVersionNO);
		APP_Printf("LogoCache Section OK!!\n");
		LogoReadRamFlash = 2;
		LogoStopFilter = true;
		return;
	}
	#endif
	LogoStartFlag = 1;
	APP_PRINT_INFO("LogoCacheSectionNum = %d,LastSectionNumber = %d\n",LogoCacheSectionNum,LastSectionNumber);
	if(LogoCacheSectionNum>LastSectionNumber)
	{
		LogoVersionNO = VersionNumber;
		APP_LOG_IPI_StopDemuxLog();
		LOGO_ArrangeData();
		LogoStopFilter = true;
		return;
	}
	
	if(SectionNumber >= LogoMaxSectionNum || LogoSectionNum[SectionNumber] == 1 || LogoCacheSectionNum>LastSectionNumber)
		return;

	LogoSectionNum[SectionNumber] = 1;
	LogoCacheSectionNum++;
	
	stTempData16.byte.ucByte1	= SectionData_p[8];
	stTempData16.byte.ucByte0	= SectionData_p[9];
	BlockNumber  = stTempData16.sWord16;
	
	temp 		= SectionData_p[10];
	temp3 		= temp<<24;
	temp = 0;
	temp	 	= SectionData_p[11];
	temp2 		= temp<<16;
	temp = 0;
	temp 		= SectionData_p[12];
	temp1 		= temp<<8;
	temp = 0;
	temp0 		= SectionData_p[13];
	FileLength 	= temp3 | temp2 | temp1 | temp0;
	LogoFileTotalLength = FileLength;

	temp0 = 0;
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp = 0;
	temp 		= SectionData_p[14];
	temp3 		= temp<<24;
	temp = 0;
	temp 		= SectionData_p[15];
	temp2 		= temp<<16;
	temp = 0;
	temp 		= SectionData_p[16];
	temp1 		= temp<<8;
	temp = 0;
	temp0 		= SectionData_p[17];
	StopTime 	= temp3 | temp2 | temp1 | temp0;

	iLogoWaitTime=StopTime;

	if (iLogoWaitTime > 20)
	{
		iLogoWaitTime = 5;
	}

	/*point to Logo start data byte*/
	SectionData_p		= SectionData_p + 18;
	/*this section's logo data length*/
	SectionLogoLength = SectionLength - 19;
	
	APP_Printf("-------------nnnnBlockNumber = %d---------------\n",BlockNumber);
	APP_Printf("logoLength = %d,BlockNumber = %d,LogoFileTotalLength = %d\n",SectionLogoLength,BlockNumber,LogoFileTotalLength);
	LOGO_SaveData(SectionData_p,SectionLogoLength,BlockNumber,LogoFileTotalLength);
	
}

void APP_LOG_IPI_StartDemuxLog(void)
{
	BU08 coef[1] = {LogoTableID};
	BU08 mask[1] = {0xFF};
	BU08 excl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;

	LOGO_DefaultData();
	APP_LOG_IPI_StopDemuxLog();

	APP_Parser_IPI_StartDemux(1, LogoPID, coef, mask, excl, &logChannelID, &logFilterID);
	parFuncNode.channelId = logChannelID;
	parFuncNode.filterId = logFilterID;
	parFuncNode.ParserFunc = APP_LOG_IPI_StartParserLog;
	APP_IPI_AddPasFuncNode(parFuncNode);
}


void APP_LOG_IPI_StopDemuxLog(void)
{
	APP_Parser_IPI_StopDemux(&logChannelID, &logFilterID);
}

void APP_LOG_IPI_StartParserLog(BU08 *section, BU32 channelId, BU32 filterId)
{
	LOGO_ParseSection(section);
}

/* ----------------------------------------------------------------------------
初使化数据
---------------------------------------------------------------------------- */
static void LOGO_DefaultData(void)
{
	BU32 i;

	LogoCacheSectionNum = 0;
	for(i=0;i<LogoMaxSectionNum;i++)
	{
		LogoSectionNum[i] = 0;
	}
	for(i=0;i<LogoMaxDataK;i++)
	{
		LogoLengthTag_p[i] = 0;
	}
}


/* ----------------------------------------------------------------------------
分析开机界面的入口函数
---------------------------------------------------------------------------- */
void  LOGO_DisplayLogoBack(void)
{}
BU32 APP_LOG_IPI_GetWaitTime(void)
{	
	return iLogoWaitTime;
}

BU32 APP_LOG_IPI_GetLogState(BU08 ** data,BU32 *len,BU08 * version)
{
	if(LogoStopFilter == true)
	{
		*len = LogoFileTotalLength;
		* data = data_bitmap_Logo; 
		* version = LogoVersionNO;
		return APP_SUCCESS;
	}
	else
	{
		return APP_FAIL;
	}
}
BU32 APP_LOG_IPI_GetLogStart(void)
{
	return LogoStartFlag;
}

void APP_LOG_MemoryAllocate(void)
{
	data_bitmap_Logo = (BU08 *)FYF_API_malloc (256*1024);
}

void APP_LOG_MemoryFree(void)
{
	if(data_bitmap_Logo)
	{
		FYF_API_free (data_bitmap_Logo);
	}
}

