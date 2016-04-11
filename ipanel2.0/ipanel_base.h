/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the base Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_BASE_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_BASE_API_FUNCTOTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*Alloc memory function. */
void *ipanel_porting_malloc(int memsize);

/*Free memory function. */
void ipanel_porting_free(void* ptr);

/*log output function. */
int ipanel_porting_dprintf(const char* fmt, ...);

/*iPanel MiddleWare runtime base time, and Unit is millisecond (1/1000 second).*/
unsigned int ipanel_porting_time_ms(void);

int ipanel_porting_dprintf(const char* fmt, ...);
    
#ifdef __cplusplus
}
#endif

#endif
