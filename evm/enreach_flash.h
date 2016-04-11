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
 
#ifndef __ENREACH_FLASH_H__
#define __ENREACH_FLASH_H__

#include "hi_type.h"
#include "eBrowser_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* 给flash分配一块独立内存,20M */
HI_S32 eBrowser_flash_setMemory(eBrowserHandler* pThis,
                                HI_VOID *pBuffer,
                                HI_U32 u32Size);

/* flash的版本号 */
HI_CHAR *getFlashVersion(HI_VOID);

/* 创建任务 */
HI_U32 FLS_task_create(HI_CHAR *pszName,
                       HI_VOID *pProc,
                       HI_VOID *pArgs,
                       HI_U32  u32StackSize);

/* 任务自杀 */
HI_VOID FLS__destroyself(HI_VOID);

/* 任务他杀 */
HI_VOID FLS_task_destroy(HI_U32 u32Pid);

/* 打印 */
HI_VOID Fla_printf(const HI_CHAR *pszFmt, ...);

/* 返回当前毫秒数 */
HI_U32 get_tick_count(HI_VOID);

/* 任务sleep */
HI_VOID msleep(HI_U32 u32Delay);

/* 通知机顶盒开始进入FLASH，机顶盒根据需要，实现此函数 */
HI_VOID EnReach_Fls_Start(HI_VOID);

/* 通知机顶盒退出FLASH，机顶盒根据需要，实现此函数 */
HI_VOID EnReach_Fls_Finish(HI_VOID);

/* 机顶盒与指定的URL建立连接 */
HI_S32 Fls_Sock_Open(HI_CHAR *pszUrl , HI_S32 s32Port);

/* 通知机顶盒FLASH启动网络功能，机顶盒根据需要，实现此函数 */
HI_VOID Fls_SockStart(HI_VOID);

/* 通知机顶盒FLASH退出网络，机顶盒根据需要，实现此函数 */
HI_VOID Fls_SockStop(HI_VOID);

/* 打印 */
HI_VOID Rec_printf(const HI_CHAR *pszFmt, ...);

/* 机顶盒发送数据 */
HI_S32 Fls_Sock_Send(HI_S32 s32Sock, HI_CHAR *pszBuf , HI_S32 s32Len);

/* 机顶盒接受数据 */
HI_S32 Fls_Sock_Recv(HI_S32 s32Sock, HI_CHAR *pszBuf , HI_S32 s32Len);

/* 关闭socket */
HI_S32 Fls_Closesocket (HI_S32 s32Sock);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
