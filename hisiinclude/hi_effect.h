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

/*屏幕刷新回调函数*/
typedef HI_S32 (*pRefresh_Callback)(HI_VOID);
typedef enum hiEFFECT_MODE_E
{
    HI_EFFECT_NONE = 0, 				/*拷贝显示*/
    HI_EFFECT_TURNPAGE, 				/*翻页*/
    HI_EFFECT_ROLLPAGE, 				/*卷轴*/
    HI_EFFECT_VERTICALSHUTTER,		/* 垂直百叶窗*/
    HI_EFFECT_HORIZONTALSHUTTER,		/* 水平百叶窗*/
    HI_EFFECT_LEFTIN,					/* 从左抽出*/
    HI_EFFECT_TOPIN, 					/* 从上抽出*/
    HI_EFFECT_TRANSIN, 				/* 渐进渐出*/
    HI_EFFECT_ROTATE,  				/*螺旋*/
    HI_EFFECT_CENTEROUT,  				/*中央渐出*/
    HI_EFFECT_CENTERIN, 				/*中央渐入*/
} EFFECT_MODE_E;

typedef enum hiEFFECT_SPEED
{
    HI_EFFECT_SPEED_FAST   = 1,
    HI_EFFECT_SPEED_NORMAL = 2,
    HI_EFFECT_SPEED_SLOW = 4,
} EFFECT_SPEED;

/*特效初始化函数,
注册屏幕刷新回调函数并设置显示模式
bLetfbox 为HI_TRUE, 表示图像比例与屏幕比例不一致时添加黑边
bLetfbox 为HI_FALSE, 表示图像缩放成全屏,如果图形比例与屏幕比例不一致
会导致变形
注:无论那种方式,图像都全屏显示
 */
extern HI_S32  HI_Effect_Init(pRefresh_Callback pfCallBack, HI_BOOL bLetfbox);
extern HI_S32  HI_Effect_Deinit(HI_VOID);

/*特效播放函数, 该函数为阻塞函数,特效完成才退出
所有surface的像素格式必须保持一致
目前支持的格式:
    HIGO_PF_4444,
    HIGO_PF_0444,
    HIGO_PF_1555,
    HIGO_PF_0555,
    HIGO_PF_565,
 */
extern HI_S32  HI_Effect_Play(HI_HANDLE hNewSurface, HI_HANDLE hScreenSurface, EFFECT_MODE_E mode);

/*特效播放中止,可以提高按键响应速度*/
extern HI_S32  HI_Effect_Stop(HI_VOID);

/*特效播放速度*/
extern HI_S32  HI_Effect_SetSpeed(EFFECT_SPEED eSpeed);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_PEFFECT_H__ */
