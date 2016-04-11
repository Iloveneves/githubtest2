/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: tunersvr.c
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-09-01   diaoqiangwei/d60770                
 ******************************************************************************/
#include <sys/time.h>

#include <memory.h>
#include <pthread.h>
#include <semaphore.h>


#include "hi_unf_tuner.h"

#include "software.h"
#include "hardware.h"

#include "svrdebug.h"
#include "hi_msg_queue.h"
#include "msg_event.h"
#include "stbpetimer.h"

#include "hi_tunersvr.h"
#include "hi_tunersvr_ext.h"

#define TUNER_CHECK_TIMER       (300)
#define TUNER_SENDMSG_TIMER		(5)


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/************************static var************************/

/* timer */
static petimer_t  s_pTunerTimerHandle = HI_NULL;	/*tunersvr timer handle*/

static pthread_t s_pTunerTaskHandle = HI_NULL;		/*tunersvr task handle*/
static HI_MsgQueue_p  s_pTunerMsgQueue = HI_NULL;	/*tunersvr msg queue*/

/* 回调函数*/
static T_TunerCallback s_pTunerCallback = HI_NULL;	/*tunersvr call back function*/

/* TIMER 记数器，用于产生超时时间*/
static HI_U32  s_u32TunerTimerCount = 0;                 /*timer count*/

/* TUNER任务处理状态*/
static TUNERSVR_STATE_E  s_enTunerCurState = TUNER_IDLE;/*tunersvr status*/

/* 信号属性*/
static Tuner_Propety_S 	s_stTunerPropety;
static DB_FRONTEND_S	g_strTunerPara;

/*****************************************************************************
 * Function:      TunerTimerProc
 * Description:   tuner timer proc, send timer msg to tunersvr task or sectionsvr task
 * Data Accessed:None
 * Data Updated: s_u32TunerTimerCount
 * Input:             None
 * Output:           None
 * Return:  None
 * Others:     None
 *****************************************************************************/
static HI_VOID TunerTimerProc(HI_VOID *pArg)
{
	HI_S32 s32Ret = HI_FAILURE;
	MSG_MESSAGE_S  struTunerTimerMsg = {0};

	if( HI_NULL == pArg )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, "TunerTimerProc arg null\n");
		return;
	}

	switch(((HI_CHAR *)pArg)[0])
	{
		case 'a': /*tunner timer*/
			{
				struTunerTimerMsg.u32MessageID = MESSAGE_TUNERSVR_CHECK_TIMER;

				/*send this timer msg to tunersvr task proc*/
				s32Ret = TunerSendMsg(&struTunerTimerMsg);

				//debug
				//SendKey(0 , 0,  0x35caff00);
				
				if ( HI_SUCCESS != s32Ret )
				{
					SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner timer send message error!\n");
				}

				break;
			}
		default:
			{
				break;
			}
	}
}




static HI_S32 TunerStartTimer(HI_VOID)
{
	HI_S32 s32Ret = HI_FAILURE;

	/*create tuner timer */
	s_pTunerTimerHandle = stb_petimer_create_reltimer(TunerTimerProc,  PETF_AUTO_RESTART);
	if (HI_NULL == s_pTunerTimerHandle)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner timer create error\n");      
		return HI_FAILURE;
	}

	//s32Ret = stb_petimer_start_reltimer(APPMNGSVR_GetTimeThreadID(), s_pTunerTimerHandle, TUNER_CHECK_TIMER, (HI_VOID *)"a");
	if ( HI_SUCCESS != s32Ret )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner timer start error\n");      
		return HI_FAILURE;
	}
	return HI_SUCCESS;
}





/*****************************************************************************
 * Function:      TunerTunerCallBack
 * Description:   in this function call the tunersvr callback function
 * Data Accessed:s_pTunerCallback
 * Data Updated: None
 * Input:             None
 * Output:           None
 * Return: HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/
static HI_VOID  TunerTunerCallBack( HI_U32 MessageID,  HI_U16 FrontEndDeviceID,  HI_U16 FrontEndID)
{
	SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, "TunerTunerCallBack\n");

	if (HI_NULL == s_pTunerCallback )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner callback function not register\n");
		return;
	}   

	/*call callback function that be appmngsvr registered*/
	s_pTunerCallback( MessageID,   FrontEndDeviceID,  FrontEndID);
	return;
}

/*****************************************************************************
 * Function:      TunerConnectTuner
 * Description:   connect tuner through tuner driver
 * Data Accessed:None
 * Data Updated: None
 * Input:             None
 * Output:           pstruTunerSet
 * Return:  HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/
static HI_S32 TunerConnectTuner(HI_U16 DeviceID, DB_FRONTEND_S  struTunerFrontEnd )
{
	HI_S32 s32Ret = HI_FAILURE;

	HI_UNF_TUNER_CONNECT_PARA_S struTunerSet;
	
	/*config tuner param*/    
	struTunerSet.unConnectPara.stCab.u32Freq = struTunerFrontEnd.u32Frequency;
	struTunerSet.unConnectPara.stCab.u32SymbolRate = struTunerFrontEnd.u32SymbolRate;
	struTunerSet.unConnectPara.stCab.enModType = struTunerFrontEnd.u8Modulation+0x100;
	
	struTunerSet.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
	struTunerSet.unConnectPara.stCab.bReverse = HI_FALSE;


	if (HARDWARETUNER_TYPE == HARDWARETUNER_NXP1216S)
	{
		//debug
//		struTunerSet.unConnectPara.stCab.u32Freq = 610000;
		struTunerSet.unConnectPara.stCab.u32SymbolRate = 6875000;
//		struTunerSet.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
		
		s32Ret =  HI_UNF_TUNER_Connect( DeviceID , &struTunerSet, 10);
		if(s32Ret != 0)
			printf("\n +++ tuner connect error 0x%x +++ \n",s32Ret);

	}

	
	return s32Ret;
}


HI_S32 TunerLockCheck(HI_VOID)
{
	HI_S32 s32Ret = HI_FAILURE;

	HI_UNF_TUNER_STATUS_S 	TunerStatus; 

	if (HARDWARETUNER_TYPE == HARDWARETUNER_NXP1216S)
	{
		s32Ret = HI_UNF_TUNER_GetStatus(0 ,  &TunerStatus);
		
	}

	
	if  (HI_SUCCESS == s32Ret)
	{

		if (HARDWARETUNER_TYPE == HARDWARETUNER_NXP1216S)
		{
			
			if (HI_UNF_TUNER_SIGNAL_LOCKED == TunerStatus.enLockStatus)
			{

				s_stTunerPropety.IsLocked = 0;
		   		return(HI_SUCCESS);
			}
		}

		
	}

	s_stTunerPropety.IsLocked = 1;

	return HI_FAILURE;
}


/*****************************************************************************
 * Function:      TunerGetSignalProperty
 * Description:   get SignalProperty through tuner driver
 * Data Accessed:None
 * Data Updated: None
 * Input:             None
 * Output:           pstruTunerSet
 * Return:  HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/

static HI_VOID TunerGetSignalProperty(HI_VOID)
{
	return;
}

/*****************************************************************************
 * Function:      TunerSendMsg
 * Description:   send msg to tunersvr msg queue
 * Data Accessed:s_pTunerMsgQueue
 * Data Updated: None
 * Input:             None
 * Output:           None
 * Return:  HI_SUCCESS, HI_FAILURE
 * Others:     
 *****************************************************************************/
static HI_S32  TunerSendMsg(MSG_MESSAGE_S *pstruTunerMsg)
{
	MSG_MESSAGE_S *pTmpMsg = HI_NULL;

	SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, "TunerSendMsg\n");


	if (HI_NULL == pstruTunerMsg)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " pstruTunerMsg is null\n");
		return HI_FAILURE;		
	}
	
	if (HI_NULL == s_pTunerMsgQueue )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " s_pTunerMsgQueue is null\n");
		return HI_FAILURE;
	}

	pTmpMsg =  (MSG_MESSAGE_S*)HI_OSMsgClaim(s_pTunerMsgQueue, 0);
	if (HI_NULL == pTmpMsg)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " HI_OSMsgClaim fail\n");
		return HI_FAILURE;
	}

	memcpy(pTmpMsg, pstruTunerMsg, sizeof(MSG_MESSAGE_S));
	HI_OSMsgSend(s_pTunerMsgQueue, pTmpMsg);/*send msg to tunersvr*/

	return HI_SUCCESS;
}

/*****************************************************************************
 * Function:      TunerTaskProc
 * Description:   tunersvr, process the msg to control tunersvr flow
 * Data Accessed:s_pTunerMsgQueue
 * Data Updated: s_enTunerCurState
 * Input:             None
 * Output:           None
 * Return:  None
 * Others:     None
 *****************************************************************************/
static HI_VOID TunerTaskProc (HI_VOID)
{
	HI_S32 s32Ret = HI_FAILURE;
	HI_U16 u16DeviceID;
	HI_U16 u16FrontEndID;

	DB_FRONTEND_S   struTunerFrontEnd;  

	MSG_MESSAGE_S *pTunerTmpMsg = HI_NULL;/*tmp receive msg*/	
	MSG_MESSAGE_S *pTunerProcMsg = HI_NULL;/*tmp receive msg*/	

	MSG_MESSAGE_S struTunerTmpMsg;
	MSG_MESSAGE_S struTunerProcMsg;

		
	pTunerTmpMsg = &struTunerTmpMsg;
	pTunerProcMsg = &struTunerProcMsg;
		
	while ( 1 )
	{
		pTunerTmpMsg = (MSG_MESSAGE_S*)HI_OSMsgReceive(s_pTunerMsgQueue, 0);

		if (HI_NULL == pTunerTmpMsg )/*if receive msg is null , the continue to get msg*/
		{
			SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR," message pointer in NULL, receive message wrong!\n");
		}
		else
		{
			memcpy( pTunerProcMsg, pTunerTmpMsg, sizeof(MSG_MESSAGE_S) );
			HI_OSMsgRelease(s_pTunerMsgQueue, pTunerTmpMsg);/*release this received msg from tunersvr msg queue*/

			switch ( s_enTunerCurState )
			{
				case TUNER_IDLE:/*idle status, only process start msg*/
					{
						switch ( pTunerProcMsg->u32MessageID )
						{
							case MESSAGE_TUNERSVR_ID:
								{
									u16DeviceID = (HI_U16)pTunerProcMsg->s32Para1;
									u16FrontEndID = (HI_U16)pTunerProcMsg->s32Para2;
									break;								
								}
							
							case MESSAGE_TUNERSVR_START:/*start msg*/
								{
									s_u32TunerTimerCount = 0;
									
									SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR," idle status:MESSAGE_TUNERSVR_START\n");

									struTunerFrontEnd.u32Frequency = (HI_U32)pTunerProcMsg->s32Para1;
									struTunerFrontEnd.u32SymbolRate =  (HI_U32)pTunerProcMsg->s32Para2;
									struTunerFrontEnd.u8Modulation = (HI_U8)pTunerProcMsg->s32Para3;

									/*connect tuner through tuner driver, this connect is unlock*/

									memcpy((void *)&g_strTunerPara, (void *)&struTunerFrontEnd, sizeof(DB_FRONTEND_S));
									
									s32Ret = TunerConnectTuner( u16DeviceID, struTunerFrontEnd);
									if (HI_SUCCESS != s32Ret)
									{
										SVR_TRACE(HI_STB_LOG_LEVEL_WARNING, TUNERSVR,"idle status: TunerConnectTuner fail\n");
									}     
									
									s_enTunerCurState = TUNER_LOCKING;/*in this, tuner isn't locked*/
									break;
								}
							default:  
								{
									break;
								}
						}
						break;
					}
				case TUNER_LOCKING:/*unlock status ,in this status, to lock tuner each check timer, process : start, stop, check timer, unlock timer msg*/
					{
						switch ( pTunerProcMsg->u32MessageID )
						{
							case MESSAGE_TUNERSVR_ID:
								{
									u16DeviceID = (HI_U16)pTunerProcMsg->s32Para1;
									u16FrontEndID = (HI_U16)pTunerProcMsg->s32Para2;
									break;								
								}
							case  MESSAGE_TUNERSVR_START:/*start msg*/
								{
									s_u32TunerTimerCount = 0;
										
									SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR," locking status:MESSAGE_TUNERSVR_START\n");

									struTunerFrontEnd.u32Frequency = (HI_U32)pTunerProcMsg->s32Para1;
									struTunerFrontEnd.u32SymbolRate = (HI_U32)pTunerProcMsg->s32Para2;
									struTunerFrontEnd.u8Modulation = (HI_U8)pTunerProcMsg->s32Para3;

									memcpy((void *)&g_strTunerPara, (void *)&struTunerFrontEnd, sizeof(DB_FRONTEND_S));

									s32Ret = TunerConnectTuner( u16DeviceID, struTunerFrontEnd);
									if ( HI_SUCCESS != s32Ret )
									{
										SVR_TRACE(HI_STB_LOG_LEVEL_WARNING, TUNERSVR,"  locking status: TunerConnectTuner fail\n");  
									}
									
									break;
								}
							case  MESSAGE_TUNERSVR_STOP:/*stop msg*/
								{
									TunerTunerCallBack( MESSAGE_TUNERSVR_DROP, u16DeviceID,  u16FrontEndID);

									s_u32TunerTimerCount = 0;									
									s_enTunerCurState = TUNER_IDLE;
									break;
								}
								/*check timer: check the tuner status, if unlock, then relock;after unlock timer, send unlock msg to appmngsvr*/
							case MESSAGE_TUNERSVR_CHECK_TIMER :
								{								
									s32Ret = TunerLockCheck();
									
									if ( HI_SUCCESS == s32Ret )
									{
										SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, " tuner locked!\n");

										printf("\n +++ TUNER LOCKED AND SEND MESSAGE TO APPMNGSVR +++\n");
										
										TunerTunerCallBack(MESSAGE_TUNERSVR_LOCK, u16DeviceID,  u16FrontEndID);
										
										s_enTunerCurState = TUNER_LOCKED;
										
										s_u32TunerTimerCount = 0;

										break;
									}
									else
									{
										s_u32TunerTimerCount ++;
										
										if (s_u32TunerTimerCount >TUNER_SENDMSG_TIMER)
										{
											s_u32TunerTimerCount = 0;									
											SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR,"  locking status: no SIGNAL_CONNECTED \n");  
											TunerTunerCallBack(MESSAGE_TUNERSVR_UNLOCK, u16DeviceID,  u16FrontEndID);
										}
										
									}
									
									break;                            
								}
						
							default:
								{
									SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR," locking status:default message = %d\n",pTunerProcMsg->u32MessageID);
									break;
								}
						}
						break;
					}
				case TUNER_LOCKED: /*locked status, process: start, stop, check timer msg*/
					{
						switch ( pTunerProcMsg->u32MessageID )
						{
							case MESSAGE_TUNERSVR_ID:
								{
									u16DeviceID = (HI_U16)pTunerProcMsg->s32Para1;
									u16FrontEndID = (HI_U16)pTunerProcMsg->s32Para2;
									break;								
								}

							case MESSAGE_TUNERSVR_START:
								{
									s_u32TunerTimerCount =0;
									
									struTunerFrontEnd.u32Frequency = (HI_U32)pTunerProcMsg->s32Para1;
									struTunerFrontEnd.u32SymbolRate = (HI_U32)pTunerProcMsg->s32Para2;
									struTunerFrontEnd.u8Modulation = (HI_U8)pTunerProcMsg->s32Para3;

									memcpy((void *)&g_strTunerPara, (void *)&struTunerFrontEnd, sizeof(DB_FRONTEND_S));

									s32Ret = TunerConnectTuner(u16DeviceID,  struTunerFrontEnd);
									if ( HI_SUCCESS != s32Ret )
									{
										SVR_TRACE(HI_STB_LOG_LEVEL_WARNING, TUNERSVR,"  locking status: TunerConnectTuner fail\n");  
									}
									
									s_enTunerCurState = TUNER_LOCKING;
									break;
								}
							case  MESSAGE_TUNERSVR_STOP:
								{
									TunerTunerCallBack( MESSAGE_TUNERSVR_DROP, u16DeviceID, u16FrontEndID);

									s_u32TunerTimerCount = 0;
									s_enTunerCurState = TUNER_IDLE;
									
									break;
								}


							case MESSAGE_TUNERSVR_CHECK_TIMER:
								{
									s32Ret = TunerLockCheck();
									
									if ( HI_SUCCESS != s32Ret )
									{
										TunerTunerCallBack(MESSAGE_TUNERSVR_LOST, u16DeviceID,  u16FrontEndID);

										s_enTunerCurState = TUNER_LOCKING;
									}

									break;
								}
							default:
								{
									break;
								}
						}
						default:
						{
							break;
						}
					}
			}
		}

	}
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerCreateTask
 * Description:  creat tunersvr task and tunersvr timer task
 * Data Accessed:None
 * Data Updated:  s_pTunerTimerHandle, s_pTunerMsgQueue
 s_u32TunerTimerCount, s_enTunerCurState
 * Input:             None
 * Output:           None
 * Return:          tunersvr task id 
 * Others:          None
 *****************************************************************************/
pthread_t  TUNERSVR_TunerCreateTask(HI_VOID )
{
	HI_S32 s32Ret = HI_FAILURE;
	HI_S32 s32TunerTaskRT  = -1;


//	SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, "SVR_TunerCreateTask\n\n");

	if( HI_NULL!= s_pTunerTaskHandle )
	{
		return s_pTunerTaskHandle;
	}

	/*create the tunersvr message Queue*/
	s_pTunerMsgQueue =  HI_OSMsgQueueCreate(sizeof(MSG_MESSAGE_S), 1000);
	if (HI_NULL == s_pTunerMsgQueue)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner task create failed!\n");

		return HI_NULL;
	}

	/*create the task only once in the appmngsvr module */
	SVR_TRACE(HI_STB_LOG_LEVEL_INFO,TUNERSVR, " tuner task create!\n");

	s32TunerTaskRT = pthread_create(&s_pTunerTaskHandle, HI_NULL, (HI_VOID *)&TunerTaskProc, HI_NULL);  
	if (HI_NULL == s_pTunerTaskHandle || HI_NULL != s32TunerTaskRT)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner task create failed!\n");
		
		HI_OSMsgQueueDel(s_pTunerMsgQueue);
		return HI_NULL;
	}

	SVR_TRACE(HI_STB_LOG_LEVEL_INFO,TUNERSVR, " start TunerStartTimer\n");
#if 0	
	s32Ret = TunerStartTimer();
	if ( HI_FAILURE == s32Ret )
	{   
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " TunerStartTimer failed!\n");

		HI_OSMsgQueueDel(s_pTunerMsgQueue);
		pthread_cancel(s_pTunerTaskHandle);
		s_pTunerTaskHandle = HI_NULL;
		return HI_NULL;
	}
#endif
	s_enTunerCurState = TUNER_IDLE;/*initial tunersvr status to idle*/

	return (s_pTunerTaskHandle);
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerDestroyTask
 * Description:   destory tunersvr task and tunersvr timer
 * Data Accessed:g_pTimerSyncBlk, s_pTunerTimerHandle
 * Data Updated: None
 * Input:             None
 * Output:           None
 * Return:          HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/
HI_S32   TUNERSVR_TunerDestroyTask ( HI_VOID)
{
	HI_S32 s32Ret = HI_FAILURE;

	SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, "SVR_TunerDestroyTask\n");

	/* debug */
	//stb_petimer_free_reltimer(s_pTunerTimerHandle );
	
	HI_OSMsgQueueDel(s_pTunerMsgQueue);/*del tunersvr msg queue*/
	s_pTunerMsgQueue = HI_NULL;

	pthread_cancel(s_pTunerTaskHandle);/*destory tunersvr task*/
	
	s_pTunerTaskHandle = HI_NULL;
	s_enTunerCurState = TUNER_IDLE; /*initial tunersvr status to idle*/

	return HI_SUCCESS;
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerIsReady
 * Description:   tunersvr task ready or not
 * Data Accessed:s_pTunerTaskHandle
 * Data Updated: None
 * Input:             None
 * Output:           None
 * Return:           HI_TRUE - ready, HI_FALSE - not ready
 * Others:           None
 *****************************************************************************/
HI_BOOL TUNERSVR_TunerTaskReady(HI_VOID)
{
	if( HI_NULL != s_pTunerTaskHandle )
	{
		return HI_TRUE;
	}
	else
	{
		return HI_FALSE;
	}
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerRegisterCallBack
 * Description:   registe tunersvr call back , this call back be resigted by appmngsvr
 * Data Accessed: s_pTunerCallback 
 * Data Updated:  None
 * Input:             None
 * Output:           None
 * Return:           HI_SUCCESS, HI_FAILURE
 * Others:      None
 *****************************************************************************/
HI_S32 TUNERSVR_TunerRegisterCallBack( T_TunerCallback pFunCallBack )
{
	SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, "SVR_TunerRegisterCallBack\n");

	if (HI_NULL ==  pFunCallBack)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " tuner callback function null\n");
		return  HI_FAILURE;
	}

	/* 进行回调赋值*/
	s_pTunerCallback = pFunCallBack;
	return HI_SUCCESS;
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerStartLock
 * Description:   send lock msg to tunersvr, by called appmngsvr; if the param is invalidate, the send unlock send msg to appmngsvr 
 * Data Accessed:None
 * Data Updated: None
 * Input:             None
 * Output:           None
 * Return:  HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/
HI_S32 TUNERSVR_TunerStartLock(HI_U16 FrontEndDeviceID,  HI_U16 FrontEndID, DB_FRONTEND_S *pTunerPara)
{
	HI_S32 s32Ret = HI_FAILURE;
	
	MSG_MESSAGE_S  struTunerLockMsg;

	SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, "HI_SVR_TunerStartLock\n");
	
	if (HI_NULL ==  pTunerPara)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " input pointer is NULL\n");
		return ( HI_FAILURE);
	}


	struTunerLockMsg.u32MessageID = MESSAGE_TUNERSVR_ID;
	struTunerLockMsg.s32Para1 = (HI_S32)FrontEndDeviceID;
	struTunerLockMsg.s32Para2 = (HI_S32)FrontEndID;
	struTunerLockMsg.s32Para3 = 0;

	
	s32Ret = TunerSendMsg( &struTunerLockMsg );
	if ( HI_SUCCESS != s32Ret )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " TunerSendMsg error\n");
		return(HI_FAILURE);
	}


	/*send tuner start lock signal msg to tunersvr task to lock */
	struTunerLockMsg.u32MessageID = MESSAGE_TUNERSVR_START;
	struTunerLockMsg.s32Para1 = (HI_S32)pTunerPara->u32Frequency;
	struTunerLockMsg.s32Para2 = (HI_S32)pTunerPara->u32SymbolRate;
	struTunerLockMsg.s32Para3 = (HI_S32)pTunerPara->u8Modulation;

	printf("\n +++ TUNERSVR_TunerStartLock %d  %d  %d\n", struTunerLockMsg.s32Para1, struTunerLockMsg.s32Para2, struTunerLockMsg.s32Para3);
	s32Ret = TunerSendMsg( &struTunerLockMsg );
	
	if ( HI_SUCCESS != s32Ret )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " TunerSendMsg error\n");
		return(HI_FAILURE);
	}

	return (HI_SUCCESS);
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerStopLock
 * Description:   stop tunersvr lock fre: send stop msg to tunersvr 
 * Data Accessed:   None
 * Data Updated:    None 
 * Input:             None
 * Output:           None
 * Return:  HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/
HI_VOID  TUNERSVR_TunerStopLock(HI_U16 FrontEndDeviceID,  HI_U16 FrontEndID )
{
	HI_S32 s32Ret = HI_FAILURE;
	
	MSG_MESSAGE_S  struTunerLockMsg;

	SVR_TRACE(HI_STB_LOG_LEVEL_INFO, TUNERSVR, "HI_SVR_TunerStartLock\n");
	
	/*send tuner start lock signal msg to tunersvr task to lock */
	struTunerLockMsg.u32MessageID = MESSAGE_TUNERSVR_STOP;
	struTunerLockMsg.s32Para1 = 0;
	struTunerLockMsg.s32Para2 = 0;
	struTunerLockMsg.s32Para3 = 0;

	s32Ret = TunerSendMsg(&struTunerLockMsg);
	
	if ( HI_SUCCESS != s32Ret )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " TunerSendMsg error\n");
		return;
	}

	struTunerLockMsg.u32MessageID = MESSAGE_TUNERSVR_ID;
	struTunerLockMsg.s32Para1 = (HI_S32)FrontEndDeviceID;
	struTunerLockMsg.s32Para2 = (HI_S32)FrontEndID;
	struTunerLockMsg.s32Para3 = 0;

	s32Ret = TunerSendMsg( &struTunerLockMsg );
	if ( HI_SUCCESS != s32Ret )
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " TunerSendMsg error\n");
		return;
	}

	printf("\n +++ TUNERSVR_TunerStopLock +++\n");

	return;
}

/*****************************************************************************
 * Function:      TUNERSVR_TunerSignal
 * Description:   give tuner property: signal strength, ber, snr
 * Data Accessed:None
 * Data Updated: None
 * Input:             None
 * Output:           tuner property
 * Return:      HI_SUCCESS, HI_FAILURE
 * Others:     None
 *****************************************************************************/
HI_S32 TUNERSVR_TunerSignal(Tuner_Propety_S *pinfo)
{
	if (HI_NULL == pinfo)
	{
		SVR_TRACE(HI_STB_LOG_LEVEL_ERROR, TUNERSVR, " Tuner_Propety_S null\n");
		return(HI_FAILURE);
	}

	memcpy(pinfo, &s_stTunerPropety, sizeof(Tuner_Propety_S));
	return (HI_SUCCESS);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


