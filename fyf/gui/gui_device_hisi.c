

#include "hi_stb_api.h"
#include "hi_osdgal.h"
#include "gui/gui_common.h"
#include "../../tpversion/ui_demo/osd/include/resource.h"
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 720
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 576
#endif

static BU16 UpdateDlgID = 0xffff;
static BU32 count = 0;
void RegisterPSD(void);
static PSD Hisi_OSD_open(PSD psd);
static void Hisi_OSD_close(PSD psd);

static void Hisi_OSD_drawpixel(PSD psd, BU32 x, BU32 y, BU32 c);
static BU32 Hisi_OSD_readpixel(PSD psd, BU32 x, BU32 y);
static void Hisi_OSD_drawhline(PSD psd, BU32 x, BU32 y, BU32 w,
						  BU32 c);
static void Hisi_OSD_drawvline(PSD psd, BU32 x, BU32 y, BU32 h,
						  BU32 c);
static void Hisi_OSD_fillrect(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h, BU32 c);
static void Hisi_OSD_linear32_putbox(PSD psd, BU32 x, BU32 y, BU32 w,
								BU32 h, void *buf);
static void Hisi_OSD_linear32_getbox(PSD psd, BU32 x, BU32 y, 
							BU32 w, BU32 h, void* buf);
static void Hisi_OSD_blit(PSD dstpsd, BU32 destx, BU32 desty, BU32 w,
					 BU32 h, PSD srcpsd, BU32 srcx, BU32 srcy);
static void Hisi_OSD_linear32_copybox (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2);
static	void Hisi_OSD_linear32_putboxmask ( PSD psd, BU32 x, BU32 y, 
									  BU32 w, BU32 h, void *buf, BU32 cxx);

static void Hisi_OSDUpdateRect(PSD psd, BU32 l, BU32 t, BU32 r, BU32 b);

static void Hisi_OSD_Printf(PU08 fmt);
static void*  Hisi_OSD_MemMalloc(BU32 size);
static void Hisi_OSD_MemFree(void * lpmem);
static void Hisi_OSD_TimerDelay(BU32 ms);

void Hisi_SetUpdateDlgID(BU16 id)
{
	UpdateDlgID = id;
}


GUI_ScreenDevice_s scrdev = {
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 
	NULL, 
	NULL,
	Hisi_OSD_open,
	Hisi_OSD_close,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,	
	Hisi_OSD_fillrect,
	Hisi_OSD_drawpixel,			/* DrawPixel subdriver*/
	Hisi_OSD_readpixel,			/* ReadPixel subdriver*/
	Hisi_OSD_drawhline,			/* DrawHLine subdriver*/
	Hisi_OSD_drawvline,			/* DrawVLine subdriver*/	
	NULL,					/* Blit subdriver*/
	Hisi_OSD_linear32_putbox,	/* PutBox subdriver*/
	Hisi_OSD_linear32_getbox,	/* CopyBox subdriver*/
	Hisi_OSD_linear32_putboxmask,/* PutBoxMask subdriver*/
	NULL,	/* CopyBox subdriver*/
	Hisi_OSDUpdateRect,
	//extern
	Hisi_OSD_Printf,
	Hisi_OSD_MemMalloc,
	Hisi_OSD_MemFree,
	Hisi_OSD_TimerDelay	
};


/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BU32 GUI_Hisi_SemaphoreCreate(const PU08 name,BS32 initial_count,BU32 wait_mode)
{
	return FYF_API_semaphore_create(name,initial_count,wait_mode);

}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BS32 GUI_Hisi_SemaphoreDestroy(BU32 handle)
{
	return FYF_API_semaphore_destroy( handle);
}

/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BS32 GUI_Hisi_SemaphoreWait(BU32 handle,BS32 wait_time)
{
	return FYF_API_semaphore_wait(handle,wait_time);
}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BS32 GUI_Hisi_SemaphoreRelease(BU32 handle)
{
	return FYF_API_semaphore_release(handle); 
}

GUI_Semaphore_s hisiguisem = 
{
	GUI_Hisi_SemaphoreCreate,
	GUI_Hisi_SemaphoreDestroy,
	GUI_Hisi_SemaphoreWait,
	GUI_Hisi_SemaphoreRelease
};
void RegisterPSD(void)
{
	printf("RegisterPSD\n");
	GUI_API_SetOSDDevice(&scrdev);
	GUI_API_SetSYSDevice_Semaphore(&hisiguisem);
}

static PSD Hisi_OSD_open(PSD psd)
{
	printf("Hisi_OSD_open\n");
	HI_S32 ret;
	psd->xres = psd->xvirtres = SCREEN_WIDTH;
	psd->yres = psd->yvirtres = SCREEN_HEIGHT;
	psd->bpp = 16;
	psd->ncolors = 1<<16;
	psd->linelen = (psd->xres * psd->bpp) >> 3;
	psd->size = psd->yres * psd->linelen;
	psd->gr_mode = 15;
	psd->planes = 1;
	psd->pixtype = 0;
	psd->flags = PSF_SCREEN;

	FYF_API_OSD_Open();
/*
	OSDGAL_OpenOSDLayer(OSDGAL_LAYER1, &OSD1_Handle);
	OSDGAL_Enable(OSD1_Handle,OSDGAL_ON);
*/
	return &scrdev;
}

static void Hisi_OSD_close(PSD psd)
{
	FYF_API_OSD_Close();
}


static void Hisi_OSD_drawpixel(PSD psd, BU32 x, BU32 y, BU32 c)
{
	
//	printf("%s %d %d %d\n", __FUNCTION__, x, y, c);
	FYF_API_OSD_DrawPixel(x, y, c);
}

static BU32 Hisi_OSD_readpixel(PSD psd, BU32 x, BU32 y)
{
//	printf("%s\n", __FUNCTION__);
	BU16 c;
	FYF_API_OSD_ReadPixel(x, y, &c);
	return (BU32)c;
}

static void Hisi_OSD_drawhline(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c)
{
//	printf("%s\n", __FUNCTION__);
	FYF_API_OSD_DrawLine(x, y, x + w, y, c);
                  
}

static void Hisi_OSD_drawvline(PSD psd, BU32 x, BU32 y, BU32 h, BU32 c)
{
//	printf("%s\n", __FUNCTION__);
	FYF_API_OSD_DrawLine(x, y, x, y + h, c);
}

static void Hisi_OSD_fillrect(PSD psd, BU32 x, BU32 y, BU32 w, BU32 h, BU32 c)
{
//	printf("%s\n", __FUNCTION__);

	FYF_API_OSD_FillRect(x, y, w, h, c);

}

static void Hisi_OSD_linear32_putbox(PSD psd, BU32 x, BU32 y, BU32 w,
								BU32 h, void *buf)
{
//	printf("%s\n", __FUNCTION__);
	FYF_API_OSD_WriteRectangleData(x, y, w, h, buf);
}

static void Hisi_OSD_linear32_getbox(PSD psd, BU32 x, BU32 y, 
							BU32 w, BU32 h, void* buf)
{
//	printf("%s\n", __FUNCTION__);
	FYF_API_OSD_ReadRectangleData(x, y, w, h, buf);
}

					 
static void Hisi_OSD_linear32_copybox (PSD psd, BU32 x1, BU32 y1, 
								  BU32 w, BU32 h, BU32 x2, BU32 y2)
{

}
static	void Hisi_OSD_linear32_putboxmask ( PSD psd, BU32 x, BU32 y, 
									  BU32 w, BU32 h, void *buf, BU32 cxx)
{
//	printf("%s\n", __FUNCTION__);
	FYF_API_OSD_WriteRectangleDataMask(x, y, w, h, buf, cxx);
}

static void Hisi_OSDUpdateRect(PSD psd, BU32 l, BU32 t, BU32 r, BU32 b)
{

	BU32 hwnd;
	GUI_Rect_s rect;
	if (UpdateDlgID == 0xffff)
	{
	//printf("-------------Hisi_OSDUpdateRect----------------\n");
	//	printf("l = %d, t = %d, r = %d, b = %d\n", l, t, r, b);

	//	printf("time1 = %d\n", FYF_API_time_ms());
		FYF_API_OSD_Refresh(l, t, r, b);
	//	printf("time2 = %d\n", FYF_API_time_ms());
	}
	else
	{
		switch(UpdateDlgID)
		{
		case ID_DLG_MainMenu:
			count++;
			if (count == 2)
			{
				//printf("-------------Hisi_OSDUpdateRect----------------\n");
				//printf("l = %d, t = %d, r = %d, b = %d\n", l, t, r, b);
				count = 0;
				UpdateDlgID = 0xffff;
				hwnd = GUI_API_GetDlgItem(ID_VFM_MainMenu);
				GUI_API_GetWindowRect(hwnd,&rect);
				APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
				FYF_API_OSD_Refresh(l, t, r, b);
			}	
			break;
		case ID_DLG_ProgManager:
			count++;
			if (count == 1)
			{
				count = 0;
				UpdateDlgID = 0xffff;
				hwnd = GUI_API_GetDlgItem(ID_VFM_ProgManager);
				GUI_API_GetWindowRect(hwnd,&rect);
				APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);
				FYF_API_OSD_Refresh(l, t, r, b);
			}
			break;
		case ID_DLG_EPG:
			count++;
			if (count == 23)
			{
				count = 0;
				UpdateDlgID = 0xffff;
				hwnd = GUI_API_GetDlgItem(ID_VFM_EPG);
				GUI_API_GetWindowRect(hwnd,&rect);
				APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);
				FYF_API_OSD_Refresh(l, t, r, b);
			}
			break;
		case ID_DLG_ProgList:
			count++;
			if (count == 5)
			{
				count = 0;
				UpdateDlgID = 0xffff;
				hwnd = GUI_API_GetDlgItem(ID_VFM_ProgList);
				GUI_API_GetWindowRect(hwnd,&rect);
				APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);
				FYF_API_OSD_Refresh(l, t, r, b);
			}
			break;
		defalut:
			break;	
			
		}
	}
	
	//FYF_API_OSD_Refresh(l, t, r, b);
	//printf("time2 = %d\n", FYF_API_time_ms());
	/*
	static BU32 count = 0;
	HI_OSDGAL_RECT_S rect;
	rect.u32x = l;
	rect.u32y = t;
	rect.u32Width = r;
	rect.u32Height = b;
	if (UpdateDlgID == 0xffff)
	{
		//printf("-------------Hisi_OSDUpdateRect----------------\n");
	//	printf("l = %d, t = %d, r = %d, b = %d\n", l, t, r, b);

	//	printf("time1 = %d\n", FYF_API_time_ms());
		FYF_API_OSD_Refresh(l, t, r, b);
	//	printf("time2 = %d\n", FYF_API_time_ms());
	}
	else
	{
		switch(UpdateDlgID)
		{
		case ID_DLG_EPG:
			count++;
			if (count == 1 || count == 16)
			{
				//printf("-------------Hisi_OSDUpdateRect----------------\n");
				//printf("l = %d, t = %d, r = %d, b = %d\n", l, t, r, b);
				if (count != 1)
				{
					count = 0;
					UpdateDlgID = 0xffff;
				}
				FYF_API_OSD_Refresh(l, t, r, b);
			}	
			break;
		defalut:
			break;	
			
		}
	}
	*/
}


static void Hisi_OSD_Printf(PU08 fmt)
{
	printf(fmt);
}

static void*  Hisi_OSD_MemMalloc(BU32 size)
{
	return FYF_API_malloc(size);
}

static void Hisi_OSD_MemFree(void * lpmem)
{
	FYF_API_free(lpmem);
}


static void Hisi_OSD_TimerDelay(BU32 ms)
{
	FYF_API_thread_sleep(ms);
}
