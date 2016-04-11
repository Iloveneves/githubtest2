
#include "fyf_com.h"

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <dirent.h>
#include "string.h"
#include <unistd.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <getopt.h>
#include <time.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/ioctl.h>

#define IPANEL_MSG_NAME "IPANELMSG"
#define SCREENWIDTH  (640)
#define SCREENHEIGTH (526)

//#define IPANEL_INFO
#ifdef IPANEL_INFO
#define IPANEL_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define IPANEL_DEBUG( fmt, args... )
#endif

#define IPANEL_WARNING
#ifdef IPANEL_WARNING
#define IPANEL_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define IPANEL_ERROR( fmt, args... )
#endif

typedef struct hiIPANEL_MSG_S
{
    BU32 u32Para1;
    BU32 u32Para2;
    BU32 u32Para3;    
}IPANEL_MSG_S;

typedef struct hiTRD_FREQINFO_S
{
    BU32       frq;  
    BU32       sym;           
    BU32       eqamType;
} HI_TRD_FREQINFO_S;

typedef struct tagEisTime 
{
	unsigned long tm_sec;
	unsigned long tm_usec;
}EisTime;


typedef struct
{
	BU32 FreqMhz;
	BU08 FEC_outer;
	BU08 Modulation;
	BU32 Symbol_rate;
	BU08 FEC_inner;
	BU16 RegionID;
	BU16 RegionPid;
	BU08 bUsed;
} DMUX_VodTsParams_t;


BU08 Ipanel_GetEntrance_Flag(void);
void Ipanel_SetEntrance_Flag(BU08 flag);
BU08 Ipanel_GetEnterVodOrTvod_Flag(void);
void Ipanel_SetEnterVodOrTvod_Flag(BU08 flag);


