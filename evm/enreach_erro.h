/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_erro.h
* Description:
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#ifndef __ENREACH_ERRO_H__
#define __ENREACH_ERRO_H__

#include <stdio.h>
#include <stdlib.h>
#include "hi_type.h"

#if 0
#include "hi_s2_errno.h"
#include "hi_s2_log.h"
#endif


#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ---- enreach使用的打印函数 ---- */
#if 0
#define ENREACH_INFO(pszFormat...)   \
            (HI_VOID)HI_LOG_Outputf(HI_SVR_MODID_NULL, HI_SVR_ERRLEVEL_INFO, HI_SUCCESS, __FILE__, __FUNCTION__, __LINE__, pszFormat)
#define ENREACH_DEBUG(pszFormat...) \
            (HI_VOID)HI_LOG_Outputf(HI_SVR_MODID_NULL, HI_SVR_ERRLEVEL_DEBUG, HI_SUCCESS, __FILE__, __FUNCTION__, __LINE__, pszFormat)
#define ENREACH_ERROR(pszFormat...)   \
            (HI_VOID)HI_LOG_Outputf(HI_SVR_MODID_NULL, HI_SVR_ERRLEVEL_ERROR, (HI_FAILURE), __FILE__, __FUNCTION__, __LINE__, pszFormat)
#define ENREACH_FATAL(enoErrno, pszFormat...)   \
            (HI_VOID)HI_LOG_Outputf(HI_SVR_MODID_NULL, HI_SVR_ERRLEVEL_FATAL, (enoErrno), __FILE__, __FUNCTION__, __LINE__, pszFormat)
#else

//#define EVM_INFO(fmt,arg...)  //do{fmt,##arg;}while(0)
//#define EVM_DEBUG(fmt,arg...) //do{fmt,##arg;}while(0)
//#define EVM_ERROR(fmt,arg...) //do{fmt,##arg;}while(0)

#if 1
#define EVM_INFO(fmt,arg...) \
do { \
    printf("[EVM:%s\\%s:%d]: ", __FILE__, __FUNCTION__, __LINE__);\
    printf(fmt"\n",##arg);\
} while (0);

#define EVM_DEBUG(fmt,arg...) \
do { \
    printf( "\033[1;33m[EVM:%s\\%s:%d]: ", __FILE__,  __FUNCTION__,  __LINE__);\
    printf(fmt "\033[0m\n",##arg);\
} while (0);

#define EVM_ERROR(fmt,arg...)\
do { \
    printf("\033[0;31m[EVM:%s\\%s:%d]: ", __FILE__, __FUNCTION__, __LINE__ );\
    printf(fmt"\033[0m\n",##arg);\
} while (0);	
#endif


#endif


#ifndef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ENREACH_ERRO_H__ */

