#ifndef __FYF_DEF_H_
#define __FYF_DEF_H_

//#define _HAISI_DEMO_
#define _NEWLAND_DEMO_

/*FLASH分区表*/
#define FYF_FLASH_DEVID_STBINFO  1
#define FYF_FLASH_DEVID_LOADREINFO 2
#define FYF_FLASH_DEVID_SYSDATA  6
#define FYF_FLASH_DEVID_SYSDATABK  7
#define FYF_FLASH_DEVID_CA  8
#define FYF_FLASH_DEVID_CABK  9
#define FYF_FLASH_DEVID_ADVLOGO_INFO  11
#define FYF_FLASH_DEVID_ADVLOGO_DATA  12
#define FYF_FLASH_DEVID_VODINFO 13

#ifndef TYPE_BU08
#define TYPE_BU08
typedef unsigned char     BU08;
typedef BU08 *			  PU08;
#endif

#ifndef TYPE_BS08
#define TYPE_BS08
typedef signed char		  BS08;
typedef BS08 *			  PS08;
#endif

#ifndef TYPE_BU16
#define TYPE_BU16
typedef unsigned short	  BU16;
typedef BU16 *            PU16;
#endif

#ifndef TYPE_BS16
#define TYPE_BS16
typedef signed short      BS16;
typedef BS16 * 			  PS16;
#endif

#ifndef TYPE_BU32
#define TYPE_BU32
typedef unsigned long     BU32;
typedef BU32 *            PU32;
#endif

#ifndef TYPE_BS32
#define TYPE_BS32
typedef signed long       BS32;
typedef BS32 *            PS32;
#endif

#ifndef TYPE_BU64
#define TYPE_BU64
typedef unsigned long long     BU64;
typedef BU64 *            PU64;
#endif


#ifndef TYPE_BS64
#define TYPE_BS64
typedef long long     BS64;
typedef BS64 *            PS64;
#endif


#define FYF_NULL		0


#define FYF_OK			0
#define FYF_ERR			-1


#define FYF_THREAD_WAIT_FIFO	0
#define FYF_THREAD_WAIT_PRIO    1


#define FYF_NO_WAIT				0
#define FYF_WAIT_FOREVER		-1 

#define FYF_OS_QUEUE_SEM_STATUS_AVAILABLE	   FYF_OK
#define FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE    FYF_ERR

/*for cdca*/
#define FYF_RC_UNKNOWN               0x01  /* 未知错误 */
#define FYF_RC_POINTER_INVALID       0x02  /* 指针无效 */
#define FYF_RC_CARD_INVALID          0x03  /* 智能卡无效 */
#define FYF_RC_PIN_INVALID           0x04  /* PIN码无效 */
#define FYF_RC_DATASPACE_SMALL       0x06  /* 所给的空间不足 */
#define FYF_RC_CARD_PAIROTHER        0x07  /* 智能卡已经对应别的机顶盒 */
#define FYF_RC_DATA_NOT_FIND         0x08  /* 没有找到所要的数据 */
#define FYF_RC_PROG_STATUS_INVALID   0x09  /* 要购买的节目状态无效 */
#define FYF_RC_CARD_NO_ROOM          0x0A  /* 智能卡没有空间存放购买的节目 */
#define FYF_RC_WORKTIME_INVALID      0x0B  /* 设定的工作时段无效 */
#define FYF_RC_IPPV_CANNTDEL         0x0C  /* IPPV节目不能被删除 */
#define FYF_RC_CARD_NOPAIR           0x0D  /* 智能卡没有对应任何的机顶盒 */
#define FYF_RC_WATCHRATING_INVALID   0x0E  /* 设定的观看级别无效 */
#define FYF_RC_CARD_NOTSUPPORT       0x0F  /* 当前智能卡不支持此功能 */
#define FYF_RC_DATA_ERROR            0x10  /* 数据错误，智能卡拒绝 */
#define FYF_RC_FEEDTIME_NOT_ARRIVE   0x11  /* 喂养时间未到，子卡不能被喂养 */
#define FYF_RC_CARD_TYPEERROR        0x12  /* 子母卡喂养失败，插入智能卡类型错误 */

typedef struct
{
	BU32 x;
	BU32 y;
	BU32 w;
	BU32 h;
}FYF_Rect_s;

#define FYF_FALSE	0
#define FYF_TRUE    1

#ifndef LOWORD
#define LOWORD(_val)    ((BU16)(_val))
#endif

#ifndef HIWORD
#define HIWORD(_val)    ((BU16)((BU32)(_val)>>16))
#endif

#ifndef LOBYTE
#define LOBYTE(_val)    ((BU08)(_val))
#endif

#ifndef HIBYTE
#define HIBYTE(_val)    ((BU08)((BU16)(_val)>>8))
#endif



#define FYF_INFO
#ifdef FYF_INFO
#define FYF_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define FYF_DEBUG( fmt, args... )
#endif

#define FYF_WRAMING
#ifdef FYF_WRAMING
#define FYF_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define FYF_ERROR( fmt, args... )
#endif

#endif

