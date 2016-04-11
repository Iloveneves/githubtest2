/*Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_tde_ioctl.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006/7/10
Last Modified :
Description   : 
Function List :
History       :
 ******************************************************************************/

#ifndef __HI_TDE_IOCTL_H__
#define __HI_TDE_IOCTL_H__

#include <linux/ioctl.h>
#include "hi_tde_type.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* Use 't' as magic number */
#define TDE_IOC_MAGIC 't'

#define TDE_BEGIN_JOB _IOW(TDE_IOC_MAGIC, 1, TDE_HANDLE)
#define TDE_BIT_BLIT _IOW(TDE_IOC_MAGIC, 2, TDE_BITBLIT_CMD_S)
#define TDE_SOLID_DRAW _IOW(TDE_IOC_MAGIC, 3, TDE_SOLIDDRAW_CMD_S)
#define TDE_QUICK_COPY _IOW(TDE_IOC_MAGIC, 4, TDE_QUICKCOPY_CMD_S)
#define TDE_QUICK_RESIZE _IOW(TDE_IOC_MAGIC, 5, TDE_QUICKRESIZE_CMD_S)
#define TDE_QUICK_FILL _IOW(TDE_IOC_MAGIC, 6, TDE_QUICKFILL_CMD_S)
#define TDE_QUICK_DEFLICKER _IOW(TDE_IOC_MAGIC, 7, TDE_QUICKDEFLICKER_CMD_S)
#define TDE_MB_BITBLT _IOW(TDE_IOC_MAGIC, 8, TDE_MBBITBLT_CMD_S)
#define TDE_END_JOB _IOW(TDE_IOC_MAGIC, 9, TDE_ENDJOB_CMD_S)
#define TDE_WAITFORDONE _IOW(TDE_IOC_MAGIC, 10, TDE_HANDLE)
#define TDE_CANCEL_JOB _IOW(TDE_IOC_MAGIC, 11, TDE_HANDLE)
#define TDE_BITMAP_MASKROP _IOW(TDE_IOC_MAGIC, 12, TDE_BITMAP_MASKROP_CMD_S)
#define TDE_BITMAP_MASKBLEND _IOW(TDE_IOC_MAGIC, 13, TDE_BITMAP_MASKBLEND_CMD_S)
#define TDE_WAITALLDONE _IO(TDE_IOC_MAGIC, 14)
#define TDE_RESET _IO(TDE_IOC_MAGIC, 15)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __HI_TDE_IOCTL_H__ */

