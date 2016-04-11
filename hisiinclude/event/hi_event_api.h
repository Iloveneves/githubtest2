#ifndef __EVENT_API_H__
#define __EVENT_API_H__

#include "hi_type.h"
#include "hi_event_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


HI_S32 HI_EVENT_Open(HI_VOID);
HI_S32 HI_EVENT_Close(HI_VOID);

HI_S32 HI_EVENT_Register(EVENT_TYPE_E event);
HI_S32 HI_EVENT_UnRegister(EVENT_TYPE_E event);

HI_S32 HI_EVENT_Wait(EVENT_TYPE_E care, HI_VOID *event, HI_U32 timeout);
HI_S32 HI_EVENT_Cancel(HI_VOID);
HI_S32 HI_EVENT_Get_Info(EVENT_TYPE_E care, HI_VOID *info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EVENT_API_H__ */


