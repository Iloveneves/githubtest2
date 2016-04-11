/*===============================================================================

 File Name:app_config.h

 Description:

 History:
===============================================================================*/
#ifndef _APP_CONFIG_API_H_
#define _APP_CONFIG_API_H_


/*------------------------------------------------------------*/
/*              ���ֹ��ܵ������ļ�                            */
/*------------------------------------------------------------*/


#define TVMODE_ALL 0
#define TVMODE_ONLY_PAL 1
#define TVMODE_ONLY_NTSC 2

#define VIDEO_OUTPUT_YUV_YC 0
#define VIDEO_OUTPUT_YUV 1
#define VIDEO_OUTPUT_YC 2

/*
e2p crcУ�飬��ֹ��ͬ�汾����ͬһ�����ӵ�ʱ��Ҫ��e2p�ָ�һ��
*/

#define E2P_CRC_DATA	(0x1918)

/*
�Ƿ��б�����Ƶ�㣬ͨ��Ƶ�����Ƿ���nit���ж�
*/
//#define APP_CONFIG_MAIN_FREQ_BACK


#define APP_CONFIG_FORBID_VOD_FREQ 0
//#define SANMING_VERSION


/*
Ӣ�Ľ���
*/
//#define APP_CONFIG_LANGUAGE_ENGLISH

/*
ʱ��
*/
#define APP_CONFIG_TIME_ZONE 			8

/*
�Ƿ���0Ƶ��
*/
//#define APP_CONFIG_ZERO_CHANNEL

/*
����֧�ֵ���ʽ
*/
#define APP_CONFIG_TVMODE 				TVMODE_ALL

/*
��Ƶ���
*/

#define APP_CONFIG_VIDEO_OUTPUT			VIDEO_OUTPUT_YUV_YC
/*
pat����
*/
#define APP_CONFIG_PAT_MONITOR

#define APP_CONFIG_PAT_MONTIOR_TIME		3

/*
pmt����
*/
#define APP_CONFIG_PMT_MONITOR

#define APP_CONFIG_PMT_MONTIOR_TIME		3

/*
sdt����
*/
#define APP_CONFIG_SDT_MONITOR
#define APP_CONFIG_SDT_MONITOR_Time		3

/*
bat�������
����bat�����Ϣ
*/
#define APP_CONFIG_BAT_START_CHECK		1

/*
nit�������
����Ŀ�Ƿ���ºͳ�������
*/
#define APP_CONFIG_NIT_START_CHECK		1

/*
nit��ȫ�̼��
*/
#define APP_CONFIG_NIT_FULL_MONITOR		0

/*
����logo����
*/
#define APP_CONFIG_LOGO_UPDATE			0

/*
�ж϶��ٴ�û������Ƶ����ʾ��Ŀ��ͣ
*/
#define APP_CONFIG_SHOW_PROGRAM_STOP	30

/*
�ж϶��ٴ�û���źţ���ʾ���ź�logo
*/
#define APP_CONFIG_SHOW_NO_SIGNAL		30






//-----------------------------------------------------------------------------------------
/*
��ӡ�궨��
*/

//add by cwd 2009-12-15
//���ui����м���ӡ���õĺ�
//��ӡ�������
#define APP_PRINT_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
//���Դ�ӡ����
#define APP_PRINT_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);

//������ӡ����
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


