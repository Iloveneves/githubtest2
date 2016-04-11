#ifndef _FYF_JPEG_H_
#define _FYF_JPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FYF_EFFECT_MODE_E
{
    FYF_EFFECT_NONE = 0, 				/*拷贝显示*/
    FYF_EFFECT_TURNPAGE, 				/*翻页*/
    FYF_EFFECT_ROLLPAGE, 				/*卷轴*/
    FYF_EFFECT_VERTICALSHUTTER,		/* 垂直百叶窗*/
    FYF_EFFECT_HORIZONTALSHUTTER,		/* 水平百叶窗*/
    FYF_EFFECT_LEFTIN,					/* 从左抽出*/
    FYF_EFFECT_TOPIN, 					/* 从上抽出*/
    FYF_EFFECT_TRANSIN, 				/* 渐进渐出*/
    FYF_EFFECT_ROTATE,  				/*螺旋*/
    FYF_EFFECT_CENTEROUT,  				/*中央渐出*/
    FYF_EFFECT_CENTERIN, 				/*中央渐入*/
} FYF_EFFECT_MODE_E;




BS32 FYF_API_JPEG_Init();


BS32 FYF_API_JPEG_Effect_Play(BU32 srcSurface, BU32 desSurface, FYF_EFFECT_MODE_E effect_mode);

BS32 FYF_API_JPEG_Effect_Stop();

BS32 FYF_API_JPEG_CreateDecoder(BU08 *filename, BU32 *pDecoder);

BS32 FYF_API_JPEG_DecImageData(BU32 hDecoder, BU32 width, BU32 height, FYF_OSD_PF_E PixelFormat, BU32 *pDecSurface);

BS32 FYF_API_JPEG_DestroyDecoder(BU32 hDecoder);

#ifdef __cplusplus
}
#endif

#endif

