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
*                   JPEG API ��������                                       *
****************************************************************************/

#define JPG_INVALID_HANDLE 0xFFFFFFFF

/*handle type*/


/*JPG������λ�û�׼*/
typedef enum hiJPG_POSITION_E
{
    JPG_POSITION_SET = 0, /*��ʼλ��*/
    JPG_POSITION_CUR,     /*�ӵ�ǰλ��*/
    JPG_POSITION_END,     /*�ӽ���λ��*/
    JPG_POSITION_BUTT
}JPG_POSITION_E;

/*����Դ����*/
typedef enum hiJPG_SRCTYPE_E
{
    JPG_SRCTYPE_FILE = 0, /*�ļ�*/
    JPG_SRCTYPE_MEM,      /*�ڴ�*/
    JPG_SRCTYPE_STREAM,   /*��ʽ*/
    JPG_SRCTYPE_BUTT
} JPG_SRCTYPE_E;

/*�����������*/
typedef enum hiJPG_RESULT_E
{
    JPG_RESULT_IDLE = 0,   /*δ����*/
    JPG_RESULT_FINISH,     /*���*/
    JPG_RESULT_FAILURE,    /*ʧ��*/
    JPG_RESULT_RUNNING,    /*������*/
    JPG_RESULT_BUTT
} JPG_RESULT_E;

/*��������Դ��ϸ��Ϣ��������ʽ��˵������Ϣ��Ч*/
typedef union hiJPG_SRCINFO_U
{
    struct 
    {
        HI_CHAR* pAddr;     /*�ڴ�ָ���ַ*/        
        HI_U32   Length;    /*����*/
    }MemInfo;               /*����ԴΪ�ڴ��ʱ��Ҫ����Ϣ*/

    HI_CHAR *pFileName;     /*����ԴΪ�ļ�ʱ��Ҫ����Ϣ:�ļ���*/
}JPG_SRCINFO_U;

/*����Դ������Ϣ*/
typedef struct hiJPG_SRCDESC_S
{
    JPG_IMGTYPE_E ImgType; /*JPEGͼƬ����*/
    JPG_SRCTYPE_E SrcType; /*����Դ����*/
    JPG_SRCINFO_U SrcInfo; /*����Դ����ϸ��Ϣ*/
} JPG_SRCDESC_S;

/*�������Ļص��������ϲ��������ʽ���������Ӧ��ʵ�ִ˺�����
  �ڴ˺���������������������������²���ʵ�ִ˺���*/
typedef HI_S32 (*PTR_JPG_WriteStreamCB_FN)(JPG_POSITION_E Position, 
                                               HI_S32 Offset);

/*��������Ϣ*/
typedef struct hiJPG_WRITESTREAM_S
{
    HI_VOID *pStreamAddr;    /*��������ʼ��ַ*/
    HI_U32   StreamLen;      /*������*/
    HI_BOOL  EndFlag;        /*�����������*/
    HI_U32   CopyLen;        /*ʵ�ʿ��������ݳ���*/
}JPG_WRITESTREAM_S;

/*****************************************************************************/
/*                                 JPG�ӿڶ���                               */
/*****************************************************************************/

/*******************************************************************************
 * Function:        HI_JPG_Open
 * Description:     ��JPEGӲ��������
 * Input:           ��
 * Output:          ��
 * Return:          HI_SUCCESS:    �ɹ�
 *                  HI_FAILURE     ϵͳ���ô���
 * Others: 
*******************************************************************************/
HI_S32  HI_JPG_Open(HI_VOID);

/*******************************************************************************
 * Function:        HI_JPG_Close
 * Description:     �ر�JPEGӲ��������
 * Input:           ��
 * Output:          HI_SUCCESS    �ɹ�
 *                  HI_FAILURE    ϵͳ����
 * Return:          ��
 * Others:    
*******************************************************************************/
HI_S32 HI_JPG_Close(HI_VOID);

/******************************************************************************
* Function:      HI_JPG_CreateHandle
* Description:   ����JPG����ʵ��
* Input:         pSrcDesc ����Դ������Ϣ
*                pWriteCB �������Ļص��������ϲ��������ʽ����������Ƽ�ʵ��
*                         �˺������ϲ�������ս�������Ҫ���ڱ�����������������
*                         �������Ϊ�գ���ʾ���û���������
* Output:        ��
* Return:        HI_SUCCESS:               �ɹ�
*                HI_ ERR_JPG_PTR_NULL:       ��ָ��
*                HI_ERR_JPG_INVALID_PARA:  ��Ч�Ĳ���  
*                HI_ERR_JPG_INVALID_FILE:  ��Ч���ļ�
*                HI_ERR_JPG_NOSUPPORT_FMT: ��֧�ֵĸ�ʽ
*                HI_ERR_JPG_DEC_BUSY:      ������æ���������ٴ���
*                HI_ERR_JPG_NO_MEM:        �ڴ治��
*                HI_FAILURE:               ϵͳ���ô���
* Others:        ��
******************************************************************************/
HI_S32  HI_JPG_CreateHandle(HI_HANDLE *pHandle, const JPG_SRCDESC_S *pSrcDesc, 
                            PTR_JPG_WriteStreamCB_FN pWriteCB);

/******************************************************************************
* Function:      HI_JPG_WriteStream
* Description:   ����JPG��������
* Input:         Handle     JPG����ʵ�����
*                pWriteInfo ��������Ϣ
*                TimeOut    ������ʱֵ�����Ϊ0����ʾ���÷�������ʽ
* Output:        pWriteInfo �����¼��ʵ�ʿ����ĳ���
* Return:        HI_SUCCESS:                �ɹ�
*                HI_ERR_JPG_INVALID_HANDLE: ��Ч�ľ��
*                HI_ERR_JPG_PTR_NULL:       ��ָ��
*                HI_ERR_JPG_INVALID_PARA:   ��Ч�Ĳ���     
*                HI_ERR_JPG_INVALID_SOURCE: ��Ч������Դ
*                HI_FAILURE:                ϵͳ���ô���
* Others:        ������ʽ�����������Ч
******************************************************************************/
HI_S32  HI_JPG_WriteStream(HI_HANDLE Handle, JPG_WRITESTREAM_S *pWriteInfo,
                           HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_GetPrimaryInfo
* Description:   ��ȡJPGͼƬ��Ϣ
* Input:         Handle  ������ʵ�����                
*                TimeOut ������ʱֵ�����Ϊ0����ʾ���÷�������ʽ
* Output:        pImageInfo  ͼƬ��Ϣ
* Return:        HI_SUCCESS:                �ɹ�
*                HI_ERR_JPG_INVALID_HANDLE: ��Ч�ľ��
*                HI_ERR_JPG_TIME_OUT:       ��ʱ����
*                HI_ERR_JPG_DEC_RUNNING:    ���ڽ�������δ������
*                HI_ERR_JPG_DEC_BUSY:       ������æ
*                HI_ERR_JPG_PARSE_FAIL:     ��������
*                HI_ERR_JPG_NO_MEM:         �ڴ治��
*                HI_FAILURE:                ϵͳ���ô���
* Others:        ��
******************************************************************************/
HI_S32  HI_JPG_GetPrimaryInfo(HI_HANDLE Handle, JPG_PRIMARYINFO_S **pImageInfo, 
                              HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_ReleasePrimaryInfo
* Description:   �ͷ�JPGͼƬ��Ϣ
* Input:         Handle     ������ʵ��                
*                pImageInfo ͼƬ��Ϣ
* Output:        ��
* Return:        HI_SUCCESS:          �ɹ�
*                HI_ERR_JPG_INVALID_HANDLE: ��Ч�ľ��
*                HI_ERR_JPG_PTR_NULL: ��ָ��
*                HI_FAILURE:          ϵͳ���ô���
* Others:        ��
******************************************************************************/
HI_S32  HI_JPG_ReleasePrimaryInfo(HI_HANDLE Handle, 
                                  JPG_PRIMARYINFO_S *pImageInfo);

/******************************************************************************
* Function:      HI_JPG_GetPicInfo
* Description:   ��ȡָ����ŵ�ͼ��Ϣ
* Input:         Handle   ������ʵ��  
*                Index    ͼ������0��ʾ��ͼ��������ʾ����ͼ���
*                TimeOut  ������ʱֵ�����Ϊ0����ʾ���÷�������ʽ
* Output:        pPicInfo ͼ��Ϣ
* Return:        HI_SUCCESS:               �ɹ�
*                HI_ERR_JPG_INVALID_HANDLE:��Ч�ľ��
*                HI_ERR_JPG_TIME_OUT:      ��ʱ����
*                HI_ERR_JPG_THUMB_NOEXIST: ָ��������ͼ������
*                HI_ERR_JPG_DEC_RUNNING:    ���ڽ�������δ������
*                HI_ERR_JPG_DEC_BUSY:       ������æ
*                HI_ERR_JPG_PARSE_FAIL:    ��������
*                HI_FAILURE:               ϵͳ���ô���
* Others:        ��
******************************************************************************/
HI_S32  HI_JPG_GetPicInfo(HI_HANDLE Handle, JPG_PICINFO_S *pPicInfo, 
                          HI_U32 Index, HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_Decode
* Description:   ͼƬ����
* Input:         Handle      ������ʵ��  
*                Index       ����ͼ������0��ʾ��ͼ��������ʾ����ͼ���
*                TimeOut     ������ʱֵ�����Ϊ0����ʾ���÷�������ʽ
*                pSurface    �����Ϣ��JPGģ�鵱ǰ֧�ֵĸ�ʽ����:
*                            ��֯���:
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
*                            ������:
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr400MBP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr422MBHP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr422MBVP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr420MBP,
*                                 JPG_MBCOLOR_FMT_JPG_YCbCr444MBP,
* Output:        pSurface    �����Ϣ
* Return:        HI_SUCCESS:                �ɹ�
*                HI_ERR_JPG_PTR_NULL:       ��ָ��        
*                HI_ERR_JPG_INVALID_HANDLE: ��Ч�ľ��
*                HI_ERR_JPG_INVALID_PARA:   ��Ч�Ĳ���
*                HI_ERR_JPG_TIME_OUT:       ��ʱ����
*                HI_ERR_JPG_DEC_RUNNING:    ���ڽ��룬��δ������
*                HI_ERR_JPG_DEC_BUSY:       ������æ
*                HI_ERR_JPG_THUMB_NOEXIST:  ָ��������ͼ������
*                HI_ERR_JPG_DEC_FAIL:       �������
*                HI_FAILURE:                ϵͳ���ô���
* Others:        ���������Ϊ�������ʱ����֧������
******************************************************************************/
HI_S32  HI_JPG_Decode(HI_HANDLE Handle, JPG_SURFACE_S *pSurface, 
                      HI_U32 Index, HI_U32 TimeOut);

/******************************************************************************
* Function:      HI_JPG_Inquire
* Description:   ��ѯ��������״̬
* Input:         Handle   ������ʵ��  
* Output:        pResult  �����������
* Return:        HI_SUCCESS:                �ɹ�
*                HI_ERR_JPG_PTR_NULL:       ��ָ��
*                HI_ERR_JPG_INVALID_HANDLE: ��Ч�ľ��
* Others:        ��
******************************************************************************/
HI_S32  HI_JPG_Inquire(HI_HANDLE Handle, JPG_RESULT_E *pResult);

/******************************************************************************
* Function:      HI_JPG_DestroyHandle
* Description:   ��λ������ʵ�����������״̬������BUF
* Input:         Handle   ������ʵ�����  
* Output:        ��
* Return:        HI_SUCCESS:                �ɹ�
*                HI_ERR_JPG_INVALID_HANDLE: ��Ч�ľ��   
*                HI_FAILURE:                ϵͳ���ô���
* Others:        ��
******************************************************************************/
HI_S32  HI_JPG_ResetHandle(HI_HANDLE Handle);

/******************************************************************************
* Function:      HI_JPG_DestroyHandle
* Description:   ɾ��������ʵ��
* Input:         Handle   ������ʵ�����  
* Output:        ��
* Return:        ��
* Others:        ��
******************************************************************************/
HI_VOID  HI_JPG_DestroyHandle(HI_HANDLE Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_JPG_API_H_*/

