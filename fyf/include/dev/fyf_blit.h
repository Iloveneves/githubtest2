/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/10/29                                           	  */
/******************************************************************************/
#ifndef __FYF_BLIT_H_
#define __FYF_BLIT_H_
typedef struct
{ 
	void (*FYF_CPI_blit_fill_rectangle)(BU08 *dst,FYF_Rect_s *dst_rect,BU32 colorValue);
	void (*FYF_CPI_blit_copy_rectangle)(BU08 *dst,FYF_Rect_s *dst_rect,BU08 *src,FYF_Rect_s *src_rect);     
}FYF_BLIT_PORTING;
/******************************************************************************/
/*Description: register blit                                                  */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_blit_register(FYF_BLIT_PORTING *porting);
/******************************************************************************/
/*Description: fill rectangle                                                 */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_blit_fill_rectangle(BU08 *dst,FYF_Rect_s *dst_rect,BU32 colorValue); 
/******************************************************************************/
/*Description: copy rectangle                                                 */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_blit_copy_rectangle(BU08 *dst,FYF_Rect_s *dst_rect,BU08 *src,FYF_Rect_s *src_rect);
#endif



