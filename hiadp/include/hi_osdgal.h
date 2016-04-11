/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_osdgal.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/09/17
  Description   : 
  History       :
  1.Date        : 2008/09/17
    Author      : g00123327
    Modification: Created file
******************************************************************************/
#ifndef  __HI_OSDGAL_H__
#define  __HI_OSDGAL_H__

//#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef IN
#define IN                    /* ������� */
#endif

#ifndef OUT
#define OUT                   /* ������� */
#endif

#define HI_OSD_MAXWIDTH     720
#define HI_OSD_MAXHEIGHT    576

typedef enum hiOSDGAL_MODE_E
{
    OSDGAL_MODE_2BIT,         /* 2bits */
    OSDGAL_MODE_4BIT,         /* 4bits */
    OSDGAL_MODE_8BIT,         /* RGB888 */
    OSDGAL_MODE_16BIT,        /* ARGB */
    OSDGAL_MODE_32BIT,        /* ARGB */
    OSDGAL_MODE_BUTT
} HI_OSDGAL_MODE_E;

typedef enum hiOSDGAL_ENABLE_E
{
    OSDGAL_OFF,               /* �ر�OSD */
    OSDGAL_ON                 /* ����OSD */
} HI_OSDGAL_ENABLE_E;

typedef enum hiOSDGAL_LAYER_E
{
    OSDGAL_LAYER1,            /* OSDͼ��1 */
    OSDGAL_LAYER2,            /* OSDͼ��2 */
    OSDGAL_LAYER_BUTT
} HI_OSDGAL_LAYER_E;

typedef struct hiOSDGAL_RECT_S
{
    HI_U32 u32x;              /* ͼ����ʼλ�� */
    HI_U32 u32y;
    HI_U32 u32Width;          /* ͼ��Ŀ�� */
    HI_U32 u32Height;         /* ͼ�εĸ߶� */
} HI_OSDGAL_RECT_S;

typedef HI_OSDGAL_LAYER_E HI_OSDGAL_HANDLE;
typedef HI_OSDGAL_RECT_S  HI_OSDGAL_IMAGE_S;

/* ===================== �����붨�� ===================== */
#define HI_OSDGAL_SUCCESS           HI_SUCCESS /*�����ɹ�*/
#define HI_OSDGAL_FAILED            HI_FAILURE /*����ʧ��*/
#define HI_ERR_OSDGAL_INVARG        -2 /*����Ĳ���*/

#define HI_ERR_OSDGAL_NOTINIT       -3 /*û�г�ʼ��*/
#define HI_ERR_OSDGAL_NOPEN         -4 /*ͼ��û�д�*/
#define HI_ERR_OSDGAL_INVSTATE      -5 /*״̬����*/
#define HI_ERR_OSDGAL_NOTSUPPORT    -6 /*��֧�ִ˲���*/




/* ===================== �������� ===================== */

/* OSD��ʼ������ */
HI_S32 OSDGAL_Init();

/* ��OSDͼ���豸������ͼ���� */
HI_S32 OSDGAL_OpenOSDLayer(IN HI_OSDGAL_LAYER_E eOSDLayer,
                           OUT HI_OSDGAL_HANDLE *pHandle);

/* �ر�OSDͼ���豸 */
HI_S32 OSDGAL_CloseOSDLayer(IN HI_OSDGAL_HANDLE hHandle);

/* ��ȡͼ���Դ� */
HI_S32 OSDGAL_GetSurfaceAddr(IN HI_OSDGAL_HANDLE hHandle,
				OUT HI_U32 * SurfaceBuffAddr);


/* ʹ��OSDͼ�� */
HI_S32 OSDGAL_Enable(IN HI_OSDGAL_HANDLE hHandle,
                     IN HI_OSDGAL_ENABLE_E eEnable);

/* OSDȥ��ʼ������ */
HI_S32 OSDGAL_DeInit();
	   
HI_S32 OSDGAL_DrawPixel(IN HI_OSDGAL_HANDLE hHandle,
						IN HI_U32 u32X,
                        IN HI_U32 u32Y,
                        IN HI_U32 u32Color);

/* ���ƾ��� */
HI_S32 OSDGAL_DrawRectangle(IN HI_OSDGAL_HANDLE hHandle,
                            IN const HI_OSDGAL_RECT_S *pstruRectSize,
                            IN HI_U32 u32Color);

/* ������Բ */
HI_S32 OSDGAL_DrawEllipse(IN HI_OSDGAL_HANDLE hHandle,
                          IN const HI_OSDGAL_RECT_S *pstruEllipseSize,
                          IN HI_U32 u32Color);

/* ������ */
HI_S32 OSDGAL_DrawLine(IN HI_OSDGAL_HANDLE hHandle,
                       IN HI_U32 u32StartX,
                       IN HI_U32 u32StartY,
                       IN HI_U32 u32EndX,
                       IN HI_U32 u32EndY,
                       IN HI_U32 u32Color);

/* �����Բ */
HI_S32 OSDGAL_FillEllipse(IN HI_OSDGAL_HANDLE hHandle,
                          IN const HI_OSDGAL_RECT_S* pstruEllipseSize,
                          IN HI_U32 u32Color);

/* ������ */
HI_S32 OSDGAL_FillRectangle(IN HI_OSDGAL_HANDLE hHandle,
                            IN const HI_OSDGAL_RECT_S* pstruRectSize,
                            IN HI_U32 u32Color);

/* ������ɫģʽ���ο�HI_OSDGAL_MODE_E���� */
HI_S32 OSDGAL_SetColorMode(IN HI_OSDGAL_HANDLE hHandle,
                           IN HI_OSDGAL_MODE_E eColorMode);

/* ��ȡ��ɫģʽ���ο�HI_OSDGAL_MODE_E���� */
HI_S32 OSDGAL_GetColorMode(IN HI_OSDGAL_HANDLE hHandle,
                           OUT HI_OSDGAL_MODE_E *peColorMode);

/* ��ȡָ�����ο��ڵ����� */
HI_S32 OSDGAL_ReadRectangleData(IN HI_OSDGAL_HANDLE hHandle,
                                IN const HI_OSDGAL_RECT_S *pstruRectSize,
                                IN HI_U32 u32DataLen,                                
                                IN OUT HI_U8 *pu8Data);

/* ��ָ�����ο���д���� */
HI_S32 OSDGAL_WriteRectangleData(IN HI_OSDGAL_HANDLE hHandle,
                                 IN const HI_OSDGAL_RECT_S *pstruRectSize,
                                 IN HI_U32 u32DataLen,                                
                                 IN OUT HI_U8 *pu8Data);

HI_S32 OSDGAL_WriteRectangleDataMask(IN HI_OSDGAL_HANDLE hHandle,
                                 IN const HI_OSDGAL_RECT_S *pstruRectSize,
                                 IN HI_U32 u32DataLen,
                                 IN OUT HI_U8 *pu8Data,
                                 IN HI_U32 maskcolor);

/* ��ȡĳ�����ص�ֵ */
HI_S32 OSDGAL_GetPixel(IN HI_OSDGAL_HANDLE hHandle,
                       IN HI_U32 u32x,
                       IN HI_U32 u32y,
                       IN OUT HI_U8 *pu8Data);

/* ���õ�ɫ�� */
HI_S32 OSDGAL_SetColorCult(IN HI_OSDGAL_HANDLE hHandle,
                           IN HI_U32 u32CultCount,                            
                           IN HI_U32  u32Start,
                           IN HI_U16* pu16Red,
                           IN HI_U16* pu16Green,
                           IN HI_U16* pu16Blue,
                           IN HI_U16* pu16alph);

/* ���õ�ɫ����չ�ӿ� */
HI_S32 OSDGAL_SetColorCultEx(IN HI_OSDGAL_HANDLE hHandle,
                             IN HI_U32  u32CultCount,
                             IN HI_U32  u32Start,
                             IN HI_U32* pColor);


HI_S32 OSDGAL_SetAlpha(IN HI_OSDGAL_HANDLE hHandle,
                     HI_U8 u8value);


HI_S32 OSDGAL_Refresh(IN HI_OSDGAL_HANDLE hHandle,
	IN const HI_OSDGAL_RECT_S *pstruRectSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /*__HI_OSDGAL_H__*/


