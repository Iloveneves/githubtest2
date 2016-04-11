#ifndef __EVENT_TYPE_H__
#define __EVENT_TYPE_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum hiEVENT_TYPE_E
{
    CHECK_ENCODER_FORMAT = 0x1,
    CHECK_VID_STREAM = 0x2,
    CHECK_VID_DECODER = 0x4,
    CHECK_VID_DISPLAY = 0x8,
    CHECK_NEW_SEQ = 0x10,
    CHECK_NEW_VID_FRAME = 0x20,
    CHECK_NEW_AUD_FRAME = 0x40,
    CHECK_NEW_USER_DATA = 0x80,
    CHECK_VID_STOP = 0x100,
    CHECK_AUD_STOP = 0x200,
    CHECK_VID_BUF_STATE = 0x400,
    CHECK_AUD_BUF_STATE = 0x800,
    CHECK_EVENT_CANCEL = 0x80000000,
}EVENT_TYPE_E;

typedef enum hiEVENT_FORMAT_CHANGE_E
{
    EVENT_FORMAT_PAL = 0,
    EVENT_FORMAT_NTSC,
    EVENT_FORMAT_BUTT
}EVENT_FORMAT_CHANGE_E;

typedef enum hiEVENT_STREAM_E
{
    EVENT_STREAM_STOP = 0,
    EVENT_STREAM_START,
    EVENT_STREAM_BUTT
}EVENT_STREAM_E;

typedef enum hiEVENT_VID_DECODER_E
{
    EVENT_VID_DECODER_RUNNING = 0,
    EVENT_VID_DECODER_STOPPED,
    EVENT_VID_DECODER_BUTT
}EVENT_VID_DECODER_E;

typedef enum hiEVENT_VID_DISPLAY_E
{
    EVENT_VID_DISPLAY_NEW_FRAME = 0,
    EVENT_VID_DISPLAY_NO_FRAME,
    EVENT_VID_DISPLAY_BUTT
}EVENT_VID_DISPLAY_E;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EVENT_TYPE_H__ */

