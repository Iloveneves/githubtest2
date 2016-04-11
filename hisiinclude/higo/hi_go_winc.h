/**
 \file
 \brief Wincģ��ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-8-5
 */

#ifndef __HI_GO_WINC_H__
#define __HI_GO_WINC_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/

/** @defgroup hi_go_winc_data_structure Wincģ��
 *  @ingroup H2
 *  @brief ��ϸ����WMģ������ݽṹ����������
 *  @{  */

/**����Z�������ʽ*/
typedef enum
{
    HIGO_ZORDER_MOVETOP = 0,  /**<�Ƶ����*/
    HIGO_ZORDER_MOVEUP,       /**<������*/
    HIGO_ZORDER_MOVEBOTTOM,   /**<�Ƶ���ײ�*/
    HIGO_ZORDER_MOVEDOWN,     /**<������*/
    HIGO_ZORDER_BUTT
} HIGO_ZORDER_E;
#if 0
/** �������� */
typedef enum 
{
    HIGO_WINSTYLE_NORMAL = 0,  /**< һ�㴰�� */ 
    HIGO_WINSTYLE_TOPMOST,     /**< ���㴰�� */
    HIGO_WINSTYLE_BUTT
} HIGO_WINSTYLE_E;
#endif
/** ��󴰿ڲ��� */
#define HIGO_WINTREE_NUM 16

/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/** @defgroup hi_go_winc_api WMģ��
 *  @ingroup H1
 *  @brief ��ϸ����WMģ���API����
 *  @{  */

/**
\brief ��������
\attention \n
һ��ͼ��ֻ�ܴ���һ������
\param[in] Layer ͼ����
\param[out] pDesktop �������ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_ALREADYBIND

\see \n
::HI_GO_DestroyDesktop
 */
HI_S32 HI_GO_CreateDesktop(HI_HANDLE Layer, HI_HANDLE *pDesktop);

/**
\brief ��������
\attention \n
��
\param[in] Desktop ������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDesktop
*/
HI_S32 HI_GO_DestroyDesktop(HI_HANDLE Desktop);

/**
\brief ��ָ�������ϴ�������
\attention \n
����������ظ�ʽ��������alphaʱ�����ڵ�surface����ѡ���Ƿ�ʹ������alpha
\param[in] Desktop  ������
\param[in] pRect ���δ�������Ϊ�ձ�ʾ������������
\param[in] LayerNum ���������Ĳ��,���Ϊ0-15��
\param[in] bHasPixelAlpha �����Ƿ������alpha
\param[out] pWindow  �洢���ھ���ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_DestroyWindow
*/
HI_S32 HI_GO_CreateWindow(HI_HANDLE Desktop, const HI_RECT* pRect, 
                                    HI_U32 LayerNum, HI_BOOL bHasPixelAlpha,
                                    HI_HANDLE* pWindow);


/**
\brief ���ٴ���
\attention \n
�´����Ĵ���ʼ����������
\param[in] Window �����ٴ��ھ��
\param[in] LayerNum ���������Ĳ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateWindow
*/
HI_S32 HI_GO_DestroyWindow(HI_HANDLE Window, HI_U32 LayerNum);

/**
\brief ��ȡ���ھ���
\attention \n
��
\param[in] Window ���ھ��
\param[out] pRect  ���ھ��ε�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowPos \n
::HI_GO_ResizeWindow
*/
HI_S32 HI_GO_GetWindowRect(HI_HANDLE Window, HI_RECT* pRect);

/**
\brief ���ô����ڰ������ϵ���ʼλ��
\attention \n
��
\param[in] Window ���ھ��
\param[in] StartX X����
\param[in] StartY Y����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_SetWindowPos(HI_HANDLE Window, HI_S32 StartX, HI_S32 StartY);

/**
\brief �ı䴰�ڴ�С
\attention \n
�ı䴰�ڴ�С�󣬴�����Ҫ�ػ�
\param[in] Window ���ھ��
\param[in] Width ���ڿ��
\param[in] Height ���ڸ߶�

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_ResizeWindow(HI_HANDLE Window, HI_S32 Width, HI_S32 Height);

/**
\brief ��ȡ����Z��
\attention \n
��
\param[in] Window ���ھ��
\param[in] LayerNum ���������Ĳ��
\param[out] pZOrder �洢����Z��ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_ChangeWindowZOrder
*/
HI_S32 HI_GO_GetWindowZOrder(HI_HANDLE Window, HI_U32 LayerNum, HI_U32* pZOrder);

/**
\brief ���Ĵ���Z��
\attention \n
��
\param[in] Window ���ھ��
\param[in] EnType ����Z�������ʽ
\param[in] LayerNum ���������Ĳ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVZORDERTYPE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_GetWindowZOrder
*/
HI_S32 HI_GO_ChangeWindowZOrder(HI_HANDLE Window, HIGO_ZORDER_E EnType, HI_U32 LayerNum);

/**
\brief ��ȡ���ڿɼ���
\attention \n
��
\param[in] Window ���ھ��
\param[out] pOpacity �洢���ڲ�͸���ȵĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowOpacity
*/
HI_S32 HI_GO_GetWindowOpacity(HI_HANDLE Window, HI_U8* pOpacity);

/**
\brief ���ô��ڲ�͸����
\attention \n
��
\param[in] Window ���ھ��
\param[in] Opacity ���ڲ�͸����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_GetWindowOpacity
*/
HI_S32 HI_GO_SetWindowOpacity(HI_HANDLE Window, HI_U8 Opacity);

/**
\brief ���ô��ڵ�colorkey
\attention \n
����colorkey������������������ͬʱ���У����細�ڿɼ��Ȳ�Ϊ0xFFʱ�޷����ô��ڵ�colorkey
\param[in] Window ���ھ��
\param[in] ColorKey Colorkeyֵ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
��
*/
HI_S32 HI_GO_SetWindowColorkey(HI_HANDLE Window, HI_COLOR ColorKey);

/**
\brief ��ȡ���ڵ�surface���
\attention \n
��
\param[in] Window   ���ھ��
\param[in] pSurface �洢����surface����ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
��
*/
HI_S32 HI_GO_GetWindowSurface(HI_HANDLE Window, HI_HANDLE* pSurface);

/**
\brief ˢ�´���
\attention \n
���´���surface����֮����Ҫ���ô˽ӿڲ�����Ч
\param[in] Window ���ھ��
\param[in] pRect ���µĴ�������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_UpdateWindow(HI_HANDLE Window, HI_RECT* pRect);

/**
\brief ��������ı�����ɫ
\attention \n
��
\param[in] Desktop ������
\param[in] Color ���汳����ɫ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_SetDesktopBGColor(HI_HANDLE Desktop, HI_COLOR Color);

/**
\brief ������������һ��������
\attention \n
��������������ظ�ʽ��һ�£��ƶ����ں���Ҫ�ػ洰��
\param[in] Window ���ھ��
\param[in] Desktop Ŀ��������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_NOMEM

\see \n
��
*/
HI_S32 HI_GO_MoveWindow(HI_HANDLE Window, HI_HANDLE Desktop);

/**
\brief ˢ������
\attention \n
��
\param[in] Desktop ������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_RefreshDesktop(HI_HANDLE Desktop);


/**
\brief ���ڼ���
\attention \n
��
\param[in] Window ���ھ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/

HI_S32 HI_GO_LockWindow(HI_HANDLE Window);

/**
\brief ���ڽ���
\attention \n
��
\param[in] Window ���ھ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/

HI_S32 HI_GO_UnlockWindow(HI_HANDLE Window);

/**
\brief �ͷŴ���surface
\attention \n
�˽ӿڵ�Ŀ����Ϊ�������ش��ڵ�ʱ���ʡ�ڴ���ͷŴ��ڵ��ڴ���Դ�����õġ�
\param[in] Window ���ھ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
HI_GO_RebuildWindowSurface
*/
HI_S32 HI_GO_FreeWindowSurface(HI_HANDLE Window);

/**
\brief �ؽ�����surface
\attention \n
��
\param[in] Window ���ھ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
HI_GO_FreeWindowSurface
*/
HI_S32 HI_GO_RebuildWindowSurface(HI_HANDLE Window);


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_WINC_H__ */
