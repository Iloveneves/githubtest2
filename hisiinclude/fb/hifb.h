#ifndef __HIFB_H__
#define __HIFB_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"

#define IOC_TYPE_HIFB       'F'
#define FBIOGET_COLORKEY_HIFB           _IOR(IOC_TYPE_HIFB, 0x60, HIFB_COLORKEY_S*)
#define FBIOPUT_COLORKEY_HIFB           _IOW(IOC_TYPE_HIFB, 0x61, HIFB_COLORKEY_S*)
#define FBIOGET_ALPHA_HIFB              _IOR(IOC_TYPE_HIFB, 0x62, HIFB_ALPHA_S*)
#define FBIOPUT_ALPHA_HIFB              _IOW(IOC_TYPE_HIFB, 0x63, HIFB_ALPHA_S*)
#define FBIOGET_SCREEN_ORIGIN_HIFB      _IOR(IOC_TYPE_HIFB, 0x64, HIFB_POINT_S*)
#define FBIOPUT_SCREEN_ORIGIN_HIFB      _IOW(IOC_TYPE_HIFB, 0x65, HIFB_POINT_S*)
#define FBIOGET_DEFLICKER_HIFB          _IOR(IOC_TYPE_HIFB, 0x66, HIFB_DEFLICKER_S*)
#define FBIOPUT_DEFLICKER_HIFB          _IOW(IOC_TYPE_HIFB, 0x67, HIFB_DEFLICKER_S*)
#define FBIOGET_VBLANK_HIFB             _IO (IOC_TYPE_HIFB, 0x68)
#define FBIOPUT_SHOW_HIFB               _IOW(IOC_TYPE_HIFB, 0x69, HI_BOOL*)
#define FBIOGET_SHOW_HIFB               _IOR(IOC_TYPE_HIFB, 0x6a, HI_BOOL*)
#define FBIOGET_CAPABILITY_HIFB         _IOR(IOC_TYPE_HIFB, 0x6b, HIFB_CAPABILITY_S*)

/*
    ������Ļ����Ĵ�С��
    �û����Ƶ���Ļ��С�ǲ���,�����С���Ը����û�������������ţ���Ҫ����N/P��ʽ�л���
    �ýӿ�ֻ����˫����ģʽ����Ч��
    
*/
#define FBIOPUT_SCREENSIZE              _IOW(IOC_TYPE_HIFB, 0x6c, HIFB_SCREENSIZE_S*)
#define FBIOGET_SCREENSIZE              _IOR(IOC_TYPE_HIFB, 0x6d, HIFB_SCREENSIZE_S*)
/*
   ����ˢ��ģʽ,֧������ģʽ,

    HIFB_FLUSH_ANTIFLICKER, ֧�ֿ���˸
    HIFB_FLUSH_FLIP,        ֧��flip ��ʽ
    HIFB_FLUSH_DOUBBUFER,   ֧��˫����
        ���ַ�ʽ�����������. 
        HIFB_FLUSH_DOUBBUFER|HIFB_FLUSH_FLIP|HIFB_FLUSH_ANTIFLICKER,
        HIFB_FLUSH_DOUBBUFER|HIFB_FLUSH_FLIP,
        HIFB_FLUSH_FLIP|HIFB_FLUSH_ANTIFLICKER,
        HIFB_FLUSH_FLIP|HIFB_FLUSH_ANTIFLICKER,
        
    HIFB_FLUSH_NORMAL,      ��BUFFERģʽ,Ҳ��Ĭ��ģʽ,���ڵ���.��������ģʽ����,
   
*/
#define FBIOPUT_FLUSHYPE                _IOW(IOC_TYPE_HIFB, 0x6e, HIFB_FLUSHTYPE_E*)
#define FBIOGET_FLUSHYPE                _IOR(IOC_TYPE_HIFB, 0x6f, HIFB_FLUSHTYPE_E*)

/* 
    �л�BUFFER,�õ��µ��Դ�BUFFER, ����Ҫ��ʾbuffer,
    Ӧ�ý���������쳣�˳�,FB���Ի����û�ռ�õ�BUFFER.
*/
#define FBIOPUT_SWITCHBUFFER            _IOWR(IOC_TYPE_HIFB, 0x70, HIFB_BUFFER_S*) 
#define FBIOPUT_P2N_HIFB                _IOW (IOC_TYPE_HIFB, 0x71, HI_BOOL*) 
#define FBIOGET_P2N_HIFB                _IOR (IOC_TYPE_HIFB, 0x72, HI_BOOL*) 


/*hifb extend*/
typedef struct
{
    HI_BOOL bKeyEnable;         /* colorkey enable flag */
    HI_BOOL bMaskEnable;        /* key mask enable flag */
    HI_U32 u32Key;              /* colorkey value, maybe contains alpha */
    HI_U8 u8RedMask;            /* red mask */
    HI_U8 u8GreenMask;          /* green mask */
    HI_U8 u8BlueMask;           /* blue mask */
    HI_U8 u8Reserved;   
}HIFB_COLORKEY_S;

typedef struct
{
    HI_U32 u32PosX;         /* horizontal position */
    HI_U32 u32PosY;         /* vertical position */
}HIFB_POINT_S;

typedef struct hiHIFB_DEFLICKER_S
{
    HI_U32  u32HDfLevel;    /* horizontal deflicker level */     
    HI_U32  u32VDfLevel;    /* vertical deflicker level */
    HI_U8   *pu8HDfCoef;    /* horizontal deflicker coefficient */
    HI_U8   *pu8VDfCoef;    /* vertical deflicker coefficient */
}HIFB_DEFLICKER_S;

typedef struct
{
    HI_BOOL bAlphaEnable;   /* alpha enable flag */
    HI_BOOL bAlphaChannel;  /* alpha channel enable flag */
    HI_U8 u8Alpha0;         /* alpha0 value, used in ARGB1555 */
    HI_U8 u8Alpha1;         /* alpha1 value, used in ARGB1555 */
    HI_U8 u8GlobalAlpha;    /* global alpha value */
    HI_U8 u8Reserved;
}HIFB_ALPHA_S;

typedef enum
{
    HIFB_FMT_1BPP = 0,      /* 1bpp */
    HIFB_FMT_2BPP,          /* 2bpp */    
    HIFB_FMT_4BPP,          /* 4bpp */
    HIFB_FMT_8BPP,          /* 8bpp */
    HIFB_FMT_KRGB444,       /* RGB444 */
    HIFB_FMT_KRGB555,       /* RGB555 */
    HIFB_FMT_RGB565,        /* RGB565 */
    HIFB_FMT_ARGB4444,      /* RGB4444 */   
    HIFB_FMT_ARGB1555,      /* RGB1555 */
    HIFB_FMT_KRGB888,       /* RGB888 */
    HIFB_FMT_ARGB8888,      /* RGB8888 */
    HIFB_FMT_BUTT
}HIFB_COLOR_FMT_E;

typedef struct
{
    HI_BOOL bKeyAlpha;      /* whether support colorkey alpha */
    HI_BOOL bGlobalAlpha;   /* whether support global alpha */
    HI_BOOL bCmap;          /* whether support color map */
    HI_BOOL bColFmt[HIFB_FMT_BUTT]; /* support which color format */
    HI_U32  u32MaxWidth;    /* the max pixels per line */
    HI_U32  u32MaxHeight;   /* the max lines */
    HI_U32  u32MinWidth;    /* the min pixels per line */
    HI_U32  u32MinHeight;   /* the min lines */ 
    HI_U32  u32VDefLevel;   /* vertical deflicker level, 0 means vertical deflicker is unsupported */
    HI_U32  u32HDefLevel;   /* horizontal deflicker level, 0 means horizontal deflicker is unsupported  */
}HIFB_CAPABILITY_S;


typedef struct
{
    HI_U8 u8Cr;
    HI_U8 u8Cb;
    HI_U8 u8Y;
    HI_U8 u8Reserved;
}HIFB_YCBCR_S;

typedef enum
{
    HIFB_FLUSH_ANTIFLICKER = 0x01,   /* ֧�ֿ���˸ */
    HIFB_FLUSH_FLIP = 0x02,          /* ֧��flip */
    HIFB_FLUSH_DOUBBUFER = 0x04,     /* ֧��˫���� */
    HIFB_FLUSH_NORMAL = 0x08,        /* ֧�ֵ�bufferģʽ*/  
}HIFB_FLUSHTYPE_E;

typedef struct
{
    HI_U32  u32SWidth;         /* ��Ļ������Ŀ�� */
    HI_U32  u32SHeight;        /* ��Ļ������ĸ߶� */
}HIFB_SCREENSIZE_S;

typedef struct
{
    HI_U32  OpFlag;           /* ��ѡ:������־, 0x00000001:��ȡ����, 0x00000002:��������,���߿��Ե���ʹ��,Ҳ����ͬʱʹ��*/
    HI_U32  PhyAddr;          /* ��ѡ:physical addr, ��ʾҪ�ύ��ʾ����������ַ���߻�ȡ��ʾ����������ַ,ȡ����OpFlag��־.*/
    HI_U32  VirAddr;          /* ��ѡ:physical addr*/
    HI_U32  u32Width;         /* ��ѡ:buffer width*/
    HI_U32  u32Height;        /* ��ѡ:buffer height*/
    HI_U32  Pitch;            /* ��ѡ:width between line*/
    HI_U32  Bpp;              /* ��ѡ:byte per pixel*/
}HIFB_BUFFER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HIFB_H__ */

