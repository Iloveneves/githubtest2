
#include "stdarg.h"
#include "string.h"

#define	INCREASE_VOLUME			0
#define	DECREASE_VOLUME			1

#define TPAPP_DECODER_NUMBER				(0)
#define	TPSIDB_SERVICES_IN_ONE_TS		(30)
#define MAX_NO_OF_PROGRAMS_IN_ONE_TS	 TPSIDB_SERVICES_IN_ONE_TS   /* һ��Ƶ�������Ŀ���� */
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



/*���Դ�ӡ*/
#define ENREACH_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);

/*�����ӡ*/
#define ENREACH_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);

/*������ӡ*/
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
	unsigned short DescDataEx;			/* SDT ���и��ֶα�ʾ������������ ServiceID*/
								/* BAT ���и��ֶα�ʾ������������ transport_stream_id*/
								/* PMT ���и��ֶα�ʾ������������ stream_type */
								
	unsigned short DescDataEx2;			/* BAT ���и��ֶα�ʾ������������ original_network_id*/
								/* PMT ���и��ֶα�ʾ������������ elementary_PID */
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
ԭ�ͣ�int evtc_init(char *host, EVTC_INIT *INITPARA)
������vodʱ�ƿͻ��˳�ʼ��
������char *host��vodʱ��ǰ�˵�IP��ַ
	EVTC_INIT *INITPARA�����ز���������IPQAM����������Ƶ�����Ϣ�������EVTC_INIT�ṹ����
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_init(char *host, EVTC_INIT *INITPARA);
/*
ԭ�ͣ�int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist)
�������õ�ʱ�ƽ�Ŀ�б�
������char *host��vodʱ��ǰ�˵�IP��ַ
EVTC_TSLIST *Timeshiftlist��һ��ָ��EVTC_TSLIST�ṹ��ָ�룬�����ʱ�ƽ�Ŀ�Ļ������˺�������ֵ����0����ָ��һ��EVTC_TSLIST�ṹ������׵�ַ���������Ϊʱ�ƽ�Ŀ�ĸ���
���أ�0��ʧ�ܣ�����0��Ϊʱ�ƽ�Ŀ�ĸ���
*/
int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist);
/*
ԭ�ͣ�int evtc_getIPQAMGroup(char *host, int tsid)
��������ǰ�˵õ������е�IPQAM�����
������char *host��vodʱ��ǰ�˵�IP��ַ
	int tsid��evtc_init���ز������tsid�������tsid����ס�����ñ��õ�tsid
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_getIPQAMGroup(char *host, int tsid);
/*
ԭ�ͣ�int evtc_register(char* host)
��������������������֤
������char *host��vodʱ��ǰ�˵�IP��ַ
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_register(char* host);
/*
ԭ�ͣ�int evtc_vodplay(char *host, int moveid, EVTC_VOD*VODPLAYPARA)
������vod����
������char *host��vodʱ��ǰ�˵�IP��ַ
	int moveid���㲥�Ľ�Ŀid
	EVTC_VOD*VODPLAYPARA�����ز���������vod������Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_vodplay(char *host, char *moveid, EVTC_VOD*VODPLAYPARA);

/*
ԭ�ͣ�int evtc_ff(char *host, int status)
���������
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�����0Ϊ�������
*/
int evtc_ff(char *host, int status);
/*
ԭ�ͣ�int evtc_fb(char *host, int status)
����������
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�����0Ϊ���˱���
*/
int evtc_fb(char *host, int status);
/*
ԭ�ͣ�int evtc_replay (char *host, int status)
����������������ˣ���ͣ�����²���
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_replay (char *host, int status);
/*
ԭ�ͣ�int evtc_pause (char *host, int status)
��������ͣ
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_pause (char *host, int status);
/*
ԭ�ͣ�int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA)
������vod����
������char *host��vodʱ��ǰ�˵�IP��ַ
	int freq��ʱ�ƵĽ�ĿƵ��
int serviceid��ʱ�ƽ�Ŀ��service id
	EVTC_TS*RELAYPARA�����ز���������ʱ�Ʋ�����Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA);
/*
ԭ�ͣ�int evtc_pgdn(char *host, EVTC_TS*RELAYPARA)
�������л�����һʱ�ƽ�Ŀ
������char *host��vodʱ��ǰ�˵�IP��ַ
	EVTC_TS*RELAYPARA�����ز�����������һ��ʱ�ƽ�Ŀ������Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_pgdn(char *host, EVTC_TS*RELAYPARA);

/*
ԭ�ͣ�int evtc_pgup(char *host, EVTC_TS*RELAYPARA)
�������л�����һʱ�ƽ�Ŀ
������char *host��vodʱ��ǰ�˵�IP��ַ
	EVTC_TS*RELAYPARA�����ز�����������һ��ʱ�ƽ�Ŀ������Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_pgup(char *host, EVTC_TS*RELAYPARA);


/*
ԭ�ͣ�int evtc_getcurtime(char *host, int status, char* curtime)
��������ʹ������ʱ���հ�������Ҽ����ˣ���ȡ��Ŀ��ǰ���ŵ�ʱ��
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
char* curtime�����ز��������ؽ�Ŀ�ĵ�ǰ����ʱ�䣬�ַ�����ʽ�����磺20100329140525����ʾ2010��3��29��14��5��25��
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_getcurtime(char *host, int status, char* curtime);
/*
ԭ�ͣ�int evtc_settime(char *host, char* timestr, int status)
�����������갴ȷ�ϼ��󣬰����˵�ʱ��λ�ô�������������
������char *host��vodʱ��ǰ�˵�IP��ַ
char* timestr�����˵�ʱ��λ�ã��ַ�����ʽ�����磺20100329140525����ʾ2010��3��29��14��5��25��
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_settime(char *host, char* timestr, int status);
/*
ԭ�ͣ�int evtc_end (char *host, int status)
����������vodʱ�Ʋ���
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ1
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_end (char *host, int status);
/*
ԭ�ͣ�int evtc_close(void)
�������ر�vodʱ�ƿͻ���
��������
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_close(void);
/*
ԭ�ͣ�void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid))
�����������е��ã���ͻ���ע����ɱ��ʽ��������Ļص�����
������void (*my_cb_task_destroy)(unsigned long pid)����ɱ��ʽ��������ص�����
���أ���

*/
void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid));


/*
ԭ�ͣ�void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void))
�����������е��ã���ͻ���עע����ɱ��ʽ��������Ļص�����
������void (*my_cb_task_destroy_self)(void)����ɱ��ʽ��������ص�����
���أ���
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
ԭ�ͣ�void evtc_dbg (const char *fmt, ...)
���������Դ�ӡ��Ϣ���
������const char *fmt, ...����ӡ��������ݼ���ʽ�ַ���
���أ���
*/
void evtc_dbg (const char *fmt, ...);
/*
ԭ�ͣ�unsigned long evtc_getCurMS(void)
��������ȡ��ǰʱ�䣨��λ�����룩������������ڿ����ĺ�������
��������
���أ���ǰʱ�䣨��λ�����룩

*/
unsigned long evtc_getCurMS(void);

/*
ԭ�ͣ�long evtc_sock_open( EVTC_SOCK *sock )
����������socket������������������ӡ�
������һ��ָ�����socket��EVTC_SOCK�ṹ��ָ��
���أ�������socket������0�ɹ�������ʧ��
*/
long evtc_sock_open( EVTC_SOCK *sock );
/*
ԭ�ͣ�long evtc_sock_send( EVTC_SOCK *sock, char *buf, long len )
�������������ݡ�
������EVTC_SOCK *sock��һ��ָ�����socket��EVTC_SOCK�ṹ��ָ��
char *buf���������ݵ��׵�ַ��
long len���������ݵĳ��ȣ�
���أ��ܹ��ɹ��������ݵĳ��ȣ�С��0ʧ��
*/
long evtc_sock_send( EVTC_SOCK *sock, char *buf, long len );


/*
ԭ�ͣ�long evtc_sock_recv( EVTC_SOCK *sock, char *buf, long len )
�������������ݡ�����������
������EVTC_SOCK *sock��һ��ָ�����socket��EVTC_SOCK�ṹ��ָ��
char *buf�����ջ��������׵�ַ��
long len��buf�Ĵ�С
���أ��ܹ��ɹ��������ݵĳ��ȣ�С��0ʧ��
*/
long evtc_sock_recv( EVTC_SOCK *sock, char *buf, long len );


/*
ԭ�ͣ�long evtc_sock_close( EVTC_SOCK *sock )
�������ر�socket��
������EVTC_SOCK *sock��һ��ָ�����socket��EVTC_SOCK�ṹ��ָ��
���أ�0�ɹ�������ʧ��
*/
long evtc_sock_close( EVTC_SOCK *sock );
/*
ԭ�ͣ�unsigned long evtc_sock_getIpAddr( char *name)
�������������������IP��ַ��
������char *name������IP�����������ַ�����
���أ������ֽ����IP��ַ
*/
unsigned long evtc_sock_getIpAddr( char *name);
/*
ԭ�ͣ�void evtc_msleep(unsigned long ms)
��������������
������unsigned long ms�����ߵĺ�����
���أ���
*/
void evtc_msleep(unsigned long ms);
/*
ԭ�ͣ�unsigned char *evtc_getMACAddress( void )
�������õ������е�mac��ַ
��������
���أ�Сд�ַ�����ʽ��mac��ַ����MAC��01-23-45-67-89-AB���Ƿ����ַ���Ϊ"0123456789ab"������Ϊ"ffffffffffff"
*/
unsigned char *evtc_getMACAddress( void );
/*
ԭ�ͣ�unsigned long evtc_task_create(char* name, void *proc, void *args, unsigned long stackSize )
���������������̣߳�
������char* name				����������
void *proc				��������������ԭ��typedef int (*TaskProc)(void *args);
void *args				����������������
unsigned long stackSize	����ջ��С����λ���ֽ�
���أ���������id������0��ʾ����
*/
unsigned long evtc_task_create(char* name, void *proc, void *args, unsigned long stackSize );

/*
ԭ�ͣ�void evtc_task_destroy(unsigned long id)
������ɾ�������̣߳�
������unsigned long id				��Ҫɾ�������id
���أ���
*/
void evtc_task_destroy(unsigned long id);


/*
ԭ�ͣ�void evtc_stop(void)
������vodʱ�ƿͻ��˵��ô˺�����֮�����пͻ���Ҫ�˳����ڻ����н�������ķ�ʽΪ��ɱ������£�����ͻ��˴�����������Ҫ����������Χ�����е���evtc_close�ر�vodʱ�ƿͻ���
��������
���أ���
*/
void evtc_stop(void);


#define EVTC_FUN
/*
ԭ�ͣ�int evtc_init(char *host, EVTC_INIT *INITPARA)
������vodʱ�ƿͻ��˳�ʼ��
������char *host��vodʱ��ǰ�˵�IP��ַ
	EVTC_INIT *INITPARA�����ز���������IPQAM����������Ƶ�����Ϣ�������EVTC_INIT�ṹ����
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_init(char *host, EVTC_INIT *INITPARA);
/*
ԭ�ͣ�int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist)
�������õ�ʱ�ƽ�Ŀ�б�
������char *host��vodʱ��ǰ�˵�IP��ַ
EVTC_TSLIST *Timeshiftlist��һ��ָ��EVTC_TSLIST�ṹ��ָ�룬�����ʱ�ƽ�Ŀ�Ļ������˺�������ֵ����0����ָ��һ��EVTC_TSLIST�ṹ������׵�ַ���������Ϊʱ�ƽ�Ŀ�ĸ���
���أ�0��ʧ�ܣ�����0��Ϊʱ�ƽ�Ŀ�ĸ���
*/
int evtc_getTSList(char *host, EVTC_TSLIST *Timeshiftlist);
/*
ԭ�ͣ�int evtc_getIPQAMGroup(char *host, int tsid)
��������ǰ�˵õ������е�IPQAM�����
������char *host��vodʱ��ǰ�˵�IP��ַ
	int tsid��evtc_init���ز������tsid�������tsid����ס�����ñ��õ�tsid
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_getIPQAMGroup(char *host, int tsid);
/*
ԭ�ͣ�int evtc_register(char* host)
��������������������֤
������char *host��vodʱ��ǰ�˵�IP��ַ
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_register(char* host);
/*
ԭ�ͣ�int evtc_vodplay(char *host, int moveid, EVTC_VOD*VODPLAYPARA)
������vod����
������char *host��vodʱ��ǰ�˵�IP��ַ
	int moveid���㲥�Ľ�Ŀid
	EVTC_VOD*VODPLAYPARA�����ز���������vod������Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_vodplay(char *host, int moveid, EVTC_VOD*VODPLAYPARA);
/*
ԭ�ͣ�int evtc_ff(char *host, int status)
���������
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�����0Ϊ�������
*/
int evtc_ff(char *host, int status);
/*
ԭ�ͣ�int evtc_fb(char *host, int status)
����������
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�����0Ϊ���˱���
*/
int evtc_fb(char *host, int status);
/*
ԭ�ͣ�int evtc_replay (char *host, int status)
����������������ˣ���ͣ�����²���
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_replay (char *host, int status);
/*
ԭ�ͣ�int evtc_pause (char *host, int status)
��������ͣ
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_pause (char *host, int status);
/*
ԭ�ͣ�int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA)
������vod����
������char *host��vodʱ��ǰ�˵�IP��ַ
	int freq��ʱ�ƵĽ�ĿƵ��
int serviceid��ʱ�ƽ�Ŀ��service id
	EVTC_TS*RELAYPARA�����ز���������ʱ�Ʋ�����Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_relay(char *host, int freq, int serviceid, EVTC_TS*RELAYPARA);
/*
ԭ�ͣ�int evtc_pgdn(char *host, EVTC_TS*RELAYPARA)
�������л�����һʱ�ƽ�Ŀ
������char *host��vodʱ��ǰ�˵�IP��ַ
	EVTC_TS*RELAYPARA�����ز�����������һ��ʱ�ƽ�Ŀ������Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_pgdn(char *host, EVTC_TS*RELAYPARA);

/*
ԭ�ͣ�int evtc_pgup(char *host, EVTC_TS*RELAYPARA)
�������л�����һʱ�ƽ�Ŀ
������char *host��vodʱ��ǰ�˵�IP��ַ
	EVTC_TS*RELAYPARA�����ز�����������һ��ʱ�ƽ�Ŀ������Ҫ�Ĳ���
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_pgup(char *host, EVTC_TS*RELAYPARA);


/*
ԭ�ͣ�int evtc_getcurtime(char *host, int status, char* curtime)
��������ʹ������ʱ���հ�������Ҽ����ˣ���ȡ��Ŀ��ǰ���ŵ�ʱ��
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ0
char* curtime�����ز��������ؽ�Ŀ�ĵ�ǰ����ʱ�䣬�ַ�����ʽ�����磺20100329140525����ʾ2010��3��29��14��5��25��
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_getcurtime(char *host, int status, char* curtime);
/*
ԭ�ͣ�int evtc_settime(char *host, char* timestr, int status)
�����������갴ȷ�ϼ��󣬰����˵�ʱ��λ�ô�������������
������char *host��vodʱ��ǰ�˵�IP��ַ
char* timestr�����˵�ʱ��λ�ã��ַ�����ʽ�����磺20100329140525����ʾ2010��3��29��14��5��25��
	int status��������չ������Ϊ0
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_settime(char *host, char* timestr, int status);
/*
ԭ�ͣ�int evtc_end (char *host, int status)
����������vodʱ�Ʋ���
������char *host��vodʱ��ǰ�˵�IP��ַ
	int status��������չ������Ϊ1
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_end (char *host, int status);
/*
ԭ�ͣ�int evtc_close(void)
�������ر�vodʱ�ƿͻ���
��������
���أ�0��ʧ�ܣ�1���ɹ�
*/
int evtc_close(void);
/*
ԭ�ͣ�void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid))
�����������е��ã���ͻ���ע����ɱ��ʽ��������Ļص�����
������void (*my_cb_task_destroy)(unsigned long pid)����ɱ��ʽ��������ص�����
���أ���

*/
void regist_callback_task_destroy(void (*my_cb_task_destroy)(unsigned long pid));


/*
ԭ�ͣ�void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void))
�����������е��ã���ͻ���עע����ɱ��ʽ��������Ļص�����
������void (*my_cb_task_destroy_self)(void)����ɱ��ʽ��������ص�����
���أ���
*/
void regist_callback_task_destroyself(void (*my_cb_task_destroy_self)(void));


#endif



