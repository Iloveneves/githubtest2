#include "ipanel_video.h"
#include "trd_ipanel_init.h"

#ifdef __cplusplus
extern "C" {
#endif


#define BRIGHT_MAX 100

int ipanel_porting_set_alpha(int alpha)
{
    BU32 s32Ret = 0;

	IPANEL_DEBUG("alpha = %d\n", alpha);    
    if( alpha < 0 || alpha > 100)
    {
        return FYF_ERR;
    }

    alpha = (alpha*255)/100;
	s32Ret = FYF_API_vout_ioctl(NULL, FYF_VOUT_SET_WIN_TRANSPARENT, alpha);
	if(FYF_OK != s32Ret)
    {
        printf("HI_GO_SetLayerAlpha failed in 0x%x \n", s32Ret);
        return FYF_ERR;
    }

    return FYF_OK;    
}

int ipanel_porting_get_alpha(void)
{
    BU08 u8Alpha = 0;
    BS32 s32Ret = 0;
    int s32RetAlpha = 0;
	
	IPANEL_DEBUG("\n");    

#if 0
    //s32Ret = OSDGAL_GetAlpha(OSDGAL_LAYER1, &u8Alpha);
	s32Ret = FYF_API_vout_ioctl(NULL, FYF_VOUT_IOCTL_e op, void * arg)
	if( HI_SUCCESS != s32Ret )
    {
        printf("HI_GO_GetLayerAlpha failed in 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    s32RetAlpha = ((int)u8Alpha*100)/255;
    
    return s32RetAlpha; 
#endif

}

int ipanel_porting_set_video_mode(int mode)
{
    BS32 s32Ret = 0;
	FYF_VOUT_TVENC_MODE_e arg;
    IPANEL_DEBUG("mode %d", mode);     

	arg = (FYF_VOUT_TVENC_MODE_e )mode;
	s32Ret = FYF_API_vout_ioctl(NULL, FYF_VOUT_SET_MODE, &arg);
	if(FYF_OK != s32Ret)
    {
        printf("HI_TRD_AV_SetEncodingFormat failed in 0x%x \n", s32Ret);
        return FYF_ERR;
    }

    return FYF_OK;    
}

/*get video decode mode*/
int ipanel_porting_get_video_mode(void)
{
    BU32 u32mode = 0;;
    BS32 s32Ret = 0;
#if 0    
    s32Ret = Hi_AV_GetVideoMode(&u32mode);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_TRD_AV_GetVolMode failed in 0x%x \n",s32Ret);
        return HI_FAILURE;
    }
    IPANEL_DEBUG("u32mode %d", u32mode);            
#endif
    return u32mode;
}

/*set the video scale*/
int ipanel_porting_set_video_scale(int scale)
{
    BS32 s32Ret = 0;
    BU32 aspectradio = scale;
    FYF_VOUT_AR_MODE_e arg;
	
	IPANEL_DEBUG("scale %d", scale);
	
	arg = (FYF_VOUT_AR_MODE_e)scale;
    //s32Ret = Hi_AV_SetAspectRatio(aspectradio);
	s32Ret = FYF_API_vout_ioctl(NULL, FYF_VOUT_SET_ASPECT_RATIO, &arg);
	if(FYF_OK != s32Ret)
    {
        IPANEL_ERROR("HI_TRD_AV_SetOutputAspectRatio failed in 0x%x \n",s32Ret);
        return FYF_ERR;
    }
    return FYF_OK;    
}

/*get the video scale*/
int ipanel_porting_get_video_scale(void)
{
    BS32  s32Ret = 0;
    BU32 u32Ratio = 0;
#if 0    
    s32Ret = Hi_AV_GetAspectRatio(&u32Ratio);
    if(FYF_OK != s32Ret)
    {
        IPANEL_ERROR("HI_TRD_AV_SetVolMode failed in 0x%x \n",s32Ret);
        return FYF_ERR;
    }
    IPANEL_DEBUG("u32Ratio %d", u32Ratio);  
#endif
    return (BS32)u32Ratio;    
}


/*set the TV picture chroma*/
int ipanel_porting_set_picture_chroma(int chroma)
{
    BS32 s32Ret = 0;
    BU32 contrast = chroma;

    IPANEL_DEBUG("chroma %d", chroma);      
    //s32Ret = Hi_AV_SetContrast(contrast);

	s32Ret = FYF_API_vout_ioctl(0, FYF_VOUT_SET_CONTRAST, (void *)&chroma);

    if( FYF_OK != s32Ret)
    {
        IPANEL_ERROR("FYF_VOUT_SET_CONTRAST failed in 0x%x \n", s32Ret);
        return FYF_ERR;
    }
    
    return FYF_OK;    
}

/*get the TV picture chroma*/
int ipanel_porting_get_picture_chroma(void)
{
    BS32 s32Ret = 0;
    BU32 u32Contrast = 0;
#if 0
    s32Ret = Hi_AV_GetContrast(&u32Contrast);
    if(HI_SUCCESS != s32Ret)
    {
        IPANEL_ERROR("HI_TRD_AV_GetContrast failed in 0x%x \n",s32Ret);
        return HI_FAILURE;
    }
#endif
    return (BS32)u32Contrast;    
}

/*set the TV picture color*/
int ipanel_porting_set_picture_color(int color)
{
    BS32 s32Ret = 0;
    BU32 u32Saturation = color;

    IPANEL_DEBUG("color %d", color);    
    //s32Ret = Hi_AV_SetSaturation(u32Saturation);
    

	s32Ret = FYF_API_vout_ioctl(0, FYF_VOUT_SET_SATURATION, (void *)&color);

    if( FYF_OK != s32Ret)
    {
        IPANEL_ERROR("FYF_VOUT_SET_SATURATION failed in 0x%x \n", s32Ret);
        return FYF_ERR;
    }
    
    return FYF_OK;    
}

/*get the TV picture color*/
int ipanel_porting_get_picture_color(void)
{
    BS32 s32Ret = 0;
    BU32 u32Saturation = 0;
#if 0   
    s32Ret = Hi_AV_GetSaturation(&u32Saturation);
    if(HI_SUCCESS != s32Ret)
    {
        IPANEL_ERROR("HI_TRD_AV_GetSaturation failed in 0x%x \n",s32Ret);
        return HI_FAILURE;
    }
#endif
    return (BS32)u32Saturation;     
}

/*set the TV picture brightness*/
int ipanel_porting_set_picture_brightness(int bright)
{
    BS32 s32Ret = 0;
    BU32 u32Bright = bright;
   
    IPANEL_DEBUG("bright %d", bright);
    //s32Ret = Hi_AV_SetBrightness(u32Bright);
	s32Ret = FYF_API_vout_ioctl(0, FYF_VOUT_SET_SHARPNESS, (void *)&bright);
    if( FYF_OK != s32Ret)
    {
        IPANEL_ERROR("HI_MIXER_SetBrightness failed in 0x%x \n", s32Ret);
        return FYF_ERR;
    }

    return FYF_OK; 

}

/*get the TV picture brightness*/
int ipanel_porting_get_picture_brightness(void)
{
    BS32 s32Ret = 0;
    BU32 u32Bright = 0;
#if 0   
    s32Ret = Hi_AV_GetBrightness(&u32Bright);
    if(FYF_OK != s32Ret)
    {
        IPANEL_ERROR("HI_TRD_AV_GetBrightness failed in 0x%x \n",s32Ret);
        return HI_FAILURE;
    }
#endif
	return (BS32)u32Bright;     
}

#ifdef __cplusplus
}
#endif

