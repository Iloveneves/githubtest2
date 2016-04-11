#ifndef __HI_API_DEBUG_H__
#define __HI_API_DEBUG_H__

#include "hi_type.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static FILE *gs_Debugfile = 0;
#define API_DEBUG_SWITCH       "/tmp/apidebug"

static HI_U32 inline HI_Api_IsDebug()
{
    HI_U8 buffer[5] = {0};
    HI_U8 ret = 0;

    if(0 == gs_Debugfile)
    {
        gs_Debugfile = fopen(API_DEBUG_SWITCH, "r");
        
        if(HI_NULL == gs_Debugfile)
        {
            perror("");
            return 0;
        }
    }

    ret = fread(buffer, 1, 4, gs_Debugfile);
    fseek(gs_Debugfile, 0, SEEK_SET);
    
    if(ret > 0)
    {
        if(0 == strcmp(buffer, "on\n"))
        {
            ret = 1;
        }
        else if(0 == strcmp(buffer, "off\n"))
        {
            ret = 0;
        }
    }

    return ret;
}


#define MSG(fmt, args...)\
do{\
    if(1 == HI_Api_IsDebug())\
    {\
        printf("\033[0;33m""%s(%d)"#fmt,__FUNCTION__,__LINE__, ##args);\
        printf("\n\033[0m");\
    }\
}while(0)


#define DO(api, ret)\
do{\
    ret = api;\
    if((ret != 0) && (1 == HI_Api_IsDebug()))\
        printf("\033[0;31m""%s failed !!in %s at %d line ret = 0x%x\n""\033[0m", #api, __FUNCTION__, __LINE__, ret);\
}while(0)


#define HI_TRANS(errcode)\
    return #errcode;

#define HI_TEST(api)\
do{\
    HI_S32 ret = api;\
    if(ret)\
        printf("\033[0;31m""%s failed !!in %s at %d line ret = 0x%x\n""\033[0m", #api, __FUNCTION__, __LINE__, ret);\
    else\
        printf("%s success at %d line\n", #api, __LINE__);\
}while(0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __HI_API_DEBUG_H__ */

