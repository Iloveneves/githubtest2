/*******************************************************************************
 *              Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_vo_errcode.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-01-22       z67193    HI_NULL      Create this file.
 ******************************************************************************/
#ifndef __VOMPI_ERRDEF_H__
#define __VOMPI_ERRDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */




/********************************************************
  VO¥ÌŒÛ¬Î∂®“Â
 *********************************************************/


#define MID_VO 0x24/*VO module ID*/

/*VO suberrcode define*/
enum hiVO_ErrorCode_E
{
    ERR_VO_PTR_NULL                  = 0x0,
    ERR_VO_NO_OPEN                   = 0x1,                  
    ERR_VO_OPENED                    = 0x2,                     
    ERR_VO_NO_MEM                    = 0x3,
    ERR_VO_INVALID_PARA              = 0x4,
    ERR_VO_BYTENOALIGN               = 0x5,
    ERR_VO_CLUT_BEYOND               = 0x6,
    ERR_VO_ALPHA                     = 0x7,
    ERR_VO_NO_BUF                    = 0x8,
    ERR_VO_INVALID_INTERFACE         = 0x9,
    ERR_VO_INVALID_OUTPUT_FORMAT     = 0xa,
    
    ERR_VO_OSD_INVALID_REGION        = 0x10,
    ERR_VO_OSD_BEYOND                = 0x11,
    ERR_VO_OSD_MEM_BEYOND            = 0x12,
    ERR_VO_OSD_OVERLAP               = 0x13,
    ERR_VO_OSD_INVALID_PICMOD        = 0x14,
    ERR_VO_OSD_REGION_NUM            = 0x15,
    ERR_VO_OSD_REGION_SIZE           = 0x16,
    ERR_VO_OSD_INVALID_LAYER         = 0x17,
    ERR_VO_OSD_SCREEN_SIZE           = 0x18,
    ERR_VO_OSD_WINDOW_SIZE           = 0x19,
    ERR_VO_OSD_CLIPPING              = 0x1a,
    ERR_VO_OSD_COLORKEY              = 0x1b,
    ERR_VO_OSD_AR                    = 0x1c,
    ERR_VO_OSD_HAS_REGION            = 0x1d,
    ERR_VO_OSD_NOT_INIT              = 0x1e,
    ERR_VO_OSD_HAS_INIT              = 0x1f,
    ERR_VO_OSD_INVALID_ASPECT        = 0x20,
    
    ERR_VO_CURSOR_NO_PARA            = 0x30,
    ERR_VO_CURSOR_INVALID_POSITION   = 0x31,
    
    ERR_VO_MIXER_BRI                 = 0x40,
    ERR_VO_MIXER_CON                 = 0x41,
    ERR_VO_MIXER_SAT                 = 0x42,
    ERR_VO_MIXER_ORDER               = 0x43,

    ERR_DISP_INV_HANDLE         = 0x51, /* at lease one parameter is illegal*/
    ERR_DISP_INV_PARA           = 0x52, /* at lease one parameter is illegal*/
    ERR_DISP_NULL_PTR           = 0x53, /* using a NULL pointer*/
    ERR_DISP_NOT_INIT           = 0x54, /* using a NULL pointer*/
    ERR_DISP_USED               = 0x55,
    ERR_DISP_NOT_SUPPORT        = 0x56,  /*current not supported*/

};

/*VO error code define*/
#define HI_ERR_VO_PTR_NULL\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_PTR_NULL)

#define HI_ERR_VO_OPENED\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OPENED)

#define HI_ERR_VO_NO_OPEN\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_NO_OPEN)

#define HI_ERR_VO_NO_MEM\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_NO_MEM)
    
#define HI_ERR_VO_INVALID_PARA\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_INVALID_PARA)
    
#define HI_ERR_VO_BYTENOALIGN\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_BYTENOALIGN)

#define HI_ERR_VO_CLUT_BEYOND\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_CLUT_BEYOND)

#define HI_ERR_VO_ALPHA\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_ALPHA)

#define HI_ERR_VO_NO_BUF\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_NO_BUF)

#define HI_ERR_VO_INVALID_INTERFACE\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_INVALID_INTERFACE)

#define HI_ERR_VO_INVALID_OUTPUT_FORMAT\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_INVALID_OUTPUT_FORMAT)


/*OSD ERROR CODE*/
#define HI_ERR_VO_OSD_INVALID_REGION\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_INVALID_REGION)

#define HI_ERR_VO_OSD_MEM_BEYOND\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_MEM_BEYOND)
    
#define HI_ERR_VO_OSD_BEYOND\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_BEYOND)

#define HI_ERR_VO_OSD_INVALID_PICMOD\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_INVALID_PICMOD)

#define HI_ERR_VO_OSD_OVERLAP\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_OVERLAP)

#define HI_ERR_VO_OSD_REGION_NUM\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_REGION_NUM)
    
#define HI_ERR_VO_OSD_REGION_SIZE\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_REGION_SIZE)

#define HI_ERR_VO_OSD_INVALID_LAYER\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_INVALID_LAYER)

#define HI_ERR_VO_OSD_SCREEN_SIZE\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_SCREEN_SIZE)

#define HI_ERR_VO_OSD_WINDOW_SIZE\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_WINDOW_SIZE)

#define HI_ERR_VO_OSD_CLIPPING\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_CLIPPING)

#define HI_ERR_VO_OSD_COLORKEY\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_COLORKEY)

#define HI_ERR_VO_OSD_AR\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_AR)

#define HI_ERR_VO_OSD_HAS_REGION\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_HAS_REGION)

#define HI_ERR_VO_OSD_NOT_INIT\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_NOT_INIT)

#define HI_ERR_VO_OSD_HAS_INIT\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_HAS_INIT)

#define HI_ERR_VO_OSD_INVALID_ASPECT\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_OSD_INVALID_ASPECT)


/*CURSOR ERROR CODE*/
#define HI_ERR_VO_CURSOR_NO_PARA\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_CURSOR_NO_PARA)

#define HI_ERR_VO_CURSOR_INVALID_POSITION\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_CURSOR_INVALID_POSITION)


/*MIXER ERROR CODE*/
#define HI_ERR_VO_MIXER_BRI\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_MIXER_BRI)

#define HI_ERR_VO_MIXER_CON\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_MIXER_CON)

#define HI_ERR_VO_MIXER_SAT\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_MIXER_SAT)

#define HI_ERR_VO_MIXER_ORDER\
    HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_VO_MIXER_ORDER)
    
#define HI_UNF_ERR_DISP_INV_HANDLE HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_DISP_INV_HANDLE)
#define HI_UNF_ERR_DISP_INV_PARA HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_DISP_INV_PARA)
#define HI_UNF_ERR_DISP_NULL_PTR HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_DISP_NULL_PTR)
#define HI_UNF_ERR_DISP_NOT_INIT HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_DISP_NOT_INIT)
#define HI_UNF_ERR_DISP_USED HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_DISP_USED)
#define HI_UNF_ERR_DISP_NOTSUPPORT HI_DEF_ERR(MID_VO, HI_LOG_LEVEL_ERROR, ERR_DISP_NOT_SUPPORT)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __VOMPI_ERRDEF_H__ */

