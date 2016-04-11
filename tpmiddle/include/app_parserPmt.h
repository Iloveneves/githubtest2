#ifndef _APP_PARSERPMT_H_
#define _APP_PARSERPMT_H_


#define	SIE_MPEG1_VIDEO_STREAM	 0x01
#define	SIE_MPEG2_VIDEO_STREAM	 0x02
#define	SIE_MPEG1_AUDIO_STREAM	 0x03
#define	SIE_MPEG2_AUDIO_STREAM	 0x04

#define SIE_BROADCAST_AUDIO_STREAM 0x0b
#define SIE_MPEG4_VIDEO_STREAM	 0x10
#define SIE_H264_VIDEO_STREAM    0x1b
#define SIE_AC3_AUDIO_STREAM	 0x81
#define SIE_AAC_ADTS_AUDIO_STREAM  0x0F 
#define SIE_AAC_LATM_AUDIO_STREAM  0x11


/**************************************************************
分析PMT表的channel信息
**************************************************************/
typedef struct _PMT_Channel_s
{
	BU32 pmtPID;
	BU32 pmtChannelID;
	BU32 pmtFilterID;
	BU32 isStop;
}PMT_Channel_s;

/*******************************************
Desic:    设置和获取nvod时移业务数
********************************************/
BU32 APP_PMT_API_GetSearchNvodNum(void);
void APP_PMT_API_SetSearchNvodNum(BU32 num);



#endif







