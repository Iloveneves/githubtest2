/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/07/03                                           	  */
/******************************************************************************/ 
#ifndef __FYF_THREAD_H__
#define __FYF_THREAD_H__
typedef void (*FYF_THREAD_PROC)(void * param);
typedef struct 
{ 
	BU32 (*FYF_CPI_thread_create)(const PU08 name,FYF_THREAD_PROC func,void * 
			param,BS32 priority,BS32 stack_size); 
	BS32 (*FYF_CPI_thread_destroy)(BU32 handle); 
	void (*FYF_CPI_thread_sleep)(BS32 ms); 
	BU32 (*FYF_CPI_thread_getCurID)(void);
}FYF_THREAD_PORTING;
/******************************************************************************/
/*Description: register thread                                                */
/*Input      : porting														  */
/*Output     : no                                                             */
/*Return     : no					                                          */
/******************************************************************************/
void FYF_API_thread_register(FYF_THREAD_PORTING *porting);
/******************************************************************************/
/*Description: create thread                                                  */
/*Input      : thread name thread process function, function parameter,       */
/*             priority, stack size, arg1, arg2                               */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_API_thread_create(const PU08 name,FYF_THREAD_PROC func,void * param,BS32 priority,BS32 stack_size);
/******************************************************************************/
/*Description: destroy thread                                                 */
/*Input      : thread handle                                                  */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_thread_destroy(BU32 handle);
/******************************************************************************/
/*Description: thread  sleep                                                  */
/*Input      : the time of thread to sleep                                    */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_thread_sleep(BS32 ms);
/******************************************************************************/
/*Description: get cur thread  id                                                 */
/*Input      :                                     */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_thread_getCurID(void);
#endif





