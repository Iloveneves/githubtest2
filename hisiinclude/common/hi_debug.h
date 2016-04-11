/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_debug.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/6/17
Last Modified :
Description   : .....
Function List :
History       :
 ******************************************************************************/
#ifndef __HI_DEBUG_H__
#define __HI_DEBUG_H__

#include "hi_type.h"
#include <linux/kernel.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef __KERNEL__
extern HI_U32 g_u32DebugLevel;
extern HI_U32 g_u32LogModule;
#endif

typedef enum 
{
    AUDIO_ISR,
    VIDEO_ISR,
    DEMUX_ISR,
    SYNC_ISR,
    VO_ISR,
    TDE_ISR,


    
    MPI_ISR,
}MPI_ISR_E;

typedef enum hiHI311X_MODULE_E
{
    DEMUX       = 0x01,
    SYNC        = 0x02,
    VIDEO       = 0x04,
    AUDIO       = 0x08,
    VO          = 0x10,
    VBI         = 0x20, 
    TDE         = 0x40,
    JPEG        = 0x80,
    PLAYER      = 0x100
}HI311X_MODULE_E;

#ifndef __HI_LOG_LEVEL_E__
#define __HI_LOG_LEVEL_E__


typedef enum hiLOG_ERRLEVEL_E
{
    HI_LOG_LEVEL_DEBUG   = 0,               /* debug-level                                  */
    HI_LOG_LEVEL_INFO ,                     /* informational                                */
    HI_LOG_LEVEL_NOTICE,                    /* normal but significant condition             */
    HI_LOG_LEVEL_WARNING,                   /* warning conditions                           */
    HI_LOG_LEVEL_ERROR,                     /* error conditions                             */
    HI_LOG_LEVEL_CRIT,                      /* critical conditions                          */
    HI_LOG_LEVEL_ALERT,                     /* action must be taken immediately             */
    HI_LOG_LEVEL_FATAL,                     /* just for compatibility with previous version */
    HI_LOG_LEVEL_BUTT
} LOG_ERRLEVEL_E;

/******************************************************************************
  |----------------------------------------------------------------| 
  | 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            | 
  |----------------------------------------------------------------| 
  |<--><--7bits----><----8bits---><--3bits---><------13bits------->|
 ******************************************************************************/
#define HI_ERR_APPID  (0x80UL + 0x20UL)

#define HI_DEF_ERR( mid, level, errid) \
    ((HI_S32)( ((HI_ERR_APPID)<<24) | ((mid) << 16 ) | ((level)<<13) | (errid) ))

#endif

/********************************************************
  ¹«¹²´íÎóÂë¶¨Òå
 *********************************************************/
#define HI311XMPD 0x88                      /*HI311X_MPD*/

enum HI311XMPD_ErrorCode_E
{
    ERR_HI311XMPD_CODE_UNDEF,               /*Î´¶¨ÒåÃüÁîÂë*/
    ERR_HI311XMPD_PTR_NULL,                 /*Ö¸Õë²ÎÊýÎª¿Õ*/
    ERR_HI311XMPD_NO_OPEN,                  /*Éè±¸Ã»ÓÐ´ò¿ª*/
    ERR_HI311XMPD_OPENED,                   /*Éè±¸ÒÑ¾­´ò¿ª*/
    ERR_HI311XMPD_CLOSED,                   /*Éè±¸ÒÑ¾­¹Ø±Õ*/

    /* New */
    ERR_HI311XMPD_INVALID_DEVICE,           /* invalid device */
    ERR_HI311XMPD_INVALID_HANDLE,
    ERR_HI311XMPD_INVALID_PARAMETER,        /* invalid parameter */
    ERR_HI311XMPD_INVALID_OPERATION,
    ERR_HI311XMPD_ADDR_FAULT,               /* address fault */
    ERR_HI311XMPD_NO_MEM                    /* mem alloc fail */
};

#define HI_ERR_HI311XMPD_CODE_UNDEF\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_CODE_UNDEF)

#define HI_ERR_HI311XMPD_PTR_NULL\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_PTR_NULL)

#define HI_ERR_HI311XMPD_OPENED\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_OPENED)

#define HI_ERR_HI311XMPD_CLOSED\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_CLOSED)

#define HI_ERR_HI311XMPD_NO_OPEN\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_NO_OPEN)

/****************************** New *************************/
#define HI_ERR_HI311XMPD_INVALID_DEVICE\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_INVALID_DEVICE)

#define HI_ERR_HI311XMPD_INVALID_HANDLE\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_INVALID_HANDLE)

#define HI_ERR_HI311XMPD_INVALID_PARAMETER\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_INVALID_PARAMETER)

#define HI_ERR_HI311XMPD_INVALID_OPERATION\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_INVALID_OPERATION)

#define HI_ERR_HI311XMPD_ADDR_FAULT\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_ADDR_FAULT)

#define HI_ERR_HI311XMPD_NO_MEM\
    HI_DEF_ERR(HI311XMPD, HI_LOG_LEVEL_ERROR, ERR_HI311XMPD_NO_MEM)

#define HI_DEBUG
#ifdef HI_DEBUG
#ifdef __KERNEL__
#define HI_TRACE( level, module, fmt, args... )\
 do { \
        if (module & g_u32LogModule) \
        { \
            printk("<%d>""%s() %d: "fmt, (HI_LOG_LEVEL_FATAL-level), __FUNCTION__,  __LINE__ , ##args);\
        }\
    } while (0)
#else
#define HI_TRACE( level, module, fmt, args... ) \
do { \
		 printf( "%s-%s(): %d Line\n: ", __FILE__,  __FUNCTION__,  __LINE__);\
		 printf(fmt, ##args); \
} while (0)
#endif

#define HI_FUNCTION_TRACE(module) HI_TRACE(HI_LOG_LEVEL_INFO, module, " ")

#define ParamCheck(mode, param, bottom, errorcode)\
    do{\
        if((HI_U32)param >= bottom)\
        {\
            HI_TRACE(HI_LOG_LEVEL_ERROR, mode,"Parameter error!\n");\
            return errorcode;\
        }\
    }while(0)

#define NullPointerCheck(mode, pointer) \
    do{\
        if(HI_NULL == pointer)\
        {\
            HI_TRACE(HI_LOG_LEVEL_ERROR, mode, "Audio null pointer\n");\
            return HI_ERR_HI311XMPD_PTR_NULL;\
        }\
    }while(0)

#else
#define HI_TRACE( level, module, fmt, args... )
#define HI_FUNCTION_TARCE(module)
//#define ParamCheck(mode, Param, Bottom, ErrorCode)
#define NullPointerCheck(mode, Pointer) 
#endif

#define HI311X_WRITE_REG(base, offset, value) \
    (*(volatile unsigned int   *)((unsigned int)(base) + (offset)) = (value))

#define HI311X_READ_REG(base, offset) \
    (*(volatile unsigned int   *)((unsigned int)(base) + (offset)))



/****************************add by qianli*********************************/
#ifdef __KERNEL__
    
#endif
        
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEBUG_H__ */


