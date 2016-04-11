/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment of the Network Porting APIs needed by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/
#include "ipanel_porting_event.h"
#include "ipanel_socket.h"
#include "ipanel_base.h"
#include "trd_ipanel_init.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"

#define SOCKET   int
#define closesocket  close

#define  MAX_SOCKETS  6
#define  MAX_EVENTS   128
#define  SOCKET_BUFF_SIZE  4096

typedef struct tagSystemSocketManager {
	int n_idle;
	int sockets[MAX_SOCKETS];
	int maxfds;
	fd_set fds_r, fds_w, fds_e;
	int event_indx_read;
	int event_indx_write;
	int event_socket[MAX_EVENTS];
	int event_type[MAX_EVENTS];
} SystemSocketManager;
static SystemSocketManager *sock_mgr=NULL;
static SystemSocketManager my_manger;
BU08 g_fromIpanel = FYF_FALSE;

#ifdef USE_FZVOD
#include "../ow_vod/ow_debug.h"
int 	recStarTime = 0;
int 	recLongTime = 0;
char g_progAdd[1024] = {0};
char g_StartClock[25] = {0};
//BU08 ipanel_done = FYF_FALSE;
//BU08 g_fromIpanelvod = FYF_FALSE;
extern BU08 bExitSocket;
extern 	BS32 s_pIpanelMsgQueue;
extern 	BU08 bPlayTVOD;
extern  int OW_VOD_MainMenu(char * progAdd);
extern  int RTSP_GetFiledNumStr(char * DataPointer,char * FiledName,char * DataBuf);
extern  void OW_SetRecordFlag(BU08 flag);
BU08 bStopwhitescreen = FYF_FALSE;
extern OW_PLAY_PARA VodPlayPara;
extern 	HI_U32 w,r;
extern HI_U32 num;
#endif

BU08  bDrawAgain = FYF_FALSE;

BU08  socket_buf[720*576*2] = {0};

static SystemSocketManager *SystemSocketManager_new(void);
static void SystemSocketManager_delete(SystemSocketManager *me);
static int SystemSocketManager_register(SystemSocketManager *me, int s);
static int SystemSocketManager_unregister(SystemSocketManager *me, int s);
static void SystemSocketManager_put_event(SystemSocketManager *me, int s, int evt);
static int SystemSocketManager_pop_event(SystemSocketManager *me, int *s, int *event);
static int SystemSocketManager_poll(SystemSocketManager *me);

/******************************************************************/
/*Description: Get max socket count.                              */
/*Input      : No                                                 */
/*Output     : No                                                 */
/*Return     : max socket count.                                  */
/******************************************************************/
int ipanel_porting_get_max_sockets(void)
{
	//IPANEL_ERROR("ipanel_porting_get_max_sockets\n");
	return MAX_SOCKETS;
}

/******************************************************************/
/*Description: Open a Socket,It'll send(SOCKET_EVENT_TYPE_CONNECT)*/
/*             to iPanel MiddleWare if Connect Successful.        */
/*Input      : IP value, Port, and Type(TCP(0) or UDP(1))         */
/*Output     : No                                                 */
/*Return     : socket value or socket ID.(< 0, is Fail.)          */
/******************************************************************/
int ipanel_porting_socket_open(int ip, int port, int type)
{
	SOCKET m_s;
	struct sockaddr_in serv_addr;
	int status, i;
	
	struct timeval tv; 
	fd_set writefds; 
	int error;
	int tmp = EINPROGRESS;
	int seret = -1;

	int ret = 0;

#if defined(USE_FZVOD) || defined(USE_ENREACH)
	ret = FYF_API_GetLinkStatus();
	if (ret != 1)
	{
		return -3;
	}

	
#endif

	IPANEL_ERROR("ipanel_porting_socket_open\n");
	if (sock_mgr->n_idle<=0) {
		IPANEL_ERROR("Socket mgr full\n");
		m_s = -1;
		goto ret1;
	}

	




	m_s = socket(AF_INET, (type == 0 ? SOCK_STREAM:SOCK_DGRAM), 0);
	if (m_s<0) {
		IPANEL_ERROR("CreateSocket Failed\n");
		m_s = -2;
		goto ret1;
	}    

	//printf("m_s = %d\n", m_s);
	//bzero(&serv_addr,sizeof(serv_addr));//66582
	serv_addr.sin_family = AF_INET;
	
	serv_addr.sin_addr.s_addr = ntohl(ip);
	serv_addr.sin_port = htons(port);
	//IPANEL_DEBUG("create socket successfully m_s is %d\n",m_s);
	
	/*ret = RTSP_SOCKET_SetNoneBlock(m_s);
	if (ret < 0)
	{
		IPANEL_ERROR("RTSP_SOCKET_SetNoneBlock error\n");
		return FYF_ERR;
	}*/

	ret = fcntl(m_s, F_SETFL, O_NONBLOCK);
	if (ret < 0)
	{
		IPANEL_ERROR("RTSP_SOCKET_Receive fcntl err!\n");
		return FYF_ERR;
	}

	for(i=0;i<2;i++){
		status = connect(m_s, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
		if(status == 0)
			break;
		else 
		{	
			//IPANEL_DEBUG("connecting...\n");

			/*if (OW_VOD_GetNetErrFlag() == FYF_TRUE)
			{
				closesocket(m_s);
				return -3;
			}*/

			tv.tv_sec = 2; 
			tv.tv_usec = 0; 
			FD_ZERO(&writefds); 
			FD_SET(m_s, &writefds); 
			if ((seret = select(m_s + 1,NULL, &writefds, NULL, &tv)) > 0)
			{
				int len = sizeof(int); 
				getsockopt(m_s, SOL_SOCKET, SO_ERROR, &error, &len); 
				//printf("error = %d, seret = %d\n", error, seret);
				
				//perror("");
			}	
		}


	}
	//IPANEL_ERROR("connect end.\n");
	if (status != 0) {
		closesocket(m_s);
		IPANEL_ERROR("Socket Connect Error\n");
		return -3;
	}
    
	SystemSocketManager_register(sock_mgr, m_s);          
	SystemSocketManager_put_event(sock_mgr, m_s, SOCKET_EVENT_TYPE_CONNECT);
ret1:
	return m_s;
}

/******************************************************************/
/*Description: Send data throught socket.                         */
/*Input      : Socket, Data, and Data Length                      */
/*Output     : No                                                 */
/*Return     : Send real data Length                              */
/******************************************************************/
int ipanel_porting_socket_send(int s, const char *buf, int len)
{
	int nbytes;
//	IPANEL_ERROR("ipanel_porting_socket_send\n");
	nbytes = send(s, buf, len, 0);
//	IPANEL_ERROR("\n[eis_socket_send]Send data %d bytes at socket %d, and need send %d bytes\n",nbytes,s,len);   
	return nbytes;
}

/******************************************************************/
/*Description: Receive data from socket.                          */
/*Input      : Socket, Buffer, and Buffer Length                  */
/*Output     : Data to buffer                                     */
/*Return     : Receive real data Length                           */
/******************************************************************/
int ipanel_porting_socket_recv(int s, char *buf, int len)
{
	int nbytes;
//	IPANEL_ERROR("ipanel_porting_socket_recv\n");
	nbytes = recv(s, buf, len, 0);    
//	IPANEL_ERROR("\n[eis_socket_recv]Got %d bytes from socket %d.\n",nbytes,s);
	if (nbytes <= 0) {
		SystemSocketManager_put_event(sock_mgr, s, SOCKET_EVENT_TYPE_CLOSE);    
	}
	return nbytes;
}

/******************************************************************/
/*Description: Close a Socket                                     */
/*Input      : Socket                                             */
/*Output     : No                                                 */
/*Return     : 0 -- success, other fail.                          */
/******************************************************************/
int ipanel_porting_socket_close(int s)
{
	//IPANEL_DEBUG("\n[eis_socket_close]close socket %d.\n",s);
	//IPANEL_DEBUG("ipanel_porting_socket_close\n");
	SystemSocketManager_unregister(sock_mgr, s);
	return closesocket(s);
}

/******************************************************************/
/*Description: Poll Socket Event, and Send Socket Message to BRWS.*/
/*    The following is a description of Browser and Socket Module:*/
/*      Browser Module                   Socket Module            */
/*    1) Http Request Call              Poll or Select all Socket.*/
/*       "eis_socket_open()";                                     */
/*    2)                                When Connected success,   */
/*                                      Send READ message to BRWS;*/
/*    3) When Receive READ Message,                               */
/*       Call "eis_socket_send()" to send data;                   */
/*    4)                                When have data on socket, */
/*                                     Send WRITE message to BRWS;*/
/*    5) When Receive WRITE Message,                              */
/*       Call "eis_socket_recv()" to receive data;                */
/*    6)                         When no data or error on socket, */
/*                                     Send WRITE message to BRWS;*/
/*    7) When Receive CLOSE Message,                              */
/*       Call "eis_socket_close()" to close a close.              */
/*Input      : Event Array[3]                                     */
/*Output     : event value                                        */
/* 1) event[0] is iPanel Event Message Type, as following value.  */
/* enum {                                                         */
/*     EIS_EVENT_TYPE_NETWORK = 4                                 */
/* };                                                             */
/* 2) event[1] is the socket value or socket ID.                  */
/* 3) event[2] is the socket status, as following enum value.     */
/* enum {                                                         */
/*     SOCKET_EVENT_TYPE_CONNECT = 1,                             */
/*     SOCKET_EVENT_TYPE_CLOSE,                                   */
/*     SOCKET_EVENT_TYPE_READ,                                    */
/*     SOCKET_EVENT_TYPE_WRITE,                                   */
/*     SOCKET_EVENT_TYPE_ACCEPT,                                  */
/*     SOCKET_EVENT_TYPE_READ_SHUTDOWN,                           */
/*     SOCKET_EVENT_TYPE_WRITE_SHUTDOWN,                          */
/*     SOCKET_EVENT_TYPE_ERROR                                    */
/* };                                                             */
/*Return     : 1 -- have Event, 0 -- no Event.                    */
/******************************************************************/
int ipanel_porting_socket_ontimer(unsigned int event[3])
{//IPANEL_DEBUG("aaa\n");
	int eventCode;


   	if(!Ipanel_GetEntrance_Flag())
	{
		return 0;
   	}
	
	if(sock_mgr->n_idle >= MAX_SOCKETS)
	{
		return 0;
	}
    
	eventCode = SystemSocketManager_pop_event(sock_mgr, &event[2], &event[1]);
	
	if (eventCode == 0) {
		SystemSocketManager_poll(sock_mgr);
		eventCode = SystemSocketManager_pop_event(sock_mgr, &event[2], &event[1]);
	}

	if(eventCode != 0)
	{
		event[0] = EIS_EVENT_TYPE_NETWORK;
		return 1;
	}
	else
	{
		event[0] = 0;
		return 0;
	}
}

/******************************************************************/
/*Description: Initialize Network                                 */
/*Input      : No                                                 */
/*Output     : No                                                 */
/*Return     : No                                                 */
/******************************************************************/
void ipanel_porting_network_init(void)
{
	//IPANEL_DEBUG("ipanel_porting_network_init\n");
	if(!Ipanel_GetEntrance_Flag())
	{
		return;
	}
	sock_mgr = SystemSocketManager_new();
}

/******************************************************************/
/*Description: free Network resource which initialized by init()  */
/*Input      : No                                                 */
/*Output     : No                                                 */
/*Return     : No                                                 */
/******************************************************************/
void ipanel_porting_network_exit(void)
{
	//IPANEL_DEBUG("ipanel_porting_network_exit\n");

	if(!Ipanel_GetEntrance_Flag())
	{
		return;
	}

	SystemSocketManager_delete(sock_mgr);
	sock_mgr = NULL;
}


/******************************************************************/
/*Description: Get network status.                                */
/*             1 -- is ok                                         */
/*Input      : No                                                 */
/*Output     : No                                                 */
/*Return     : network status                                     */
/******************************************************************/
int ipanel_porting_get_network_status(void)
{
	//IPANEL_DEBUG("ipanel_porting_get_network_status\n");
	return 1;
}


static SystemSocketManager *SystemSocketManager_new(void)
{
	SystemSocketManager *me;
	int i;

	//IPANEL_DEBUG("SystemSocketManager_new\n");

	me = (SystemSocketManager*) &my_manger;
	memset(me,0,sizeof(SystemSocketManager));
	FD_ZERO(&me->fds_r);
	FD_ZERO(&me->fds_w);
	FD_ZERO(&me->fds_e);
	for (i=0;i<MAX_SOCKETS;i++) 
	{
		me->sockets[i] = -1;
    }
    	me->n_idle = MAX_SOCKETS;
    	return me;
}

static void SystemSocketManager_delete(SystemSocketManager *me)
{
	int i;
	//IPANEL_DEBUG("SystemSocketManager_delete\n");
    	for (i=0;i<MAX_SOCKETS;i++) 
		{
        	if (me->sockets[i]!=-1) {
			closesocket(me->sockets[i]);
        	}
    	}
}


static int SystemSocketManager_register(SystemSocketManager *me, int s)
{
	int i, indx=-1;
	//IPANEL_DEBUG("SystemSocketManager_register\n");
	for (i=0;i<MAX_SOCKETS;i++) {
		if (me->sockets[i]==-1) {
			me->sockets[i] = s;
			if (me->maxfds<s) me->maxfds = s;
			FD_SET(s, &me->fds_r);
			FD_SET(s, &me->fds_w);
			me->n_idle --;
			indx = i;
			break;
		}
	}
	return indx;
}

static int SystemSocketManager_unregister(SystemSocketManager *me, int s)
{
	int i, indx=-1;
	//IPANEL_DEBUG("SystemSocketManager_unregister\n");
	for (i=0;i<MAX_SOCKETS;i++) {
		if (me->sockets[i]==s) {
			me->sockets[i] = -1;
			FD_CLR(s, &me->fds_r);
			FD_CLR(s, &me->fds_w);
			me->n_idle ++;
			indx = i;
			break;
		}
	}
	return indx;
}

static void SystemSocketManager_put_event(SystemSocketManager *me, int s, int evt)
{
	int indx = me->event_indx_write;
	if ((indx + 1)%MAX_EVENTS == me->event_indx_read) {
		//IPANEL_DEBUG("[SystemSocketManager]lose event, queque overflow!!!\n");
		return;
	}
	me->event_socket[indx] = s;
	me->event_type[indx] = evt;
	indx ++;
	if (indx>=MAX_EVENTS) indx = 0;
	me->event_indx_write = indx;   
}

static int SystemSocketManager_pop_event(SystemSocketManager *me, int *s, int *event)
{
	int indx;
	int eventCode;
	indx = me->event_indx_read;
	if (indx != me->event_indx_write) {
		*s = me->event_socket[indx];
		*event = me->event_type[indx];
		me->event_socket[indx] = 0;
		me->event_type[indx] = 0;
		indx++;
		if (indx>=MAX_EVENTS) indx=0;
		me->event_indx_read = indx;
		eventCode=1;
	} else {
		eventCode=0;
	}
	return eventCode;
}
  
static int SystemSocketManager_poll(SystemSocketManager *me)
{
	int nactive, i;
	struct timeval tm = {0,7};
	SOCKET m_s;
	fd_set fds_r, fds_w, fds_e;
	fds_r = me->fds_r;
	fds_w = me->fds_w;
	FD_ZERO(&fds_e);
  
	nactive = select(me->maxfds+1, &fds_r, NULL, &fds_e, &tm);
	if (nactive<0) {  /* failure */
		goto ret_poll;
	} else if (nactive==0) {  /* time out; */
		goto ret_poll;
	}
	for (i=0;i<MAX_SOCKETS;i++) {
		if ((m_s=me->sockets[i])==-1) continue;
		if (FD_ISSET(m_s, &fds_r)) {
			SystemSocketManager_put_event(me, m_s, SOCKET_EVENT_TYPE_READ);
		}
    	}
ret_poll:
    	return nactive;
}

/*static void SystemSocketManager_process(void)
{
	IPANEL_DEBUG("SystemSocketManager_process\n");
}*/


BS08 *vaild_protocol_str = NULL;
BU08 move_id[1000];
extern BS08 g_bEnterEnrichVod ;
BU08 g_bplaymove = 0;//是否播放点播节目

int ipanel_get_playmove_flag()
{
	return g_bplaymove;
}

int ipanel_set_playmove_flag(char flag)
{
	g_bplaymove = flag;
}

int ipanel_porting_protocol_processor(const char *protocolUrl)
{
#ifdef USE_ENREACH
	BU32 index;
	vaild_protocol_str = NULL;
	
	if ((vaild_protocol_str = strstr(protocolUrl, "contentid")) != NULL)
	{
		vaild_protocol_str += 10;
		printf("vaild_protocol_str = %s\n", vaild_protocol_str);
		if (vaild_protocol_str[0] == '-' && vaild_protocol_str[1] == '1')
		{
			g_bplaymove = 0;
		}
		else
		{
			g_bplaymove = 1;
		}
	}
	strcpy(move_id, vaild_protocol_str);
	printf("protocolUrl = %s\n",protocolUrl);
	printf("moveid = %s\n",move_id);


	memset(socket_buf, 0, sizeof(socket_buf));
	//GUI_API_GetBitmap(GUI_API_GetDesktopWindow(), 0, 0, 720, 576, (void *)socket_buf);
	GUI_ENC_API_SwitchState(ID_DLG_FULLTOTVOD, 1);
	g_bEnterEnrichVod = 1;
	while(g_bEnterEnrichVod)
	{
		FYF_API_thread_sleep(1000);
	}
	g_bplaymove = 0;
	FYF_API_OSD_FillRect(0, 0, 720, 576, 0x8000);
	FYF_API_gdi_draw_image(0, 0, 640, 526, 0, 0);
	//GUI_API_DrawBitmap(GUI_API_GetDesktopWindow(), 0, 0, 720, 576, /*(void *)buf*/(void *)socket_buf);
#endif

#if defined (USE_FZVOD)
	unsigned int event[3] = {0};
	int  i = 0, j = 0;
	char *DataPointer = FYF_NULL;
	char StartClock[25] = {0}, StartClockTemp[20] = {0};
    BU08 ipanel_entervod_mute_state;
    FYF_QUEUE_MESSAGE stMsg;    
	FYF_QUEUE_MESSAGE vodmsg;
	int ret = -1;
	BU08  *buf = FYF_NULL;

	//buf = (BU08 *)malloc(720*576*2);
	printf("protocolUrl = %s\n",protocolUrl);
	bStopwhitescreen = FYF_TRUE;



	FYF_API_thread_sleep(500);
	/*printf("buf = ");
	for (j = 0; j < 1000; j++)
	{
		printf("%2x ", buf[1000 + j]);
	}
	printf("\n");*/

	ipanel_entervod_mute_state = ipanel_GetAudMuteState();
	printf("ipanel_entervod_mute_state = %d\n", ipanel_entervod_mute_state);
	if ((DataPointer = (char*)strstr((char*)protocolUrl, "start-time")) != FYF_NULL)
	{
		if(bPlayTVOD)
		{
			if(RTSP_GetFiledNumStr(protocolUrl, "start-time", StartClockTemp) == FYF_ERR)
			{
				IPANEL_ERROR("can't find tvod start time !!!\n");
				
				g_fromIpanel = FYF_TRUE;
				memset(g_StartClock, 0, 25);
				OW_TVOD_MainMenu(protocolUrl, 2, NULL);
			}
			else
			{
				printf("start-time StartClockTemp = %s \n",StartClockTemp);

				FYF_API_Memcpy(StartClock, StartClockTemp, 8);
				StartClock[8] = 'T';
				FYF_API_Memcpy(&(StartClock[9]), &(StartClockTemp[8]) ,6);
				StartClock[15] = '.';
				StartClock[16] = '0';
				StartClock[17] = '0';
				StartClock[18] = 'Z';
				IPANEL_DEBUG("start-time StartClock = %s \n",StartClock);
				
				memset(g_StartClock, 0, 25);
				memcpy(g_StartClock, StartClock, strlen(StartClock));
				

				g_fromIpanel = FYF_TRUE;

				OW_TVOD_MainMenu(protocolUrl, 2, StartClock);
			}
			bPlayTVOD = FYF_FALSE;
		}
	}
	else
	{
		if ((DataPointer = (char*)strstr((char*)protocolUrl, "duration")) != FYF_NULL)
		{
			if(RTSP_GetFiledNumStr(protocolUrl, "duration", StartClockTemp) == FYF_ERR)
			{
				IPANEL_ERROR("can't find tvod start time !!!\n");
			}
			else
			{
				recStarTime = atoi(StartClockTemp);	
				printf("recStarTime = %d\n", recStarTime);
				
				FYF_API_Memset(StartClockTemp, 0, sizeof(StartClockTemp));
				for(i = 0; i < 20; i++)
				{
					if(DataPointer[i] == '_')
					{
						break;
					}
				}
				if(i == 20)
				{
					recLongTime = -1;
				}
				else
				{
					i++;
					DataPointer += i;
					if(DataPointer[0] < '0' || DataPointer[0] > '9')
					{
						recLongTime = -1;
					}
					else
					{
						for(i = 0; i < 10; i++)
						{
							if(DataPointer[i] >= '0' && DataPointer[i] <= '9')
							{
								StartClockTemp[i] = DataPointer[i];
							}
							else
							{
								break;
							}
						}
						if(i != 10)
						{
							recLongTime = atoi(StartClockTemp);
						}
						else
						{
							recLongTime = -1;
						}
					}
				}
				
				IPANEL_DEBUG("recLongTime = %d\n", recLongTime);
				OW_SetRecordFlag(FYF_TRUE);
			}
			

		}	

				
		//g_fromIpanelvod = FYF_TRUE;

		OW_VOD_MainMenu(protocolUrl);
		
	}

	bExitSocket = FYF_FALSE;
	
	if(ipanel_entervod_mute_state != ipanel_GetAudMuteState())
	{
		IPANEL_DEBUG("Get Mute State.\n");
		event[0] = EIS_EVENT_TYPE_IRKEY;
		event[1] = EIS_IRKEY_VOLUME_MUTE;
		event[2] = 0;
		stMsg.qlstWordOfMsg 	= (BU32)event[0];
		stMsg.q2ndWordOfMsg 	= (BU32)event[1];
		stMsg.q3rdWordOfMsg 	= (BU32)event[2];	
		stMsg.q4thWordOfMsg 	= 0;
		if (0 != s_pIpanelMsgQueue)
		{
			FYF_API_queue_send(s_pIpanelMsgQueue, &stMsg);	
		}
	}

	//ipanel_done = FYF_TRUE;

	/*while (bExitSocket == FYF_FALSE)
	{
		FYF_API_thread_sleep(1000);
	}*/


	
	printf("bExitSocket = %d\n",bExitSocket);
	
	if (VodPlayPara.VodOrTvod == 1)
		FYF_API_thread_sleep(1000);
	
	FYF_API_thread_sleep(500);

	//free(buf);
	//buf = NULL;
	w = 0;
	r = 0;
	num = 0;
	APP_IPI_StopAudioVideo();
	//OW_DLRTPInfo_StopChannel();

	
	GUI_API_DrawBitmap(GUI_API_GetDlgItem(ID_DLG_Null), 0, 0, 720, 576, /*(void *)buf*/(void *)socket_buf);
	//IPANEL_ERROR("end ?????????????????????????????????");
	
	bDrawAgain = FYF_TRUE;
#elif defined(USE_ENREACH)
	printf("protocolUrl enrich = %s\n",protocolUrl);
#endif
	return 0;
}



