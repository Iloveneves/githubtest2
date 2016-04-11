/*****************************************************************************
 *
 * Description: 
 *    
 *
 * Revision History:
 *    Date          Author         Modification
 * ---------------------------------------------------------------------
 * 1. 2008-10-7   
 *
 *****************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>   
#include <sys/ioctl.h>   
#include <errno.h>

#include "hi_type.h"
#include "enreach_pub.h"
#include "enreach_erro.h"
#include "ebrowser_api.h"

#include "enreach_sock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAPI_ETH_DEV_NAME "eth0"
#define SOCKET_MAP 
#define SOCKET             HI_S32
#define closesocket(s)     close(s)

#define  MAX_SOCKETS       10
#define  MAX_EVENTS        128
#define  MAC_ADDR_LEN      32
#define  SOCKET_BUFF_SIZE  4096
#define  TIME_OUT_TIMES    35

typedef enum tagSOCKET_STATUS_S
{
    SOCKET_EVENT_TYPE_CONNECT = 1,
    SOCKET_EVENT_TYPE_CLOSE,
    SOCKET_EVENT_TYPE_READ
} SOCKET_STATUS_S;

typedef struct tagSYSTEM_SOCKET_MAG_S
{
    HI_S32     s32Idle;                     /* 最大可创建的Socket的数 */
    HI_S32     s32Sockets[MAX_SOCKETS];     /* Socket句柄             */
    HI_S32     s32SockFlag[MAX_SOCKETS];    /* reserve                */
    HI_S32     s32SockState[MAX_SOCKETS];   /* Socket是否创建         */
    HI_S32     s32MaxFds;                   /* reserve                */
    HI_S32     s32EventIndxRead;            /* reserve                */
    HI_S32     s32EventIndxWrite;           /* reserve                */
    HI_S32     s32EventSocket[MAX_EVENTS];  /* reserve                */
    HI_S32     s32EventType[MAX_EVENTS];    /* reserve                */
    fd_set     struFds_r, struFds_w, struFds_e;
} SYSTEM_SOCKET_MAG_S;

/* BGN: 退出浏览器前停止select操作，防止浏览器在网络状态不好的情
   况下，多次执行sock_send,sock_recv操作，导致超时很长，此时可以
   通过设置g_bStop_select的值让浏览器快速推出 */
static pthread_mutex_t g_mutex_select;
static HI_BOOL g_bStop_select  = HI_FALSE;

static HI_BOOL g_bExitRecv_Tag = HI_TRUE;
static SYSTEM_SOCKET_MAG_S g_struSysSocket_Mng;            /* Socket句柄管理变量     */
static SYSTEM_SOCKET_MAG_S *g_pstruSysSocket_Mng = NULL;   /* Socket句柄管理变量指针 */
static HI_U8  g_u8Mac_Address[MAC_ADDR_LEN] = {0};         /* 机顶盒Mac地址存储      */

static SYSTEM_SOCKET_MAG_S *systemSocketManagerNew(HI_VOID)
{  
    HI_S32 i = 0;
    SYSTEM_SOCKET_MAG_S *pstruSockMng = NULL;

    pstruSockMng = (SYSTEM_SOCKET_MAG_S*)&g_struSysSocket_Mng;
    memset(pstruSockMng, 0, sizeof(SYSTEM_SOCKET_MAG_S));

    FD_ZERO(&pstruSockMng->struFds_r);
    FD_ZERO(&pstruSockMng->struFds_w);
    FD_ZERO(&pstruSockMng->struFds_e);

    for (i = 0; i < MAX_SOCKETS; i++)
    {
        pstruSockMng->s32Sockets[i]   = -1;
        pstruSockMng->s32SockState[i] = 0;
        pstruSockMng->s32SockFlag[i]  = -1;
    }

    pstruSockMng->s32Idle = MAX_SOCKETS;

    return pstruSockMng;
}

/*
	枣庄数据广播使用
	惠州FLASH使用
	使用枣庄版本
*/
static HI_S32 systemSocketManagerSockSelect(ESOCK_S *pstruSock, HI_CHAR szSend)
{
    HI_S32 i = 0, s32Ret   = 0;
    HI_U32 u32TimeOutTry   = 0;
    
    struct timeval timeOut;
    fd_set struFds;

    if (NULL == pstruSock)
    {
        return HI_FAILURE;
    }

    u32TimeOutTry = TCP_SELECT_RETRY;

    for (i = 0; i < (HI_S32)u32TimeOutTry; i++)
    {
        pthread_mutex_lock(&g_mutex_select);
        if (HI_TRUE == g_bStop_select)
        {
            pthread_mutex_unlock(&g_mutex_select);
            break;
        }
        
        pthread_mutex_unlock(&g_mutex_select);
        FD_ZERO(&struFds);
        FD_SET((HI_U32)(pstruSock->s32Socket), &struFds);
        timeOut.tv_sec  = TCP_SELECT_SEC;
        timeOut.tv_usec = TCP_SELECT_MSEC;

        if (szSend)
        {
            s32Ret = select(FD_SETSIZE, 0, &struFds, 0, &timeOut);
        }
        else
        {
            s32Ret = select(FD_SETSIZE, &struFds, 0, 0, &timeOut);
            
            /* if (s32Ret < 0)
            {
                perror("select");
            } */
        }

        if (s32Ret > 0)
        {
            if (FD_ISSET(pstruSock->s32Socket, &struFds))
            {
                EVM_ERROR("### can recv data !###\r\n");
                return 0;
            }
        }
        else if ((s32Ret < 0) && (errno == EINTR))
        {
            /* select操作收到signal，重新执行select操作 */
            if (0 < i)
            {
                i = i - 1;
            }
        }
        else
        {
            EVM_ERROR("### can not recv data, ret = 0x%x, errno = %d ###\r\n", s32Ret, errno);
        }

        /* 1s，如果时间太短会导致数据丢失 */
        
        usleep(1000 * 1000);
    }

    return TIMEOUT_ERROR;
}

/* 注意:使用了多个的socket */

static HI_VOID systemSocketManagerDelete(SYSTEM_SOCKET_MAG_S * pstruSocketMng)
{
    HI_S32 i = 0;

    if (NULL == pstruSocketMng)
    {
        return;
    }
    
    for (i = 0; i < MAX_SOCKETS; i++) 
    {
        if (-1 != pstruSocketMng->s32Sockets[i]) 
        {
            closesocket(pstruSocketMng->s32Sockets[i]); 
        }
    }
}

static HI_S32 systemSocketManagerRegister(SYSTEM_SOCKET_MAG_S * pstruSocketMng,
                                          HI_S32 s32Num)
{
    HI_S32 i = 0, s32Indx = -1;

    if (NULL == pstruSocketMng)
    {
        return s32Indx;
    }

    for (i = 0; i < MAX_SOCKETS; i++) 
    {
        if (-1 != pstruSocketMng->s32Sockets[i]) 
        {
            continue;
        }
        
        pstruSocketMng->s32Sockets[i]   = s32Num;
        pstruSocketMng->s32SockState[i] = 1;

        if (pstruSocketMng->s32MaxFds < s32Num)
        {
            pstruSocketMng->s32MaxFds = s32Num;
        }

        FD_SET(s32Num, &pstruSocketMng->struFds_r);
        FD_SET(s32Num, &pstruSocketMng->struFds_w);
        
        pstruSocketMng->s32Idle--;
        s32Indx = i;
        break;
    }
    
    return s32Indx;
}

static HI_S32 systemSocketManagerUnregister(SYSTEM_SOCKET_MAG_S * pstruSocketMng,
                                            HI_S32 s32Num)
{
    HI_S32 i = 0, s32Indx = -1;

    if (NULL == pstruSocketMng)
    {
        return s32Indx;
    }

    for (i = 0; i < MAX_SOCKETS; i++) 
    {
        if (pstruSocketMng->s32Sockets[i] != s32Num) 
        {
            continue;
        }
        
        pstruSocketMng->s32Sockets[i]   = -1;
        pstruSocketMng->s32SockState[i] = 0;
        pstruSocketMng->s32SockFlag[i]  = -1;
        
        FD_CLR(s32Num, &pstruSocketMng->struFds_r);
        FD_CLR(s32Num, &pstruSocketMng->struFds_w);

        pstruSocketMng->s32Idle++;
        s32Indx = i;
        break;
    }
    
    return s32Indx;
}

static HI_VOID systemSocketManagerPutEvent(SYSTEM_SOCKET_MAG_S * pstruSocketMng,
                                           HI_S32 s32Num,
                                           HI_S32 s32Evt)
{
    HI_S32 s32Indx = 0;

    if (NULL == pstruSocketMng)
    {
        return;
    }

    s32Indx = pstruSocketMng->s32EventIndxWrite;
    
    if (((s32Indx + 1) % MAX_EVENTS) == pstruSocketMng->s32EventIndxRead)
    {
    	return;
    }
    
    pstruSocketMng->s32EventSocket[s32Indx] = s32Num;
    pstruSocketMng->s32EventType[s32Indx]   = s32Evt;
    s32Indx++;
    
    if (MAX_EVENTS >= s32Indx)
    {
        s32Indx = 0;
    }
    
    pstruSocketMng->s32EventIndxWrite = s32Indx;

    return;
}

#if 0
static HI_S32 systemSocketManagerPopEvent(SYSTEM_SOCKET_MAG_S * pstruSocketMng,
                                          HI_S32 * ps32Nums,
                                          HI_S32 * ps32Event)
{
    HI_S32 s32Indx      = -1;
    HI_S32 s32EventCode = 0;

    if (NULL == pstruSocketMng)
    {
        return s32Indx;
    }

    s32Indx = pstruSocketMng->s32EventIndxRead;
    
    if (s32Indx != pstruSocketMng->s32EventIndxWrite) 
    {
        *ps32Nums  = pstruSocketMng->s32EventSocket[s32Indx];
        *ps32Event = pstruSocketMng->s32EventType[s32Indx];

        pstruSocketMng->s32EventSocket[s32Indx] = 0;
        pstruSocketMng->s32EventType[s32Indx]   = 0;
        s32Indx++;
        
        if (MAX_EVENTS >= s32Indx)
        {
            s32Indx = 0;
        }
        
        pstruSocketMng->s32EventIndxRead = s32Indx;
        s32EventCode = 1;
    }
    else
    {
        s32EventCode = 0;
    }
    
    return s32EventCode;
}

static HI_S32 systemSocketManagerPoll(SYSTEM_SOCKET_MAG_S * pstruSocketMng)
{
    HI_S32 s32Nactive  = 0, i = 0;
    struct timeval tm  = {0, 4};
    SOCKET s32SocketId = 0;

    if (NULL == pstruSocketMng)
    {
        return 0;
    }

    fd_set struFds_r, struFds_w, struFds_e;

    struFds_r = pstruSocketMng->struFds_r;
    struFds_w = pstruSocketMng->struFds_w;
    FD_ZERO(&struFds_e);

    s32Nactive = select(pstruSocketMng->s32MaxFds + 1,
                        &struFds_r, NULL, &struFds_e, &tm);

    if (0 > s32Nactive) 
    {
        /* failure */
        goto ret_poll;
    } 
    else if (0 == s32Nactive) 
    {
        /* time out; */
        goto ret_poll;
    }
    
    for (i = 0; i < MAX_SOCKETS; i++) 
    {
        if (-1 == (s32SocketId = pstruSocketMng->s32Sockets[i])) 
        {
            continue;
        }
        
        if (0 == pstruSocketMng->s32SockFlag[i])
        {
            if (FD_ISSET(s32SocketId, &struFds_r)) 
            {
                
                systemSocketManagerPutEvent(pstruSocketMng,
                                            s32SocketId,
                                            SOCKET_EVENT_TYPE_READ); 
            }
        }
    }
    
ret_poll:
    
    return s32Nactive;
}
#endif


#if 0
static int is_ip(char *ipaddr)
{
  char *pnum,*pdot=ipaddr;
  int i=0;

  for(;*ipaddr;ipaddr=pdot++,i++)
  {
    int t=0,e=1;
    if(*(pnum=pdot)=='.') return 1;

    while(*pdot!='.'&&*pdot)
	{
		++pdot;
	}

	ipaddr = pdot-1;
    while(ipaddr>=pnum)
	{
		t+=e*(*ipaddr---'0');
		e*=10;
	}

    if(t<0||t>255)
		return 1;
  }

  if (i !=4 ) return 1;

  return 0;
}

HI_S32 sock_ethernetIPAddressSet(HI_CHAR *ipAdd)
{
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *sin;

    if (NULL == ipAdd)
    {
        return HI_FAILURE;
    }

    if (is_ip(ipAdd) != 0)
    {
        return HI_FAILURE;
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, MAPI_ETH_DEV_NAME, sizeof(ifr.ifr_name)-1);
    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    sin->sin_family = AF_INET;

    if (inet_pton(AF_INET, ipAdd, &sin->sin_addr) <= 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    if(ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    close(sockfd);

    return HI_SUCCESS;    
}

HI_S32 sock_ethernetIPAddressGet(HI_CHAR *ipAdd)
{
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *s_in;

    if (NULL == ipAdd)
    {
        return HI_FAILURE;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, MAPI_ETH_DEV_NAME);

    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    s_in = (struct sockaddr_in *)(&ifr.ifr_addr);
    memcpy((void *)ipAdd, inet_ntoa(s_in->sin_addr), 15);
    close(sockfd);

    return HI_SUCCESS;
}

HI_S32 sock_ethernetSubNetmaskSet(HI_CHAR *subNetmask)
{
    int sockfd;
    struct ifreq    ifr;
    struct sockaddr_in *sin;

    if (NULL == subNetmask)
    {
        return HI_FAILURE;
    }

    if (is_ip(subNetmask) != 0)
    {
        return HI_FAILURE;
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, MAPI_ETH_DEV_NAME, sizeof(ifr.ifr_name)-1);
    sin = (struct sockaddr_in *)&ifr.ifr_netmask;
    sin->sin_family = AF_INET;

    if (inet_pton(AF_INET, subNetmask, &sin->sin_addr) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    if (ioctl(sockfd, SIOCSIFNETMASK, &ifr) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    close(sockfd);

    return HI_SUCCESS;
}

HI_S32 sock_ethernetSubNetmaskGet(HI_CHAR *subNetmask)
{
    int skfd;
    struct ifreq ifr;
    struct sockaddr_in *s_in;

    if (NULL == subNetmask)
    {
        return HI_FAILURE;
    }

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (skfd < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, MAPI_ETH_DEV_NAME);

    if (ioctl(skfd, SIOCGIFNETMASK, &ifr) < 0)
    {
        EVM_ERROR("### erro! ###\r\n");
        return HI_FAILURE;
    }

    s_in = (struct sockaddr_in *)(&ifr.ifr_netmask);
    memcpy((void *)subNetmask, inet_ntoa(s_in->sin_addr), 15);
    close(skfd);

    return HI_SUCCESS;
}

HI_S32 sock_ethernetGatewaySet(HI_CHAR *gateway)
{
    char str[512];
    int rt = 0;

    if (gateway == NULL)
    {
        return HI_FAILURE;
    }

    if (is_ip(gateway) != 0)
    {
        return HI_SUCCESS;
    }

    rt = sprintf(str, "route add default gw %s", gateway);

    if ( rt < 0)
    {
        return HI_FAILURE;
    }

    rt = system(str);

    if ( rt < 0)
    {
        return HI_FAILURE;
    }
    else
    {
        return HI_SUCCESS;
    }
}


static char* skip_space(char* line)
{
	char* p = line;
	while (*p == ' ' || *p == '\t')
	{
		p++;
	}
	return p;
}

static char* skip_word(char* line)
{
	char* p = line;
	while (*p != '\t' && *p != ' ' && *p != '\n' && *p != 0)
	{
		p++;
	}
	return p;
}

static char* get_word(char* line, char* value)
{
	char* p = line;

	p = skip_space(p);
	while (*p != '\t' && *p != ' ' && *p != '\n' && *p != 0)
	{
		*value++ = *p++; 
	}
	*value = 0;
	return p;
}

HI_S32 sock_ethernetGatewayGet(HI_CHAR *gateway)
{
	char line[512];
	char str[16];
	FILE* fp;

	if (gateway == NULL)
	{
		strcpy(gateway, "0.0.0.0");		
		return HI_FAILURE;
	}

	fp = fopen("/proc/net/route", "r");
    
	if (fp)
	{
        memset(str, 0, sizeof(str));
        while (fgets(line, sizeof(line), fp) != 0)
        {
            char* p = line;
            p = strstr(p, MAPI_ETH_DEV_NAME);

            if (p)
            {
                p = get_word(p, str);
                p = get_word(p, str);

                if (strcmp(str, "00000000") == 0)
                {
                    p = get_word(p, str);

                    if (strcmp(str, "00000000") != 0)
					{
                        int a, b, c, d;
                        sscanf(str, "%02X%02X%02X%02X", &a, &b, &c, &d);
                        sprintf(gateway, "%d.%d.%d.%d", d, c, b, a);
                        fclose(fp);
                        return HI_SUCCESS;
                    }
                }
            }
        }
		fclose(fp);		
    }

    strcpy(gateway, "0.0.0.0");	
    
	return HI_FAILURE;
}

#endif

/*
	未使用
*/

HI_VOID sock_start(HI_VOID)
{
    EVM_DEBUG("Enter");

    if (NULL != g_pstruSysSocket_Mng)
    {
        return;
    }
    
    pthread_mutex_init(&g_mutex_select, NULL);
    g_pstruSysSocket_Mng = systemSocketManagerNew();

    g_bStop_select = HI_FALSE;

    return;
}

HI_VOID sock_stop(HI_VOID)
{
    EVM_ERROR("### call func! ###\r\n");

	return;

    if (NULL == g_pstruSysSocket_Mng)
    {
        return;
    }

    systemSocketManagerDelete(g_pstruSysSocket_Mng);
    g_pstruSysSocket_Mng = NULL;
    g_bStop_select = HI_FALSE;
    pthread_mutex_destroy(&g_mutex_select);

    return;
}
HI_S32  sock_open( ESOCK_S *pstruSock )
{
    HI_S32 s32Indx     = 0;
    HI_S32 s32SockType = 0;
    HI_S32 s32Status   = 0, i = 0;
    SOCKET s32SocketId = 0;
    struct timeval tm  = {10, 0};
    struct sockaddr_in serv_addr; 

    EVM_ERROR("Enter");

    if (NULL == pstruSock)
    {
        EVM_ERROR("### erro! ###\r\n");
        s32SocketId = -1;
        
        goto CREATEERRO;
    }

    if (g_pstruSysSocket_Mng->s32Idle <= 0) 
    {
        EVM_ERROR("### erro! ###\r\n");
        s32SocketId          = -1;
        pstruSock->s32Socket = s32SocketId;
        
        goto CREATEERRO;
    }

    s32SockType = (0 == pstruSock->szTypeOption) ? SOCK_STREAM : SOCK_DGRAM;
    s32SocketId = socket(AF_INET, s32SockType, 0);

    /* Socket创建失败 */

    if (0 > s32SocketId) 
    {
        EVM_ERROR("### erro! ###\r\n");
        s32SocketId          = -1;
        pstruSock->s32Socket = -1;
        
        goto CREATEERRO;
    }

    /* ip已经经过网络字节转换 */

    serv_addr.sin_family      		= AF_INET;
    serv_addr.sin_addr.s_addr 	= pstruSock->struIP.u32Addr;
    serv_addr.sin_port        		= htons(pstruSock->struIP.u16Port);
    bzero(&(serv_addr.sin_zero), 8);

    /* set connect timeout, if tcp. and Not set if other */
    for (i = 0; i < SOCK_CONNECT_TIME;  i++)
    {
        pthread_mutex_lock(&g_mutex_select);
        if (HI_TRUE == g_bStop_select)
        {
            pthread_mutex_unlock(&g_mutex_select);
            break;
        }

        pthread_mutex_unlock(&g_mutex_select);
        
        tm.tv_sec = SOCK_CONNECT_SEC;
        EVM_ERROR("connecting....%d", i);  
        s32Status = setsockopt(s32SocketId, SOL_SOCKET, SO_SNDTIMEO, &tm, sizeof(tm));

        if (s32Status < 0)
        {
            continue;
        }
       
        s32Status
            = connect(s32SocketId, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        if (0 == s32Status)
        {
            break;
        }
        else
        {
            EVM_ERROR("connecting....%d fail", i);  
        }
    }
    
    EVM_ERROR("### connect end. ret = %d \n", s32Status);

    if (0 != s32Status) 
    {
        EVM_ERROR("### Socket Connect Error status = %d\n", s32Status);
        
        closesocket(s32SocketId);
        s32SocketId          = -1;
        pstruSock->s32Socket = -1;
        
        goto CREATEERRO;
    }


	

    pstruSock->s32Socket = s32SocketId;
    s32Indx = systemSocketManagerRegister(g_pstruSysSocket_Mng, s32SocketId); 

    if (-1 == s32Indx)
    {
        closesocket(s32SocketId);
        s32SocketId          = -1;
        pstruSock->s32Socket = -1;

        goto CREATEERRO;
    }

    g_pstruSysSocket_Mng->s32SockFlag[s32Indx] = 0;

    /* Socket事件保留，暂未使用 */
    systemSocketManagerPutEvent(g_pstruSysSocket_Mng,
                                s32SocketId,
                                SOCKET_EVENT_TYPE_CONNECT);
CREATEERRO:

    return s32SocketId;
}
HI_S32  sock_close( ESOCK_S *pstruSock )
{
#if 1
    SOCKET socket = 0;
    HI_S32 i = TIME_OUT_TIMES;

    while (i > 0)
    {
        if (HI_TRUE == g_bExitRecv_Tag)
        {
            break;
        }

        usleep(1000000);
    }
    
    EVM_ERROR("$$$ function $$$\r\n");

    if (!pstruSock || (pstruSock->s32Socket < 0))
    {
        return TCPIP_ERROR;
    }

    socket = pstruSock->s32Socket;
    pstruSock->s32Socket = TCPIP_ERROR;
    systemSocketManagerUnregister(g_pstruSysSocket_Mng, socket);
    
    return closesocket(socket);
#endif
}


HI_S32  sock_send( ESOCK_S *pstruSock, HI_CHAR *pszBuf, HI_S32 s32Len )
{
    HI_S32 s32Ret = 0;

    EVM_ERROR("$$$ function $$$\r\n");

    if (HI_TRUE == g_bStop_select)
    {
        return TCPIP_ERROR;
    }

    if (!pstruSock || (pstruSock->s32Socket < 0) || !pszBuf || (s32Len <= 0))
    {
        EVM_ERROR("$$$ erro $$$\r\n");
        return TCPIP_ERROR;
    }

    if (ISUSESENDTO(pstruSock->szTypeOption))
    {
        EVM_ERROR("$$$ erro $$$\r\n");

        struct sockaddr_in addr;

        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(pstruSock->struIP.u16Port);
        addr.sin_addr.s_addr = pstruSock->struIP.u32Addr;

        s32Ret = sendto(pstruSock->s32Socket, pszBuf, s32Len,
                        0, (struct sockaddr *)&addr, sizeof(addr));
    }
    else
    {
        EVM_ERROR("$$$ send buf is : \r\n %s $$$\r\n", pszBuf);
        s32Ret = send(pstruSock->s32Socket, pszBuf, s32Len, 0);
        EVM_ERROR("$$$ socket send end $$$\r\n");
    }

    if (s32Ret < 0)
    {
        return TCPIP_ERROR;
    }

    return s32Ret;
}

HI_VOID sock_stopRecv()
{
    EVM_ERROR("$$$ function $$$\r\n");
    pthread_mutex_lock(&g_mutex_select);
    g_bStop_select = HI_TRUE;
    pthread_mutex_unlock(&g_mutex_select);

    usleep(10000);
    sock_close(NULL);

    return;
}
/*
	枣庄数据广播使用
	惠州FLASH使用
	使用枣庄版本
*/

HI_S32  sock_recv( ESOCK_S *pstruSock, HI_CHAR *pszBuf, HI_S32 s32Len )
{    
    HI_S32 s32Ret = 0;

    g_bExitRecv_Tag = HI_FALSE;

    if (!pstruSock || (pstruSock->s32Socket < 0) || !pszBuf || (s32Len <= 0))
    {
        EVM_ERROR("$$$ erro $$$\r\n");
        g_bExitRecv_Tag = HI_TRUE;
        return TCPIP_ERROR;
    }

    /* sock超时处理，根据需要选择是否开启 */
#if 1
    s32Ret = systemSocketManagerSockSelect(pstruSock, 0);

    if (s32Ret < 0)
    {
        EVM_ERROR("$$$ erro $$$\r\n");
        g_bExitRecv_Tag = HI_TRUE;

        return s32Ret;
    }
#endif

    if (ISUSERECVFROM(pstruSock->szTypeOption))
    {
        EVM_ERROR("$$$ erro $$$\r\n");
        struct sockaddr_in addr;
        HI_S32 len = sizeof(addr);
        
        s32Ret = recvfrom(pstruSock->s32Socket, pszBuf, s32Len,
                          0, (struct sockaddr *)&addr, &len);
        if (s32Ret >= 0)
        {
            pstruSock->struFromIP.u32Addr = addr.sin_addr.s_addr;
            pstruSock->struFromIP.u16Port = ntohs(addr.sin_port);
        }
    }
    else
    {
        s32Ret = recv(pstruSock->s32Socket, pszBuf, s32Len, 0);
    }

    if (s32Ret < 0)
    {
        systemSocketManagerPutEvent(g_pstruSysSocket_Mng,
                                    pstruSock->s32Socket,
                                    SOCKET_EVENT_TYPE_CLOSE);
        s32Ret = TCPIP_ERROR;
    }

    g_bExitRecv_Tag = HI_TRUE;

    return s32Ret;
}

HI_U32  sock_getIpAddr( HI_CHAR *pszName, ESOCKCTRL_S *pCtrl,
                        HI_U16 *pu16Port, HI_CHAR szCheckNumIP )
{
	HI_U32 u32Ret = 0, i = 0, tmp = 0;
	HI_CHAR *pszName0 = NULL;

	EVM_ERROR("Enter, name:%s, port:%u, check_num_ip:%d", pszName, pu16Port, szCheckNumIP);

#if 0
	if (!pszName || !pszName[0])
	{

	return 0;
	}
#endif

	ENREACH_RETURN((!pszName || !pszName[0]), 0);

	/* 解析域名 */
	pszName0 = pszName;

	for (i = 24; i >= 0; i -= 8)
	{
		while (*pszName0 && (*pszName0 < '0' || *pszName0 > '9'))
		{
			if (*pszName0 == '-' || (*pszName0 >= 'a' && *pszName0 <= 'z')
				|| (*pszName0 >= 'A' && *pszName0 <= 'Z'))
			{
				break;
			}

			pszName0++;
		}

		if (!(*pszName0))
		{
			break;
		}

		tmp = (HI_U32)atoi(pszName0);
		EVM_DEBUG("ip,temp:%u, temp:0x%x", tmp,tmp);

		if (tmp < 0 || tmp > 255)
		{
			break;
		}

		u32Ret |= tmp << i;

		if (!(pszName0 = strchr(pszName0, '.')))
		{
			break;
		}
		
	}

	if (!u32Ret || !((0 == i) && !pszName0))
	{
		if (pu16Port && (pszName0 = strchr(pszName, ':')))
		{
			*pu16Port = (HI_U16)atoi(pszName0 + 1);
			EVM_ERROR("");
		}

		if (szCheckNumIP)
		{
			return 0;
		}

		return Egethostbyname(pszName, pCtrl);
	}

	if (pu16Port && (pszName0 = strchr(pszName, ':')))
	{
		*pu16Port = (HI_U16)atoi(pszName0 + 1);
		EVM_DEBUG("get the port:%d", *pu16Port);
	}

	/* 网络字节转换 */

	EVM_DEBUG("ret:0x%x", u32Ret);
	
	return htonl(u32Ret);
}

/**
 * 获取mac地址
 * 
 * @return unsigned char*
 */
HI_U8 *EnReach_getMACAddress(HI_VOID)
{
    HI_S32 i = 0;
    HI_S32 s32Sock   = 0;
    struct ifreq struIfreq;

    EVM_DEBUG("Enter");

    if ((s32Sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        EVM_ERROR("create socket ERROR");
        return NULL;
    }
    
    strcpy(struIfreq.ifr_name, "eth0");
    memset(g_u8Mac_Address, 0, MAC_ADDR_LEN);

    if (ioctl(s32Sock, SIOCGIFHWADDR, &struIfreq) < 0)
    {
        EVM_ERROR("$$$ erro $$$\r\n");
        close(s32Sock);
        
        return NULL;
    }

    for (i = 0; i < 6; i++)
    {
        sprintf(g_u8Mac_Address + 2 * i, "%02x",
                (HI_U8)struIfreq.ifr_hwaddr.sa_data[i]);
    }
    
    close(s32Sock);
    EVM_DEBUG("get the mac addr :%s", g_u8Mac_Address);

    return g_u8Mac_Address;
}

HI_VOID sock_debug( ESOCK_S *pstruSock ) 
{
    if (NULL == pstruSock)
    {
        return;
    }
    
    HI_CHAR szUdp = GET_TYPE(pstruSock->szTypeOption);
    ntohl(pstruSock->struIP.u32Addr);

    if (!szUdp)
    {
        EVM_ERROR("TCP ");
    }
    else if (ESOCK_UDP == szUdp)
    {
        EVM_ERROR("UDP ");
    }
    else
    {
        EVM_ERROR("RAW ");
    }

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
