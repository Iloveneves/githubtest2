/*******************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: jpg_osr.h
 * Description: jpg驱动操作系统相关层头文件.
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-04-07       d37024    HI_NULL      Create this file.
 ******************************************************************************/

#ifndef __HI_JPG_OSR_H__
#define __HI_JPG_OSR_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include "hi_type.h"

#define JPG_CMD_MAGIC     'j' 

#define CMD_JPG_GETDEVICE        _IO( JPG_CMD_MAGIC, 0x0)
#define CMD_JPG_RELEASEDEVICE    _IO( JPG_CMD_MAGIC, 0x1)
#define CMD_JPG_GETINTSTATUS     _IOWR( JPG_CMD_MAGIC, 0x2, JPG_GETINTTYPE_S *)
#define CMD_JPG_SETVO            _IOWR( JPG_CMD_MAGIC, 0x3, JPG_VOSET_S *)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


