

/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hi_demux_errcode.h
* Description:  Define error code of demux
*
* History:
* Version      Date         Author     DefectNum    Description
* main\1    2006-11-01      l55251        NULL      Create this file.
***********************************************************************************/

#ifndef __HI_DEMUX_ERRCODE_H__
#define __HI_DEMUX_ERRCODE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/********************************************************
  DEMUX  error code definition
 *********************************************************/
#define MID_DEMUX                          (0x20) /*DEMUX  module ID*/

/* Sub Error Code for API and Driver */
#define ERR_DMX_INVALID_PID                (0x02) /*Input invalid pid */
#define ERR_DMX_NOTSET_PID                 (0x03) /*Pid has not been set*/
#define ERR_DMX_OCCUPIED_PID               (0x04) /*Pid has been set to another channel*/

#define ERR_DMX_INVALID_PORTTYPE           (0x10) /*Input invalid port type*/
#define ERR_DMX_OPENING_PORT               (0x11) /*Port type can not be modified when port is open*/
#define ERR_DMX_INVALID_RECVMODE           (0x12) /*Input invalid clock recovery mode*/
#define ERR_DMX_INVALID_PORTID            (0x13)

#define ERR_DMX_INVALID_CHNTYPE            (0x20) /*Input invalid channel type*/
#define ERR_DMX_INVALID_CHNID              (0x21) /*Input invalid channel id*/
#define ERR_DMX_NOFIND_CHNID               (0x22) /*Channel matched with input pid has not been found*/
#define ERR_DMX_NO_CHAN                    (0x23) /*No free channel to be allocated*/
#define ERR_DMX_NOT_NEWCHN                 (0x24) /*The channel has not been allocated*/
#define ERR_DMX_OPENING_CHN                (0x25) /*Input operation is invalid when channel is open*/
#define ERR_DMX_INVALID_BUFFERSIZE         (0x26) /*Input invalid buffer size, not SIZE1, SIZE2 or SIZE3*/


#define ERR_DMX_INVALID_FILTERDEPTH        (0x30) /*Input invalid filterdepth*/
#define ERR_DMX_INVALID_FILTERID           (0x31) /*Input invalid filter id*/
#define ERR_DMX_ATTACH_FILTER              (0x32) /*Filter parameter can not be modified when filter has been attatched to channel*/
#define ERR_DMX_NOATTACH_FILTER            (0x33) /*The filter can not be detached because it has not been attatched to any channel*/
#define ERR_DMX_NOMATCH_KEY                (0x34) /*The key can not be detached because it has not been attatched to any channel*/
#define ERR_DMX_NO_NEWFILTER               (0x36) /*Filter has not been allocated*/
#define ERR_DMX_NO_MATCHFILTER             (0x37) /*The filter has not been attatched to this channel*/
#define ERR_DMX_NO_FREEFILTER              (0x38) /*No free filter to be allocated*/

#define ERR_DMX_INVAID_KEYID               (0x40) /*Input invalid key id*/
#define ERR_DMX_ATTACH_KEY                 (0x41) /*The key can not be deleted because it is attatched to some channel*/
#define ERR_DMX_NOATTACH_KEY               (0x42) /*The key has not been attatched to any channel*/
#define ERR_DMX_NOT_NEWKEY                 (0x44) /*key has not been allocated*/
#define ERR_DMX_NOT_FREEKEY                (0x45) /*No free key to be allocated*/
#define ERR_DMX_MULTI_KEYSET               (0x46) /*The channel has been attatched key and can not be attatched again*/
#define ERR_DMX_NOFIND_KEYID               (0x47) /*The channel has not been attatched key*/

#define ERR_DMX_AUD_MEM_COPY               (0x50) /*Audio channel can not be opened when audio is in memory copying process*/
#define ERR_DMX_WAIT_TIMEOUT               (0x51) /*Wait timeout*/
#define ERR_DMX_VID_MEM_COPY               (0x52) /*Video channel can not be opened when video is in memory copying process*/

#define ERR_DMX_TS_POST_START              (0x53) /*Ts post treshold can not be set when ts post has been enabled */
#define ERR_DMX_INVALID_TSPOST_TRESH       (0x54) /*Input invalid ts post treshold */
#define ERR_DMX_INVALID_PACKET_CNTCTRL     (0x55) /*Input invalid packet count control command*/

#define ERR_DMX_TSPOSTSIZE_BEYOND_BUFSIZE  (0x56) /*The ts post treshold has beyonded buffersize, then data will be covered*/

#define ERR_DMX_NONEW_DATA_TO_READ         (0x57) /*No new data to be read out*/

#define ERR_DMX_INVALID_CHANOPTION         (0x58) /*invalid channel option*/

#define ERR_DMX_PTR_NULL                   (0x60) /*null pointer*/
#define ERR_DMX_OPENED                     (0x61) /*demux device has been opened, and can not open twice*/
#define ERR_DMX_NO_OPEN                    (0x62) /*demux device has not been opened*/
#define ERR_DMX_INVALID_DEVICE             (0x63) /*invalid device*/

/*统一接口添加******************/
#define ERR_DMX_SUB_DEVICE_NOT_OPEN        (0x64)
/*******************/

#define ERR_DMX_SYNC_PLAYBACK              (0x65)  /* AI7D01991 */

#define ERR_DMX_INVALID_DEVICEID  (0x66)
#define ERR_DMX_DEVICEID_USING (0x67)
#define ERR_DMX_WRONG_BUFFER_ID (0x68)
#define ERR_DMX_NO_ALLOCATE_BUFFER (0x69)
#define ERR_DMX_INVALID_CHAN_CREAT_TYPE (0x6a)
#define ERR_DMX_INVALID_REC_TYPE (0x6b)
#define ERR_DMX_INVALID_TS_SOURCE_TYPE (0x6c)
#define ERR_DMX_INVALID_CHAN_FUNC_TYPE (0x6d)
#define ERR_DMX_NO_DATA_OR_SPACE (0x6e)
#define ERR_DMX_NO_FRERARG       (0x6f)
#define ERR_DMX_INVALID_REF_CHAN_TYPE (0x70)

/*AI7D03076 mxf00129935 08/09/25 begin*/
#define ERR_DMX_OVER_RECPID_MAXNUM (0x71)
/*AI7D03076 mxf00129935 08/09/25 begin*/
#define ERR_DMX_INVALID_ERRORTYPE  (0x72)
#define ERR_DMX_CHAN_FILTER_NOT_MATCH (0x73)
#define ERR_DMX_CHAN_KEY_NOT_MATCH (0x74)
#define ERR_DMX_INVALID_CRC_MODE   (0x75)

#define ERR_DMX_NOT_SURPPORTED_FEATURE     (0xFF)  

/* DEMUX Error Code Define */
#define HI_ERR_DMX_INVALID_PID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_PID)
#define HI_ERR_DMX_NOTSET_PID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOTSET_PID)    

/*add by c55781*/
#define HI_ERR_DMX_OCCUPIED_PID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_OCCUPIED_PID)

#define HI_ERR_DMX_INVALID_PORTTYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_PORTTYPE)
#define HI_ERR_DMX_OPENING_PORT\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_OPENING_PORT)
#define HI_ERR_DMX_INVALID_RECVMODE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_RECVMODE)
#define HI_ERR_DMX_INVALID_PORTID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_PORTID)

#define HI_ERR_DMX_INVALID_CHAN_TYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_CHNTYPE)
#define HI_ERR_DMX_INVALID_CHNID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_CHNID)
#define HI_ERR_DMX_NOFIND_CHNID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOFIND_CHNID)
#define HI_ERR_DMX_NO_CHAN\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_CHAN)
#define HI_ERR_DMX_NOT_NEWCHN\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOT_NEWCHN)
#define HI_ERR_DMX_OPENING_CHN\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_OPENING_CHN)
#define HI_ERR_DMX_INVALID_BUFFER_SIZE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_BUFFERSIZE)


#define HI_ERR_DMX_INVALID_FILTERDEPTH\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_FILTERDEPTH)
#define HI_ERR_DMX_INVALID_FILTERID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_FILTERID)
#define HI_ERR_DMX_ATTACH_FILTER\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_ATTACH_FILTER)
#define HI_ERR_DMX_NOATTACH_FILTER\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOATTACH_FILTER)
#define HI_ERR_DMX_NOT_NEWFILTER\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_NEWFILTER)
#define HI_ERR_DMX_NO_MATCHFILTER\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_MATCHFILTER)
#define HI_ERR_DMX_NO_FREEFILTER\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_FREEFILTER)


#define HI_ERR_DMX_INVALID_KEYID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVAID_KEYID)
#define HI_ERR_DMX_ATTACH_KEY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_ATTACH_KEY)
#define HI_ERR_DMX_NOATTACH_KEY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOATTACH_KEY)
#define HI_ERR_DMX_NOMATCH_KEY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOMATCH_KEY)
#define HI_ERR_DMX_NOT_NEWKEY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOT_NEWKEY)
#define HI_ERR_DMX_NOT_FREEKEY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOT_FREEKEY)
#define HI_ERR_DMX_MULTI_KEYSET\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_MULTI_KEYSET)
#define HI_ERR_DMX_NOFIND_KEYID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOFIND_KEYID)

#define HI_ERR_DMX_AUD_MEM_COPY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_AUD_MEM_COPY)
#define HI_ERR_DMX_VID_MEM_COPY\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_VID_MEM_COPY)

#define HI_ERR_DMX_WAIT_TIMEOUT\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_WAIT_TIMEOUT)

#define HI_ERR_DMX_NO_DATA_OR_SPACE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_DATA_OR_SPACE)

#define HI_ERR_DMX_TS_POST_START\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_TS_POST_START)
#define HI_ERR_DMX_INVALID_TSPOST_TRESH\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_TSPOST_TRESH)

#define HI_ERR_DMX_INVALID_PACKET_CNTCTRL\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_PACKET_CNTCTRL)

#define HI_ERR_DMX_TSPOSTSIZE_BEYOND_BUFSIZE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_TSPOSTSIZE_BEYOND_BUFSIZE)
#define HI_ERR_DMX_NONEW_DATA_TO_READ\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NONEW_DATA_TO_READ)
#define HI_ERR_DMX_INVALID_CHANOPTION\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_CHANOPTION)
    
#define HI_ERR_DMX_PTR_NULL\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_PTR_NULL)
#define HI_ERR_DMX_OPENED\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_OPENED)
#define HI_ERR_DMX_NO_OPEN\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_OPEN)
#define HI_ERR_DMX_INVALID_DEVICE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_DEVICE)

/**************************/
#define HI_ERR_DMX_SUB_DEVICE_NOT_OPEN\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_SUB_DEVICE_NOT_OPEN)
/**************************/
#define HI_ERR_DMX_SYNC_PLAYBACK\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_SYNC_PLAYBACK)  /* AI7D01991 */

#define HI_ERR_DMX_INVALID_DEVICE_ID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_DEVICEID)
#define HI_ERR_DMX_SUB_DEVICE_USING\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_DEVICEID_USING)

#define HI_ERR_DMX_WRONG_BUFFER_ID\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_WRONG_BUFFER_ID)

#define HI_ERR_DMX_NO_ALLOCATE_BUFFER\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_ALLOCATE_BUFFER)

#define HI_ERR_DMX_INVALID_CHAN_CREAT_TYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_CHAN_CREAT_TYPE)

#define HI_ERR_DMX_INVALID_CHAN_FUNC_TYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_CHAN_FUNC_TYPE)

    
#define HI_ERR_DMX_INVALID_REC_TYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_REC_TYPE)
#define HI_ERR_DMX_INVALID_TS_SOURCE_TYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_TS_SOURCE_TYPE)
#define HI_ERR_DMX_NO_FRERARG\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NO_FRERARG)
#define HI_ERR_DMX_INVALID_REF_CHAN_TYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_REF_CHAN_TYPE)

/*AI7D03076 mxf00129935 08/09/25 begin*/
#define HI_ERR_DMX_OVER_RECPID_MAXNUM\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_OVER_RECPID_MAXNUM)
/*AI7D03076 mxf00129935 08/09/25 begin*/

/*错误码字变更********************/
#define HI_ERR_DMX_INVALID_ERRORTYPE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_ERRORTYPE)
    
/*统一接口添加********************************/
#define HI_ERR_DMX_CHAN_FILTER_NOT_MATCH\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_CHAN_FILTER_NOT_MATCH)
    
#define HI_ERR_DMX_CHAN_KEY_NOT_MATCH\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_CHAN_KEY_NOT_MATCH)
    
#define HI_ERR_DMX_INVALID_CRC_MODE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_INVALID_CRC_MODE)//AI7D04684
    
#define HI_ERR_DMX_NOT_SURPPORTED_FEATURE\
    HI_DEF_ERR(MID_DEMUX, HI_LOG_LEVEL_ERROR, ERR_DMX_NOT_SURPPORTED_FEATURE)  

/*统一接口添加宏定义  begin*/
#define HI_UNF_DEMUX
#ifdef HI_UNF_DEMUX
#define HI_ERR_DMX_INIT                   HI_ERR_DMX_OPENED
#define HI_ERR_DMX_NOT_INIT               HI_ERR_DMX_NO_OPEN
#define HI_ERR_DMX_INVALID_CHANHANDLE     HI_ERR_DMX_INVALID_CHNID
#define HI_ERR_DMX_INVALID_FILTERHANDLE   HI_ERR_DMX_INVALID_FILTERID
#define HI_ERR_DMX_INVALID_KEYHANDLE      HI_ERR_DMX_INVALID_KEYID
#define HI_ERR_DMX_INVALID_CHAN_OUTPUTMOD HI_ERR_DMX_INVALID_CHAN_FUNC_TYPE
#define HI_ERR_DMX_INVALID_PORT_MODE      HI_ERR_DMX_INVALID_TS_SOURCE_TYPE
#define HI_ERR_DMX_NOT_OPEN               HI_ERR_DMX_SUB_DEVICE_NOT_OPEN
#define HI_ERR_DMX_NOFIND_KEYHANDLE       HI_ERR_DMX_NOFIND_KEYID
#define HI_ERR_DMX_NOFIND_CHANHANDLE      HI_ERR_DMX_NOFIND_CHNID
#endif
/*统一接口专用  end*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEMUX_ERRCODE_H__ */


