/**
 \file
 \brief i18n对外头文件
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
/** @defgroup hi_go_i18n_data_structure I18N模块 
 *  @ingroup H2 
 *  @brief 详细描述I18N模块的数据结构和数据类型
 *  @{  */

/**
 * @brief RFC 2278中定义的字符集
 * 
 */
typedef enum hiI18N_Charset_E
{
    I18N_Charset_GBK = 0,
    I18N_Charset_Default,
    I18N_Charset_BUTT
} I18N_Charset_E;

/**
 * @brief ISO 639中定义的两字母语言缩写
 * 
 */
typedef enum hiI18N_Lang_E
{
    I18N_Lang_zh = 0,
    I18N_Lang_Default,
    I18N_Lang_BUTT
} I18N_Lang_E;

/**
 * @brief ISO 3166中定义的两字母国家缩写
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

/** @defgroup hi_go_i18n_api I18N模块
*   @ingroup H1
*   @brief 详细描述I18N模块的API函数
*   @{  */


/** 
\brief 创建I18N实例
\attention \n
无
\param[out] pI18N 存储I18N实例句柄的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_DestroyI18N
*/

HI_S32 HI_GO_CreateI18N(HI_HANDLE* pI18N);


/** 
\brief 设置I18N Locale信息
\attention \n
Locale由三部分组成：国家、语言以及字符集
\param[in] I18N  I18N实例句柄
\param[in] Country Locale中的国家信息
\param[in] Lang    Locale中的语言信息
\param[in] Charset Locale中的字符集信息

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM

\see \n
::HI_GO_GetLocale
*/

HI_S32 HI_GO_SetLocale(HI_HANDLE I18N, I18N_Country_E Country, I18N_Lang_E Lang, I18N_Charset_E Charset);

/** 
\brief 获取I18N的Locale信息
\attention \n
country、lang和charset三者不能同时为空
\param[in]  I18N   I18N实例句柄
\param[out] Country Locale中的国家信息，为空表示不需要获取该信息
\param[out] Lang    Locale中的语言信息，为空表示不需要获取该信息
\param[out] Charset Locale中的字符集信息，为空表示不需要获取该信息

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetLocale
*/
HI_S32 HI_GO_GetLocale(HI_HANDLE I18N, I18N_Country_E* Country, I18N_Lang_E* Lang, I18N_Charset_E* Charset);

/** 
\brief 设置多语言资源文件
\attention \n
必须使用本接口来指定模块使用的资源文件才能获取本地字符串
\param[in] I18N I18N实例的句柄
\param[in] pTextDomain 资源文件名，不能为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_NOMEM

\see \n
无
*/
HI_S32 HI_GO_SetTextDomain(HI_HANDLE I18N, const HI_CHAR* pTextDomain);

/** 
\brief 根据原始字符串获取本地语言字符串
\attention \n
若没有指定资源文件或者指定的资源文件中找不到此项，则返回原始字符串
\param[in] I18N  I18N实例的句柄
\param[in] pText 原始字符串

\retval 返回本地语言字符串、原始字符串

\see \n
无
*/
const HI_CHAR* HI_GO_GetLocalText(HI_HANDLE I18N, const HI_CHAR* pText);

/** 
\brief 释放I18N实例
\attention \n
无
\param[in] I18N I18N实例的句柄

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
