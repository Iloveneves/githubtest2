/*******************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_jpg_errcode.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-02-19       z67193    HI_NULL      Create this file.
 ******************************************************************************/
#ifndef __JPGMPI_ERRDEF_H__
#define __JPGMPI_ERRDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/********************************************************
  JPG错误码定义
 *********************************************************/
#define MID_PLAYER 0x26 /*player module ID*/

/*Player API层错误码*/
enum hiPlayer_ErrorCode_E
{
    ERR_PLAYER_PTR_NULL = 1,     /*空指针*/
    ERR_PLAYER_OPENED,           /*播放器已打开*/            
    ERR_PLAYER_NO_OPEN,          /*播放器未打开*/
    ERR_PLAYER_NO_MEM,           /*内存不足*/
    ERR_PLAYER_INVALID_FILE,     /*无效的文件或文件操作错误*/
    ERR_PLAYER_CREATE_DECODE,    /*创建解码器失败*/
    ERR_PLAYER_INVALID_STATUS,   /*错误的状态转换*/
    ERR_PLAYER_OUT_RANGE,        /*超出范围*/
    ERR_PLAYER_NOSUPPORT_FMT,    /*不支持的格式*/
};

/*JPG error code define*/
#define HI_ERR_PLAYER_PTR_NULL\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_PTR_NULL)

#define HI_ERR_PLAYER_OPENED\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_OPENED)

#define HI_ERR_PLAYER_NO_OPEN\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_NO_OPEN)

#define HI_ERR_PLAYER_NO_MEM\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_NO_MEM)

#define HI_ERR_PLAYER_INVALID_FILE\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_INVALID_FILE)

#define HI_ERR_PLAYER_CREATE_DECODE\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_CREATE_DECODE)

#define HI_ERR_PLAYER_INVALID_STATUS\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_INVALID_STATUS)

#define HI_ERR_PLAYER_OUT_RANGE\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_OUT_RANGE)

#define HI_ERR_PLAYER_NOSUPPORT_FMT\
    HI_DEF_ERR(MID_PLAYER, HI_LOG_LEVEL_ERROR, ERR_PLAYER_NOSUPPORT_FMT)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __JPGMPI_ERRDEF_H__ */

