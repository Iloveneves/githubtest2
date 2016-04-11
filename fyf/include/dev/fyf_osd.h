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
    FYF_OSD_PF_CLUT8 = 0,  /**< ��ɫ�� */
    FYF_OSD_PF_CLUT1,
    FYF_OSD_PF_CLUT4,
    FYF_OSD_PF_4444,       /**< һ������ռ16bit, ARGBÿ����ռ4bit, ����ַ�ɸ��������� */
    FYF_OSD_PF_0444,       /**< һ������ռ16bit, ARGBÿ����ռ4bit, ����ַ�ɸ���������, A������������ */

    FYF_OSD_PF_1555,       /**< һ������ռ16bit, RGBÿ����ռ5bit, A����1bit, ����ַ�ɸ��������� */
    FYF_OSD_PF_0555,       /**< һ������ռ16bit, RGBÿ����ռ5bit, A����1bit, ����ַ�ɸ���������, A������������ */
    FYF_OSD_PF_565,        /**< һ������ռ16bit, RGBÿ�����ֱ�ռ5bit,6bit,5bit, ����ַ�ɸ��������� */
    FYF_OSD_PF_8888,       /**< һ������ռ32bit, ARGBÿ����ռ8bit, ����ַ�ɸ��������� */
    FYF_OSD_PF_0888,       /**< һ������ռ32bit, ARGBÿ����ռ8bit, ����ַ�ɸ���������, A������������ */

    FYF_OSD_PF_YUV420,     /**< ��˼�����semi-planar YUV 420��ʽ */
    FYF_OSD_PF_YUV422,     /**< ��˼�����semi-planar YUV 422��ʽ */
    FYF_OSD_PF_BUTT
} FYF_OSD_PF_E;


BS32 FYF_API_OSD_open();
BS32 FYF_API_OSD_close();

//HI_OSDGAL_HANDLE FYF_API_OSD_GetHandle();

/*������*/
BS32 FYF_API_OSD_DrawPixel(BU32 x, BU32 y, BU32 c);

/* ��ȡĳ�����ص�ֵ */
BS32 FYF_API_OSD_ReadPixel(BU32 x, BU32 y, BU08 *data);
/* ������ */	
BS32 FYF_API_OSD_DrawLine(BU32 x1, BU32 y1, BU32 x2, BU32 y2, BU32 c);


/* ������ */
BS32 FYF_API_OSD_FillRect(BU32 x, BU32 y, BU32 w, BU32 h, BU32 c);
/* ��ָ�����ο���д���� */
BS32 FYF_API_OSD_WriteRectangleData(BU32 x, BU32 y, BU32 w, BU32 h, void *buf);


/* ��ȡָ�����ο��ڵ����� */

BS32 FYF_API_OSD_ReadRectangleData(BU32 x, BU32 y, BU32 w, BU32 h, void* buf);


BS32 FYF_API_OSD_WriteRectangleDataMask (BU32 x, BU32 y, BU32 w, BU32 h, void *buf, BU32 maskcolor);

BS32 FYF_API_OSD_Refresh(BU32 x, BU32 y, BU32 w, BU32 h);


#endif



