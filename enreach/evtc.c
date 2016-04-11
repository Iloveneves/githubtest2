/**********************************文件头部注释*******************************************/

//////////////////////////////////////////////////////////////////////////////////////////
//
// 文件名：     evtc.c
//
// 创建者：     曾振宇
//
// 创建时间：   2010.04.07
//
// 文件描述：   
//
// 修改记录： 日       期      作      者       修定
//                    ---------         ---------         -----
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "../ipanel2.0/ipanel_porting_event.h"
#include "../ipanel2.0/ipanel_socket.h"
#include "../ipanel2.0/ipanel_base.h"
#include "../ipanel2.0/trd_ipanel_init.h"
#include "../ipanel2.0/ipanel_typedef.h"
#include "../ipanel2.0/ipanel_os.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"
#include "../hisiinclude/hi_stb_api.h"
#include "../hiadp/include/msg_event.h"
#include "../hisiinclude/higo/hi_go.h"
#include "../tpmiddle/include/app_sys.h"
#include "../hisiinclude/common/hi_type.h"
#include "evtc.h"
#include "enreach_keymap.h"
#include "../tpmiddle/include/app_basic_type.h"
#include "../tpmiddle/include/app_dba.h"
//#include "en_gif_data.h"  /*pingbi by zhengsenlin*/


EVTC_TASK en_taskId[EN_TASKID_MAX];
unsigned char hwmacs1[13] = "00606e56bf83";
unsigned char hwmacs[13];
unsigned char hwmacs2[13] = "00606e56bf84";
char revbuf[MAX_SOCKET_NUM][REV_BUF_SIZE] = {0};
int cur_ts_index = -1;
int bTSVOD = false;
static sem_t   *sem_protect_socket = 0;


extern EVTC_TSLIST *Timeshiftlist;
extern int tvod_num ;/*时移节目个数*/
extern int b_get_tslist;
extern int	 iChannelCurListingNo;
extern PROG_TYPE vodortvod ;
int reved_data_len = 0;

#define  	 JLEFT   		10
#define   	JRIGHT 	 		11
#define   	JCENTER	 		12
#define 	PROTECT         0

int evtc_drawReLogo(void);
int evtc_clearReLogo(void);

void evtc_init_sem_prtct(void)
{	
	int ret;
	#if PROTECT
	ret = HI_OS_SemaphoreCreate(1, &my_sem);
	if(ret != 0)
	{	
		printf(" _________________create semaphore error________________");
	}	
	#endif
}

void evtc_lock_socket(void)
{	
	#if PROTECT
	if(sem_protect_socket != 0)
	{
           HI_OS_SemaphoreWait(sem_protect_socket,TIMEOUT_INFINITY);
	}
	#endif
}

void evtc_unlock_socket(void)
{
	#if PROTECT
	if(sem_protect_socket != 0)
	{
       	 HI_OS_SemaphoreSignal(sem_protect_socket);
	}
	#endif
}

int evtc_get_tsvodtag(vid)
{
	return bTSVOD;
}


void evtc_dbg (const char *fmt, ...)
{
#if 0
	char report_buffer[8 * 1024];
	va_list list;

	va_start(list, fmt);
	vsprintf(report_buffer, fmt, list);
	va_end(list);
//	ENREACH_DEBUG(("enreach:     "));
//	ENREACH_DEBUG((report_buffer));
	printf("enreach:     ");
	printf(report_buffer);
#endif
}


void evtc_printf (const char *fmt, ...)
{
#if 0
	char report_buffer[4 * 1024];
	va_list list;

	va_start(list, fmt);
	vsprintf(report_buffer, fmt, list);
	va_end(list);
//	ENREACH_DEBUG(("evtc:     "));
//	ENREACH_DEBUG((report_buffer));
	printf("evtc:     ");
	printf(report_buffer);
#endif
}

unsigned long evtc_getCurMS(void)
{
	return FYF_API_time_ms();
}

long evtc_sock_open( EVTC_SOCK *sock )
{
	int m_s;
	struct sockaddr_in serv_addr;
	struct timeval tm = { 10, 0 };	
	int status, iIndex;
	int ip = sock->addr;
	int port = sock->port;
	int type = 0;	
	struct in_addr addr;
	struct timeval tv; 
	fd_set writefds; 
	int error;
	int tmp = EINPROGRESS;
	int seret = -1;
	int blockflag;

	evtc_lock_socket();

	m_s = socket(AF_INET, SOCK_STREAM , 0);
	if (m_s < 0)
	{
		evtc_printf("evtc_sock_open Failed\n");
		m_s = -1;
		evtc_unlock_socket();
		return m_s;
	}

	memset(&serv_addr, 0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ip;	//ntohl(ip);
	serv_addr.sin_port = htons(port);
	addr.s_addr = ip;

	blockflag = fcntl(m_s, F_GETFL, 0); 
	blockflag |=  O_NONBLOCK;
	fcntl(m_s, F_SETFL, blockflag); 
	
	evtc_printf("evtc_sock_open ip = %s,port = %d\n", inet_ntoa(addr),port);
	/*set connect timeout, if tcp. and Not set if other */
	for (iIndex = 0; iIndex < 2; iIndex++)	//for dahua connect i<3 but in huawei it is too long so change i<2
	{
		status = connect(m_s, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (status == 0)
		{
			break;
		}
		else
		{	
			evtc_printf("connecting....%d\n", iIndex);
			printf("connecting..........%d, errno = %d, status = %d\n",iIndex, errno, status);
			//iIndex++;
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
		}
	}
	if (status != 0)
	{
		close(m_s);
		evtc_unlock_socket();
		evtc_printf("Socket Connect Error status = %d\n", status);
		return -1;
	}
	sock->socket = m_s;
	evtc_unlock_socket();
	return m_s;
}

long evtc_sock_send( EVTC_SOCK *sock, char *buf, long len )
{
	long ret = 0;


	if (!sock || sock->socket < 0 || !buf || len <= 0)
	{	
		return -1;
	}

	if (len > 1400)
	{
		evtc_printf("注意发送字节长 %d\n", len );
	}
	evtc_lock_socket();
	ret = send(sock->socket, buf, len, 0);

	evtc_msleep(10);//此处为了防止时序混乱而使快进快退无法进行。
	

	if (ret < 0)
	{
		evtc_printf("send fail %d\n", ret);
		evtc_unlock_socket();
		return -1;
	}
	evtc_unlock_socket();
	reved_data_len = 0;
	evtc_printf("sock_send ret=%d\n", ret);
	return ret;
}
	
long evtc_sock_select(EVTC_SOCK * sock)
{
	struct timeval timeOut = {0,7};
	fd_set fds;
	long i, ret;
	
	//evtc_printf("evtc_sock_select !\n");

	FD_ZERO(&fds);
	FD_SET((unsigned long)(sock->socket), &fds);
	ret = select(FD_SETSIZE, &fds, 0, 0, &timeOut);
	if (ret > 0)
	{
		if (FD_ISSET(sock->socket, &fds))
		{
			evtc_printf("evtc_sock_select ok!\n");
			return 0;
		}
		return -1;
	}
	else if (ret < 0)
	{
		return -1;
	}
	
	return -1;
}
#if 1

int  test_rec(int s, char *buf, int len, int timeOut)
{
	int nbytes = 0,iBroadcastKey = -1;
	int timeCount = 0;
	char recBufTemp[REV_BUF_SIZE] = { 0 };
	int nBytesTemp = 0;
	char *dataPointer = NULL;
	int textLen = 0;
/* zhengsenlin 
	{
		unsigned long ul = 1;
		if (-1 == ioctlsocket(s, FIONBIO, (unsigned long *)&ul))
		{
			evtc_printf( "RTSP_RecvSET_PARAMETER fcntl err!\n");
		}
	}
*/
	evtc_msleep(1);
	while (1)
	{
		evtc_printf( "before  recv s = %d!\n",s);
		nBytesTemp = recv(s, recBufTemp, 2048, 0);
		evtc_printf( "after  recv s = %d!nBytesTemp = %d\n",s,nBytesTemp);
		if (nBytesTemp <= 0)			//本次没接收到数据
		{
			if (nbytes <= 0)			//先前也没收到数据
			{
				//iBroadcastKey = TPUSIF_ReadKey(CURSOR_OFF, WAIT_FOR_1_SEC/10);
				evtc_msleep(100);
				
				if (timeCount == timeOut ||(iBroadcastKey != -1))
				{
					nbytes = -1;
					return nbytes;
				}
				timeCount++;
			}
			else						//先前有收到数据
			{
				evtc_printf( "!!!!!!!!!!!!!!TP_OW_RecFeiZushe data success nbytes = %d \n", nbytes);
				//printf("%s\n",buf);

				if (nbytes >= textLen)
				{
					// printf("!!!!!!!!!!!!!!TP_OW_RecFeiZushe data success nbytes = %d \nbuf = %s\n",nbytes,buf);  
					return nbytes;
				}
				else
				{
					//iBroadcastKey = TPUSIF_ReadKey(CURSOR_OFF, WAIT_FOR_1_SEC/10);
					evtc_msleep(100);

					timeCount++;
					if (timeCount == timeOut ||(iBroadcastKey != -1))
					{
						printf("注意数据接受不全nbytes = %d\n", nbytes);
						nbytes = -1;
						//TP_Print_Debug(ow_rtsp,ow_dos,"!!!!!!!!!!!!!!TP_OW_RecFeiZushe data err %d bytes at socket %d, and need rec %d bytes\n",nbytes,s,len);   
						return nbytes;
					}
				}

			}
		}
		else
		{
			memcpy((buf + nbytes), recBufTemp, nBytesTemp);
			if (nbytes == 0)			//说明第一次接收到数据,解析长度
			{
				if ((dataPointer = strstr(recBufTemp, "Content-Length")) == NULL)
				{
					evtc_printf( "get data!but can't find Content-Length \n");
				}
				else
				{
					evtc_printf("RTSP_RecNew textLen = %d\n", textLen);
				}
			}
			nbytes += nBytesTemp;
			//strcat(buf,recBufTemp);
			evtc_printf("!!!!!!!!!!!!!!!!!!!!!!!!!get data!nBytesTemp = %d\n", nBytesTemp);
			memset(recBufTemp, 0, 2048);
		}
	}
}
#endif
//long evtc_sock_recv( EVTC_SOCK *sock, char **buf, long len )
long evtc_sock_recv( EVTC_SOCK *sock, char **buf, long len )
{
	long ret = 0;
	if (!sock || sock->socket < 0 ||  len <= 0)
		return -1;
#if 0
#if 1	// 是否打开 厂家决定
	ret = evtc_sock_select(sock);
	if (ret < 0)
	{
		evtc_printf("select error.\n");
		//return ret;
	}
#else
	{
		unsigned long ul = 1;
		if (-1 == lwip_ioctl(sock->socket, FIONBIO, (unsigned long *)&ul))
		{
			evtc_printf("RTSP_RecvSET_PARAMETER fcntl err!\n");
		}
	}
#endif	 /*ESOCKET*/
	//MILLI_DELAY(10);

	ret = recv(sock->socket, revbuf, len, 0);
	
	if (ret < 0)
	{
		evtc_printf("receive fail %d\n", ret);
		ret = -1;
	}
	else
	{
		*buf = (char*)&revbuf[0];
		evtc_printf("receive success len = %d \n", ret);
	}

	return ret;
	#else	
	evtc_lock_socket();
	
	
	ret = fcntl(sock->socket, F_SETFL, O_NONBLOCK);
	if (ret < 0)
	{
		evtc_printf("RTSP_SOCKET_Receive fcntl err!\n");
		return -1;
	}
	
	if(!reved_data_len)/*第一次接收多等一点*/
	{
		ret = test_rec(sock->socket,revbuf[sock->socket],len,20);
	}
	else
	{
		ret = test_rec(sock->socket,revbuf[sock->socket],len,0);
	}
	if (ret < 0)
	{
		evtc_printf("receive fail %d\n", ret);
		ret = -1;
	}
	else
	{
		*buf = (char*)&(revbuf[sock->socket][0]);
		evtc_printf("receive success len = %d \n", ret);
		printf("receive success len = %d %s\n", ret,*buf);
	}
	reved_data_len += ret;
	evtc_unlock_socket();
	return ret;
	#endif
}

long evtc_sock_close( EVTC_SOCK *sock )
{
	int socket = 0,i,ret;
	

	evtc_printf("evtc_sock_close \n");
	
	if (!sock || sock->socket < 0)
	{	
		return -1;
	}
	evtc_lock_socket();
	memset(revbuf[sock->socket],0,REV_BUF_SIZE);
	socket = sock->socket;
	sock->socket = -1;
	reved_data_len = 0;
	ret = close(socket);
	evtc_unlock_socket();
	return ret;
}


unsigned long evtc_sock_getIpAddr( char *name)
{
	unsigned long ret = 0, i, tmp;
	char *name0;
	char port[6] = {0};
	
	if (!name || !name[0])
		return 0;
	
	name0 = name;
	for (i = 24; i >= 0; i -= 8)
	{
		while (*name0 && (*name0 < '0' || *name0 > '9'))
		{
			if (*name0 == '-' || (*name0 >= 'a' && *name0 <= 'z') || (*name0 >= 'A' && *name0 <= 'Z'))
				break;
			name0++;
		}
		if (!*name0)
			break;
		tmp = (unsigned long)atoi(name0);
		if (tmp < 0 || tmp > 255)
			break;
		ret |= tmp << i;
		if (!(name0 = strchr(name0, '.')))
			break;
	}
	if (!ret || !(i == 0 && !name0))
	{
		if (port && (name0 = strchr(name, ':')))
			*port = (unsigned short)atoi(name0 + 1);

#if defined(DNS_SUPPORT)
		{
			struct hostent *pHost;
			if (pHost = gethostbyname(name))
				return ((struct in_addr *)(pHost->h_addr_list[0]))->s_addr;
			else
				return 0;
		}
#endif	/*DNS_SUPPORT */
	}
	if (port && (name0 = strchr(name, ':')))
		*port = (unsigned short)atoi(name0 + 1);

	evtc_printf("sock_getIpAddr ret=%x\n", ret);

	return htonl(ret);
}

void evtc_msleep(unsigned long ms)
{
/*	FYF_API_thread_sleep(time); */ /* the same as the follow context*/
	FYF_API_thread_sleep(ms);
}

unsigned char *evtc_getMACAddress( void )
{
	static unsigned char hwmac[6]={0x00,0x60,0x6e,0x56,0xbf,0x83};
	int i,j;
	MAC_BOX_INFO_STRUCT pstMacBoxInfo;
	memset(&pstMacBoxInfo, 0, sizeof(MAC_BOX_INFO_STRUCT));
	APP_SYS_API_GetStbID(&pstMacBoxInfo);
	//printf("evtc_getMACAddress start\n");
	for (i = 0; i < 12; i++)
	{
		hwmacs[i] = pstMacBoxInfo.ucMacAddress[i];
	}
	//printf("mac = %s\n", hwmacs);
	//printf("evtc_getMACAddress end\n");
	return hwmacs;
#if 0		
	STBIDSetting_Init_STRUCT(0);/*有效性检查*/
	
	if(pstMacBoxInfo != NULL)
	{
		if ((pstMacBoxInfo->ucMacAddress[0] == 'f')&& 
			(pstMacBoxInfo->ucMacAddress[1] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[2] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[3] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[4] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[5] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[6] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[7] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[8] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[9] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[10] == 'f')&&
			(pstMacBoxInfo->ucMacAddress[11] == 'f'))
		{
		}
		else
	{
		for(i=0,j=0; i<6; i++,j++)
		{
			hwmac[i] = TP_MAC_Tran(pstMacBoxInfo->ucMacAddress[i+j],pstMacBoxInfo->ucMacAddress[i+j+1]);
		}
	}
	}
	else
	{
		hwmac[0]=0x0;
		hwmac[1]=0x60;

		hwmac[2]=SerialNo_InitMacAdress(3);
		hwmac[3]=SerialNo_InitMacAdress(2);
		hwmac[4]=SerialNo_InitMacAdress(1);
		hwmac[5]=SerialNo_InitMacAdress(0);
	}
	#else
	return hwmacs1;
	//return hwmacs2;
	evtc_printf("注意这里evtc_getMACAddress写死 \n");
#endif
   return hwmac;
}



void evtc_defaultTaskId(void) 
{
	int i;

	for(i=0;i<EN_TASKID_MAX;i++)
	{
		en_taskId[i].pStack = NULL;
		en_taskId[i].pTaskDesc = NULL;
		en_taskId[i].UsedTag = FALSE;
	}
}



/*---------------------------------------------------------------------------------*/
/*  func：线程主体函数入口地址，函数原型定义如下；                    */
/*    typedef VOID (*ENREACH_TASK_PROC)(VOID *param);                                                              */
/*---------------------------------------------------------------------------------*/
UINT32_T evtc_task_create(const CHAR_T *name, ENREACH_TASK_PROC func, VOID *param,
                                    INT32_T priority, UINT32_T stack_size)
{     	
	pthread_t  	thread = NULL;
	pthread_attr_t attr;
	BS32 ret;
	
	
	ENREACH_ERROR("[evtc_task_create]taskname=%s, func=%p, param=0x%x, prio=%d, stack=0x%x", name, func, param,priority, stack_size);

	if ((func == NULL) || (stack_size < 0))
	{
		ENREACH_ERROR("[evtc_task_create]param error!\n");
		return 0;  
	}
	//if (thread == NULL)
	{
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
		ret = pthread_create(&thread, &attr, (void *)func, NULL);
		pthread_attr_destroy (&attr);
		//thread_id = FYF_API_thread_create((const PU08)name,  func,  param,  priority,  stack_size);
		if (ret != 0)//(NULL  == thread_id)
		{
			ENREACH_ERROR("[ENREACH_ERROR]failed, taskname=%s, func=%p,  prio=%d, stack=0x%x", name, func,  priority, stack_size);
			return 0;
		
		}
	}
	//else
	{
		ENREACH_ERROR("thread = 0x%x\n", thread);
	}

	return thread;
}

void evtc_stop(void)
{
	ENREACH_ERROR("evtc_stop\n");
	envod_set_exit(true);
}

INT32_T evtc_task_destroy(BS32 thread_id)
{
	ENREACH_ERROR("[evtc_task_destory] handle = 0x%x\n", thread_id);
	
	if (pthread_cancel(thread_id) != 0)
	{	
		ENREACH_DEBUG("[evtc_task_destory] failed\n");
		return -1;
	}
	return 0;  	
}

void evtc_task_destroyself()
{
	ENREACH_ERROR("evtc_task_destroyself\n");
	envod_set_exit(true);
	pthread_exit(0);



}

/*
evtc_set_tuner is the same to the TRD_LockTuner(BU32 freq, BU32 sym, BU32 qam)
From ipanel_tuner.c
*/
BS32 evtc_set_tuner(BU32 freq, BU32 sym, BU32 qam)
{
	BS32 s32Ret = -1;
	BU32 j= 0;
	
	s32Ret = FYF_API_tuner_lock_delivery(0, freq, sym, qam, NULL);

	for(j = 0; j < 10; j++)
	{
		if((s32Ret = FYF_API_tuner_get_status(0)) == FYF_OK)
		{
			printf("Tuner Lock Success.\n");
			break;
		}
		printf(" wait to lock tuner j = %d\n", j);
		FYF_API_thread_sleep(100);
	}

	if (j == 10)
	{
		printf("unable to lock fre =%d, sym = %d, qam = %d.\n",freq, sym, qam);
		s32Ret = FYF_ERR;
	}

	return s32Ret;
}


void test_av(void)
{
	HI_U32 u32PressStatus, u32KeyId;
	int iBroadcastKey;
	
	printf( "test_av begin   \n");
	evtc_open_av(802, 6875,64,65, 66, 65); //change by zhengsenlin
	while(1)
	{
		iBroadcastKey = HI_UNF_IR_GetValue(&u32PressStatus, &u32KeyId);
		if(iBroadcastKey == -1)
		{
			continue;
		}

		switch (u32KeyId)		//快进的时候不能暂停，暂停的时候不能快进
		{
		case GUI_VK_EXIT:
			printf( "test_av end   \n");
			return;
		case GUI_VK_UP:
			evtc_open_av(602, 6875,64,1612, 1613, 8190);//cctv  //change by zhengsenlin
			break;
		case GUI_VK_DOWN:
			evtc_open_av(802, 6875,64,81, 82, 81);//ceshi  //change by zhengsenlin
			break;
		}
	}
}


int evtc_open_av(short fre_mhz,short sym, short qam,int video_pid, int audio_pid, int pcr_pid)
{
	int ret = 0;

	evtc_printf("evtc_open_av video_pid=%d,audio_pid=%d,pcr_pid=%d\n", video_pid, audio_pid, pcr_pid);


	if (video_pid == EN_INVALID_PID && audio_pid == EN_INVALID_PID)
		return 1;

	// ret = evtc_set_tuner(fre_mhz); /*change by zhengsenlin */
	ret = evtc_set_tuner(fre_mhz,sym,qam);

	if(ret != envod_success)
	{
		evtc_printf("evtc_set_tuner err\n");
		return envod_fail;
	}

#if 0   /* this function is from ST */
	TPMPEG_SetAudioOutputMode(CHANNEL_AUDIO_TRACK_LEFT, FALSE);
	if (TPSYSTEM_GetSystemVolLevel()<=MIN_VOL_LEVEL)
	{
		TPMPEG_AudioMute();
	}
	
	task_delay(ST_GetClocksPerSecond() / 100);

	TPMPEG_EnableAVSync(FALSE);
	TPMPEG_AVplay(video_pid, audio_pid /*DEMUX_INVALID_PID */ , /*pcr_pid */ DEMUX_INVALID_PID, FALSE, TRUE, 50);
	TPMPEG_EnableAVSync(TRUE);
#endif

	ENREACH_DEBUG("pcr_pid %d,video_pid %d, audio_pid %d\n", pcr_pid, video_pid, audio_pid);
	APP_IPI_PlayProg( video_pid,  audio_pid,  pcr_pid, FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
	ret = 0;
}

int evtc_parse_num2str(BU16 curYear, BU16 curMonth, BU16 curDay, BU16 hour, BU16 min, BU16 sec, char *yearStr, char *monthStr,
	char *dayStr, char *hourStr, char *minStr, char *secStr)
{
	if (yearStr == NULL)
	{
		
	}
	else if (curYear != 0)
	{
		sprintf(yearStr, "%d", curYear);
	}
	else
	{
		sprintf(yearStr, "%s", "0000");
	}

	if (monthStr == NULL)
	{
		
	}
	else if (curMonth != 0)
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

	if (dayStr == NULL)
	{
		
	}
	else if (curDay != 0)
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

	if (hourStr == NULL)
	{
		
	}
	else if (hour < 10)
	{
		sprintf(hourStr, "0%d", hour);
	}
	else
	{
		sprintf(hourStr, "%d", hour);
	}

	if (minStr == NULL)
	{
		
	}
	else if (min < 10)
	{
		sprintf(minStr, "0%d", min);
	}
	else
	{
		sprintf(minStr, "%d", min);
	}

	if (secStr == NULL)
	{
		
	}
	else if (sec < 10)
	{
		sprintf(secStr, "0%d", sec);
	}
	else
	{
		sprintf(secStr, "%d", sec);
	}
	return envod_success;
}

int evtc_add_time(BU16 * year, BU16 * month, BU16 * day, BU16 * hour, BU16 * min, BU16 * sec, BU16 perAdd)
{
	BU16 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	char bRunNian = false;
	BU16 days = 0;

	//evtc_printf("TP_OW_AddTime input year=%d,month =%d,day=%d,hour=%d,min=%d\n ", *year, *month,
	//	*day, *hour, *min);

	/*if (*sec + 2 < 60)
	{
		*sec += 2;
	}
	else
	{
		*sec = 0;
	}*/

	if(30 != *sec)
	{
		*sec = 30;
	}
	else
	{
		*sec = 0;
	}


	if (*min + perAdd < 60)				//分未溢出
	{
		*min += perAdd;
	}
	else								//向时进位
	{
		*min = *min + perAdd - 60;
		if (*hour + 1 < 24)				//时未溢出
		{
			*hour += 1;
		}
		else							//向天进位
		{
			*hour = 0;
			/******检查当前月份的总天数********/
			if (*month == 2)			//当前月为2月份
			{
				if (*year % 4 == 0)
				{
					if (*year % 100 != 0)
					{
						bRunNian = true;
					}
					else if (*year % 400 == 0)
					{
						bRunNian = true;
					}
				}
				if (bRunNian)			//闰年
				{
					days = 29;
				}
				else
				{
					days = 28;
				}
			}
			else
			{
				days = monthDays[*month - 1];
			}
			/***************************/
			if (*day + 1 <= days)		//日未溢出
			{
				*day += 1;
			}
			else						//向月进位
			{
				*day = 1;
				if (*month + 1 <= 12)	//月未溢出
				{
					*month += 1;
				}
				else					//向年进位
				{
					*month = 1;
					*year += 1;
				}
			}
		}
	}
	return envod_success;
}

int evtc_sub_time(BU16 * year, BU16 * month, BU16 * day, BU16 * hour, BU16 * min, BU16 * sec, BU16 perSub)
{
	BU16 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	char bRunNian = false;

	//evtc_printf( "TP_OW_SubTime input year=%d,month =%d,day=%d,hour=%d,min=%d\n ", *year, *month,
	//	*day, *hour, *min);
	/*if (*sec - 2 >= 0)
	{
		*sec -= 2;
	}
	else
	{
		*sec = 59;
	}*/

	if(30 != *sec)
	{
		*sec = 30;
	}
	else
	{
		*sec = 0;
	}

	if (*min - perSub >= 0)				//分钟够减
	{
		*min -= perSub;
	}
	else								//向时借位
	{
		*min = 60 + *min - perSub;
		if (*hour - 1 >= 0)				//时够减
		{
			*hour -= 1;
		}
		else							//向天借位
		{
			*hour = 23;
			if (*day - 1 > 0)			//天够减
			{
				*day -= 1;
			}
			else						//向月借位
			{
				/*****此时天数要算当前月的前一个月有几天,若前一个月为2月份,还要算年份看是不是闰年******/
				if (*month - 1 == 2)	//若果前个月为二月份
				{
					if (*year % 4 == 0)
					{
						if (*year % 100 != 0)
						{
							bRunNian = true;
						}
						else if (*year % 400 == 0)
						{
							bRunNian = true;
						}
					}
					if (bRunNian)		//闰年
					{
						*day = 29;
					}
					else
					{
						*day = 28;
					}
				}
				else
				{
					if (*month - 1 > 0)
					{
						*day = monthDays[*month - 2];
					}
					else				//12月份
					{
						*day = 31;
					}
				}
				/***************************/
				if (*month - 1 > 0)		//月份够减
				{
					*month -= 1;
				}
				else					//向年借位
				{
					*month = 12;

					if (*year - 1 > 0)
					{
						*year -= 1;
					}
					else				//12月份
					{
						*year = 0;
						*month = 0;
						*day = 0;
						*hour = 0;
						*min = 0;
					}
				}
			}
		}
	}
	evtc_printf("TP_OW_SubTime output year=%d,month =%d,day=%d,hour=%d,min=%d\n ", *year, *month,
		*day, *hour, *min);
	return envod_success;
}

int evtc_cau_all_secs(BU16 year, BU16 month, BU16 day, BU16 hour, BU16 min, BU16 sec, int *allSecs)	//计算从2000年开始到现在走过的秒数
{
	BU16 yearTemp = 0;
	BU16 runNianNum = 0;
	BU16 i = 2001;
	BU16 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	yearTemp = year % 100;

	if ((year == 0) || (month == 0) || (day == 0))
	{
		*allSecs = 0;
		return envod_fail;
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
	return envod_success;

}

int evtc_get_secondByClock(char *clock, int *allsecs, BU16 * year, BU16 * month, BU16 * day, char bYearSecs)
{
	char hour[3] = { 0 };
	char min[3] = { 0 };
	char sec[3] = { 0 };
	char days[3] = { 0 };
	char months[3] = { 0 };
	char years[5] = { 0 };

	years[0] = clock[0];
	years[1] = clock[1];
	years[2] = clock[2];
	years[3] = clock[3];
	*year = atoi(years);				//20080616164644

	months[0] = clock[4];
	months[1] = clock[5];
	*month = atoi(months);				//20080616164644

	days[0] = clock[6];
	days[1] = clock[7];
	*day = atoi(days);					//20080616164644


	hour[0] = clock[8];
	hour[1] = clock[9];
	min[0] = clock[10];
	min[1] = clock[11];
	sec[0] = clock[12];
	sec[1] = clock[13];


	if (bYearSecs)						//算到年份秒数
	{
		evtc_cau_all_secs(*year, *month, *day, (BU16) atoi(hour), (BU16) atoi(min), (BU16) atoi(sec), allsecs);
	}
	else
	{
		*allsecs = atoi(hour) * 3600 + atoi(min) * 60 + atoi(sec);
	}
	return envod_success;
}

int evtc_GetIPStrAndPortStr(char *progAdd, char *ipStr, char *portStr)
{

	char *ipTemp = NULL;
	char *ipTemp2 = NULL;
	int len = 0;

	if (NULL != (ipTemp = strstr(progAdd, "rtsp://")))
	{
		ipTemp += 7;
		if (NULL != (ipTemp2 = strstr(ipTemp, ":")))
		{
			len = (int)(ipTemp2 - ipTemp);
			memcpy(ipStr, ipTemp, len);
			ipTemp2 += 1;
			if (NULL != (ipTemp = strstr(ipTemp2, "/")))
			{
				len = (int)(ipTemp - ipTemp2);
				memcpy(portStr, ipTemp2, len);
				return envod_success;
			}

		}

	}

	evtc_printf( "RTSP_GetIPStrAndPortStr err!!!progAdd =  \n");
	return envod_fail;
}

int evtc_parseSecToHourMin(int orisec, int *hour, int *min, int *sec)
{
	*hour = orisec / 3600;
	orisec %= 3600;
	*min = orisec / 60;
	orisec %= 60;
	*sec = orisec;
}
int evtc_get_curtsindex()
{
	return cur_ts_index;
}
int evtc_check_channel(void)
{
	unsigned short serid = 0;
	int tsgSerid = 0;
	BU08 i;
	static signed short sProgIndex = -1;
	ChnDataBase_ChnInfo_S ChnInfo;

	//return envod_success;


	if (!b_get_tslist)
	{
		return envod_fail;
	}
	else
	{	

		if (APP_ChnDatabase_API_GetCurChnType() != APP_CHN_AV_TYPE_TV_e)
		{
			bTSVOD = false;
			return envod_fail;
		}
		if (sProgIndex != -1 && sProgIndex == APP_ChnDatabase_API_GetCurChnNO() && bTSVOD)
		{
			return envod_success;
		}
		sProgIndex = APP_ChnDatabase_API_GetCurChnNO();
		APP_ChnDatabase_API_GetChnInfo(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e, sProgIndex, &ChnInfo);
		serid = SeparateSevId(ChnInfo.m_ProgId);
		for (i = 0; i < tvod_num; i++)
		{
			tsgSerid = Timeshiftlist[i].serviceid;
			if (serid == (unsigned short) tsgSerid)
			{
				printf("当前节目为时移!!!!\n");
				/*evtc_drawReLogo();zhengsenlin*/
				//TP_OW_DrawReLogo();
				cur_ts_index = i;
				bTSVOD = true;
				return envod_success;
			}
		}
		bTSVOD = false;
		//TP_OW_ClearReLogo();
		/*evtc_clearReLogo();zhengsenlin*/
		return envod_fail;
	}
	bTSVOD = false;
	/*evtc_clearReLogo();zhengsenlin*/
	return envod_fail;
}



#define DEMUX_FUN
int evtc_getvidaid(EVTC_TS * RELAYPARA,int serid)
{
	int 			Result = envod_success;
	TPSIDB_DescListElem_t	VodTsList[50];
	int 					TimeoutMs = 1000;
	int 					DescNum = 0;
	int						i,j,k;
	int 					TimeOut;
	unsigned short OrgNetId;
	unsigned short TsId,ServiceId;
	int ElemNum1,ElemNum2;
	TPSIDB_ServiceListElem_t  ServiceList;
	TPSIDB_ServiceMapEs_t ServiceMapEs[20];
	TPSIDB_ServiceListElem_t SearchServiceList[MAX_NO_OF_PROGRAMS_IN_ONE_TS];
	int bTag = false;

printf("RELAYPARA->playmod =%d \n",RELAYPARA->playmod );
	RELAYPARA->playmod = SERVICEID_MOD;
	if(RELAYPARA->playmod == AID_VID_PCR_MOD)
	{
		return envod_success;
	}
	else if(RELAYPARA->playmod == SERVICEID_MOD)
	{
		if (evtc_set_tuner(RELAYPARA->freq,RELAYPARA->symbolrate,RELAYPARA->qam) == envod_fail)
		{
			return envod_fail;
		}
		evtc_msleep(200);
/*		Result = TPSIDB_GetServiceList(TPAPP_DECODER_NUMBER, MAX_NO_OF_PROGRAMS_IN_ONE_TS, 
			TimeOut, &OrgNetId, &TsId, &ElemNum1, SearchServiceList);zhengsenlin*/

		for(i = 0;i < ElemNum1;i++)
		{
	/*		Result = TPSIDB_GetServiceMapEs(TPAPP_DECODER_NUMBER, 20, TimeoutMs, SearchServiceList[i].ServiceId, &ElemNum2,	
				ServiceMapEs);	zhengsenlin*/				
			for(j = 0;j < ElemNum2;j++)
			{
				if(j == 0)
				{
					RELAYPARA->vpid = ServiceMapEs[j].Pid;
				}
				else if(j == 1)
				{
					RELAYPARA->apid = ServiceMapEs[j].Pid;
				}
				else if(j == 2)
				{
					RELAYPARA->pcrpid = ServiceMapEs[j].Pid;
				}

			}
		}

	}
	else if(RELAYPARA->playmod == PMTPID_MOD)
	{
		
	}

	return Result;
}


/*________________________________________________________________________*/
/*________________________________________________________________________*/
/*________________________________________________________________________*/
/*________________________________________________________________________*/

#if 0  //zhengsenlin
int evtc_default_bmp(void)
{
	int i = 0;

	for (i = 0; i < EN_OSD_PIC_NUM; i++)
	{
		enosd_bmp[i].BMPDataPointer = NULL;
		enosd_bmp[i].width = 0;
		enosd_bmp[i].height = 0;
	}
	enosd_bmp[EN_BPZhun_EM].GIFDataPointer = EN_BPZhun;
	enosd_bmp[EN_InBP_EM].GIFDataPointer = EN_InBP;
	enosd_bmp[EN_Jump_EM].GIFDataPointer = EN_Jump;
	enosd_bmp[EN_JumpTo_EM].GIFDataPointer = EN_JumpTo;
	enosd_bmp[EN_seek_EM].GIFDataPointer = EN_seek;
	enosd_bmp[EN_BPJump_EM].GIFDataPointer = EN_BPJump;
	enosd_bmp[EN_ReLogo_EM].GIFDataPointer = EN_ReLogo;
	enosd_bmp[EN_PP_EM].GIFDataPointer = EN_PP;
}

int evtc_draw_gif(U16 x, U16 y, U8 picNum, U16 * wid, U16 * hig)
{
	int ret = 0;
	TPGXOBJ_Bitmap_t SrcBitmap;
	BMP_ATTRIBUTE BmpBack ;

#if 1
	BmpBack.Data_p = (U8 *)TPDATABUFF_GetSharedMemory(TIMEOUT_INFINITY, 1);

   ret = TPGRAPH_GIF2BMP(enosd_bmp[picNum].GIFDataPointer, &BmpBack, 1);

   SrcBitmap.ColorType = STGXOBJ_COLOR_TYPE_ARGB1555;
   SrcBitmap.Data_p = BmpBack.Data_p;
   SrcBitmap.Width = BmpBack.width;
   SrcBitmap.Height = BmpBack.height; 
	enosd_bmp[picNum].width = BmpBack.width;
	enosd_bmp[picNum].height = BmpBack.height;
	enosd_bmp[picNum].BMPDataPointer = BmpBack.Data_p;
#else
	SrcBitmap.ColorType = STGXOBJ_COLOR_TYPE_ARGB1555;
	SrcBitmap.Width = enosd_bmp[picNum].width;
	SrcBitmap.Height = enosd_bmp[picNum].height;
	SrcBitmap.Data_p = enosd_bmp[picNum].BMPDataPointer;
#endif
	if (EN_PP_EM == picNum)
	{
		SrcBitmap.Height -= 7;
	}

	//printf("gif to bmp width = %d,height = %d \n", SrcBitmap.Width, SrcBitmap.Height);

	TPGRAPH_DrawBMP(x, y, SrcBitmap);
	if (wid != NULL)
	{
		*wid = SrcBitmap.Width;
	}
	if (hig != NULL)
	{
		*hig = SrcBitmap.Height;
	}
	return envod_success;
}

int evtc_clear_num(int x, int y, int with, int hight)
{
	/*U16 width = 0,hight = 0;

	   switch(with)
	   {
	   case 200:
	   TP_OW_DrawGIF2BMP(x, y,ow_kong200,&width,&hight);
	   break;
	   case 100:
	   TP_OW_DrawGIF2BMP(x, y,ow_kong100,&width,&hight);
	   break;
	   case 80:
	   TP_OW_DrawGIF2BMP(x, y,ow_kong80,&width,&hight);
	   break;
	   default:
	   break;
	   } */
	TPGRAPH_DrawRectangle(x, y, with, hight, EN_SEEK_COLOR);
	return envod_success;
}

int evtc_draw_bar(char * host,EVTC_TS  *RELAYPARA )		//画时间条
{
	U8 perPassBar = 4;					//每次偏移的像数
	U16 x = 50;
	U16 y = 400;
	U16 seekX = x + 38;
	U16 seekY = y + 40;
	U16 barLen = 400;					//进度条总像素
	U8 barHeight = 14;
	U8 perPassTime = 3;					//每次偏移的时间300秒
	int oneBarAllSec = 18000;			//整条时间5小时
	int startSecs = 0;
	int endSecs = 0;
	int curSecs = 0;					//
	U16 startYear = 0;
	U16 endYear = 0;
	U16 curYear = 0;
	U8 startMonth = 0;
	U8 endMonth = 0;
	U8 curMonth = 0;
	U8 startDay = 0;
	U8 endDay = 0;
	U8 curDay = 0;
	U16 yelBarLen = 0;
	U16 preYelBarLen = 0;
	int iBroadcastKey = -1;
	BOOLEAN bExit = false;
	int optionTimeCount = 0;
	BOOLEAN stopTag = false;
	U8 hour = 0;
	U8 min = 0;
	U8 sec = 0;
	char adjTimeStr[30] = { 0 };
	BOOLEAN bLarger = false;
	BOOLEAN bSmaller = false;			//时间超出范围
	char yearStr[5] = { 0 };
	char monthStr[3] = { 0 };
	char dayStr[3] = { 0 };
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };
	U16 width = 0;
	U16 hight = 0;
	clock_t bTime = 0;
	clock_t pTime = 0;
	char curtime[TIMELEN] = {0};

#if 1
	if((RELAYPARA->begintime == 0)||(RELAYPARA->endtime == 0)||(RELAYPARA->curtime == 0))
	{
		evtc_draw_gif(x, y, EN_BPZhun_EM, &width, &hight);
		TPGRAPH_DrawTextInRegion(200, 220, width, 25, "服务器无播放参数", JCENTER, BLACKCOLOR);
		MILLI_DELAY(1000);
		TPGRAPH_DrawRectangle(200, 220, 300, 60, TRANSPARENTCOLOR);
		return envod_fail;
	}
	evtc_get_secondByClock(RELAYPARA->begintime, &startSecs, &startYear, &startMonth, &startDay, true);
	evtc_get_secondByClock(RELAYPARA->endtime, &endSecs, &endYear, &endMonth, &endDay, true);
	evtc_get_secondByClock(RELAYPARA->curtime, &curSecs, &curYear, &curMonth, &curDay, false);

       if(evtc_getcurtime(host, 0, RELAYPARA->curtime) == envod_fail)
       {
		evtc_printf( "evtc_draw_bar  evtc_getcurtime err!!!  \n");
       }
	   else
	   {
	   	printf("evtc_draw_bar evtc_getcurtime = %s begin = %s,end = %s \n",RELAYPARA->curtime,RELAYPARA->begintime,RELAYPARA->endtime);
	   }
	hour = curSecs / 3600;
	curSecs %= 3600;
	min = curSecs / 60;
	curSecs %= 60;
	sec = (U8) curSecs;

	evtc_parse_num2str(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
		secStr);
	sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);

	evtc_draw_gif(x, y, EN_Jump_EM, &width, &hight);

	preYelBarLen = yelBarLen;
	TPGRAPH_DrawRectangle(x + 48, y + 66, barLen, barHeight, BLUECOLOR);
	TPGRAPH_DrawRectangle(x + 48, y + 66, yelBarLen, barHeight, YELLOWCOLOR);
	TPGRAPH_DrawTextInRegion(x + 150, y + 10, 220, 25, adjTimeStr, EN_JLEFT, BLACKCOLOR);	//用这句坐标不好对
	evtc_get_secondByClock(RELAYPARA->curtime, &curSecs, &curYear, &curMonth, &curDay, true);

	if (curSecs - startSecs < (oneBarAllSec / 2))	//说明靠近起始点
	{
		yelBarLen = (curSecs - startSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
	}
	else if (endSecs - curSecs < (oneBarAllSec / 2))	//说明靠近结束点
	{
		yelBarLen = barLen - (endSecs - curSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
	}
	else								//否则都取中间点
	{
		yelBarLen = barLen / 2;
	}
	TPGRAPH_DrawRectangle(x + 48, y + 66, barLen, barHeight, BLUECOLOR);
	TPGRAPH_DrawRectangle(x + 48, y + 66, yelBarLen, barHeight, YELLOWCOLOR);
	TPGRAPH_DrawTextInRegion(x + 150, y + 10, 220, 25, adjTimeStr, EN_JLEFT, BLACKCOLOR);	//用这句坐标不好对
	evtc_draw_gif(seekX + yelBarLen, seekY, EN_seek_EM, NULL, NULL);
	
#endif

#if 0
	evtc_getcurtime( host, 0, curtime);
#else
     sprintf(curtime,"%s",RELAYPARA->curtime);
#endif
	evtc_get_secondByClock(RELAYPARA->begintime, &startSecs, &startYear, &startMonth, &startDay, true);
	evtc_get_secondByClock(RELAYPARA->endtime, &endSecs, &endYear, &endMonth, &endDay, true);
	evtc_get_secondByClock(curtime, &curSecs, &curYear, &curMonth, &curDay, false);

	hour = curSecs / 3600;
	curSecs %= 3600;
	min = curSecs / 60;
	curSecs %= 60;
	sec = (U8) curSecs;

	evtc_parse_num2str(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
		secStr);
	sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);

	evtc_get_secondByClock(curtime, &curSecs, &curYear, &curMonth, &curDay, true);


	if (curSecs - startSecs < (oneBarAllSec / 2))	//说明靠近起始点
	{
		yelBarLen = (curSecs - startSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
	}
	else if (endSecs - curSecs < (oneBarAllSec / 2))	//说明靠近结束点
	{
		yelBarLen = barLen - (endSecs - curSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
	}
	else								//否则都取中间点
	{
		yelBarLen = barLen / 2;
	}

	evtc_draw_gif(x, y, EN_Jump_EM, &width, &hight);
	evtc_draw_gif(seekX + yelBarLen, seekY, EN_seek_EM, NULL, NULL);

	preYelBarLen = yelBarLen;
	TPGRAPH_DrawRectangle(x + 48, y + 66, barLen, barHeight, BLUECOLOR);
	TPGRAPH_DrawRectangle(x + 48, y + 66, yelBarLen, barHeight, YELLOWCOLOR);
	TPGRAPH_DrawTextInRegion(x + 150, y + 10, 220, 25, adjTimeStr, EN_JLEFT, BLACKCOLOR);	//用这句坐标不好对
	bTime = time_now();

	while (!bExit)
	{
		iBroadcastKey = TPUSIF_ReadKey(CURSOR_OFF, WAIT_FOR_1_SEC);

		if (iBroadcastKey == -1)
		{
			optionTimeCount++;
			if (optionTimeCount == 10)
			{
				iBroadcastKey = EXIT_KEY;
				//break;
			}
			else
			{
				continue;
			}
		}
		optionTimeCount = 0;

		switch (iBroadcastKey)
		{
		case SELECT_KEY:
			bExit = true;
			break;
		case EXIT_KEY:
			bExit = true;
			break;
		case LEFT_KEY:
		case PAGE_DOWN_KEY:

			evtc_sub_time(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
			evtc_cau_all_secs(curYear, curMonth, curDay, hour, min, sec, &curSecs);
			if ((curSecs < startSecs) || (curSecs == 0))
			{
				bSmaller = true;
				bLarger = false;
			}
			else
			{
				bSmaller = false;
				bLarger = false;
			}

			if (!bSmaller)
			{
				if (yelBarLen - perPassBar >= 0)
				{
					yelBarLen -= perPassBar;

				}
				else
				{
					yelBarLen = barLen;
				}
			}
			else
			{
				evtc_add_time(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
			}
			break;
		case RIGHT_KEY:
		case PAGE_UP_KEY:
			evtc_add_time(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
			evtc_cau_all_secs(curYear, curMonth, curDay, hour, min, sec, &curSecs);
			if ((curSecs > endSecs) || (curSecs == 0))
			{
				bLarger = true;
				bSmaller = false;
			}
			else
			{
				bLarger = false;
				bSmaller = false;
			}
			if (!bLarger)
			{
				if (yelBarLen + perPassBar <= barLen)
				{
					yelBarLen += perPassBar;
				}
				else
				{
					yelBarLen = 0;
				}

			}
			else
			{
				evtc_sub_time(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
			}

			break;
		default:
			break;
		}

		if (!bLarger && !bSmaller)
		{
			TPGRAPH_DrawRectangle(seekX + preYelBarLen, seekY, 20, 24, EN_SEEK_COLOR);
			evtc_draw_gif(seekX + yelBarLen, seekY, EN_seek_EM, NULL, NULL);
			preYelBarLen = yelBarLen;
			TPGRAPH_DrawRectangle(x + 48, y + 66, barLen, barHeight, BLUECOLOR);
			TPGRAPH_DrawRectangle(x + 48, y + 66, yelBarLen, barHeight, YELLOWCOLOR);
			evtc_parse_num2str(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr,
				minStr, secStr);
			sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
			evtc_clear_num(x + 150, y + 10, 200, 25);
			TPGRAPH_DrawTextInRegion(x + 150, y + 10, 220, 25, adjTimeStr, EN_JLEFT, BLACKCOLOR);	//用这句坐标不好对
		}

	}

	if (SELECT_KEY == iBroadcastKey)
	{

		evtc_parse_num2str(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
			secStr);


		TPGRAPH_DrawRectangle(x, y, width, hight, TRANSPARENTCOLOR);

		if ((curSecs < startSecs) || (curSecs == 0))
		{
			evtc_printf("in TP_OW_DrawTVODBarJump too long time!! \n");
			sprintf(curtime, "%s", RELAYPARA->begintime);
		}
		else
		{
			sprintf(curtime, "%s%s%s%s%s%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
		}

		evtc_settime( host, curtime, 0);
		return envod_success;
	}
	TPGRAPH_DrawRectangle(x, y, width, hight, TRANSPARENTCOLOR);
	return envod_fail;
}


int evtc_check_channel(void)
{
	USHORT serid = 0;
	int tsgSerid = 0;
	U8 i;
	SHORT sProgIndex = -1;


	if (!b_get_tslist)
	{
		return envod_fail;
	}
	else
	{
		sProgIndex = TPDBASE_GetProgIndex(iChannelCurListingNo);
		serid = TPDBASE_GetProgBaseProgServiceIndex(sProgIndex);
		for (i = 0; i < tvod_num; i++)
		{
			tsgSerid = Timeshiftlist[i].serviceid;
			if (serid == (USHORT) tsgSerid)
			{
				printf("当前节目为时移!!!!\n");
				evtc_drawReLogo();
				//TP_OW_DrawReLogo();
				cur_ts_index = i;
				bTSVOD = true;
				return envod_success;
			}
		}
		bTSVOD = false;
		//TP_OW_ClearReLogo();
		evtc_clearReLogo();
		return envod_fail;
	}
	bTSVOD = false;
	evtc_clearReLogo();
	return envod_fail;
}
#define DRAW_FUN


int evtc_drawBPZhun()
{
	U16 x = 210, y = 200;
	int second = 5;
	char secStr[3] = { 0 };
	int timeCount = 0;
	int readKey = -1;
	U16 width = 0, hight = 0;

	evtc_draw_gif(x, y,EN_BPZhun_EM, &width, &hight);

	TPGRAPH_DrawTextInRegion(x + 15, y + 15, 300, 25, "回放准备    秒 按任意键退出", JLEFT, iBlackColor);

	while (1)
	{
		TPGRAPH_DrawRectangle(x + 95, y + 20, 10, 20, EN_SEEK_COLOR);

		itoa(second, secStr, 10);

		TPGRAPH_DrawTextInRegion(x + 95, y + 20, 10, 20, secStr, JLEFT, iBlackColor);
		if (second == 0)
		{
			TPGRAPH_DrawRectangle(x, y, width, hight, ibackgroundColor);
			return envod_success;
		}
		readKey = TPUSIF_ReadKey(CURSOR_OFF, WAIT_FOR_1_SEC);
		if (readKey != -1)
		{
			TPGRAPH_DrawRectangle(x, y, width, hight, ibackgroundColor);

			return envod_fail;
		}
		else
		{
			second--;
		}
	}

}

int evtc_draw_pp(char *curtime)
{
	int x = 350;
	int y = 50;
	int yOff = 25;
	int curTime = 0;
	int hour;
	int min;
	int sec;
	char hourStr[5] = { 0 };
	char minStr[5] = { 0 };
	char secStr[5] = { 0 };
	char timeStr[10] = { 0 };
	U16 year;
	U8 month;
	U8 day;
	U16 width = 200;
	U16 hight = 100;

	printf("TP_OW_DrawPP  ------------!!\n ");

	TPGRAPH_DrawRectangle(x, y, width, hight, TRANSPARENTCOLOR);

	if (vodortvod == TS_VOD )
	{
		yOff = 15;
		width += 40;
	}

	evtc_draw_gif(x, y, EN_PP_EM, &width, &hight);
	if (vodortvod == TS_VOD)
	{
		TPGRAPH_DrawTextInRegion(x, y + yOff, width, 25, "回看暂停", JCENTER, BLACKCOLOR);
		/*evtc_get_secondByClock(curtime, &curTime, &year, &month, &day, false);
		evtc_parseSecToHourMin(curTime, &hour, &min, &sec);
		evtc_parse_num2str(0, 0, 0, hour, min, sec, 0, 0, 0, hourStr, minStr, secStr);
		sprintf(timeStr, "%s:%s:%s", hourStr, minStr, secStr);
		TPGRAPH_DrawTextInRegion(x + 30, y + yOff + 50, width, 25, "播放时间:", JLEFT, BLACKCOLOR);
		TPGRAPH_DrawTextInRegion(x + 120, y + yOff + 53, width, 25, timeStr, JLEFT, BLACKCOLOR);*/
	}
	else
	{
		TPGRAPH_DrawTextInRegion(x, y + yOff, width, 25, "暂停播放", JCENTER, BLACKCOLOR);
	}
	TPGRAPH_DrawTextInRegion(x, y + yOff + 25, width, 25, "按[确认]键继续", JCENTER, BLACKCOLOR);

	return envod_success;
}

int evtc_clear_pp(void)
{
	int x = 350, y = 50;
	int w = 250, h = 120;

	TPGRAPH_DrawRectangle(x, y, w, h, TRANSPARENTCOLOR);
	return envod_success;
}

void evtc_draw_speed(int speednum,char * curtime)
{
	char speedstring[4] = { 0 };
	char drawString[10] = { 0 };
	int speednumToStringLen = 0;
	BOOLEAN fu_tag = false;
	int x = 500, y = 120;

	TPGXOBJ_Bitmap_t bitmap;
	if (speednum == 0)
	{
		evtc_draw_pp(curtime);
		return;
	}
	else if (speednum == 1)
	{
		evtc_clear_pp();
		return;
	}
	evtc_clear_pp();
	bitmap.ColorType = STGXOBJ_COLOR_TYPE_ARGB1555;

	evtc_printf("============stb_draw_speed speednum =%d\n", speednum);

	bitmap.Width = 30;
	bitmap.Height = 25;
	bitmap.Data_p = (void *)EN_Data_Bitmap_Speed;

	//iPanel_graphics_dprintf("dstX:%d  dstY:%d   srcW:%d   srcH:%d\n", dstX ,dstY ,srcW,srcH);

	TPGRAPH_DrawRectangle(x, y, 250, 150, ibackgroundColor);
	TPGRAPH_DrawBMP(x, y, bitmap);
	TPGRAPH_DrawBMP(x + 30, y, bitmap);
	TPGRAPH_DrawBMP(x + 60, y, bitmap);
	//TPGRAPH_DrawTextInRegion(30, 525, 120, 25, "电视回看", JCENTER, iRedColor);

	if (speednum < 0)
	{
		fu_tag = true;
		speednum *= (-1);
	}

	itoa((int)speednum, (char *)speedstring, 10);

	printf("stb_draw_speed speedstring =%s,speednum = %d\n", speedstring, speednum);

	if (fu_tag)
	{
		sprintf(drawString, "快退-%s", speedstring);
		TPGRAPH_DrawTextInRegion(x - 7, y, 100, 23, drawString, JCENTER, iBlackColor);
	}
	else
	{
		sprintf(drawString, "快进x%s", speedstring);
		TPGRAPH_DrawTextInRegion(x - 7, y, 100, 23, drawString, JCENTER, iBlackColor);
	}
}

int evtc_draw_connect(void)
{

	U16 x = 200, y = 220;
	U16 width = 300, hight = 60;

	//TPGRAPH_DrawRectangle(x, y,width, hight, BLUECOLOR);
	evtc_draw_gif(x, y, EN_BPZhun_EM, &width, &hight);
	TPGRAPH_DrawTextInRegion(x, y + 20, width, 25, "正在连接,请稍后...", JCENTER, BLACKCOLOR);

	return envod_success;
}

int evtc_clear_connect(void)
{

	U16 x = 200, y = 220;

	TPGRAPH_DrawRectangle(x, y, 300, 60, TRANSPARENTCOLOR);
	return envod_success;
}


int evtc_draw_exit(void)
{
	int x = 240, y = 200;
	int readKey = -1;
	U16 width = 0, hight = 0;
	U8 optionTimeCount = 0;
	BOOLEAN stopTag = false;

	//TP_OW_DrawJPEG(x, y,PP,PP_size,&width,&hight);
	evtc_draw_gif(x, y, EN_PP_EM, &width, &hight);

	if(vodortvod == VOD)
	{
		TPGRAPH_DrawTextInRegion(x + 10, y + 20, 220, 25, "是否退出点播节目?", JCENTER, iBlackColor);
		TPGRAPH_DrawTextInRegion(x + 10, y + 45, 220, 25, "[确认] 退出点播", JCENTER, iBlackColor);
		TPGRAPH_DrawTextInRegion(x + 10, y + 70, 220, 25, "[退出] 返回点播", JCENTER, iBlackColor);
	}
	else
	{
		TPGRAPH_DrawTextInRegion(x + 10, y + 20, 220, 25, "是否退出回看状态?", JCENTER, iBlackColor);
		TPGRAPH_DrawTextInRegion(x + 10, y + 45, 220, 25, "[确认] 退出回看", JCENTER, iBlackColor);
		TPGRAPH_DrawTextInRegion(x + 10, y + 70, 220, 25, "[退出] 返回回看", JCENTER, iBlackColor);
	}

	while (1)
	{
		readKey = TPUSIF_ReadKey(CURSOR_OFF, WAIT_FOR_1_SEC);
		if (readKey == -1)
		{
			continue;
		}
		switch (readKey)
		{
		case SELECT_KEY:
			TPGRAPH_DrawRectangle(x, y, width, hight, ibackgroundColor);
			return envod_success;
			break;
		case EXIT_KEY:
			TPGRAPH_DrawRectangle(x, y, width, hight, ibackgroundColor);
			return envod_fail;
			break;
		default:
			break;
		}

	}
}
int evtc_drawReLogo(void)
{
	U16 x = 20, y = 500;
	U16 width = 0, hight = 0;

	evtc_draw_gif(x, y, EN_ReLogo_EM, &width, &hight);
	return envod_success;
}

int evtc_clearReLogo(void)
{
	U16 x = 20, y = 500;
	U16 width = 60, hight = 60;

	TPGRAPH_DrawRectangle(x, y, width, hight, ibackgroundColor);
	return envod_success;
}


#endif

/*________________________________________________________________________*/
/*________________________________________________________________________*/
/*________________________________________________________________________*/
/*________________________________________________________________________*/

