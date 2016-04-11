/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/10/14                                           	  */
/******************************************************************************/
#include <stdio.h>
#include "hi_type.h"
#include "hi_unf_demux.h"
#include "fyf_def.h"
#include "sectionsvr_ext.h"
#include "eBrowsersvr.h"
#include "fyf_demux.h"
#include "fyf_descr.h"

extern HI_U32 video_channel;
extern HI_U32 audio_channel;
extern HI_U32 pcr_channel;

extern HI_U16 video_pid;
extern HI_U16 audio_pid;
extern HI_U16 pcr_pid;

HI_BOOL descr_init_audio_flag = HI_FALSE;
HI_BOOL descr_init_video_flag = HI_FALSE;

static BU32 audio_key[3];
static BU32 video_key[3];

BU32 FYF_API_descr_GetAudioDescrambler(BU32 demux_id)
{
	return audio_key[demux_id];
}

BU32 FYF_API_descr_GetVideoDescrambler(BU32 demux_id)
{
	return video_key[demux_id];
}


/******************************************************************************/
/*Description: descramble open                                                */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_descr_open(void)
{
		BU32 key;
		BS32 ret;
		BU32 audiochannel,videochannel;
		BU08 CA_CW[8];
		HI_UNF_DMX_CHAN_STATUS_S chanstatus;
		BU32 i;
		
		if(descr_init_audio_flag == HI_FALSE)
		{	
			for (i = 0; i < 2; i++)
			{
				ret = HI_UNF_DMX_NewDescrambler(i, &audio_key[i]);
				if(ret != HI_SUCCESS)
				{
					printf(" +++ FYF_CPI_descr_open audio error %x\n",ret);
					return FYF_ERR;
				}	
				printf(" +++ FYF_CPI_descr_open get audio key %d \n",audio_key[i]);
				FYF_API_Memset (CA_CW, 0, 8);
				ret |= HI_UNF_DMX_SetDescramblerEvenKey(audio_key[i], CA_CW);
				ret |= HI_UNF_DMX_SetDescramblerOddKey(audio_key[i],  CA_CW);
				if (i == 0)
				{
					ret |= HI_UNF_DMX_GetAudioChannelHandle(0,&audiochannel);
					ret |= HI_UNF_DMX_GetChannelStatus(audiochannel,&chanstatus);
					if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
						ret |= HI_UNF_DMX_CloseChannel(audiochannel);
					ret |= HI_UNF_DMX_AttachDescrambler(audio_key[i],audiochannel);
					//if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
					ret |= HI_UNF_DMX_OpenChannel(audiochannel);
					if(ret != HI_SUCCESS)
					{
						printf(" +++ attach audio key error %x\n",ret);
						return FYF_ERR;
					}	
				}
				printf(" +++ attack audio chanel \n");			
				
			}
			descr_init_audio_flag = HI_TRUE;
		
		}
		
		if(descr_init_video_flag == HI_FALSE)
		{	
			for (i = 0; i < 2; i++)
			{
				ret = HI_UNF_DMX_NewDescrambler(i, &video_key[i]);
				if(ret != HI_SUCCESS)
				{
					printf(" +++ FYF_CPI_descr_open video error %x\n",ret);
					return FYF_ERR;
				}	
				printf(" +++ FYF_CPI_descr_open get video key %d \n",video_key[i]);
				FYF_API_Memset (CA_CW, 0, 8);
				ret |= HI_UNF_DMX_SetDescramblerEvenKey(video_key[i], CA_CW);
				ret |= HI_UNF_DMX_SetDescramblerOddKey(video_key[i],  CA_CW);
				if (i == 0)
				{
					ret = HI_UNF_DMX_GetVideoChannelHandle(0,&videochannel);
					ret |= HI_UNF_DMX_GetChannelStatus(videochannel,&chanstatus);
					if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
						ret |= HI_UNF_DMX_CloseChannel(videochannel);
					ret |= HI_UNF_DMX_AttachDescrambler(video_key[i],videochannel);
					//if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
					ret |= HI_UNF_DMX_OpenChannel(videochannel);
					if(ret != HI_SUCCESS)
					{
						printf(" +++ attach video key error %x\n",ret);
						return FYF_ERR;
					}	
				}
				printf(" +++ attack video chanel \n");
			}
			
			descr_init_video_flag = HI_TRUE;
		}

		return FYF_OK;
}
/******************************************************************************/
/*Description: associate pid                                                  */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_descr_associate_pid(BU32 handle,BU32 pid)
{
	BS32 ret= 0;
	HI_UNF_DMX_CHAN_STATUS_S chanstatus;
	return FYF_OK;
	
	printf(" +++ FYF_CPI_descr_associate_pid set handle %d to pid %x\n",handle,pid);
	if(video_pid == pid)
	{
		ret = HI_UNF_DMX_GetChannelStatus(video_channel,&chanstatus);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_CloseChannel(video_channel);
		ret |= HI_UNF_DMX_AttachDescrambler(handle,video_channel);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_OpenChannel(video_channel);
		printf(" +++ FYF_CPI_descr_associate_pid attack video chanel \n");
	}	
	else if(audio_pid == pid)
	{
		ret = HI_UNF_DMX_GetChannelStatus(audio_channel,&chanstatus);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_CloseChannel(audio_channel);
		ret |= HI_UNF_DMX_AttachDescrambler(handle,audio_channel);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_OpenChannel(audio_channel);
		printf(" +++ FYF_CPI_descr_associate_pid attack audio chanel \n");
	}	
	else
	{
		printf(" +++ FYF_CPI_descr_associate_pid cant find pid %x \n",pid);
	}	
	if(ret != HI_SUCCESS)
	{
		printf(" +++ FYF_CPI_descr_associate_pid error %x\n",ret);
		return FYF_ERR;
	}	
	return FYF_OK;
}
/******************************************************************************/
/*Description: descramble close                                               */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_descr_close(BU32 handle)
{		
	BS32 ret;
	return FYF_OK;
	
	ret = HI_UNF_DMX_DeleteDescrambler(handle);
	if(ret != HI_SUCCESS)
	{
		printf(" +++ FYF_CPI_descr_close handle %d error %x\n",handle,ret);
		return FYF_ERR;
	}	
	printf(" +++ FYF_CPI_descr_open close handle %x\n",handle);
	return FYF_OK;
}
/******************************************************************************/
/*Description: disassociate pid                                               */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_descr_disassociate_pid(BU32 handle,BU32 pid)
{
	BS32 ret= 0;
	HI_UNF_DMX_CHAN_STATUS_S chanstatus;
	return FYF_OK;
	
	if(video_pid == pid)
	{
		ret = HI_UNF_DMX_GetChannelStatus(video_channel,&chanstatus);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_CloseChannel(video_channel);
		ret |= HI_UNF_DMX_DetachDescrambler(handle,video_channel);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_OpenChannel(video_channel);
		printf(" +++ FYF_CPI_descr_associate_pid detach video chanel \n");
	}	
	else if(audio_pid == pid)
	{
		ret = HI_UNF_DMX_GetChannelStatus(audio_channel,&chanstatus);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_CloseChannel(audio_channel);
		ret |= HI_UNF_DMX_DetachDescrambler(handle,audio_channel);
		if(chanstatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			ret |= HI_UNF_DMX_OpenChannel(audio_channel);
		printf(" +++ FYF_CPI_descr_associate_pid detach audio chanel \n");
	}	
	else
	{
		printf(" +++ FYF_CPI_descr_associate_pid cant find  chanel pid %x \n",pid);
	}	

	if(ret != HI_SUCCESS)
	{
		printf(" +++ FYF_CPI_descr_disassociate_pid error %x\n",ret);
		return FYF_ERR;
	}	
	return FYF_OK;
}
/******************************************************************************/
/*Description: descramble set                                                 */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_descr_set(BU32 handle, FYF_DESCR_KeyParity_t Parity, FYF_DESCR_KeyUsage_t Usage, BU08 *Data_p)
{	
	BS32 ret;

	if(Data_p == FYF_NULL)
		return FYF_ERR;

	//printf(" +++ FYF_CPI_descr_set set handle %d Parity %d  \n",handle,Parity);

	//printf(" 0x%x 0x%x 0x%x 0x%x   \n",Data_p[0],Data_p[1],Data_p[2],Data_p[3]);
	//printf(" 0x%x 0x%x 0x%x 0x%x   \n",Data_p[4],Data_p[5],Data_p[6],Data_p[7]);

	
	if(Parity == FYF_DESCR_KEY_PARITY_EVEN_PARITY)
		ret = HI_UNF_DMX_SetDescramblerEvenKey(handle, Data_p);
	else if(Parity == FYF_DESCR_KEY_PARITY_ODD_PARITY)
		ret = HI_UNF_DMX_SetDescramblerOddKey(handle,  Data_p);
	else
	{
		ret = HI_UNF_DMX_SetDescramblerEvenKey(handle, Data_p);
		ret |= HI_UNF_DMX_SetDescramblerOddKey(handle, Data_p);
	}	

	if(ret != HI_SUCCESS)
	{
		printf(" +++ FYF_CPI_descr_set error %x\n",ret);
		return FYF_ERR;
	}	
	return FYF_OK;

}


static FYF_DESCR_PORTING FYF_DESCR_porting = 
{
	FYF_CPI_descr_open,
	FYF_CPI_descr_associate_pid,
	FYF_CPI_descr_close,
	FYF_CPI_descr_disassociate_pid,
	FYF_CPI_descr_set
};

void FYF_API_DESCR_init(void)
{
	FYF_API_descr_register(&FYF_DESCR_porting);
}
