#ifndef _FYF_JPEG_H_
#define _FYF_JPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FYF_EFFECT_MODE_E
{
    FYF_EFFECT_NONE = 0, 				/*������ʾ*/
    FYF_EFFECT_TURNPAGE, 				/*��ҳ*/
    FYF_EFFECT_ROLLPAGE, 				/*����*/
    FYF_EFFECT_VERTICALSHUTTER,		/* ��ֱ��Ҷ��*/
    FYF_EFFECT_HORIZONTALSHUTTER,		/* ˮƽ��Ҷ��*/
    FYF_EFFECT_LEFTIN,					/* ������*/
    FYF_EFFECT_TOPIN, 					/* ���ϳ��*/
    FYF_EFFECT_TRANSIN, 				/* ��������*/
    FYF_EFFECT_ROTATE,  				/*����*/
    FYF_EFFECT_CENTEROUT,  				/*���뽥��*/
    FYF_EFFECT_CENTERIN, 				/*���뽥��*/
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

