//#include "ali_osd.h"
#include <sys_config.h>
#include <basic_types.h>
#include <mediatypes.h>
#include <os/tds2/itron.h>
#include <api/libc/printf.h>
#include <osal/osal_timer.h>
#include <api/libc/printf.h>
#include <api/libc/string.h>
#include <api/libchunk/chunk.h>
#include <osal/osal.h>
#include <hal/hal_mem.h>
#include <hal/hal_gpio.h>
#include <bus/i2c/i2c.h>
#include <bus/sci/sci.h>
#include <bus/tsi/tsi.h>
#include <hld/pan/pan.h>
//#include <hld/pan/pan_key.h>
#include <hld/hld_dev.h>
#include <hld/sto/sto.h>
#include <hld/sto/sto_dev.h>
#include <hld/osd/osddrv.h>
#include <hld/osd/osddrv_dev.h>
#include <hld/dis/vpo.h>
#include <hld/ge/ge.h> 

#include "../../fyf/inc/gui/gui_common.h"
#include "../../fyf/inc/os/fyf_semaphore.h"

//#include "..\copper_common\system_data.h"//wm
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 720
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 576
#endif
#ifndef ALI_SCREEN_WIDTH
#define ALI_SCREEN_WIDTH 720
#endif

#ifndef ALI_SCREEN_HEIGHT
#define ALI_SCREEN_HEIGHT 576
#endif
#define USE_MEM_MON 0
#define     PAL_Y_R           299
#define     PAL_Y_G           587
#define     PAL_Y_B           114

#define     PAL_CB_R    -169
#define     PAL_CB_G    -331
#define     PAL_CB_B    500

#define     PAL_CR_R    500
#define     PAL_CR_G    -419
#define     PAL_CR_B    -81

void RegisterPSD(void);
static PSD ALI_OSD_open(PSD psd);
static void ALI_OSD_close(PSD psd);
static void ALI_OSD_setpalette(PSD psd, BU32 first, BU32 count,PU32 pal);
static void ALI_OSD_getpalette(PSD psd, BU32 first, BU32 count,PU32 pal);
static void ALI_OSD_drawpixel(PSD psd, BU32 x, BU32 y, BU32 c);
static BU32 ALI_OSD_readpixel(PSD psd, BU32 x, BU32 y);
static void ALI_OSD_drawhline(PSD psd, BU32 x1, BU32 y1, BU32 w,
						  BU32 c);
static void ALI_OSD_drawvline(PSD psd, BU32 x1, BU32 y1, BU32 h,
						  BU32 c);
static void ALI_OSD_fillrect(PSD psd, BU32 x1, BU32 w, BU32 h,
						 BU32 y2, BU32 c);
static void ALI_OSD_linear32_putbox(PSD psd, BU32 x, BU32 y, BU32 w,
								BU32 h, void *buf);
static void ALI_OSD_linear32_getbox(PSD psd, BU32 x, BU32 y, 
							BU32 w, BU32 h, void* buf);
static void ALI_OSD_blit(PSD dstpsd, BU32 destx, BU32 desty, BU32 w,
					 BU32 h, PSD srcpsd, BU32 srcx, BU32 srcy);
static void ALI_OSD_linear32_copybox (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2);
static	void ALI_OSD_linear32_putboxmask ( PSD psd, BU32 x, BU32 y, 
									  BU32 w, BU32 h, void *buf, BU32 cxx);
void OSD_API_SetTransp(char value);

static void ALI_OSD_Printf(PU08 fmt);
static void*  ALI_OSD_MemMalloc(BU32 size);
static void ALI_OSD_MemFree(void * lpmem);
static void ALI_OSD_TimerDelay(BU32 ms);


static lpVSCR pVscr;
static BU08 * dipsbuf_addr =(BU08 * ) __MM_OSD_START_ADDR;
BU08 g_pal[256*4];
static struct osd_device *osd_dev;

static struct ge_device* ge_dev = NULL;
ge_surface_desc_t gfx_src,*gfx_bmp,*gfx_dst = NULL;

GUI_ScreenDevice_s scrdev = {
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 
	NULL, 
	NULL,
	ALI_OSD_open,
	ALI_OSD_close,
	ALI_OSD_setpalette,
	ALI_OSD_getpalette,
	NULL,
	NULL,
	NULL,
	NULL,	
	ALI_OSD_fillrect,
	ALI_OSD_drawpixel,			/* DrawPixel subdriver*/
	ALI_OSD_readpixel,			/* ReadPixel subdriver*/
	ALI_OSD_drawhline,			/* DrawHLine subdriver*/
	ALI_OSD_drawvline,			/* DrawVLine subdriver*/	
	NULL,					/* Blit subdriver*/
	ALI_OSD_linear32_putbox,	/* PutBox subdriver*/
	ALI_OSD_linear32_getbox,	/* CopyBox subdriver*/
	ALI_OSD_linear32_putboxmask,/* PutBoxMask subdriver*/
	NULL,	/* CopyBox subdriver*/
	NULL,
	//extern
	ALI_OSD_Printf,
	ALI_OSD_MemMalloc,
	ALI_OSD_MemFree,
	ALI_OSD_TimerDelay	
};

/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BU32 GUI_ALI_SemaphoreCreate(const PU08 name,BS32 initial_count,BU32 wait_mode)
{
	return FYF_API_semaphore_create(name,initial_count,wait_mode);

}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BS32 GUI_ALI_SemaphoreDestroy(BU32 handle)
{
	return FYF_API_semaphore_destroy( handle);
}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BS32 GUI_ALI_SemaphoreWait(BU32 handle,BS32 wait_time)
{
	return FYF_API_semaphore_wait(handle,wait_time);
}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BS32 GUI_ALI_SemaphoreRelease(BU32 handle)
{
	return FYF_API_semaphore_release(handle); 
}

GUI_Semaphore_s aliguisem = 
{
	GUI_ALI_SemaphoreCreate,
	GUI_ALI_SemaphoreDestroy,
	GUI_ALI_SemaphoreWait,
	GUI_ALI_SemaphoreRelease
};

void RegisterPSD(void)
{
	GUI_API_SetOSDDevice(&scrdev);
//	GUI_API_SetSYSDevice_Semaphore(&aliguisem);
}	
/* called from keyboard/mouse/screen */
static PSD ALI_OSD_open(PSD psd)
{
	struct OSDRect tOpenRect, osdrect;
	enum OSDSys eOSDSys;
	struct OSDPara tOpenPara;
	enum TVSystem tvsys;
	
	psd->xres = psd->xvirtres = OSD_MAX_WIDTH;
	psd->yres = psd->yvirtres = OSD_MAX_HEIGHT;
	psd->bpp = 8;
	psd->ncolors = 256;
	psd->linelen = (psd->xres * psd->bpp) >> 3;
	psd->size = psd->yres * psd->linelen;

	psd->planes = 1;
	psd->pixtype = 0;
	psd->flags = PSF_SCREEN;

	tOpenRect.uLeft = (720 - OSD_MAX_WIDTH)>>1;
	tOpenRect.uWidth = OSD_MAX_WIDTH;
	tOpenRect.uTop = ((576 - OSD_MAX_HEIGHT)>>1);
	tOpenRect.uHeight = OSD_MAX_HEIGHT;

	
	tOpenPara.eMode = OSD_256_COLOR;
	tOpenPara.uGAlphaEnable = IF_GLOBAL_ALPHA;
	tOpenPara.uGAlpha = 0x0F;
	tOpenPara.uPalletteSel = 0;
	
	osdrect.uLeft = (720-OSD_MAX_WIDTH) >> 1;
	osdrect.uTop = (576-OSD_MAX_HEIGHT) >> 1;
	osdrect.uWidth = OSD_MAX_WIDTH;
	osdrect.uHeight = OSD_MAX_HEIGHT;
	
	osd_dev = (struct osd_device *)dev_get_by_id(HLD_DEV_TYPE_OSD, 0);
	OSDDrv_Open((HANDLE)osd_dev, &tOpenPara);	
	OSDDrv_IoCtl((HANDLE)osd_dev, OSD_IO_SET_TRANS_COLOR, OSD_TRANSPARENT_COLOR);
	OSDDrv_CreateRegion((HANDLE)osd_dev, 0, &(osdrect), NULL);   
	OSDDrv_ShowOnOff((HANDLE)osd_dev,OSDDRV_ON);
	return &scrdev;
}

static void ALI_OSD_close(PSD psd)
{

	OSDDrv_Close((HANDLE)osd_dev);

}


static void ALI_OSD_setpalette(PSD psd, BU32 first, BU32 count, PU32 pal)
{

	UINT8 *value;
	BU32 i;
	UINT8 u8Y,u8U,u8V;
	UINT8 Y,Cb,Cr;
	UINT8 R,G,B;
	value = (UINT8 *)pal;


	for(i=0;i<count;i++)
	{
		R = value[4*i+2];
		G = value[4*i+1];
		B = value[4*i];

		Y   =   (   (     66   *   R   +   129   *   G   +     25   *   B   +   128)*10 / 256 +5)/10   +     16;   
		Cb=   (   (   -38   *   R   -     74   *   G   +   112   *   B   +   128)*10  /256 +5)/10  +   128 ;  
		Cr   =   (   (   112   *   R   -     94   *   G   -     18   *   B   +   128)*10  /256 +5)/10   +   128;
		g_pal[3*i] = Y;
		g_pal[3*i+1] = Cb;
		g_pal[3*i+2] = Cr;
		//printf("%x\n", i);
		if (i == 255)
			OSDDrv_ModifyPallette((HANDLE)osd_dev,i,Y,Cb,Cr,0x00);
		else
			OSDDrv_ModifyPallette((HANDLE)osd_dev,i,Y,Cb,Cr,0x0f);
	}

	/*modify by wm 2007-10-30*/
#if 0
	Osd_Set osd_set;
	tp_sys_data_get(TP_SYS_OSDSET, &osd_set, NULL);
	OSD_API_SetTransp(15 - (osd_set.osd_trans));
#endif
}

static void ALI_OSD_getpalette(PSD psd, BU32 first, BU32 count, PU32 pal)
{
}

static void ALI_OSD_drawpixel(PSD psd, BU32 x, BU32 y, BU32 c)
{
#ifdef CONFIG_ALI_OSD_ORIGINAL_DRV
	struct OSDRect	r;
	r.uWidth  = 1;
    r.uHeight = 1;
	r.uLeft   = x;
	r.uTop    = y;
	OSDDrv_RegionFill(( HANDLE )osd_dev, 0, &r, c);	
#else
	*(BU08 *)(dipsbuf_addr+y*psd->linelen+x) = (BU08)c;
#endif
}

static BU32 ALI_OSD_readpixel(PSD psd, BU32 x, BU32 y)
{
	return(*(BU08 *)(dipsbuf_addr+y*psd->linelen+x));

}


static void ALI_OSD_drawhline(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c)
{

#ifdef CONFIG_ALI_OSD_ORIGINAL_DRV
	struct OSDRect	r;
	r.uWidth  = w;
    r.uHeight = 2;
	r.uLeft   = x;
	r.uTop    = y;
	OSDDrv_RegionFill(( HANDLE )osd_dev, 0, &r, c);	
#else
	BU32 i;
	BU08 *dispbufaddr;
	i = w;
	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	while(i--)
	{
		*dispbufaddr++ = (BU08)c ;
	
	}		
#endif

}

static void ALI_OSD_drawvline(PSD psd, BU32 x, BU32 y, BU32 h, BU32 c)
{
#ifdef CONFIG_ALI_OSD_ORIGINAL_DRV
	struct OSDRect	r;
	r.uWidth  = 1;
    r.uHeight = h;
	r.uLeft   = x;
	r.uTop    = y;
	OSDDrv_RegionFill(( HANDLE )osd_dev, 0, &r, c);	
#else
	BU32 i;
	BU08 *dispbufaddr;
	i = h;
	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	while(i--)
	{
		*dispbufaddr = (BU08)c ;
		dispbufaddr += psd->linelen;				
	}
#endif
}

static void ALI_OSD_fillrect(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h,BU32 c)
{
#ifdef CONFIG_ALI_OSD_ORIGINAL_DRV
	struct OSDRect	r;
    r.uHeight = h;
	r.uWidth  = w;
	r.uLeft   = x;
	r.uTop    = y;
	OSDDrv_RegionFill(( HANDLE )osd_dev, 0, &r, c);	
#else
	BU32 i,j;
	BU08 *dispbufaddr;
	BU08 *dispbufaddr1;	
	i = h;
	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	dispbufaddr1 = dispbufaddr;
	while(i--)
	{
		j = w;
		while(j--)
		{
			*dispbufaddr++ = (BU08)c ;
		}				
		dispbufaddr1 += psd->linelen;
		dispbufaddr = dispbufaddr1 ;
	}
#endif

}

static void ALI_OSD_blit(PSD dstpsd, BU32 destx, BU32 desty, BU32 w,
					 BU32 h, PSD srcpsd, BU32 srcx, BU32 srcy)
{
}



/*do clip*/
static void ALI_OSD_linear32_putbox ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void *buf)
{
#ifdef CONFIG_ALI_OSD_ORIGINAL_DRV
	VSCR 		vscr;	
	vscr.vR.uHeight = Height;
	vscr.vR.uWidth  = Width;
	vscr.vR.uLeft   = x;
	vscr.vR.uTop    = y;	
	
	vscr.lpbScr = buf;
	
    OSDDrv_RegionWrite(( HANDLE )osd_dev, 0, &vscr,&vscr.vR);
#else
	BU32 h,w;

	BU08 *dispbufaddr;
	BU08 *dispbuftemp;	
	BU08 *bmpvalueaddr;	

	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	dispbuftemp = dispbufaddr;
	bmpvalueaddr = (BU08 *)buf;

	for(h = 0;h<Height;h++)
 	{

		for(w=0;w<Width;w++)
		{
			*dispbufaddr = *bmpvalueaddr;
			dispbufaddr++;
			bmpvalueaddr++;
		}		  
		dispbuftemp = dispbuftemp+psd->linelen;
		dispbufaddr = dispbuftemp;		
 	}
#endif

}


/*clip to screen*/
static void ALI_OSD_linear32_getbox ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void* buf )
{
#ifdef CONFIG_ALI_OSD_ORIGINAL_DRV
	VSCR 		vscr;	
	vscr.vR.uHeight = Height;
	vscr.vR.uWidth  = Width;
	vscr.vR.uLeft   = x;
	vscr.vR.uTop    = y;	
	
	vscr.lpbScr = buf;
	
    OSDDrv_RegionRead(( HANDLE )osd_dev, 0, &vscr, &vscr.vR);
#else
	BU32 h,w;

	BU08 *dispbufaddr;
	BU08 *dispbuftemp;	
	BU08 *bmpvalueaddr;	

	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	dispbuftemp = dispbufaddr;
	bmpvalueaddr = (BU08 *)buf;

	for(h = 0;h<Height;h++)
 	{

		for(w=0;w<Width;w++)
		{
			*bmpvalueaddr = *dispbufaddr;
			dispbufaddr++;
			bmpvalueaddr++;
		}		  
		dispbuftemp = dispbuftemp+psd->linelen;
		dispbufaddr = dispbuftemp;		
 	}
#endif

}

static void ALI_OSD_linear32_copybox (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2)
{

}


static	void ALI_OSD_linear32_putboxmask ( PSD psd, BU32 x, BU32 y, 
									  BU32 Width, BU32 Height, void *buf, BU32 cxx)
{
	BU32 h,w;

	BU08 *dispbufaddr;
	BU08 *dispbuftemp;	
	BU08 *bmpvalueaddr;	

	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	dispbuftemp = dispbufaddr;
	bmpvalueaddr = (BU08 *)buf;

	for(h = 0;h<Height;h++)
 	{
		
		
		for(w=0;w<Width;w++)
		{
			if((BU08)cxx != (*bmpvalueaddr))
			{
				*dispbufaddr = *bmpvalueaddr;
			}
			dispbufaddr++;
			bmpvalueaddr++;
		}	
		dispbuftemp += psd->linelen;
		dispbufaddr = dispbuftemp;
 	}

}
static void ALI_OSD_Printf(PU08 fmt)
{
	printf(fmt);
}
#if USE_MEM_MON == 1
static BU32 all_size_max = 0;
static BU32 cur_size = 0;
#endif
static void*  ALI_OSD_MemMalloc(BU32 size)
{
#if USE_MEM_MON == 1
	BU32 * lp;
	cur_size +=size;
	if(cur_size > all_size_max)
	{
		all_size_max = cur_size;
		if(all_size_max >100000)
		{
			soc_printf("gui mem use = %d\n",all_size_max);
		}
	}
	lp =	(BU32 *)MALLOC(size+4);
	*lp = size;
	lp++;
	return (void *)lp;
#else
	if(size != 0)
	{
		return MALLOC(size);
	}
	else
	{
		return 0;
	}
#endif
}
static void ALI_OSD_MemFree(void * lpmem)
{
#if USE_MEM_MON == 1
	BU32 * lp;
	lp = (BU32 *)lpmem;
	lp--;
	
	cur_size -= *lp;
	soc_printf("gui mem free = %d\n",*lp);
	FREE((void *)lp);

#else
	if(lpmem != NULL)
	{
	 	FREE(lpmem);
	}
#endif	

}
static void ALI_OSD_TimerDelay(BU32 ms)
{
	osal_task_sleep(ms);
}
void OSD_API_SetTransp(char value)
{
	int i;
	for(i=0;i<255;i++)
	{
		OSDDrv_ModifyPallette((HANDLE)osd_dev,i,g_pal[3*i],g_pal[3*i+1],g_pal[3*i+2],value);
	}
}


static PSD ALI_OSD_openGE(PSD psd);
static void ALI_OSD_closeGE(PSD psd);
static void ALI_OSD_setpaletteGE(PSD psd, BU32 first, BU32 count, PU32 pal);
static void ALI_OSD_drawpixelGE(PSD psd, BU32 x, BU32 y, BU32 c);
static BU32 ALI_OSD_readpixelGE(PSD psd, BU32 x, BU32 y);
static void ALI_OSD_drawhlineGE(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c);
static void ALI_OSD_drawvlineGE(PSD psd, BU32 x, BU32 y, BU32 h, BU32 c);
static void ALI_OSD_fillrectGE(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h,BU32 c);
static void ALI_OSD_linear32_putboxGE ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void *buf);
static void ALI_OSD_linear32_getboxGE ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void* buf );

static void ALI_OSD_linear32_copyboxGE (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2);


static	void ALI_OSD_linear32_putboxmaskGE ( PSD psd, BU32 x, BU32 y, 
									  BU32 Width, BU32 Height, void *buf, BU32 cxx);

GUI_ScreenDevice_s scrdevge = {
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 
	NULL, 
	NULL,
	ALI_OSD_openGE,
	ALI_OSD_closeGE,
	ALI_OSD_setpaletteGE,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,	
	ALI_OSD_fillrectGE,
	ALI_OSD_drawpixelGE,			/* DrawPixel subdriver*/
	ALI_OSD_readpixelGE,			/* ReadPixel subdriver*/
	ALI_OSD_drawhlineGE,			/* DrawHLine subdriver*/
	ALI_OSD_drawvlineGE,			/* DrawVLine subdriver*/	
	NULL,					/* Blit subdriver*/
	ALI_OSD_linear32_putboxGE,	/* PutBox subdriver*/
	ALI_OSD_linear32_getboxGE,	/* CopyBox subdriver*/
	ALI_OSD_linear32_putboxmaskGE,/* PutBoxMask subdriver*/
	NULL,	/* CopyBox subdriver*/
	NULL,
	//extern
	ALI_OSD_Printf,
	ALI_OSD_MemMalloc,
	ALI_OSD_MemFree,
	ALI_OSD_TimerDelay	
};
void RegisterPSDGE(void)
{
	GUI_API_SetOSDDevice(&scrdevge);
	GUI_API_SetSYSDevice_Semaphore(&aliguisem);
}	

static PSD ALI_OSD_openGE(PSD psd)
{

	ge_region_t region_para;
	ge_rect_t dst_rect;
	INT32 pitch = 0;
	ge_anti_flick_t af;

	
	psd->xres = psd->xvirtres = ALI_SCREEN_WIDTH;
	psd->yres = psd->yvirtres = ALI_SCREEN_HEIGHT;
	psd->bpp = 8;
	psd->ncolors = 1<<8;
	psd->linelen = (psd->xres * psd->bpp) >> 3;
	psd->size = psd->yres * psd->linelen;
	//psd->gr_mode = 15;

	psd->planes = 1;
	psd->pixtype = 0;
	psd->flags = PSF_SCREEN;


	//ge open
	ge_dev = (struct ge_device*)dev_get_by_id(HLD_DEV_TYPE_GE, 0);
	if (SUCCESS != ge_open(ge_dev))
	{
	    printf("%s() ge_open failed\n", __FUNCTION__);
	    return  (PSD)(- 1);
	}
    
	gfx_dst = NULL;

	memset((UINT8 *)&gfx_src, 0, sizeof(ge_surface_desc_t));


    pitch = ALI_SCREEN_WIDTH;
    region_para.pixel_fmt = GE_PF_CLUT8;

	gfx_src.width = ALI_SCREEN_WIDTH;
	gfx_src.height = ALI_SCREEN_HEIGHT;
	gfx_src.pitch = pitch; 
	gfx_src.endian = GE_PIXEL_LITTLE_ENDIAN;
	gfx_src.alpha_range = GE_LARGE_ALPHA;	

	gfx_dst = ge_create_surface(ge_dev, &gfx_src, 0, GMA_HW_SURFACE_1);	
	
	
    if(NULL==gfx_dst)
    {
        printf("%s() ge_create_surface fail!\n", __FUNCTION__);
        return  (PSD)(- 1);        
    }	
	dst_rect.left = 0;
	dst_rect.top = 0;
	dst_rect.width = ALI_SCREEN_WIDTH;
	dst_rect.height = ALI_SCREEN_HEIGHT;

	region_para.glob_alpha_en = 0;
	region_para.glob_alpha = 0x7f;
	region_para.pallet_seg = 0;
	region_para.pitch = pitch;
	region_para.pixel_fmt = GE_PF_CLUT8;

	region_para.dst_rect_29e.top = 0;
	region_para.dst_rect_29e.left = 0;
	region_para.dst_rect_29e.width = 720;
	region_para.dst_rect_29e.height = 480;

	if(RET_SUCCESS!=ge_create_region(ge_dev, gfx_dst, 0, &dst_rect, &region_para))
	{
        printf("%s() ge_create_region fail!\n", __FUNCTION__);
        return (PSD)(- 1);         
	}
#if 0
	memset((UINT8 *)&gfx_src, 0, sizeof(ge_surface_desc_t));


    pitch = ALI_SCREEN_WIDTH;
    region_para.pixel_fmt = GE_PF_CLUT8;

	gfx_src.width = ALI_SCREEN_WIDTH;
	gfx_src.height = ALI_SCREEN_HEIGHT;
	gfx_src.pitch = pitch; 
	gfx_src.endian = GE_PIXEL_LITTLE_ENDIAN;
	gfx_src.alpha_range = GE_LARGE_ALPHA;	

	gfx_bmp = ge_create_surface(ge_dev, &gfx_src, 0, GMA_MEM_SURFACE);	
	
    pitch = ALI_SCREEN_WIDTH;
    region_para.pixel_fmt = GE_PF_CLUT8;


	
    if(NULL==gfx_bmp)
    {
        printf("%s() ge_create_surface fail!\n", __FUNCTION__);
        return  - 1;        
    }	
	dst_rect.left = 0;
	dst_rect.top = 0;
	dst_rect.width = ALI_SCREEN_WIDTH;
	dst_rect.height = ALI_SCREEN_HEIGHT;

	region_para.glob_alpha_en = 0;
	region_para.glob_alpha = 0x7f;
	region_para.pallet_seg = 0;
	region_para.pitch = pitch;
	region_para.pixel_fmt = GE_PF_CLUT8;

	if(RET_SUCCESS!=ge_create_region(ge_dev, gfx_bmp, 0, &dst_rect, &region_para))
	{
        printf("%s() ge_create_region fail!\n", __FUNCTION__);
        return  - 1;         
	}
#endif	
   memcpy((UINT8 *)&gfx_src, gfx_dst, sizeof(ge_surface_desc_t));
	gfx_src.surf_id =2;


    af.layer = GMA_HW_SURFACE_1;
    af.valid = 1;
    ge_io_ctrl(ge_dev,GE_IO_ANTI_FLICK_29E,(UINT32)&af);
    ge_fill_rect(ge_dev,gfx_dst,&dst_rect,0);


    
	ge_show_onoff(ge_dev, gfx_dst, 1);
		
	return &scrdev;
}

static void ALI_OSD_closeGE(PSD psd)
{

	//OSDDrv_Close((HANDLE)osd_dev);

}
static void ALI_OSD_setpaletteGE(PSD psd, BU32 first, BU32 count, PU32 pal)
{

	UINT8 *value;
	BU32 i;
	UINT8 u8Y,u8U,u8V;
	UINT8 Y,Cb,Cr;
	UINT8 R,G,B;
	value = (UINT8 *)pal;


	for(i=0;i<count;i++)
	{
		R = value[4*i+2];
		G = value[4*i+1];
		B = value[4*i];

		Y   =   (   (     66   *   R   +   129   *   G   +     25   *   B   +   128)*10 / 256 +5)/10   +     16;   
		Cb=   (   (   -38   *   R   -     74   *   G   +   112   *   B   +   128)*10  /256 +5)/10  +   128 ;  
		Cr   =   (   (   112   *   R   -     94   *   G   -     18   *   B   +   128)*10  /256 +5)/10   +   128;

		g_pal[4*i+3] = 0x0f;
		g_pal[4*i+2] = Cr;
		g_pal[4*i+1] = Cb;
		g_pal[4*i] = Y;
		
		if (i == 255)
		{
			g_pal[4*i+3] = 0; 
		}
	}
	
	ge_set_pallette(ge_dev, gfx_dst,g_pal,COLOR_N,0x01,GE_PALLET_ALPHA_16);

}


static void ALI_OSD_drawpixelGE(PSD psd, BU32 x, BU32 y, BU32 c)
{
#if 0
	ge_draw_pixel(ge_dev, gfx_dst,  x,  y,  c);
#else
	*(BU08 *)(dipsbuf_addr+y*psd->linelen+x) = (BU08)c;
#endif
}

static BU32 ALI_OSD_readpixelGE(PSD psd, BU32 x, BU32 y)
{
	return ge_read_pixel(ge_dev, gfx_dst,  x,  y);

}


static void ALI_OSD_drawhlineGE(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c)
{
	ge_draw_hor_line(ge_dev, gfx_dst,  x,  y, w,c);
}

static void ALI_OSD_drawvlineGE(PSD psd, BU32 x, BU32 y, BU32 h, BU32 c)
{
	ge_draw_ver_line(ge_dev, gfx_dst,  x,  y, h,c);
}

static void ALI_OSD_fillrectGE(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h,BU32 c)
{
	ge_rect_t gerect;
	gerect.left = x;
	gerect.top = y;
	gerect.width = w;
	gerect.height = h;
	ge_fill_rect(ge_dev, gfx_dst,  &gerect, c);
}




/*do clip*/
static void ALI_OSD_linear32_putboxGE ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void *buf)
{
	ge_rect_t dst_rect, src_rect;
	ge_surface_desc_t gfx;
	ge_region_t region_para;

	src_rect.left = 0;
	src_rect.top = 0;
	src_rect.width = Width;
	src_rect.height = Height;
	
	dst_rect.left = x;
	dst_rect.top = y;
	dst_rect.width = Width;
	dst_rect.height = Height;

	gfx_src.region[0].addr = (unsigned char *)buf; 
	gfx_src.region[0].pitch = Width;//srcW<<1;
	gfx_src.region[0].pixel_fmt = GE_PF_CLUT8;
	gfx_src.width = Width;
	gfx_src.height = Height;
	gfx_src.region[0].rect.top = 0;
	gfx_src.region[0].rect.left= 0;
	gfx_src.region[0].rect.width= Width;
	gfx_src.region[0].rect.height= Height;


	
    ge_blt(ge_dev,gfx_dst,&gfx_src,&dst_rect,&src_rect,GE_BLT_SRC2_VALID );

}


/*clip to screen*/
static void ALI_OSD_linear32_getboxGE ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void* buf )
{
	ge_rect_t dst_rect, src_rect;

	src_rect.left = x;
	src_rect.top = y;
	src_rect.width = (BU16)Width;
	src_rect.height = (BU16)Height;
	
	dst_rect.left = 0;
	dst_rect.top = 0;
	dst_rect.width = (BU16)Width;
	dst_rect.height = (BU16)Height;

	gfx_src.region[0].addr = (unsigned char *)buf; 
	gfx_src.region[0].pitch = Width;//srcW<<1;
	gfx_src.region[0].pixel_fmt = GE_PF_CLUT8;
	gfx_src.width = Width;
	gfx_src.height = Height;
	gfx_src.region[0].rect.top = 0;
	gfx_src.region[0].rect.left= 0;
	gfx_src.region[0].rect.width= Width;
	gfx_src.region[0].rect.height= Height;




    ge_blt(ge_dev,&gfx_src,gfx_dst,&dst_rect,&src_rect,GE_BLT_SRC2_VALID);

}

static void ALI_OSD_linear32_copyboxGE (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2)
{


}


static	void ALI_OSD_linear32_putboxmaskGE ( PSD psd, BU32 x, BU32 y, 
									  BU32 Width, BU32 Height, void *buf, BU32 cxx)
{
	ge_rect_t dst_rect, src_rect;
	ge_surface_desc_t gfx;
	ge_region_t region_para;
#if 0
	static unsigned char * buf1 = NULL;
	unsigned char * buf2 = (unsigned char *)buf;
	BU32 h;
	BU32 size;
	
	if(buf1 != NULL)
	{
		FREE(buf1);
		buf1 = NULL;
	}
	size = Width*Height;
	buf1 = MALLOC(size);
	ALI_OSD_linear32_getboxGE (psd, x, y, Width,Height,buf1 );


	for(h = 0;h<size;h++)
 	{
		if((BU08)cxx != (*buf2))
		{
			*buf1 = *buf2;
		}
		buf1++;
		buf2++;
 	}
#endif
#if 0
	src_rect.left = 0;
	src_rect.top = 0;
	src_rect.width = Width;
	src_rect.height = Height;
	
	dst_rect.left = x;
	dst_rect.top = y;
	dst_rect.width = Width;
	dst_rect.height = Height;

	gfx_src.region[0].addr = (unsigned char *)buf; 
	gfx_src.region[0].pitch = Width;//srcW<<1;
	gfx_src.region[0].pixel_fmt = GE_PF_CLUT8;
	gfx_src.width = Width;
	gfx_src.height = Height;
	gfx_src.region[0].rect.top = 0;
	gfx_src.region[0].rect.left= 0;
	gfx_src.region[0].rect.width= Width;
	gfx_src.region[0].rect.height= Height;

	//gfx_dst->alpha_blend.src_mode = GE_AB_SRC_NOT_PREMULTI;
	//gfx_dst->alpha_blend.glob_alpha = 0x7f;
	//gfx_dst->rop = GE_ROP_AND;

	ge_blt(ge_dev,gfx_dst,&gfx_src,&dst_rect,&src_rect,GE_BLT_SRC2_VALID );
   // ge_blt_ex(ge_dev,gfx_dst,&gfx_src,gfx_dst,&dst_rect,&src_rect,&dst_rect,GE_BLT_SRC2_VALID|GE_BLT_SRC1_VALID|GE_BLT_ROP);

	//ALI_OSD_linear32_putboxGE (  psd,  x,  y,  Width,  Height,  buf);
#endif	
	ALI_OSD_linear32_putboxmask(psd,  x,  y,  Width,  Height,  buf,  cxx);

}
void RegisterPSD1555(void);
static PSD ALI_OSD_open1555(PSD psd);
static void ALI_OSD_close1555(PSD psd);
static void ALI_OSD_setpalette(PSD psd, BU32 first, BU32 count,PU32 pal);
static void ALI_OSD_getpalette(PSD psd, BU32 first, BU32 count,PU32 pal);
static void ALI_OSD_drawpixel1555(PSD psd, BU32 x, BU32 y, BU32 c);
static BU32 ALI_OSD_readpixel1555(PSD psd, BU32 x, BU32 y);
static void ALI_OSD_drawhline1555(PSD psd, BU32 x1, BU32 y1, BU32 w,
						  BU32 c);
static void ALI_OSD_drawvline1555(PSD psd, BU32 x1, BU32 y1, BU32 h,
						  BU32 c);
static void ALI_OSD_fillrect1555(PSD psd, BU32 x1, BU32 w, BU32 h,
						 BU32 y2, BU32 c);
static void ALI_OSD_linear32_putbox1555(PSD psd, BU32 x, BU32 y, BU32 w,
								BU32 h, void *buf);
static void ALI_OSD_linear32_getbox1555(PSD psd, BU32 x, BU32 y, 
							BU32 w, BU32 h, void* buf);
static void ALI_OSD_blit1555(PSD dstpsd, BU32 destx, BU32 desty, BU32 w,
					 BU32 h, PSD srcpsd, BU32 srcx, BU32 srcy);
static void ALI_OSD_linear32_copybox1555 (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2);
static	void ALI_OSD_linear32_putboxmask1555 ( PSD psd, BU32 x, BU32 y, 
									  BU32 w, BU32 h, void *buf, BU32 cxx);

GUI_ScreenDevice_s scrdev1555 = {
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 
	NULL, 
	NULL,
	ALI_OSD_open1555,
	ALI_OSD_close1555,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,	
	ALI_OSD_fillrect1555,
	ALI_OSD_drawpixel1555,			/* DrawPixel subdriver*/
	ALI_OSD_readpixel1555,			/* ReadPixel subdriver*/
	ALI_OSD_drawhline1555,			/* DrawHLine subdriver*/
	ALI_OSD_drawvline1555,			/* DrawVLine subdriver*/	
	NULL,					/* Blit subdriver*/
	ALI_OSD_linear32_putbox1555,	/* PutBox subdriver*/
	ALI_OSD_linear32_getbox1555,	/* CopyBox subdriver*/
	ALI_OSD_linear32_putboxmask1555,/* PutBoxMask subdriver*/
	NULL,	/* CopyBox subdriver*/
	NULL,
	//extern
	ALI_OSD_Printf,
	ALI_OSD_MemMalloc,
	ALI_OSD_MemFree,
	ALI_OSD_TimerDelay	
};
void RegisterPSD1555(void)
{
	GUI_API_SetOSDDevice(&scrdev1555);
	GUI_API_SetSYSDevice_Semaphore(&aliguisem);
}	
/* called from keyboard/mouse/screen */
static PSD ALI_OSD_open1555(PSD psd)
{
	struct OSDRect tOpenRect, osdrect;
	enum OSDSys eOSDSys;
	struct OSDPara tOpenPara;
	enum TVSystem tvsys;
	
	psd->xres = psd->xvirtres = ALI_SCREEN_WIDTH;
	psd->yres = psd->yvirtres = ALI_SCREEN_HEIGHT;
	psd->bpp = 16;
	psd->ncolors = 1<<16;
	psd->linelen = (psd->xres * psd->bpp) >> 3;
	psd->size = psd->yres * psd->linelen;
	psd->gr_mode = 15;

	psd->planes = 1;
	psd->pixtype = 0;
	psd->flags = PSF_SCREEN;


	ge_region_t region_para;
	ge_rect_t dst_rect;
	INT32 pitch = 0;
	ge_anti_flick_t af;

	//ge open
	ge_dev = (struct ge_device*)dev_get_by_id(HLD_DEV_TYPE_GE, 0);
	if (SUCCESS != ge_open(ge_dev))
	{
	    printf("%s() ge_open failed\n", __FUNCTION__);
	    return  (PSD)(- 1);
	}
    
	gfx_dst = NULL;
//	gfx_buf = NULL;
	memset((UINT8 *)&gfx_src, 0, sizeof(ge_surface_desc_t));

	//if (sys_ic_get_rev_id() >= IC_REV_2)
	{
        pitch = ALI_SCREEN_WIDTH<<1;
        region_para.pixel_fmt = GE_PF_ARGB1555;
	}
	//else
	{
  //      pitch = 4*ALI_SCREEN_WIDTH;
  //      region_para.pixel_fmt = GE_PF_AYCBCR8888;
	}
	
	gfx_src.width = ALI_SCREEN_WIDTH;
	gfx_src.height = ALI_SCREEN_HEIGHT;
	gfx_src.pitch = pitch; 
	gfx_src.endian = GE_PIXEL_LITTLE_ENDIAN;
	gfx_src.alpha_range = GE_LARGE_ALPHA;	
	/*A surface is a real display layer in HW*/
	gfx_dst = ge_create_surface(ge_dev, &gfx_src, 0, GMA_HW_SURFACE_1);	
	
    if(NULL==gfx_dst)
    {
        printf("%s() ge_create_surface fail!\n", __FUNCTION__);
        return  (PSD)(- 1);        
    }	
	dst_rect.left = 0;
	dst_rect.top = 0;
	dst_rect.width = ALI_SCREEN_WIDTH;
	dst_rect.height = ALI_SCREEN_HEIGHT;

	region_para.glob_alpha_en = 0;
	region_para.glob_alpha = 0x7f;
	region_para.pallet_seg = 0;
	region_para.pitch = pitch;
	

	if(RET_SUCCESS!=ge_create_region(ge_dev, gfx_dst, 0, &dst_rect, &region_para))
	{
        printf("%s() ge_create_region fail!\n", __FUNCTION__);
        return  (PSD)(- 1);         
	}

    
//	if (sys_ic_get_rev_id() >= IC_REV_2)
	{
        //enable the anti-flick
	    af.layer = GMA_HW_SURFACE_1;
	    af.valid = 1;
	    ge_io_ctrl(ge_dev,GE_IO_ANTI_FLICK_29E,(UINT32)&af);
	    ge_fill_rect(ge_dev,gfx_dst,&dst_rect,0);
	}
//	else
	{
		/*Fill the rectangle of this region with defined color. the color data sequence is A-Cr-Y-Cb from high to low when 32bits*/
//s		ge_fill_rect(ge_dev,gfx_dst,&dst_rect,0x801080);
	}

	//libc_memcpy((UINT8 *)&gfx_src, gfx_dst, sizeof(ge_surface_desc_t));
	//gfx_src.surf_id = 2;//GMA_MEM_SURFACE
    
	ge_show_onoff(ge_dev, gfx_dst, 1);
		
	return &scrdev;
}

static void ALI_OSD_close1555(PSD psd)
{

	//OSDDrv_Close((HANDLE)osd_dev);

}


static void ALI_OSD_drawpixel1555(PSD psd, BU32 x, BU32 y, BU32 c)
{
	ge_draw_pixel(ge_dev, gfx_dst,  x,  y,  c);
}

static BU32 ALI_OSD_readpixel1555(PSD psd, BU32 x, BU32 y)
{
	return ge_read_pixel(ge_dev, gfx_dst,  x,  y);

}


static void ALI_OSD_drawhline1555(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c)
{
	ge_draw_hor_line(ge_dev, gfx_dst,  x,  y, w,c);
}

static void ALI_OSD_drawvline1555(PSD psd, BU32 x, BU32 y, BU32 h, BU32 c)
{
	ge_draw_ver_line(ge_dev, gfx_dst,  x,  y, h,c);
}

static void ALI_OSD_fillrect1555(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h,BU32 c)
{
	ge_rect_t gerect;
	gerect.left = x;
	gerect.top = y;
	gerect.width = w;
	gerect.height = h;
	ge_fill_rect(ge_dev, gfx_dst,  &gerect, c);
}


/*do clip*/
static void ALI_OSD_linear32_putbox1555 ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void *buf)
{
	ge_rect_t dst_rect, src_rect;

	src_rect.left = x;
	src_rect.top = y;
	src_rect.width = Width;
	src_rect.height = Height;
	
	dst_rect.left = x;
	dst_rect.top = y;
	dst_rect.width = Width;
	dst_rect.height = Height;

	gfx_src.region[0].addr = (unsigned char *)buf; 
	gfx_src.region[0].pitch = ALI_SCREEN_WIDTH<<1;//srcW<<1;
	gfx_src.region[0].pixel_fmt = GE_PF_ARGB1555;

	printf("%s() srcX:%d,srcY:%d,W:%d,H:%d,dstX:%d,dstY:%d\n", __FUNCTION__,
	x,y,Width,Height,x,y);

//	if (sys_ic_get_rev_id() >= IC_REV_2)//m3202
	{
        ge_blt(ge_dev,gfx_dst,&gfx_src,&dst_rect,&src_rect,GE_BLT_SRC2_VALID );
	}
//	else
	{
  //  	ge_blt(ge_dev,gfx_dst,&gfx_src,&dst_rect,&src_rect,GE_BLT_SRC2_VALID|GE_BLT_RGB2YUV);
	}

}


/*clip to screen*/
static void ALI_OSD_linear32_getbox1555 ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void* buf )
{
	ge_rect_t dst_rect, src_rect;

	src_rect.left = x;
	src_rect.top = y;
	src_rect.width = Width;
	src_rect.height = Height;
	
	dst_rect.left = 0;
	dst_rect.top = 0;
	dst_rect.width = Width;
	dst_rect.height = Height;

	gfx_src.region[0].addr = (unsigned char *)buf; 
	gfx_src.region[0].pitch = ALI_SCREEN_WIDTH<<1;
	gfx_src.region[0].pixel_fmt = GE_PF_ARGB1555;

	printf("%s() srcX:%d,srcY:%d,W:%d,H:%d,dstX:%d,dstY:%d\n", __FUNCTION__,
	x,y,Width,Height,x,y);

//	if (sys_ic_get_rev_id() >= IC_REV_2)//m3202
	{
        ge_blt(ge_dev,&gfx_src,gfx_dst,&dst_rect,&src_rect,GE_BLT_SRC2_VALID);
	}
// 	else
	{
    	
	}

}

static void ALI_OSD_linear32_copybox1555 (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2)
{


}


static	void ALI_OSD_linear32_putboxmask1555 ( PSD psd, BU32 x, BU32 y, 
									  BU32 Width, BU32 Height, void *buf, BU32 cxx)
{

	ALI_OSD_linear32_putbox1555 (  psd,  x,  y,  Width,  Height,  buf);

}

static PSD ALI_OSD_openex(PSD psd);
static void ALI_OSD_closeex(PSD psd);
static void ALI_OSD_setpaletteex(PSD psd, BU32 first, BU32 count,PU32 pal);
static void ALI_OSD_getpaletteex(PSD psd, BU32 first, BU32 count,PU32 pal);
static void ALI_OSD_drawpixelex(PSD psd, BU32 x, BU32 y, BU32 c);
static BU32 ALI_OSD_readpixelex(PSD psd, BU32 x, BU32 y);
static void ALI_OSD_drawhlineex(PSD psd, BU32 x1, BU32 y1, BU32 w,
						  BU32 c);
static void ALI_OSD_drawvlineex(PSD psd, BU32 x1, BU32 y1, BU32 h,
						  BU32 c);
static void ALI_OSD_fillrectex(PSD psd, BU32 x1, BU32 w, BU32 h,
						 BU32 y2, BU32 c);
static void ALI_OSD_linear32_putboxex(PSD psd, BU32 x, BU32 y, BU32 w,
								BU32 h, void *buf);
static void ALI_OSD_linear32_getboxex(PSD psd, BU32 x, BU32 y, 
							BU32 w, BU32 h, void* buf);
static void ALI_OSD_blitex(PSD dstpsd, BU32 destx, BU32 desty, BU32 w,
					 BU32 h, PSD srcpsd, BU32 srcx, BU32 srcy);
static void ALI_OSD_linear32_copyboxex (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2);
static	void ALI_OSD_linear32_putboxmaskex ( PSD psd, BU32 x, BU32 y, 
									  BU32 w, BU32 h, void *buf, BU32 cxx);

GUI_ScreenDevice_s scrdevex = {
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 
	NULL, 
	NULL,
	ALI_OSD_openex,
	ALI_OSD_closeex,
	ALI_OSD_setpaletteex,
	ALI_OSD_getpaletteex,
	NULL,
	NULL,
	NULL,
	NULL,	
	ALI_OSD_fillrectex,
	ALI_OSD_drawpixelex,			/* DrawPixel subdriver*/
	ALI_OSD_readpixelex,			/* ReadPixel subdriver*/
	ALI_OSD_drawhlineex,			/* DrawHLine subdriver*/
	ALI_OSD_drawvlineex,			/* DrawVLine subdriver*/	
	NULL,					/* Blit subdriver*/
	ALI_OSD_linear32_putboxex,	/* PutBox subdriver*/
	ALI_OSD_linear32_getboxex,	/* CopyBox subdriver*/
	ALI_OSD_linear32_putboxmaskex,/* PutBoxMask subdriver*/
	NULL,	/* CopyBox subdriver*/
	NULL,
	//extern
	ALI_OSD_Printf,
	ALI_OSD_MemMalloc,
	ALI_OSD_MemFree,
	ALI_OSD_TimerDelay	
};

BU32 ALI_SendMsgHookPre(BU32 hWnd,BU32 type,BU32 * value,BU32 * Param)
{
	BU32 id = 0;
	
//	id = GUI_API_GetCtrlID(hWnd);
//	printf("<-id=%d, type=0x%X\n", id, type);
}
BU32 ALI_SendMsgHookEnd(BU32 hWnd,BU32 type,BU32 * value,BU32 * Param)
{
	BU32 id = 0;
	
//	id = GUI_API_GetCtrlID(hWnd);
//	printf("id=%d, type=0x%X->\n", id, type);
}
void RegisterPSDEX(void)
{
	GUI_API_SetOSDDevice(&scrdevex);
	GUI_API_SetSYSDevice_Semaphore(&aliguisem);
//	GUI_API_SetSendMessageHookPre(ALI_SendMsgHookPre);
//	GUI_API_SetSendMessageHookEnd(ALI_SendMsgHookEnd);
	
}	
/* called from keyboard/mouse/screen */
static PSD ALI_OSD_openex(PSD psd)
{
	struct OSDRect tOpenRect, osdrect;
	enum OSDSys eOSDSys;
	struct OSDPara tOpenPara;
	enum TVSystem tvsys;
	
	psd->xres = psd->xvirtres = OSD_MAX_WIDTH;
	psd->yres = psd->yvirtres = OSD_MAX_HEIGHT;
	psd->bpp = 8;
	psd->ncolors = 256;
	psd->linelen = (psd->xres * psd->bpp) >> 3;
	psd->size = psd->yres * psd->linelen;

	psd->planes = 1;
	psd->pixtype = 0;
	psd->flags = PSF_SCREEN;

	tOpenRect.uLeft = (720 - OSD_MAX_WIDTH)>>1;
	tOpenRect.uWidth = OSD_MAX_WIDTH;
	tOpenRect.uTop = ((576 - OSD_MAX_HEIGHT)>>1);
	tOpenRect.uHeight = OSD_MAX_HEIGHT;

	
	tOpenPara.eMode = OSD_256_COLOR;
	tOpenPara.uGAlphaEnable = IF_GLOBAL_ALPHA;
	tOpenPara.uGAlpha = 0x0F;
	tOpenPara.uPalletteSel = 0;
	
	osdrect.uLeft = (720-OSD_MAX_WIDTH) >> 1;
	osdrect.uTop = (576-OSD_MAX_HEIGHT) >> 1;
	osdrect.uWidth = OSD_MAX_WIDTH;
	osdrect.uHeight = OSD_MAX_HEIGHT;
	
	osd_dev = (struct osd_device *)dev_get_by_id(HLD_DEV_TYPE_OSD, 0);
	OSDDrv_Open((HANDLE)osd_dev, &tOpenPara);	
	OSDDrv_IoCtl((HANDLE)osd_dev, OSD_IO_SET_TRANS_COLOR, OSD_TRANSPARENT_COLOR);
	OSDDrv_CreateRegion((HANDLE)osd_dev, 0, &(osdrect), NULL);   
	OSDDrv_ShowOnOff((HANDLE)osd_dev,OSDDRV_ON);
	return &scrdev;
}

static void ALI_OSD_closeex(PSD psd)
{

	OSDDrv_Close((HANDLE)osd_dev);

}


static void ALI_OSD_setpaletteex(PSD psd, BU32 first, BU32 count, PU32 pal)
{

	UINT8 *value;
	BU32 i;
	UINT8 u8Y,u8U,u8V;
	UINT8 Y,Cb,Cr;
	UINT8 R,G,B;
	value = (UINT8 *)pal;


	for(i=0;i<count;i++)
	{
		R = value[4*i+2];
		G = value[4*i+1];
		B = value[4*i];

		Y   =   (   (     66   *   R   +   129   *   G   +     25   *   B   +   128)*10 / 256 +5)/10   +     16;   
		Cb=   (   (   -38   *   R   -     74   *   G   +   112   *   B   +   128)*10  /256 +5)/10  +   128 ;  
		Cr   =   (   (   112   *   R   -     94   *   G   -     18   *   B   +   128)*10  /256 +5)/10   +   128;
		g_pal[3*i] = Y;
		g_pal[3*i+1] = Cb;
		g_pal[3*i+2] = Cr;
		//printf("%x\n", i);
		if (i == 255)
			OSDDrv_ModifyPallette((HANDLE)osd_dev,i,Y,Cb,Cr,0x00);
		else
			OSDDrv_ModifyPallette((HANDLE)osd_dev,i,Y,Cb,Cr,0x0f);
	}

	/*modify by wm 2007-10-30*/
#if 0
	Osd_Set osd_set;
	tp_sys_data_get(TP_SYS_OSDSET, &osd_set, NULL);
	OSD_API_SetTransp(15 - (osd_set.osd_trans));
#endif
}

static void ALI_OSD_getpaletteex(PSD psd, BU32 first, BU32 count, PU32 pal)
{
}

static void ALI_OSD_drawpixelex(PSD psd, BU32 x, BU32 y, BU32 c)
{
	*(BU08 *)(dipsbuf_addr+y*psd->linelen+x) = (BU08)c;
}

static BU32 ALI_OSD_readpixelex(PSD psd, BU32 x, BU32 y)
{
	return(*(BU08 *)(dipsbuf_addr+y*psd->linelen+x));

}


static void ALI_OSD_drawhlineex(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c)
{
	OSDDrv_DrawHorLine(( HANDLE )osd_dev, 0,  x,  y,  w, c);
}

static void ALI_OSD_drawvlineex(PSD psd, BU32 x, BU32 y, BU32 h, BU32 c)
{
	BU32 i;
	BU08 *dispbufaddr;
	i = h;
	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	while(i--)
	{
		*dispbufaddr = (BU08)c ;
		dispbufaddr += psd->linelen;				
	}
}

static void ALI_OSD_fillrectex(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h,BU32 c)
{
	struct OSDRect	r;
    r.uHeight = h;
	r.uWidth  = w;
	r.uLeft   = x;
	r.uTop    = y;
	OSDDrv_RegionFill(( HANDLE )osd_dev, 0, &r, c);	


}

static void ALI_OSD_blitex(PSD dstpsd, BU32 destx, BU32 desty, BU32 w,
					 BU32 h, PSD srcpsd, BU32 srcx, BU32 srcy)
{
}



/*do clip*/
static void ALI_OSD_linear32_putboxex ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void *buf)
{
	VSCR 		vscr;	
	vscr.vR.uHeight = Height;
	vscr.vR.uWidth  = Width;
	vscr.vR.uLeft   = x;
	vscr.vR.uTop    = y;		
	vscr.lpbScr = buf;
	
    OSDDrv_RegionWrite(( HANDLE )osd_dev, 0, &vscr,&vscr.vR);
}


/*clip to screen*/
static void ALI_OSD_linear32_getboxex ( PSD psd, BU32 x, BU32 y, BU32 Width, BU32 Height, void* buf )
{
	VSCR 		vscr;	
	vscr.vR.uHeight = Height;
	vscr.vR.uWidth  = Width;
	vscr.vR.uLeft   = x;
	vscr.vR.uTop    = y;	
	
	vscr.lpbScr = buf;
	
    OSDDrv_RegionRead(( HANDLE )osd_dev, 0, &vscr, &vscr.vR);
}

static void ALI_OSD_linear32_copyboxex (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2)
{

}


static	void ALI_OSD_linear32_putboxmaskex ( PSD psd, BU32 x, BU32 y, 
									  BU32 Width, BU32 Height, void *buf, BU32 cxx)
{
	BU32 h,w;

	BU08 *dispbufaddr;
	BU08 *dispbuftemp;	
	BU08 *bmpvalueaddr;	

	dispbufaddr = (BU08 *)(dipsbuf_addr+y*psd->linelen+x);
	dispbuftemp = dispbufaddr;
	bmpvalueaddr = (BU08 *)buf;

	for(h = 0;h<Height;h++)
 	{
		
		
		for(w=0;w<Width;w++)
		{
			if((BU08)cxx != (*bmpvalueaddr))
			{
				*dispbufaddr = *bmpvalueaddr;
			}
			dispbufaddr++;
			bmpvalueaddr++;
		}	
		dispbuftemp += psd->linelen;
		dispbufaddr = dispbuftemp;
 	}

}




