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
    设置屏幕输出的大小。
    用户绘制的屏幕大小是不变,输出大小可以根据用户的需求进行缩放，主要用于N/P制式切换。
    该接口只有在双缓冲模式下有效。
    
*/
#define FBIOPUT_SCREENSIZE              _IOW(IOC_TYPE_HIFB, 0x6c, HIFB_SCREENSIZE_S*)
#define FBIOGET_SCREENSIZE              _IOR(IOC_TYPE_HIFB, 0x6d, HIFB_SCREENSIZE_S*)
/*
   设置刷新模式,支持四种模式,

    HIFB_FLUSH_ANTIFLICKER, 支持抗闪烁
    HIFB_FLUSH_FLIP,        支持flip 方式
    HIFB_FLUSH_DOUBBUFER,   支持双缓冲
        三种方式可以任意组合. 
        HIFB_FLUSH_DOUBBUFER|HIFB_FLUSH_FLIP|HIFB_FLUSH_ANTIFLICKER,
        HIFB_FLUSH_DOUBBUFER|HIFB_FLUSH_FLIP,
        HIFB_FLUSH_FLIP|HIFB_FLUSH_ANTIFLICKER,
        HIFB_FLUSH_FLIP|HIFB_FLUSH_ANTIFLICKER,
        
    HIFB_FLUSH_NORMAL,      单BUFFER模式,也是默认模式,用于调试.不与其他模式混用,
   
*/
#define FBIOPUT_FLUSHYPE                _IOW(IOC_TYPE_HIFB, 0x6e, HIFB_FLUSHTYPE_E*)
#define FBIOGET_FLUSHYPE                _IOR(IOC_TYPE_HIFB, 0x6f, HIFB_FLUSHTYPE_E*)

/* 
    切换BUFFER,得到新的显存BUFFER, 送入要显示buffer,
    应用进程如果被异常退出,FB可以回收用户占用的BUFFER.
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
    HIFB_FLUSH_ANTIFLICKER = 0x01,   /* 支持抗闪烁 */
    HIFB_FLUSH_FLIP = 0x02,          /* 支持flip */
    HIFB_FLUSH_DOUBBUFER = 0x04,     /* 支持双缓冲 */
    HIFB_FLUSH_NORMAL = 0x08,        /* 支持单buffer模式*/  
}HIFB_FLUSHTYPE_E;

typedef struct
{
    HI_U32  u32SWidth;         /* 屏幕上输出的宽度 */
    HI_U32  u32SHeight;        /* 屏幕上输出的高度 */
}HIFB_SCREENSIZE_S;

typedef struct
{
    HI_U32  OpFlag;           /* 必选:操作标志, 0x00000001:获取数据, 0x00000002:送入数据,两者可以单独使用,也可以同时使用*/
    HI_U32  PhyAddr;          /* 必选:physical addr, 表示要提交显示区域的物理地址或者获取显示区域的物理地址,取决与OpFlag标志.*/
    HI_U32  VirAddr;          /* 可选:physical addr*/
    HI_U32  u32Width;         /* 可选:buffer width*/
    HI_U32  u32Height;        /* 可选:buffer height*/
    HI_U32  Pitch;            /* 可选:width between line*/
    HI_U32  Bpp;              /* 可选:byte per pixel*/
}HIFB_BUFFER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HIFB_H__ */

