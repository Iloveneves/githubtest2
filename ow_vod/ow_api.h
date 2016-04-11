#ifndef __OW_API_H_
#define __OW_API_H_

#ifdef __cplusplus
extern "C" {
#endif


#define SEND_BUF_LEN 		(1024)
#define DNS_PORT     		(53)
#define RECV_BUF_LEN    	(1024 * 10)  
#define TSG_CHANNEL_NUM   	(30)
#define OPTION_TIME  		(11)					//定义每隔多少秒发一次option

typedef struct
{
    BU16 id;
    BU16 flags;
    BU16 questNum;
    BU16 answerNum;
    BU16 authorNum;
    BU16 additionNum;
	BU08 name[100];//所请求的域名
}DNSHDR;


typedef struct
{
	BU16 type;
	BU16 classs;
}QUERYHDR;

typedef struct response
{
	BU16 name;
	BU16 type;
	BU16 classes;
	BU32 ttl;
	BU16 length;
	BU32 addr;
	BU08 ipStr[16];
	BU16 yuPointer;
}RESPONSE;


typedef struct parseDNS
{
	DNSHDR		head;
	QUERYHDR	types;
	RESPONSE	res[10];//可能有几组ip数据
}PARSEDNS,*PRESPONSE;

/* Exported Types ----------------------------------------------------------- */

typedef struct
{
	char 	progAddr[SEND_BUF_LEN];
	char 	session[20];

	int 	pFre;
	int 	pPmtPid;
	BS16 	pVPid;
	BS16 	pAPid;
	BS16 	PcrPid;					/*zsy:080724 */
	int 	pVTrackID;
	int 	pATrackID;
	BU16 	regionID;

	char 	clockStart[20];
	char 	clockEnd[20];
	char 	clockCur[20];
	BU08 	bBOS;
	BU08	bEOS;

	int 	pStartTime;
	int 	pEndTime;
	int 	pCurTime;
	int 	reqScale;					//请求倍速
	int	 	trueScale;					//实际倍速
	int 	maxTime;					//缓存时间
	BU08 	bReLocate;
	int 	VodOrTvod;
	unsigned int cSeqCount;
	BU08    mAudioType;
	BU08    mVideoType;
} OW_PLAY_PARA;

typedef struct{
	BS08 		TsId[10];
	BS08 		SerId[10];
	BS08		ProgURL[50];
	BU08 		bPurchased;
}TSG_CHANNELS_PARA;

typedef struct{
	BS08 purProgCount;
	TSG_CHANNELS_PARA TsgChannel[TSG_CHANNEL_NUM];//定义30个频道
}OW_TVOD_CHANNELS;

typedef enum
{
	VOD = 0,
	TVOD
} VOD_TVOD;

/*typedef struct 
{
	BU08 ipaddr[16];
	BU08 netmask[16];
	BU08 gateway[16];
	BU08 firstdns[16];
	BU08 secdns[16];
}NetSetInfo;*/

/*-----------------------------RTSP------------------------------------*/

int RTSP_DefaultPlayPara(OW_PLAY_PARA *play);

int RTSP_DefaultTvodChanInfo(OW_TVOD_CHANNELS *Info);

int RTSP_GetFiledNumStr(char *DataPointer, char *FiledName, char *DataBuf);

int RTSP_GetFiledValue(char *DataPointer,char *name, int *value);

int RTSP_GetFiledString(char *dataPointer, char *filedName, char *dataBuf);

int RTSP_GetIPStrAndPortStr(char *progAdd,char *ipStr,char *portStr);

int RTSP_SearchLineEnd(char *DataPointer,int *Length);

int RTSP_ChecProgAddr(char *progAddr);



int RTSP_ParseIpString2Value(char *ipAdd,int *ipValue);

int RTSP_ParseRsponeOk(char *ResponeBuf);

int RTSP_ParseCDN_DESCRIBE_Rspone(char *ResponeBuf,int *CSeqCount,char *SerName,char *Location);

int RTSP_ParseUServer_DESCRIBE_Rspone(char *ResponeBuf, int *PmtFreq,int *PmtPid,int *RangStart,int *RangEnd,int *VTrackId,int *ATrackId);

int RTSP_ParseUServer_TDESCRIBE_Rspone(char *ResponeBuf,int *pmtFreq,int *pmtPid,int *rangStart,int *rangEnd,int *vTrackId,int *aTrackId);

int RTSP_SOCKET_SetNoneBlock(int socketfd);

int RTSP_SOCKET_OpenSocket(int *sockets,char *ServIp, char *ServPort);

int RTSP_SOCKET_Send(int socketfd, const char *buf, int len);

int RTSP_SOCKET_Receive(int socketfd,char *buf,int len,int timeout);

int RTSP_SendAndRecText(int socket, char *sends, char *recvs);


int RTSP_CreateDescribe(int socket,  OW_PLAY_PARA * const play);

int RTSP_CreateSETUP(int socket, OW_PLAY_PARA * const play);

int RTSP_CreatePLAY(int socket, OW_PLAY_PARA * const play);

int RTSP_CreatePAUSE(int socket, OW_PLAY_PARA * const play);

int RTSP_GetTSGChannelInfo(char *channelBuf);

int RTSP_CreateTSGChannel_XML(int socket, char *TsgIpStr,char *TsgPortStr);

int RTSP_CreateTDescribe(int socket, BU16 RegionID);

int RTSP_CreateOPTIONS(int socket, BU08 *StopTag, OW_PLAY_PARA * const play);

int RTSP_CreateTGET_PARAMETER(int socket, OW_PLAY_PARA * const play);

int RTSP_TVOD_ChannelInit(char *TsgAddr);

int RTSP_TVOD_GetAllProgrameInfo(char *TsgAddr);	/*获取频道列表所有信息*/

int RTSP_CreateFiled(char *DataBuf, char *FiledName, char *BeforeFiledName,char *AfterFiledName);

int RTSP_UpdateXMLFiled(char *DataBuf,char *FiledName,int Num,char *FileNameValue);

int RTSP_CreateChannelXMLDATA(char *XmlBuf,char *StbID);

int RTSP_GetXMLFiled(char *dataBuf,char *filedName,int num,char *fileNameStr,int nameSize);

BS32 RTSP_CreateTimer(void * pTimerCallback, BS32 s32TimerMode);
BS32 RTSP_DestroyTimer(BS32 s32TimerHdl);
BS32 RTSP_EnableTimer(BS32 s32TimerHdl, BU32 u32Timems, void * pParam);
BS32 RTSP_DisableTimer(BS32 s32TimerHdl);
void RTSP_TimerCallback(void *p);
int RTSP_ParseCloseReason(char *recText);

//int RTSP_NokeyPress(void);		//无按键150分钟后退出
int RTSP_GetSecondByClock(char *clock, int *allsecs, BU16 * year, BU08 * month, BU08 * day, BU08 bYearSecs);
int RTSP_CauAllSecs(BU16 year, BU08 month, BU08 day, BU08 hour, BU08 min, BU08 sec, int *allSecs);	//计算从2000年开始到现在走过的秒数

int RTSP_ParseNumTimeToStr(BU16 curYear, BU08 curMonth, BU08 curDay, BU08 hour, BU08 min, BU08 sec, char *yearStr, char *monthStr,
	char *dayStr, char *hourStr, char *minStr, char *secStr);

int RTSP_CauClockTime( char *clock,int *allSecs);
int RTSP_CheckChannel(void);
int RTSP_PauseTimeCount(BU08 * bPause, BU16 * timeNum);

/*-----------------------------DNS------------------------------------*/

void DNS_DefaultParseBuf(PARSEDNS *buf);

void DNS_CreateDNSPacket(char *addr,char *DNSSendBuff,char *bufLen);

int  DNS_ParseDNSRecBuf(char *buf, char *ipString, unsigned int *ipValue, int allLen, char *ipAddr);

int  DNS_GetIPByDNS(char *ipAddName, char *ipAddr);


/*-----------------------------DRAW------------------------------------*/
int OW_AddTime(BU16 *year,BU08 *month,BU08 *day,BU08 *hour,BU08 *min,BU08 *sec,BU08 perAdd);

int OW_SubTime(BU16 *year,BU08 *month,BU08 *day,BU08 *hour,BU08 *min,BU08 *sec,BU08 perSub);

int OW_ParseSecToTimeStr(int secs,char* timeStr);

int OW_DrawConnect(void);

int OW_DrawInfo(char *info,BU08 bClearSelf,BU08 bOnlyClear,BU08 bPressOut);

int OW_DrawExit5(void);
int OW_DrawBPZhun(void);
int OW_DrawBPJump(int ms, int type);	//画时间跳转
int OW_DrawInBP();
int OW_DrawInBPText();
int RTSP_CheckPlayErr(void);

/*-----------------------------DEMUX------------------------------------*/

int OW_DLVodPidInfo(int FreqKhz,int pmtPid,BU16 * const VidPid,BU16 * const AudPid,BU16 * const PcrPid,BU08 * const AudioType,BU08 * const VideoType);


/*-----------------------------MAIN------------------------------------*/

BS32 FYF_API_ETH_DNSGet(BU08 *dns);

BS32 FYF_API_ETH_DNSSet(BU08 *dns);

void OW_SetExitFlag(BU08 flag);

BU08 OW_GetExitFlag(void);

void OW_IncreOptionErrCount(void);

BU08 OW_GetOptionErrCount(void);

int  OW_CheckOption(void);							/*option中断5分钟后退出*/

void OW_DefaultOptionErrCount(void);

void OW_SetFirstPauseFlag(BU08 flag);

BU08 OW_GetFirstPauseFlag(void);

void OW_SetRecordFlag(BU08 flag);

BU08 OW_GetRecordFlag(void);

void OW_VOD_SetNetErrFlag(BU08 flag);

BU08 OW_VOD_GetNetErrFlag(void);

void OW_VOD_SetDhcpFlag(BU08 flag);

BU08 OW_VOD_GetDhcpFlag(void);

int  OW_VOD_MainMenu(char *progAdd);
int  OW_TVOD_MainMenu(char *progAdd, BU08 enterType, char *startClock);


#ifdef __cplusplus
}
#endif

#endif
