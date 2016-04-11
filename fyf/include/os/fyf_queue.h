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
#ifndef __FYF_QUEUE_H__
#define __FYF_QUEUE_H__
typedef struct
{
	unsigned int qlstWordOfMsg;
	unsigned int q2ndWordOfMsg;
	unsigned int q3rdWordOfMsg;
	unsigned int q4thWordOfMsg;
}FYF_QUEUE_MESSAGE;
typedef struct
{
	BU32 (*FYF_CPI_queue_create)(const PU08 name,BU32 len,BU32 wait_mode);
	BS32 (*FYF_CPI_queue_destroy)(BU32 handle);
	BS32 (*FYF_CPI_queue_send)(BU32 handle,FYF_QUEUE_MESSAGE *msg);
	BS32 (*FYF_CPI_queue_recv)(BU32 handle,FYF_QUEUE_MESSAGE *mgs,
			BS32 wait_time);
}FYF_QUEUE_PORTING; 
/******************************************************************************/
/*Description: register queue                                                 */
/*Input      : porting														  */
/*Output     : no                                                             */
/*Return     : no					                                          */
/******************************************************************************/
void FYF_API_queue_register(FYF_QUEUE_PORTING *porting);
/******************************************************************************/
/*Description: message queue create                                           */
/*Input      : max queue length, wait mode                                    */
/*Output     : no                                                             */
/*Return     : >0 success, <=0 failure                                        */
/******************************************************************************/
BU32 FYF_API_queue_create(const PU08 name,BU32 len,BU32 wait_mode);
 /******************************************************************************/
/*Description: message queue destroy                                           */
/*Input      : queue handle                                                    */
/*Output     : no                                                              */
/*Return     : 0 success, -1 failure                                           */
/*******************************************************************************/ 
BS32 FYF_API_queue_destroy(BU32 handle);
/******************************************************************************/
/*Description: message queue send                                             */
/*Input      : queue handle, message                                          */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_queue_send(BU32 handle,FYF_QUEUE_MESSAGE * msg);
/******************************************************************************/
/*Description: message queue receive                                          */
/*Input      : queue handle, address to record message, wait mode, wait time  */
/*Output     : no                                                             */
/*Return     : FYF_OS_QUEUE_SEM_STATUS_AVAILABLE success,                     */
/*             FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE failure                    */
/******************************************************************************/   
BS32 FYF_API_queue_recv(BU32 handle,FYF_QUEUE_MESSAGE * msg,BS32 wait_time);
#endif



