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
#ifndef __FYF_SEMAPHORE_H__
#define __FYF_SEMAPHORE_H__
typedef struct 
{ 
    BU32 (*FYF_CPI_semaphore_create)(const PU08 name,BS32 initial_count,BU32 
	      wait_mode); 
  	BS32 (*FYF_CPI_semaphore_destroy)(BU32 handle); 
  	BS32 (*FYF_CPI_semaphore_wait)(BU32 handle,BS32 wait_time); 
  	BS32 (*FYF_CPI_semaphore_release)(BU32 handle); 
}FYF_SEMAPHORE_PORTING; 
/******************************************************************************/
/*Description: register semaphore                                             */
/*Input      : porting														  */
/*Output     : no                                                             */
/*Return     : no					                                          */
/******************************************************************************/
void FYF_API_semaphore_register(FYF_SEMAPHORE_PORTING *porting);
/******************************************************************************/
/*Description: create semaphore                                               */
/*Input      : initial token count, task wait mode                            */
/*Output     : no                                                             */
/*Return     : >0 handle, success, 0 failure                                  */
/******************************************************************************/
BU32 FYF_API_semaphore_create(const PU08 name,BS32 initial_count,BU32 wait_mode);
/******************************************************************************/
/*Description: destroy semaphore                                              */
/*Input      : semaphore handle                                               */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_semaphore_destroy(BU32 handle);
/******************************************************************************/
/*Description: wait semaphore                                                 */
/*Input      : semaphore handle                                               */
/*Output     : no                                                             */
/*Return     : FYF_OS_QUEUE_SEM_STATUS_AVAILABLE success,                     */
/*              FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE failure                   */
/******************************************************************************/
BS32 FYF_API_semaphore_wait(BU32 handle,BS32 wait_time);
/******************************************************************************/
/*Description: release semaphore                                              */
/*Input      : semaphore handle                                               */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_semaphore_release(BU32 handle); 
#endif






