/******************************************************************************
Copyright (C), 2007-2017, Hisilicon Tech. Co., Ltd.
*******************************************************************************
File Name     : subtitle.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/02/28
Last Modified :
Description   : 字幕解析模块描述头文件
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
typedef  struct  hiUNF_SUBT_COLOR_S      /* 颜色结构*/
{
    HI_U8                  u8red		;		
    HI_U8                  u8green		;
    HI_U8                  u8blue		;	
    HI_U8                  u8alpha		;
}HI_UNF_SUBT_COLOR_S , *HI_UNF_SUBT_COLOR_S_PTR ;

/******************************************************************************/
typedef		struct	hiUNF_SUBT_GFX_DISP_DATA_S
{
	HI_S32		s32len 	;				/* 数据长度 */
	HI_S32		s32bitWidth	;			/* 象素位宽 , 可以为 2,4,8位*/
	HI_U8		*pu8pixData	;			/* 图像数据 */
}HI_UNF_SUBT_GFX_DATA_S , *HI_UNF_SUBT_GFX_DATA_S_PTR ;

/******************************************************************************/
typedef		struct	hiUNF_SUBT_TXT_DATA_S
{
	HI_S32		s32len			;			/* 字符长度 */
	HI_U8		u8bkColor		;			/* 字符背景色 */
	HI_U8		u8frontColor	;			/* 字符前景色 */
	HI_U8       	u8nonModifyColorFlag;   	/*不显示调色板入口值为1点 ，用于透明效果: "挖洞"*/

	HI_U8		*pu8CharData	;				/* 字符数据 */
}HI_UNF_SUBT_TXT_DATA_S , *HI_UNF_SUBT_TXT_DISP_S_PTR ;

/******************************************************************************/
#define		SUBT_CLUT_MAX_ENTRY_NUM		256

/******************************************************************************/
typedef		enum	hiUNF_SUBT_DISP_MSG_TYPE_E
{
	SUBT_DISP_MSG_NORM	,			/* 正常显示数据 */
	SUBT_DISP_MSG_ERASE	,			/* 擦除 */
	SUBT_DISP_MSG_END				/* 整页显示结束 */
}HI_UNF_SUBT_DISP_MSG_TYPE_E , *HI_UNF_SUBT_DISP_MSG_TYPE_E_PTR ; 

/******************************************************************************/
typedef		struct	hiUNF_SUBT_DISP_ELE_S
{
	HI_UNF_SUBT_DISP_MSG_TYPE_E	enMsgType ;	/* 显示消息类型 */	

	HI_S32		s32left	;			/* 显示数据的横坐标 x */
	HI_S32		s32top		;		/* 显示数据的纵坐标 y */
	HI_S32		s32width	;		/* 图形数据的宽度, 字符数据 为0 */
	HI_S32		s32height	;		/* 图形数据的宽度, 字符数据 为0 */

	HI_UNF_SUBT_COLOR_S		stClut[ SUBT_CLUT_MAX_ENTRY_NUM ] ;
	HI_S32				s32bkColor	;		/* 背景色 */

	HI_BOOL				bGfxDataFlag	;	/* 是否为图形数据 	*/
	HI_UNF_SUBT_GFX_DATA_S		stGfxData			;	/* 图形数据 		*/
	HI_UNF_SUBT_TXT_DATA_S		stTxtData			;	/* 字符数据 		*/

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

