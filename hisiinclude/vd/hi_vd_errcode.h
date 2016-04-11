/***********************************************************************************
*             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   hi_vd_errcode.h
* Description:errorcode defination of video module
*
* History:
* Version   Date              Author     DefectNum    Description
* main\1    2008-03-28        z67193     NULL         Create this file.
***********************************************************************************/
 
#ifndef __HI_VD_ERRCODE_H__
#define __HI_VD_ERRCODE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/********************************************************
  VIDEO errorcode defination
 *********************************************************/

#define MID_VD 0x24/*VIDEO MODULE ID*/

typedef enum hiVd_ErrorCode 
{
    ERR_VD_NOT_INIT                             = 0x80,
    ERR_VD_HAS_INIT                             = 0x81,
    ERR_VD_INVALID_FRAME_SOURCE                 = 0x82,
    ERR_VD_FRAME_SIZE                           = 0x83,
    ERR_VD_INVALID_COMPRESS_TYPE                = 0x84,
    ERR_VD_INVALID_FILTER                       = 0x85,
    ERR_VD_INVALID_DISPLAY_SEQUENCE             = 0x86,
    ERR_VD_INVALID_ASPECT_RATIO                 = 0x87,
    ERR_VD_INVALID_ENCODE_FORMAT                = 0x88,
    ERR_VD_INVALID_PIC_STRUCTURE                = 0x89,
    ERR_VD_INVALID_COLORFORMAT                  = 0x8a,
    ERR_VD_NOT_ALIGN                            = 0x8b,
    ERR_VD_STRIDE_TOO_SMALL                     = 0x8c,
    ERR_VD_FRAME_SOURCE                         = 0x8d,
    ERR_VD_INVALID_ASPECT_CONVERSION_MODE       = 0x8e,
    ERR_VD_INVALID_INPUT_WINDOW                 = 0x8f,
    ERR_VD_INVALID_OUTPUT_WINDOW                = 0x90,
    ERR_VD_INVALID_CAPTURE_PARAMETER            = 0x91,
    ERR_VD_NO_DISP_FRAME                        = 0x92,
    ERR_VD_VDEC_NOT_OPEN                        = 0x93,
    ERR_VD_INV_HANDLE                           = 0x94,
    ERR_VD_INV_PARA                             = 0x95,
    ERR_VD_NULL_PTR                             = 0x96,
    ERR_VD_USED                                 = 0x97,
    ERR_VD_NOT_SUPPORT                          = 0x98,
} VD_ERRORCODE_E;


#define HI_ERR_VD_NOT_INIT\
    HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_NOT_INIT)

#define HI_ERR_VD_HAS_INIT\
    HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_HAS_INIT)

#define HI_ERR_VD_INVALID_FRAME_SOURCE\
    HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_FRAME_SOURCE)

#define HI_ERR_VD_FRAME_SIZE\
    HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_FRAME_SIZE)

#define HI_ERR_VD_INVALID_COMPRESS_TYPE\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_COMPRESS_TYPE)

#define HI_ERR_VD_INVALID_FILTER\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_FILTER)

#define HI_ERR_VD_INVALID_DISPLAY_SEQUENCE\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_DISPLAY_SEQUENCE)

#define HI_ERR_VD_INVALID_ASPECT_RATIO\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_ASPECT_RATIO)

#define HI_ERR_VD_INVALID_ENCODE_FORMAT\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_ENCODE_FORMAT)

#define HI_ERR_VD_INVALID_PIC_STRUCTURE\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_PIC_STRUCTURE)

#define HI_ERR_VD_INVALID_COLORFORMAT\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_COLORFORMAT)

#define HI_ERR_VD_NOT_ALIGN\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_NOT_ALIGN)

#define HI_ERR_VD_STRIDE_TOO_SMALL\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_STRIDE_TOO_SMALL)

#define HI_ERR_VD_FRAME_SOURCE\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_FRAME_SOURCE)

#define HI_ERR_VD_NO_DISP_FRAME\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_NO_DISP_FRAME)

#define HI_ERR_VD_INVALID_ASPECT_CONVERSION_MODE\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_ASPECT_CONVERSION_MODE)

#define HI_ERR_VD_INVALID_INPUT_WINDOW\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_INPUT_WINDOW)

#define HI_ERR_VD_INVALID_OUTPUT_WINDOW\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_OUTPUT_WINDOW)

#define HI_ERR_VD_INVALID_CAPTURE_PARAMETER\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INVALID_CAPTURE_PARAMETER)

#define HI_ERR_VD_VDEC_NOT_OPEN\
        HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_VDEC_NOT_OPEN)

#define HI_ERR_VD_INV_HANDLE HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INV_HANDLE)
#define HI_ERR_VD_INV_PARA HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_INV_PARA)
#define HI_ERR_VD_NULL_PTR HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_NULL_PTR)
#define HI_ERR_VD_USED HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_USED)
#define HI_ERR_VD_NOTSUPPORT HI_DEF_ERR(MID_VD, HI_LOG_LEVEL_ERROR, ERR_VD_NOT_SUPPORT)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_ERRDEF_H__ */


