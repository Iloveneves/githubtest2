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
 
#ifndef __ENREACH_SOCK_H__
#define __ENREACH_SOCK_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define SOCK_CONNECT_SEC    5
#define SOCK_CONNECT_TIME   3
//这个时间在枣庄是1秒
#define TCP_SELECT_SEC      2
#define TCP_SELECT_MSEC	    0
#define TCP_SELECT_RETRY    10
#define UDP_SELECT_SEC      1
#define UDP_SELECT_MSEC     0
#define UDP_SELECT_RETRY    5
#define SOCK_SEND_MAXSIZE   0x8000
#define SOCK_RECV_MAXSIZE   0x8000
#define CONNECT_NONBLOCK

/* enum ESOCK_OPTION */

#define	GET_TYPE(a)         ((a) & 0x7)
#define	ISUSEBIND(a)        ((a) & 0x38)
#define	SETUSEBIND(a)       (a) = ((a) | 0x08)
#define	CLRUSEBIND(a)       (a) = ((a) & 0xf7)
#define	ISUSEMULTICAST(a)   ((a) & 0x30)
#define	ISRECVMULTICAST(a)  ((a) & 0x10)
#define	SETRECVMULTICAST(a) (a) = ((a) | 0x10)
#define	CLRRECVMULTICAST(a) (a) = ((a) & 0xef)
#define	ISSENDMULTICAST(a)  ((a) & 0x20)
#define	SETSENDMULTICAST(a) (a) = ((a) | 0x20)
#define	CLRSENDMULTICAST(a) (a) = ((a) & 0xdf)
#define	ISUSESENDTO(a)      ((a) & 0x40)
#define	SETUSESENDTO(a)     (a) = ((a) | 0x40)
#define	CLRUSESENDTO(a)     (a) = ((a) & 0xbf)
#define	ISUSERECVFROM(a)    ((a) & 0x80)
#define	SETUSERECVFROM(a)   (a) = ((a) | 0x80)
#define	CLRUSERECVFROM(a)   (a) = ((a) & 0x7f)

typedef enum tagRET_STATUS_E
{
     BIND_ERROR     = -9,
     CONNECT_ERROR,
     TIMEOUT_ERROR,
     TCPIP_ERROR,
     SSL_ERROR,
     HTTP_ERROR,
     STOP_ERROR,
     STORE_ERROR,
     ADDR_ERROR     = -1
} RET_STATUS_E;

typedef enum tagCTRL_STATUS_E
{
    EMPTY,
    START,
    NET_RESET,
    ABORT
} CTRL_STATUS_E;

typedef enum tagESOCK_TYPE_E
{
    ESOCK_TCP,
    ESOCK_UDP,
    ESOCK_RAW
} ESOCK_TYPE_E;

typedef struct tagESOCKCTRL_S
{
    HI_U32  u32TimeOutSec;       /* Maximum time for select to wait second */
    HI_U32  u32TimeOutMSec;      /* Maximum time for select to wait microsecond */
    HI_U32  u32TimeOutTry;       /* time out retry */
    HI_CHAR *pCtrlStatus;        /* point to CTRL_STATUS */
} ESOCKCTRL_S;

typedef struct tagEIP_S
{
    HI_U32  u32Addr;             /* network order, 12.34.56.78 for big endian 
                                    is 0x12345678 for little endian is 0x78563412 */
    HI_U16  u16Port;
} EIP_S;

typedef struct tagESOCK_S
{
    HI_S32      s32Socket;
    HI_U32      u32SendSz;       /* total per-socket buffer space reserved for receives */
    HI_U32      u32RecvSz;       /* total per-socket buffer space reserved for sends */
    ESOCKCTRL_S *pCtrl;
    EIP_S       struIP;
    EIP_S       struFromIP;
    HI_CHAR     szTypeOption;    /* point to ESOCK_TYPE/ESOCK_OPTION */
} ESOCK_S;

HI_VOID sock_debug( ESOCK_S *pstruSock );
HI_VOID sock_start( HI_VOID );
HI_VOID sock_stop( HI_VOID);
HI_S32  sock_open( ESOCK_S *pstruSock );
HI_S32  sock_send( ESOCK_S *pstruSock, HI_CHAR *pszBuf, HI_S32 s32Len );
HI_S32  sock_recv( ESOCK_S *pstruSock, HI_CHAR *pszBuf, HI_S32 s32Len );
HI_S32  sock_close( ESOCK_S *pstruSock );
HI_U32  sock_getIpAddr( HI_CHAR *pszName, ESOCKCTRL_S *pCtrl,
                        HI_U16 *pu16Port, HI_CHAR szCheckNumIP );
HI_U8   *EnReach_getMACAddress( HI_VOID );

#if 0
HI_S32 sock_ethernetIPAddressSet(HI_CHAR *ipAdd);
HI_S32 sock_ethernetIPAddressGet(HI_CHAR *ipAdd);
HI_S32 sock_ethernetSubNetmaskSet(HI_CHAR *subNetmask);
HI_S32 sock_ethernetSubNetmaskGet(HI_CHAR *subNetmask);
HI_S32 sock_ethernetGatewaySet(HI_CHAR *gateway);
HI_S32 sock_ethernetGatewayGet(HI_CHAR *gateway);
#endif

/* 退出浏览器前，先停止sock接收，防止超时过长 */
HI_VOID sock_stopRecv();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
