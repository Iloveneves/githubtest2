/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/09/28                                           	  */
/******************************************************************************/
#define __FYF_GDI_H_
#ifdef  __FYF_GDI_H_ 
typedef enum
{
	FYF_GDI_AVAIL_WIN_NOTIFY = 1
}FYF_GDI_IOCTL_e;
typedef struct
{
	int x;
	int y;
	int w;
	int h;
}FYF_GDI_WIN_RECT;
typedef struct
{
	BS32 (*FYF_CPI_gdi_get_info)(BS32 *width,BS32 *height,void **pBuffer,BS32 *bufWidth,BS32 *bufHeight);
	BS32 (*FYF_CPI_gdi_install_palette)(BU32 *pal,BS32 npals);
	BS32 (*FYF_CPI_gdi_draw_image)(BS32 x, BS32 y, BS32 w, BS32 h, BU08 *bits, BS32 w_src);
	BS32 (*FYF_CPI_gdi_set_alpha)(BS32 alpha);
	BS32 (*FYF_CPI_gdi_ioctl)(FYF_GDI_IOCTL_e op, void *arg);
}FYF_GDI_PORTING;
/******************************************************************************/
/*Description: register gdi                                                   */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_gdi_register(FYF_GDI_PORTING *porting);
/******************************************************************************/
/*Description: get display information                                        */
/*Input      : no														      */
/*Output     : width,height,pBuffer,bufWidth,bufHeight                        */
/*Return     : FYF_OK:success FYF_ERR:fail                                    */
/******************************************************************************/
BS32 FYF_API_gdi_get_info(BS32 *width,BS32 *height,void **pBuffer,BS32 *bufWidth,BS32 *bufHeight);
/******************************************************************************/
/*Description: install palette, only 8bits need                               */
/*Input      : pal,npals												      */
/*Output     : no										                      */
/*Return     : FYF_OK:success FYF_ERR:fail                                    */
/******************************************************************************/
BS32 FYF_API_gdi_install_palette(BU32 *pal,BS32 npals);
/******************************************************************************/
/*Description: draw image						                              */
/*Input      : x,y,w,h,bits,w_src										      */
/*Output     : no										                      */
/*Return     : FYF_OK:success FYF_ERR:fail                                    */
/******************************************************************************/
BS32 FYF_API_gdi_draw_image(BS32 x, BS32 y, BS32 w, BS32 h, BU08 *bits, BS32 w_src);
/******************************************************************************/
/*Description: set alpha						                              */
/*Input      : alpha													      */
/*Output     : no										                      */
/*Return     : FYF_OK:success FYF_ERR:fail                                    */
/******************************************************************************/
BS32 FYF_API_gdi_set_alpha(BS32 alpha);
/******************************************************************************/
/*Description: gdi ioctl                                                      */
/*Input      : op          										      		  */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_gdi_ioctl(FYF_GDI_IOCTL_e op, void *arg);
#endif



