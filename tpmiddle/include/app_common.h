#ifndef _APP_COMMON_H_
#define _APP_COMMON_H_

#include <stdarg.h> 

#include "../../fyf/include/fyf_com.h"
#include "../../fyf/include/gui/config/gui_config.h"
#include "../../fyf/include/gui/def/gui_typedef.h"
#include "../../fyf/include/fyf_def.h"
#include "../../fyf/include/dev/fyf_demux.h"
#include "../../fyf/include/dev/fyf_tuner.h"
#include "../../fyf/include/os/fyf_semaphore.h"
#include "../../fyf/include/os/fyf_thread.h"
#include "../../fyf/include/os/fyf_queue.h"
#include "../../fyf/include/dev/fyf_adec.h"
#include "../../fyf/include/dev/fyf_vdec.h"
#include "../../fyf/include/dev/fyf_vout.h"
#include "../../fyf/include/dev/fyf_aout.h"
#include "../../fyf/include/dev/fyf_panel.h"
#include "../../fyf/include/dev/fyf_e2p.h"
#include "../../fyf/include/sys/fyf_system.h"
#include "../../fyf/include/gui/core/gui_lib_api.h"
#include "../../fyf/include/fyf_def.h"
#include "../../fyf/include/ca/fyf_ca.h"
#include "../../fyf/nvod/nvod_core.h"

#include "app_config.h"
#include "app_basic_type.h"
#include "app_dba.h"
#include "app_search.h"
#include "app_sys.h"
#include "app_init.h"
#include "app_parser.h"
#include "app_parserNit.h"
#include "app_parserPat.h"
#include "app_parserPmt.h"
#include "app_parserSdt.h"
#include "app_parserCat.h"
#include "app_parserEit.h"
#include "app_parserTdt.h"
#include "app_parserTot.h"
#include "app_parserBat.h"
#include "app_ChnDatabase.h"
#include "app_epg.h"
#include "app_signal.h"
#include "app_com.h"
#include "app_audio.h"
#include "app_dba_v2.h"
#include "app_ca.h"
#include "app_usb.h"
#include "app_player.h"
#include "app_picture_decoder.h"
#include "app_pvr.h"
#include "app_nvod.h"
#include "app_parserAdv.h"
#include "app_enrichvod.h"

typedef struct 
{
	BU08 ipaddr[16];
	BU08 netmask[16];
	BU08 gateway[16];
	BU08 firstdns[16];
	BU08 secdns[16];
	BU32 netflag;
}NetSetInfo;

enum FULLTOTVOD_STATE
{
	FULLTOTVOD_NULL_STATE,
	BPJUMP_STATE,
	BPZHUN_STATE,
	TVOD_PLAY_STATE,
	PP_EXIT_STATE,
	FAST_STATE,
	TVOD_JUMPTO_STATE,
	TVOD_JUMPBAR_STATE,
	TVOD_PAUSE_STATE,
	IPANEL_TO_TVOD_STATE,
	TVOD_VOL_STATE,
	TVOD_HELP_STATE,
	TVOD_NOKEY_PRESS_TIMEOUT_STATE,
	TVOD_ERROR_STATE,
};

enum VOD_STATE
{
	VOD_NULL_STATE,/*0*/
	VOD_PLAY_STATE,/*1*/
	VOD_PP_EXIT_STATE,/*2*/
	VOD_JUMPTO_STATE,/*3*/
	VOD_JUMPBAR_STATE, /*4*/
	VOD_VOL_STATE, /*5*/
	VOD_PAUSE_STATE, /*6*/
	VOD_HELP_STATE /*7*/
};


#define WM_SHOWCURTIME		(WM_USER+1)				//显示时间消息
#define WM_SHOWCAPROMPT		(WM_USER+2)				//显示CA提示信息
#define WM_HIDECAPROMPT		(WM_USER+3)				//隐藏CA提示信息
#define WM_SHOWPROGRAMPAUSE (WM_USER+4)				//显示节目暂停信息
#define WM_HIDE_PAUSE		(WM_USER+5)				//信号锁定，节目播放
#define WM_SHOWNOPROG		(WM_USER+6)				//显示无节目提示
#define WM_HIDENOPROG		(WM_USER+7)				//隐藏无节目提示
#define WM_CHECKLOCK		(WM_USER+8)				//节目加锁
#define WM_RESERVEPROMPT	(WM_USER+9)				//预约提示
#define WM_SIGNAL_UNLOCK    (WM_USER+10)			//信号中断
#define WM_SIGNAL_LOCK     	(WM_USER+11)			//信号锁定
#define WM_SIGNAL_STOP     	(WM_USER+12)			//信号锁定,节目暂停
#define WM_START_SAVEFLASH  (WM_USER+13)			//保存FLASH
#define WM_END_SAVEFLASH    (WM_USER+14)			//FLASH 保存完毕
#define WM_SHOW_CUR_EPG		(WM_USER+15)			//显示当前EPG
#define WM_CHECKLOCK_END	(WM_USER+16)			//节目加锁结束
#define WM_RESERVEPROMPT_END (WM_USER+17)			//预约提示结束
#define WM_FULLSCREENCHARGEPROG (WM_USER+18)			//全屏状态下改变节目播放
#define WM_SHOWSYSTIME  	(WM_USER+19)			//显示时间
#define WM_ISRESERVEONTIME  	(WM_USER+20)			//判断预约是否到达
#define WM_GETPMTONTIME  	(WM_USER+21)			//对pmt表的监控
#define WM_GETPATONTIME  	(WM_USER+22)			//对pat表的监控
#define WM_SIGNALDETECT  	(WM_USER+23)			//信号检测
#define WM_START_CARD		(WM_USER+24)			//开始卡通讯,值不能修改
#define WM_END_CARD			(WM_USER+25)			//结束卡通讯,值不能修改

#define WM_SIGNAL_PLAYCHANGE     	(WM_USER+26)			//节目改变

#define WM_NIT_VER_UPDATED	(WM_USER+27)
#define WM_OTA_DESC_DETECTED (WM_USER+28)

#define WM_IPP_SHOW			(WM_USER+29)			//显示IPP提示框
#define WM_IPP_HIDE			(WM_USER+30)			//隐藏IPP提示框
#define WM_IPP_NOTIFY		(WM_USER+31)			//IPP订购结果通知
#define WM_PROGRESS_STRIP	(WM_USER+32)			//显示进度

#define WM_REQUEST_FEEDING		(WM_USER+33)			//母卡数据读取结果提示
#define WM_REQUEST_FEEDING_END	(WM_USER+34)			//隐藏母卡数据读取结果

#define WM_OSD_SHOW_START		(WM_USER+35)			//屏显消息
#define WM_OSD_SHOW_END     	(WM_USER+36)			//隐藏屏显

#define WM_SHOWNOUSBREC		(WM_USER+37)            //未插入USB,无法录制
//#define LOCK_COLOR   RGB(0x68, 0xb8, 0xf8)//RGB(0,173,249)
//#define UNLOCK_COLOR RGB(0,173,249)//RGB(113,165,214)
#if defined(USE_FZVOD) || defined(USE_ENREACH)

#define WM_SHOWRELOGO		(WM_USER+38)            //显示回看logo
#define WM_HIDERELOGO		(WM_USER+39)            //隐藏回看logo
#endif
#define WM_GETSDTONTIME		(WM_USER+40)			//对sdt表的监控
#define WM_GETNITOTAONTIME	(WM_USER+41)

#define WM_DETECT_USB_INSERT	(WM_USER + 50) //检测到usb设备插入
#define WM_DETECT_USB_EVULSION	(WM_USER + 51) //检测到usb设备拔出
#define WM_REC_DISK_Full	    (WM_USER + 52)


#define WM_CHN_CHANGE			(WM_USER + 53)

#define WM_KEY_MESSAGE          (WM_USER + 54) //音量控制时使用
#define WM_Show_HDPROMPT		(WM_USER + 55)
#define WM_Hide_HDPROMPT		(WM_USER + 56)

#define WM_Show_EnrichVodError	(WM_USER + 57)
#endif






