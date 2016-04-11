/*----------------------------------------------------------------------------
   File Name:ow_rtsp.c

   Description: 

   Version:REL_V1.0.0

   Copyright (c) 2006-2100 Triumph Corporation.  All rights reserved.
-----------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */

#include "ow_debug.h"
#include "./../tpmiddle/include/app_basic_type.h"
#include "./../tpversion/ui_demo/osd/include/S_Chinese_strs_id.h"
#include "./../tpversion/ui_demo/osd/include/resource.h"

#define PAUSETIMEOUT  (1755)				//暂时定义三十分钟

extern char stbid[27];					/*stbid*/ /*hqx:do it later*/
int CSeqCountAll = 1;					/*记录对话过程次数*/
char *User_Agent = "NewLand_5105";		/*客户机名称*/
OW_TVOD_CHANNELS   TvodChanInfo;
static void * s_TrdTimerEntry = NULL;
BU08 serSetParaType = 0xff;				//服务器发给机顶盒异常消息类型 0: 节目关闭; 1:节目开头;2:节目结尾
BU08 bDrawSpeed = FYF_TRUE;
BU08 bGetTsgSuc = FYF_FALSE;
BU08  bGetIP = FYF_FALSE;
int  bCableDownCount = 0;

extern char getipaddr[20];
//static char get_ipaddr[20] = {0};
char *const fuzhouTSGAddr ="huabeiyoutian-tsg.wasu.cn";//"fuzhou-tsg.wasu.cn";
char *const fuzhouTSGPort = "5777";//"2777";
int curChannelNum = -1;

BU08 bInProcess = FYF_FALSE;
BU08 bTVOD = FYF_FALSE;					//表示当前节目是否为时移
BU08 bSuspendCheckKickoff = FYF_FALSE;
BU08 bTurnerError =  FYF_FALSE;
BU32 g_RTSPPTS1 = 0;
BU32 g_RTSPPTS2 = 0;
extern 	char sends[SEND_BUF_LEN];
extern	char recvs[RECV_BUF_LEN];
extern  OW_PLAY_PARA VodPlayPara;
extern  int optionSocket;
extern BU08 bNetError ;
extern int  NetErrorCount;
extern BU08 bPauseDisPly;
extern BU32 GUI_IpanelState;
extern BU08 bInTvodTimer;
extern HI_U8		mp4Head[];
extern HI_U8		mp4aHead[];
int video_rate = 90000, audio_rate = 44100;
BU32  bAudioMode = FYF_ACODEC_TYPE_AAC;
extern BU32 PTSStart;
extern BU32 g_PreviewPts;
extern BU08 bHadRes;
extern BU08 bTVODPlay;
void OW_SetSuspendFlag(BU08 flag)
{
	bSuspendCheckKickoff = flag;
}

BU08 OW_GetSuspendFlag(void)
{
	return bSuspendCheckKickoff;
}

//static pthread_mutex_t socket_mutex;
//static BU08 bHaveInitMutex = FYF_FALSE;

/*-----------------------------------------------------------------------------
	函数名：RTSP_DefaultPlayPara
		
	描述：
			初始化各种参数
	参数：
			in: 
				OW_PLAY_PARA *play:
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_DefaultPlayPara(OW_PLAY_PARA *play)
{
	play->pFre = -1;
	play->pPmtPid = 0x1fff;
	play->pVPid = 0x1fff;
	play->pAPid = 0x1fff;
	play->PcrPid = 0x1fff;
	play->pVTrackID = 0xff;
	play->pATrackID = 0xff;
	play->pStartTime = 0;
	play->pEndTime = 0;
	play->pCurTime = 0;
	play->reqScale = 1;
	play->trueScale = 1;
	play->maxTime = 0;
	play->cSeqCount = 1;
	play->bReLocate = FYF_FALSE;
	play->bBOS = FYF_FALSE;
	play->bEOS = FYF_FALSE;
	play->mAudioType = FYF_ACODEC_TYPE_MP3;
	play->mVideoType = FYF_VCODEC_TYPE_MPEG2;
	
	FYF_API_Memset(play->progAddr, 0, SEND_BUF_LEN);
	FYF_API_Memset(play->session, 0, 20);
	FYF_API_Memset(play->clockStart, 0, 20);
	FYF_API_Memset(play->clockCur, 0, 20);
	FYF_API_Memset(play->clockEnd, 0, 20);
	return FYF_OK;
}

/*-----------------------------------------------------------------------------
	函数名：RTSP_DefaultTvodChanInfo
		
	描述：
			初始化时移各种参数
	参数：
			in: 
				OW_TVOD_CHANNELS *Info:
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_DefaultTvodChanInfo(OW_TVOD_CHANNELS *Info)
{
	int i = 0;
	Info->purProgCount = 0;
	for(i = 0; i < TSG_CHANNEL_NUM; i++)
	{
		Info->TsgChannel[i].bPurchased = FYF_FALSE;
		FYF_API_Memset(Info->TsgChannel[i].TsId, 0, 10);
		FYF_API_Memset(Info->TsgChannel[i].SerId, 0, 10);
		FYF_API_Memset(Info->TsgChannel[i].ProgURL, 0, 50);
	}
	return FYF_OK;
}

void RTSP_IncreaseCSeqCount(void)
{
	CSeqCountAll++;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_GetFiledNumStr
		
	描述：
			专门用于寻找指定区域的字符串数据是1234567890
	参数：
			in: 
				DataPointer: 数据起始指针
				FiledName: 所需元素名字
			out:
				DataBuf:存储所需元素的数据
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_GetFiledNumStr(char *DataPointer, char *FiledName, char *DataBuf)
{
	char *file_name_pointer = FYF_NULL;
	int i = 0;
	int len;

	if ((FYF_NULL == DataPointer) || (FYF_NULL == FiledName))
	{
		OW_ERROR("RTSP_GetFiledNumStr err! (NULL == dataPointer)||(NULL == filedName)!!\n");
		return FYF_ERR;
	}
	
	file_name_pointer = strstr(DataPointer, FiledName);

	if(FYF_NULL == file_name_pointer)
	{
		OW_ERROR("RTSP_GetFiledNumStr err!can't find %s!!\n", FiledName);
		return FYF_ERR;
	}
	else
	{
		file_name_pointer += strlen(FiledName);
	}
	
	len = (int)(strlen(DataPointer) - (file_name_pointer-DataPointer));
	for(i = 0; i < len; i++)
	{
		if(file_name_pointer[i]<'0'||(file_name_pointer[i]>'9'))
		{
			//file_name_pointer+=i;
		}
		else
		{
			break;
		}
	}
	
	if(i < len)
	{
		file_name_pointer += i;
	}
	else
	{
		OW_ERROR("RTSP_GetFiledNumStr can't find num!!\n");
		return FYF_ERR;
	}
	
	for(i = 0; i < len; i++)
	{
		if(FYF_NULL == DataBuf)
		{
			OW_ERROR("RTSP_GetFiledNumStr err! NULL== dataBuf!!\n");
			return FYF_ERR;
		}
		else
		{
			if(file_name_pointer[i] >= '0' && (file_name_pointer[i] <= '9'))
			{
				DataBuf[i] = file_name_pointer[i];
			}
			else
			{
				OW_DEBUG("RTSP_GetFiledNumStr end, DataBuf = %s\n", DataBuf);
				return FYF_OK;
			}
		}
	}
	return FYF_ERR;	
}

		
/*-----------------------------------------------------------------------------
	函数名：RTSP_GetFiledValue
		
	描述：
			寻找指定区域的整形数值
	参数：
			in: 
				DataPointer: 数据起始指针
				name: 所需元素名字
			out:
				value:已经转换过的整形数值
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_GetFiledValue(char *DataPointer,char *name, int *value)
{
	char strTemp[50] = {0};
	
	if (RTSP_GetFiledNumStr(DataPointer, name, strTemp) == FYF_ERR)
	{
		OW_ERROR("RTSP_GetFiledValue can't find %s!!\n",name);
		return FYF_ERR;
	}
	*value = atoi(strTemp);
	printf("%s = %d\n",name,*value);
	return FYF_OK;
}

/*-----------------------------------------------------------------------------
	函数名：RTSP_GetFiledString
		
	描述：
			寻找指定区域的字符串数据
	参数：
			in: 
				DataPointer: 数据起始指针
				filedName: 所需元素名字
			out:
				dataBuf:存储所需元素的数据
				datalen: 存储数据的长度
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_GetFiledString(char *dataPointer, char *filedName, char *dataBuf)
{
	char *file_name_pointer = NULL;
	int len = 0;
	int i;

	if((NULL == dataPointer)||(NULL == filedName))
	{
		OW_ERROR("RTSP_GetFiledString err, (NULL == dataPointer)||(NULL == filedName).\n");
		return FYF_ERR;
	}
	
	file_name_pointer = strstr(dataPointer, filedName);

	if(NULL == file_name_pointer)
	{
		OW_ERROR("RTSP_GetFiledString err,  can't find %s \n",filedName);
		return FYF_ERR;
	}

	file_name_pointer += strlen(filedName);
	
	if(NULL == dataBuf)
	{
		OW_ERROR("RTSP_GetFiledString err, NULL==dataBuf \n");
		return FYF_ERR;
	}

	len = (int)(strlen(dataPointer) - (file_name_pointer - dataPointer));
	for(i = 0; i < len; i++)
	{
		if(file_name_pointer[i] == ' ')
		{
			file_name_pointer++;
			
		}
		else
		{
			break;
		}
	}

	for(i = 0; i < len; i++)
	{
		if(NULL == dataBuf)
		{
			OW_ERROR("RTSP_GetFiledString err, NULL==dataBuf!!\n");
			return FYF_ERR;
		}
		else
		{
			if((file_name_pointer[i] == ' ') || (file_name_pointer[i] == 0xd) || (file_name_pointer[i] == 0xa))
			{
				return FYF_OK;
			}
			else
			{
				dataBuf[i] = file_name_pointer[i];
			}
		}
	}

	printf("RTSP_GetFiledString out of range\n");
	return FYF_ERR;
}

/*-----------------------------------------------------------------------------
	函数名：RTSP_GetIPStrAndPortStr
		
	描述：
			从给定的url中找到包含ip和端口的字符串
	参数：
			in: 
				progAdd:给定的URL
			out:
				ipStr:包含IP的字符串
				portStr:包含端口的字符串
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_GetIPStrAndPortStr(char *progAdd,char *ipStr,char *portStr)
{		
	char *ipTemp = NULL;
	char *ipTemp2 = NULL;
	int len = 0;

	if(NULL != (ipTemp = strstr(progAdd, "rtsp://")))
	{
		ipTemp += 7;
		if(NULL != (ipTemp2 = strstr(ipTemp, ":")))
		{
			len = (int)(ipTemp2 - ipTemp);
			FYF_API_Memcpy(ipStr, ipTemp, len);
			ipTemp2 += 1;
			if(NULL != (ipTemp = strstr(ipTemp2, "/")))
			{
				len = (int)(ipTemp - ipTemp2);
				FYF_API_Memcpy(portStr,ipTemp2,len);
				OW_DEBUG("progAdd = %s,ipStr = %s,portStr=%s\n",progAdd,ipStr,portStr);
				return FYF_OK;
			}
		}
	}

	OW_ERROR("TP_GetIPStrAndPortStr err! progAdd = %s \n",progAdd);
	return FYF_ERR;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_SearchLineEnd
		
	描述：
			寻找换行符
	参数：
			in: 
				DataPointer: 数据起始指针
			out:
				Length:换行符到起始指针的偏移量
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_SearchLineEnd(char *DataPointer,int *Length)
{
	int offset1 = 0;
	int offset2 = 0;

	offset1 = (int)(strstr(DataPointer, "\n") - DataPointer);
	offset2 = (int)(strstr(DataPointer, "\r") - DataPointer);


	if((offset1 < 0) && (offset2 < 0))
	{
		OW_ERROR("RTSP_SearchLineEnd err!\n");
		return FYF_ERR;
	}
	else if((offset1 > 0) && (offset2 < 0))
	{
		*Length = offset1;
	}
	else if((offset1 < 0) && (offset2 > 0))
	{
		*Length = offset2;
	}
	else
	{
		(offset1 < offset2)?(*Length = offset1):(*Length = offset2);
	}

	OW_DEBUG("length = %d\n",*Length);
	return FYF_OK;
}


int RTSP_GetClock(char *dataPointer, char *clock, int *offset)
{
	int i;
	
	for(i = 0; i < 20; i++)
	{
		if((dataPointer[i] > '0') && (dataPointer[i] < '9'))
		{
			break;
		}
	}
	if(i == 20)
	{
		OW_ERROR("RTSP_GetClock can't find num\n");
		return FYF_ERR;
	}
	dataPointer += i;
	
	for(i = 0; i < 20; i++)
	{
		if(dataPointer[i] == 'Z')
		{
			clock[i] = dataPointer[i];
			break;
		}
		else
		{
			clock[i] = dataPointer[i];
		}
	}
	clock[i] = 0;
	*offset = i;
	return FYF_OK;

}



int RTSP_GetProgClock(char *dataPointer, char *startClock, char *endClock, char *curClock)
{
	char *pointerTemp = NULL;
	int offset = 0;

	if(NULL == (pointerTemp = strstr(dataPointer, "x-Timeshift_Range")))
	{
		OW_ERROR("RTSP_GetProgClock  find x-Timeshift_Range ERROR\n");
		return FYF_ERR;
	}

	if(NULL == (pointerTemp = strstr(pointerTemp,"clock=")))
	{
		OW_ERROR("RTSP_GetProgClock  find clock= ERROR\n");
		return FYF_ERR;
	}

	pointerTemp += 6;

	if(RTSP_GetClock(pointerTemp, startClock, &offset) == FYF_ERR)
	{
		OW_ERROR("RTSP_GetProgClock startClock ERROR\n");
		return FYF_ERR;
	}

	pointerTemp += offset;

	if(RTSP_GetClock(pointerTemp, endClock, &offset) == FYF_ERR)
	{
		OW_ERROR("RTSP_GetProgClock endClock ERROR\n");
		return FYF_ERR;
	}
	pointerTemp += offset;
	if(NULL == (pointerTemp = strstr(pointerTemp, "x-Timeshift_Current")))
	{
		OW_ERROR("RTSP_GetProgClock  find x-Timeshift_Current ERROR\n");
		return FYF_ERR;
	}

	if(NULL == (pointerTemp = strstr(pointerTemp, "clock=")))
	{
		OW_ERROR("RTSP_GetProgClock  find clock= 2222 Error\n");
		return FYF_ERR;
	}
	pointerTemp += 6;
	if(RTSP_GetClock(pointerTemp, curClock, &offset) == FYF_ERR)
	{
		OW_ERROR("RTSP_GetProgClock  curClock Error\n");
		return FYF_ERR;
	}
	OW_DEBUG("RTSP_GetProgClock  startClock curClock = %s\n",curClock);
	
	return FYF_OK;
	
}



int RTSP_CauAllSecs(BU16 year, BU08 month, BU08 day, BU08 hour, BU08 min, BU08 sec, int *allSecs)	//计算从2000年开始到现在走过的秒数
{
	BU08 yearTemp = 0;
	BU08 runNianNum = 0;
	BU16 i = 2001;
	BU08 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	yearTemp = year % 100;

	if ((year == 0) || (month == 0) || (day == 0))
	{
		*allSecs = 0;
		return FYF_ERR;
	}

	for (i = 2001; i < year; i++)
	{
		if (i % 4 == 0)
		{
			if (i % 100 != 0)
			{
				runNianNum++;
			}
			else if (i % 400 == 0)
			{
				runNianNum++;
			}
		}

	}

	*allSecs = 3600 * 24 * 365 * (year - 2000) + 3600 * 24 * runNianNum;

	for (i = 0; i < month - 1; i++)
	{
		if (i != 1)
		{
			*allSecs += (3600 * 24 * monthDays[i]);
		}
		else
		{
			if (year % 4 == 0)
			{
				if (year % 100 != 0)
				{
					*allSecs += (3600 * 24 * 29);
				}
				else if (year % 400 == 0)
				{
					*allSecs += (3600 * 24 * 29);
				}
				else
				{
					*allSecs += (3600 * 24 * 28);
				}
			}
			else
			{
				*allSecs += (3600 * 24 * 28);
			}
		}
	}

	*allSecs += (3600 * 24 * day + 3600 * hour + 60 * min + sec);
	//OW_DEBUG("=========RTSP_CauAllSecs allSecs = %d \n",*allSecs);
	return FYF_OK;

}

int RTSP_GetSecondByClock(char *clock, int *allsecs, BU16 * year, BU08 * month, BU08 * day, BU08 bYearSecs)
{
	char *dataPointer = NULL;
	char hour[3] = { 0 };
	char min[3] = { 0 };
	char sec[3] = { 0 };
	char days[3] = { 0 };
	char months[3] = { 0 };
	char years[5] = { 0 };

	if (NULL == (dataPointer = strstr(clock, "T")))
	{
		OW_ERROR("RTSP_GetSecondByClock  dataPointer = strstr(recvs,T) err!!!\n");
		return FYF_ERR;
	}
	dataPointer++;

	hour[0] = dataPointer[0];
	hour[1] = dataPointer[1];
	min[0] = dataPointer[2];
	min[1] = dataPointer[3];
	sec[0] = dataPointer[4];
	sec[1] = dataPointer[5];
	dataPointer -= 3;
	days[0] = dataPointer[0];
	days[1] = dataPointer[1];
	*day = atoi(days);					//20080616T164644.12

	years[0] = clock[0];
	years[1] = clock[1];
	years[2] = clock[2];
	years[3] = clock[3];
	*year = atoi(years);				//20080616T164644.12

	months[0] = clock[4];
	months[1] = clock[5];
	*month = atoi(months);				//20080616T164644.12

	if (bYearSecs)						//算到年份秒数
	{
		RTSP_CauAllSecs(*year, *month, *day, (BU08) atoi(hour), (BU08) atoi(min), (BU08) atoi(sec), allsecs);
	}
	else
	{
		*allsecs = atoi(hour) * 3600 + atoi(min) * 60 + atoi(sec);
	}
	OW_DEBUG("RTSP_GetSecondByClock cur clock = %s,secs = %ld \n",clock,*allsecs);
	return FYF_OK;
}


/**将传进来的地址?后面的验证部分去掉**/

int RTSP_ChecProgAddr(char *progAddr)
{
	char *dataPointer = NULL;
	int  progLen = 0;
	int  headLen =0;
	
	if(progAddr == NULL)
	{
		return FYF_ERR;
	}

	progLen = strlen(progAddr);

	if((dataPointer = strstr(progAddr,"?")) != NULL)
	{
	    headLen = (int)(dataPointer - progAddr);
	    FYF_API_Memset(&progAddr[headLen], 0, (progLen - headLen));
		
		printf("progaddr = %s\n", progAddr);
	    return FYF_OK;
	}

	return FYF_ERR;
	
}
extern BU08 bFreezeVideo;
int RTSP_PassBadFrame(void)
{
	int ret = -1;
	FYF_VDEC_STOP_MODE_e fyfStopMode = 0;
	BU32 audiotype = VodPlayPara.mAudioType;
	
	printf("RTSP_PassBadFrame .\n");
	if (OW_GetMPEGType() == FYF_TRUE)
	{
		if (VodPlayPara.pVPid != 0x1fff && VodPlayPara.pAPid != 0x1fff)
		{
			printf("vid = %d, aid = %d\n", VodPlayPara.pVPid, VodPlayPara.pAPid);
			
			FYF_API_AV_Freeze();
			FYF_API_adec_ioctl(0, FYF_ADEC_STOP, (void *)&fyfStopMode);
		
			FYF_API_thread_sleep(1500);
		
			FYF_API_AV_Resume();
			FYF_API_adec_ioctl(0, FYF_ADEC_START, (void *)&audiotype);
		
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}

	}
	else
	{
		//FYF_API_CTLVod(3);
		//FYF_API_CTLVod(4);
		//FYF_API_CTLVod(2);
	}



}


int RTSP_CheckTunerOk(void)
{
	static BU16 unlockLong = 0;			//150分钟信号未恢复,则退出
	int readKey = 0;
	BU08 optionTimeCount = 0;
	BU08 stopTag = FYF_FALSE;
	static int turnercount = 0;


	if (GUI_ENC_API_GetCurState() != GUI_IpanelState )
	{
		if (GUI_ENC_API_GetCurState() == ID_DLG_FULLTOTVOD && FULLTOTVOD_GetCurState() == BPJUMP_STATE/*1*/)
		{
			printf("In BPJUMP_STATE , Don't check Turner.\n");	/*防止在输入时间值的时候弹出这个框而无法输入时间值*/
			return ;
		}
	}
	
	turnercount++;

	if (turnercount == 3)
	{
		turnercount = 0;
		
		if( FYF_API_tuner_get_status(0) != 0)
		{
			
			bTurnerError = FYF_TRUE;
			printf("请检查电缆信号\n");
			
			if (/*!bInTvodTimer && */(GUI_IpanelState != GUI_ENC_API_GetCurState()))
			{
				if (VodPlayPara.VodOrTvod == 0)
				{
					GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "请检查电缆信号", 0);
					GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
					GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
				}
				else if (VodPlayPara.VodOrTvod == 1)
				{
					GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "请检查电缆信号", 0);
					GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
				}
			}
		
					
			unlockLong++;
			printf("unlockLong = %d\n", unlockLong);
			if (unlockLong == 3000)
			{
				OW_SetExitFlag(FYF_TRUE);
				unlockLong = 0;
				bTurnerError = FYF_FALSE;
				
				OW_ERROR("OW_TVOD_MainMenu out from RTSP_CheckTunerOk !!\n ");
				return FYF_ERR;
			}
					
			/*if (readKey == EXIT_KEY)
			{
				OW_SetExitFlag(FYF_TRUE);
				
				OW_ERROR("RTSP_CheckTunerOk out from 22222!!\n ");
		
				unlockLong = 0;
				return FYF_ERR;
			}*/
			
		}

			
	}

	if( FYF_API_tuner_get_status(0) == 0 && bTurnerError)
	{
		printf("电缆信号恢复\n");
		unlockLong = 0;
		turnercount = 0;
		bTurnerError = FYF_FALSE;
		
		if (/*!bInTvodTimer && */(GUI_IpanelState != GUI_ENC_API_GetCurState()))
		{
			if (VodPlayPara.VodOrTvod == 0)
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "电缆信号恢复", 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
			}
			else if (VodPlayPara.VodOrTvod == 1)
			{
				GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "电缆信号恢复", 0);
				GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
			}
		}

		FYF_API_thread_sleep(1000);
		
		if (/*!bInTvodTimer && */(GUI_IpanelState != GUI_ENC_API_GetCurState()))
		{
			if (VodPlayPara.VodOrTvod == 0)
			{
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_DISABLE);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_DISABLE);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
				
				FYF_API_OSD_FillRect(200, 200, 300, 60, 0);
				FYF_API_OSD_Refresh(200, 200, 300, 60);
			}
			else if (VodPlayPara.VodOrTvod == 1)
			{
				GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_DISABLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_DISABLE);
				GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
				
				FYF_API_OSD_FillRect(222, 236, 300, 60, 0);
				FYF_API_OSD_Refresh(222, 236, 300, 60);
			}
		}

		
		return FYF_OK;
	}

}


int RTSP_CheckNetOk(void)
{
	int ret  = -1;
	BU08 bCreateTimer = FYF_FALSE;
	BU08 bDraw = FYF_FALSE;
	int Timerid = -1;
	//OW_DEBUG("RTSP_CheckNetOk\n");

	ret = FYF_API_GetLinkStatus();
	if (ret != 1)
	{
		OW_ERROR("LINK DOWN !\n");
		
		/*if(!bCreateTimer)
		{
			Timerid = RTSP_CreateTimer(RTSP_TimerCallback,  0);
			if(-1 != Timerid)
			{
				RTSP_EnableTimer(Timerid, (5 * 60 * 1000), NULL);
				bCreateTimer = FYF_TRUE;
			}
		}*/
#if 0			/*hqx:do it later*/
		//draw bmpzhun
#endif
		bDraw = FYF_TRUE;
		printf("请检查网络连接, NetErrorCount = %d\n", NetErrorCount);

		if ((bNetError == FYF_FALSE) && (NetErrorCount == 0))
		{
			if (VodPlayPara.VodOrTvod == 0) /*VOD*/
			{
				VOD_SetCurState(VOD_PLAY_STATE/*1*/);
				GUI_ENC_API_SetFocus(ID_DLG_VOD);
				if (!bInTvodTimer)
					VOD_ClearScreen();
			}
			else if (VodPlayPara.VodOrTvod == 1)
			{
				FULLTOTVOD_SetCurState(TVOD_PLAY_STATE/*3*/);
				GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
				if (!bInTvodTimer)
					FULLTOTVOD_ClearScreen();
			}
			bNetError = FYF_TRUE;
		}

		if (!bInTvodTimer && (GUI_IpanelState != GUI_ENC_API_GetCurState()))
		{
			if (VodPlayPara.VodOrTvod == 0)
			{
				printf("vod, 请检查网络连接\n");
				GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "请检查网络连接", 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
			}
			else if (VodPlayPara.VodOrTvod == 1)
			{
				printf("tvod, 请检查网络连接\n");
				GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "请检查网络连接", 0);
				GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
			}
		}

		
		return FYF_ERR;
	}

	if (OW_VOD_GetNetErrFlag() == FYF_TRUE)
	{
		ret = FYF_API_GetLinkStatus();
		if (ret != 1)
		{
			printf("请检查网络连接\n");
		}
		else
		{
			//OW_VOD_SetNetErrFlag(FYF_FALSE);
			if (bCableDownCount < CABLEDOWN)
				bCableDownCount++;
			printf("bCableDownCount = %d\n", bCableDownCount/*, ipanel_porting_time_ms()*/);
			if (bCableDownCount == CABLEDOWN)
			{
				APP_IPI_StopAudioVideo();
				//bCableDownCount = 0;
				
				if (!bInTvodTimer && (GUI_IpanelState != GUI_ENC_API_GetCurState()))
				{

					if (bHadRes)
					{
						if (!bNetError)
						{
							if (serSetParaType != 0 && RTSP_CreateTEARDOWN(optionSocket, &VodPlayPara) == FYF_ERR)
							{
								OW_ERROR("OW_VOD_MainMenu RTSP_CreateTEARDOWN  err!!\n ");
							}
						}
						
						APP_Com_API_StopProg(0);
						//OW_DLRTPInfo_StopChannel();
					}
					bHadRes = FYF_FALSE;				//释放服务器前端分配资源
						
					if (VodPlayPara.VodOrTvod == 0)
					{
						
						printf("vod, 请检查网络连接\n");
						GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "网络不通，请退出点播", 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
					}
					else if (VodPlayPara.VodOrTvod == 1)
					{
						/*GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "请检查网络连接", 0);
						GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
						GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);*/
						
						printf("tvod, 请检查网络连接\n");
						GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "网络不通，请退出回看", 0);
						GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
						GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);

						
					}
				}

				//FYF_API_thread_sleep(3000);
				
				//VOD_ClearScreen();
				//OW_VOD_SetNetErrFlag(FYF_FALSE);
				//OW_SetExitFlag(FYF_TRUE);
				return FYF_ERR;
			}
		}
	}

	if (OW_VOD_GetNetErrFlag() == FYF_FALSE)
	{
		bCableDownCount = 0;
	}

	return FYF_OK;
	
}



//检查服务器是否将机顶盒资源释放

int RTSP_CheckSerKickOff(int s)
{
	char *dataPointer = NULL;
	char eInfo[15] = { 0 };
	int nBytesTemp = 0;
	char recBufTemp[1024] = { 0 };
	int x = 110;
	int y = 200;
	BU16 width = 0;
	BU16 height = 0;
	int ret = -1;

	/*ret = RTSP_SOCKET_SetNoneBlock(s);
	if (ret < 0)
	{
		OW_ERROR("RTSP_SOCKET_SetNoneBlock error\n");
		return FYF_ERR;
	}*/

	if (OW_GetSuspendFlag() == FYF_TRUE)
	{
		return;
	}
	
	nBytesTemp = recv(s, recvs, 1024, 0);

	if (nBytesTemp <= 0)
	{
		//OW_ERROR("RTSP_CheckSerKickOff recv nBytesTemp = %d\n",nBytesTemp);
		return FYF_ERR;
	}

	printf("recBufTemp = %s\n", recvs);

	
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	
	if (RTSP_GetXMLFiled(recvs, "x-Info:", 1, eInfo, sizeof(eInfo)) == FYF_ERR)
	{
		OW_ERROR("RTSP_CheckSerKickOff RTSP_GetXMLFiled get x-Info err!!!\n");
		return FYF_ERR;
	}

	if (strcmp(eInfo, "CLOSE") == 0)
	{
		serSetParaType = 0;
		RTSP_ParseCloseReason(recvs);

		if (VodPlayPara.VodOrTvod == 0)
		{
			VOD_ClearScreen();
			APP_Com_API_StopProg(1);
			//OW_DLRTPInfo_StopChannel();
			
			GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
			GUI_ENC_API_Update(ID_STA_THANKS, 1);
			
			FYF_API_thread_sleep(1000);
			VOD_ClearScreen();
			
			//OW_SetExitFlag(FYF_TRUE);	
			bPauseDisPly = FYF_FALSE;
			printf("RTSP_CheckSerKickOff, Exit vod==============\n");
			
			
		}
		else if (VodPlayPara.VodOrTvod == 1)
		{

		}
		OW_SetExitFlag(FYF_TRUE);
		//GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
		OW_DEBUG("RTSP_RecvSET_PARAMETER out from RTSP_GetXMLFiled get x-Info CLOSE!!!\n");
	}
	else if (strcmp(eInfo, "BOS") == 0)
	{
		serSetParaType = 1;

		/*if (GUI_ENC_API_GetCurState() == ID_DLG_TVOD_FAST)
		{
			
			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			
			
		}*/
		
		FULLTOTVOD_ClearScreen();
		printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );
		printf("已超出时移起点\n");
		printf("从时移起点开始播放\n");
		
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutTimeshiftStart, 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);
		GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
		
		GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
		GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
		GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);

		FYF_API_thread_sleep(2000);
		
		FULLTOTVOD_SetCurState(TVOD_PLAY_STATE/*3*/);
		GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
		FULLTOTVOD_ClearScreen();
		
		VodPlayPara.reqScale = 1;
		OW_DrawSpeed(VodPlayPara.reqScale);
		VodPlayPara.bBOS = FYF_TRUE;
		if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
		{
			OW_ERROR("RTSP_CreateOPTIONS RTSP_CreatePLAY  err!!\n ");
		}
		else
		{
			RTSP_PassBadFrame();
		}
		OW_DEBUG("RTSP_RecvSET_PARAMETER RTSP_GetXMLFiled get x-Info BOS!!!\n");
				
		FULLTOTVOD_ClearScreen();

	}
	else if (strcmp(eInfo, "EOS") == 0)
	{
		serSetParaType = 2;
		
		FULLTOTVOD_ClearScreen();
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutLiveTime, 0);
		printf("已超出直播时间点\n");
		if (Ipanel_GetEntrance_Flag())
		{	
			printf("返回到浏览器\n");
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "返回浏览器", 0);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_ReturnToLive, 0);
			printf(" 进入直播 \n");
		}
		
		GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
		
		GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
		GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
		GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
		
		FYF_API_thread_sleep(3000);

		FULLTOTVOD_ClearScreen();
		OW_SetExitFlag(FYF_TRUE);
	}

	return FYF_OK;
}



int RTSP_PauseTimeCount(BU08 * bPause, BU16 * timeNum)
{
	if (*bPause == FYF_TRUE)
	{
		(*timeNum)++;
		printf("pause timenum = %d\n", *timeNum);
		if ((*timeNum) == PAUSETIMEOUT)
		{
			if ((VodPlayPara.VodOrTvod == 0) && (GUI_IpanelState != 0))
			{
				VOD_ClearScreen();
				GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
				GUI_ENC_API_Update(ID_STA_THANKS, 1);
			}
			
			OW_SetExitFlag(FYF_TRUE);
			//TP_OW_ClearPP();
			printf("暂停超时!\n");
			OW_ERROR("OW_TVOD_MainMenu out from TP_OW_DrawJump !!\n ");
			*timeNum = 0;
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
	}
	else
	{
		*timeNum = 0;
		return FYF_ERR;
	}

}


#define ERR_CHECK

int RTSP_CheckChannel(void)
{
	BU16 serid = 0;
	int tsgSerid = 0;
	int i;
	short sProgIndex = -1;
	int ret = -1;
	ChnDataBase_ChnInfo_S ChannelInfo;
#if 1
	/*if (!bGetIP)
	{
		if(getip() != 0)
		{
			bGetIP = true;
		}
		else
		{
			return TP_FAIL;
		}
	}*//*hqx:do it later*/


	printf("bGetTsgSuc = %d\n", bGetTsgSuc);
	if (!bGetTsgSuc)
	{
		printf("error========\n");
		return FYF_ERR;
	}
	else
	{
		APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &ChannelInfo);
		serid = ((ChannelInfo.m_ProgId) & 0xffff);
		
		for (i = 0; i < TvodChanInfo.purProgCount; i++)
		{
			tsgSerid = atoi(TvodChanInfo.TsgChannel[i].SerId);
			//printf("tsgSerid = %d, TvodChanInfo.purProgCount = %d\n", tsgSerid, TvodChanInfo.purProgCount);
			
			if (serid == (BU16) tsgSerid)
			{
				printf("tsgSerid = %d\n", tsgSerid);
				printf("当前节目为时移!!!!\n");
				//TP_OW_DrawReLogo();
				curChannelNum = i;
				bTVOD = FYF_TRUE;
				
				ret = FYF_API_GetLinkStatus();
				if (ret != 1)
				{
					OW_ERROR("RTSP_RetryTsgService, check net\n");
					
					return 2;
				}
				
				memset(getipaddr, 0, sizeof(getipaddr));
				FYF_API_ETH_IPAddressGet(getipaddr);
				
				if (strlen(getipaddr) == 0)
				{
					strcpy(getipaddr, "000.000.000.000");
					OW_ERROR("FYF_API_ETH_IPAddressGet getip = %s\n",getipaddr);
					
					return FYF_ERR;
				}
				printf("getipaddr = %s\n", getipaddr);
				

				return FYF_OK;
			}
		}
		//TP_OW_ClearReLogo();
		bTVOD = FYF_FALSE;
		printf("当前节目不是时移!!!!\n");
		return FYF_ERR;
	}
	return FYF_ERR;
#endif
}


int RTSP_CauClockTime( char *clock,int *allSecs)
{
	BU16 year = 0;
	BU08 month = 0;
	BU08 day = 0;

	RTSP_GetSecondByClock(clock, allSecs, &year, &month, &day, FYF_TRUE);

	return FYF_OK;
}


#define PARSE


int RTSP_ParseNumTimeToStr(BU16 curYear, BU08 curMonth, BU08 curDay, BU08 hour, BU08 min, BU08 sec, char *yearStr, char *monthStr,
	char *dayStr, char *hourStr, char *minStr, char *secStr)
{

	if (curYear != 0)
	{
		sprintf(yearStr, "%d", curYear);
	}
	else
	{
		sprintf(yearStr, "%s", "0000");
	}

	if (curMonth != 0)
	{
		if (curMonth < 10)
		{
			sprintf(monthStr, "0%d", curMonth);
		}
		else
		{
			sprintf(monthStr, "%d", curMonth);
		}

	}

	if (curDay != 0)
	{
		if (curDay < 10)
		{
			sprintf(dayStr, "0%d", curDay);
		}
		else
		{
			sprintf(dayStr, "%d", curDay);
		}
	}

	if (hour < 10)
	{
		sprintf(hourStr, "0%d", hour);
	}
	else
	{
		sprintf(hourStr, "%d", hour);
	}

	if (min < 10)
	{
		sprintf(minStr, "0%d", min);
	}
	else
	{
		sprintf(minStr, "%d", min);
	}

	if (sec < 10)
	{
		sprintf(secStr, "0%d", sec);
	}
	else
	{
		sprintf(secStr, "%d", sec);
	}
	return FYF_OK;
}



/*-----------------------------------------------------------------------------
	函数名：RTSP_ParseIpString2Value
		
	描述：
			把给定的ip字符串转换为整数值
	参数：
			in: 
				ipAdd:给定的ip字符串,例:"192.168.1.20"
			out:
				ipValue:转换后的整数值,例:0xc0a80114
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_ParseIpString2Value(char *ipAdd,int *ipValue)
{
	char *ipAddTemp = NULL;
	int len = 0;
	char ipStr[5] = {0};
	int ipValue2 = 0;
	unsigned char valueTemp = 0;
	int i=0;
	char *oriIpStr = NULL;

	oriIpStr = ipAdd;

	for(i = 0; i < 3; i++)
	{
		if(NULL != (ipAddTemp = strstr(ipAdd, ".")))
		{
			len = (int)(ipAddTemp - ipAdd);
			FYF_API_Memcpy(ipStr, ipAdd, len);
			valueTemp = atoi(ipStr);
			FYF_API_Memset(ipStr, 0, 5);
			ipValue2 |= (valueTemp << (8 * (3 - i)));
			ipAdd = ipAddTemp + 1;
		}
		else
		{
			OW_ERROR("RTSP_ParseIpString2Value err! ipAdd = %s \n",ipAdd);
			return FYF_ERR;
		}
	}

	FYF_API_Memcpy(ipStr, ipAdd, strlen(ipAdd));
	valueTemp = atoi(ipStr);
	FYF_API_Memset(ipStr, 0, 5);
	ipValue2 |= (valueTemp);

	*ipValue = ipValue2;
	OW_DEBUG("ipAdd = %s, ipValue = 0x%8x\n",oriIpStr,*ipValue);
	return FYF_OK;


}


/*-----------------------------------------------------------------------------
	函数名：RTSP_ParseRsponeOk
		
	描述：
			解析是否收到"200 OK"
	参数：
			in: 
				ResponeBuf:给定的数据
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_ParseRsponeOk(char *ResponeBuf)
{
	int i = 0;
	int len = 0;

	len = strlen(ResponeBuf);

	if(len == 0)
	{
		OW_DEBUG("RTSP_ParseRsponeOk 收到数据全部为0. \n" );
		return FYF_OK;
	}


	if(NULL != strstr(ResponeBuf,"200 OK"))
	{
		return FYF_OK;
	}
	else 
	{
		len = (300 > len)?len:300;
		OW_ERROR("RTSP_ParseRsponeOk err, len = %d\n",len);
#if 0		
		for(i = 0; i < len; i++)
		{
			OW_DEBUG("0x%x,",ResponeBuf[i]);
			if(i % 25 == 0)
			{
				OW_DEBUG("\n");
			}
		}
		
		OW_DEBUG("\n");
#endif		
		return FYF_ERR;
	}

}



int RTSP_ParseCloseReason(char *recText)
{

	char x_reason[15] = { 0 };

	if (RTSP_GetXMLFiled(recText, "x-Reason:", 1, x_reason, sizeof(x_reason)) == FYF_ERR)
	{
		OW_ERROR("RTSP_ParseCloseReason RTSP_GetXMLFiled get x-Reason err!!!\n");
		return FYF_ERR;
	}

	if (strcmp(x_reason, "ADMIN KICK OFF") == 0)
	{
		//TP_OW_DrawInfo("已被服务器断开", true, false, false); /*hqx:do it later*/
		OW_ERROR("已被服务器断开, ADMIN KICK OFF\n");
	}
	else if (strcmp(x_reason, "END") == 0)
	{
		printf("节目结束\n");
	}
	else if (strcmp(x_reason, "SOURCE ERR") == 0)
	{
		OW_ERROR("已被服务器断开, SOURCE ERR\n");
	}
	else if (strcmp(x_reason, "IP DENY") == 0)
	{
		OW_ERROR("已被服务器断开, IP DENY\n");
	}
	else if (strcmp(x_reason, "LOGIN FAIL") == 0)
	{
		OW_ERROR("已被服务器断开,LOGIN FAIL\n");
	}
	else if (strcmp(x_reason, "AUTH FAIL") == 0)
	{
	}
	else if (strcmp(x_reason, "NO RESOURCE") == 0)
	{
		OW_ERROR("已被服务器断开, NO RESOURCE\n");
	}
	else if (strcmp(x_reason, "PROXY FAIL") == 0)
	{
		OW_ERROR("已被服务器断开, PROXY FAIL\n");
	}
	else if (strcmp(x_reason, "PAUSE TIMEOUT") == 0)
	{
		OW_ERROR("已被服务器断开, PAUSE TIMEOUT\n");
	}
	else if (strcmp(x_reason, "EXIT TIMEOUT") == 0)
	{
		OW_ERROR("已被服务器断开, EXIT TIMEOUT\n");
	}
	else if (strcmp(x_reason, "START TIMEOUT") == 0)
	{
		OW_ERROR("已被服务器断开, START TIMEOUT\n");
	}
	else if (strcmp(x_reason, "NOT ALVIE") == 0)
	{
		OW_ERROR("已被服务器断开, NOT ALVIE\n");
	}
	printf("RTSP_ParseCloseReason %s \n", x_reason);
	return FYF_OK;

}

/*-----------------------------------------------------------------------------
	函数名：RTSP_ParseCDN_DESCRIBE_Rspone
		
	描述：
			
	参数：
			in: 
				
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_ParseCDN_DESCRIBE_Rspone(char *ResponeBuf,int *CSeqCount,char *SerName,char *Location)
{
	char *bufPointer = NULL;
	int  offset = 0;
	char buf1[50] = {0};
    char buf2[50] = {0};	

		
	if(NULL == ResponeBuf)
	{
		OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, NULL == ResponeBuf\n");
		return FYF_ERR;
	}

	if(NULL == (bufPointer = strstr(ResponeBuf, "Temporarily")))
	{
		OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find Temporaily.\n");
		return FYF_ERR;
	}
	
	if(NULL == (bufPointer = strstr(ResponeBuf, "CSeq:")))
	{
		OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find CSeq: \n");
		return FYF_ERR;
	}
	else
	{    
		 if((offset = (int)(strstr(bufPointer, "\n") - bufPointer))<0)
		{
			OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find enter key:\n");
			return FYF_ERR;
		}
	    strncpy(Location, buf1 + strlen("CSeq:"), offset - strlen("CSeq:"));
        *CSeqCount = atoi(buf2);
	}
	FYF_API_Memset(buf1, 0, sizeof(buf1));
	FYF_API_Memset(buf2, 0, sizeof(buf2));
	
	if(NULL == (bufPointer = strstr(ResponeBuf, "Server:")))
	{
		OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find Server: \n");
		return FYF_ERR;
	}
	else
	{    
		
		if((offset = (int)(strstr(bufPointer, "-") - bufPointer))<0)
		{
			OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find -: \n");
			return FYF_ERR;
		}
	    strncpy(SerName, buf1 + strlen("Server:"), offset - strlen("Server:"));
	 
	}
	FYF_API_Memset(buf1, 0, sizeof(buf1));
	FYF_API_Memset(buf2, 0, sizeof(buf2));
	if(NULL == (bufPointer = strstr(ResponeBuf, "Location:")))
	{
		OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find Location: \n");
		return FYF_ERR;
	}
	else
	{    
		if((offset = (int)(strstr(bufPointer, "\n") - bufPointer))<0)
		{
			OW_ERROR("RTSP_ParseCDN_DESCRIBE_Rspone ERROR, can't find enter key: \n");
			return FYF_ERR;
		}
	    strncpy(Location, buf1 + strlen("Location:"), offset - strlen("Location:"));
	}

	return FYF_OK;
}




int RTSP_ParseUServer_DESCRIBE_Rspone(char *ResponeBuf, int *PmtFreq,int *PmtPid,int *RangStart,int *RangEnd,int *VTrackId,int *ATrackId)
{
	char *DataPointer = NULL;
	char strTemp2[100] = {0};
	char strTemp[100] = {0};
	char strTempAudio[20] = {0};
	int i = 0;
	int j = 0;
	int inttmp1 = 0, inttmp2 = 0;
	
	if((NULL == ResponeBuf) || (NULL == PmtFreq) || (NULL == PmtPid))
	{
		OW_ERROR("(NULL==ResponeBuf)||(NULL==SerName)||(NULL==location) Error.\n");
		return FYF_ERR;
	}

	if(NULL == (DataPointer = strstr(ResponeBuf, "200 OK")))
	{
		OW_ERROR("回复错误\n");
		return FYF_ERR;
	}

	if(RTSP_GetFiledValue(ResponeBuf, "frequency:", PmtFreq) == FYF_ERR)
	{
		OW_ERROR("frequency: Error\n");
		return FYF_ERR;
	}

	if(RTSP_GetFiledValue(ResponeBuf, "x-pid:", (int*)PmtPid) == FYF_ERR)
	{
		OW_ERROR("x-pid: Error\n");
		return FYF_ERR;
	}

	if(NULL == (DataPointer = strstr(ResponeBuf, "range:")))
	{
		OW_ERROR("can not find filed range:\n");
		//return FYF_ERR;
		if(NULL == (DataPointer = strstr(ResponeBuf, "Range:")))
		{
			OW_ERROR("Range Error\n");
			//return FYF_ERR;
		}
		else
		{
			if(RTSP_GetFiledValue(DataPointer,"Range:", RangStart) == FYF_ERR)
			{
				OW_ERROR("Range Error\n");
				return FYF_ERR;
			}
		}
	}
	else
	{
		
		if(RTSP_GetFiledValue(DataPointer,"range:", RangStart) == FYF_ERR)
		{
			OW_ERROR("Range Error\n");
			return FYF_ERR;
		}


	}


	/*if(RTSP_GetFiledValue(DataPointer,"Range:", RangStart) == FYF_ERR)
	{
		OW_ERROR("Range: Error\n");
		return FYF_ERR;
	}*/

	if(NULL == (DataPointer = strstr(DataPointer, "-")))
	{
		OW_ERROR("RTSP_ParseUServer_DESCRIBE_Rspone - Error\n");
		return FYF_ERR;
	}

	if(RTSP_GetFiledValue(DataPointer, "-", RangEnd) == FYF_ERR)
	{
		OW_ERROR("RTSP_ParseUServer_DESCRIBE_Rspone - Error\n");
		return FYF_ERR;
	}

	if(NULL == (DataPointer = strstr(ResponeBuf, /*"mpgv"*/"video 0")))
	{
		OW_ERROR("Can't find video 0\n");
	}
	else
	{
		printf("find video 0===========\n");
		
		if(NULL == (DataPointer = strstr(DataPointer, "RTP/AVP")))
		{
			printf("Can't find RTP/AVP\n");
			//return FYF_ERR;
			if (OW_GetMPEGType() == FYF_FALSE)
			{
				return FYF_ERR;
			}
			
			if(NULL == (DataPointer = strstr(ResponeBuf, "mpgv")))
			{
				OW_ERROR("Can't find mpgv \n");
				return FYF_ERR;
			}
			else
			{
				printf("find mpgv===========\n");
				if(NULL == (DataPointer = strstr(DataPointer, "trackID=")))
				{
					OW_ERROR("Can't fine mpgv trackID=\n");
					return FYF_ERR;
				}
				
				if(RTSP_GetFiledValue(DataPointer, "trackID=", (int*)VTrackId) == FYF_ERR)
				{
					OW_ERROR("vTrackId trackID= Error\n");
					return FYF_ERR;
				}

			}
		}
		else 
		{

		
			
			if(RTSP_GetFiledValue(DataPointer, "MP4V-ES/", &video_rate) == FYF_ERR)
			{
				OW_ERROR("RTP/AVP  video_rate Error\n");
				//return FYF_ERR;
			}
			printf("video_rate = %d\n", video_rate);
			if(NULL == (DataPointer = strstr(DataPointer, "trackID=")))
			{
				OW_ERROR("Can't find RTP/AVP trackID=\n");
				return FYF_ERR;
			}


			if(RTSP_GetFiledValue(DataPointer, "trackID=", (int*)VTrackId) == FYF_ERR)
			{
				OW_ERROR("vTrackId trackID= Error\n");
				//return FYF_ERR;
			}

			if(NULL == (DataPointer = strstr(DataPointer, "fmtp")))
			{
				OW_ERROR("Can't find RTP/AVP video fmtp\n");
				return FYF_ERR;
			}
			else
			{
				
				if(NULL == (DataPointer = strstr(DataPointer, "config=")))
				{
					OW_ERROR("Can't find RTP/AVP video fmtp config=\n");
					return FYF_ERR;
				}

				
				if (RTSP_GetFiledString(DataPointer, "config=", strTemp) == FYF_ERR)
				{
					OW_ERROR("RTSP_GetFiledValue can't find  config= !!\n");
					return FYF_ERR;
				}
				inttmp1 = 0;
				inttmp2 = 0;

				printf("mp4Head = %s\n", strTemp);
				for (i = 0, j = 0; j < strlen(strTemp)/2; i++, j++)
				{
					if ((strTemp[i] >= '0') && (strTemp[i] <= '9'))
					{
						inttmp1 = strTemp[i] - '0';
					}
					else if ((strTemp[i] >= 'a') && (strTemp[i] <= 'f'))
					{
						inttmp1 = (strTemp[i] - 'a' + 10);
					}
					else if ((strTemp[i] >= 'A') && (strTemp[i] <= 'F'))
					{
						inttmp1 = (strTemp[i] - 'A' + 10);
					}
					
					
					if ((strTemp[i + 1] >= '0') && (strTemp[i + 1] <= '9'))
					{
						inttmp2 = strTemp[i + 1] - '0';
					}
					else if ((strTemp[i + 1] >= 'a') && (strTemp[i + 1] <= 'f'))
					{
						inttmp2 = (strTemp[i + 1] - 'a' + 10);
					}
					else if ((strTemp[i + 1] >= 'A') && (strTemp[i + 1] <= 'F'))
					{
						inttmp2 = (strTemp[i + 1] - 'A' + 10);
					}
					//printf("inttmp1 = %x, tmp2 = %x\n", inttmp1, inttmp2);
					mp4Head[j] = inttmp1 * 16 + inttmp2;
					//printf("%02x\n",mp4Head[j]);
					i++;
				}

				
	
			}
		}
		
	

		
	}

	if(NULL == (DataPointer = strstr(ResponeBuf, /*"mpga"*/"audio 0")))
	{
		OW_ERROR("Can't find video 0\n");
	}
	else
	{
		if(NULL == (DataPointer = strstr(DataPointer, "RTP/AVP")))
		{
			OW_ERROR("Can't find RTP/AVP\n");
			//return FYF_ERR;
			
			if (OW_GetMPEGType() == FYF_FALSE)
			{
				return FYF_ERR;
			}

			if(NULL == (DataPointer = strstr(ResponeBuf, "mpga")))
			{
				OW_ERROR("Can't find mpga \n");
				return FYF_ERR;
			}
			else
			{
				if(NULL == (DataPointer = strstr(DataPointer, "trackID=")))
				{
					OW_ERROR("Can't fine mpgv trackID=\n");
					return FYF_ERR;
				}
				
				if(RTSP_GetFiledValue(DataPointer, "trackID=", (int*)ATrackId) == FYF_ERR)
				{
					OW_ERROR("vTrackId trackID= Error\n");
					return FYF_ERR;
				}

			}
			
		}
		else 
		{

			

		
			if(RTSP_GetFiledValue(DataPointer, "mpeg4-generic/", &audio_rate) == FYF_ERR)
			{
				OW_ERROR("RTP/AVP  audio_rate Error\n");
				//return FYF_ERR;
			}
			printf("audio = %d\n", audio_rate);
			/*if(NULL == (DataPointer = strstr(DataPointer, "/")))
			{
				OW_ERROR("Can't find RTP/AVP  audio_rate mpeg4-generic/  / \n");
				//return FYF_ERR;
			}
			else
			{
				
			}*/
			
			if(NULL == (DataPointer = strstr(DataPointer, "trackID=")))
			{
				OW_ERROR("Can't fine mpgv trackID=\n");
				return FYF_ERR;
			}
			
			if(RTSP_GetFiledValue(DataPointer, "trackID=", (int*)ATrackId) == FYF_ERR)
			{
				OW_ERROR("vTrackId trackID= Error\n");
				//return FYF_ERR;
			}


			if(NULL == (DataPointer = strstr(DataPointer, "fmtp")))
			{
				OW_ERROR("Can't find RTP/AVP audio fmtp\n");
				return FYF_ERR;
			}
			else
			{
				if(NULL == (DataPointer = strstr(DataPointer, "mode=")))
				{
					OW_ERROR("Can't find RTP/AVP video fmtp config=\n");
					return FYF_ERR;
				}
				else
				{
					
					if (RTSP_GetFiledString(DataPointer, "mode=", strTempAudio) == FYF_ERR)
					{
						OW_ERROR("RTSP_GetFiledValue can't find  mode= !!\n");
						return FYF_ERR;
					}
					
					if (strTempAudio[0] == 'A' && strTempAudio[1] == 'C' && strTempAudio[2] == '3')
					{
						bAudioMode = FYF_ACODEC_TYPE_AC3;
					}
					else if (strTempAudio[0] == 'A' && strTempAudio[1] == 'A' && strTempAudio[2] == 'C')
					{
						bAudioMode = FYF_ACODEC_TYPE_AAC;
					}
					printf("audio mode = %d\n", bAudioMode);
				}
			
				if(NULL == (DataPointer = strstr(DataPointer, "config=")))
				{
					OW_ERROR("Can't find RTP/AVP video fmtp config=\n");
					return FYF_ERR;
				}

				
				if (RTSP_GetFiledString(DataPointer, "config=", strTemp2) == FYF_ERR)
				{
					OW_ERROR("RTSP_GetFiledValue can't find  config= !!\n");
					return FYF_ERR;
				}

				printf("mp4aHead = %s\n", strTemp2);
				inttmp1 = 0;
				inttmp2 = 0;
				for (i = 0, j = 0; j < 2; i++, j++)
			    {
					   if ((strTemp2[i] >= '0') && (strTemp2[i] <= '9'))
					   {
							   inttmp1 = strTemp2[i] - '0';
					   }
					   else if ((strTemp2[i] >= 'a') && (strTemp2[i] <= 'f'))
					   {
							   inttmp1 = (strTemp2[i] - 'a' + 10);
					   }
					   else if ((strTemp2[i] >= 'A') && (strTemp2[i] <= 'F'))
					   {
							   inttmp1 = (strTemp2[i] - 'A' + 10);
					   }
	

					   if ((strTemp2[i + 1] >= '0') && (strTemp2[i + 1] <= '9'))
					   {
							   inttmp2 = strTemp2[i + 1] - '0';
					   }
					   else if ((strTemp2[i + 1] >= 'a') && (strTemp2[i + 1] <= 'f'))
					   {
							   inttmp2 = (strTemp2[i + 1] - 'a' + 10);
					   }
					   else if ((strTemp2[i + 1] >= 'A') && (strTemp2[i + 1] <= 'F'))
					   {
							   inttmp2 = (strTemp2[i + 1] - 'A' + 10);
					   }
					   //printf("inttmp1 = %x, tmp2 = %x\n", inttmp1, inttmp2);
					   mp4aHead[j] = inttmp1 * 16 + inttmp2;
					   //printf("j = %d, %02x\n",j,mp4aHead[j]);
					   i++;
			     }

	
			}
		}
		
	

		
	}
#if 0
	if(NULL == (DataPointer = strstr(ResponeBuf, "fmtp:96")))
	{
		OW_ERROR("Can't fine mpgv\n");
	}
	else
	{
		if(NULL == (DataPointer = strstr(DataPointer, "config=")))
		{
			OW_ERROR("Can't fine mpgv config=\n");
			return FYF_ERR;
		}

		
		if (RTSP_GetFiledString(DataPointer, "config=", strTemp) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetFiledValue can't find  config= !!\n");
			return FYF_ERR;
		}

		printf("strTemp = %s\n", strTemp);
		
		for (i = 0, j = 0; j < 29; i++, j++)
		{
			if ((strTemp[i] >= '0') && (strTemp[i] <= '9'))
			{
				inttmp1 = strTemp[i] - '0';
			}
			else if ((strTemp[i] >= 'a') && (strTemp[i] <= 'f'))
			{
				inttmp1 = (strTemp[i] - 'a' + 10);
			}
			else if ((strTemp[i] >= 'A') && (strTemp[i] <= 'F'))
			{
				inttmp1 = (strTemp[i] - 'A' + 10);
			}
			
			
			if ((strTemp[i + 1] >= '0') && (strTemp[i + 1] <= '9'))
			{
				inttmp2 = strTemp[i + 1] - '0';
			}
			else if ((strTemp[i + 1] >= 'a') && (strTemp[i + 1] <= 'f'))
			{
				inttmp2 = (strTemp[i + 1] - 'a' + 10);
			}
			else if ((strTemp[i + 1] >= 'A') && (strTemp[i + 1] <= 'F'))
			{
				inttmp2 = (strTemp[i + 1] - 'A' + 10);
			}
			printf("inttmp1 = %x, tmp2 = %x\n", inttmp1, inttmp2);
			mp4Head[j] = inttmp1 * 16 + inttmp2;
			printf("%02x\n",mp4Head[j]);
			i++;
		}

		
	}

	if(NULL == (DataPointer = strstr(ResponeBuf, /*"mpgv"*/"video 0 RTP/AVP")))
	{
		OW_ERROR("Can't fine mpgv\n");
	}
	else
	{
		if(NULL == (DataPointer = strstr(DataPointer, "trackID=")))
		{
			OW_ERROR("Can't fine mpgv trackID=\n");
			return FYF_ERR;
		}

		if(RTSP_GetFiledValue(DataPointer, "trackID=", (int*)VTrackId) == FYF_ERR)
		{
			OW_ERROR("vTrackId trackID= Error\n");
			return FYF_ERR;
		}

		if(RTSP_GetFiledValue(DataPointer, "MP4V-ES/", &video_rate) == FYF_ERR)
		{
			OW_ERROR("vTrackId trackID= Error\n");
			return FYF_ERR;
		}
		
	}

	if(NULL == (DataPointer = strstr(ResponeBuf, /*"mpga"*/"audio 0 RTP/AVP")))
	{
		OW_ERROR("Can't fine mpga\n");
		return FYF_ERR;
	}
	else
	{
		if(NULL == (DataPointer = strstr(DataPointer, "trackID=")))
		{
			OW_ERROR("Can't fine mpga trackID=\n");
			return FYF_ERR;
		}
		if(RTSP_GetFiledValue(DataPointer, "trackID=", (int*)ATrackId) == FYF_ERR)
		{
			OW_ERROR("trackID aTrackId= Error\n");
			return FYF_ERR;
		}
		
		if(RTSP_GetFiledValue(DataPointer, "mpeg4-generic/", &audio_rate) == FYF_ERR)
		{
			OW_ERROR("vTrackId trackID= Error\n");
			return FYF_ERR;
		}
	}	
#endif
	printf("PmtFreq = %d\n", *PmtFreq);
	return FYF_OK;
}




int RTSP_ParseUServer_TDESCRIBE_Rspone(char *ResponeBuf,int *pmtFreq,int *pmtPid,int *rangStart,int *rangEnd,int *vTrackId,int *aTrackId)
{
	char *dataPointer = NULL;
	
	if((NULL == ResponeBuf) || (NULL == pmtFreq) || (NULL == pmtPid))
	{
		OW_ERROR("(NULL==ResponeBuf)||(NULL==SerName)||(NULL==location) Error\n");
		return FYF_ERR;
	}

	if(NULL == (dataPointer = strstr(ResponeBuf, "200 OK")))
	{
		OW_ERROR("回复错误\n");
		return FYF_ERR;
	}

	if(RTSP_GetFiledValue(ResponeBuf, "frequency:", pmtFreq) == FYF_ERR)
	{
		OW_ERROR("frequency: Error\n");
		return FYF_ERR;
	}

	if(RTSP_GetFiledValue(ResponeBuf, "x-pid:", pmtPid) == FYF_ERR)
	{
		OW_ERROR("x-pid: Error\n");
		return FYF_ERR;
	}

	if(NULL == (dataPointer = strstr(ResponeBuf, "mpgv")))
	{
		OW_ERROR("can't fine mpgv\n");
	}
	else
	{
		if(NULL == (dataPointer = strstr(dataPointer, "trackID=")))
		{
			OW_ERROR("can't fine mpgv trackID=\n");
			return FYF_ERR;
		}

		if(RTSP_GetFiledValue(dataPointer, "trackID=", vTrackId) == FYF_ERR)
		{
			OW_ERROR("vTrackId trackID= Error\n");
			return FYF_ERR;
		}
	}

	if(NULL == (dataPointer = strstr(ResponeBuf, "mpga")))
	{
		OW_ERROR("can't fine mpga\n");
		return FYF_ERR;
	}
	else
	{
		if(NULL == (dataPointer = strstr(dataPointer, "trackID=")))
		{
			OW_ERROR("can't fine mpga trackID=\n");
			return FYF_ERR;
		}
		if(RTSP_GetFiledValue(dataPointer, "trackID=", aTrackId) == FYF_ERR)
		{
			OW_ERROR("trackID aTrackId= Error\n");
			return FYF_ERR;
		}
	}
	
	return FYF_OK;
}


#define SOCKET
/*-----------------------------------------------------------------------------
	函数名：RTSP_SOCKET_SetNoneBlock
		
	描述：
			设置socket选项为非阻塞
	参数：
			in: 
				socketfd:socket句柄
				
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_SOCKET_SetNoneBlock(int socketfd)
{
	int ret = 0;

	ret = fcntl(socketfd, F_SETFL, O_NONBLOCK);
	if (ret < 0)
	{
		OW_ERROR("RTSP_SOCKET_Receive fcntl err!\n");
		return FYF_ERR;
	}

	return FYF_OK;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_SOCKET_OpenSocket
		
	描述：
			创建socket，并连接到ServIp
	参数：
			in: 
				ServIp:要连接的ip地址
				ServPort:端口
			out:
				sockets:socket句柄
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_SOCKET_OpenSocket(int *sockets,char *ServIp, char *ServPort)
{
	int m_s = -1;
	int ip = 0;
	int i = 0;
	int status = 0;
	int port = -1;
	struct sockaddr_in serv_addr;
	int ret = -1;
#if 1	
	struct timeval tv; 
	fd_set writefds; 
	int error;
	int tmp = EINPROGRESS;
	int seret = -1;
	int blockflag;
#endif

	RTSP_ParseIpString2Value(ServIp, &ip);
	port = atoi(ServPort);
	m_s = socket(AF_INET, SOCK_STREAM, 0);
	if(m_s <= 0)
	{
		OW_ERROR("Create Socket err! m_s = %d\n", m_s);
		return FYF_ERR;
	}

	OW_DEBUG("create socket success, m_s = %d\n", m_s);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(ip);
	*sockets = m_s;

	blockflag = fcntl(m_s, F_GETFL, 0); 
	blockflag |=  O_NONBLOCK;
	fcntl(m_s, F_SETFL, blockflag); 


	i = 0;
	while (i < 2)
    {
		status = connect(m_s, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
		if(status == 0)
		{
			
			printf("tcp connect() - OK!\n");
			
			break;
		}
		else
		{
			
			printf("connecting..........%d, errno = %d, status = %d\n",i, errno, status);
			i++;
			perror("");
			tv.tv_sec = 2; 
			tv.tv_usec = 0; 
			FD_ZERO(&writefds); 
			FD_SET(m_s, &writefds); 
			if ((seret = select(m_s + 1,NULL, &writefds, NULL, &tv)) > 0)
			{
				int len = sizeof(int); 
				getsockopt(m_s, SOL_SOCKET, SO_ERROR, &error, &len); 
				printf("error = %d, seret = %d\n", error, seret);
				perror("");
			}
			
			/*if (errno == EINPROGRESS)			
			{
		        tv.tv_sec = 3; 
		        tv.tv_usec = 0; 
		        FD_ZERO(&writefds); 
	        	FD_SET(m_s, &writefds); 
				if ((seret = select(m_s + 1,NULL, &writefds, NULL, &tv)) > 0)
				{
					int len = sizeof(int); 
					getsockopt(m_s, SOL_SOCKET, SO_ERROR, &error, &len); 
					printf("error = %d, seret = %d\n", error, seret);
					perror("");
				}
				printf("seret = %d\n", seret);
			}*/
			
		}
	}

	if (status != 0)
	{
		close(m_s);
		OW_ERROR("ipanel_tcp connect() - ERROR,pServerIP=%x, port = %d\n",ip, port);
		return FYF_ERR;
	}
	else
	{
		return FYF_OK;
	}
	
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_SOCKET_Send
		
	描述：
			发送数据
	参数：
			in: 
				socketfd:socket句柄
				buf:要发送的数据
				len:要发送的数据的长度
	返回：
			nbytes 实际发送的字节数
------------------------------------------------------------------------------*/

int RTSP_SOCKET_Send(int socketfd, const char *buf, int len)
{
	int nbytes = 0;

	nbytes = send(socketfd, (void *)buf, (int)len, 0);

	if(nbytes > 0)
	{
		OW_DEBUG("RTSP_SOCKET_Send success %d bytes at socket %d, and need send %d bytes  \n",nbytes, socketfd, len);   
		//printf("%s\n",buf);
	}
	else
	{
		OW_ERROR("RTSP_SOCKET_Send data err %d bytes at socket %d, and need send %d bytes\n",nbytes, socketfd, len);   
		nbytes = -1;
	}
	
	return nbytes;
}

/*-----------------------------------------------------------------------------
	函数名：RTSP_SOCKET_Receive
		
	描述：
			接收数据
	参数：
			in: 
				socketfd:socket句柄
				buf:要发送的数据
				len:要发送的数据的长度
	返回：
			nbytes 实际发送的字节数
------------------------------------------------------------------------------*/

int RTSP_SOCKET_Receive(int socketfd,char *buf,int len,int timeout)
{
	int nbytes = 0;
	int timeCount = 0;
	char RecBufTemp[2048] = {0};
	int nBytesTemp = 0;
	char *dataPointer = NULL;
	int textLen = 0;
	int ret = 0;



	ret = RTSP_SOCKET_SetNoneBlock(socketfd);
	if (ret < 0)
	{
		OW_ERROR("RTSP_SOCKET_SetNoneBlock error\n");
		return FYF_ERR;
	}

			
	while(1)
	{
		nBytesTemp = recv(socketfd, RecBufTemp, 2048, 0);	
		
		/*本次没接收到数据*/
		if(nBytesTemp <= 0)
		{
			/*先前也没收到数据*/
			if(nbytes <= 0)
			{
				FYF_API_thread_sleep(50);
				timeCount++;
				if(timeCount == timeout)
				{
					nbytes = -1;
					//printf("errno = %d\n", errno);
					
					//printf("errno = %d, error = %s\n", errno,strerror(errno));
					OW_ERROR("RTSP_SOCKET_Receive data err %d bytes at socket %d, and need rec %d bytes\n", nbytes, socketfd, len);   
					return nbytes;
				}
			}
			else          /*先前有收到数据*/
			{
				//OW_DEBUG("RTSP_SOCKET_Receive data success nbytes = %d \n",nbytes);   
				//OW_DEBUG("%s\n",buf);

				if(nbytes >= textLen)
				{
				    //OW_DEBUG("RTSP_SOCKET_Receive data success nbytes = %d \nbuf = %s\n",nbytes,buf);   
					return nbytes;
				}
				else
				{
					timeCount++;
					FYF_API_thread_sleep(100);
					if(timeCount == timeout)
					{
						OW_ERROR("注意数据接受不全nbytes = %d\n",nbytes);
						nbytes = -1;
						//OW_DEBUG("OW_DEBUG data err %d bytes at socket %d, and need rec %d bytes\n",nbytes,socketfd,len);   
						return nbytes;
					}
				}
		
			}
		}
		else
		{
			FYF_API_Memcpy((buf+nbytes), RecBufTemp, nBytesTemp);
			if(nbytes == 0)        /*说明第一次接收到数据,解析长度*/
			{
				if((dataPointer =strstr(RecBufTemp,"Content-Length"))== NULL)
				{
					//OW_ERROR("get data! but can't find Content-Length \n");
				}
				else 	if(RTSP_GetFiledValue(RecBufTemp, "Content-Length", &textLen) == FYF_ERR)
				{
					OW_ERROR("RTSP_SOCKET_Receive x-Content-Length: Err!\n");
				}
				else
				{
					OW_DEBUG("RTSP_SOCKET_Receive textLen = %d\n",textLen);
				}
			}
			nbytes += nBytesTemp;
			OW_DEBUG("get data! nBytesTemp = %d\n",nBytesTemp);
			FYF_API_Memset(RecBufTemp, 0, 2048);
		}
	}
}

#define  TSG

int RTSP_ParseTSGRsponeOK(char *ResponeBuf)
{
	if(RTSP_ParseRsponeOk(ResponeBuf) == FYF_ERR)
	{
		OW_ERROR("RTSP_ParseTSGRsponeOK  Error\n");
		return FYF_ERR;
	}
	return FYF_OK;
}


int RTSP_GetTSGChannelInfo(char *channelBuf)
{
	char *dataPointer = NULL;
	int i=0;
	char purchased[5] = {0};
	BU08 bGetChannel = FYF_FALSE;
	char errorcode[5] ={0}; 
	/*if(NULL ==(dataPointer = strstr(channelBuf, "errcode")))
	{
		OW_ERROR("RTSP_GetTSGChannelInfo can't find <errcode>.\n");
		return FYF_ERR;
	}
	else
	{
		//printf("datapoint = %s\n", dataPointer);
		
		      
		if(RTSP_GetXMLFiled(dataPointer, "errcode", 1, errorcode,sizeof(errorcode)) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetXMLFiled errcode Error\n");
		}
		//printf("%s\n",errorcode);
		if (strcmp(errorcode, "OK") != 0 )
		{
			return FYF_ERR;
		}
	}*/

	
	if(NULL ==(dataPointer = strstr(channelBuf, "Channels")))
	{
		OW_ERROR("RTSP_GetTSGChannelInfo can't find <Channels>.\n");
		return FYF_ERR;
	}
	dataPointer++;
	bGetChannel = FYF_TRUE;
	while(bGetChannel)
	{
		if(NULL == (dataPointer = strstr(dataPointer, "Channel ")))
		{
			OW_ERROR("RTSP_GetTSGChannelInfo end\n");
			return FYF_OK;
			//return FYF_ERR;
		}
		      
		if(RTSP_GetXMLFiled(dataPointer, "tsid", 1, TvodChanInfo.TsgChannel[i].TsId,sizeof(TvodChanInfo.TsgChannel[i].TsId)) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetXMLFiled tsid Error\n");
		}
		
		if(RTSP_GetXMLFiled(dataPointer, "serviceid",1,TvodChanInfo.TsgChannel[i].SerId,sizeof(TvodChanInfo.TsgChannel[i].SerId)) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetXMLFiled serviceid Error\n");
		}
		
		if(RTSP_GetXMLFiled(dataPointer, "url", 1, TvodChanInfo.TsgChannel[i].ProgURL,sizeof(TvodChanInfo.TsgChannel[i].ProgURL)) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetXMLFiled url Error\n");
		}
		
		if(RTSP_GetXMLFiled(dataPointer, "ispurchased", 1, purchased, sizeof(purchased)) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetXMLFiled ispurchased Error\n");
		}
		else
		{
			if(strcmp(purchased, "yes") == 0)
			{
				TvodChanInfo.TsgChannel[i].bPurchased = FYF_TRUE;
				TvodChanInfo.purProgCount++;
				FYF_API_Memset(purchased, 0, 5);
			}
		}
		dataPointer++;
		i++;
	}
	
	return FYF_ERR;
}



int RTSP_CreateTSGChannel_XML(int socket, char *TsgIpStr,char *TsgPortStr)
{
	char 	xmlData[SEND_BUF_LEN] = {0};
	int 	xmlDataLen = 0;

	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);

	OW_DEBUG("\n"); 	
/*#ifdef USE_MP2					
	memcpy(stbid,"21040010102052544c1e643a", strlen("21040010102052544c1e643a"));
#else
	//memcpy(stbid,"011204003011600024c104190b", strlen("011204003011600024c104190b"));
	//memcpy(stbid,"21040010102052544c1e643a", strlen("21040010102052544c1e643a"));
	memcpy(stbid,"11040010100052544c1a0099", strlen("11040010100052544c1a0099"));
#endif*/

	RTSP_CreateChannelXMLDATA(xmlData,stbid);
	xmlDataLen = strlen(xmlData);
	sprintf(sends, "POST /TSG/command HTTP/1.0\r\nHost: %s:%s\r\nUser-Agent: %s\r\nContent-Type: xml\nContent-Length: %d\r\n\r\n", TsgIpStr, TsgPortStr, User_Agent, xmlDataLen);
	strcat(sends, xmlData);

	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreateTSGChannel_XML  TP_SendAndRecText Error\n");
		return FYF_ERR;
	}

	printf("recvs:");
	printf("\033[0;31m%s\033[0m\n",recvs);
	if(RTSP_ParseTSGRsponeOK(recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_ParseTSGRsponeOK Error\n");
		return FYF_ERR;
	}

	//OW_DEBUG("RTSP_CreateTSGChannel_XML RECVS:\n%s\n",recvs);
	if(RTSP_GetTSGChannelInfo(recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_GetTSGChannelInfo Error\n");
		return FYF_ERR;
	}
	
	return FYF_OK;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_SendAndRecText
		
	描述：
			发送与接收数据
	参数：
			in: 
				socketfd:socket句柄
				sends:要发送的数据
			out:
				recvs:接收到的数据
	返回：
			0:成功   -1:失败
------------------------------------------------------------------------------*/
int RTSP_SendAndRecText(int socket, char *sends, char *recv)
{
	char 	eInfo[15] = {0};
	int 	ret = -1;
	static int rec_err_cnt = 0;
	static int rec_opt_err = 0;
	char *DataPointer = NULL;
	BU32 u32SeqValue = 0;
	BU32 u32RtpTime = 0;
	/*if (bHaveInitMutex == FYF_FALSE)
	{	
		pthread_mutex_init(&socket_mutex, NULL);
		bHaveInitMutex = FYF_TRUE;
	}*/

	//pthread_mutex_lock(&socket_mutex);

	OW_SetSuspendFlag(FYF_TRUE);
	/*ret = RTSP_CheckNetOk();
	if (ret != FYF_OK)
	{
		OW_VOD_SetNetErrFlag(FYF_TRUE);
	}
	else
	{
		OW_VOD_SetNetErrFlag(FYF_FALSE);
	}*/

	if (OW_VOD_GetNetErrFlag() == FYF_FALSE)
	{
		if((ret = RTSP_SOCKET_Send(socket, sends, strlen(sends))) < 0)
		{
			OW_ERROR("RTSP_SOCKET_Send Error, sent bytes = %x\n", ret);
			OW_VOD_SetNetErrFlag(FYF_TRUE);
			OW_SetSuspendFlag(FYF_FALSE);
			//pthread_mutex_unlock(&socket_mutex);
			return FYF_ERR;
		}
	}

		

	if((ret = RTSP_SOCKET_Receive(socket, recv, RECV_BUF_LEN, 50)) <= 0)
	{
		printf("============================!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!, bNetErr = %d\n", OW_VOD_GetNetErrFlag());
		
		//if(FYF_ERR == RTSP_ParseRsponeOk(recvs))
		{
			//if ()
			{
				printf("RTSP_SendAndRecText TP_OW_Rec err!!!\n");
				if(strstr(sends,"OPTIONS") != NULL)
				{
					rec_opt_err++;
					printf("rec_opt_err = %d\n", rec_opt_err);
					if(rec_opt_err == 5)
					{
						rec_opt_err = 0;
						OW_VOD_SetNetErrFlag(FYF_TRUE);
						OW_ERROR("OPTIONS ERROR, 可能是cabel猫掉线\n");
					}
					
					/*if(bCreateTimer)
					{
						if(TPSYSTEM_GetTimerStatus(Timerid))
						{
							bCreateTimer = false;
							TPSYSTEM_KillTimer(Timerid);
							TPGRAPH_DrawRectangle(x, y, w, h, TRANSPARENTCOLOR);
							TP_OW_DrawGIF2BMP(x, y, ow_BPZhun, &w, &h);
							if(owPlayPara.vodOrTvod == VOD)
							{
								TPGRAPH_DrawTextInRegion(x, y + 20, w, 25, "网络不通，退出点播", 12, BLACKCOLOR);
							}
							else if(owPlayPara.vodOrTvod == TVOD)
							{
								TPGRAPH_DrawTextInRegion(x, y + 20, w, 25, "网络不通，退出回看", 12, BLACKCOLOR);
							}
							MILLI_DELAY(3000);
							bOW_VOD_Exit = true;
							bNetErr = false;
							rec_err_cnt = 0;
							rec_opt_err = 0;
							printf("hahahaaaaaaah!\n");
							return TP_FAIL;
						}	
					}*/
		
				}
				else
				{
					rec_err_cnt++;
					if(rec_err_cnt == 2)
					{
						OW_ERROR("可能是cabel猫掉线， rec_err_cnt == 2\n");
						OW_VOD_SetNetErrFlag(FYF_TRUE);
					}
				}
				
				OW_ERROR("RTSP_SOCKET_Receive Error, recv bytes = %d\n", ret);
				//OW_VOD_SetNetErrFlag(FYF_TRUE);
				OW_SetSuspendFlag(FYF_FALSE);
				return FYF_ERR;
			}
			
		}
	}
	
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	rec_err_cnt = 0;
	rec_opt_err = 0;

	if(FYF_OK != RTSP_ParseRsponeOk(recv))
	{
		if(NULL != strstr(recv,"SET_PARAMETER"))
		{
	#if 1	
			if(RTSP_GetXMLFiled(recv, "x-Info:", 1, eInfo, sizeof(eInfo)) == FYF_ERR)
			{
				OW_ERROR("RTSP_GetXMLFiled Error, eInfo = %s\n", eInfo);
				
				OW_SetSuspendFlag(FYF_FALSE);
				return FYF_ERR;
			}
			
			if(strcmp(eInfo, "CLOSE") == 0)
			{
				serSetParaType = 0;
				RTSP_ParseCloseReason(recv);
				OW_SetExitFlag(FYF_TRUE);
				
				OW_DEBUG("RTSP_RecvSET_PARAMETER out from RTSP_GetXMLFiled get x-Info CLOSE!!!\n");
			}
			else if(strcmp(eInfo,"BOS") == 0)
			{
				
				serSetParaType = 1;
				//TPGRAPH_DrawRectangle(x+105, y,290, 80, BLUECOLOR);
				//TP_OW_DrawGIF2BMP(x + 105, y, ow_PP, &width, &height);
				printf("已超出时移起点\n");
				printf("从时移起点开始播放\n");
				//TPGRAPH_DrawTextInRegion(x + 105, y + 20, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
				//TPGRAPH_DrawTextInRegion(x + 105, y + 45, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
				VodPlayPara.reqScale = 1;
				//TPUSIF_ReadKey ( CURSOR_OFF, WAIT_FOR_1_SEC*3);
				FYF_API_thread_sleep(3000);
				//TPGRAPH_DrawRectangle(x + 105, y, width, height, TRANSPARENTCOLOR);
				OW_DrawSpeed(VodPlayPara.reqScale);
				VodPlayPara.bBOS = FYF_TRUE;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("RTSP_CreateOPTIONS RTSP_CreatePLAY  err!!\n ");
				}
				else
				{
					RTSP_PassBadFrame();
				}
				OW_DEBUG("RTSP_RecvSET_PARAMETER RTSP_GetXMLFiled get x-Info BOS!!!\n");
			}
			else if(strcmp(eInfo,"EOS") == 0)
			{
				
				serSetParaType = 2;
				//TP_OW_DrawGIF2BMP(x + 105, y, ow_PP, &width, &height);
				//TPGRAPH_DrawTextInRegion(x + 105, y + 20, width, 25, "已超出直播时间点", JCENTER, BLACKCOLOR);
				printf("已超出直播时间点\n");
				if (Ipanel_GetEntrance_Flag())
				{	//TPGRAPH_DrawTextInRegion(x + 105, y + 45, width, 25, "返回到浏览器", JCENTER, BLACKCOLOR);
					printf("返回到浏览器\n");
				}
				else
				{
					//TPGRAPH_DrawTextInRegion(x + 105, y + 45, width, 25, " 进入直播 ", JCENTER, BLACKCOLOR);
					
					printf(" 进入直播 \n");
				}
				FYF_API_thread_sleep(3000);
				//TPGRAPH_DrawRectangle(x + 105, y, width, height, TRANSPARENTCOLOR);
				OW_SetExitFlag(FYF_TRUE);
			}
		#endif	
#if 0      /*hqx:do it later*/
			RTSP_CheckSerKickOff(socket);
#endif
			OW_SetSuspendFlag(FYF_FALSE);

			return FYF_OK;
		}

		OW_ERROR("RTSP_ParseRsponeOk err sends = \n%s\n",sends);
		
		OW_SetSuspendFlag(FYF_FALSE);
		return FYF_ERR;

	}
	else
	{
		if(NULL != strstr(recv,"SET_PARAMETER"))
		{
			
			if (RTSP_GetXMLFiled(recv, "x-Info:", 1, eInfo, sizeof(eInfo)) == FYF_ERR)
			{
				OW_ERROR("RTSP_CheckSerKickOff RTSP_GetXMLFiled get x-Info err!!!\n");
				return FYF_ERR;
			}
			
			if (strcmp(eInfo, "CLOSE") == 0)
			{
				serSetParaType = 0;
				RTSP_ParseCloseReason(recv);
			
				if (VodPlayPara.VodOrTvod == 0)
				{
					VOD_ClearScreen();
					APP_Com_API_StopProg(1);
					//OW_DLRTPInfo_StopChannel();
					
					GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
					GUI_ENC_API_Update(ID_STA_THANKS, 1);
					
					FYF_API_thread_sleep(1000);
					VOD_ClearScreen();
					
					//OW_SetExitFlag(FYF_TRUE); 
					bPauseDisPly = FYF_FALSE;
					printf("RTSP_CheckSerKickOff, Exit vod==============\n");
					
					
				}
				else if (VodPlayPara.VodOrTvod == 1)
				{
			
				}
				OW_SetExitFlag(FYF_TRUE);
				//GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
				OW_DEBUG("RTSP_RecvSET_PARAMETER out from RTSP_GetXMLFiled get x-Info CLOSE!!!\n");
			}
			else if (strcmp(eInfo, "BOS") == 0)
			{
				serSetParaType = 1;
			
				/*if (GUI_ENC_API_GetCurState() == ID_DLG_TVOD_FAST)
				{
					
					FYF_API_OSD_FillRect(0, 0, 720, 576,0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
					
					
				}*/
				
				FULLTOTVOD_ClearScreen();
				printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );
				printf("已超出时移起点\n");
				printf("从时移起点开始播放\n");
				
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutTimeshiftStart, 0);
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);
				GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
				
				GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
			
				FYF_API_thread_sleep(2000);
				
				FULLTOTVOD_SetCurState(TVOD_PLAY_STATE/*3*/);
				GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
				FULLTOTVOD_ClearScreen();
				
				VodPlayPara.reqScale = 1;
				OW_DrawSpeed(VodPlayPara.reqScale);
				VodPlayPara.bBOS = FYF_TRUE;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("RTSP_CreateOPTIONS RTSP_CreatePLAY  err!!\n ");
				}
				else
				{
					RTSP_PassBadFrame();
				}
				OW_DEBUG("RTSP_RecvSET_PARAMETER RTSP_GetXMLFiled get x-Info BOS!!!\n");
						
				FULLTOTVOD_ClearScreen();
			
			}
			else if (strcmp(eInfo, "EOS") == 0)
			{
				serSetParaType = 2;
				
				FULLTOTVOD_ClearScreen();
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutLiveTime, 0);
				printf("已超出直播时间点\n");
				if (Ipanel_GetEntrance_Flag())
				{	
					printf("返回到浏览器\n");
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "返回浏览器", 0);
				}
				else
				{
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_ReturnToLive, 0);
					printf(" 进入直播 \n");
				}
				
				GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
				
				GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
				
				FYF_API_thread_sleep(3000);
			
				FULLTOTVOD_ClearScreen();
				OW_SetExitFlag(FYF_TRUE);
			}
		
			OW_SetSuspendFlag(FYF_FALSE);

			//return FYF_OK;
		}

		if(NULL == (DataPointer = strstr(recv, "RTP-Info:")))
		{
			OW_ERROR("Can't fine RTP-Info:\n");
		}
		else
		{
			if(NULL == (DataPointer = strstr(DataPointer, "url=trackID=0")))
			{
				OW_ERROR("Can't fine mpgv trackID=\n");
				//return FYF_ERR;
			}

			if(RTSP_GetFiledValue(DataPointer, "seq=", &u32SeqValue) == FYF_ERR)
			{
				OW_ERROR("vTrackId trackID= Error\n");
				//return FYF_ERR;
			}
			
			if(RTSP_GetFiledValue(DataPointer, "rtptime=", &u32RtpTime) == FYF_ERR)
			{
				OW_ERROR("vTrackId trackID= Error\n");
				//return FYF_ERR;
			}
			printf("start pts = %x\n", u32RtpTime/video_rate);
			//ptsValueInMS = u32RtpTime/90000;
			//g_PreviewPts =  u32RtpTime;
			g_RTSPPTS1 = u32RtpTime;

			
			if(NULL == (DataPointer = strstr(DataPointer, "url=trackID=1")))
			{
				OW_ERROR("Can't fine mpgv trackID=\n");
				//return FYF_ERR;
			}

			if(RTSP_GetFiledValue(DataPointer, "seq=", &u32SeqValue) == FYF_ERR)
			{
				OW_ERROR("vTrackId trackID= Error\n");
				//return FYF_ERR;
			}
			
			if(RTSP_GetFiledValue(DataPointer, "rtptime=", &u32RtpTime) == FYF_ERR)
			{
				OW_ERROR("vTrackId trackID= Error\n");
				//return FYF_ERR;
			}
			g_RTSPPTS2 = u32RtpTime;
			printf("audio pts = %x\n", u32RtpTime/video_rate);
		}


	}
	FYF_API_thread_sleep(1);
#if 0		/*hqx:do it later*/
	RTSP_CheckSerKickOff(socket);
#endif
	OW_SetSuspendFlag(FYF_FALSE);

	if(0xffffffff == VodPlayPara.cSeqCount)
	{
		VodPlayPara.cSeqCount = 1;
	}
	else
	{
		VodPlayPara.cSeqCount++;
	}



	return FYF_OK;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_CreateDescribe
		
	描述：
			发送DESCRIBE报文并解析
	参数：
			in: 
				socket:socket句柄
				RegionID:	RegionID
	返回：
			0:成功   -1:失败
------------------------------------------------------------------------------*/

int RTSP_CreateDescribe(int socket,  OW_PLAY_PARA * const play)
{
	int replayStartTime = 0;

	printf("RTSP_CreateDescribe.\n");

	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	
	sprintf(sends,
		"DESCRIBE %s RTSP/1.0\r\nCSeq: %d\r\nAccept: application/sdp\r\nUser-Agent: %s\r\nx-RegionID: 0x%x\r\n\r\n",
		play->progAddr, play->cSeqCount, User_Agent, play->regionID);

	printf("sends = %s\n",sends);

	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		if(NULL != strstr(recvs, "Moved Temporarily"))
		{
		
			RTSP_DefaultPlayPara(play);
			
			/*if(VodPlayPara.pCurTime	!= 0)
			{
				replayStartTime = VodPlayPara.pCurTime;
				VodPlayPara.pCurTime = replayStartTime;
			}
			else
			{
				//RTSP_DefaultPlayPara(&VodPlayPara);
			}*/
			
			if( RTSP_GetFiledString(recvs, "Location:", play->progAddr) == FYF_ERR)
			{
				OW_ERROR("RTSP_GetFiledString Error\n");
				return FYF_ERR;
			}
			printf("Location = %s\n", play->progAddr);

			//if (VodPlayPara.VodOrTvod == 1)
				//memcpy(play->progAddr, "rtsp://218.108.85.250:4554/cctv5", strlen("rtsp://218.108.85.250:4554/cctv5"));
			//			rtsp://218.108.85.250:4554/hszx

			play->bReLocate = FYF_TRUE;
			return FYF_OK;
		}
		OW_DrawSerErr();
		OW_ERROR("TP_CreateDescribe TP_SendAndRecText ERROR\n");
		return FYF_ERR;
	}
	
	
	if (RTSP_ParseUServer_DESCRIBE_Rspone(recvs, &(play->pFre), &(play->pPmtPid), &(play->pStartTime),
			&(play->pEndTime), &(play->pVTrackID), &(play->pATrackID)) == FYF_ERR)
	{
		OW_ERROR("RTSP_ParseUServer_DESCRIBE_Rspone Error\n");
		return FYF_ERR;
	}

	printf("pStartTime = %d\n", (play->pStartTime));
	return FYF_OK;
}




int RTSP_CreateSETUP(int socket, OW_PLAY_PARA * const play)
{
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);

	
	printf("RTSP_CreateSETUP.\n");

	if (OW_GetMPEGType() == FYF_FALSE)
	{
		sprintf(sends, "SETUP %s/trackID=%d RTSP/1.0\r\nCSeq: %d\r\nTransport: RTP/AVP/H.222;unicast;interleaved=0-1\r\nx-RegionID: 0x%x\r\nUser-Agent: %s\r\n\r\n",
			play->progAddr, play->pVTrackID, play->cSeqCount, play->regionID, User_Agent);

		if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
		{
			OW_ERROR("RTSP_SendAndRecText Error\n");
			return FYF_ERR;
		}

		//RTSP_IncreaseCSeqCount();  

		if( RTSP_GetFiledString(recvs, "Session:", play->session) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetFiledString Error\n");
			return FYF_ERR;
		}

		sprintf(sends, "SETUP %s/trackID=1 RTSP/1.0\r\nCSeq: %d\r\nTransport: RTP/AVP/H.222;unicast;interleaved=2-3\r\nx-RegionID: 0x%x\r\nUser-Agent: %s\r\n\r\n",
			play->progAddr, /*play->pATrackID,*/ play->cSeqCount, play->regionID, User_Agent);

		if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
		{
			OW_ERROR("RTSP_SendAndRecText Error\n");
			return FYF_ERR;
		}

		//RTSP_IncreaseCSeqCount();  

		if( RTSP_GetFiledString(recvs, "Session:", play->session) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetFiledString Error\n");
			return FYF_ERR;
		}


	}
	else
	{
		sprintf(sends, "SETUP %s/trackID=%d RTSP/1.0\r\nCSeq: %d\r\nTransport: MP2T\r\nx-RegionID: 0x%x\r\nUser-Agent: %s\r\n\r\n",
			play->progAddr, play->pVTrackID, play->cSeqCount, play->regionID, User_Agent);

		if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
		{
			OW_ERROR("RTSP_SendAndRecText Error\n");
			return FYF_ERR;
		}

		if(RTSP_GetFiledString(recvs, "Session:", play->session) == FYF_ERR)
		{
			OW_ERROR("RTSP_GetFiledString Error\n");
			return FYF_ERR;
		}
	}
	
	
	return FYF_OK;
}



int RTSP_CreatePLAY(int socket, OW_PLAY_PARA * const play)
{
	int trueScale = 0;
	char *dataPointer = NULL;
	BU32 tmpPtsstart = 0;
	printf("RTSP_CreatePLAY.\n");

	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	
	if (VOD == play->VodOrTvod)
	{
		sprintf(sends,
			"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nx-prebuffer: maxtime=%d.00\r\nRange: npt=%d.0-\r\nScale: %d\r\nUser-Agent: %s\r\n\r\n",
			play->progAddr, play->cSeqCount, play->session, play->maxTime, play->pCurTime, play->reqScale, User_Agent);
	}
	else
	{
		if (play->bBOS)
		{
			play->bBOS = FYF_FALSE;
			sprintf(sends,
				"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nRange: npt=bos-\r\nx-prebuffer: maxtime=0.00\r\nScale: %d.0\r\nUser-Agent: %s\r\n\r\n",
				play->progAddr, play->cSeqCount, play->session, play->reqScale, User_Agent);
		}
		else if (play->bEOS)
		{
			play->bEOS = FYF_FALSE;
			sprintf(sends,
				"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nRange: npt=eos-\r\nScale: %d.0\r\nx-prebuffer: maxtime=0.00\r\nUser-Agent: %s\r\n\r\n",
				play->progAddr, play->cSeqCount, play->session, play->reqScale, User_Agent);
		}
		else
		{
			sprintf(sends,
				"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nx-prebuffer: maxtime=%d.00\r\nRange: clock=%s--\r\nScale: %d\r\nUser-Agent: %s\r\n\r\n",
				play->progAddr, play->cSeqCount, play->session, play->maxTime,play->clockCur, play->reqScale, User_Agent);
		}
	}

	/*if (1 == play->reqScale)
	{

	}
	else
	{

	}*/


	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_SendAndRecText Error\n");
		return FYF_ERR;
	}
	
	if(TVOD == play->VodOrTvod)
	{
		if (bDrawSpeed)
		{
			OW_DrawSpeed(play->reqScale);
		}
		else
		{
			bDrawSpeed = FYF_TRUE;
		}
	}
	else
	{
		/*if (PTSStart == 0)
		{
			if (NULL == (dataPointer = strstr(recvs, "url=trackID=0")))
			{
				OW_ERROR("RTSP_ParseUServer_DESCRIBE_Rspone can't fine mpgv!!!\n");
				//return TP_FAIL;
			}
			else
			{
				if (NULL == (dataPointer = strstr(dataPointer, "rtptime")))
				{
					OW_ERROR("RTSP_ParseUServer_DESCRIBE_Rspone can't fine mpgv trackID=!!!\n");
					return FYF_ERR;
				}
			
				if (RTSP_GetFiledValue(dataPointer, "rtptime", &tmpPtsstart) == FYF_ERR)
				{
					OW_ERROR("RTSP_ParseUServer_DESCRIBE_Rspone vTrackId trackID= err!!!\n");
					return FYF_ERR;
				}
			}

			if (tmpPtsstart != 0)
			{
				PTSStart = tmpPtsstart/90000;
			}


		}*/
		
	}

	return FYF_OK;
}

int RTSP_CreatePAUSE(int socket, OW_PLAY_PARA * const play)
{
	printf("RTSP_CreatePAUSE.\n");

	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	sprintf(sends, "PAUSE %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nUser-Agent: %s\r\n\r\n", play->progAddr,
		play->cSeqCount, play->session, User_Agent);


	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreatePAUSE  RTSP_SendAndRecText Error\n");
		return FYF_ERR;
	}
	//RTSP_IncreaseCSeqCount();  
	return FYF_OK;
}

int RTSP_CreateTEARDOWN(int socket, char *session_id)
{
	printf("RTSP_CreateTEARDOWN.\n");

	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	
	sprintf(sends, "TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nUser-Agent: %s\r\n\r\n",VodPlayPara.progAddr, CSeqCountAll, session_id, User_Agent);
	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreateTEARDOWN  Err\n");
		return FYF_ERR;
	}
	
	//RTSP_IncreaseCSeqCount();  
	return FYF_OK;
}

int RTSP_CreateOPTIONS(int socket, BU08 *StopTag, OW_PLAY_PARA * const play)
{
	char *dataPointer = NULL;
	char eInfo[15] = {0};

	printf("RTSP_CreateOPTIONS.\n");
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	
	sprintf(sends, "OPTIONS %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nUser-Agent: %s\r\n\r\n", play->progAddr,
		play->cSeqCount, play->session, User_Agent);
	
	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
#if 0						/*hqx:do it later*/
		//optionMSGErrCount++;
		//OW_IncreOptionErrCount();
#endif
		OW_ERROR("RTSP_CreateOPTIONS  RTSP_SendAndRecText ERROR\n");
		return FYF_ERR;
	}
	
	if(NULL != strstr(recvs, "SET_PARAMETER"))
	{
		dataPointer = strstr(recvs,"x-Info:");
		if(NULL != dataPointer)
		{
			OW_DEBUG("RTSP_CreateOPTIONS  SET_PARAMETER recvs= %s\n", recvs);
			if(RTSP_GetXMLFiled(dataPointer, "x-Info:", 1, eInfo, sizeof(eInfo)) == FYF_ERR)
			{
				OW_ERROR("RTSP_RecvSET_PARAMETER RTSP_GetXMLFiled get x-Info Error\n");
				return FYF_ERR;
			}
			
			if(strcmp(eInfo, "CLOSE") == 0)
			{
				if(NULL != StopTag)
				{
					*StopTag = FYF_TRUE;
				}
				serSetParaType = 0;
#if 1						/*hqx:do it later*/			
				
				RTSP_ParseCloseReason(dataPointer);
#endif
				OW_SetExitFlag(FYF_TRUE);
				OW_DEBUG("RTSP_RecvSET_PARAMETER out from RTSP_GetXMLFiled get x-Info CLOSE\n");
			}
			else if(strcmp(eInfo, "BOS") == 0)
			{
				serSetParaType = 1;
#if 1						/*hqx:do it later*/			
				
				//TP_OW_DrawGIF2BMP(x+105 , y,ow_PP,&width,&height);
				//TPGRAPH_DrawTextInRegion(x+105, y+20, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
				//TPGRAPH_DrawTextInRegion(x+105, y+45, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
				printf("已超出时移起点\n");
				printf("从时移起点开始播放\n");
				play->reqScale = 1;
				//TPUSIF_ReadKey ( CURSOR_OFF, WAIT_FOR_1_SEC*3);
				//TPGRAPH_DrawRectangle(x+105, y,width, height, TRANSPARENTCOLOR);
				OW_DrawSpeed(play->reqScale);
				play->bBOS = FYF_TRUE;
				if(RTSP_CreateTPLAY(optionSocket, play) == FYF_ERR)
				{
					OW_DEBUG("TP_CreateOPTIONS TP_CreatePLAY  err!!\n ");
				}
				else
				{
					RTSP_PassBadFrame();
				}
#endif

				OW_DEBUG("RTSP_RecvSET_PARAMETER RTSP_GetXMLFiled get x-Info BOS\n");
			}
			else if(strcmp(eInfo,"EOS") == 0)
			{
				serSetParaType = 2;
#if 1						/*hqx:do it later*/			
				
				//TP_OW_DrawGIF2BMP(x+105 , y,ow_PP,&width,&height);
				
				printf("已超出直播时间点\n");
				//TPGRAPH_DrawTextInRegion(x + 105, y + 20, width, 25, "已超出直播时间点", JCENTER, BLACKCOLOR);
				if (Ipanel_GetEntrance_Flag())
				{	
					//TPGRAPH_DrawTextInRegion(x + 105, y + 45, width, 25, "返回到浏览器", JCENTER, BLACKCOLOR);
					printf("返回到浏览器\n");
				}
				else
				{	
					//TPGRAPH_DrawTextInRegion(x + 105, y + 45, width, 25, " 进入直播 ", JCENTER, BLACKCOLOR);
					printf("进入直播\n");
				}
				
				FYF_API_thread_sleep(3000);
				//TPGRAPH_DrawRectangle(x+105, y,width, height, TRANSPARENTCOLOR);
#endif 								
				OW_SetExitFlag(FYF_TRUE);
			}
		}
	}
	//RTSP_IncreaseCSeqCount();
#if 0						/*hqx:do it later*/			
	//optionMSGErrCount = 0;
	//OW_DefaultOptionErrCount();
#endif
	return FYF_OK;
}


int RTSP_CreateTDescribe(int socket, BU16 RegionID)
{
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	
	sprintf(sends, "DESCRIBE %s RTSP/1.0\r\nCSeq: %d\r\nAccept: application/sdp\r\nUser-Agent: %s\r\nx-RegionID: 0x%x\r\n\r\n",VodPlayPara.progAddr, CSeqCountAll, User_Agent, RegionID);

	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		if(NULL != strstr(recvs, "Moved Temporarily"))
		{
			RTSP_DefaultPlayPara(&VodPlayPara);
			if( RTSP_GetFiledString(recvs, "Location:", VodPlayPara.progAddr) == FYF_ERR)
			{   
				OW_ERROR("RTSP_GetFiledString Error\n");
				return FYF_ERR;
			}
			VodPlayPara.bReLocate = true;
			return FYF_OK;
		}
#if 0			/*hqx:do it later*/		
		TP_OW_DrawSerErr();
#endif
		OW_ERROR("RTSP_CreateTDescribe TP_SendAndRecText Error\n");
		return FYF_ERR;
	}
	
	RTSP_IncreaseCSeqCount();  

	if(RTSP_ParseUServer_TDESCRIBE_Rspone(recvs, &(VodPlayPara.pFre), &(VodPlayPara.pPmtPid), &(VodPlayPara.pStartTime), &(VodPlayPara.pEndTime), &(VodPlayPara.pVTrackID), &(VodPlayPara.pATrackID)) == FYF_ERR)
	{
		OW_ERROR("RTSP_ParseUServer_TDESCRIBE_Rspone ERROR\n");
		return FYF_ERR;
	}
	return FYF_OK;
}


int RTSP_ParseSecToHourMin(int orisec, int *hour, int *min, int *sec)
{
	*hour = orisec / 3600;
	
	printf("orisec = %d, hour = %d\n", orisec, *hour);
	orisec %= 3600;
	
	printf("orisec = %d\n", orisec);
	
	*min = orisec / 60;
	
	printf("orisec = %d, min = %d\n", orisec, *min);
	
	orisec %= 60;
	
	printf("orisec = %d\n", orisec);
	
	*sec = orisec;
	
	printf("orisec = %d, sec = %d\n", orisec,*sec);
}

int RTSP_CreateTGET_PARAMETER(int socket, OW_PLAY_PARA * const play)
{
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);

	printf("RTSP_CreateTGET_PARAMETER\n");
	sprintf(sends,
		"GET_PARAMETER %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nx-Timeshift_Range:0\r\nUser-Agent: %s\r\n\r\n",
		play->progAddr, play->cSeqCount, play->session, User_Agent);


	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreateTGET_PARAMETER  RTSP_SendAndRecText Error\n");
		return FYF_ERR;
	}

	if(RTSP_GetProgClock(recvs, play->clockStart, play->clockEnd, play->clockCur) == FYF_ERR)
	{
		OW_ERROR("RTSP_GetProgClock Error\n");
		return FYF_ERR;
	}
	return FYF_OK;
}


int RTSP_CreateTPLAY(int socket, OW_PLAY_PARA * const play)
{
	int trueScale = 0;
	
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
		
	if (VOD == play->VodOrTvod)
	{
		sprintf(sends,
			"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nx-prebuffer: maxtime=%d.00\r\nRange: npt=%d.0-\r\nScale: %d\r\nUser-Agent: %s\r\n\r\n",
			play->progAddr, play->cSeqCount, play->session, play->maxTime, play->pCurTime, play->reqScale, User_Agent);
	}
	else 
	{
		if (play->bBOS)
		{
			play->bBOS = FYF_FALSE;
			sprintf(sends,
				"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nRange: npt=bos-\r\nx-prebuffer: maxtime=0.00\r\nScale: %d.0\r\nUser-Agent: %s\r\n\r\n",
				play->progAddr, play->cSeqCount, play->session, play->reqScale, User_Agent);
		}
		else if (play->bEOS)
		{
			play->bEOS = FYF_FALSE;
			sprintf(sends,
				"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nRange: npt=eos-\r\nScale: %d.0\r\nx-prebuffer: maxtime=0.00\r\nUser-Agent: %s\r\n\r\n",
				play->progAddr, play->cSeqCount, play->session, play->reqScale, User_Agent);
		}
		else
		{
			sprintf(sends,
				"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nx-prebuffer: maxtime=%d.00\r\nRange: clock=%s--\r\nScale: %d\r\nUser-Agent: %s\r\n\r\n",
				play->progAddr, play->cSeqCount, play->session, play->maxTime,play->clockCur, play->reqScale, User_Agent);
		}
	}

   
	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreateTPLAY RTSP_SendAndRecText Error\n");
		return FYF_ERR;
	}
	

	
	if(TVOD == play->VodOrTvod)
	{
		if (bDrawSpeed)
		{
			OW_DrawSpeed(play->reqScale);
		}
		else
		{
			bDrawSpeed = true;
		}
	}

	//RTSP_IncreaseCSeqCount();  

	return FYF_OK;
}


int RTSP_CreateTPAUSE(int socket, char *session_id)
{
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);

	sprintf(sends, "PAUSE %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nUser-Agent: %s\r\n\r\n",VodPlayPara.progAddr, CSeqCountAll, session_id, User_Agent);

	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreateTPAUSE  ERROR\n");
		return FYF_ERR;
	}
	RTSP_IncreaseCSeqCount();  
	return FYF_OK;
}

int RTSP_CreateTTEARDOWN(int socket, char *session_id)
{
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
	
	sprintf(sends, "TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nSession: %s\r\nUser-Agent: %s\r\n\r\n", VodPlayPara.progAddr, CSeqCountAll, session_id, User_Agent);
	if(RTSP_SendAndRecText(socket, sends, recvs) == FYF_ERR)
	{
		OW_ERROR("RTSP_CreateTTEARDOWN  RTSP_SendAndRecText ERROR\n");
		return FYF_ERR;
	}
	
	RTSP_IncreaseCSeqCount();  
	return FYF_OK;
}


int RTSP_TVOD_ChannelInit(char *TsgAddr)
{
	int 	Sockfd = -1;
	char 	IpStr[20] = {0};
	char 	PortStr[10] = {0};

	RTSP_DefaultTvodChanInfo(&TvodChanInfo);
	
 	if(RTSP_GetIPStrAndPortStr(TsgAddr, IpStr, PortStr) == FYF_ERR)
 	{
		OW_ERROR("RTSP_TVOD_ChannelInit RTSP_GetIPStrAndPortStr Error\n ");
		return FYF_ERR;
	}
		
	if(RTSP_SOCKET_OpenSocket(&Sockfd, IpStr, PortStr) == FYF_ERR)
	{
		if(Sockfd >= 0)
		{
			close(Sockfd);
		}
		OW_ERROR("RTSP_TVOD_ChannelInit RTSP_SOCKET_OpenSocket Error\n ");
		return FYF_ERR;
	}
	
	if(RTSP_CreateTSGChannel_XML(Sockfd, IpStr, PortStr) == FYF_ERR)
	{
		close(Sockfd);
		OW_ERROR("RTSP_TVOD_ChannelInit RTSP_CreateTSGChannel_XML Error\n ");
		return FYF_ERR;
	}
	close(Sockfd);
	return FYF_OK;
}

int RTSP_TVOD_GetAllProgrameInfo(char *TsgAddr)	/*获取频道列表所有信息*/
{
	if(RTSP_TVOD_ChannelInit(TsgAddr) == FYF_ERR)
	{
		OW_ERROR("RTSP_TVOD_GetAllProgrameInfo RTSP_TVOD_ChannelInit Error\n ");
		return FYF_ERR;
	}
	return FYF_OK;
}



int RTSP_TVOD_GetProInfo(char *tsgAddr,char *tsgPort)
{
	char  	tsgIpAddr[30] = {0};
	char  	tsgIpAddrTemp[20] = {0};
	BU08    bEXIT  = FYF_FALSE;

	if (bGetTsgSuc == FYF_TRUE)
	{
		return;
	}


	if (OW_VOD_GetNetErrFlag() == FYF_TRUE)	
	{
		if (GUI_ENC_API_GetCurState() == ID_DLG_ChnInfo)
		{
			return FYF_ERR;
		}
	}

#if 1			/*hqx:do it later*/
	if(RTSP_CheckNetOk() != FYF_OK)
	{
		return FYF_ERR;
	}
		
	bGetTsgSuc = FYF_FALSE;
#endif	
	  
	if(!bEXIT && (DNS_GetIPByDNS(tsgAddr,  tsgIpAddrTemp) == FYF_ERR))
	{
		OW_ERROR("RTSP_TVOD_GetProInfo DNS_GetIPByDNS err!\n ");
		bEXIT = FYF_TRUE;
	}
	else
	{
		//memcpy(tsgIpAddrTemp, "218.108.85.252", strlen("218.108.85.252"));
		printf("注意这里设固定值\n");
		sprintf(tsgIpAddr,"rtsp://%s:%s/test.ts",tsgIpAddrTemp,tsgPort);
		OW_DEBUG("RTSP_TVOD_GetProInfo t tsgIpAddr = %s!\n ",tsgIpAddr);
	}
	
	if(!bEXIT && (RTSP_TVOD_GetAllProgrameInfo(tsgIpAddr) == FYF_ERR))
 	{
		OW_ERROR("RTSP_TVOD_GetAllProgrameInfo err!\n ");
		bEXIT = FYF_TRUE;
	}

	if(bEXIT)
	{
		printf("out TP_OW_TVOD_GetProInfo error\n");
		return FYF_ERR;
	}

#if 1
	bGetTsgSuc = FYF_TRUE;
#endif
	printf("out TP_OW_TVOD_GetProInfo \n");
	return FYF_OK;
}



int RTSP_RetryTsgService(void)
{
	int ret = -1;
	ret = FYF_API_GetLinkStatus();
	if (ret != 1)
	{
		OW_ERROR("RTSP_RetryTsgService, check net\n");

		return 2;
	}

	
	OW_DEBUG("RTSP_RetryTsgService\n");

	memset(getipaddr, 0, sizeof(getipaddr));
	FYF_API_ETH_IPAddressGet(getipaddr);
	
	if (strlen(getipaddr) == 0)
	{
		strcpy(getipaddr, "000.000.000.000");
		OW_ERROR("FYF_API_ETH_IPAddressGet getip = %s\n",getipaddr);
		
		return FYF_ERR;
	}
	printf("getipaddr = %s\n", getipaddr);

	

	if (RTSP_TVOD_GetProInfo(fuzhouTSGAddr, fuzhouTSGPort) == FYF_ERR)
	{
		OW_ERROR("TP_OW_TVOD_GetProInfo err again!\n ");
		return FYF_ERR;
	}

	/*hqx: do it later*/
	ret = RTSP_CheckChannel();
	if (ret != FYF_OK)
	{
		OW_ERROR("当前节目不是时移!!!!");
		return FYF_ERR;
	}
		
	return FYF_OK;
}


#define XML

/*-----------------------------------------------------------------------------
	函数名：RTSP_CreateFiled
		
	描述：
			生成报文中有哪些项目，如:DESCRIBE,CSeq:,Accept:,User-Agent:
	参数：
			in: 
				DataBuf: 原始报文数据
				FiledName:新增报文项目
				BeforeFiledName:新增报文要加在哪项之前
				AfterFiledName: 新增报文要加在哪项之后
			out:
				DataBuf:加入项目后生成新的报文
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_CreateFiled(char *DataBuf, char *FiledName, char *BeforeFiledName,char *AfterFiledName)
{
	int  OriLen = 0;
	int  FiledNameLen = 0;
	char *OriFiledNameTemp = NULL;
	int  OriFiledNameOffset = 0;
	int  OriFiledLen = 0;
	char BufTemp[SEND_BUF_LEN] = {0};
	
	if((NULL == DataBuf))
	{
		OW_ERROR("TP_CreateFiled err!(NULL == DataBuf)!!\n");
		return FYF_ERR;
	}

	if(NULL != strstr(DataBuf, FiledName))
	{
		OW_DEBUG("This buf had this filed! FiledName = %s\n",FiledName);
		return FYF_OK;
	}
	else
	{	
		if((BeforeFiledName != NULL) && (NULL != (OriFiledNameTemp = strstr(DataBuf, BeforeFiledName))))
		{
			OriFiledNameOffset = (int)( OriFiledNameTemp - BeforeFiledName );
			FYF_API_Memcpy(BufTemp, DataBuf, OriFiledNameOffset);
			strcat(BufTemp, FiledName);
			strcat(BufTemp, "\r\n");
			FYF_API_Memcpy((char*)(BufTemp + strlen(BufTemp)), OriFiledNameTemp, (strlen(DataBuf) - OriFiledNameOffset));
			FYF_API_Memcpy(DataBuf, BufTemp, SEND_BUF_LEN);
			return FYF_OK;
		}
		else if((AfterFiledName != NULL) && (NULL != (OriFiledNameTemp = strstr(DataBuf, AfterFiledName))))
		{
			OriFiledNameOffset = (int)( OriFiledNameTemp - AfterFiledName );
			RTSP_SearchLineEnd(OriFiledNameTemp, &OriFiledLen);
			OriFiledNameOffset += OriFiledLen;
			FYF_API_Memcpy(BufTemp, DataBuf, OriFiledNameOffset);
			strcat(BufTemp, FiledName);
			strcat(BufTemp, "\r\n");
			FYF_API_Memcpy((char*)(BufTemp + strlen(BufTemp)),OriFiledNameTemp, (strlen(DataBuf) - OriFiledNameOffset));
			FYF_API_Memcpy(DataBuf, BufTemp, SEND_BUF_LEN);
			return FYF_OK;
		}
		else
		{
			OriLen = strlen(DataBuf);
			FiledNameLen = strlen(FiledName);
			if((OriLen + FiledNameLen) < SEND_BUF_LEN)
			{
				strcat(DataBuf, FiledName);
				strcat(DataBuf, "\r\n");
				strcpy(BufTemp, DataBuf);
				FYF_API_Memcpy(DataBuf, BufTemp, SEND_BUF_LEN);
				return FYF_OK;
			}
			else
			{
				OW_ERROR("(oriLen + filedNameLen) > SEND_BUF_LEN !\n");
				return FYF_ERR;
			}
		}
	}
}



int RTSP_GetXMLFiled(char *dataBuf,char *filedName,int num,char *fileNameStr,int nameSize)
{
	int i;
	int j;
	char *filedNameTemp = NULL;
	int bufLen ;
	int offset;
	int lenTemp;
	
	if((NULL == dataBuf)||((NULL == fileNameStr)))
	{
		OW_ERROR("RTSP_GetXMLFiled err, (NULL == dataBuf).\n");
		return FYF_ERR;
	}

	bufLen = strlen(dataBuf);

	for(i = 0; i < num; i++)
	{
		if(NULL ==(filedNameTemp = strstr(dataBuf, filedName)))
		{
			OW_ERROR("RTSP_GetXMLFiled err, can't find filedName = %s\n",filedName);
			return FYF_ERR;
		}
		else
		{
			if((i + 1) != num)
			{
				dataBuf = filedNameTemp + 1;
				continue;
			}

			
			offset = (int)(filedNameTemp - dataBuf  ) ;
			offset += strlen(filedName);
			lenTemp = bufLen - offset;
			filedNameTemp = dataBuf + offset;
			
			for(j = 0; j < lenTemp; j++)
			{
				if(filedNameTemp[j] == '"')
				{
					break;
				}
			}
			
			if(j == lenTemp)
			{
				OW_ERROR("RTSP_GetXMLFiled err, can't find ''\n");
				return FYF_ERR;
			}
			
			filedNameTemp += (j + 1);
			lenTemp -= j;
			for(j = 0; (j < lenTemp && j < nameSize); j++)
			{
				if(filedNameTemp[j] != '"')
				{
					fileNameStr[j] = filedNameTemp[j];
				}
				else
				{
					break;
				}
			}
			
			if(j == lenTemp)
			{
				OW_ERROR("RTSP_GetXMLFiled err, Can't find ''\n");
				return FYF_ERR;
			}
			//OW_DEBUG("RTSP_GetXMLFiled sueecss, %s = %s \n",filedName,fileNameStr);
			return FYF_OK;
		}
	}
	return FYF_ERR;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_UpdateXMLFiled
		
	描述：
			更新报文中的数据
	参数：
			in: 
				DataBuf: 原始报文数据
				FiledName:报文项目
				Num:数量
				AfterFiledName: 报文数据
			out:
				DataBuf:加入项目后生成新的报文
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_UpdateXMLFiled(char *DataBuf,char *FiledName,int Num,char *FileNameValue)
{
	char *FiledNameTemp = NULL;
	int  Offset = 0;
	int  OffsetTemp = 0;
	char BufTemp[SEND_BUF_LEN] = {0};
	int  i=0;
	
	if((NULL == DataBuf))
	{
		OW_ERROR("RTSP_UpdateXMLFiled err! (NULL == dataBuf) !\n");
		return FYF_ERR;
	}
	
	for(i = 0; i < Num; i++)
	{
		if(NULL ==(FiledNameTemp = strstr(DataBuf, FiledName)))
		{
			OW_ERROR("RTSP_UpdateXMLFiled err! Can't find FiledName = %s\n",FiledName);
			return FYF_ERR;
		}
		else
		{
			if((i + 1) != Num)
			{
				DataBuf = FiledNameTemp + 1;
				continue;
			}

			OffsetTemp = (int)(FiledNameTemp - DataBuf  ) ;
			Offset = OffsetTemp + strlen(FiledName);
			
			FYF_API_Memcpy(BufTemp, DataBuf, Offset);
			BufTemp[Offset] = '"';
			strcat(BufTemp, FileNameValue);
			BufTemp[Offset + 1 + strlen(FileNameValue)] = '"';
			FYF_API_Memcpy((char*)(BufTemp + strlen(BufTemp)), (char *)(FiledNameTemp + strlen(FiledName)), (strlen(DataBuf) - OffsetTemp));
			FYF_API_Memcpy(DataBuf, BufTemp, strlen(BufTemp));
			return FYF_OK;
		}
	}
	
	return FYF_ERR;
}


/*-----------------------------------------------------------------------------
	函数名：RTSP_CreateChannelXMLDATA
		
	描述：
			创建XML
	参数：
			in: 
				XmlBuf1: 原始报文数据
				StbID:   stbid
			out:
				XmlBuf1:加入项目后生成新的XML
	返回：
			0: 成功    -1: 失败
------------------------------------------------------------------------------*/

int RTSP_CreateChannelXMLDATA(char *XmlBuf,char *StbID)
{
	RTSP_CreateFiled(XmlBuf, "<?xml version= encoding=?>", NULL, NULL);
	RTSP_CreateFiled(XmlBuf, "<Message module= version= >", NULL, NULL);
	RTSP_CreateFiled(XmlBuf, "<Header action= command=/>", NULL, NULL);
	RTSP_CreateFiled(XmlBuf, "<Body>",NULL,NULL);
	RTSP_CreateFiled(XmlBuf, "<STB id=/>",NULL,NULL);
	RTSP_CreateFiled(XmlBuf, "</Body>",NULL,NULL);
	RTSP_CreateFiled(XmlBuf, "</Message>\r\n",NULL,NULL);

	RTSP_UpdateXMLFiled(XmlBuf, "version=", 1, "1.0");
	RTSP_UpdateXMLFiled(XmlBuf, "encoding=", 1, "GBK");
	RTSP_UpdateXMLFiled(XmlBuf, "module=", 1, "TSG");
	RTSP_UpdateXMLFiled(XmlBuf, "version=", 2, "1.0");
	RTSP_UpdateXMLFiled(XmlBuf, "action=", 1, "REQUEST");
	RTSP_UpdateXMLFiled(XmlBuf, "command=", 1, "GET_CHANNEL_LIST_AND_CODE");
	RTSP_UpdateXMLFiled(XmlBuf, "id=", 1, StbID);
	printf("XMLbuf1 = %s\n", XmlBuf);
	return FYF_OK;
}

#if 0
#define TIMER

/*******************************************************************************
* Function:     HI_3RD_INPUT_ReqTimer
* Description:  创建定时器
* Input:        s32TimerMode-定时器模式，pTimerCallback-定时器回调函数
* Output:
* Return:       HI_FAILURE:失败，HI_SUCCESS:成功
********************************************************************************/
BS32 RTSP_CreateTimer(void * pTimerCallback, BS32 s32TimerMode)
{
    petimer_t timer = NULL;
    pthread_t tid;

    if(NULL == s_TrdTimerEntry)
    {
        petimer_start(&tid, NULL, &s_TrdTimerEntry);
    }

    timer = petimer_create_reltimer(pTimerCallback, s32TimerMode);

    return timer;
}

/*******************************************************************************
* Function:     HI_3RD_INPUT_DestroyTimer
* Description:  销毁定时器
* Input:        s32ClkTag-定时器句柄
* Output:
* Return:       HI_FAILURE:失败，HI_SUCCESS:成功
********************************************************************************/
BS32 RTSP_DestroyTimer(BS32 s32TimerHdl)
{
    return petimer_free_reltimer(s_TrdTimerEntry, (petimer_t)(s32TimerHdl));
}

/*******************************************************************************
* Function:     HI_3RD_INPUT_ReqTimer
* Description:  使能定时器
* Input:        s32TimerHdl-定时器句柄,u32Timems : 时间间隔, pParam : 回调函数参数
* Output:
* Return:       HI_FAILURE:失败，HI_SUCCESS:成功
********************************************************************************/
BS32 RTSP_EnableTimer(BS32 s32TimerHdl, BU32 u32Timems, void * pParam)
{
    return petimer_start_reltimer(s_TrdTimerEntry, (petimer_t)(s32TimerHdl), u32Timems, pParam);
}

/*******************************************************************************
* Function:     HI_3RD_INPUT_DestroyTimer
* Description:  不使能定时器
* Input:        s32ClkTag-定时器句柄
* Output:
* Return:       HI_FAILURE:失败，HI_SUCCESS:成功
********************************************************************************/
BS32 RTSP_DisableTimer(BS32 s32TimerHdl)
{
    return petimer_stop_reltimer_safe(s_TrdTimerEntry, (petimer_t)(s32TimerHdl), NULL, NULL);
}


void RTSP_TimerCallback(void *p)
{
	
}
#endif

