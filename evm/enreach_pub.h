/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_pub.h
* Description:
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#ifndef __ENREACH_PUB_H__
#define __ENREACH_PUB_H__

#include "hi_type.h"
#include "enreach_erro.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 0
/* N制屏幕大小 */

#define UI_BASEVIDEO_WND_NTSC_X    (0)
#define UI_BASEVIDEO_WND_NTSC_Y    (0)
#define UI_BASEVIDEO_WND_NTSC_W    (720)
#define UI_BASEVIDEO_WND_NTSC_H    (480)

/* PAL制屏幕大小 */

#define UI_BASEVIDEO_WND_PAL_X     (0)
#define UI_BASEVIDEO_WND_PAL_Y     (0)
#define UI_BASEVIDEO_WND_PAL_W     (720)
#define UI_BASEVIDEO_WND_PAL_H     (576)
#endif

#define INVALID_HANDLE             (0xFFFFFFFF)
#define BROWSER_CACHE_SIZE         (1*1024*1024)
#define BROWSER_BUFF_SIZE          (4*1024*1024)

#define JS_BUFF_SIZE 			(1*1024*1024) //add by jlin for the js cache 

#define OC_INIT_SIZE               (7*1024*1024)

#define OC_OPEN_WAITE_TIME         (4000)

#define BORWSER_EXIT_WAIT_TIME     (200 * 1000)


//#define ENREACH_OSD_WIDTH          (720)   /* 枣庄数据广播配置的宽度 */
//#define ENREACH_OSD_HEIGHT         (576)   /* 枣庄数据广播配置的高度 */

#define ENREACH_OSD_WIDTH          (640)   /* 枣庄数据广播配置的宽度 */
#define ENREACH_OSD_HEIGHT         (526)   /* 枣庄数据广播配置的高度 */

#if 0
#define ENREACH_RETURN(val, ret )    \
do                                   \
{                                    \
    if ((val))                       \
    {                                \
        ENREACH_ERROR("### fail ###\n");\
        return ret;                  \
    }                                \
} while (0)
#else
#define ENREACH_RETURN(val, ret )    \
do                                   \
{                                    \
    if ((val))                       \
    {                                \
        EVM_ERROR("fail");\
        return ret;                  \
    }                                \
} while (0)
#endif


#define ENREACH_SAFE_FREE(ptr)       \
do                                   \
{                                    \
    if (NULL != (ptr))               \
    {                                \
        free((ptr));                 \
        (ptr) = NULL;                \
    }                                \
} while (0)



#ifndef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ENREACH_OS_H__ */



