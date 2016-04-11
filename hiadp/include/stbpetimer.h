/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: petimer.h
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2006-12-4   fengyoulin                
 ******************************************************************************/
#ifndef __STB_PETIMER_H_EXT__
#define __STB_PETIMER_H_EXT__

#include <sys/time.h>

#include <pthread.h>
#include <time.h>

#include "hi_unf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef HI_VOID * petimer_t;

#define petimer_start stb_petimer_start
#define petimer_create_abstimer stb_petimer_create_abstimer
#define petimer_create_reltimer stb_petimer_create_reltimer
#define petimer_free_abstimer stb_petimer_free_abstimer
#define  petimer_free_reltimer stb_petimer_free_reltimer
#define petimer_start_abstimer stb_petimer_start_abstimer
#define petimer_start_reltimer stb_petimer_start_reltimer
#define petimer_stop_abstimer_safe stb_petimer_stop_abstimer_safe
#define petimer_stop_reltimer_safe stb_petimer_stop_reltimer_safe
#define petimer_start_reltimer_simple stb_petimer_start_reltimer_simple
#define petimer_register_tickhook stb_petimer_register_tickhook

#define PETF_AUTO_RESTART   0x0  /* the timer re-arms itself */
#define PETF_ONE_SHOOT      0x1  /* the timer start once and then stop */
#define PETF_AUTO_FREE      0x2  /* the timer start once and then free */

/**
 * start the whole timer system. see README for detail.
 *
 * @param thread space to hold thread id created by the function
 * @param attr attribute for creating the thread
 * @return errno
 */
extern int petimer_start(pthread_t * thread, pthread_attr_t * attr, HI_VOID **sync);

/**
 * create an absolute/relative timer.
 *
 * @param timeout the timeout function, it cannot be NULL
 * @param flag combination of PETF_ONE_SHOOT and PETF_AUTO_FREE
 * @return the time created
 */
extern petimer_t petimer_create_abstimer(HI_VOID (*timeout)(HI_VOID *), int flag);
extern petimer_t petimer_create_reltimer(HI_VOID (*timeout)(HI_VOID *), int flag);

/**
 * free an absolute/relative timer.
 *
 * @param timer the timer to free
 */
extern int petimer_free_abstimer(HI_VOID *sync_block, petimer_t timer);
extern int petimer_free_reltimer(HI_VOID *sync_block, petimer_t timer);

/**
 * start/restart an absolute/relative timer. restart happend if the timer is
 * already started.
 *
 * @param timer the timer to start
 * @param tm the time to start. only tm_sec, tm_min, tm_hour, tm_mday, tm_mon and 
 *           tm_year are senceness, the other fields are ignored. you may set a 
 *           field to -1, it means it can be anytime to match. Therefore, if you 
 *           need a timer to expired at 10:00am every day, you may set
 *           tm_sec=0 tm_min=0 tm_hour=10 tm_mday=-1 tm_mon=-1 tm_year=-1. 
 *           man 3 localtime for more detail of this field, you may have not notice
 *           that the tm_year is not the same with the current year number.
 * @param msec millisecond to start the timer, if it is zero, the timer expired
 *             when next tick come
 * @param arg the argument to start the timer
 * @return errno
 */
extern int petimer_start_abstimer(HI_VOID *sync_blk, petimer_t timer, struct tm * tm, HI_VOID * arg);
extern int petimer_start_reltimer(HI_VOID *sync_blk, petimer_t timer, unsigned int msec, HI_VOID * arg);

/**
 * stop an absolute timer.
 *
 * @param the timer to stop
 * @param finallize a function to finallize your own resource
 * @param arg argument for the finallize function
 * @return errno
 */
extern int petimer_stop_abstimer_safe(HI_VOID *sync_block, petimer_t timer, HI_VOID (*finallize)(HI_VOID *), HI_VOID * arg);
extern int petimer_stop_reltimer_safe(HI_VOID *sync_block, petimer_t timer, HI_VOID (*finallize)(HI_VOID *), HI_VOID * arg);
/* #define petimer_stop_abstimer(sync_block, timer) petimer_stop_abstimer_safe(timer, NULL, NULL); */
/* #define petimer_stop_reltimer(sync_block, timer) petimer_stop_reltimer_safe(timer, NULL, NULL); */

/**
 * like:
 */
extern int petimer_start_reltimer_simple(HI_VOID *sync_block, unsigned int msec, HI_VOID (*timeout)(HI_VOID *), HI_VOID * arg);


/**
 * 20030614 0.1 kenny create
 *
 * vi: set expandtab tabstop=4:
 */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __PETIMER_H__ */

