/***********************************************************************************
*             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   hi_video_errcode.h
* Description:errorcode defination of video module
*
* History:
* Version   Date              Author     DefectNum    Description
* main\1    2007-07-18        t52067     NULL         Create this file.
***********************************************************************************/
 
#ifndef __HI_VIDEO_ERRCODE_H__
#define __HI_VIDEO_ERRCODE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/********************************************************
  VIDEO errorcode defination
 *********************************************************/

#define MID_VIDEO 0x22/*VIDEO MODULE ID*/

typedef enum hiVideo_ErrorCode 
{
    ERR_VID_ES_BUFFER_FULL                 = 0x00, /* es buffer is full */
    ERR_VID_NO_PARSED_PARAMETERS           = 0x01, /* the bitstream parameters has not been parsed */
    ERR_VID_NO_DISPLAY_FRAME               = 0x02, /* framebuffer alloc fail */

    ERR_VID_NO_OPEN                        = 0x03, /*video device file not open*/
    ERR_VID_OPENED                         = 0x04, /*video device has been opened*/
    ERR_VID_INVALID_DEVICE                 = 0x05, /*invalid video device*/
    ERR_VID_INVALID_OPERATION              = 0x06, /*invalid video device operation*/
    ERR_VID_PTR_NULL                       = 0x07, /*pointer null error*/
    ERR_VID_NO_FRAMEBUFFER                 = 0x08, /* framebuffer alloc fail */
    ERR_VID_INVALID_TRANSFER               = 0x09, /* decoder state transfer error */
    ERR_VID_INVALID_FLUSH_MODE             = 0x10, /* invalid video flush mode*/
    ERR_VID_INVALID_STREAM_SOURCE          = 0x12, /* invalid video stream source type*/
    ERR_VID_OPEN_CHAN                      = 0x14, /* video channel was opened*/
    ERR_VID_INVALID_ADDR_TYPE              = 0x17, /* invalid video address type in memory copy */
    ERR_VID_SYNC_PALYBACK                  = 0x18,           /* AI7D01991  */
    ERR_VID_ABSTRACT_USER_DATA_TIMEOUT     = 0x19,
    ERR_VID_INVALID_PARA                   = 0x1A,
    ERR_VID_DECODE_FAIL                    = 0x1B
} VIDEO_ERRORCODE_E;


#define HI_ERR_VID_ES_BUFFER_FULL\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_ES_BUFFER_FULL)

#define HI_ERR_VID_NO_PARSED_PARAMETERS\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_NO_PARSED_PARAMETERS)

#define HI_ERR_VID_NO_DISPLAY_FRAME\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_NO_DISPLAY_FRAME)

#define HI_ERR_VID_NO_OPEN\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_NO_OPEN)

#define HI_ERR_VID_OPENED\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_OPENED)

#define HI_ERR_VID_INVALID_DEVICE\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_DEVICE)

#define HI_ERR_VID_INVALID_OPERATION\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_OPERATION)

#define HI_ERR_VID_PTR_NULL\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_PTR_NULL)

#define HI_ERR_VID_NO_FRAMEBUFFER\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_NO_FRAMEBUFFER)

#define HI_ERR_VID_INVALID_TRANSFER\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_TRANSFER)

#define HI_ERR_VID_INVALID_FLUSH_MODE\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_FLUSH_MODE)

#define HI_ERR_VID_INVALID_STREAM_SOURCE\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_STREAM_SOURCE)

#define HI_ERR_VID_OPEN_CHAN\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_OPEN_CHAN)

#define HI_ERR_VID_INVALID_ADDR_TYPE\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_ADDR_TYPE)

#define HI_ERR_VID_SYNC_PALYBACK\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_SYNC_PALYBACK)  /* AI7D01991  */

#define HI_ERR_VID_ABSTRACT_USER_DATA_TIMEOUT\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_ABSTRACT_USER_DATA_TIMEOUT)  /* AI7D01991  */

#define HI_ERR_VID_INVALID_PARA\
        HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_INVALID_PARA)

#define HI_ERR_VID_DECODE_FAIL\
    HI_DEF_ERR(MID_VIDEO, HI_LOG_LEVEL_ERROR, ERR_VID_DECODE_FAIL)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_ERRDEF_H__ */

