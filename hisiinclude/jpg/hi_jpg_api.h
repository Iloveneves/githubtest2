/*******************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_jpg_api.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-02-19       z67193    HI_NULL      Create this file.
 ******************************************************************************/
#ifndef _JPG_API_H_
#define _JPG_API_H_

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_jpg_errcode.h"
#include "hi_jpg_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/****************************************************************************
*                   JPEG API 函数声明                                       *
****************************************************************************/

#define JPG_INVALID_HANDLE 0xFFFFFFFF

/*handle type*/


/*JPG输入流位置基准*/
typedef enum hiJPG_POSITION_E
{
    JPG_POSITION_SET = 0, /*起始位置*/
    JPG_POSITION_CUR,     /*从当前位置*/
    JPG_POSITION_END,     /*从结束位置*/
    JPG_POSITION_BUTT
}JPG_POSITION_E;

/*输入源类型*/
typedef enum hiJPG_SRCTYPE_E
{
    JPG_SRCTYPE_FILE = 0, /*文件*/
    JPG_SRCTYPE_MEM,      /*内存*/
    JPG_SRCTYPE_STREAM,   /*流式*/
    JPG_SRCTYPE_BUTT
} JPG_SRCTYPE_E;

/*解码或解析结果*/
typedef enum hiJPG_RESULT_E
{
    JPG_RESULT_IDLE = 0,   /*未运行*/
    JPG_RESULT_FINISH,     /*完成*/
    JPG_RESULT_FAILURE,    /*失败*/
    JPG_RESULT_RUNNING,    /*进行中*/
    JPG_RESULT_BUTT
} JPG_RESULT_E;

/*各种输入源详细信息，对于流式来说，该信息无效*/
typedef union hiJPG_SRCINFO_U
{
    struct 
    {
        HI_CHAR* pAddr;     /*内存指针地址*/        
        HI_U32   Length;    /*长度*/
    }MemInfo;               /*输入源为内存块时需要的信息*/

    HI_CHAR *pFileName;     /*输入源为文件时需要的信息:文件名*/
}JPG_SRCINFO_U;

/*输入源描述信息*/
typedef struct hiJPG_SRCDESC_S
{
    JPG_IMGTYPE_E ImgType; /*JPEG图片类型*/
    JPG_SRCTYPE_E SrcType; /*输入源类型*/
    JPG_SRCINFO_U SrcInfo; /*输入源的详细信息*/
} JPG_SRCDESC_S;

/*输入流的回调函数，上层软件在流式输入情况下应该实现此函数，
  在此函数中送入码流，其它输入情况下不用实现此函数*/
typedef HI_S32 (*PTR_JPG_WriteStreamCB_FN)(JPG_POSITION_E Position, 
                                               HI_S32 Offset);

/*输入流信息*/
typedef struct hiJPG_WRITESTREAM_S
{
    HI_VOID *pStreamAddr;    /*流虚拟起始地址*/
    HI_U32   StreamLen;      /*流长度*/
    HI_BOOL  EndFlag;        /*码流结束标记*/
    HI_U32   CopyLen;        /*实际拷贝的数据长度*/
}JPG_WRITESTREAM_S;

/*****************************************************************************/
/*                                 JPG接口定义                               */
/*****************************************************************************/

/*******************************************************************************
 * Function:        HI_JPG_Open
 * Description:     打开JPEG硬件解码器
 * Input:           无
 * Output:          无
 * Return:          HI_SUCCESS:    成功
 *                  HI_FAILURE     系统调用错误
 * Others: 
*******************************************************************************/
HI_S32  HI_JPG_Open(HI_VOID);

/*******************************************************************************
 * Function:        HI_JPG_Close
 * Description:     关闭JPEG硬件解码器
 * Input:           无
 * Output:          HI_SUCCESS    成功
 *                  HI_FAILURE    系统错误
 * Return:          无
 * Others:    
*******************************************************************************/
HI_S32 HI_JPG_Close(HI_VOID);

/******************************************************************************
* Function:      HI_JPG_CreateHandle
* Description:   创建JPG解码实例
* Input:         pSrcDesc 输入源描述信息
*                pWriteCB 输入流的回调函数，上层软件在流式输入情况下推荐实现
*                         此函数，上层软件按照解码器的要求在本函数中输入码流，
*                         如果设置为空，表示由用户输入码流
* Output:        无
* Return:        HI_SUCCESS:               成功
*                HI_ ERR_JPG_PTR_NULL:       空指针
*                HI_ERR_JPG_INVALID_PARA:  无效的参数  
*                HI_ERR_JPG_INVALID_FILE:  无效的文件
*                HI_ERR_JPG_NOSUPPORT_FMT: 不支持的格式
*                HI_ERR_JPG_DEC_BUSY:      解码器忙，不允许再创建
*                HI_ERR_JPG_NO_MEM:        内存不足
*                HI_FAILURE:               系统调用错误
* Others:        无
******************************************************************************/
HI_S32  HI_JPG_CreateHandle(HI_HANDLE *pHandle, const JPG_SRCDESC_S *pSrcDesc, 
                            PTR_JPG_WriteStreamCB_FN pWriteCB);

/******************************************************************************
* Function:      HI_JPG_WriteStream
* Description:   输入JPG码流数据
* Input:         Handle     JPG解码实例句柄
*                pWriteInfo 输入流信息
*                TimeOut    阻塞超时值，如果为0，表示采用非阻塞方式
* Output:        pWriteInfo 里面记录了实际拷贝的长度
* Return:        HI_SUCCESS:                成功
*                HI_ERR_JPG_INVALID_HANDLE: 无效的句柄
*                HI_ERR_JPG_PTR_NULL:       空指针
*                HI_ERR_JPG_INVALID_PARA:   无效的参数     
*                HI_ERR_JPG_INVALID_SOURCE: 无效的输入源
*                HI_FAILURE:                系统调用错误
* Others:        仅在流式输入情况下有效
******************************************************************************/
HI_S32  HI_JPG_WriteStream(HI_HANDLE Handle, JPG_WRITESTREAM_S *pWriteInfo,
                           HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_GetPrimaryInfo
* Description:   获取JPG图片信息
* Input:         Handle  解码器实例句柄                
*                TimeOut 阻塞超时值，如果为0，表示采用非阻塞方式
* Output:        pImageInfo  图片信息
* Return:        HI_SUCCESS:                成功
*                HI_ERR_JPG_INVALID_HANDLE: 无效的句柄
*                HI_ERR_JPG_TIME_OUT:       超时错误
*                HI_ERR_JPG_DEC_RUNNING:    正在解析，还未解析完
*                HI_ERR_JPG_DEC_BUSY:       解码器忙
*                HI_ERR_JPG_PARSE_FAIL:     解析错误
*                HI_ERR_JPG_NO_MEM:         内存不足
*                HI_FAILURE:                系统调用错误
* Others:        无
******************************************************************************/
HI_S32  HI_JPG_GetPrimaryInfo(HI_HANDLE Handle, JPG_PRIMARYINFO_S **pImageInfo, 
                              HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_ReleasePrimaryInfo
* Description:   释放JPG图片信息
* Input:         Handle     解码器实例                
*                pImageInfo 图片信息
* Output:        无
* Return:        HI_SUCCESS:          成功
*                HI_ERR_JPG_INVALID_HANDLE: 无效的句柄
*                HI_ERR_JPG_PTR_NULL: 空指针
*                HI_FAILURE:          系统调用错误
* Others:        无
******************************************************************************/
HI_S32  HI_JPG_ReleasePrimaryInfo(HI_HANDLE Handle, 
                                  JPG_PRIMARYINFO_S *pImageInfo);

/******************************************************************************
* Function:      HI_JPG_GetPicInfo
* Description:   获取指定序号的图信息
* Input:         Handle   解码器实例  
*                Index    图索引，0表示主图，其它表示缩略图序号
*                TimeOut  阻塞超时值，如果为0，表示采用非阻塞方式
* Output:        pPicInfo 图信息
* Return:        HI_SUCCESS:               成功
*                HI_ERR_JPG_INVALID_HANDLE:无效的句柄
*                HI_ERR_JPG_TIME_OUT:      超时错误
*                HI_ERR_JPG_THUMB_NOEXIST: 指定的缩略图不存在
*                HI_ERR_JPG_DEC_RUNNING:    正在解析，还未解析完
*                HI_ERR_JPG_DEC_BUSY:       解码器忙
*                HI_ERR_JPG_PARSE_FAIL:    解析错误
*                HI_FAILURE:               系统调用错误
* Others:        无
******************************************************************************/
HI_S32  HI_JPG_GetPicInfo(HI_HANDLE Handle, JPG_PICINFO_S *pPicInfo, 
                          HI_U32 Index, HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_Decode
* Description:   图片解码
* Input:         Handle      解码器实例  
*                Index       待解图索引，0表示主图，其它表示缩略图序号
*                TimeOut     阻塞超时值，如果为0，表示采用非阻塞方式
*                pSurface    输出信息，JPG模块当前支持的格式包括:
*                            交织类别:
*                                 JPG_COLORFMT_FMT_RGB444,
*                                 JPG_COLORFMT_FMT_RGB555,
*                                 JPG_COLORFMT_FMT_RGB565,
*                                 JPG_COLOR_FMT_RGB888,
*                                 JPG_COLOR_FMT_ARGB4444,
*                                 JPG_COLOR_FMT_ARGB1555,
*                                 JPG_COLOR_FMT_ARGB8565,
*                                 JPG_COLOR_FMT_ARGB8888,
*                                 JPG_COLOR_FMT_YCBCR888,
*                                 JPG_COLOR_FMT_AYCBCR8888,
*                                 JPG_COLOR_FMT_YCBCR422,
*                            宏块类别:
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr400MBP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr422MBHP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr422MBVP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr420MBP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr444MBP,
* Output:        pSurface    输出信息
* Return:        HI_SUCCESS:                成功
*                HI_ERR_JPG_PTR_NULL:       空指针        
*                HI_ERR_JPG_INVALID_HANDLE: 无效的句柄
*                HI_ERR_JPG_INVALID_PARA:   无效的参数
*                HI_ERR_JPG_TIME_OUT:       超时错误
*                HI_ERR_JPG_DEC_RUNNING:    正在解码，还未解码完
*                HI_ERR_JPG_DEC_BUSY:       解码器忙
*                HI_ERR_JPG_THUMB_NOEXIST:  指定的缩略图不存在
*                HI_ERR_JPG_DEC_FAIL:       解码错误
*                HI_FAILURE:                系统调用错误
* Others:        在设置输出为宏块类型时，不支持缩放
******************************************************************************/
HI_S32  HI_JPG_Decode(HI_HANDLE Handle, JPG_SURFACE_S *pSurface, 
                      HI_U32 Index, HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_Inquire
* Description:   查询解码或解析状态
* Input:         Handle   解码器实例  
* Output:        pResult  解码或解析结果
* Return:        HI_SUCCESS:                成功
*                HI_ERR_JPG_PTR_NULL:       空指针
*                HI_ERR_JPG_INVALID_HANDLE: 无效的句柄
* Others:        无
******************************************************************************/
HI_S32  HI_JPG_Inquire(HI_HANDLE Handle, JPG_RESULT_E *pResult);

/******************************************************************************
* Function:      HI_JPG_DestroyHandle
* Description:   复位解码器实例，包括清除状态、码流BUF
* Input:         Handle   解码器实例句柄  
* Output:        无
* Return:        HI_SUCCESS:                成功
*                HI_ERR_JPG_INVALID_HANDLE: 无效的句柄   
*                HI_FAILURE:                系统调用错误
* Others:        无
******************************************************************************/
HI_S32  HI_JPG_ResetHandle(HI_HANDLE Handle);

/******************************************************************************
* Function:      HI_JPG_DestroyHandle
* Description:   删除解码器实例
* Input:         Handle   解码器实例句柄  
* Output:        无
* Return:        无
* Others:        无
******************************************************************************/
HI_VOID  HI_JPG_DestroyHandle(HI_HANDLE Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_JPG_API_H_*/

