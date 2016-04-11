/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author cwd 2009/08/10                                           	  */
/******************************************************************************/
#define __FYF_OSD_H_
#ifdef  __FYF_OSD_H_ 


typedef enum
{
    FYF_OSD_PF_CLUT8 = 0,  /**< 调色板 */
    FYF_OSD_PF_CLUT1,
    FYF_OSD_PF_CLUT4,
    FYF_OSD_PF_4444,       /**< 一个像素占16bit, ARGB每分量占4bit, 按地址由高至低排列 */
    FYF_OSD_PF_0444,       /**< 一个像素占16bit, ARGB每分量占4bit, 按地址由高至低排列, A分量不起作用 */

    FYF_OSD_PF_1555,       /**< 一个像素占16bit, RGB每分量占5bit, A分量1bit, 按地址由高至低排列 */
    FYF_OSD_PF_0555,       /**< 一个像素占16bit, RGB每分量占5bit, A分量1bit, 按地址由高至低排列, A分量不起作用 */
    FYF_OSD_PF_565,        /**< 一个像素占16bit, RGB每分量分别占5bit,6bit,5bit, 按地址由高至低排列 */
    FYF_OSD_PF_8888,       /**< 一个像素占32bit, ARGB每分量占8bit, 按地址由高至低排列 */
    FYF_OSD_PF_0888,       /**< 一个像素占32bit, ARGB每分量占8bit, 按地址由高至低排列, A分量不起作用 */

    FYF_OSD_PF_YUV420,     /**< 海思定义的semi-planar YUV 420格式 */
    FYF_OSD_PF_YUV422,     /**< 海思定义的semi-planar YUV 422格式 */
    FYF_OSD_PF_BUTT
} FYF_OSD_PF_E;


BS32 FYF_API_OSD_open();
BS32 FYF_API_OSD_close();

//HI_OSDGAL_HANDLE FYF_API_OSD_GetHandle();

/*画像素*/
BS32 FYF_API_OSD_DrawPixel(BU32 x, BU32 y, BU32 c);

/* 获取某个像素的值 */
BS32 FYF_API_OSD_ReadPixel(BU32 x, BU32 y, BU08 *data);
/* 绘制线 */	
BS32 FYF_API_OSD_DrawLine(BU32 x1, BU32 y1, BU32 x2, BU32 y2, BU32 c);


/* 填充矩形 */
BS32 FYF_API_OSD_FillRect(BU32 x, BU32 y, BU32 w, BU32 h, BU32 c);
/* 往指定矩形框内写数据 */
BS32 FYF_API_OSD_WriteRectangleData(BU32 x, BU32 y, BU32 w, BU32 h, void *buf);


/* 读取指定矩形框内的数据 */

BS32 FYF_API_OSD_ReadRectangleData(BU32 x, BU32 y, BU32 w, BU32 h, void* buf);


BS32 FYF_API_OSD_WriteRectangleDataMask (BU32 x, BU32 y, BU32 w, BU32 h, void *buf, BU32 maskcolor);

BS32 FYF_API_OSD_Refresh(BU32 x, BU32 y, BU32 w, BU32 h);


#endif



