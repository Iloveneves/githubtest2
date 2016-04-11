/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the Graphics Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_GRAPHICS_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_GRAPHICS_API_FUNCTOTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*Get Display information, The function is belong to display part.*/
int ipanel_porting_graphics_getInfo(int *width, int *height, void **pbuffer, int *bufWidth,int *bufHeight);

/*install palette, only 8bits need, the function is belong to display part.*/
int ipanel_porting_install_palette(unsigned int *pal, int npals);

/*draw image, This function is belong to display part.*/
int ipanel_porting_draw_image(int x, int y, int w, int h, unsigned char *bits, int w_src);

/*display data on mpeg iFrame Layer.*/
void ipanel_porting_mpegiframe_display(const char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif
