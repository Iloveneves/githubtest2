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
#include <stdarg.h>

#include "enreach_pub.h"
#include "enreach_erro.h"
#include "ebrowser_api.h"

#include "enreach_sock.h"
#include "hi_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

static ESOCK_S g_struSock = {0, };           /* socket 信息 */
#ifndef SOCKET_PRINT
#define SOCKET_PRINT printf
#endif

/**
 * 创建任务
 * 
 * @param pszName      : 任务名称
 * @param pProc        : 任务主函数
 * @param pArgs        : 任务参数
 * @param u32StackSize : 任务参数
 *
 * @return HI_U32
 */
HI_U32 FLS_task_create(HI_CHAR *pszName,
                       HI_VOID *pProc,
                       HI_VOID *pArgs,
                       HI_U32  u32StackSize)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    return HI_TaskCreate(pszName, pProc, pArgs, u32StackSize, 0);
}

/**
 * 任务自杀
 * 
 * @return HI_VOID
 */
HI_VOID FLS__destroyself(HI_VOID)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    HI_TaskDestroySelf();

    return;
}

/**
 * 任务他杀
 * 
 * @param u32Pid : 杀掉任务的pid
 *
 * @return HI_VOID
 */
HI_VOID FLS_task_destroy(HI_U32 u32Pid)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    HI_TaskDestroy(u32Pid);

    return;
}

/**
 * 打印
 * 
 * @param pszFmt : 打印内容
 *
 * @return HI_VOID
 */
HI_VOID Fla_printf(const HI_CHAR *pszFmt, ...)
{
#if 0    
    va_list argp;   
    va_start(argp, pszFmt);   
    vprintf(pszFmt,argp);   
    va_end(argp);
#endif
}

/**
 * 返回当前毫秒数
 * 
 * @return HI_U32
 */
HI_U32 get_tick_count(HI_VOID)
{
    return HI_TaskGetCurMSec();
}

/**
 * 任务sleep
 * 
 * @param u32Delay : 睡眠时间
 *
 * @return HI_VOID
 */
HI_VOID msleep(HI_U32 u32Delay)
{
    /* u32Delay ms */

    usleep(u32Delay * 1000);
}

/**
 * 通知机顶盒开始进入FLASH
 *
 * @return HI_VOID
 */
HI_VOID EnReach_Fls_Start(HI_VOID)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    return;
}

/**
 * 通知机顶盒开始退出FLASH
 *
 * @return HI_VOID
 */
HI_VOID EnReach_Fls_Finish(HI_VOID)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    return;
}

/**
 * 机顶盒与指定的URL建立连接
 *
 * @param pszUrl  : 要建立的地址
 * @param s32Port : 建立的端口号
 *
 * @return HI_S32
 */
HI_S32 Fls_Sock_Open(HI_CHAR *pszUrl , HI_S32 s32Port)
{
    ESOCKCTRL_S struSockCtrl;
    HI_U16  u16Port;
    HI_S32  s32Sock = 0;

    SOCKET_PRINT("$$$ call [%s] [%d] function, purl = %s, port = %d! $$$\r\n",
                 __FUNCTION__, __LINE__, pszUrl, s32Port);

    g_struSock.szTypeOption   = 0;
    g_struSock.struIP.u16Port = (HI_U16)s32Port;
    g_struSock.struIP.u32Addr = sock_getIpAddr(pszUrl, &struSockCtrl, &u16Port, 0);

    s32Sock = sock_open(&g_struSock);

    SOCKET_PRINT("$$$ call [%s] [%d] function, purl = %s, port = %d, socke id = %d ! $$$\r\n",
                 __FUNCTION__, __LINE__, pszUrl, s32Port, s32Sock);

    if (0 > s32Sock)
    {
        /* Sock创建失败 */
        return 0;
    }

    return s32Sock;
}

/**
 * 通知机顶盒FLASH启动网络功能
 *
 * @return HI_VOID
 */
HI_VOID Fls_SockStart(HI_VOID)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    return;
}

/**
 * 通知机顶盒FLASH退出网络功能
 *
 * @return HI_VOID
 */
HI_VOID Fls_SockStop(HI_VOID)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    return;
}

/**
 * 打印
 *
 * @param pszFmt  : 打印内容
 *
 * @return HI_VOID
 */
HI_VOID Rec_printf(const HI_CHAR *pszFmt, ...)
{
#if 0    
    va_list argp;   
    va_start(argp, pszFmt);   
    vprintf(pszFmt,argp);   
    va_end(argp);
#endif
}

/**
 * 机顶盒发送数据
 *
 * @param s32Sock  : sock id
 * @param pszBuf   : buff地址
 * @param s32Len   : buff长度
 *
 * @return HI_S32
 */
HI_S32 Fls_Sock_Send(HI_S32 s32Sock, HI_CHAR *pszBuf , HI_S32 s32Len)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    
    return sock_send(&g_struSock, pszBuf, s32Len);
}

/**
 * 机顶盒接受数据
 *
 * @param s32Sock  : sock id
 * @param pszBuf   : buff地址
 * @param s32Len   : buff长度
 *
 * @return HI_S32
 */
HI_S32 Fls_Sock_Recv(HI_S32 s32Sock, HI_CHAR *pszBuf , HI_S32 s32Len)
{
    SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                 __FUNCTION__, __LINE__);
    
    return sock_recv(&g_struSock, pszBuf, s32Len);
}

/**
 * 关闭socket
 *
 * @param s32Sock  : sock id
 *
 * @return HI_S32
 */
HI_S32 Fls_Closesocket (HI_S32 s32Sock)
{
    HI_S32 s32Ret = 0;

    SOCKET_PRINT("$$$ call [%s] [%d] function, sock = %d! $$$\r\n",
                 __FUNCTION__, __LINE__, s32Sock);

    if (0 > s32Sock)
    {
        
        SOCKET_PRINT("$$$ call [%s] [%d] function, err sock = %d! $$$\r\n",
                     __FUNCTION__, __LINE__, s32Sock);
        return 1;
    }
    
    s32Ret = sock_close(&g_struSock);

    if (0 != s32Ret)
    {
        SOCKET_PRINT("$$$ call [%s] [%d] function! $$$\r\n",
                     __FUNCTION__, __LINE__);
        return 1;
    }

    return s32Ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

