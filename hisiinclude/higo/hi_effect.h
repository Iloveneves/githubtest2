/*****************************************************************************/

/*                Copyright 2005 - 2005, Huawei Tech. Co., Ltd.              */
/*                           ALL RIGHTS RESERVED                             */
/*                                                                           */
/* FileName: hi_peffect.h                                                    */
/* Version: 01a                                                              */
/*                                                                           */
/* History:                                                                  */
/* 1. 01a,2008-12-27,z37062 Create this file.                                */
/*****************************************************************************/
#ifndef __HI_EFFECT_H__
#define __HI_EFFECT_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#include <hi_type.h>
#include <hi_go.h>

/*��Ļˢ�»ص�����*/
typedef HI_S32 (*pRefresh_Callback)(HI_VOID);
typedef enum hiEFFECT_MODE_E
{
    HI_EFFECT_NONE = 0, 				/*������ʾ*/
    HI_EFFECT_TURNPAGE, 				/*��ҳ*/
    HI_EFFECT_ROLLPAGE, 				/*����*/
    HI_EFFECT_VERTICALSHUTTER,		/* ��ֱ��Ҷ��*/
    HI_EFFECT_HORIZONTALSHUTTER,		/* ˮƽ��Ҷ��*/
    HI_EFFECT_LEFTIN,					/* ������*/
    HI_EFFECT_TOPIN, 					/* ���ϳ��*/
    HI_EFFECT_TRANSIN, 				/* ��������*/
    HI_EFFECT_ROTATE,  				/*����*/
    HI_EFFECT_CENTEROUT,  				/*���뽥��*/
    HI_EFFECT_CENTERIN, 				/*���뽥��*/
} EFFECT_MODE_E;

typedef enum hiEFFECT_SPEED
{
    HI_EFFECT_SPEED_FAST   = 1,
    HI_EFFECT_SPEED_NORMAL = 2,
    HI_EFFECT_SPEED_SLOW = 4,
} EFFECT_SPEED;

/*��Ч��ʼ������,
ע����Ļˢ�»ص�������������ʾģʽ
bLetfbox ΪHI_TRUE, ��ʾͼ���������Ļ������һ��ʱ��Ӻڱ�
bLetfbox ΪHI_FALSE, ��ʾͼ�����ų�ȫ��,���ͼ�α�������Ļ������һ��
�ᵼ�±���
ע:�������ַ�ʽ,ͼ��ȫ����ʾ
 */
extern HI_S32  HI_Effect_Init(pRefresh_Callback pfCallBack, HI_BOOL bLetfbox);
extern HI_S32  HI_Effect_Deinit(HI_VOID);

/*��Ч���ź���, �ú���Ϊ��������,��Ч��ɲ��˳�
����surface�����ظ�ʽ���뱣��һ��
Ŀǰ֧�ֵĸ�ʽ:
    HIGO_PF_4444,
    HIGO_PF_0444,
    HIGO_PF_1555,
    HIGO_PF_0555,
    HIGO_PF_565,
 */
extern HI_S32  HI_Effect_Play(HI_HANDLE hNewSurface, HI_HANDLE hScreenSurface, EFFECT_MODE_E mode);

/*��Ч������ֹ,������߰�����Ӧ�ٶ�*/
extern HI_S32  HI_Effect_Stop(HI_VOID);

/*��Ч�����ٶ�*/
extern HI_S32  HI_Effect_SetSpeed(EFFECT_SPEED eSpeed);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_PEFFECT_H__ */
