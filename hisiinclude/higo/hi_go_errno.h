/**
 \file
 \brief HiGo�������ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author ��z00141204
 \date 2009-9-14
 */

#ifndef __HI_GO_ERRNO_H__
#define __HI_GO_ERRNO_H__

/* add include here */
#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
/** @defgroup hi_go_error_code HiGo������
 *  @ingroup H3 
 *  @brief ��ϸ����HiGo������
 *  @{  */

/** HiGo ��Ŀ����ID */
#define HIGO_ERR_APPID (0x80000000L + 0x30000000L)

typedef enum hiGOLOG_ERRLEVEL_E
{
    HIGO_LOG_LEVEL_DEBUG = 0,  /* debug-level                                  */
    HIGO_LOG_LEVEL_INFO,       /* informational                                */
    HIGO_LOG_LEVEL_NOTICE,     /* normal but significant condition             */
    HIGO_LOG_LEVEL_WARNING,    /* warning conditions                           */
    HIGO_LOG_LEVEL_ERROR,      /* error conditions                             */
    HIGO_LOG_LEVEL_CRIT,       /* critical conditions                          */
    HIGO_LOG_LEVEL_ALERT,      /* action must be taken immediately             */
    HIGO_LOG_LEVEL_FATAL,      /* just for compatibility with previous version */
    HIGO_LOG_LEVEL_BUTT
} HIGO_LOG_ERRLEVEL_E;

/** HiGo �����붨��� */
#define HIGO_DEF_ERR( module, errid) \
    ((HI_S32)((HIGO_ERR_APPID) | ((module) << 16) | ((HIGO_LOG_LEVEL_ERROR) << 13) | (errid)))

/** HiGo ģ����� */
typedef enum
{
    HIGO_MOD_COMM = 0,
    HIGO_MOD_SURFACE,
    HIGO_MOD_MEMSURFACE,
    HIGO_MOD_LAYER,
    HIGO_MOD_BLITER,
    HIGO_MOD_DEC,
    HIGO_MOD_TEXTOUT,
    HIGO_MOD_WINC,
    HIGO_MOD_DESKTOP, 
    HIGO_MOD_I18N,
    HIGO_MOD_BUTT
} HIGO_MOD_E;

/** HiGo ���������� */
typedef enum
{
    ERR_COMM_NOTINIT = 0,
    ERR_COMM_INITFAILED,
    ERR_COMM_DEINITFAILED,
    ERR_COMM_NULLPTR,
    ERR_COMM_INVHANDLE,
    ERR_COMM_NOMEM,
    ERR_COMM_INTERNAL,
    ERR_COMM_INVSRCTYPE,
    ERR_COMM_INVFILE,
    ERR_COMM_INVPARAM,
    ERR_COMM_INUSE,
	ERR_BLITER_UNSUPPORTED,
    ERR_COMM_BUTT
} HIGO_ERR_E;

/** ��������ģ��δ��ʼ�� 0xB0008000 */
#define HIGO_ERR_NOTINIT HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_NOTINIT)

/** ģ���ʼ��ʧ�� 0xB0008001 */
#define HIGO_ERR_DEINITFAILED HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INITFAILED)

/** ģ��ȥ��ʼ��ʧ�� 0xB0008002 */
#define HIGO_ERR_INITFAILED HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_DEINITFAILED)

/** �������Ϊ��ָ�� 0xB0008003 */
#define HIGO_ERR_NULLPTR HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_NULLPTR)

/** ������Ч�ľ�� 0xB0008004 */
#define HIGO_ERR_INVHANDLE HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INVHANDLE)

/** �ڴ治�� 0xB0008005 */
#define HIGO_ERR_NOMEM HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_NOMEM)

/** �ڲ����� 0xB0008006 */
#define HIGO_ERR_INTERNAL HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INTERNAL)

/** ��Ч��IO��Դ 0xB0008007 */
#define HIGO_ERR_INVSRCTYPE HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INVSRCTYPE)

/** ��Ч���ļ����ļ�����ʧ�� 0xB0008008 */
#define HIGO_ERR_INVFILE HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INVFILE)

/** ��Ч�Ĳ��� 0xB0008009*/
#define HIGO_ERR_INVPARAM HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INVPARAM)

/** �˾�����ڱ�ʹ�� 0xB000800A */
#define HIGO_ERR_INUSE HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_COMM_INUSE)

/** ��Ч�Ĳ��� 0xB000800B */
#define HIGO_ERR_UNSUPPORTED HIGO_DEF_ERR(HIGO_MOD_COMM, ERR_BLITER_UNSUPPORTED)

/** HiGo surfaceģ������� */
typedef enum
{
    ERR_SURFACE_INVSURFACESIZE = 0,
    ERR_SURFACE_INVSURFACEPF,
    ERR_SURFACE_NOTLOCKED,
    ERR_SURFACE_LOCKED,
    ERR_SURFACE_NOCOLORKEY,
    ERR_SURFACE_BUTT
} HIGO_SURFACE_ERR_E;

#define SURFACE_DEF_ERR(err) HIGO_DEF_ERR(HIGO_MOD_SURFACE, err)

/** surface�ߴ粻��ȷ 0xB0018000 */
#define HIGO_ERR_INVSURFACESIZE SURFACE_DEF_ERR(ERR_SURFACE_INVSURFACESIZE)

/** surface���ظ�ʽ����ȷ 0xB0018001 */
#define HIGO_ERR_INVSURFACEPF SURFACE_DEF_ERR(ERR_SURFACE_INVSURFACEPF)

/** surfaceδ���������ܽ���surface�������� 0xB0018002 */
#define HIGO_ERR_NOTLOCKED SURFACE_DEF_ERR(ERR_SURFACE_NOTLOCKED)

/** surface����������surface���е�д��������ֹ 0xB0018003 */
#define HIGO_ERR_LOCKED SURFACE_DEF_ERR(ERR_SURFACE_LOCKED)

/** surface������colorKeyֵ 0xB0018004 */
#define HIGO_ERR_NOCOLORKEY SURFACE_DEF_ERR(ERR_SURFACE_NOCOLORKEY)

/** HiGo gdevģ�������*/
typedef enum
{
    ERR_LAYER_INVSIZE = 0,
    ERR_LAYER_INVLAYERID,
    ERR_LAYER_INVPIXELFMT,
    ERR_LAYER_FLUSHTYPE,
    ERR_LAYER_FREEMEM,
    ERR_LAYER_CLOSELAYER,
    ERR_LAYER_CANNOTCHANGE,
    ERR_LAYER_INVORDERFLAG,
    ERR_LAYER_SETALPHA,
    ERR_LAYER_ALREADYSHOW,
    ERR_LAYER_ALREADYHIDE,
    ERR_LAYER_INVLAYERPOS,
    ERR_LAYER_BUTT
} HIGO_LAYER_ERR_E;

#define LAYER_DEF_ERR(err) HIGO_DEF_ERR(HIGO_MOD_LAYER, err)

/** ��Ч��ͼ���С 0xB0038000 */
#define HIGO_ERR_INVSIZE LAYER_DEF_ERR(ERR_LAYER_INVSIZE)

/** ��Ч��Ӳ��ͼ��ID 0xB0038001 */
#define HIGO_ERR_INVLAYERID LAYER_DEF_ERR(ERR_LAYER_INVLAYERID)

/** ��Ч�����ظ�ʽ 0xB0038002 */
#define HIGO_ERR_INVPIXELFMT LAYER_DEF_ERR(ERR_LAYER_INVPIXELFMT)

/** ͼ��ˢ��ģʽ���� 0xB0038003 */
#define HIGO_ERR_INVFLUSHTYPE LAYER_DEF_ERR(ERR_LAYER_FLUSHTYPE)

/** �ͷ��Դ�ʧ�� 0xB0038004 */
#define HIGO_ERR_FREEMEM LAYER_DEF_ERR(ERR_LAYER_FREEMEM)

/** �ر�ͼ���豸ʧ�� 0xB0038005 */
#define HIGO_ERR_CLOSELAYERFAILED LAYER_DEF_ERR(ERR_LAYER_CLOSELAYER)

/** ͼ��Z�򲻿ɸı� 0xB0038006 */
#define HIGO_ERR_CANNOTCHANGE LAYER_DEF_ERR(ERR_LAYER_CANNOTCHANGE)

/** ��Ч��Z���޸ı�־ 0xB0038007 */
#define HIGO_ERR_INVORDERFLAG LAYER_DEF_ERR(ERR_LAYER_INVORDERFLAG)

/** ����surface alphaʧ�� 0xB0038008 */
#define HIGO_ERR_SETALPHAFAILED LAYER_DEF_ERR(ERR_LAYER_SETALPHA)

/** ͼ���Ѿ���ʾ 0xB0038009 */
#define HIGO_ERR_ALREADYSHOW LAYER_DEF_ERR(ERR_LAYER_ALREADYSHOW)

/** ͼ���Ѿ����� 0xB003800A */
#define HIGO_ERR_ALREADYHIDE LAYER_DEF_ERR(ERR_LAYER_ALREADYHIDE)

/** ��Ч��ͼ����ʼλ�� 0xB003800B*/
#define HIGO_ERR_INVLAYERPOS LAYER_DEF_ERR(ERR_LAYER_INVLAYERPOS)

    
/** HiGo bliterģ������� */
typedef enum
{
    ERR_BLITER_INVCOMPTYPE = 0,
    ERR_BLITER_INVCKEYTYPE,
    ERR_BLITER_INVMIRRORTYPE,
    ERR_BLITER_INVROTATETYPE,
    ERR_BLITER_INVROPTYPE,
    ERR_BLITER_INVSCALING,
    ERR_BLITER_OUTOFBOUNDS,
    ERR_BLITER_EMPTYRECT,
    ERR_BLITER_OUTOFPAL,
    ERR_BLITER_NOP,
    ERR_BLITER_BUTT
} HIGO_BLITER_ERR_S;

/** ����Ļ��ģʽ  0xB0048000 */
#define HIGO_ERR_INVCOMPTYPE HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_INVCOMPTYPE)
/** ��Ч��colorKey���� 0xB0048001 */
#define HIGO_ERR_INVCKEYTYPE HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_INVCKEYTYPE)
/** ��Ч�ľ������ 0xB0048002 */
#define HIGO_ERR_INVMIRRORTYPE HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_INVMIRRORTYPE)
/** ��Ч����ת���� 0xB0048003 */
#define HIGO_ERR_INVROTATETYPE HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_INVROTATETYPE)
/** ��Ч��ROP���� 0xB0048004 */
#define HIGO_ERR_INVROPTYPE HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_INVROPTYPE)
/** ���ű����쳣 0xB0048005 */
#define HIGO_ERR_INVSCALING HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_INVSCALING)
/** ���γ����߽� 0xB0048006*/
#define HIGO_ERR_OUTOFBOUNDS HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_OUTOFBOUNDS)
/** �վ��� 0xB0048007*/
#define HIGO_ERR_EMPTYRECT HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_EMPTYRECT)
/** ��ɫ���ڵ�ɫ���� 0xB0048008*/
#define HIGO_ERR_OUTOFPAL HIGO_DEF_ERR(HIGO_MOD_BLITER, ERR_BLITER_OUTOFPAL)

/** HiGo decodeģ�������*/
typedef enum
{
    ERR_DEC_INVIMAGETYPE = 0, 
    ERR_DEC_INVINDEX,         
    ERR_DEC_INVIMGDATA,      
    ERR_DEC_BUTT
} HIGO_ERR_DEC_E;

/**< ��Ч��ͼƬ���� 0xB0058000 */
#define HIGO_ERR_INVIMAGETYPE HIGO_DEF_ERR(HIGO_MOD_DEC, ERR_DEC_INVIMAGETYPE)
/**< ��ЧͼƬ������ 0xB0058001 */
#define HIGO_ERR_INVINDEX HIGO_DEF_ERR(HIGO_MOD_DEC, ERR_DEC_INVINDEX)
/**< ��ЧͼƬ���� 0xB0058002 */
#define HIGO_ERR_INVIMGDATA HIGO_DEF_ERR(HIGO_MOD_DEC, ERR_DEC_INVIMGDATA)


/** HiGo textoutģ������� */
typedef enum
{
    ERR_TEXTOUT_INVRECT = 0,
    ERR_TEXTOUT_UNSUPPORT_CHARSET,
    ERR_TEXTOUT_ISUSING,
    ERR_TEXTOUT_BUTT
} HIGO_TEXTOUT_ERR_S;

/** ��Ч�ľ������� 0xB0068000 */
#define HIGO_ERR_INVRECT HIGO_DEF_ERR(HIGO_MOD_TEXTOUT,ERR_TEXTOUT_INVRECT)

/** ��֧�ֵ��ַ��� 0xB0068001*/
#define HIGO_ERR_UNSUPPORT_CHARSET HIGO_DEF_ERR(HIGO_MOD_TEXTOUT,ERR_TEXTOUT_UNSUPPORT_CHARSET)

/** ��֧�ֵ��ַ��� 0xB0068002*/
#define HIGO_ERR_ISUSING HIGO_DEF_ERR(HIGO_MOD_TEXTOUT,ERR_TEXTOUT_ISUSING)

/** HiGo Wincģ������� */
typedef enum
{
    ERR_WINC_ALREADYBIND = 0, /**< �Ѿ����� */
    ERR_WINC_INVZORDERTYPE,   /**< ��Ч��Z�������ʽ */
    ERR_WINC_NOUPDATE,        /**< �����޸��� */
    ERR_WINC_BUTT
} HIGO_ERR_WINC_E;

/** ������ͼ���Ѿ��� 0xB0078000 */
#define HIGO_ERR_ALREADYBIND HIGO_DEF_ERR(HIGO_MOD_WINC, ERR_WINC_ALREADYBIND)
/** ��Ч��Z�������ʽ 0xB0078001 */
#define HIGO_ERR_INVZORDERTYPE HIGO_DEF_ERR(HIGO_MOD_WINC, ERR_WINC_INVZORDERTYPE)
/** �����޸��� 0xB0078002 */
#define HIGO_ERR_NOUPDATE HIGO_DEF_ERR(HIGO_MOD_WINC, ERR_WINC_NOUPDATE)


/** @} */  /*! <!-- Macro Definition end */



/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/


#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_ERRNO_H__ */

