/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : audio_type.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006/6/18
Last Modified :
Description   : VBI data type define¡£
Function List :
History       :
 ******************************************************************************/

#ifndef __VBI_TYPE_H__
#define __VBI_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define VBI_CHANNELID_UNKNOWN (30)

typedef enum
{
    VBI_STATUS_INITIAL = 1,
    VBI_STATUS_STOPPED,
    VBI_STATUS_RUNNING,  
    VBI_STATUS_CLOSED,
    VBI_STATUS_UNKNOWN,
} VBI_STATUS_E;

typedef enum
{
    VBI_ENCODE_MODE_PAL = 0,
    VBI_ENCODE_MODE_NTSC,   
    VBI_ENCODE_MODE_UNKNOWN,
} VBI_ENCODE_MODE_E;

typedef enum
{
    VBI_STREAM_SOURCE_DEMUX = 0,   
    VBI_STREAM_SOURCE_MEM,
    VBI_STREAM_SOURCE_UNKNOWN,
} VBI_STREAM_SOURCE_E;

typedef enum 
{
    VBI_TYPE_MACROVISION  = (1),
    VBI_TYPE_TELETEXT     = (1<<1),
    VBI_TYPE_CLOSECAPTION = (1<<2),
    VBI_TYPE_CGMS         = (1<<3),
    VBI_TYPE_WSS          = (1<<4),
    VBI_TYPE_VPS          = (1<<5),
    VBI_TYPE_ALLSUPPORTED = (0x3f),
    VBI_TYPE_UNKNOWN      = (1<<6),
}VBI_TYPE_E;


typedef enum 
{
    VBI_MACROVISION_MODE_VBI_PULSE = (1),
    VBI_MACROVISION_MODE_BACK_PORCH_PULSE = (1<<1),
    VBI_MACROVISION_MODE_COLOR_STRIPE = (1<<2),
    VBI_MACROVISION_MODE_AGC_PULSE = (1<<3),
    VBI_MACROVISION_MODE_SYNC_AMPLITUDE_REDUCTION = (1<<4),
    VBI_MACROVISION_MODE_ALL = (0x1f),
    VBI_MACROVISION_MODE_UNKNOWN = (1 << 5),
}VBI_MACROVISION_MODE;

typedef enum 
{                                                                               /* Outside VBI       Inside VBI      */
    VBI_MACROVISION_REDUCE_MODE_1 = 0,          /*      undo               undo           */
    VBI_MACROVISION_REDUCE_MODE_2,                /*      reduce             undo          */
    VBI_MACROVISION_REDUCE_MODE_3,                /*      reduce             reduce       */
    VBI_MACROVISION_REDUCE_MODE_UNKNOWN,              
}VBI_MACROVISION_REDUCE_MODE;

typedef struct hiVBI_MACROVISION_CONFIG_S
{
    VBI_ENCODE_MODE_E EncodeMode;
    VBI_MACROVISION_MODE ControlMode;
    VBI_MACROVISION_REDUCE_MODE ReduceMode;
}VBI_MACROVISION_CONFIG_S, *PTR_VBI_MACROVISION_CONFIG_S;


typedef struct hiVBI_WRITE_CONFIG_S
{
    HI_U32 SourceAddr;
    HI_U32 CopyLength;
    HI_U8 PacketComplete;
}VBI_WRITE_CONFIG_S, *PTR_VBI_WRITE_CONFIG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_TYPE_H__ */


