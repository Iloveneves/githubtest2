/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment of the base Porting APIs needed by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

/*#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <sys/time.h>*/
#include "trd_ipanel_init.h"
#include "ipanel_base.h"


#ifdef __cplusplus
extern "C" {
#endif

static BU32 system_start_time = 0;
static BU08 biPanelAudioMuteState = FYF_TRUE;


BU08  ipanel_GetAudMuteState(void)
{
    printf("ipanel_GetAudMuteState=%d\n",biPanelAudioMuteState);
	return biPanelAudioMuteState;
}

void  ipanel_SetAudMuteState(BU08 flag)
{
    printf("ipanel_SetAudMuteState=%d\n",flag);
	biPanelAudioMuteState = flag;
}


/******************************************************************/
/*Alloc memory function.                                          */
/*Input:  memory size                                             */
/*Output&return: memory address pointer if success, other NULL    */
/******************************************************************/
void *ipanel_porting_malloc(int memsize)
{
    void *pointer = NULL;
	
    pointer = (void *)FYF_API_malloc(memsize);
    if (pointer)
    {    
    	ipanel_porting_dprintf("[ipanel porting malloc]malloc 0x%x bytes start from %p.\n", memsize, pointer);
	}
    else
    {
    	IPANEL_ERROR("[ipanel porting malloc]malloc fail.\n");
	}
        
    return pointer;
}

/******************************************************************/
/*Free memory function.                                           */
/*Input:  memory address pointer                                  */
/*Output&return: No                                               */
/******************************************************************/
void ipanel_porting_free(void* memptr)
{
    ipanel_porting_dprintf("[ipanel porting free]free 0x%x.\n",memptr);
    FYF_API_free(memptr);
	memptr = NULL;
}

/******************************************************************/
/*log output, description and format is same as ANSI printf       */
/******************************************************************/
int ipanel_porting_dprintf(const char* fmt, ...)
{
#if 0
    int ret = 0; 
    va_list args;

	va_start(args,fmt);
	ret = vprintf(fmt,args);
	va_end(args);
	fflush(stdout);
	return ret;
#else
    return 0;
#endif
}

/******************************************************************/
/*Description: iPanel Browser Runtime Base time.                  */
/*             and Unit is millisecond (1/1000 second).           */
/*Input      : No                                                 */
/*Output     : No                                                 */
/*Return     : counts of millisecond.                             */
/******************************************************************/
unsigned int ipanel_porting_time_ms(void)
{

//	printf("ipanel_porting_time_ms = %d", FYF_API_time_ms());
	return FYF_API_time_ms();

#if 0
    struct timeval tm = {0};

    if(0 == gettimeofday(&tm, NULL)) 
	{
        msec = tm.tv_sec * 1000 + tm.tv_usec/1000;
    }

    if (0 == system_start_time)
    {
    	system_start_time = msec;
    }
    return (msec - system_start_time);
#endif
    
}






#ifdef __cplusplus
}
#endif

