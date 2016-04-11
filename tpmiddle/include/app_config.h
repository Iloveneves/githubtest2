/*===============================================================================

 File Name:app_config.h

 Description:

 History:
===============================================================================*/
#ifndef _APP_CONFIG_API_H_
#define _APP_CONFIG_API_H_


/*------------------------------------------------------------*/
/*              部分功能的配置文件                            */
/*------------------------------------------------------------*/


#define TVMODE_ALL 0
#define TVMODE_ONLY_PAL 1
#define TVMODE_ONLY_NTSC 2

#define VIDEO_OUTPUT_YUV_YC 0
#define VIDEO_OUTPUT_YUV 1
#define VIDEO_OUTPUT_YC 2

/*
e2p crc校验，防止不同版本烧入同一个机子的时候要把e2p恢复一下
*/

#define E2P_CRC_DATA	(0x1918)

/*
是否有备份主频点，通过频点中是否有nit来判断
*/
//#define APP_CONFIG_MAIN_FREQ_BACK


#define APP_CONFIG_FORBID_VOD_FREQ 0
//#define SANMING_VERSION


/*
英文界面
*/
//#define APP_CONFIG_LANGUAGE_ENGLISH

/*
时区
*/
#define APP_CONFIG_TIME_ZONE 			8

/*
是否有0频道
*/
//#define APP_CONFIG_ZERO_CHANNEL

/*
电视支持的制式
*/
#define APP_CONFIG_TVMODE 				TVMODE_ALL

/*
视频输出
*/

#define APP_CONFIG_VIDEO_OUTPUT			VIDEO_OUTPUT_YUV_YC
/*
pat表监控
*/
#define APP_CONFIG_PAT_MONITOR

#define APP_CONFIG_PAT_MONTIOR_TIME		3

/*
pmt表监控
*/
#define APP_CONFIG_PMT_MONITOR

#define APP_CONFIG_PMT_MONTIOR_TIME		3

/*
sdt表监控
*/
#define APP_CONFIG_SDT_MONITOR
#define APP_CONFIG_SDT_MONITOR_Time		3

/*
bat表开机检查
更新bat表的信息
*/
#define APP_CONFIG_BAT_START_CHECK		1

/*
nit表开机检查
检查节目是否更新和程序升级
*/
#define APP_CONFIG_NIT_START_CHECK		1

/*
nit表全程监控
*/
#define APP_CONFIG_NIT_FULL_MONITOR		0

/*
开机logo升级
*/
#define APP_CONFIG_LOGO_UPDATE			0

/*
判断多少次没有音视频，显示节目暂停
*/
#define APP_CONFIG_SHOW_PROGRAM_STOP	30

/*
判断多少次没有信号，显示无信号logo
*/
#define APP_CONFIG_SHOW_NO_SIGNAL		30






//-----------------------------------------------------------------------------------------
/*
打印宏定义
*/

//add by cwd 2009-12-15
//添加ui层和中间层打印调用的宏
//打印错误调用
#define APP_PRINT_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
//调试打印调用
#define APP_PRINT_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);

//正常打印调用
#define APP_PRINT_INFO( fmt, args... )
/*do { \
    printf("%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf(fmt, ##args);\
    printf("\n");\
} while (0);
*/

#define APP_Printf printf
#define APP_Search_printf   printf

#define SEARCH_DEBUG 0

#if (SEARCH_DEBUG > 0)
#define	SEARCH_DBG printf
#else
#define SEARCH_DBG
#endif


#define PATMONTIOR_DEBUG 0

#if (PATMONTIOR_DEBUG > 0)
#define	PATMONTIOR_DBG printf
#else
#define PATMONTIOR_DBG 
#endif



#define PMTMONTIOR_DEBUG 0

#if (PMTMONTIOR_DEBUG > 0)
#define	PMTMONTIOR_DBG printf
#else
#define PMTMONTIOR_DBG 
#endif



#if 1
#define FYF_API_printf printf
#else
#define FYF_API_printf(...) 
#endif


#define _SQL_DEBUG




#endif


