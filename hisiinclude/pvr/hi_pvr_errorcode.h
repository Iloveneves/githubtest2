/******************************************************************************
 
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 
 ******************************************************************************
  File Name     : hi_pvr_errorcode.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   : hi_pvr_errorcode.h header file
  History       :
  1.Date        : 2008/04/24
    Author      : q46153
    Modification: Created file
 
******************************************************************************/

#ifndef __HI_PVR_ERRORCODE_H__
#define __HI_PVR_ERRORCODE_H__

#include "hi_type.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

#if 0
#define HI_ERR_APPID  (0x80UL + 0x20UL)

/******************************************************************************
|----------------------------------------------------------------| 
| 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            | 
|----------------------------------------------------------------| 
|<--><--7bits----><----8bits---><--3bits---><------13bits------->|
******************************************************************************/

//#define PVR_MTT_UT

#ifndef PVR_MTT_UT
#define HI_DEF_ERR( module, level, errid) \
    ((HI_S32)( (HI_ERR_APPID << 24) | ((module) << 16 ) | ((level)<<13) | (errid) ))
#else
#define HI_DEF_ERR( module, level, errid) (errid)
#endif




/********************************************************
  PVR  error code definition
 *********************************************************/
#define HI_LOG_LEVEL_ERROR 0x4
#endif

#define MID_PVR                                (0x05)        /* PVR  module ID*/

/***** common error area *****/
#define HI_ERR_PVR_NUL_PTR          HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x01)
#define HI_ERR_PVR_INVAL_CHNID      HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x02)
#define HI_ERR_PVR_NOT_INIT         HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x03)
#define HI_ERR_PVR_NO_CHN_LEFT      HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x04)
#define HI_ERR_PVR_ALREADY          HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x05)
#define HI_ERR_PVR_BUSY             HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x06)
#define HI_ERR_PVR_CHN_NOT_INIT     HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x07)
#define HI_ERR_PVR_NO_MEM           HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x08)
#define HI_ERR_PVR_STRM_TYPE_ERR    HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x09)
#define HI_ERR_PVR_INVAL_PARAM      HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x0A)

/***** file error area *****/
#define HI_ERR_PVR_FILE_EXIST       HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x11)
#define HI_ERR_PVR_FILE_NOT_EXIST   HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x12)
#define HI_ERR_PVR_FILE_CANT_OPEN   HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x13)
#define HI_ERR_PVR_FILE_CANT_CLOSE  HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x14)
#define HI_ERR_PVR_FILE_CANT_SEEK   HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x15)
#define HI_ERR_PVR_FILE_CANT_WRITE  HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x16)
#define HI_ERR_PVR_FILE_CANT_READ   HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x17)
#define HI_ERR_PVR_FILE_INVAL_FNAME HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x18)
#define HI_ERR_PVR_FILE_TILL_START  HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x19)
#define HI_ERR_PVR_FILE_TILL_END    HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x1A)
#define HI_ERR_PVR_FILE_DISC_FULL   HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x1B)

/***** record error area *****/
#define HI_ERR_PVR_REC_INVAL_STATE  HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x21)
#define HI_ERR_PVR_REC_INVAL_DMXID  HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x22)
#define HI_ERR_PVR_REC_INVAL_FSIZE  HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x23)
#define HI_ERR_PVR_REC_INVAL_UDSIZE HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x24)

/***** play error area *****/
#define HI_ERR_PVR_PLAY_INVAL_STATE HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x31)
#define HI_ERR_PVR_PLAY_INVAL_DMXID HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x32)
#define HI_ERR_PVR_PLAY_NOT_SUPPORT HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x33)
#define HI_ERR_PVR_PLAY_RETRY       HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x34)

/***** index error area *****/
#define HI_ERR_PVR_INDEX_CANT_MKIDX HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x41)
#define HI_ERR_PVR_INDEX_FORMAT_ERR HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x42)

/***** interface error area *****/
#define HI_ERR_PVR_INTF_EVENT_INVAL HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x51)
#define HI_ERR_PVR_INTF_EVENT_NOREG HI_DEF_ERR(MID_PVR, HI_LOG_LEVEL_ERROR, 0x52)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __HI_PVR_ERRORCODE_H__ */

