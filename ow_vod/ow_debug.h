#ifndef __OW_DEBUG_H_
#define __OW_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hi_stb_api.h"
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
//#include "petimer.h" 

#include "ow_api.h"
//#include "hi_api_eth.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"

#define CABLEDOWN  (220)
#define  USE_HI3110
//#define KEYTEST            




#define OW_INFO
#ifdef OW_INFO
#define OW_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define OW_DEBUG( fmt, args... )
#endif




#define OW_WARNING
#ifdef OW_WARNING
#define OW_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define OW_ERROR( fmt, args... )
#endif




#ifdef __cplusplus
}
#endif

#endif
