/*********************************************************************
  Copyright (c) 2005 Embedded Internet Solutions, Inc
  All rights reserved. You are not allowed to copy or distribute
  the code without permission.
  This is the demo implenment by iPanel MiddleWare. 
  Maybe you should modify it accorrding to Platform.        

  $ver0.0.0.1 $author Huangzhiguo 2005/10/18
 *********************************************************************/

#include "ipanel_sound.h"
#include "ipanel_porting_event.h"
#include "trd_ipanel_init.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOUND_BUF_NUM 2


/*initialize sound. you maybe create a new pthread /task to play data.*/
void ipanel_porting_sound_init(void)
{
    /*我们是否需要给ipanel声音模块分一个设备
    目前只要return就好*/
    
    BS32 i = 0;  

#if 0
    memset(writepara, 0, sizeof(AUD_WRITE_PARA_S)*SOUND_BUF_NUM);
    for(i = 0; i < SOUND_BUF_NUM; i++)
    {
        writepara[i].enStreamType =  AUD_STREAM_PCM;
        writepara[i].enAddrType   = AUD_VIRTUAL_ADDRESS;
        writepara[i].u32TimeOut = 0;
    }
#endif
    return;
}

/*exit from sound, free all resource in function ipanel_porting_sound_init().*/
void ipanel_porting_sound_exit(void)
{
    /*目前只要return*/
	
    //memset(writepara, 0, sizeof(AUD_WRITE_PARA_S)*SOUND_BUF_NUM);
    return;
}


/*get STB sound channel parameters*/
int ipanel_porting_sound_get_info(int *numberofbuf, int *buf_size)
{
    /*此处完全参考sound_demo.c*/
    *numberofbuf = SOUND_BUF_NUM; //NEED_BUF_NUMBER;
//    *buf_size = BUF_SIZE_PER_BLOCK;//h66582
    return 0;
}

#if 0
/*进行采样率的转换*/
static HI_UNF_SAMPLE_RATE_E mp3GetRate( unsigned long samplerate)
{
    if (samplerate < 20*1000)
        return HI_UNF_SAMPLE_RATE_8K;//AUD_SAMPLE_FREQUENCE_16KHZ;
    else if (samplerate < 23*1000)
        return HI_UNF_SAMPLE_RATE_22K;
    else if (samplerate < 30*1000)
        return HI_UNF_SAMPLE_RATE_24K;
    else if (samplerate < 40*1000)
        return HI_UNF_SAMPLE_RATE_32K;
    else if (samplerate < 46*1000)
        return HI_UNF_SAMPLE_RATE_44K;
    else if (samplerate < 50*1000)
        return HI_UNF_SAMPLE_RATE_48K;
    else 
       return HI_UNF_SAMPLE_RATE_UNKNOWN;

}
#endif

/*open sound device, maybe you should stop the running channel.*/
unsigned int ipanel_porting_sound_setup(int format, int samplerate, int bitsperSample, int channels, int optionflag)
{
    BS32 s32Ret = 0;
	#if 0
    HI_UNF_SAMPLE_RATE_E enSampleFrequence = 0;
    enSampleFrequence = mp3GetRate(samplerate);

    //s32Ret = HI_AUD_SetPlayerOriginalSampleFrequency(enSampleFrequence);
    /*if(HI_SUCCESS != s32Ret)
    {
        printf("HI_TRD_AV_SetInputWindow failed in 0x%x\n", s32Ret);
        return HI_FAILURE;
    }*/
    #endif
    return 0;
}


/*play sound data*/
int ipanel_porting_sound_play(unsigned int handle, const char *buffer, int len)
{
    /*指明存放数据的地址*/
    BS32 i=0;
#if 0    
    for(i=0; i<SOUND_BUF_NUM; i++)
    {
        if(0 == writepara[i].pSourceAddr && 0 == writepara[i].u32StreamLen)
            break;
    }
    if(SOUND_BUF_NUM == i)
    {
        return HI_FAILURE;
    }
    writepara[i].pSourceAddr = buffer;
    writepara[i].u32StreamLen = len;
#endif
    return 0;
}

/*poll playing buffer is complete.*/
/*该函数要经常调用，检查是否播放完毕*/
int ipanel_porting_sound_poll(unsigned int event[3])
{

    BS32 i=0;
    BS32 s32Ret = 0;
    BU08 *pu8AddrTmp = 0;
    BU32 u32WriteLen = 0;
    memset(event, 0, 3);
	#if 0
    for(i=0; i < SOUND_BUF_NUM; i++)
    {
        if(0 != writepara[i].u32StreamLen)
        {
            pu8AddrTmp =  writepara[i].pSourceAddr;
            /*不停的向aud模块送数据,送完为止*/
            while(writepara[i].u32StreamLen > 0)
            {
                writepara[i].pSourceAddr = pu8AddrTmp;
                s32Ret = HI_AUD_WriteData(&writepara, &u32WriteLen);
                if(HI_SUCCESS == s32Ret)
                {
                    pu8AddrTmp += u32WriteLen;
                    writepara[i].u32StreamLen -= u32WriteLen;
                }
            }

            /*送完数据，设置event消息*/
            event[0] = 0;
            //event[1] = EIS_AUDIO_PLAY_COMPLETE;
            event[1] = EIS_AUDIO_PLAYBUF_COMPLETE;
            event[2] = writepara[i].pSourceAddr;
            break;
        }
    }
	#endif
	return 0;
}

/*close sound device*/
unsigned int ipanel_porting_sound_open(int format, int samplerate, int bitsperSample, int channels, int optionflag)
{
    BS32 s32Ret = 0;
	#if 0
    s32Ret = HI_AUD_StartPcmPlayer();
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_AUD_StartPcmPlayer failed in 0x%x \n", s32Ret);
        return HI_FAILURE;
    }
	#endif
	
    return 0;
}


/*close sound device*/
int ipanel_porting_sound_close(unsigned int handle)
{
    BS32 s32Ret = 0;
#if 0
    s32Ret = HI_AUD_StopPcmPlayer();
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_AUD_StopPcmPlayer failed in 0x%x \n", s32Ret);
        return HI_FAILURE;
    }
#endif
	
    return 0;    
}

#ifdef __cplusplus
}
#endif

