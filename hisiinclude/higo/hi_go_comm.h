/**
 \file
 \brief HiGo��������ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-5-16
 */

#ifndef __HI_GO_COMM_H__
#define __HI_GO_COMM_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/** @defgroup hi_go_common_data_structure ������������ 
 *  @ingroup H2 
 *  @brief ��ϸ������ģ�鹫�õ����ݽṹ����������
 *  @{  */

/** ��ɫ�������ɫ�� */
#define MAX_PALETTE_COLOR_SIZE 256

/** ��ɫֵ */
typedef HI_U32 HI_COLOR;

/** ��ɫ�� */
typedef HI_COLOR HI_PALETTE[MAX_PALETTE_COLOR_SIZE];


/** ���� */
typedef struct
{
    HI_S32 x, y;

    HI_S32 w, h;
} HI_RECT;

/** ���� */
typedef struct
{
    HI_S32 l;
    HI_S32 t;
    HI_S32 r;
    HI_S32 b;
} HI_REGION;

/** ��ʽλ��*/


/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/** @defgroup hi_go_common_api SYSģ��
*   @ingroup H1
*   @brief ��ϸ����SYSģ���API����
*   @{  */

/** 
\brief HiGo��ʼ��
\attention \n
ʹ��HiGo���ܱ����ȵ��øýӿڣ����HiGo�ĳ�ʼ��
\param ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INITFAILED

\see \n
::HI_GO_Deinit
*/
HI_S32	HI_GO_Init(HI_VOID);

/** 
\brief HiGoȥ��ʼ��
\attention \n
����ʹ��HiGo����ʱ��Ҫ���øýӿڣ��ͷ���Դ
\param ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_Init
*/
HI_S32	HI_GO_Deinit(HI_VOID);

/** 
\brief ��ȡ�汾��Ϣ
\attention \n
��
\param[out] ppVersion  �汾��Ϣ�ַ��������ַ������Ϊ��
\param[out] ppBuildTime Buildʱ���ַ��������ַ������Ϊ��

\retval ��

\see \n
��
*/
HI_VOID HI_GO_GetVersion(HI_CHAR ** ppVersion, HI_CHAR **ppBuildTime);

/** 
\brief ��Surface�е�����ת����BMP��ʽͼ�������
\attention \n
����ļ���Ϊ[year]-[date]-[hour]-[min]-[second]-[ms].bmp
���ͼƬ�̶�Ϊ16λͼ��
\param[in] Surface��Ҫ���н��������ݡ�
\param[in] pRect����ָ�롣
\retval none
\return none
*/
HI_S32 HI_GO_Surface2Bmp(HI_HANDLE Surface, HI_RECT *pRect);


/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_COMM_H__ */
