/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author libilong 2009/08/03                                             */
/******************************************************************************/
#include <pthread.h>
#include <stdlib.h>
#include "hi_stb_api.h"
#include "hi_type.h"
#include "fyf_com.h"

#define __FYF_SMC_PORTING_
#ifdef  __FYF_SMC_PORTING_
#include "dev/fyf_smc.h"
#include <assert.h>
#include <stdio.h>
#define DEBUG_SCI //printf
#define SCI_SLOT_0              0       /* 只用0号智能卡座 */
#define SCI_PROTOCAL_T0         0      //use T0 protocol
#define SCI_RETRY_TIMES         3 
#define SCI_TIMEOUT             2000
#define SCI_ResetValid          1       //1 //1 hard reset, 0 soft reset
#define SCI_MAX_RESP_LENGTH     256     /* max response length */
#define SCI_MAX_ATR_LEN         32      /* max atr length */
#define SCI_CMD_HEAD_LENGTH     5
#define SCI_CLA_OFFSET          0
#define SCI_INS_OFFSET          1
#if defined(SCI_PROTOCAL_T1)
#define SCI_P1_OFFSET           2
#define SCI_P2_OFFSET           3
#endif
#define SCI_P3_OFFSET           4
#define SCI_DBG
#ifdef SCI_DBG
#define SCI_DBG_PRINTF          DEBUG_SCI
#define SCI_DBG_DUMP(data,len)                                                  \
    {                                                                           \
        const BS32 l = (len);                                                 \
        BS32 i;                                                               \
        for ( i = 0 ; i < l ; i++ )                                             \
        {                                                                       \
            SCI_DBG_PRINTF(" %02x",*((char *)(void *)(data)+i));          \
        }                                                                       \
        SCI_DBG_PRINTF("\n");                                                   \
    }
#else
#define SCI_DBG_PRINTF(fmt...)
#define SCI_DBG_DUMP(data,len)
#endif
/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
#if 0
static void (*pfCardHandle)(BU32 param);
static int CardState = 0;
static void pfCardInterrupt()
{
	 static HI_BOOL bCardInLast = HI_FALSE;

	while (1)
	{
		usleep(20 * 1000);
		HI_U32 u32CardStatus;
	    HI_BOOL bCardIn;
	   
	    if(HI_SUCCESS != HI_UNF_SCI_GetCardStatus(SCI_SLOT_0, &u32CardStatus))
	    {
	        printf("get card status error\n");
	        continue;
	    }
	    
	    if((0 == u32CardStatus) || (1 == u32CardStatus))
	    {
	        bCardIn = HI_FALSE;
	    }
	    else
	    {
	        bCardIn = HI_TRUE;   
	        
	    }
	    
	    if (bCardIn != bCardInLast)
	    {
	        bCardInLast = bCardIn;
	        if (HI_TRUE == bCardIn)
	        {
	            printf("smc insert!\n");

	            pfCardHandle(1);
	        }
	        else
	        {
	            printf("smc removed!\n");
	            pfCardHandle(0);
	        }
	    } 	
	}
}
#endif
BS32 FYF_CPI_smc_open_porting(void)
{
	BS32 Ret=HI_FAILURE;
	//pfCardHandle=callback;

	Ret = HI_UNF_SCI_Open(SCI_SLOT_0, SCI_PROTOCAL_T0,4000);
    if (HI_SUCCESS != Ret)
    {
    	printf("HI_UNF_SCI_Open error 0x%x", Ret);
        return FYF_ERR;
    }
	
#ifdef _HAISI_DEMO_
	HI_UNF_SCI_CLK_MODE_E enClkMode = HI_UNF_SCI_CLK_MODE_OD;
#else
	HI_UNF_SCI_CLK_MODE_E enClkMode = HI_UNF_SCI_CLK_MODE_OD;
#endif

	BU32 Level=0;
	BU32 Detect=1;
	BU32 SCIStatus;

	 /*设置SCI时钟工作模式*/
    Ret = HI_UNF_SCI_ConfigClkMode(SCI_SLOT_0, enClkMode);
    if (HI_SUCCESS != Ret)
    {
       printf("HI_UNF_SCI_ConfigClkMode error 0x%x", Ret);
       HI_UNF_SCI_Close();
	   return FYF_ERR;
    }
     /*设置SCI检测方式，需要根据单板修改*/
	Ret = HI_UNF_SCI_ConfigVccEn(SCI_SLOT_0, Level);
    if (HI_SUCCESS != Ret)
    {
    	printf("HI_UNF_SCI_ConfigVccEn error 0x%x", Ret);
        HI_UNF_SCI_Close();
		return FYF_ERR;
    }

	Ret = HI_UNF_SCI_ConfigDetect(SCI_SLOT_0, Detect);
    if (HI_SUCCESS != Ret)
    {
    	printf("HI_UNF_SCI_ConfigVccEn error 0x%x", Ret);
        HI_UNF_SCI_Close();
		return FYF_ERR;
    }
    /* sleep for a while for wait card init ok */
    (HI_VOID)usleep(10000);
	#if 0
	 /*获取SCI卡状态*/
        Ret = HI_UNF_SCI_GetCardStatus(SCI_SLOT_0, &SCIStatus);
        if (HI_SUCCESS != Ret)
        {
            HI_UNF_SCI_Close();
			return FYF_ERR;
        }
	#endif
#if 0
	pthread_t	thread;
	BU32 ret;
	ret = pthread_create(&thread, NULL, (void *)pfCardInterrupt, NULL);
	
	if (ret != 0)
	{
		printf("thread pfCardInterrupt create fail!\n");
		HI_UNF_SCI_Close();
		return FYF_ERR;
	}

#endif	
    return FYF_OK;
}
/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BS32 FYF_CPI_smc_card_exist_porting(void)
{
    BS32 Ret=HI_FAILURE;
	BU32 SCIStatus;
	Ret = HI_UNF_SCI_GetCardStatus(SCI_SLOT_0, &SCIStatus);
    if (HI_SUCCESS != Ret)
    {
	    return FYF_ERR;
    }
	else
    {
        if (SCIStatus <= 2)
        {
            return FYF_ERR;
        }
        return FYF_OK;
    }
}
/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BS32 FYF_CPI_smc_reset_porting(BU08 *buffer, BU16*atr_size)
{

	BS32 s32Return = FYF_ERR;  
    BS32 s32Retry = 30;    /* 最多重试次数 */
    BU32 u32CardStatus;
    BU08 u8ATRCount;

    s32Return = HI_UNF_SCI_ResetCard(SCI_SLOT_0, SCI_ResetValid);
    if(HI_SUCCESS != s32Return)
    {
    	printf("HI_UNF_SCI_ResetCard error 0x%x", s32Return);
        return FYF_ERR;
    }

    do
    {
        (void)usleep(10000); 
		//FYF_API_thread_sleep(10);
        (void)HI_UNF_SCI_GetCardStatus(SCI_SLOT_0, &u32CardStatus);
    } while ((u32CardStatus == 3) && (--s32Retry > 0));

    if (u32CardStatus <= 3)
    {
        return FYF_ERR;
    }

	usleep(1000000);
    s32Return = HI_UNF_SCI_GetATR(SCI_SLOT_0, buffer, 256, &u8ATRCount);
    if(HI_SUCCESS != s32Return)
    {
    	printf("HI_UNF_SCI_GetATR error 0x%x", s32Return);
        return FYF_ERR;
    }

    *atr_size = u8ATRCount;
    return FYF_OK;
	
}
/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BS32 FYF_CPI_smc_deactive_porting(void)
{
	DEBUG_SCI("FYF_CPI_smc_deactive_porting\n");
    BS32 Ret=HI_FAILURE;
	Ret = HI_UNF_SCI_DeactiveCard(SCI_SLOT_0);
    if (HI_SUCCESS != Ret)
    {
        return FYF_ERR;
    }
	return FYF_OK;
}
/*******************************************************************************
* Function:     FYF_IPI_SCI_Send
* Description:
* Input:
* Output:
* Return:   HI_SUCCESS: success,成功
*           非HI_SUCCESS:失败
********************************************************************************/
static BS32 FYF_IPI_SCI_Send(BU08 u8SCIPortNo, BU08 *pSCISendBuf, BU32 u32SendLength)
{
	DEBUG_SCI("FYF_IPI_SCI_Send\n");
    	BS32 s32Return = HI_FAILURE;
    	BS32 s32Retry;
    	BU08 *pu8Buf = pSCISendBuf; 
    	BU32 u32Actual = 0,u32BufLength = u32SendLength; 


   	assert(NULL != pSCISendBuf);

    //retry times when transmit failed should adjust according to 
    //the smart card provider's desire.
    for (s32Retry = 0; s32Retry < SCI_RETRY_TIMES; ++s32Retry)
    {
        s32Return = HI_UNF_SCI_Send(u8SCIPortNo, pu8Buf, u32BufLength, &u32Actual, SCI_TIMEOUT);
        if(HI_SUCCESS == s32Return)
        {  
            	return HI_SUCCESS;
        }   
        DEBUG_SCI("sci send error, returned:0x%x!\n", s32Return);   
        if (u32Actual > u32BufLength) 
        { 
		DEBUG_SCI("sci send over flow!\n");
            	return HI_FAILURE; 
        } 

        pu8Buf += u32Actual; 
        u32BufLength -= u32Actual; 
    }    
    	return HI_FAILURE;
    
}

/*******************************************************************************
* Function:     FYF_IPI_SCI_Receive
* Description:
* Input:
* Output:
* Return:   HI_SUCCESS: success,成功
*           非HI_SUCCESS:失败
********************************************************************************/
static BS32 FYF_IPI_SCI_Receive (BU08 u8SCIPortNo, BU08 *pSCIReceiveBuf, 
                           BU32 u32ReceiveLength, BU32 *pu32ActualLength)
{
	DEBUG_SCI("FYF_IPI_SCI_Receive\n");
    BS32 s32Return = HI_FAILURE;
    BU08 *pu8Buf = pSCIReceiveBuf;
    BU32 u32BufLength = u32ReceiveLength;
    BS32 s32Retry;

    assert(NULL != pSCIReceiveBuf);
    assert(NULL != pu32ActualLength);    

    memset(pSCIReceiveBuf, 0, u32ReceiveLength);
    *pu32ActualLength = 0;

    //retry times when read failed should adjust according to 
    //the smart card provider's desire.
    for (s32Retry = 0; s32Retry < SCI_RETRY_TIMES && u32BufLength > 0; ++s32Retry)
    {
        BU32 u32Actual = 0;
        s32Return = HI_UNF_SCI_Receive (u8SCIPortNo, pu8Buf, u32BufLength, &u32Actual, SCI_TIMEOUT);
        *pu32ActualLength += u32Actual;

        if (HI_SUCCESS == s32Return)
        { 
            	return HI_SUCCESS;
        }
        
        DEBUG_SCI("sci receive error, returned:0x%x!\n", s32Return);
        SCI_DBG_DUMP(pu8Buf - u32Actual, *pu32ActualLength);

        if (*pu32ActualLength > SCI_MAX_RESP_LENGTH)
        {
            	DEBUG_SCI("sci receive over flow!\n");
            	return HI_FAILURE;
        }
        pu8Buf += u32Actual;
        u32BufLength -= u32Actual;
    }
    
    	return HI_FAILURE;
}

/*******************************************************************************
* Function:     FYF_IPI_SCI_ProcessProcedureBytes
* Description:  read and process procedure byte
* Input:
* Output:
            pu32TransNum ---transfer num for next action
            pu8SWBuffer  ---SW bytes when *pu32TransNum==0, means transfer complete
* Return:   HI_SUCCESS: success,成功
*           非HI_SUCCESS:失败
********************************************************************************/
static BS32 FYF_IPI_SCI_ProcessProcedureBytes(BU08 u8Ins, 
                                    BU32 *pu32TransNum, 
                                    BU08 *pu8SWBuffer)
{
    BS32 s32Return;
    BU08 u8Buff;
    BU32 u32Len, u32TransNum = 0;

    do
    {
        s32Return = FYF_IPI_SCI_Receive( SCI_SLOT_0, &u8Buff, 1, &u32Len);
        if ((s32Return != HI_SUCCESS) || (u32Len != 1))
        {
            DEBUG_SCI("sci read error!\n");
            return s32Return;
        }

        /*NULL : (=0x60) This byte is sent by the card to restart the working
          time, end to anticipate a subsequent procedure byte. It requests no
          further action neither on VPP nor on Data*/
        if (u8Buff == 0x60)
        {
            continue;
        }

        /*SW1 (= 0x6x or 0x9x, expect 0x60); waits for a SW2 byte to complete 
          the command. We do not deal with SW, just pass them to the up layer*/
        if((u8Buff&0xF0)==0x60 || (u8Buff&0xF0)==0x90)
        {
            pu8SWBuffer[0]=u8Buff;
            s32Return = FYF_IPI_SCI_Receive( SCI_SLOT_0, &u8Buff, 1, &u32Len);
            if ((s32Return != HI_SUCCESS) || (u32Len != 1))
            {
                DEBUG_SCI("sci read error!\n");
                return s32Return;
            }
            
            pu8SWBuffer[1] = u8Buff;
            *pu32TransNum = 0;
            SCI_DBG_PRINTF("SW: ");
            SCI_DBG_DUMP(pu8SWBuffer, 2);
        
            return HI_SUCCESS;
        }
        
        SCI_DBG_PRINTF("R: ");
        SCI_DBG_DUMP(&u8Buff, 1);

        /*notes: VPP active means programming the card, our chip don't 
          supplied this function*/
        if((u8Buff ^ u8Ins) == 0) /* ACK == INS*/
        {
            /*Vpp is idle. All remaining bytes are transfered subsequently.*/
            u32TransNum = *pu32TransNum;
        }
        else if((u8Buff ^ u8Ins) == 0xFF) /* ACK == ~INS*/
        {
            /*Vpp is idle. Next byte is transfered subsequently.*/
            u32TransNum = (*pu32TransNum == 0)? 0 : 1;
        }
        else if((u8Buff ^ u8Ins) == 0x01) /* ACK == INS+1*/
        {
            /*Vpp is active. All remaining bytes are transfered subsequently.*/
            u32TransNum = *pu32TransNum;
        }
        else if((u8Buff ^ u8Ins) == 0xFE) /* ACK == ~(INS+1)*/
        {
            /*Vpp is active. Next bytes is transfered subsequently.*/
            u32TransNum = (*pu32TransNum == 0)? 0 : 1;
        }
        else
        {
            DEBUG_SCI("cannot handle procedure %02x (INS=%02x)\n", u8Buff, u8Ins);
            return HI_FAILURE;
        }
    }while (u32TransNum == 0);

    *pu32TransNum = u32TransNum;
    return HI_SUCCESS;
}
/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BS32 FYF_CPI_smc_iso_transfer_porting(BU08* command, BU16 num_to_write, BU08* response, BU16 num_to_read, BU16* actual_size)
{
	
	DEBUG_SCI("FYF_CPI_smc_iso_transfer_porting\n");
	BU08 bWrite;
    BU32 u32TransLen, u32NextTransLen, u32ActualLen;
    BU08 au8SWBuff[2], au8Buff[SCI_MAX_RESP_LENGTH];
    BS32 s32Return;
    BU08 u8Ins;
    
    if ((command[SCI_CLA_OFFSET] == 0xFF) ||
       ((command[SCI_INS_OFFSET] & 0xF0) == 0x60) ||
       ((command[SCI_INS_OFFSET] & 0xF0) == 0x90) )
    {
        DEBUG_SCI("unsupported command\n");
        return HI_FAILURE;
    }

    u32TransLen = command[SCI_P3_OFFSET];  
    
    if (num_to_write > SCI_CMD_HEAD_LENGTH)
    {
        //write to sci card
        bWrite = 1;
    }
    else
    {
        //read from sci card
        bWrite = 0;
        if (u32TransLen == 0)
        {
            //according to the iso7816 protocol
            u32TransLen = 256;
        }
    }
    
    u8Ins = command[SCI_INS_OFFSET];
    u32TransLen += SCI_CMD_HEAD_LENGTH;
    u32NextTransLen = SCI_CMD_HEAD_LENGTH;  //send cmd header frist
    *actual_size = 0;
    do
    {        
        SCI_DBG_PRINTF("W: ");
        SCI_DBG_DUMP(command, u32NextTransLen);
        s32Return = FYF_IPI_SCI_Send( SCI_SLOT_0, (BU08*)command, u32NextTransLen);
        if (s32Return != HI_SUCCESS)
        {
            DEBUG_SCI("sci send error!\n");
            return HI_FAILURE;
        }
        
        command += u32NextTransLen;
        u32TransLen -= u32NextTransLen;        
        u32NextTransLen = u32TransLen;

        s32Return = FYF_IPI_SCI_ProcessProcedureBytes(u8Ins, &u32NextTransLen, au8SWBuff);
        if (s32Return != HI_SUCCESS)
        {
            return s32Return;
        }
                
        if (u32NextTransLen == 0)
        {
            FYF_API_Memcpy( response, au8SWBuff, 2);
            *actual_size = 2;

            return HI_SUCCESS;
        }
    } while(bWrite);


    do
    {
        s32Return = FYF_IPI_SCI_Receive( SCI_SLOT_0, au8Buff, u32NextTransLen, &u32ActualLen);
        //for u32NextTransLen was readed from the card by SCI_ProcessProcedureBytes
        //function, u32ActualLen must equal to u32NextTransLen, otherwise 
        //there is an error we can't handle.
        if ((s32Return != HI_SUCCESS) || (u32ActualLen != u32NextTransLen))
        {
            DEBUG_SCI("sci read error!\n");
            return HI_FAILURE;
        }
        SCI_DBG_PRINTF("R: ");
        SCI_DBG_DUMP(au8Buff, u32ActualLen);
        
        FYF_API_Memcpy( (response + *actual_size) , au8Buff, u32ActualLen);
        *actual_size += u32ActualLen;

        u32TransLen -= u32NextTransLen;        
        u32NextTransLen = u32TransLen;

        s32Return = FYF_IPI_SCI_ProcessProcedureBytes(u8Ins, &u32NextTransLen, au8SWBuff);
        if (s32Return != HI_SUCCESS)
        {
            return s32Return;
        }
        
        if (u32NextTransLen == 0)
        {
            if (u32TransLen == 0)
            {
                FYF_API_Memcpy( (response + *actual_size) , au8SWBuff, 2);
                *actual_size += 2;

                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        
    } while(u32TransLen > 0);

    return HI_FAILURE;
	
}
/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
FYF_SMC_PORTING FYF_smc_porting=
{
	FYF_CPI_smc_open_porting,
	FYF_CPI_smc_card_exist_porting,
	FYF_CPI_smc_reset_porting,
	FYF_CPI_smc_deactive_porting,
	FYF_CPI_smc_iso_transfer_porting
};
#endif

void FYF_API_SMC_Init()
{
	FYF_API_smc_register(&FYF_smc_porting);
}
