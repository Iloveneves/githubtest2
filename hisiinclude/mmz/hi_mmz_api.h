

#ifndef __HI_MMZ_API_H
#define __HI_MMZ_API_H

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


HI_VOID *HI_MMB_New(HI_U32 size , HI_U32 align, HI_U8 *mmz_name, HI_U8 *mmb_name);
HI_VOID *HI_MMB_Map(HI_U32 phys_addr, HI_U32 cached);
HI_S32 HI_MMB_Unmap(HI_U32 phys_addr);
HI_S32 HI_MMB_Delete(HI_U32 phys_addr);
HI_S32 HI_MMB_Flush();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_MMZ_API_H*/
