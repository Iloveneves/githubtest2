/*********************************************************************
  Copyright (c) 2005 Embedded Internet Solutions, Inc
  All rights reserved. You are not allowed to copy or distribute
  the code without permission.
  This is the demo implenment of the Graphics Porting APIs needed by iPanel MiddleWare.
  Maybe you should modify it accorrding to Platform.

Note: the "int" in the file is 32bits

$ver0.0.0.1 $author Zouxianyun 2005/04/28
 *********************************************************************/
#include "ipanel_graphics.h"
#include "ipanel_base.h"
#include "trd_ipanel_init.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"

extern BU08 dlg_timeout_state;
#define IPANEL_GRAPH_DEBUG //IPANEL_DEBUG

extern BU08  g_bRefreshOsd;
extern BU08 bStopwhitescreen;

/******************************************************************/
/*Description: Get Display information.                           */
/*             width,height: VOD/IPTV output area availability,   */
/*             bufWidth,bufHeight: show the actual buffer size,   */
/*pBuffer: Return to the VOD/IPTV directly output the buffer area */
/*pointer address£»if *pBuffer = 0; VOD/IPTV can auto-allocate the*/
/*buffer area£¨buffer£©£¬then later call eis_draw_image output out*/
/* to the actual display buffer area £¨buffer£©£»if *pBuffer =    */
/* actual display buffer area address£¬VOD/IPTV shall directly    */
/* output data to buffer area£¬and at the same time eis_draw_image*/
/* can also be invoked.                                           */
/*Input      : Address of wid, hgt, FrameBuffer, bufW, bufH       */
/*Output     : width, height, FB, bufWidth, bufHeight             */
/*Return     : 0 -- success, -1 -- fail.                          */
/******************************************************************/
int ipanel_porting_graphics_getInfo(int *width, int *height, void **pbuffer, int *bufWidth,int *bufHeight)
{
    BS32 ret = FYF_ERR;
	BU32 buff;
	
    IPANEL_GRAPH_DEBUG("\n");
	FYF_API_gdi_get_info(width, height,  pbuffer, bufWidth, bufHeight);
	IPANEL_GRAPH_DEBUG("width = %d, height = %d, bufwidth = %d, bufheight = %d\n",*width, *height,  *bufWidth, *bufHeight );

	if (*pbuffer == NULL)
  	{
  		IPANEL_ERROR("pbuffer = %p\n", (*pbuffer));
    }
	return FYF_OK;

}

/******************************************************************/
/*Description: Set Palette, Only 8bpp or less 8bpp need the func. */
/*Input      : pal[] -- color Array, format 0x00RRGGBB,           */
/*     npals -- counts of colors in Palette, 16-4bpp 256-8bpp etc */
/*Output     : Install Palette datas to STB Palette               */
/*Return     : 0 -- success, -1 -- fail.                          */
/******************************************************************/
int ipanel_porting_install_palette(unsigned int *pal, int npals)
{
    return 0;
}

/******************************************************************/
/*Description: Draw a rectangle image.                            */
/*Input      : start point(x,y),width(w),height(h);               */
/*             color data(bits), and data width(w_src) per line;  */
/*Output     : No                                                 */
/*Return     : 0 -- success, Other fail.                          */
/******************************************************************/
/**Note: segguest OSD address is  0xC0000000 in following.        */

int ipanel_porting_draw_image(int x, int y, int w, int h, unsigned char *bits, int w_src)
{
	int ret = -1;
	IPANEL_GRAPH_DEBUG(" x= %d y = %d w = %d h = %d \n",x,y,w,h);
	
	if (FYF_FALSE == g_bRefreshOsd || dlg_timeout_state == 1)
    {
    	IPANEL_GRAPH_DEBUG("g_bRefreshOsd = FYF_FALSE\n");
    	return FYF_OK;
    }
	
#if  0	
	if(/*w == 640 && h==526 &&*/ bStopwhitescreen)
	{
		ret = FYF_API_GetLinkStatus();
		if (ret == 1)
		{
		
			bStopwhitescreen = FYF_FALSE;
			printf("bStopwhitescreen = 0\n");
			
			if(bits[100]==0xff&&bits[101]==0xff&&bits[1000]==0xff&&bits[1001]==0xff&&bits[10000]==0xff&&bits[10001]==0xff&&bits[100000]==0xff&&bits[100001]==0xff&&bits[200000]==0xff&&bits[200001]==0xff&&bits[500000]==0xff&&bits[500001]==0xff\
				&&bits[337280]==0xff&&bits[337281]==0xff&&bits[339440]==0xff&&bits[339441]==0xff)
			{
				//if(CableModemMSGErrCount==0)
				{
				    //FYF_API_thread_sleep(10);
				    
				    printf("white screen\n");
					return 0;
				}
			}
		}
	}
#endif
	
	FYF_API_gdi_draw_image( x  ,  y  , w,  h,  bits,  w_src);

	return FYF_OK;
}

/******************************************************************/
/*Description: Display data on mpeg iFrame Layer.                 */
/*Input      : Data buf, and data length                          */
/*Output     : No                                                 */
/*Return     : No                                                 */
/******************************************************************/
void ipanel_porting_mpegiframe_display(const char *buf, int len)
{
    /*Draw in iFrame */
    IPANEL_GRAPH_DEBUG("ipanel_porting_mpegiframe_display\n");
}


