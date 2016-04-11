/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: appmngsvr.c
 * Description:
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-09-01   diaoqiangwei/d60770
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <memory.h>


//#include "hi_go.h"

#include "hi_stb_api.h"
#include "hi_osdgal.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif


	/***********************************
		HI_OSD_BPP CANT SET 1  for HI3110E
	***********************************/
#define UI_OSD_BPP					(2)
#define HI_OSD_BPP              			(2)
#define HI_OSD_STRIDE         			(HI_OSD_MAXWIDTH*HI_OSD_BPP)
#define HI_OSD_MAXCOLORCULT     	(256)
#define HI_OSD_ANTIFLICKER			(1)

#define OSDGAL_PRINT printf

#define CHECK_OSDHANDLE(handle)\
 do{\
    if(0 > handle || OSDGAL_LAYER_BUTT <= handle)\
    {\
        OSDGAL_PRINT("Error ,invalied handle %d.\r\n",handle);\
        return (HI_ERR_OSDGAL_INVARG);\
    }\
 }while(0)


#define CHECK_PTR(pointer) \
do{\
    if(NULL == pointer)\
    {\
        OSDGAL_PRINT("Error ,invalied pointer NULL.\r\n");\
        return (HI_ERR_OSDGAL_INVARG);\
    }\
}while(0)

#define CHECK_BOOL(b) \
do{\
    if(OSDGAL_OFF != b && OSDGAL_ON != b)\
    {\
        OSDGAL_PRINT("Error ,invalied BOOL type %d.\r\n",b);\
        return HI_ERR_OSDGAL_INVARG;\
    }\
}while(0)

#define ASSERT_NOPEN(VALIED,FD) \
do{\
    if(HI_FALSE == VALIED && -1 != FD)\
    {\
        OSDGAL_PRINT("Error ,not inited ,VALIED=%d, FD=%d.\r\n",VALIED,FD);\
        return (HI_ERR_OSDGAL_NOTINIT);\
    }\
    else if(HI_TRUE == VALIED && -1 == FD)\
    {\
        OSDGAL_PRINT("Error ,invalied state, VALIED=%d, FD=%d.\r\n",VALIED,FD);\
        return HI_ERR_OSDGAL_INVSTATE;\
    }\
    else if(HI_TRUE == VALIED && -1 != FD)\
    {\
         return (HI_SUCCESS);\
    }\
}while(0)


#define ASSERT_OPENED(VALIED,FD) \
do{\
    if(HI_TRUE != VALIED)\
    {\
         OSDGAL_PRINT("Error ,not inited ,VALIED=%d, FD=%d.\r\n",VALIED,FD);\
         return (HI_ERR_OSDGAL_NOTINIT);\
    }\
    if(HI_TRUE == VALIED && -1 == FD)\
    {\
        OSDGAL_PRINT("Error ,invalied state, VALIED=%d, FD=%d.\r\n",VALIED,FD);\
        return (HI_ERR_OSDGAL_INVSTATE);\
    }\
}while(0)


#define CHECK_RECT(pRECT)\
do{                                   \
    if(HI_OSD_MAXWIDTH   <= pRECT->u32x     \
        || HI_OSD_MAXWIDTH   <  pRECT->u32Width    \
        || HI_OSD_MAXHEIGHT  <= pRECT->u32y     \
        || HI_OSD_MAXHEIGHT  <  pRECT->u32Height     \
        || 0                 == pRECT->u32Width     \
        || 0                 == pRECT->u32Height     \
        || HI_OSD_MAXWIDTH   <  (pRECT->u32x+pRECT->u32Width) \
        || HI_OSD_MAXHEIGHT  <  (pRECT->u32y+pRECT->u32Height))\
    {                                 \
        OSDGAL_PRINT("Error ,invalied rect,x=%d, y=%d, w=%d ,h=%d\r\n",\
                        pRECT->u32x,\
                        pRECT->u32y,\
                        pRECT->u32Width,\
                        pRECT->u32Height);\
        return (HI_ERR_OSDGAL_INVARG);  \
    }                                 \
}while(0);


#define CHECK_PIX(x,y)                \
do{                                   \
    if(    HI_OSD_MAXWIDTH   <= x     \
        || HI_OSD_MAXHEIGHT  <= y)    \
    {                                 \
        OSDGAL_PRINT("Error ,invalied rect,x=%d, y=%d.\r\n",x,y);\
        return (HI_ERR_OSDGAL_INVARG);  \
    }                                 \
}while(0)


	typedef struct hi_GuiContent_S
	{
		HI_U32  u32PhyAddr;
		HI_U8*  pu8Screen;
		HI_U32  u32hLayer;
		HI_U32  u32hLayerSurface;
		HI_BOOL bValied;
	}HI_GuiContent_S;


	/*全局OSD上下文定义*/
	static HI_GuiContent_S g_astruGuiContent[OSDGAL_LAYER_BUTT] ;
#if (UI_OSD_BPP == 1)
	static HI_VOID*  u32DispPhy;
	static HI_VOID*  u32DispVir;
#if (HI_OSD_BPP ==2)
	static HI_U16 uClut[256];
#else
	static HI_U32 uClut[256];
#endif
#endif


	/*画点*/
	inline void draw_pixel(HI_GuiContent_S* pstruOsdContent,
	                       HI_U32 u32X,
	                       HI_U32 u32Y,
	                       HI_U32 u32Color)
	{
#if (UI_OSD_BPP == 1)
		{
			HI_U8* p8Pix = u32DispVir + u32Y * HI_OSD_MAXWIDTH *UI_OSD_BPP + u32X *UI_OSD_BPP;
			*p8Pix = (HI_U8)u32Color;
#if (HI_OSD_BPP ==2)
			HI_U16* pPix = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE  + u32X *HI_OSD_BPP;
#else
			HI_U32* pPix = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE  + u32X *HI_OSD_BPP;
#endif
			*pPix = uClut[u32Color];

		}
#else
		{
			HI_U8* p8Pix = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X* HI_OSD_BPP;
			HI_U16* pPix = (HI_U16 *)p8Pix;
			*pPix = (HI_U16)u32Color;
		}
#endif
		return;
	}

	/*以横斜线画点*/
	inline void draw_xish_line(HI_GuiContent_S* pstruOsdContent,
	                           HI_U32 u32X,
	                           HI_U32 u32Y,
	                           HI_S32 s32Dx,
	                           HI_S32 s32Dy,
	                           HI_S32 s32Direct,
	                           HI_U32 u32Color)
	{
		HI_S32 s32DyX2 = 0;
		HI_S32 s32DyX2_M_DxX2 = 0;
		HI_S32 s32Error = 0;

		s32DyX2 = s32Dy * 2;
		s32DyX2_M_DxX2 = s32DyX2 -(s32Dx * 2);
		s32Error = s32DyX2 - s32Dx;

		draw_pixel(pstruOsdContent, u32X, u32Y, u32Color);

		while (s32Dx--)
		{
			if (s32Error >= 0)
			{
				u32Y++;
				s32Error += s32DyX2_M_DxX2;
			}
			else
			{
				s32Error += s32DyX2;
			}
			u32X += s32Direct;
			draw_pixel(pstruOsdContent, u32X, u32Y, u32Color);
		}
	}

	/*以竖斜线画点*/
	inline void draw_yish_line(HI_GuiContent_S* pstruOsdContent,
	                           HI_U32 u32X,
	                           HI_U32 u32Y,
	                           HI_S32 s32Dx,
	                           HI_S32 s32Dy,
	                           HI_S32 s32Direct,
	                           HI_U32 u32Color)
	{
		HI_S32 s32DxX2 = 0;
		HI_S32 s32DxX2_M_DyX2 = 0;
		HI_S32 s32Error = 0;

		s32DxX2 = s32Dx * 2;
		s32DxX2_M_DyX2 = s32DxX2 -(s32Dy * 2);
		s32Error = s32DxX2 - s32Dy;

		draw_pixel(pstruOsdContent, u32X, u32Y, u32Color);

		while (s32Dy--)
		{
			if (s32Error >= 0)
			{
				u32X++;
				s32Error += s32DxX2_M_DyX2;
			}
			else
			{
				s32Error += s32DxX2;
			}
			u32Y ++;
			draw_pixel(pstruOsdContent, u32X, u32Y, u32Color);
		}
	}

	/*以画直线*/
	inline void draw_line(HI_GuiContent_S* pstruOsdContent,
	                      HI_U32 u32X1,
	                      HI_U32 u32Y1,
	                      HI_S32 u32X2,
	                      HI_S32 u32Y2,
	                      HI_U32 u32Color)
	{
		HI_S32 s32Dx = 0;
		HI_S32 s32Dy = 0;

		if (u32Y1 > u32Y2)
		{
			HI_S32 s32Temp = u32Y1;
			u32Y1 = u32Y2;
			u32Y2 = s32Temp;

			s32Temp = u32X1;
			u32X1 = u32X2;
			u32X2 = s32Temp;
		}

		s32Dx = (HI_S32)u32X2 - (HI_S32)u32X1;
		s32Dy = (HI_S32)u32Y2 - (HI_S32)u32Y1;

		if (0 < s32Dx)
		{
			if (s32Dx > s32Dy)
			{
				draw_xish_line(pstruOsdContent,u32X1, u32Y1, s32Dx, s32Dy, 1, u32Color);
			}
			else
			{
				draw_yish_line(pstruOsdContent,u32X1, u32Y1, s32Dx, s32Dy, 1, u32Color);
			}
		}
		else
		{
			s32Dx = -s32Dx;
			if (s32Dx > s32Dy)
			{
				draw_xish_line(pstruOsdContent,u32X1, u32Y1, s32Dx, s32Dy, -1, u32Color);
			}
			else
			{
				draw_yish_line(pstruOsdContent,u32X1, u32Y1, s32Dx, s32Dy, -1, u32Color);
			}
		}

	}


	/*填充指定长度的水平线*/
	inline void fill_line(HI_GuiContent_S* pstruOsdContent,
	                      HI_U32 u32X,
	                      HI_U32 u32Y,
	                      HI_U32 u32LWidth,
	                      HI_U32 u32Color)
	{


		int i;
#if (UI_OSD_BPP == 1)
		{
			HI_U8* pu8Buf = u32DispVir + u32Y * HI_OSD_MAXWIDTH * UI_OSD_BPP+ u32X*UI_OSD_BPP;
			HI_U8 u8Color = (HI_U8)u32Color;
			memset((void*)pu8Buf, u32Color, u32LWidth*UI_OSD_BPP);
#if (HI_OSD_BPP ==2)
			HI_U16* pu16Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE  + u32X *HI_OSD_BPP;
#else
			HI_U32* pu16Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE  + u32X *HI_OSD_BPP;
#endif
			for (i =0; i<u32LWidth; i++)
			{
				pu16Buf[i] = uClut[u8Color];
			}

		}
#else
		{
			HI_U8* pu8Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
			HI_U16* pu16Buf = (HI_U16*) pu8Buf;
			HI_U16 u16Color = (HI_U16)u32Color;

			/* only 8bit color */
			for (i =0; i<u32LWidth; i++)
			{
				pu16Buf[i] = u16Color;
			}
		}
#endif
		return;
	}

	/*设置指定长度的水平线*/
	inline void Set_line(HI_GuiContent_S* pstruOsdContent,
	                     HI_U32 u32X,
	                     HI_U32 u32Y,
	                     HI_U32 u32LWidth,
	                     HI_U8* pu8Data)
	{
		int i;
#if UI_OSD_BPP == 1
		HI_U8* pu8Buf = u32DispVir + u32Y * HI_OSD_MAXWIDTH*UI_OSD_BPP + u32X*UI_OSD_BPP ;
		memcpy((void*)pu8Buf, pu8Data, u32LWidth*UI_OSD_BPP);
#if (HI_OSD_BPP ==2)
		HI_U16* pu16Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
#else
		HI_U32* pu16Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
#endif
		for (i =0; i<u32LWidth; i++)
		{
			pu16Buf[i] = uClut[pu8Data[i]];
		}

#else
		HI_U8* pu8Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
		HI_U16* pu16Buf = (HI_U16*) pu8Buf;
		HI_U16* pu16Data = (HI_U16*)pu8Data;
		/* only 8bit color */
		for (i =0; i<u32LWidth; i++)
		{
			pu16Buf[i] = pu16Data[i];
		}
#endif
		return;
	}

	/*设置指定长度的水平线*   cwd 2009-07-31*/
	inline void Set_linemask(HI_GuiContent_S* pstruOsdContent,
	                         HI_U32 u32X,
	                         HI_U32 u32Y,
	                         HI_U32 u32LWidth,
	                         HI_U8* pu8Data,
	                         HI_U32 maskcolor)
	{
		int i;
#if UI_OSD_BPP == 1
		HI_U8* pu8Buf = u32DispVir + u32Y * HI_OSD_MAXWIDTH*UI_OSD_BPP + u32X*UI_OSD_BPP ;
		memcpy((void*)pu8Buf, pu8Data, u32LWidth*UI_OSD_BPP);
#if (HI_OSD_BPP ==2)
		HI_U16* pu16Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
#else
		HI_U32* pu16Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
#endif
		for (i =0; i<u32LWidth; i++)
		{
			pu16Buf[i] = uClut[pu8Data[i]];
		}

#else
		HI_U8* pu8Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
		HI_U16* pu16Buf = (HI_U16*) pu8Buf;
		HI_U16* pu16Data = (HI_U16*)pu8Data;
		/* only 8bit color */
		for (i =0; i<u32LWidth; i++)
		{
			if (pu16Data[i] != (HI_U16)maskcolor)
				pu16Buf[i] = pu16Data[i];
		}
#endif
		return;
	}

	/*获取指定长度的水平线数据*/
	inline void Get_line(HI_GuiContent_S* pstruOsdContent,
	                     HI_U32 u32X,
	                     HI_U32 u32Y,
	                     HI_U32 u32LWidth,
	                     HI_U8* pu8Data)
	{
#if UI_OSD_BPP==1
		HI_U8* pu8Buf = u32DispVir + u32Y * HI_OSD_MAXWIDTH*UI_OSD_BPP + u32X * UI_OSD_BPP;
		memcpy((void*)pu8Data, pu8Buf, u32LWidth * UI_OSD_BPP);
#else
		HI_U8* pu8Buf = pstruOsdContent->pu8Screen + u32Y * HI_OSD_STRIDE + u32X * HI_OSD_BPP;
		memcpy((void*)pu8Data, pu8Buf, u32LWidth * HI_OSD_BPP);
#endif
		return;
	}

	inline void draw_ellopsefill(HI_GuiContent_S* pstruOsdContent,
	                             HI_U32 u32X,
	                             HI_U32 u32Y,
	                             HI_U32 u32A,
	                             HI_U32 u32B,
	                             HI_U32 u32Color)
	{

		HI_S32 s32Wx = 0;
		HI_S32 s32Wy = 0;
		HI_S32 s32Thresh = 0;
		HI_S32 s32Asq = u32A * u32A;
		HI_S32 s32Bsq = u32B * u32B;

		HI_S32 s32Xa = 0;
		HI_S32 s32Ya = 0;


		s32Wx = 0;
		s32Wy = u32B;
		s32Xa = 0;
		s32Ya = 1 +s32Asq * u32B * 2;

		s32Thresh = s32Asq / 4 - s32Asq * u32B;

		for (;;)
		{

			s32Thresh += s32Xa + s32Bsq;
			if (s32Thresh >= 0)
			{
				s32Ya -= s32Asq * 2;
				s32Thresh -= s32Ya;
				s32Wy--;
			}
			s32Xa += s32Bsq * 2;
			s32Wx++;
			if (s32Xa >= s32Ya)
			{
				break;
			}

			draw_line(pstruOsdContent, u32X - s32Wx, u32Y - s32Wy,  u32X + s32Wx,  u32Y - s32Wy, u32Color);
			draw_line(pstruOsdContent, u32X - s32Wx, u32Y + s32Wy-1,  u32X + s32Wx,  u32Y + s32Wy-1, u32Color);

		}


		draw_line(pstruOsdContent, u32X - u32A, u32Y,  u32X + u32A,  u32Y, u32Color);

		s32Wx = u32A;
		s32Wy = 0;
		s32Xa = s32Bsq * 2 * u32A;
		s32Ya = 0;
		s32Thresh = s32Bsq / 4 - s32Bsq * u32A;

		for (;;)
		{

			s32Thresh += s32Ya + s32Asq;
			if (s32Thresh >= 0)
			{
				s32Xa -= s32Bsq * 2;
				s32Thresh -= s32Xa;
				s32Wx--;
			}
			s32Ya += s32Asq * 2;
			s32Wy++;
			if (s32Ya >= s32Xa)
			{
				break;
			}

			draw_line(pstruOsdContent, u32X - s32Wx, u32Y - s32Wy,  u32X + s32Wx,  u32Y - s32Wy, u32Color);
			draw_line(pstruOsdContent, u32X - s32Wx, u32Y + s32Wy,  u32X + s32Wx,  u32Y + s32Wy, u32Color);

		}

	}


	inline void draw_ellopse(HI_GuiContent_S* pstruOsdContent,
	                         HI_U32 u32X,
	                         HI_U32 u32Y,
	                         HI_U32 u32A,
	                         HI_U32 u32B,
	                         HI_U32 u32Color)
	{
		HI_S32 s32Wx = 0;
		HI_S32 s32Wy = 0;
		HI_S32 s32Thresh = 0;
		HI_S32 s32Asq = u32A * u32A;
		HI_S32 s32Bsq = u32B * u32B;

		HI_S32 s32Xa = 0;
		HI_S32 s32Ya = 0;

		draw_pixel(pstruOsdContent, u32X, u32Y - u32B, u32Color);
		draw_pixel(pstruOsdContent, u32X, u32Y + u32B, u32Color);

		s32Wx = 0;
		s32Wy = u32B;
		s32Xa = 0;
		s32Ya = 1 + s32Asq * u32B * 2;

		s32Thresh = s32Asq / 4 - s32Asq * u32B;
		for (;;)
		{

			s32Thresh += s32Xa + s32Bsq;
			if (s32Thresh >= 0)
			{
				s32Ya -= s32Asq * 2;
				s32Thresh -= s32Ya;
				s32Wy--;
			}
			s32Xa += s32Bsq * 2;
			s32Wx++;
			if (s32Xa >= s32Ya)
			{
				break;
			}

			draw_pixel(pstruOsdContent, u32X + s32Wx, u32Y - s32Wy, u32Color);
			draw_pixel(pstruOsdContent, u32X - s32Wx, u32Y - s32Wy, u32Color);
			draw_pixel(pstruOsdContent, u32X + s32Wx, u32Y + s32Wy, u32Color);
			draw_pixel(pstruOsdContent, u32X - s32Wx, u32Y + s32Wy, u32Color);
		}

		draw_pixel(pstruOsdContent, u32X + u32A, u32Y, u32Color);
		draw_pixel(pstruOsdContent, u32X - u32A, u32Y, u32Color);

		s32Wx = u32A;
		s32Wy = 0;
		s32Xa = s32Bsq * 2 * u32A;
		s32Ya = 0;
		s32Thresh = s32Bsq / 4 - s32Bsq * u32A;

		for (;;)
		{

			s32Thresh += s32Ya + s32Asq;
			if (s32Thresh >= 0)
			{
				s32Xa -= s32Bsq * 2;
				s32Thresh -= s32Xa;
				s32Wx--;
			}
			s32Ya += s32Asq * 2;
			s32Wy++;
			if (s32Ya >= s32Xa)
			{
				break;
			}

			draw_pixel(pstruOsdContent, u32X + s32Wx, u32Y - s32Wy, u32Color);

			draw_pixel(pstruOsdContent, u32X - s32Wx, u32Y - s32Wy, u32Color);

			draw_pixel(pstruOsdContent, u32X + s32Wx, u32Y + s32Wy, u32Color);

			draw_pixel(pstruOsdContent, u32X - s32Wx, u32Y + s32Wy, u32Color);
		}

	}


	/* OSD初始化函数 */
	HI_S32 OSDGAL_Init()
	{
		HI_S32 i = 0;
		HI_S32 s32Ret = 0;
		HI_BOOL bInit = HI_FALSE;
		for (i = OSDGAL_LAYER1; i < OSDGAL_LAYER_BUTT; i++)
		{
			if (g_astruGuiContent[i].bValied != HI_FALSE)
			{
				bInit = HI_TRUE;
				break;
			}
		}

		if (bInit == HI_TRUE)
		{
			return HI_SUCCESS;
		}

		s32Ret = HI_GO_Init();
		if (HI_SUCCESS != s32Ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
			return HI_FAILURE;
		}

		memset((void*)(&g_astruGuiContent[0]),0,sizeof(HI_GuiContent_S )* OSDGAL_LAYER_BUTT);
		for (i = OSDGAL_LAYER1; i < OSDGAL_LAYER_BUTT; i++)
		{
			g_astruGuiContent[i].bValied = HI_FALSE;
			g_astruGuiContent[i].u32hLayer  = -1;
			g_astruGuiContent[i].u32hLayerSurface  = -1;
		}
		return HI_SUCCESS;
	}


	HI_S32 OSDGAL_Refresh(IN HI_OSDGAL_HANDLE hHandle,IN const HI_OSDGAL_RECT_S *pstruRectSize)
	{

#if (HI_OSD_ANTIFLICKER==1)
		HI_S32 ret = 0;
		HI_GuiContent_S * pstruGuiContent = NULL;
		HI_HANDLE hLayer;
		CHECK_OSDHANDLE(hHandle);
		//CHECK_PTR(pstruRectSize);

		//CHECK_RECT(pstruRectSize);
		pstruGuiContent = &g_astruGuiContent[hHandle];

		hLayer = pstruGuiContent->u32hLayer;
		ret = HI_GO_RefreshLayer(hLayer);
		//printf("%s: %d ErrorCode=0x%x hLayer = %d hHandle= %d\n", __FILE__, __LINE__, ret , hLayer , hHandle);
		//printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
		if (HI_SUCCESS != ret)// && ret != 0xffffffff)
		{
			//if(ret != 0xffffffff)
			{
				//	ret = HI_GO_RefreshLayer(hLayer);
				if (HI_SUCCESS != ret)
				{
					printf("%s: %d ErrorCode=0x%x hLayer = %d hHandle= %d\n", __FILE__, __LINE__, ret , hLayer , hHandle);
					return HI_FAILURE;
				}
			}
		}

#endif

		return HI_SUCCESS;

	}

	/* 打开OSD图层设备，返回图层句柄 */
	HI_S32 OSDGAL_OpenOSDLayer(IN HI_OSDGAL_LAYER_E eOSDLayer,
	                           OUT HI_OSDGAL_HANDLE *pHandle)
	{
		HI_S32 ret = 0;
		HI_HANDLE hLayer,hLayerSurface;
		HIGO_LAYER_INFO_S stLayerInfo;
		HI_PIXELDATA data;
		HI_RECT struRectSize;
		HI_GuiContent_S * pstruGuiContent = NULL;


		CHECK_OSDHANDLE(eOSDLayer);
		CHECK_PTR(pHandle);

		pstruGuiContent = &g_astruGuiContent[eOSDLayer];
		ASSERT_NOPEN(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		stLayerInfo.Width  = HI_OSD_MAXWIDTH;
		stLayerInfo.Height = HI_OSD_MAXHEIGHT;

#if (HI_OSD_ANTIFLICKER == 1)
		stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_FLIP | HIGO_LAYER_FLUSH_DOUBBUFER | HIGO_LAYER_FLUSH_ANTIFLICKER;
#else
		stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
#endif

		stLayerInfo.FlipCount   = 2;

#if (HI_OSD_BPP ==2)
		stLayerInfo.PixelFormat = HIGO_PF_1555;
#else
		stLayerInfo.PixelFormat = HIGO_PF_8888;
#endif

		if (eOSDLayer == OSDGAL_LAYER1)
			stLayerInfo.LayerID = HIGO_LAYER_RGB0;
		else if (eOSDLayer == OSDGAL_LAYER2)
			stLayerInfo.LayerID = HIGO_LAYER_RGB1;

		ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
		if (HI_SUCCESS != ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			return HI_FAILURE;
		}

		printf(" OSDGAL_OpenOSDLayer hLayer= %d handle = %d \n",hLayer,eOSDLayer);

		pstruGuiContent->u32hLayer = hLayer;

		/*获取图层相关surface信息，以及显存地址等*/
		ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			ret = HI_GO_DestroyLayer(hLayer);
			return HI_FAILURE;
		}

		pstruGuiContent->u32hLayerSurface = hLayerSurface;


		ret = HI_GO_LockSurface(hLayerSurface, data);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			ret = HI_GO_DestroyLayer(hLayer);
			return HI_FAILURE;
		}

		ret = HI_GO_UnlockSurface(hLayerSurface);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			ret = HI_GO_DestroyLayer(hLayer);
			return HI_FAILURE;
		}

		pstruGuiContent->u32PhyAddr  = data[0].pPhyData;
		pstruGuiContent->pu8Screen = data[0].pData;
		pstruGuiContent->bValied = HI_TRUE;
		*pHandle = eOSDLayer;

		ret =HI_GO_ShowLayer(hLayer, HI_TRUE);
		if ((HI_SUCCESS != ret)
		        && (HIGO_ERR_ALREADYSHOW != ret)
		        && (HIGO_ERR_ALREADYHIDE != ret))
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
		}


		struRectSize.x = 0;
		struRectSize.y = 0;
		struRectSize.w = HI_OSD_MAXWIDTH;
		struRectSize.h = HI_OSD_MAXHEIGHT;
//    ret = HI_GO_FillRect(hLayerSurface, &struRectSize, 0x00000000, HIGO_COMPOPT_NONE);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
		}

		ret = HI_GO_RefreshLayer(hLayer);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
		}

		//ret = HI_GO_RefreshLayer(hLayer);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
		}


#if (UI_OSD_BPP ==1)
		u32DispPhy = HI_MMB_New(720*576, 4, "others", HI_NULL);
		u32DispVir = HI_MMB_Map(u32DispPhy,HI_TRUE);
		memset(u32DispVir,0,720*576);
#endif

		printf(" === OSDGAL_OpenOSDLayer ok === \n");

		return HI_SUCCESS;
	}

	HI_S32 OSDGAL_GetOSDLayerHandle(IN HI_OSDGAL_HANDLE hHandle, HI_HANDLE *hLayer)
	{
		HI_GuiContent_S * pstruGuiContent = NULL;
		CHECK_OSDHANDLE(hHandle);
		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		*hLayer = pstruGuiContent->u32hLayer;
		return HI_SUCCESS;
	}

	HI_S32 OSDGAL_GetSurface(IN HI_OSDGAL_HANDLE hHandle,OUT HI_HANDLE * SurfaceBuff)
	{
		HI_GuiContent_S* pstruGuiContent = NULL;
		CHECK_PTR(SurfaceBuff);
		CHECK_OSDHANDLE(hHandle);
		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		*SurfaceBuff =  pstruGuiContent->u32hLayerSurface;
		return HI_SUCCESS;
	}


	HI_S32 OSDGAL_GetOSDBuf(IN HI_OSDGAL_LAYER_E eOSDLayer,
	                        OUT HI_U32 *pBuf)
	{
		HI_GuiContent_S * pstruGuiContent = NULL;
		CHECK_OSDHANDLE(eOSDLayer);
		pstruGuiContent = &g_astruGuiContent[eOSDLayer];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		*pBuf = pstruGuiContent->pu8Screen;

	}
	/* 关闭OSD图层设备 */
	HI_S32 OSDGAL_CloseOSDLayer(IN HI_OSDGAL_HANDLE hHandle)
	{
		HI_S32 ret = 0;
		HI_GuiContent_S * pstruGuiContent = NULL;
		HI_HANDLE hLayer;

		CHECK_OSDHANDLE(hHandle);
		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		hLayer = pstruGuiContent->u32hLayer;
		ret = HI_GO_DestroyLayer(hLayer);
		if (ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			return HI_FAILURE;
		}
		pstruGuiContent->u32hLayer          = -1;
		pstruGuiContent->u32hLayerSurface          = -1;

		pstruGuiContent->u32PhyAddr     = 0;
		pstruGuiContent->pu8Screen      = NULL;
		pstruGuiContent->bValied        = HI_FALSE;
#if (UI_OSD_BPP == 1)
		HI_MMB_Unmap(u32DispPhy);
		HI_MMB_Delete(u32DispPhy);
#endif
		return HI_SUCCESS;
	}

	HI_S32 OSDGAL_GetSurfaceAddr(IN HI_OSDGAL_HANDLE hHandle,OUT HI_U32 * SurfaceBuffAddr)
	{
		HI_GuiContent_S* pstruGuiContent = NULL;
		CHECK_PTR(SurfaceBuffAddr);
		CHECK_OSDHANDLE(hHandle);
		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		*SurfaceBuffAddr =  pstruGuiContent->pu8Screen;
		return HI_SUCCESS;
	}


	/* 使能OSD图层 */
	HI_S32 OSDGAL_Enable(IN HI_OSDGAL_HANDLE hHandle,
	                     IN HI_OSDGAL_ENABLE_E eEnable)
	{
		HI_S32 ret = 0;
		HI_HANDLE hLayer;
		HI_GuiContent_S* pstruGuiContent = NULL;
		CHECK_BOOL(eEnable);
		CHECK_OSDHANDLE(hHandle);

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		hLayer = pstruGuiContent->u32hLayer;

		ret = HI_GO_ShowLayer(hLayer, (HI_BOOL)eEnable);
		if ((HI_SUCCESS == ret)
		        || (HIGO_ERR_ALREADYSHOW == ret)
		        || (HIGO_ERR_ALREADYHIDE == ret))
		{
			return HI_SUCCESS;
		}
		else
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			return HI_FAILURE;
		}

	}

	/* 使能OSD图层 */
	HI_S32 OSDGAL_SetAlpha(IN HI_OSDGAL_HANDLE hHandle,  HI_U8 u8value)
	{
		HI_S32 ret = 0;
		HI_HANDLE hLayer;
		HI_GuiContent_S* pstruGuiContent = NULL;
		CHECK_OSDHANDLE(hHandle);

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		printf("\n +++ u8value =%d\n", u8value);

		hLayer = pstruGuiContent->u32hLayer;

		if (HI_SUCCESS != HI_GO_SetLayerAlpha(hLayer,u8value))
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			return HI_FAILURE;
		}

		return HI_SUCCESS;
	}


	/* OSD去初始化函数 */
	HI_S32 OSDGAL_DeInit()
	{
		HI_S32 ret = 0;
		HI_S32 i = 0;
		for (i = OSDGAL_LAYER1; i < OSDGAL_LAYER_BUTT; i++)
		{
			if (g_astruGuiContent[i].bValied == HI_TRUE && g_astruGuiContent[i].u32hLayer != -1)
			{
				OSDGAL_CloseOSDLayer(i);
			}
		}

		ret = HI_GO_Deinit();
		if (HI_SUCCESS != ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ret);
			return HI_FAILURE;
		}
		return HI_SUCCESS;
	}


	/*cwd  add 2009-07-31 画点*/
	HI_S32 OSDGAL_DrawPixel(IN HI_OSDGAL_HANDLE hHandle,
	                        IN HI_U32 u32X,
	                        IN HI_U32 u32Y,
	                        IN HI_U32 u32Color)
	{
		HI_S32 ret = 0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PIX(u32X, u32Y);

		pstruGuiContent = &g_astruGuiContent[hHandle];

		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		CHECK_PTR(pstruGuiContent->pu8Screen);

		draw_pixel(pstruGuiContent, u32X, u32Y, u32Color);

		return HI_SUCCESS;


	}

	/* 绘制矩形 */
	HI_S32 OSDGAL_DrawRectangle(IN HI_OSDGAL_HANDLE hHandle,
	                            IN const HI_OSDGAL_RECT_S *pstruRectSize,
	                            IN HI_U32 u32Color)
	{
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pstruRectSize);
		CHECK_RECT(pstruRectSize);

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		CHECK_PTR(pstruGuiContent->pu8Screen);
		/*画上水平线*/
		draw_line(pstruGuiContent,
		          pstruRectSize->u32x,
		          pstruRectSize->u32y,
		          pstruRectSize->u32x + pstruRectSize->u32Width,
		          pstruRectSize->u32y,
		          u32Color);
		/*画下水平线*/
		draw_line(pstruGuiContent,
		          pstruRectSize->u32x,
		          pstruRectSize->u32y + pstruRectSize->u32Height,
		          pstruRectSize->u32x + pstruRectSize->u32Width,
		          pstruRectSize->u32y + pstruRectSize->u32Height,
		          u32Color);
		/*画左竖线*/
		draw_line(pstruGuiContent,
		          pstruRectSize->u32x,
		          pstruRectSize->u32y,
		          pstruRectSize->u32x,
		          pstruRectSize->u32y + pstruRectSize->u32Height,
		          u32Color);
		/*画右竖线*/
		draw_line(pstruGuiContent,
		          pstruRectSize->u32x + pstruRectSize->u32Width,
		          pstruRectSize->u32y,
		          pstruRectSize->u32x + pstruRectSize->u32Width,
		          pstruRectSize->u32y + pstruRectSize->u32Height,
		          u32Color);

//   OSDGAL_Refresh(hHandle,pstruRectSize);

		return HI_SUCCESS;
	}

	/* 绘制椭圆 */
	HI_S32 OSDGAL_DrawEllipse(IN HI_OSDGAL_HANDLE hHandle,
	                          IN const HI_OSDGAL_RECT_S *pstruEllipseSize,
	                          IN HI_U32 u32Color)
	{
		HI_S32 ret = 0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pstruEllipseSize);
		CHECK_RECT(pstruEllipseSize);

		pstruGuiContent = &g_astruGuiContent[hHandle];

		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		CHECK_PTR(pstruGuiContent->pu8Screen);


		draw_ellopse(pstruGuiContent,
		             pstruEllipseSize->u32x + (pstruEllipseSize->u32Width  >> 1),
		             pstruEllipseSize->u32y + (pstruEllipseSize->u32Height >> 1),
		             pstruEllipseSize->u32Width  >> 1,
		             pstruEllipseSize->u32Height >> 1,
		             u32Color);

//    OSDGAL_Refresh(hHandle,pstruEllipseSize);

		return (HI_SUCCESS);
	}

	/* 绘制线 */
	HI_S32 OSDGAL_DrawLine(IN HI_OSDGAL_HANDLE hHandle,
	                       IN HI_U32 u32StartX,
	                       IN HI_U32 u32StartY,
	                       IN HI_U32 u32EndX,
	                       IN HI_U32 u32EndY,
	                       IN HI_U32 u32Color)
	{
		HI_GuiContent_S* pstruGuiContent = NULL;
		HI_OSDGAL_RECT_S struRect;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PIX(u32StartX, u32StartY);
		CHECK_PIX(u32EndX, u32EndY);

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		CHECK_PTR(pstruGuiContent->pu8Screen);
		/*画线*/
		draw_line(pstruGuiContent,
		          u32StartX,
		          u32StartY,
		          u32EndX,
		          u32EndY,
		          u32Color);

		struRect.u32x = u32StartX;
		struRect.u32y = u32StartY;
		struRect.u32Width = u32EndX - u32StartX;
		struRect.u32Height = u32EndY - u32StartY;

//    OSDGAL_Refresh(hHandle,&struRect);



		return HI_SUCCESS;
	}

	/* 填充椭圆 */
	HI_S32 OSDGAL_FillEllipse(IN HI_OSDGAL_HANDLE hHandle,
	                          IN const HI_OSDGAL_RECT_S* pstruEllipseSize,
	                          IN HI_U32 u32Color)
	{

		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pstruEllipseSize);
		CHECK_RECT(pstruEllipseSize);

		pstruGuiContent = &g_astruGuiContent[hHandle];

		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		CHECK_PTR(pstruGuiContent->pu8Screen);


		draw_ellopsefill(pstruGuiContent,
		                 pstruEllipseSize->u32x + (pstruEllipseSize->u32Width  >> 1),
		                 pstruEllipseSize->u32y + (pstruEllipseSize->u32Height >> 1),
		                 pstruEllipseSize->u32Width  >> 1,
		                 pstruEllipseSize->u32Height >> 1,
		                 u32Color);

//    OSDGAL_Refresh(hHandle,pstruEllipseSize);

		return(HI_SUCCESS);
	}

	/* 填充矩形 */
	HI_S32 OSDGAL_FillRectangle(IN HI_OSDGAL_HANDLE hHandle,
	                            IN const HI_OSDGAL_RECT_S* pstruRectSize,
	                            IN HI_U32 u32Color)
	{
		HI_S32 i = 0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pstruRectSize);

		CHECK_RECT(pstruRectSize);

		pstruGuiContent = &g_astruGuiContent[hHandle];

		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		CHECK_PTR(pstruGuiContent->pu8Screen);

		for (i = 0; i < pstruRectSize->u32Height ; i++)
		{
			fill_line(pstruGuiContent,
			          pstruRectSize->u32x,
			          pstruRectSize->u32y+i,
			          pstruRectSize->u32Width,
			          u32Color);
		}

//    OSDGAL_Refresh(hHandle,pstruRectSize);

		return HI_SUCCESS;
	}


	/* 读取指定矩形框内的数据 */
	HI_S32 OSDGAL_ReadRectangleData(IN HI_OSDGAL_HANDLE hHandle,
	                                IN const HI_OSDGAL_RECT_S *pstruRectSize,
	                                IN HI_U32 u32DataLen,
	                                IN OUT HI_U8 *pu8Data)
	{
		HI_S32 i = 0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pu8Data);
		CHECK_PTR(pstruRectSize);
		CHECK_RECT(pstruRectSize);

		if (u32DataLen < pstruRectSize->u32Height * pstruRectSize->u32Width)
		{
			return HI_ERR_OSDGAL_INVARG;
		}

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		CHECK_PTR(pstruGuiContent->pu8Screen);

		for (i = 0; i < pstruRectSize->u32Height; i++)
		{
			Get_line(pstruGuiContent,
			         pstruRectSize->u32x,
			         pstruRectSize->u32y+i,
			         pstruRectSize->u32Width,
			         pu8Data);
			pu8Data += (pstruRectSize->u32Width*UI_OSD_BPP);
		}

		return HI_SUCCESS;
	}

	/* 往指定矩形框内写数据 */
	HI_S32 OSDGAL_WriteRectangleData(IN HI_OSDGAL_HANDLE hHandle,
	                                 IN const HI_OSDGAL_RECT_S *pstruRectSize,
	                                 IN HI_U32 u32DataLen,
	                                 IN OUT HI_U8 *pu8Data)
	{
		HI_S32 i = 0,j=0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pu8Data);
		CHECK_PTR(pstruRectSize);



		CHECK_RECT(pstruRectSize);

		if (u32DataLen < pstruRectSize->u32Height * pstruRectSize->u32Width)
		{
			return HI_ERR_OSDGAL_INVARG;
		}

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		CHECK_PTR(pstruGuiContent->pu8Screen);

		for (i = 0; i < pstruRectSize->u32Height; i++)
		{

//    	HI_U8* pu8Buf = u32DispVir + (pstruRectSize->u32y+i) * HI_OSD_MAXWIDTH *UI_OSD_BPP+ pstruRectSize->u32x*UI_OSD_BPP;
//	memcpy((void*)pu8Buf, pu8Data, pstruRectSize->u32Width * UI_OSD_BPP);

			Set_line(pstruGuiContent,
			         pstruRectSize->u32x,
			         pstruRectSize->u32y+i,
			         pstruRectSize->u32Width,
			         pu8Data);

			pu8Data += (pstruRectSize->u32Width*UI_OSD_BPP);
		}

//   OSDGAL_Refresh(hHandle,pstruRectSize);

		return HI_SUCCESS;
	}


	/* 往指定矩形框内写数据 cwd 2009-07-31 */
	HI_S32 OSDGAL_WriteRectangleDataMask(IN HI_OSDGAL_HANDLE hHandle,
	                                     IN const HI_OSDGAL_RECT_S *pstruRectSize,
	                                     IN HI_U32 u32DataLen,
	                                     IN OUT HI_U8 *pu8Data,
	                                     IN HI_U32 maskcolor)
	{
		HI_S32 i = 0,j=0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pu8Data);
		CHECK_PTR(pstruRectSize);



		CHECK_RECT(pstruRectSize);

		if (u32DataLen < pstruRectSize->u32Height * pstruRectSize->u32Width)
		{
			return HI_ERR_OSDGAL_INVARG;
		}

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		CHECK_PTR(pstruGuiContent->pu8Screen);

		for (i = 0; i < pstruRectSize->u32Height; i++)
		{

//    	HI_U8* pu8Buf = u32DispVir + (pstruRectSize->u32y+i) * HI_OSD_MAXWIDTH *UI_OSD_BPP+ pstruRectSize->u32x*UI_OSD_BPP;
//	memcpy((void*)pu8Buf, pu8Data, pstruRectSize->u32Width * UI_OSD_BPP);

			Set_linemask(pstruGuiContent,
			             pstruRectSize->u32x,
			             pstruRectSize->u32y+i,
			             pstruRectSize->u32Width,
			             pu8Data, maskcolor);

			pu8Data += (pstruRectSize->u32Width*UI_OSD_BPP);
		}

//  OSDGAL_Refresh(hHandle,pstruRectSize);

		return HI_SUCCESS;
	}

	/* 获取某个像素的值 */
	HI_S32 OSDGAL_GetPixel(IN HI_OSDGAL_HANDLE hHandle,
	                       IN HI_U32 u32x,
	                       IN HI_U32 u32y,
	                       IN OUT HI_U8 *pu8Data)
	{

		HI_GuiContent_S* pstruGuiContent = NULL;
		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pu8Data);

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);
		CHECK_PIX(u32x, u32y);
		CHECK_PTR(pstruGuiContent->pu8Screen);
		memcpy(pu8Data,
		       pstruGuiContent->pu8Screen+u32x * UI_OSD_BPP + u32y * HI_OSD_MAXWIDTH*UI_OSD_BPP,
		       UI_OSD_BPP);
		return HI_SUCCESS;
	}
#if (UI_OSD_BPP == 1)
	/* 设置调色板 */
	HI_S32 OSDGAL_SetColorCult(IN HI_OSDGAL_HANDLE hHandle,
	                           IN HI_U32  u32CultCount,
	                           IN HI_U32  u32Start,
	                           IN HI_U16* pu16Red,
	                           IN HI_U16* pu16Green,
	                           IN HI_U16* pu16Blue,
	                           IN HI_U16* pu16alph)
	{
		HI_GuiContent_S* pstruGuiContent = NULL;
		HI_U32 uRed   = 0;
		HI_U32 uGreen = 0;
		HI_U32 uBlue  = 0;
		HI_U32 ualph  = 0;
		HI_U32 u16Color = 0;
		HI_U32 u32Color = 0;

		HI_S32 i = 0;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pu16Red);
		CHECK_PTR(pu16Green);
		CHECK_PTR(pu16Blue);
		CHECK_PTR(pu16alph);

		if ( 0   == u32CultCount
		        || HI_OSD_MAXCOLORCULT <  u32CultCount
		        || HI_OSD_MAXCOLORCULT <= u32Start
		        || HI_OSD_MAXCOLORCULT <  (u32Start + u32CultCount))
		{
			return HI_ERR_OSDGAL_INVARG;
		}

		if (OSDGAL_LAYER1 != hHandle)
		{
			return HI_ERR_OSDGAL_NOTSUPPORT;
		}

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		for (i = 0; i < u32CultCount; i++)
		{

#if (HI_OSD_BPP ==2)
			ualph  = (HI_U32)( pu16alph[i]&0xff);
			uBlue   = (HI_U32)(pu16Blue[i]&0xf8)>>3;
			uGreen = (HI_U32)(pu16Green[i]&0xf8)<<2;
			uRed  = (HI_U32)(pu16Red[i]&0xf8)<<7;
			u16Color =  (HI_U16)uBlue + (HI_U16)uGreen + (HI_U16)uRed;
			if (ualph == 0)
				u16Color = u16Color&0x8000;
			uClut[u32Start+i] = u16Color;
#else
			ualph  = (HI_U32)( pu16alph[i]&0xff)<<24;
			uBlue   = (HI_U32)(pu16Blue[i]&0xff);
			uGreen = (HI_U32)(pu16Green[i]&0xff)<<8;
			uRed  = (HI_U32)(pu16Red[i]&0xff)<<16;
			u32Color =  uBlue + uGreen + uRed + ualph;
			uClut[u32Start+i] = u32Color;
#endif
		}
		return 0;

		//return ioctl(pstruOsdContent->u32Fd, FBIOPUTCMAP, &cmap);

	}

	/* 设置调色板扩展接口 */
	HI_S32 OSDGAL_SetColorCultEx(IN HI_OSDGAL_HANDLE hHandle,
	                             IN HI_U32  u32CultCount,
	                             IN HI_U32  u32Start,
	                             IN HI_U32* pColor)
	{
		HI_S32 i = 0;
		HI_S32 s32Ret = 0;
		HI_GuiContent_S* pstruGuiContent = NULL;

		HI_U32 uRed   = 0;
		HI_U32 uGreen = 0;
		HI_U32 uBlue  = 0;
		HI_U32 ualph  = 0;
		HI_U32 u16Color = 0;
		HI_U32 u32Color = 0;

		CHECK_OSDHANDLE(hHandle);
		CHECK_PTR(pColor);

		if ( 0   == u32CultCount
		        || HI_OSD_MAXCOLORCULT <  u32CultCount
		        || HI_OSD_MAXCOLORCULT <= u32Start
		        || HI_OSD_MAXCOLORCULT <  (u32Start + u32CultCount))
		{
			return HI_ERR_OSDGAL_INVARG;
		}

		if (OSDGAL_LAYER1 != hHandle)
		{
			return HI_ERR_OSDGAL_NOTSUPPORT;
		}

		pstruGuiContent = &g_astruGuiContent[hHandle];
		ASSERT_OPENED(pstruGuiContent->bValied,pstruGuiContent->u32hLayer);

		for (i = 0; i < u32CultCount; i++)
		{
#if (HI_OSD_BPP ==2)
			ualph  = (HI_U32)( pColor[i]&0xff000000);
			uBlue   = (HI_U32)(pColor[i]&0xf80000)>>19;
			uGreen = (HI_U32)(pColor[i]&0xf800)>>6;
			uRed  = (HI_U32)(pColor[i]&0xf8)<<7;
			u16Color =  (HI_U16)uBlue + (HI_U16)uGreen + (HI_U16)uRed;
			if (ualph == 0)
				u16Color = u16Color&0x8000;
			uClut[u32Start+i] = u16Color;
#else
			ualph  = (HI_U32)( pColor[i]&0xff000000);
			uBlue   = (HI_U32)(pColor[i]&0xff0000)>>16;
			uGreen = (HI_U32)(pColor[i]&0xff00);
			uRed  = (HI_U32)(pColor[i]&0xff)<<16;
			u32Color =  uBlue + uGreen + uRed + ualph;
			uClut[u32Start+i] = u32Color;
#endif
		}
		printf(" === OSDGAL_SetColorCultEx ok === \n");
		return s32Ret;
	}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif



