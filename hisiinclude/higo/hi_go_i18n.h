/**
 \file
 \brief i18n����ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author y57480
 \date 2008-9-5
 */

#ifndef __HI_GO_I18N_H__
#define __HI_GO_I18N_H__

/* add include here */
#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/
/** @defgroup hi_go_i18n_data_structure I18Nģ�� 
 *  @ingroup H2 
 *  @brief ��ϸ����I18Nģ������ݽṹ����������
 *  @{  */

/**
 * @brief RFC 2278�ж�����ַ���
 * 
 */
typedef enum hiI18N_Charset_E
{
    I18N_Charset_GBK = 0,
    I18N_Charset_Default,
    I18N_Charset_BUTT
} I18N_Charset_E;

/**
 * @brief ISO 639�ж��������ĸ������д
 * 
 */
typedef enum hiI18N_Lang_E
{
    I18N_Lang_zh = 0,
    I18N_Lang_Default,
    I18N_Lang_BUTT
} I18N_Lang_E;

/**
 * @brief ISO 3166�ж��������ĸ������д
 * 
 */
typedef enum hiI18N_Country_E
{
    I18N_Country_CN = 0,
    I18N_Country_Default,
    I18N_Country_BUTT
} I18N_Country_E;
/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/** @defgroup hi_go_i18n_api I18Nģ��
*   @ingroup H1
*   @brief ��ϸ����I18Nģ���API����
*   @{  */


/** 
\brief ����I18Nʵ��
\attention \n
��
\param[out] pI18N �洢I18Nʵ������ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_DestroyI18N
*/

HI_S32 HI_GO_CreateI18N(HI_HANDLE* pI18N);


/** 
\brief ����I18N Locale��Ϣ
\attention \n
Locale����������ɣ����ҡ������Լ��ַ���
\param[in] I18N  I18Nʵ�����
\param[in] Country Locale�еĹ�����Ϣ
\param[in] Lang    Locale�е�������Ϣ
\param[in] Charset Locale�е��ַ�����Ϣ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM

\see \n
::HI_GO_GetLocale
*/

HI_S32 HI_GO_SetLocale(HI_HANDLE I18N, I18N_Country_E Country, I18N_Lang_E Lang, I18N_Charset_E Charset);

/** 
\brief ��ȡI18N��Locale��Ϣ
\attention \n
country��lang��charset���߲���ͬʱΪ��
\param[in]  I18N   I18Nʵ�����
\param[out] Country Locale�еĹ�����Ϣ��Ϊ�ձ�ʾ����Ҫ��ȡ����Ϣ
\param[out] Lang    Locale�е�������Ϣ��Ϊ�ձ�ʾ����Ҫ��ȡ����Ϣ
\param[out] Charset Locale�е��ַ�����Ϣ��Ϊ�ձ�ʾ����Ҫ��ȡ����Ϣ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetLocale
*/
HI_S32 HI_GO_GetLocale(HI_HANDLE I18N, I18N_Country_E* Country, I18N_Lang_E* Lang, I18N_Charset_E* Charset);

/** 
\brief ���ö�������Դ�ļ�
\attention \n
����ʹ�ñ��ӿ���ָ��ģ��ʹ�õ���Դ�ļ����ܻ�ȡ�����ַ���
\param[in] I18N I18Nʵ���ľ��
\param[in] pTextDomain ��Դ�ļ���������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_NOMEM

\see \n
��
*/
HI_S32 HI_GO_SetTextDomain(HI_HANDLE I18N, const HI_CHAR* pTextDomain);

/** 
\brief ����ԭʼ�ַ�����ȡ���������ַ���
\attention \n
��û��ָ����Դ�ļ�����ָ������Դ�ļ����Ҳ�������򷵻�ԭʼ�ַ���
\param[in] I18N  I18Nʵ���ľ��
\param[in] pText ԭʼ�ַ���

\retval ���ر��������ַ�����ԭʼ�ַ���

\see \n
��
*/
const HI_CHAR* HI_GO_GetLocalText(HI_HANDLE I18N, const HI_CHAR* pText);

/** 
\brief �ͷ�I18Nʵ��
\attention \n
��
\param[in] I18N I18Nʵ���ľ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateI18N
*/
HI_S32 HI_GO_DestroyI18N(HI_HANDLE I18N);


/** @} */  /*! <!-- API declaration end */



#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_I18N_H__ */
