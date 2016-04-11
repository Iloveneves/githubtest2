/**
\file
\brief textģ��ͷ�ļ�
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
/** @defgroup hi_go_text_data_structure Textģ�� 
 *  @ingroup H2 
 *  @brief ��ϸ����Textģ������ݽṹ����������
 *  @{  */

#define MAX_CHARSETNAME_LEN 31

#define LAYOUT_LEFT      0x00000001  /**< ˮƽ����*/
#define LAYOUT_RIGHT     0x00000002  /**< ˮƽ����*/
#define LAYOUT_HCENTER   0x00000004  /**< ˮƽ����*/
#define LAYOUT_TOP       0x00000008  /**< ��ֱ�Ӷ�*/
#define LAYOUT_BOTTOM    0x00000010  /**< ��ֱ�ӵ�*/
#define LAYOUT_VCENTER   0x00000020  /**< ��ֱ����*/
#define LAYOUT_WRAP      0x00000040  /**< �Զ�����*/
#define LAYOUT_WORDELLIPSIS      0x00000080  /**< ʹ��ʡ�Ժŷ��*/


/** �������� */
typedef struct
{
    HI_CHAR Charset[MAX_CHARSETNAME_LEN + 1]; /**< �ַ������� */
    HI_U8 Height;         /**< ����߶� */
    HI_U8 MaxWidth;      /**< ��������� */
}HIGO_FONTATTR_S;

/** �ı������������ */
typedef struct
{
    HI_COLOR        BgColor;     /**< ����ɫ */
    HI_COLOR        FgColor;     /**< ǰ��ɫ */ 
    HIGO_FONTATTR_S SbFontAttr;  /**< ���ֽ��ַ����������� */  
    HIGO_FONTATTR_S MbFontAttr;  /**< ���ֽ��ַ����������� */    
} HIGO_TEXTOUTATTR_S;

/** @} */  /*! <!-- Structure Definition end */

/******************************* API declaration *****************************/
/** @defgroup hi_go_text_api Textģ��
*   @ingroup H1
*   @brief ��ϸ����Textģ���API����
*   @{  */

/** 
\brief �����ı��������
\attention \n
pSbFontFile��pMbFontFile����ͬʱΪ��
\param[in] pSbFontFile ���ֽ��ַ��������ļ���Ϊ�ձ�ʾ��ʹ�õ��ֽ��ַ����ֿ�
\param[in] pMbFontFile ���ֽ��ַ��������ļ���Ϊ�ձ�ʾ��ʹ�ö��ֽ��ַ����ֿ⣬���ǲ����뵥�ֽ��ַ��ļ�ͬʱΪ�ա�
\param[out]  pTextOut �ı����������

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
\brief �����ı��������
\attention \n
��
\param[in] TextOut �ı����������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateText
*/
HI_S32 HI_GO_DestroyText(HI_HANDLE TextOut);

/** 
\brief ��ȡ�ı������������
\attention \n
��
\param[in] TextOut �ı����������
\param[out] pTextOutAttr  �ı������������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_GetTextAttr(HI_HANDLE TextOut,HIGO_TEXTOUTATTR_S *pTextOutAttr);

/** 
\brief ��ȡ�ı����ݵĿ��
\attention \n
��
\param[in] TextOut �ı����������
\param[in] pText �ı�����
\param[out] pWidth �ı����ݿ�ȣ�����Ϊ��
\param[out] pHeight �ı����ݸ߶ȣ�����Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET

\see \n
��
*/
HI_S32 HI_GO_GetTextExtent(HI_HANDLE TextOut,HI_CHAR *pText,HI_S32 *pWidth,HI_S32 *pHeight);

/** 
\brief �����ı��������ı���ɫ
\attention \n
�˽ӿ���ʱ��֧��
\param[in] TextOut �ı����������
\param[in] Color  �ı�������󱳾�ɫ����������λΪ00(����0x00FFFFFF)����ʾ����ɫ͸��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_SetTextBGColor(HI_HANDLE TextOut,HI_COLOR Color);


/** 
\brief �����ı���������������ɫ
\attention \n
��
\param[in] TextOut �ı����������
\param[in] Color  �ı��������������ɫ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
��
*/
HI_S32 HI_GO_SetTextColor(HI_HANDLE TextOut,HI_COLOR Color);

/** 
\brief ����ı����ݵ�ָ��surface
\attention
��
\param[in] TextOut �ı����������
\param[in] Surface Surface���
\param[in] pText  �ı�����
\param[in] pRect  �ı��������Ϊ�ձ�ʾ��surfaceԭ�����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVRECT
\retval ::HIGO_ERR_UNSUPPORT_CHARSET
\retval ::HIGO_ERR_NOMEM

\see \n
��
*/
HI_S32 HI_GO_TextOut(HI_HANDLE TextOut,HI_HANDLE Surface,HI_CHAR *pText,HI_RECT *pRect);

/** 
\brief �ı������չ�ӿ�,������ݿ��԰����û�ָ�����Ű���������
\param[in] hTextOut �ı�������
\param[in] hSurface Surface���
\param[in] pText    ��/0��β���ı�����
\param[in] pRect    �ı��������
\param[in] Style    �Ű��ʽ����LAYOUT_LEFT��LAYOUT_RIGHT��LAYOUT_HCENTER��

\retval 
��

\see \n
��
*/
HI_S32 HI_GO_TextOutEx(HI_HANDLE hTextOut, HI_HANDLE hSurface,HI_CHAR *pText, HI_RECT *pRect,HI_S32 Style);


/** 
\brief ����ָ���ı���������ַ�����
\param[in] hTextOut �ı�������
\param[in] hSurface Surface���
\param[in] pText    ��/0��β���ı�����
\param[in] Width    �ı����������
\param[in] Height   �ı��������߶�
\param[in] pLen     �ַ�������ָ��

\retval 
��

\see \n
��
*/
HI_S32 HI_GO_GetTextExtentEx(HI_HANDLE TextOut, const HI_CHAR *pText, HI_RECT *pRect, HI_U32 *pLen);


/** @} */  /*! <!-- API declaration end */
#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_TEXT_H__ */

