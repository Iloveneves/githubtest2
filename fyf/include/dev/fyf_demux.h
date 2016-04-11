/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/09/18                                           	  */
/******************************************************************************/ 
#ifndef __FYF_DEMUX_H__
#define __FYF_DEMUX_H__

#define FYF_DEMUX_EXTEND_OP_TAG 0x1A2B3C4D //For ali only

typedef void (*FYF_DEMUX_NOTIFY_FUNC)(BU32 ch, BU32 filter, BU08 *buf, BS32 len); 
/* demux channel types */
typedef enum {
	FYF_DEMUX_DATA_PSI_CHANNEL = 0,
	FYF_DEMUX_DATA_PES_CHANNEL,
	FYF_DEMUX_VIDEO_CHANNEL,
	FYF_DEMUX_AUDIO_CHANNEL,
	FYF_DEMUX_PCR_CHANNEL
} FYF_DEMUX_CHANNEL_TYPE_e; 

typedef struct
{
	BS32 (*FYF_CPI_demux_set_notify)(FYF_DEMUX_NOTIFY_FUNC func);
	BS32 (*FYF_CPI_demux_create_channel)(BS32 poolsize, FYF_DEMUX_CHANNEL_TYPE_e type, BU32 *pChannelid);
	BS32 (*FYF_CPI_demux_set_channel_pid)(BU32 channel, BU16 pid);
	BS32 (*FYF_CPI_demux_destroy_channel)(BU32 channel);
	BS32 (*FYF_CPI_demux_start_channel)(BU32 channel);
	BS32 (*FYF_CPI_demux_stop_channel)(BU32 channel); 
	BS32 (*FYF_CPI_demux_create_filter)(BU32 channel, BU32 *pFilterid);
	BS32 (*FYF_CPI_demux_set_filter)(BU32 channel, BU32 filter, BU32 wide, BU08 coef[], BU08 mask[], BU08 excl[]);
	BS32 (*FYF_CPI_demux_destroy_filter)(BU32 channel, BU32 filter);
	BS32 (*FYF_CPI_demux_enable_filter)(BU32 channel, BU32 filter);
	BS32 (*FYF_CPI_demux_disable_filter)(BU32 channel, BU32 filter);
}FYF_DEMUX_PORTING;


/******************************************************************************/
/*Description: register demux                                                 */
/*Input      : porting														  */
/*Output     : no                                                             */
/*Return     : no					                                          */
/******************************************************************************/
void FYF_API_demux_register(FYF_DEMUX_PORTING *porting);
/******************************************************************************/
/*Description: set notify func                                                */
/*Input      : func														      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_set_notify(FYF_DEMUX_NOTIFY_FUNC func);
BS32 FYF_API_demux_set_ipanel_notify(BU32 channelid, FYF_DEMUX_NOTIFY_FUNC func);
/******************************************************************************/
/*Description: create channel                                                 */
/*Input      : poolsize,type    										      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_create_channel(BS32 poolsize, FYF_DEMUX_CHANNEL_TYPE_e type, BU32 *channel_id, BU16 pid);
/******************************************************************************/
/*Description: set channel pid                                                */
/*Input      : channel,pid       										      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_set_channel_pid(BU32 channel, BU16 pid);
/******************************************************************************/
/*Description: set channel pid                                                */
/*Input      : channel,pid       										      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_destroy_channel(BU32 channel);
/******************************************************************************/
/*Description: start channel                                 	              */
/*Input      : channel       											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_start_channel(BU32 channel);
/******************************************************************************/
/*Description: stop channel                                 	              */
/*Input      : channel       											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_stop_channel(BU32 channel);
/******************************************************************************/
/*Description: create filter                                 	              */
/*Input      : channel       											      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_create_filter(BU32 channel, BU32 *pFilterid);
/******************************************************************************/
/*Description: set filter                                   	              */
/*Input      : channel,filter,wide,coef,mask,excl						      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_set_filter(BU32 channel, BU32 filter, BU32 wide, BU08 coef[], BU08 mask[], BU08 excl[]);
/******************************************************************************/
/*Description: destroy filter                                  	              */
/*Input      : channel,filter											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_destroy_filter(BU32 channel, BU32 filter);
/******************************************************************************/
/*Description: enable filter                                  	              */
/*Input      : channel,filter											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/  
BS32 FYF_API_demux_enable_filter(BU32 channel, BU32 filter);
/******************************************************************************/
/*Description: disable filter                                  	              */
/*Input      : channel,filter											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_disable_filter(BU32 channel, BU32 filter);
#endif






