/**
\file
\brief text模块头文件
\author Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version 1.0
\author s37678
\date 2008-7-21
*/

#ifndef __HI_GO_TEXT_H__
#define __HI_GO_TEXT_H__

/* add include here */
#include "hi_go_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
/** @defgroup hi_go_text_data_structure Text模块 
 *  @ingroup H2 
 *  @brief 详细描述Text模块的数据结构和数据类型
 *  @{  */

#define MAX_CHARSETNAME_LEN 31

#define LAYOUT_LEFT      0x00000001  /**< 水平居左*/
#define LAYOUT_RIGHT     0x00000002  /**< 水平居右*/
#define LAYOUT_HCENTER   0x00000004  /**< 水平居中*/
#define LAYOUT_TOP       0x00000008  /**< 垂直居顶*/
#define LAYOUT_BOTTOM    0x00000010  /**< 垂直居底*/
#define LAYOUT_VCENTER   0x00000020  /**< 垂直居中*/
#define LAYOUT_WRAP      0x00000040  /**< 自动换行*/
#define LAYOUT_WORDELLIPSIS      0x00000080  /**< 使用省略号风格*/


/** 字体属性 */
typedef struct
{
    HI_CHAR Charset[MAX_CHARSETNAME_LEN + 1]; /**< 字符集名称 */
    HI_U8 Height;         /**< 字体高度 */
    HI_U8 MaxWidth;      /**< 字体最大宽度 */
}HIGO_FONTATTR_S;

/** 文本输出对像属性 */
typedef struct
{
    HI_COLOR        BgColor;     /**< 背景色 */
    HI_COLOR        FgColor;     /**< 前景色 */ 
    HIGO_FONTATTR_S SbFontAttr;  /**< 单字节字符集字体属性 */  
    HIGO_FONTATTR_S MbFontAttr;  /**< 多字节字符集字体属性 */    
} HIGO_TEXTOUTATTR_S;

/** @} */  /*! <!-- Structure Definition end */

/******************************* API declaration *****************************/
/** @defgroup hi_go_text_api Text模块
*   @ingroup H1
*   @brief 详细描述Text模块的API函数
*   @{  */

/** 
\brief 创建文本输出对象
\attention \n
pSbFontFile和pMbFontFile不可同时为空
\param[in] pSbFontFile 单字节字符集字体文件，为空表示不使用单字节字符集字库
\param[in] pMbFontFile 多字节字符集字体文件，为空表示不使用多字节字符集字库，但是不能与单字节字符文件同时为空。
\param[out]  pTextOut 文本输出对象句柄

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVFILE

\see \n
::HI_GO_DestroyText
*/
HI_S32 HI_GO_CreateText(HI_CHAR* pSbFontFile,HI_CHAR* pMbFontFile, HI_HANDLE* pTextOut);

/** 
\brief 销毁文本输出对象
\attention \n
无
\param[in] TextOut 文本输出对象句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateText
*/
HI_S32 HI_GO_DestroyText(HI_HANDLE TextOut);

/** 
\brief 获取文本输出对象属性
\attention \n
无
\param[in] TextOut 文本输出对象句柄
\param[out] pTextOutAttr  文本输出对象属性

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_GetTextAttr(HI_HANDLE TextOut,HIGO_TEXTOUTATTR_S *pTextOutAttr);

/** 
\brief 获取文本内容的宽高
\attention \n
无
\param[in] TextOut 文本输出对象句柄
\param[in] pText 文本内容
\param[out] pWidth 文本内容宽度，不可为空
\param[out] pHeight 文本内容高度，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET

\see \n
无
*/
HI_S32 HI_GO_GetTextExtent(HI_HANDLE TextOut,HI_CHAR *pText,HI_S32 *pWidth,HI_S32 *pHeight);

/** 
\brief 设置文本输出对象的背景色
\attention \n
此接口暂时不支持
\param[in] TextOut 文本输出对象句柄
\param[in] Color  文本输出对象背景色，如果最高两位为00(例如0x00FFFFFF)，表示背景色透明

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_SetTextBGColor(HI_HANDLE TextOut,HI_COLOR Color);


/** 
\brief 设置文本输出对象的字体颜色
\attention \n
无
\param[in] TextOut 文本输出对象句柄
\param[in] Color  文本输出对象字体颜色

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_SetTextColor(HI_HANDLE TextOut,HI_COLOR Color);

/** 
\brief 输出文本内容到指定surface
\attention
无
\param[in] TextOut 文本输出对象句柄
\param[in] Surface Surface句柄
\param[in] pText  文本内容
\param[in] pRect  文本输出区域，为空表示从surface原点输出

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVRECT
\retval ::HIGO_ERR_UNSUPPORT_CHARSET
\retval ::HIGO_ERR_NOMEM

\see \n
无
*/
HI_S32 HI_GO_TextOut(HI_HANDLE TextOut,HI_HANDLE Surface,HI_CHAR *pText,HI_RECT *pRect);

/** 
\brief 文本输出扩展接口,输出内容可以按照用户指定的排版进行输出。
\param[in] hTextOut 文本输出句柄
\param[in] hSurface Surface句柄
\param[in] pText    以/0结尾的文本内容
\param[in] pRect    文本输出区域
\param[in] Style    排版格式，如LAYOUT_LEFT、LAYOUT_RIGHT、LAYOUT_HCENTER等

\retval 
无

\see \n
无
*/
HI_S32 HI_GO_TextOutEx(HI_HANDLE hTextOut, HI_HANDLE hSurface,HI_CHAR *pText, HI_RECT *pRect,HI_S32 Style);


/** 
\brief 计算指定文本输出区域字符个数
\param[in] hTextOut 文本输出句柄
\param[in] hSurface Surface句柄
\param[in] pText    以/0结尾的文本内容
\param[in] Width    文本输出区域宽度
\param[in] Height   文本输出区域高度
\param[in] pLen     字符串长度指针

\retval 
无

\see \n
无
*/
HI_S32 HI_GO_GetTextExtentEx(HI_HANDLE TextOut, const HI_CHAR *pText, HI_RECT *pRect, HI_U32 *pLen);


/** @} */  /*! <!-- API declaration end */
#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_TEXT_H__ */

