/**
 \file
 \brief Gdevģ��ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author w66592
 \date 2008-5-16
 */

#ifndef __HI_GO_GDEV_H__
#define __HI_GO_GDEV_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/
/** @defgroup hi_go_gdev_data_structure Gdevģ�� 
 *  @ingroup H2 
 *  @brief ��ϸ����Gdevģ������ݽṹ����������
 *  @{  */

/** Ӳ��ͼ��ID�ĺ궨�� */
/** ��Ƶ�� */
#define HIGO_LAYER_VIDEO0 0

/** ͼ�ε��Ӳ�0 */
#define HIGO_LAYER_RGB0 1

/** ͼ�ε��Ӳ�1 */
#define HIGO_LAYER_RGB1 2

/** Ӳ������0 */
#define HIGO_LAYER_CURSOR0 3


/**ͼ���ˢ�·�ʽ*/
typedef enum 
{
    HIGO_LAYER_FLUSH_ANTIFLICKER = 0x01,     /**< ֧�ֿ���˸,���ܵ������ڱ����FLIP��DOUBBUFFERһ����� */
    HIGO_LAYER_FLUSH_FLIP = 0x02,            /**< ֧��flip */
    HIGO_LAYER_FLUSH_DOUBBUFER = 0x04,       /**< ֧��˫���� */
    HIGO_LAYER_FLUSH_NORMAL = 0x08,          /**< ��BUFFERģʽ,���ڵ��Ի���,ֻ�ܵ���ʹ��,���ܺ�����ģʽ���� */    
    HIGO_LAYER_FLUSH_OVER = 0x10,            /**< ���¹���Ƶ����ʱ��֡����ֱ�Ӹ��ǣ�Ĭ�ϱ�ʾ��֡��OVER��ʾ���и���*/  
    HIGO_LAYER_FLUSH_BUTT
} HIGO_LAYER_FLUSHTYPE_E;

/**ͼ�����Բ���*/
typedef struct 
{
    HI_S32                 Width;          /**< ͼ�������߶ȣ������Դ�Ĵ������������0 */
    HI_S32                 Height;         /**< ͼ��������ȣ������Դ�Ĵ������������0 */
    HIGO_LAYER_FLUSHTYPE_E LayerFlushType; /**< ͼ���ˢ�·�ʽ���û��ɸ���ʹ�ó���ѡ��ͬ��ˢ��ģʽ�����ˢ��Ч��,����0ʱĬ��ʹ��˫����+Flipˢ��ģʽ */
    HI_U32                 FlipCount;      /**< ����Flip���ڴ������С��2ʱĬ��Ϊ2 */
    HIGO_PF_E              PixelFormat;    /**< ͼ������ظ�ʽ���˸�ʽ����ΪӲ��ͼ����֧�ֵĸ�ʽ������ݲ�ͬӲ���豸��ѡ��ͼ������ظ�ʽ���� */
    HI_U32                 LayerID;        /**< ͼ��Ӳ��ID��Ŀǰ��֧��2��ͼ�ε��Ӳ㣬��HIGO_LAYER_RGB0��HIGO_LAYER_RGB1 */
} HIGO_LAYER_INFO_S;

/**Z������ķ�ʽ*/
typedef enum 
{
    HIGO_LAYER_ZORDER_MOVETOP = 0,  /**<�Ƶ����*/
    HIGO_LAYER_ZORDER_MOVEUP,       /**<�����Ƶ�*/
    HIGO_LAYER_ZORDER_MOVEBOTTOM,   /**<�Ƶ���ײ�*/
    HIGO_LAYER_ZORDER_MOVEDOWN,     /**<�����Ƶ�*/
    HIGO_LAYER_ZORDER_BUTT
} HIGO_LAYER_ZORDER_E;


/** ͼ��״̬�ṹ */
typedef struct 
{
    HI_BOOL bShow;             /**< ͼ���Ƿ���ʾ */
    HI_BOOL bEnableCK;         /**< ͼ���Ƿ�ʹ��colorkey */
} HIGO_LAYER_STATUS_S;

/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** @defgroup hi_go_gdev_api Gdevģ��
*   @ingroup H1
*   @brief ��ϸ����Gdevģ���API����
*   @{  */

/** 
\brief ��ʼ����ʾ�豸
\attention \n
::HI_GO_Init�Ѱ����˶Ըýӿڵĵ���
\param ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INITFAILED

\see \n
::HI_GO_Init \n
::HI_GO_DeinitDisplay
*/
HI_S32 HI_GO_InitDisplay(HI_VOID);

/** 
\brief ȥ��ʼ����ʾ�豸
\attention \n
::HI_GO_Deinit�Ѱ����˶Ըýӿڵĵ���
\param ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_Deinit \n
::HI_GO_InitDisplay
*/
HI_S32 HI_GO_DeinitDisplay(HI_VOID);

/** 
\brief ����ͼ��
\attention \n
��
\param[in]  pLayerInfo ͼ��������ԣ�����Ϊ��
\param[out] pLayer ͼ��ľ��ָ�룬����Ϊ��

\retval ::HIGO_ERR_NOTINIT 
\retval ::HIGO_ERR_NULLPTR 
\retval ::HIGO_ERR_INVSIZE 
\retval ::HIGO_ERR_INVLAYERID 
\retval ::HIGO_ERR_INVPIXELFMT 
\retval ::HIGO_ERR_INVFLUSHTYPE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_DestroyLayer
*/
HI_S32 HI_GO_CreateLayer (const HIGO_LAYER_INFO_S *pLayerInfo, HI_HANDLE* pLayer);

/** 
\brief ����ͼ��
\attention \n
��ͼ�����ڱ�ʹ��ʱ��������ڴ�ͼ�������δ���٣�����ͼ���޷�������
\param[in] Layer ͼ����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_CLOSELAYERFAILED
\retval ::HIGO_ERR_FREEMEM
\retval ::HIGO_ERR_INUSE


\see \n
::HI_GO_CreateLayer
*/
HI_S32 HI_GO_DestroyLayer(HI_HANDLE Layer);


/** 
\brief ����ͼ������Ļ�е�λ��
\attention \n
������Ч������ˢ�¡������õ�ͼ�����ʼλ�ó�����Ļ��Χʱ���Զ���������Ļ�ı߽�ֵ
\param[in] Layer  ͼ����
\param[in] StartX ͼ������Ļ�ϵ���ʾλ��X����
\param[in] StartY ͼ������Ļ�ϵ���ʾλ��Y����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVLAYERPOS
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_GetLayerPos
*/
HI_S32 HI_GO_SetLayerPos(HI_HANDLE Layer, HI_U32 StartX, HI_U32 StartY);

/** 
\brief ��ȡͼ������Ļ�ϵ�λ��
\attention \n
��
\param[in] Layer ͼ����
\param[out] pStartX ͼ������Ļ�ϵ���ʾλ��X����ָ�룬����Ϊ��
\param[out] pStartY ͼ������Ļ�ϵ���ʾλ��Y����ָ�룬����Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_SetLayerPos
*/
HI_S32 HI_GO_GetLayerPos(HI_HANDLE Layer, HI_U32 *pStartX, HI_U32 *pStartY);


/** 
\brief �����������ʾ�豸�ϵ�����ĸ߶ȺͿ�ȡ�
\attention \n
�ýӿ�ֻ�ܽ�����С����
\param[in]  Layer ͼ����
\param[in] SWidth  ʵ�������ȣ�Ĭ��Ϊ0��ʾ����������ͬ,�����������Ƚ�����С,����ʵ�ʿ�Ƚ������
\param[in] SHeight ʵ������߶ȣ�Ĭ��Ϊ0��ʾ������߶���ͬ,����������߶Ƚ�����С,����ʵ�ʸ߶Ƚ������ 
\retval none

\see \n
::HI_GO_GetScreenSize
*/
HI_S32 HI_GO_SetScreenSize(HI_HANDLE Layer, HI_U32 SWidth, HI_U32 SHeight);


/** 
\brief ��ȡ�������ʾ�豸�ϵ�����ĸ߶ȺͿ�ȡ�
\attention \n
�ýӿ�ֻ�ܽ�����С����,����ֻ����˫����ģʽ�Ϳ���˸ģʽ����Ч��
\param[in]  Layer ͼ����
\param[out] pSWidth  ʵ��������ָ�룬Ĭ��Ϊ0��ʾ����������ͬ,�����������Ƚ�����С,����ʵ�ʿ�Ƚ������
\param[out] pSHeight ʵ������߶�ָ�룬Ĭ��Ϊ0��ʾ������߶���ͬ,����������߶Ƚ�����С,����ʵ�ʸ߶Ƚ������ 
\retval none

\see \n
::HI_GO_SetScreenSize
*/
HI_S32 HI_GO_GetScreenSize(HI_HANDLE Layer, HI_U32 *pSWidth, HI_U32 *pSHeight);

/** 
\brief ����ͼ��surface��ȫ��alphaֵ
\attention \n
������Ч������ˢ��
\param[in] Layer ͼ����
\param[in] Alpha ȫ��alphaֵ������Ϊ�գ���Χ0-255

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_SETALPHAFAILED
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_GetLayerAlpha
*/

HI_S32 HI_GO_SetLayerAlpha(HI_HANDLE Layer, HI_U8 Alpha);

/** 
\brief ��ȡͼ��surface��ȫ��alphaֵ
\attention \n
��
\param[in] Layer ͼ����
\param[out] Alpha ȫ��alphaָ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetLayerAlpha
*/
HI_S32 HI_GO_GetLayerAlpha(HI_HANDLE Layer, HI_U8* Alpha);

/** 
\brief �ı�ͼ���Z��
\attention \n
�ù�����ҪӲ��֧��Z����޸ģ�������Ч������ˢ��
\param[in] Layer ͼ����
\param[in] ZFlag �޸�Z���־

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVORDERFLAG
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_CANNOTCHANGE

\see \n
::HI_GO_GetLayerZorder
*/
HI_S32 HI_GO_ChangeLayerZorder(HI_HANDLE Layer, HIGO_LAYER_ZORDER_E ZFlag);

/** 
\brief ��ȡͼ���Z��
\attention \n
Z��ԽС��ͼ��Խ����
\param[in] Layer ͼ����
\param[out] pZOrder ͼ����Ӳ�����ϵ�Z��ָ�룬����Ϊ�� 

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_ChangeLayerZorder
*/
HI_S32 HI_GO_GetLayerZorder(HI_HANDLE Layer, HI_U32 *pZOrder);

/** 
\brief ��ȡͼ���surface
\attention \n
ͼ��surface������ʹ��HI_GO_FreeSurface���ͷš�ֻ��������ͼ����ʱ��Żᱻ�ͷ�
\param[in] Layer ͼ����
\param[out] pSurface surface���ָ�룬����Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_GetLayerSurface(HI_HANDLE Layer, HI_HANDLE *pSurface);

/** 
\brief ��ʾ������ͼ��
\attention \n
������Ч,����ˢ��
\param[in] Layer ͼ����
\param[in] bVisbile ��ʾ���ر�־��HI_TRUE����ʾ��HI_FALSE������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_ALREADYSHOW
\retval ::HIGO_ERR_ALREADYHIDE
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_GetLayerStatus
*/
HI_S32 HI_GO_ShowLayer(HI_HANDLE Layer, HI_BOOL bVisbile);

/** 
\brief ��ȡͼ�㵱ǰ״̬
\attention \n
��
\param[in] Layer ͼ����
\param[out] pLayerStatus ͼ�㵱ǰ״̬�ṹָ�룬����Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_ShowLayer
*/
HI_S32 HI_GO_GetLayerStatus(HI_HANDLE Layer, HIGO_LAYER_STATUS_S* pLayerStatus);

/** 
\brief ˢ��ͼ��
\attention \n
������ɺ���ˢ��ͼ�������ʾ���ƺ�����
ע��˽ӿڵ���Ƶ�ȹ��ߣ��п��Է���ʧ�ܣ��û����Բ��öԴ�������д���
\param[in] Layer ͼ����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INTERNAL

\see \n
��
*/
HI_S32 HI_GO_RefreshLayer(HI_HANDLE Layer);


/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_GDEV_H__ */
