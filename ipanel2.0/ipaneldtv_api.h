/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the APIs opened by iPanel MiddleWare.

    Note: the "int" in the file is 32bits

    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_API_H_
#define _IPANEL_MIDDLEWARE_API_H_

#include <hi_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create iPanel Browser. you must input a stack memory size (>=5M).
 * And return Browser handle, Null is fail.
 */
void *ipaneldtv_create(unsigned int mem_size);

/** Deal events*/
void ipaneldtv_proc(void *handle, int msg, unsigned int p1, unsigned int p2);

/** Destroy iPanel Browser.*/
void ipaneldtv_destroy(void *handle);

/** Open Url.*/
int ipaneldtv_open_url(void *handle, const char *url);

/* added by yanhaifeng */
typedef enum tagIPANEL_STATUS
{
    IPANEL_STATUS_STOP,
    IPANEL_STATUS_PAUSE,
    IPANEL_STATUS_RUN,
    IPANEL_STATUS_BUTT
}IPANEL_STATUS;

int ipanel_init();
void ipanel_exit();
HI_U32 ipanel_get_exit_flag();
#ifdef __cplusplus
}
#endif

#endif
