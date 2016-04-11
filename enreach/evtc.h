
#include "stdarg.h"
#include "string.h"

#define	INCREASE_VOLUME			0
#define	DECREASE_VOLUME			1

#define TPAPP_DECODER_NUMBER				(0)
#define	TPSIDB_SERVICES_IN_ONE_TS		(30)
#define MAX_NO_OF_PROGRAMS_IN_ONE_TS	 TPSIDB_SERVICES_IN_ONE_TS   /* 一个频点的最大节目个数 */
#define MAXPRGNAMELEN 20
#define TIMELEN		  15
#define EN_TASKID_MAX  15
#define MAX_TSVOD_NUM  50
#define REV_BUF_SIZE  2048
#define MAX_SOCKET_NUM   5
#ifndef NULL
#define NULL  0
#endif
#define envod_success 1
#define envod_fail     0
#define EN_INVALID_PID             0x1fff
#define EN_SEEK_COLOR            0xff96e2ff	// 0xffccffff
#define EN_FRAME_COLOR  		0xff88bff1
#define   EN_JLEFT   					10
#define   EN_JRIGHT 	 				11
#define   EN_JCENTER	 				12



/*调试打印*/
#define ENREACH_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);

/*错误打印*/
#define ENREACH_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);

/*正常打印*/
#define ENREACH_INFO( fmt, args... )\
do { \
    printf("%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf(fmt, ##args);\
    printf("\n");\
} while (0);

typedef  void (*ENREACH_TASK_PROC)(void *param);

typedef enum
{
	EN_BPZhun_EM = 0,
	EN_InBP_EM,
	EN_Jump_EM,
	EN_JumpTo_EM,
	EN_seek_EM,
	EN_BPJump_EM,
	EN_ReLogo_EM,
	EN_PP_EM,
	EN_OSD_PIC_NUM
} EN_PIC_NUM;

/*zhengsl-----------------------------------------------------
typedef struct
{
	void *GIFDataPointer;
	void *BMPDataPointer;
	U16 width;
	U16 height;
} GIF2BMP;

GIF2BMP enosd_bmp[EN_OSD_PIC_NUM];
-----------------------------------------------------------*/

typedef struct _evtc_sock {
	long socket;
	unsigned long	time;
	unsigned long	addr;
	unsigned short port;
	unsigned int status;
} EVTC_SOCK;


typedef struct _evtc_init{
	int result;
	int mainfreq;
	int maintsid;
	int sndfreq;
	int sndtsid;
	int hearttime;
	char serverIP[16];
}EVTC_INIT;

typedef struct _evtc_tslist
{
	int freq;
	int serviceid;
}EVTC_TSLIST;


typedef struct _evtc_vod{
	int result;
	int freq;
	int symbolrate;
	int qam;
	int apid;
	int vpid;
	int pcrpid;
	int pmtpid;
	int serviceid;
	char begintime[TIMELEN];
	char endtime[TIMELEN];
	char curtime[TIMELEN];
	char preprg[MAXPRGNAMELEN];
	char nextprg[MAXPRGNAMELEN];
	char curprg[MAXPRGNAMELEN];
	int playmod;
}EVTC_VOD;



typedef struct _evtc_ts{
	int result;
	int freq;
	int symbolrate;
	int qam;
	int apid;
	int vpid;
	int pcrpid;
	int pmtpid;
	int serviceid;
	char begintime[TIMELEN];
	char endtime[TIMELEN];
	char curtime[TIMELEN];
	char preprg[MAXPRGNAMELEN];
	char nextprg[MAXPRGNAMELEN];
	char curprg[MAXPRGNAMELEN];
	int playmod;
}EVTC_TS;



typedef struct _evtc_task
{
	pthread_t* Taskid;
	unsigned char* pStack;
	void* pTaskDesc;
	char UsedTag;
}EVTC_TASK;

typedef enum _prog_type
{
	EN_VOD = 0,
	TS_VOD = 1
}PROG_TYPE;

typedef struct
{									/* Description List Element */
	BU08 DescTag;
	BU08 DescLeng;
	BU08 DescData[256];
	unsigned short DescDataEx;			/* SDT 表中该字段表示描述符所属的 ServiceID*/
								/* BAT 表中该字段表示描述符所属的 transport_stream_id*/
								/* PMT 表中该字段表示描述符所属的 stream_type */
								
	unsigned short DescDataEx2;			/* BAT 表中该字段表示描述符所属的 original_network_id*/
								/* PMT 表中该字段表示描述符所属的 elementary_PID */
} TPSIDB_DescListElem_t;

typedef struct
{									/* Service List Element */
	unsigned short ServiceId;
	BU08 ServiceType;
} TPSIDB_ServiceListElem_t;

typedef struct
{									/* Service Map Elementary Stream Information */
	unsigned short Pid;
	BU08 StreamType;
	BU08 ComponentTag;
	int NumCas;
	union
	{
		struct
		{
			unsigned int Language;
		} Video;
		struct
		{
			unsigned int Language[2];
		} Audio;
		struct
		{
			unsigned int Language;
			unsigned short CompositionPage;
			unsigned short AncillaryPage;
		} Subtitle;
		struct
		{
			unsigned int Language;
			BU08 Magazine;
			BU08 Page;
		} Teletext;
	} Type;
} TPSIDB_ServiceMapEs_t;

typedef enum
{
	AID_VID_PCR_MOD = 0,
	SERVICEID_MOD,
	PMTPID_MOD
} PLAY_MOD;


void evtc_init_sem_prtct(void);
void evtc_lock_socket(void);
void evtc_unlock_socket(void);
int evtc_get_tsvodtag(vid);
void envod_printf (const char *fmt, ...);
void evtc_dbg (const char *fmt, ...);
void evtc_printf (const char *fmt, ...);
unsigned long evtc_getCurMS(void);
long evtc_sock_open( EVTC_SOCK *sock );
long evtc_sock_send( EVTC_SOCK *sock, char *buf, long len );
long evtc_sock_select(EVTC_SOCK * sock);
int  test_rec(int s, char *buf, int len, int timeOut);
long evtc_sock_recv( EVTC_SOCK *sock, char **buf, long len );
long evtc_sock_close( EVTC_SOCK *sock );
unsigned long evtc_sock_getIpAddr( char *name);
void evtc_msleep(unsigned long ms);
unsigned char *evtc_getMACAddress( void );
void evtc_defaultTaskId(void) ;
unsigned int evtc_task_create(const char *name, ENREACH_TASK_PROC func, void *param,
                                    int priority, unsigned int stack_size);


void evtc_stop(void);
int evtc_task_destroy(BS32 thread_id);
void evtc_task_destroyself(void);
BS32 evtc_set_tuner(BU32 freq, BU32 sym, BU32 qam);
void test_av(void);
int evtc_open_av(short fre_mhz,short sym, short qam,int video_pid, int audio_pid, int pcr_pid);
int evtc_parse_num2str(BU16 curYear, BU16 curMonth, BU16 curDay, BU16 hour, BU16 min, BU16 sec, char *yearStr, char *monthStr,
	char *dayStr, char *hourStr, char *minStr, char *secStr);

int evtc_add_time(BU16 * year, BU16 * month, BU16 * day, BU16 * hour, BU16 * min, BU16 * sec, BU16 perAdd);
int evtc_sub_time(BU16 * year, BU16 * month, BU16 * day, BU16 * hour, BU16 * min, BU16 * sec, BU16 perSub);
int evtc_cau_all_secs(BU16 year, BU16 month, BU16 day, BU16 hour, BU16 min, BU16 sec, int *allSecs);	
int evtc_get_secondByClock(char *clock, int *allsecs, BU16 * year, BU16 * month, BU16 * day, char bYearSecs);
int evtc_GetIPStrAndPortStr(char *progAdd, char *ipStr, char *portStr);
int evtc_parseSecToHourMin(int orisec, int *hour, int *min, int *sec);
int evtc_check_channel(void);
int evtc_getvidaid(EVTC_TS * RELAYPARA,int serid);

#define EVTC_FUN
/*
原型：int evtc_init(char *host, EVTC_INIT *INITPARA)
描述：vod时移客户端初始化
参数：char *host：vod时移前端的IP地址
	EVTC_INIT *INITPARA：返回参数，返回IPQAM所发的主备频点等信息，具体见EVTC_INIT结构定义
返回：0，失败；1，成功
*/
int evtc_init(char *host, EVTC_INIT *INITPARA);
/*
原型：int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist)
描述：得到时移节目列表
参数：char *host：vod时移前端的IP地址
EVTC_TSLIST *Timeshiftlist：一个指向EVTC_TSLIST结构的指针，如果有时移节目的话（即此函数返回值大于0）则指向一个EVTC_TSLIST结构数组的首地址，数组个数为时移节目的个数
返回：0，失败；大于0即为时移节目的个数
*/
int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist);
/*
原型：int evtc_getIPQAMGroup(char *host, int tsid)
描述：让前端得到机顶盒的IPQAM分组号
参数：char *host：vod时移前端的IP地址
	int tsid：evtc_init返回参数里的tsid，如果主tsid锁不住，就用备用的tsid
返回：0，失败；1，成功
*/
int evtc_getIPQAMGroup(char *host, int tsid);
/*
原型：int evtc_register(char* host)
描述：跟服务器进行认证
参数：char *host：vod时移前端的IP地址
返回：0，失败；1，成功
*/
int evtc_register(char* host);
/*
原型：int evtc_vodplay(char *host, int moveid, EVTC_VOD*VODPLAYPARA)
描述：vod播放
参数：char *host：vod时移前端的IP地址
	int moveid：点播的节目id
	EVTC_VOD*VODPLAYPARA：返回参数，返回vod播放需要的参数
返回：0，失败；1，成功
*/
int evtc_vodplay(char *host, char *moveid, EVTC_VOD*VODPLAYPARA);

/*
原型：int evtc_ff(char *host, int status)
描述：快进
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；大于0为快进倍数
*/
int evtc_ff(char *host, int status);
/*
原型：int evtc_fb(char *host, int status)
描述：快退
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；大于0为快退倍数
*/
int evtc_fb(char *host, int status);
/*
原型：int evtc_replay (char *host, int status)
描述：（快进，快退，暂停后）重新播放
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；1，成功
*/
int evtc_replay (char *host, int status);
/*
原型：int evtc_pause (char *host, int status)
描述：暂停
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；1，成功
*/
int evtc_pause (char *host, int status);
/*
原型：int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA)
描述：vod播放
参数：char *host：vod时移前端的IP地址
	int freq：时移的节目频点
int serviceid：时移节目的service id
	EVTC_TS*RELAYPARA：返回参数，返回时移播放需要的参数
返回：0，失败；1，成功
*/
int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA);
/*
原型：int evtc_pgdn(char *host, EVTC_TS*RELAYPARA)
描述：切换到下一时移节目
参数：char *host：vod时移前端的IP地址
	EVTC_TS*RELAYPARA：返回参数，返回下一个时移节目播放需要的参数
返回：0，失败；1，成功
*/
int evtc_pgdn(char *host, EVTC_TS*RELAYPARA);

/*
原型：int evtc_pgup(char *host, EVTC_TS*RELAYPARA)
描述：切换到上一时移节目
参数：char *host：vod时移前端的IP地址
	EVTC_TS*RELAYPARA：返回参数，返回上一个时移节目播放需要的参数
返回：0，失败；1，成功
*/
int evtc_pgup(char *host, EVTC_TS*RELAYPARA);


/*
原型：int evtc_getcurtime(char *host, int status, char* curtime)
描述：在使用拉杆时，刚按下左或右键拉杆，获取节目当前播放的时间
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
char* curtime：返回参数，返回节目的当前播放时间，字符串格式，例如：20100329140525，表示2010年3月29日14点5分25秒
返回：0，失败；1，成功
*/
int evtc_getcurtime(char *host, int status, char* curtime);
/*
原型：int evtc_settime(char *host, char* timestr, int status)
描述：拉杆完按确认键后，把拉杆的时间位置传给服务器播放
参数：char *host：vod时移前端的IP地址
char* timestr：拉杆的时间位置，字符串格式，例如：20100329140525，表示2010年3月29日14点5分25秒
	int status：留待扩展，现设为0
返回：0，失败；1，成功
*/
int evtc_settime(char *host, char* timestr, int status);
/*
原型：int evtc_end (char *host, int status)
描述：结束vod时移播放
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为1
返回：0，失败；1，成功
*/
int evtc_end (char *host, int status);
/*
原型：int evtc_close(void)
描述：关闭vod时移客户端
参数：无
返回：0，失败；1，成功
*/
int evtc_close(void);
/*
原型：void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid))
描述：机顶盒调用，向客户端注册他杀方式销毁任务的回调函数
参数：void (*my_cb_task_destroy)(unsigned long pid)：他杀方式销毁任务回调函数
返回：无

*/
void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid));


/*
原型：void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void))
描述：机顶盒调用，向客户端注注册自杀方式销毁任务的回调函数
参数：void (*my_cb_task_destroy_self)(void)：自杀方式销毁任务回调函数
返回：无
*/
void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void));

#if 0

#include "stdarg.h"
#include "string.h"
#include "..\include\stpti.h"
#include "tpddefs.h"
#include "tpstruct.h"
#include "..\..\..\tpapp_system\tpsystem.h"
#include "..\..\..\tpdriver_ip\tpip_api.h"
#include "..\..\..\tpdriver_graph\tpgraph_api.h"
#include "..\..\..\tpapp_usif\tpusif.h"
#include "..\..\..\tpapp_usif\usif_keymap.h" 
#include "..\..\..\tpdriver_graph\tpgraph.h"

#define MAXPRGNAMELEN 20
#define TIMELEN			30
#define EN_TASKID_MAX  15
#define MAX_TSVOD_NUM  50
#ifndef NULL
#define NULL  0
#endif
#define envod_success 1
#define envod_fail     0
#define EN_INVALID_PID             0x1fff
#define EN_SEEK_COLOR            0xff96e2ff	// 0xffccffff
#define EN_FRAME_COLOR  		0xff88bff1
#define   EN_JLEFT   					10
#define   EN_JRIGHT 	 				11
#define   EN_JCENTER	 				12

typedef enum
{
	EN_BPZhun_EM = 0,
	EN_InBP_EM,
	EN_Jump_EM,
	EN_JumpTo_EM,
	EN_seek_EM,
	EN_BPJump_EM,
	EN_ReLogo_EM,
	EN_PP_EM,
	EN_OSD_PIC_NUM
} EN_PIC_NUM;

typedef struct
{
	void *GIFDataPointer;
	void *BMPDataPointer;
	U16 width;
	U16 height;
} GIF2BMP;

GIF2BMP enosd_bmp[EN_OSD_PIC_NUM];

typedef struct _evtc_sock {
	long socket;
	unsigned long	time;
	unsigned long	addr;
	unsigned short port;
	unsigned int status;
} EVTC_SOCK;


typedef struct _evtc_init{
	int result;
	int mainfreq;
	int maintsid;
	int sndfreq;
	int sndtsid;
	int hearttime;
	char serverIP[16];
}EVTC_INIT;

typedef struct _evtc_tslist
{
	int freq;
	int serviceid;
}EVTC_TSLIST;

typedef struct _evtc_vod{
	int result;
	int freq;
	int symbolrate;
	int qam;
	int apid;
	int vpid;
	int pcrpid;
	int pmtpid;
	int serviceid;
}EVTC_VOD;

typedef struct _evtc_ts{
	int result;
	int freq;
	int symbolrate;
	int qam;
	int apid;
	int vpid;
	int pcrpid;
	int pmtpid;
	char begintime[TIMELEN];
	char endtime[TIMELEN];
	char curtime[TIMELEN];
	char preprg[MAXPRGNAMELEN];
	char nextprg[MAXPRGNAMELEN];
	char curprg[MAXPRGNAMELEN];
}EVTC_TS;

typedef struct _evtc_task
{
	task_t *Taskid;
	unsigned char*	pStack;
	void* pTaskDesc;
	char UsedTag;
}EVTC_TASK;

typedef enum _prog_type
{
	VOD = 0,
	TS_VOD = 1
}PROG_TYPE;



/*
原型：void evtc_dbg (const char *fmt, ...)
描述：调试打印信息输出
参数：const char *fmt, ...：打印输出的内容及格式字符串
返回：无
*/
void evtc_dbg (const char *fmt, ...);
/*
原型：unsigned long evtc_getCurMS(void)
描述：获取当前时间（单位：毫秒），可以是相对于开机的毫秒数。
参数：无
返回：当前时间（单位：毫秒）

*/
unsigned long evtc_getCurMS(void);

/*
原型：long evtc_sock_open( EVTC_SOCK *sock )
描述：创建socket，并与服务器进行链接。
参数：一个指向包括socket的EVTC_SOCK结构的指针
返回：创建的socket，大于0成功，否则失败
*/
long evtc_sock_open( EVTC_SOCK *sock );
/*
原型：long evtc_sock_send( EVTC_SOCK *sock, char *buf, long len )
描述：发送数据。
参数：EVTC_SOCK *sock：一个指向包括socket的EVTC_SOCK结构的指针
char *buf：发送数据的首地址；
long len：发送数据的长度；
返回：总共成功发送数据的长度；小于0失败
*/
long evtc_sock_send( EVTC_SOCK *sock, char *buf, long len );


/*
原型：long evtc_sock_recv( EVTC_SOCK *sock, char *buf, long len )
描述：接收数据。非阻塞类型
参数：EVTC_SOCK *sock：一个指向包括socket的EVTC_SOCK结构的指针
char *buf：接收缓冲区的首地址；
long len：buf的大小
返回：总共成功接收数据的长度；小于0失败
*/
long evtc_sock_recv( EVTC_SOCK *sock, char *buf, long len );


/*
原型：long evtc_sock_close( EVTC_SOCK *sock )
描述：关闭socket。
参数：EVTC_SOCK *sock：一个指向包括socket的EVTC_SOCK结构的指针
返回：0成功，否则失败
*/
long evtc_sock_close( EVTC_SOCK *sock );
/*
原型：unsigned long evtc_sock_getIpAddr( char *name)
描述：解析出参数里的IP地址。
参数：char *name：带有IP或者域名的字符串。
返回：网络字节序的IP地址
*/
unsigned long evtc_sock_getIpAddr( char *name);
/*
原型：void evtc_msleep(unsigned long ms)
描述：任务休眠
参数：unsigned long ms：休眠的毫秒数
返回：无
*/
void evtc_msleep(unsigned long ms);
/*
原型：unsigned char *evtc_getMACAddress( void )
描述：得到机顶盒的mac地址
参数：无
返回：小写字符串形式的mac地址，如MAC：01-23-45-67-89-AB，那返回字符串为"0123456789ab"，不能为"ffffffffffff"
*/
unsigned char *evtc_getMACAddress( void );
/*
原型：unsigned long evtc_task_create(char* name, void *proc, void *args, unsigned long stackSize )
描述：创建任务（线程）
参数：char* name				：任务名称
void *proc				：任务主函数，原型typedef int (*TaskProc)(void *args);
void *args				：任务主函数参数
unsigned long stackSize	：堆栈大小，单位是字节
返回：返回任务id，返回0表示错误
*/
unsigned long evtc_task_create(char* name, void *proc, void *args, unsigned long stackSize );

/*
原型：void evtc_task_destroy(unsigned long id)
描述：删除任务（线程）
参数：unsigned long id				：要删除任务的id
返回：无
*/
void evtc_task_destroy(unsigned long id);


/*
原型：void evtc_stop(void)
描述：vod时移客户端调用此函数告之机顶盒客户端要退出。在机顶盒结束任务的方式为他杀的情况下，如果客户端创建了任务，需要机顶盒在外围任务中调用evtc_close关闭vod时移客户端
参数：无
返回：无
*/
void evtc_stop(void);


#define EVTC_FUN
/*
原型：int evtc_init(char *host, EVTC_INIT *INITPARA)
描述：vod时移客户端初始化
参数：char *host：vod时移前端的IP地址
	EVTC_INIT *INITPARA：返回参数，返回IPQAM所发的主备频点等信息，具体见EVTC_INIT结构定义
返回：0，失败；1，成功
*/
int evtc_init(char *host, EVTC_INIT *INITPARA);
/*
原型：int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist)
描述：得到时移节目列表
参数：char *host：vod时移前端的IP地址
EVTC_TSLIST *Timeshiftlist：一个指向EVTC_TSLIST结构的指针，如果有时移节目的话（即此函数返回值大于0）则指向一个EVTC_TSLIST结构数组的首地址，数组个数为时移节目的个数
返回：0，失败；大于0即为时移节目的个数
*/
int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist);
/*
原型：int evtc_getIPQAMGroup(char *host, int tsid)
描述：让前端得到机顶盒的IPQAM分组号
参数：char *host：vod时移前端的IP地址
	int tsid：evtc_init返回参数里的tsid，如果主tsid锁不住，就用备用的tsid
返回：0，失败；1，成功
*/
int evtc_getIPQAMGroup(char *host, int tsid);
/*
原型：int evtc_register(char* host)
描述：跟服务器进行认证
参数：char *host：vod时移前端的IP地址
返回：0，失败；1，成功
*/
int evtc_register(char* host);
/*
原型：int evtc_vodplay(char *host, int moveid, EVTC_VOD*VODPLAYPARA)
描述：vod播放
参数：char *host：vod时移前端的IP地址
	int moveid：点播的节目id
	EVTC_VOD*VODPLAYPARA：返回参数，返回vod播放需要的参数
返回：0，失败；1，成功
*/
int evtc_vodplay(char *host, int moveid, EVTC_VOD*VODPLAYPARA);
/*
原型：int evtc_ff(char *host, int status)
描述：快进
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；大于0为快进倍数
*/
int evtc_ff(char *host, int status);
/*
原型：int evtc_fb(char *host, int status)
描述：快退
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；大于0为快退倍数
*/
int evtc_fb(char *host, int status);
/*
原型：int evtc_replay (char *host, int status)
描述：（快进，快退，暂停后）重新播放
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；1，成功
*/
int evtc_replay (char *host, int status);
/*
原型：int evtc_pause (char *host, int status)
描述：暂停
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
返回：0，失败；1，成功
*/
int evtc_pause (char *host, int status);
/*
原型：int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA)
描述：vod播放
参数：char *host：vod时移前端的IP地址
	int freq：时移的节目频点
int serviceid：时移节目的service id
	EVTC_TS*RELAYPARA：返回参数，返回时移播放需要的参数
返回：0，失败；1，成功
*/
int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA);
/*
原型：int evtc_pgdn(char *host, EVTC_TS*RELAYPARA)
描述：切换到下一时移节目
参数：char *host：vod时移前端的IP地址
	EVTC_TS*RELAYPARA：返回参数，返回下一个时移节目播放需要的参数
返回：0，失败；1，成功
*/
int evtc_pgdn(char *host, EVTC_TS*RELAYPARA);

/*
原型：int evtc_pgup(char *host, EVTC_TS*RELAYPARA)
描述：切换到上一时移节目
参数：char *host：vod时移前端的IP地址
	EVTC_TS*RELAYPARA：返回参数，返回上一个时移节目播放需要的参数
返回：0，失败；1，成功
*/
int evtc_pgup(char *host, EVTC_TS*RELAYPARA);


/*
原型：int evtc_getcurtime(char *host, int status, char* curtime)
描述：在使用拉杆时，刚按下左或右键拉杆，获取节目当前播放的时间
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为0
char* curtime：返回参数，返回节目的当前播放时间，字符串格式，例如：20100329140525，表示2010年3月29日14点5分25秒
返回：0，失败；1，成功
*/
int evtc_getcurtime(char *host, int status, char* curtime);
/*
原型：int evtc_settime(char *host, char* timestr, int status)
描述：拉杆完按确认键后，把拉杆的时间位置传给服务器播放
参数：char *host：vod时移前端的IP地址
char* timestr：拉杆的时间位置，字符串格式，例如：20100329140525，表示2010年3月29日14点5分25秒
	int status：留待扩展，现设为0
返回：0，失败；1，成功
*/
int evtc_settime(char *host, char* timestr, int status);
/*
原型：int evtc_end (char *host, int status)
描述：结束vod时移播放
参数：char *host：vod时移前端的IP地址
	int status：留待扩展，现设为1
返回：0，失败；1，成功
*/
int evtc_end (char *host, int status);
/*
原型：int evtc_close(void)
描述：关闭vod时移客户端
参数：无
返回：0，失败；1，成功
*/
int evtc_close(void);
/*
原型：void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid))
描述：机顶盒调用，向客户端注册他杀方式销毁任务的回调函数
参数：void (*my_cb_task_destroy)(unsigned long pid)：他杀方式销毁任务回调函数
返回：无

*/
void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid));


/*
原型：void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void))
描述：机顶盒调用，向客户端注注册自杀方式销毁任务的回调函数
参数：void (*my_cb_task_destroy_self)(void)：自杀方式销毁任务回调函数
返回：无
*/
void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void));


#endif



