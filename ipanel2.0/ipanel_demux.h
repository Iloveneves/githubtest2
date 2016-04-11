/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the Demux Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef __HI_IPANEL_DEMUX_H__
#define __HI_IPANEL_DEMUX_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NOTIFY_FUNC)(BU32 ch, BU32 filters, BU32 DataStartAddr,BU32 len);

int ipanel_porting_set_demux_notify(NOTIFY_FUNC func);
BU32 ipanel_porting_create_demux_channel(BS32 poolsize, BU16 pid);
BU32 ipanel_porting_create_demux_filter(BU32 physical_channel, BU32 wide,
                                            BU08 coef[], BU08 mask[], BU08 excl[]);
int ipanel_porting_start_demux_channel(BU32 channel);
int ipanel_porting_stop_demux_channel(BU32 channel);
int ipanel_porting_disable_demux_filter(BU32 filter);
int ipanel_porting_enable_demux_filter(BU32 filter);
int ipanel_porting_destroy_demux_filter(BU32 filter);
int ipanel_porting_destroy_demux_channel(BU32 channel);

#ifdef __cplusplus
}
#endif

#endif
