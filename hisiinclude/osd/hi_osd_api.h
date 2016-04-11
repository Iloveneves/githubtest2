/*******************************************************************************
 *              Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_osd_api.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-01-22       z67193    HI_NULL      Create this file.
 ******************************************************************************/

#ifndef _OSD_API_H_
#define _OSD_API_H_

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_vo_errcode.h"
#include "hi_vo_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/*****************************************************************************
* Function:      HI_OSD_Init
* Description:   ��ʼ��OSD
* Input:         eLayer                         Ҫ��λ��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Init(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Exit
* Description:   �˳�OSD
* Input:         eLayer                         Ҫ��λ��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Exit(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Reset
* Description:   ��λOSD
* Input:         eLayer                         Ҫ��λ��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Reset(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Show
* Description:   ��ʾOSD
* Input:         eLayer                         Ҫ��ʾ��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Show(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Hide
* Description:   ����OSD
* Input:         eLayer                         Ҫ���ص�OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Hide(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_GetFrameBuffer
* Description:   ����OSD
* Input:         ��
* Output:        pFrameBuffer                   �Դ�ṹָ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        OSD��������Ϊ�û������Դ棬�����޸�Ϊ���������Դ�ʱ����ȥ��
******************************************************************************/
HI_S32 HI_OSD_GetFrameBuffer(OSD_LAYER_E eLayer, 
                             OSD_FRAME_BUFFER_S *pFrameBuffer);

/*****************************************************************************
* Function:      HI_OSD_ReleaseFrameBuffer
* Description:   �ͷ�OSD���Դ�
* Input:         ��
* Output:        eLayer                   
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
* Others:        
******************************************************************************/
HI_S32 HI_OSD_ReleaseFrameBuffer(OSD_LAYER_E eLayer);/*AI7D02566*/

/*****************************************************************************
* Function:      HI_OSD_SetScreenPara
* Description:   ������Ļ����
* Input:         pScreenPara                    ��Ļ����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_SCREEN_SIZE      ��Ļ��С��Ч
*                HI_ERR_VO_OSD_WINDOW_SIZE      ��ʾ������Ч
*                HI_ERR_VO_OSD_AR               ���ͱ���Ч
*                HI_ERR_VO_OSD_HAS_REGION       ��ǰ��region����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetScreenPara(OSD_LAYER_E eLayer, VO_RECTANGLE_S *pScreenPara);

/*****************************************************************************
* Function:      HI_OSD_GetScreenPara
* Description:   ��ѯ��Ļ����
* Input:         pScreenPara                    ��Ļ����ָ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetScreenPara(OSD_LAYER_E eLayer, VO_RECTANGLE_S *pScreenPara);

/*****************************************************************************
* Function:      HI_OSD_SetOutputAspectRatio
* Description:   ������Ļ����
* Input:         pScreenPara                    ��Ļ����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_SCREEN_SIZE      ��Ļ��С��Ч
*                HI_ERR_VO_OSD_WINDOW_SIZE      ��ʾ������Ч
*                HI_ERR_VO_OSD_AR               ���ͱ���Ч
*                HI_ERR_VO_OSD_HAS_REGION       ��ǰ��region����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetOutputAspectRatio(OSD_LAYER_E eLayer, OSD_ASPECT_RATIO_E eRatio);

/*****************************************************************************
* Function:      HI_OSD_GetOutputAspectRatio
* Description:   ������Ļ����
* Input:         pScreenPara                    ��Ļ����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_SCREEN_SIZE      ��Ļ��С��Ч
*                HI_ERR_VO_OSD_WINDOW_SIZE      ��ʾ������Ч
*                HI_ERR_VO_OSD_AR               ���ͱ���Ч
*                HI_ERR_VO_OSD_HAS_REGION       ��ǰ��region����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetOutputAspectRatio(OSD_LAYER_E eLayer, OSD_ASPECT_RATIO_E *pRatio);

/*****************************************************************************
* Function:      HI_OSD_EnableP2N
* Description:   ʹ��P2N����
* Input:         eLayer                         Ҫʹ��P2N��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableP2N(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_DisableP2N
* Description:   ��ֹP2N����
* Input:         eLayer                         Ҫ��ֹP2N��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableP2N(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_EnableDeflicker
* Description:   ʹ�ܿ���˸����
* Input:         eLayer                         Ҫʹ�ܿ���˸��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableDeflicker(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_DisableDeflicker
* Description:   ��ֹ����˸����
* Input:         eLayer                         Ҫ��ֹ����˸��OSD�Ĳ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableDeflicker(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_CreateRegion
* Description:   ����region
* Input:         pRegionPara                    region����ָ��
* Output:        pHandle                        region�ľ��ָ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_LAYER    ��Ч��OSD���
*                HI_ERR_VO_OSD_INVALID_PICMOD   ��Ч����ɫ��ʽ
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_BYTENOALIGN          δ�ֽڶ���
*                HI_ERR_VO_OSD_MEM_BEYOND       �ڴ�Խ��
*                HI_ERR_VO_OSD_REGION_SIZE      region��С��Ч
*                HI_ERR_VO_OSD_BEYOND           ������Ļ��Χ
*                HI_ERR_VO_OSD_OVERLAP          region֮�����ص�
*                HI_ERR_VO_OSD_CLIPPING         �ü�������Ч
*                HI_ERR_VO_ALPHA                ��Ч��͸����
*                HI_ERR_VO_OSD_COLORKEY         �ؼ�ɫ�Ĳ�����Ч
*                HI_ERR_VO_OSD_REGION_NUM       region��Ŀ����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_CreateRegion(OSD_REGION_PARA_S *pRegionPara, 
                          OSD_REGION_HANDLE_T *pHandle);

/*****************************************************************************
* Function:      HI_OSD_DestroyRegion
* Description:   ����һ��region
* Input:         Handle                         region�ľ��
* Output:
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DestroyRegion(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_ResizeRegion
* Description:   ���¶���region�Ĵ�С
* Input:         Handle                         region�ľ��
*                u32Width                       region�Ŀ��
*                u32Height                      region�ĸ߶�
* Output:        pBuffer                        �µ�bufferָ��
*                pBufferPhy                     �µ�buffer����ָ��
*                u32BufferSize                  �µ�buffer��С
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_OSD_OVERLAP          ����region�ص�
* Others:        
******************************************************************************/
HI_S32 HI_OSD_ResizeRegion(OSD_REGION_HANDLE_T Handle, HI_U32 u32Width, 
                           HI_U32 u32Height, HI_U32 *pBuffer, HI_U32 pBufferPhy,
                           HI_U32 u32BufferSize);

/*****************************************************************************
* Function:      HI_OSD_ShowRegion
* Description:   ��ʾregion
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_ShowRegion(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_HideRegion
* Description:   ����region
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_HideRegion(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_SetClut
* Description:   ����װ��clut�����
* Input:         Handle                         region�ľ��
*                pClutPhy                       CLUT��ָ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetClut(OSD_REGION_HANDLE_T Handle, HI_U32 *pClut);

/*****************************************************************************
* Function:      HI_OSD_SetBmpAddr
* Description:   ����ָ��ͼƬ���ݻ�������ַ
* Input:         Handle                         region�ľ��
*                pBmpAddr                       ͼƬ���ݻ��������߼���ַ
*                u32BmpPhy                      ͼƬ���ݻ������������ַ
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetBmpAddr(OSD_REGION_HANDLE_T Handle, HI_U32 *pBmpVir, HI_U32 u32BmpPhy);

/*****************************************************************************
* Function:      HI_OSD_SetRegionPosition
* Description:   ����region����ʼ������
* Input:         Handle                         region�ľ��
*                pDisplayArea                   ��ʾ�������
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_OSD_BEYOND           ������Ļ��Χ
*                HI_ERR_VO_OSD_OVERLAP          region֮�����ص�
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetRegionPosition(OSD_REGION_HANDLE_T Handle, 
                                VO_POSITION_S *pPosition);


/*****************************************************************************
* Function:      HI_OSD_EnableAlphaPremul
* Description:   ����REGION��Ԥ������
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableAlphaPremul(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_DisableAlphaPremul
* Description:   �ر�REGION��Ԥ������
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableAlphaPremul(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_EnableClipArea
* Description:   ����Clipping����
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableClipArea(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_DisableClipArea
* Description:   �ر�Clipping����
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableClipArea(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_SetClippingArea
* Description:   ����region�Ĳü�����
* Input:         Handle                         region�ľ��
*                sClipArea                      �ü��������
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_OSD_BEYOND           �ü�����Խ��
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetClipArea(OSD_REGION_HANDLE_T Handle, 
                          VO_RECTANGLE_S *pClipArea);

/*****************************************************************************
* Function:      HI_OSD_EnableColorKey
* Description:   ����ColorKey����
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableColorKey(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_DisableColorKey
* Description:   �ر�ColorKey����
* Input:         Handle                         region�ľ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableColorKey(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_SetColorKeyPara
* Description:   ��ȡ��Ƶ����Դ
* Input:         Handle                         region�ľ��
*                pColorSpace                    ColorSpace����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_OSD_COLORKEY         �ؼ�ɫ��������ȷ
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetColorKeyPara(OSD_REGION_HANDLE_T Handle, 
                              OSD_COLORKEY_S *pColorKey);

/*****************************************************************************
* Function:      HI_OSD_GetPixel
* Description:   ��ȡ����������ɫ
* Input:         Handle                         region�ľ��
*                pPosition                      λ�ò���
* Output:        pColor                         ��ɫָ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_OSD_BEYOND           �ü�����Խ��
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetPixel(OSD_REGION_HANDLE_T Handle, VO_POSITION_S *pPosition,
                       HI_U32 *pColor);

/*****************************************************************************
* Function:      HI_OSD_SetColorFormat
* Description:   ������ɫģʽ
* Input:         Handle                         region�ľ��
*                eColorFormat                   ��ɫģʽ
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_OSD_INVALID_PICMOD   ��Ч����ɫ��ʽ
* Others:        ͬ�����ؿ�ȿ�������
******************************************************************************/
HI_S32 HI_OSD_SetColorFormat(OSD_REGION_HANDLE_T Handle, 
                             OSD_COLOR_FORMAT_E eColorFormat);

/*****************************************************************************
* Function:      HI_OSD_SetAlpha
* Description:   ����region��ȫ��͸����
* Input:         Handle                         region�ľ��
*                u32Alpha                       ͸����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_ALPHA                ��Ч��͸����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetAlpha(OSD_REGION_HANDLE_T Handle, HI_U32 u32Alpha);

/*****************************************************************************
* Function:      HI_OSD_SetExtAlpha
* Description:   �������1555��ɫģʽ����չ͸����
* Input:         Handle                         region�ľ��
*                u32ExtAlpha0                   1555��λΪ0ʱ��͸����
*                u32ExtAlpha1                   1555��λΪ1ʱ��͸����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
*                HI_ERR_VO_ALPHA                ��Ч��͸����
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetExtAlpha(OSD_REGION_HANDLE_T Handle, HI_U32 u32ExtAlpha0,
                          HI_U32 u32ExtAlpha1);

/*****************************************************************************
* Function:      HI_OSD_OperationSync
* Description:   ͬ������ͼƬ���ݲ���
* Input:         Handle                         region�ľ��
*                pOperationArea                 ������������
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_OperationSync(OSD_REGION_HANDLE_T Handle, 
                            VO_RECTANGLE_S *pOperationArea, HI_BOOL bBlock);

/*****************************************************************************
* Function:      HI_OSD_IsSyncComplete
* Description:   ��ѯͬ������ͼƬ���ݲ����Ƿ��Ѿ���ɣ�����operationSync���첽����
* Input:         Handle                         region�ľ��
*                pbComplete                     �Ƿ��Ѿ����
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_IsSyncComplete(OSD_REGION_HANDLE_T Handle, HI_BOOL *pbComplete);

/*****************************************************************************
* Function:      HI_OSD_GetRegionPara
* Description:   ��ȡregion�Ĳ���
* Input:         Handle                         region�ľ��
* Output:        pRegionPara                    region�Ĳ���ָ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_OSD_INVALID_REGION   ��Ч��region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetRegionPara(OSD_REGION_HANDLE_T Handle, 
                            OSD_REGION_PARA_S *pRegionPara);



#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VO_API_H_*/

