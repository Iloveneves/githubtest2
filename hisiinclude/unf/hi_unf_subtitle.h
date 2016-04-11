/******************************************************************************
Copyright (C), 2007-2017, Hisilicon Tech. Co., Ltd.
*******************************************************************************
File Name     : subtitle.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/02/28
Last Modified :
Description   : ��Ļ����ģ������ͷ�ļ�
Function List :
History       :
******************************************************************************/
#ifndef __HI_UNF_SUBT_H__
#define __HI_UNF_SUBT_H__
/******************************************************************************/
#include "hi_type.h"
#include "hi_unf_common.h"
/******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
typedef  struct  hiUNF_SUBT_COLOR_S      /* ��ɫ�ṹ*/
{
    HI_U8                  u8red		;		
    HI_U8                  u8green		;
    HI_U8                  u8blue		;	
    HI_U8                  u8alpha		;
}HI_UNF_SUBT_COLOR_S , *HI_UNF_SUBT_COLOR_S_PTR ;

/******************************************************************************/
typedef		struct	hiUNF_SUBT_GFX_DISP_DATA_S
{
	HI_S32		s32len 	;				/* ���ݳ��� */
	HI_S32		s32bitWidth	;			/* ����λ�� , ����Ϊ 2,4,8λ*/
	HI_U8		*pu8pixData	;			/* ͼ������ */
}HI_UNF_SUBT_GFX_DATA_S , *HI_UNF_SUBT_GFX_DATA_S_PTR ;

/******************************************************************************/
typedef		struct	hiUNF_SUBT_TXT_DATA_S
{
	HI_S32		s32len			;			/* �ַ����� */
	HI_U8		u8bkColor		;			/* �ַ�����ɫ */
	HI_U8		u8frontColor	;			/* �ַ�ǰ��ɫ */
	HI_U8       	u8nonModifyColorFlag;   	/*����ʾ��ɫ�����ֵΪ1�� ������͸��Ч��: "�ڶ�"*/

	HI_U8		*pu8CharData	;				/* �ַ����� */
}HI_UNF_SUBT_TXT_DATA_S , *HI_UNF_SUBT_TXT_DISP_S_PTR ;

/******************************************************************************/
#define		SUBT_CLUT_MAX_ENTRY_NUM		256

/******************************************************************************/
typedef		enum	hiUNF_SUBT_DISP_MSG_TYPE_E
{
	SUBT_DISP_MSG_NORM	,			/* ������ʾ���� */
	SUBT_DISP_MSG_ERASE	,			/* ���� */
	SUBT_DISP_MSG_END				/* ��ҳ��ʾ���� */
}HI_UNF_SUBT_DISP_MSG_TYPE_E , *HI_UNF_SUBT_DISP_MSG_TYPE_E_PTR ; 

/******************************************************************************/
typedef		struct	hiUNF_SUBT_DISP_ELE_S
{
	HI_UNF_SUBT_DISP_MSG_TYPE_E	enMsgType ;	/* ��ʾ��Ϣ���� */	

	HI_S32		s32left	;			/* ��ʾ���ݵĺ����� x */
	HI_S32		s32top		;		/* ��ʾ���ݵ������� y */
	HI_S32		s32width	;		/* ͼ�����ݵĿ��, �ַ����� Ϊ0 */
	HI_S32		s32height	;		/* ͼ�����ݵĿ��, �ַ����� Ϊ0 */

	HI_UNF_SUBT_COLOR_S		stClut[ SUBT_CLUT_MAX_ENTRY_NUM ] ;
	HI_S32				s32bkColor	;		/* ����ɫ */

	HI_BOOL				bGfxDataFlag	;	/* �Ƿ�Ϊͼ������ 	*/
	HI_UNF_SUBT_GFX_DATA_S		stGfxData			;	/* ͼ������ 		*/
	HI_UNF_SUBT_TXT_DATA_S		stTxtData			;	/* �ַ����� 		*/

}HI_UNF_SUBT_DISP_ELE_S , *HI_UNF_SUBT_DISP_ELE_S_PTR;
/******************************************************************************/
 

/******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
/******************************************************************************/

#endif

