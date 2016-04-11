/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the Tuner Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
   
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_TUNER_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_TUNER_API_FUNCTOTYPE_H_

#if defined(IPANEL_API_VERSION)

#ifdef __cplusplus
extern "C" {
#endif

int ipanel_porting_get_delSignalQuality(void);
int ipanel_porting_get_delSignalStrength(void);
int ipanel_porting_dvb_select_av(int pcr_pid,   int emm_pid, int video_pid,  int audio_pid, int audio_ecm_pid, int video_ecm_pid);
int ipanel_porting_delivery_tune(int frequency, int symbol_rate, int modulation);

typedef struct tagDVBServicePlay DVBServicePlay;

int ipanel_porting_service_open(DVBServicePlay *serviceplay);
unsigned short ipanel_porting_get_startup_serviceid(void);
int ipanel_porting_select_service(int network_id, int ts_id, int service_id);

#define MAX_TEXT_LENGTH     64
#define MAX_NUM_PID             8
#define MAX_NUM_DSMCC        8
#define MAX_NUM_APP             8

typedef struct {
	unsigned short PID;
	unsigned short type;
	unsigned short tag; // component_tag
	unsigned int carouselId;
	/* maybe extended */
} DSMCCStreamInfo;

typedef struct {
	unsigned short ON_id, TS_id, service_id; // 和service的相同, 可以不存以省空间
	unsigned char component_tag;
	unsigned int version;
	/* maybe extended */
	char name[MAX_TEXT_LENGTH];
	char path[MAX_TEXT_LENGTH];
} APPInfo;

struct tagDVBServicePlay
{
    unsigned int frequency;
    unsigned int qam;
    unsigned int symbolrate;
    unsigned int original_network_id;
    unsigned int transport_stream_id;
    //
    unsigned int servicetype; /* 1 - AV; 2- AudioOnly; 3- DataService  */
    unsigned int serviceid;
    //
    int num_video_pids;
    unsigned int video_pids[MAX_NUM_PID];
    int num_audio_pids;
    unsigned int audio_pids[MAX_NUM_PID];
    unsigned int pcr_pid;
    //
    unsigned int audio_mode;
    unsigned int audio_channel;
    unsigned int volume_ref;
    unsigned int volume;
    //
    unsigned int ait_pid; /* AIT PID(if exists) */
    //
    int num_apps; // 如果service.applications.length>0, 设置这里; 否则, 为0
    APPInfo apps[MAX_NUM_APP]; // 每个application的参数设置在这里
    //
    int num_dsmcc_streams; // 如果service.dsmccPIDs.length>0, 设置这里; 否则, 为0
    DSMCCStreamInfo dsmcc_streams[MAX_NUM_DSMCC];
    //
    unsigned int flag_pf; /* service P/F EIT flag */
    unsigned int flag_schedule; /* service schedule EIT flag */
    unsigned int running_status; /* service running status */
    };

#undef MAX_TEXT_LENGTH

#ifdef __cplusplus
}
#endif

#endif // defined(IPANEL_API_VERSION)
#include "hi_type.h"
#include "hi_ecs_type.h"

#endif
