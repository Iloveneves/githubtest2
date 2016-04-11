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

/* ��flash����һ������ڴ�,20M */
HI_S32 eBrowser_flash_setMemory(eBrowserHandler* pThis,
                                HI_VOID *pBuffer,
                                HI_U32 u32Size);

/* flash�İ汾�� */
HI_CHAR *getFlashVersion(HI_VOID);

/* �������� */
HI_U32 FLS_task_create(HI_CHAR *pszName,
                       HI_VOID *pProc,
                       HI_VOID *pArgs,
                       HI_U32  u32StackSize);

/* ������ɱ */
HI_VOID FLS__destroyself(HI_VOID);

/* ������ɱ */
HI_VOID FLS_task_destroy(HI_U32 u32Pid);

/* ��ӡ */
HI_VOID Fla_printf(const HI_CHAR *pszFmt, ...);

/* ���ص�ǰ������ */
HI_U32 get_tick_count(HI_VOID);

/* ����sleep */
HI_VOID msleep(HI_U32 u32Delay);

/* ֪ͨ�����п�ʼ����FLASH�������и�����Ҫ��ʵ�ִ˺��� */
HI_VOID EnReach_Fls_Start(HI_VOID);

/* ֪ͨ�������˳�FLASH�������и�����Ҫ��ʵ�ִ˺��� */
HI_VOID EnReach_Fls_Finish(HI_VOID);

/* ��������ָ����URL�������� */
HI_S32 Fls_Sock_Open(HI_CHAR *pszUrl , HI_S32 s32Port);

/* ֪ͨ������FLASH�������繦�ܣ������и�����Ҫ��ʵ�ִ˺��� */
HI_VOID Fls_SockStart(HI_VOID);

/* ֪ͨ������FLASH�˳����磬�����и�����Ҫ��ʵ�ִ˺��� */
HI_VOID Fls_SockStop(HI_VOID);

/* ��ӡ */
HI_VOID Rec_printf(const HI_CHAR *pszFmt, ...);

/* �����з������� */
HI_S32 Fls_Sock_Send(HI_S32 s32Sock, HI_CHAR *pszBuf , HI_S32 s32Len);

/* �����н������� */
HI_S32 Fls_Sock_Recv(HI_S32 s32Sock, HI_CHAR *pszBuf , HI_S32 s32Len);

/* �ر�socket */
HI_S32 Fls_Closesocket (HI_S32 s32Sock);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
