/**********************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
***********************************************************************
* FileName: enreach_base.c
* Description: 该文件实现浏览器定义的基本接口，浏览器自己调用的接口
*
* History:
* Version   Date              Author       DefectNum      Description
***********************************************************************/
 
#ifndef __ENREACH_BASE_H__
#define __ENREACH_BASE_H__

#include "hi_type.h"
#include "ebrowser_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

int EnReach_GetTSID(int iFre, int iSym, int iQam)
{
    return 0;
}

char* EnReach_GetSTBID(void)
{
    return NULL;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


