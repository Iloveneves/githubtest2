/*----------------------------------------------------------------------------
   File Name:ow_demux.c

   Description: 

   Version:REL_V1.0.0

   Copyright (c) 2006-2100 Triumph Corporation.  All rights reserved.
-----------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */

#include "ow_debug.h"

#define FYF_QUICK_OUPUT 1
#define RTP_DEBUG  //printf

#define DEMUX_INVALID_PID 				(0x1fff)
#define	MPEG1_VIDEO_STREAM				(0x01)
#define	MPEG2_VIDEO_STREAM				(0x02)
#define	MPEG1_AUDIO_STREAM				(0x03)
#define	MPEG2_AUDIO_STREAM				(0x04)

#define MPEG4_VIDEO_STREAM	 			(0x10)
#define H264_VIDEO_STREAM    			(0x1b)
#define AC3_AUDIO_STREAM	 			(0x81)
#define AAC_ADTS_AUDIO_STREAM  			(0x0F) 
#define AAC_LATM_AUDIO_STREAM  			(0x11)

//#define USE_HI3110
typedef	union
{
	BU16	sWord16;
	struct
	{
		BU08	ucByte0;	/* LSB */
		BU08    ucByte1; 	/* MSB	*/
	} byte;
} stSHORT2BYTE;

static BU16 u16VideoPid = DEMUX_INVALID_PID;
static BU16 u16AudioPid = DEMUX_INVALID_PID;
static BU16 u16PcrPid 	= DEMUX_INVALID_PID;
BU08 u8AudioType = FYF_ACODEC_TYPE_MP3;
BU08 u8VideoType = FYF_VCODEC_TYPE_MPEG2;
#ifndef USE_HI3110
extern HI_UNF_AVPLAY_HANDLE s_AVPLAYHandle;
#endif
extern BU32 GUI_IpanelState;


static int DMUX_ParseProgDescriptor(const BU08 * const pSectionData);
static void DMUX_ParsePmtSection(BU08 * pSectionData,BU16 * const VidPid,BU16 * const AudPid,BU16 * const PcrPid,BU08 * const AudioType,BU08 * const VideoType);
void OW_DLVodPidInfo_Callback(BU32 ch,BU32 filter,BU32 DataStartAddr,BU32 len);

#ifdef USE_IPANEL
extern 	BS32 TRD_LockTuner(BU32 freq, BU32 sym, BU32 qam);
extern  void DeleteFilter(BU32 ch,BU32 filter);
#endif
extern int audio_rate, video_rate;
BU32 g_PreviewPts = 0;

/* ----------------------------------------------------------------------------
   Name: DMUX_ParseProgDescriptor()

   Description:
   parse descriptor for parse nit/pmt section.

   Parameters:
   pSectionData:				descriptor data buffer

   Return Value: 				descriptor length
   ---------------------------------------------------------------------------- */
static int DMUX_ParseProgDescriptor(const BU08 * const pSectionData)
{
	int iDescriptorLength;
	iDescriptorLength = pSectionData[1];

	/*
	 * return the length field + 2
	 */
	return (iDescriptorLength + 2);
}

/* ----------------------------------------------------------------------------
   Name: DMUX_ParsePmtSection()

   Description:
   Parse pmt section for ow_dmux.

   Parameters:
   pSectionData:           		pmt section data buffer,the first byte is table_id
   VidPid:						the pointer to video pid[OUT]
   AudPid:						the pointer to audio pid[OUT]
   PcrPid:                      the pointer to PCR   pid[OUT]
   
   Return Value:
   ---------------------------------------------------------------------------- */
static void DMUX_ParsePmtSection(BU08 *pSectionData, BU16 * const VidPid, BU16 * const AudPid, BU16 * const PcrPid, BU08 * const AudioType, BU08 * const VideoType)
{
	BU08 *pucSectionData;
	int iNoOfDataBytes;
	int iInfoLength;
	int iEsInfoLength;
	int iNoOfBytesParsedSoFar;
	stSHORT2BYTE stProgramNo;
	stSHORT2BYTE stTempData16;
	BU08 ucLastSectionNumber;
	BU08 ucSectionNumber;
	BU08 ucVersionNumber;
	BU08 ucCurNextIndicator;
	BS16 sSectionLength;
	BU08 ecmType = 0;
	BU08 audio_type = FYF_ACODEC_TYPE_MP3, video_type = FYF_VCODEC_TYPE_MPEG2;

	*VidPid = DEMUX_INVALID_PID;
	*AudPid = DEMUX_INVALID_PID;
	*PcrPid = DEMUX_INVALID_PID;
	*AudioType = audio_type;
	*VideoType = video_type;


	sSectionLength = (pSectionData[1] & 0x0f) << 8 | pSectionData[2];
	sSectionLength -= 1;

	
	stProgramNo.byte.ucByte1 = pSectionData[3];
	stProgramNo.byte.ucByte0 = pSectionData[4];

	ucCurNextIndicator = pSectionData[5] & 0x01;
	if (ucCurNextIndicator != 1)
	{
		return;
	}

	stTempData16.byte.ucByte1 = pSectionData[8] & 0x1F;
	stTempData16.byte.ucByte0 = pSectionData[9];
	*PcrPid = stTempData16.sWord16;

	pucSectionData = pSectionData + 12;
	ucVersionNumber = (pSectionData[5] & 0x3E) >> 1;
	ucCurNextIndicator = pSectionData[5] & 0x01;
	ucSectionNumber = pSectionData[6];
	ucLastSectionNumber = pSectionData[7];

	stTempData16.byte.ucByte1 = pSectionData[10] & 0xF;
	stTempData16.byte.ucByte0 = pSectionData[11];
	iInfoLength = stTempData16.sWord16;
	if (iInfoLength)
	{
		iNoOfBytesParsedSoFar = 0;
		while (iNoOfBytesParsedSoFar < iInfoLength)
		{
			iNoOfDataBytes = DMUX_ParseProgDescriptor(pucSectionData);
			pucSectionData += iNoOfDataBytes;
			iNoOfBytesParsedSoFar += iNoOfDataBytes;
		}
	}

	iInfoLength = sSectionLength - iInfoLength - 9;
	while (iInfoLength > 0)
	{
		stTempData16.byte.ucByte1 = pucSectionData[3] & 0xF;
		stTempData16.byte.ucByte0 = pucSectionData[4];

		iEsInfoLength = stTempData16.sWord16;

		stTempData16.byte.ucByte1 = pucSectionData[1] & 0x1F;
		stTempData16.byte.ucByte0 = pucSectionData[2];

		printf("streamtype = %d\n", pucSectionData[0]);
		switch (pucSectionData[0])
		{
		case MPEG1_VIDEO_STREAM:		/* 0x01 */
		case MPEG2_VIDEO_STREAM:		/* 0x02 */
		case MPEG4_VIDEO_STREAM:
		case H264_VIDEO_STREAM: 
			*VidPid = stTempData16.sWord16;
			printf("-------VidPid=%d\t", *VidPid);
			switch (pucSectionData[0])
			{
				case MPEG1_VIDEO_STREAM:
				case MPEG2_VIDEO_STREAM:
					video_type = FYF_VCODEC_TYPE_MPEG2;
					*VideoType = video_type;
					printf("-------VideoType=%d\t", *VideoType);
					break;
					
				case MPEG4_VIDEO_STREAM:
					video_type = FYF_VCODEC_TYPE_MPEG4;
					*VideoType = video_type;
					printf("-------VideoType=%d\t", *VideoType);
					break;
					
				case H264_VIDEO_STREAM: 
					video_type = FYF_VCODEC_TYPE_H264;
					*VideoType = video_type;
					printf("-------VideoType=%d\t", *VideoType);
					break;
			}

			break;

		case MPEG1_AUDIO_STREAM:		/* 0x03 */
		case MPEG2_AUDIO_STREAM:		/* 0x04 */
		case AAC_ADTS_AUDIO_STREAM:
		case AAC_LATM_AUDIO_STREAM:
		case AC3_AUDIO_STREAM:
			

			*AudPid = stTempData16.sWord16;
			printf("-------AudPid=%d\n", *AudPid);
	
			switch(pucSectionData[0])
			{
				case MPEG1_AUDIO_STREAM:
				case MPEG2_AUDIO_STREAM:
					audio_type = FYF_ACODEC_TYPE_MP3;
					*AudioType = audio_type;
					
					printf("-------AudioType=%d\t", *AudioType);
					break;
					
				case AAC_ADTS_AUDIO_STREAM:
				case AAC_LATM_AUDIO_STREAM:
					audio_type = FYF_ACODEC_TYPE_AAC;
					*AudioType = audio_type;
					
					printf("-------AudioType=%d\t", *AudioType);
					break;
					
				case AC3_AUDIO_STREAM:
					audio_type = FYF_ACODEC_TYPE_AC3;
					*AudioType = audio_type;
					
					printf("-------AudioType=%d\t", *AudioType);
					break;
			}

			break;

		default:
			break;
		}

		/* 第二个描述子分析循环 */
		iInfoLength -= (iEsInfoLength + 5);
		pucSectionData += 5;

		if (iEsInfoLength)
		{
			iNoOfBytesParsedSoFar = 0;
			while (iNoOfBytesParsedSoFar < iEsInfoLength)
			{
				iNoOfDataBytes = DMUX_ParseProgDescriptor(pucSectionData);
				pucSectionData += iNoOfDataBytes;
				iNoOfBytesParsedSoFar += iNoOfDataBytes;
			}
		}
	}
}

void OW_DLVodPidInfo_Callback(BU32 ch,BU32 filter,BU32 DataStartAddr,BU32 len)
{
	BU08 *buff = (BU08 *)DataStartAddr;

	printf("OW_DLVodPidInfo_Callback======================\n");
	
	DMUX_ParsePmtSection(buff, &u16VideoPid, &u16AudioPid, &u16PcrPid, &u8AudioType, &u8VideoType);
}



BU32 tmpLen = 0;
BU08  btmpInit = 0;
//extern BU08 configvalue[30];
#define 	ES_READ_BLOCK   (1024*4)
#define		RTP_PACK_SIZE		(0x10000)
#define	RTP_BUFF_SIZE	  (3*1024*1024)
FILE *fpout;

typedef void VOS_VOID;

typedef unsigned char VOS_UINT8;
typedef signed char VOS_INT8;
typedef char VOS_CHAR;

typedef unsigned short VOS_UINT16;
typedef signed short VOS_INT16;

typedef unsigned long VOS_UINT32;
typedef signed long VOS_INT32;

typedef unsigned int VOS_UINT;
typedef int VOS_INT;

typedef unsigned long VOS_BOOL;

typedef float VOS_FLOAT;
typedef double VOS_DOUBLE;

#define     HI_FAILURE          -1
#define     HI_SUCCESS          0

#define     RTP_VERSION		    0x2		//RTP版本号

#define     LITTLE_ENDIAN       1234	/* least-significant byte first (vax) */
#define     BIG_ENDIAN          4321	/* most-significant byte first (IBM, net) */
#define     BYTE_ORDER          1234

#if	BYTE_ORDER==BIG_ENDIAN
#define	RTP_NTOHL(x)	(x)
#define	RTP_HTONL(x)	(x)
#define	RTP_NTOHS(x)	(x)
#define	RTP_HTONS(x)	(x)
#else
#define RTP_NTOHL(x)	((((x) & 0x000000ff) << 24) |  (((x) & 0x0000ff00) <<  8) |  (((x) & 0x00ff0000) >>  8) |  (((x) & 0xff000000) >> 24))
#define RTP_HTONL(x)	((((x) & 0x000000ff) << 24) |  (((x) & 0x0000ff00) <<  8) |  (((x) & 0x00ff0000) >>  8) | (((x) & 0xff000000) >> 24))
#define RTP_NTOHS(x)	((((x) & 0x00ff) << 8) |  (((x) & 0xff00) >> 8))
#define RTP_HTONS(x)	((((x) & 0x00ff) << 8) |  (((x) & 0xff00) >> 8))
#endif									/* VTOP_BYTE_ORDER==VTOP_BIG_ENDIAN */

/************************* RTP & RTCP DEFINITION ***************************/
/***************************************************************************
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P|X|  CC   |M|     PT      |       sequence number         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 ***************************************************************************/
#if ( BYTE_ORDER==BIG_ENDIAN )
typedef struct vtopRTP_S_HDR
{
	VOS_UINT32 version:2;				//RTP的版本号，本规范的版本号是2
	VOS_UINT32 p:1;						//填充位
	VOS_UINT32 x:1;						//扩展位
	VOS_UINT32 cc:4;					//CSRC标识符总数
	VOS_UINT32 m:1;						//由配置定义规定,标识帧边界
	VOS_UINT32 pt:7;					//媒体数据类型
	VOS_UINT32 seq:16;					//RTP数据包的流水号
	VOS_UINT32 ts;						//时标,RTP数据包首字节的采样时标
	VOS_UINT32 ssrc;					//同步源标识
	VOS_UINT32 *csrc;					//RTP数据包中贡献源列表
} RTP_S_HDR;
#else
typedef struct vtopRTP_S_HDR
{
	VOS_UINT8 cc:4;
	VOS_UINT8 x:1;
	VOS_UINT8 p:1;
	VOS_UINT8 version:2;
	VOS_UINT8 pt:7;
	VOS_UINT8 m:1;
	VOS_UINT16 seq;
	VOS_UINT32 ts;						//时标,RTP数据包首字节的采样时标
	VOS_UINT32 ssrc;					//同步源标识
	VOS_UINT32 *csrc;					//RTP数据包中贡献源列表
} RTP_S_HDR;
#endif

HI_U32 num=0;
HI_U32 w = 0,r = 0;
HI_U8  rtpBuf[RTP_BUFF_SIZE + 8192];
#ifndef USE_HI3110
HI_UNF_AVPLAY_BUF_S *pBuf = NULL;
HI_UNF_AVPLAY_BUFID bufId = HI_UNF_AVPLAY_BUF_ID_ES_VID;
/* sample rates (table 4.5.1) */
static const int sampRateTab[12] = {
    96000, 88200, 64000, 48000, 44100, 32000, 
	24000, 22050, 16000, 12000, 11025,  8000
};
BU32 ptsValueInMS = 0;

typedef struct _ADTSHeader {
    /* fixed */
    unsigned int  sync;                           /* syncword */
    unsigned char id;                             /* MPEG bit - should be 1 */
    unsigned char layer;                          /* MPEG layer - should be 0 */
    unsigned char protectBit;                     /* 0 = CRC word follows, 1 = no CRC word */
    unsigned char profile;                        /* 0 = main, 1 = LC, 2 = SSR, 3 = reserved */
    unsigned char sampRateIdx;                    /* sample rate index range = [0, 11] */
    unsigned char privateBit;                     /* ignore */
    unsigned char channelConfig;                  /* 0 = implicit, >0 = use default table */
    unsigned char origCopy;                       /* 0 = copy, 1 = original */
    unsigned char home;                           /* ignore */

    /* variable */
    unsigned char copyBit;                        /* 1 bit of the 72-bit copyright ID (transmitted as 1 bit per frame) */
    unsigned char copyStart;                      /* 1 = this bit starts the 72-bit ID, 0 = it does not */
    int           frameLength;                    /* length of frame */
    int           bufferFull;                     /* number of 32-bit words left in enc buffer, 0x7FF = VBR */
    unsigned char numRawDataBlocks;               /* number of raw data blocks in frame */

    /* CRC */
    int           crcCheckWord;                   /* 16-bit CRC check word (present if protectBit == 0) */
} ADTSHeader;

extern BU32 PTSStart;
VOS_CHAR   rtpBuff[128]  __attribute__((aligned(4)));
BU08 BuffError= 0;
HI_U32		prertpts = 0;

VOS_UINT32 RTP_DecodeRtpBuf ( VOS_CHAR* pB, VOS_UINT32 uiLen, VOS_CHAR **pOutData, VOS_UINT32 *pOutLen , VOS_UINT32 *pTs , VOS_UINT16 *Seg )
{
    VOS_UINT32 extLen = 0;
    VOS_UINT32 datalength;
    VOS_UINT8 padlength = 0;
    RTP_S_HDR* pRTP;
    VOS_UINT8* pExt;
    VOS_CHAR* pData;
	int i = 0;

	
    memcpy(rtpBuff, pB,sizeof(RTP_S_HDR));
	
    pRTP = ( RTP_S_HDR* )rtpBuff;   //取出RTP头

    /* 解析出版本号并判断是否正确 */
    if( pRTP->version != RTP_VERSION )
    {
        printf( "RTP version error %d\n" ,pRTP->version);
        return HI_FAILURE;
    }

    /* 判断包长度是否小于RTP的固定长度 */
    if( uiLen < (VOS_UINT32)( 12 + (4*pRTP->cc)))
    {
        printf( "RTP Packet length error\n" );
        return HI_FAILURE;
    }

    /* Look for extensions */
    if(pRTP->x)
    {
        pExt = (VOS_UINT8*)(pB + 12 + (4*pRTP->cc));
        extLen = (pExt[2] << 8) | pExt[3];
        extLen = RTP_NTOHS((VOS_UINT16) extLen) *4+4;
    }

    /* Pointer to actual payload */
    pData = pB + 12 + (4*pRTP->cc) + extLen;
    /* Payload data length */
    datalength = uiLen - extLen - (4*pRTP->cc) - 12;
    /* Check for padding */
    if(pRTP->p)
    {
        if(pData[datalength-1] > (VOS_INT8)datalength)
        {
            printf( "RTP Packet padding length error\n" );
            return HI_FAILURE;
        }
        if(pData[datalength-1] < 1)
        {
            printf( "RTP Packet padding error\n" );
            return HI_FAILURE;
        }
        padlength = (VOS_UINT8)(pData[datalength-1]);
        datalength-=padlength;
    }

    /* Check for valid header length */
    if(extLen > (uiLen- 12 - (4 * pRTP->cc) -  padlength))
    {
        printf( "RTP Packet head length error\n" );
        return HI_FAILURE;
    }

	
    *Seg = RTP_NTOHS(pRTP->seq);
    *pTs = RTP_NTOHL(pRTP->ts);
	

	
	//printf("seq = %d\n", *Seg);
	//*seq = RTP_NTOHS(pRTP->seq);
	//printf("pts = %x(%x)\n", (*pts), (*pts/video_rate));
    *pOutData = pData;
    *pOutLen = datalength;

    return HI_SUCCESS;
}

HI_S32 ProcessAACPayload(HI_U8 * payload, HI_U32 payloadLength, HI_U8  * pOutBuffer, HI_S32 * outSize)
{
	HI_U32 hdr_section_length;
	hdr_section_length = (*payload<<8|*(payload+1))/8+2;
	if(*outSize < payloadLength-hdr_section_length)
	{
		*outSize = payloadLength-hdr_section_length;
		return -1;
	}
	memcpy(pOutBuffer,payload+hdr_section_length, payloadLength-hdr_section_length);
	*outSize=payloadLength-hdr_section_length;
	return HI_SUCCESS;
}

int  AACStreamRaw2ADTS(
		unsigned char  * pSrc,
        unsigned char  * pDst,
        int     frameLength,            /*!< raw data length(Unit: Bytes) */
        int     sampleRate,             /*!< sampling frequency: 48000/44100... > */
        int     ch)                     /*!< channel number:1 or 2 > */
{
	ADTSHeader  sADTS;
	ADTSHeader* fhADTS;
    unsigned char sampRateIdx;
    unsigned int  head0, head1;
    int nADTSFrameLength;

	fhADTS = &sADTS;
	sampRateIdx=0;
	while(1)
	{
	   if( (sampRateTab[sampRateIdx]) == sampleRate )
		   break;
	   sampRateIdx++;
	}
    nADTSFrameLength = frameLength + 7;         /* 7 bytes ADTS Header */
	/* fixed fields - should not change from frame to frame */ 
	fhADTS->sync =             0x0fff;          /* 12bit: */
	fhADTS->id =               0;               /* 1bit: MPEG bit - should be 1 */
	fhADTS->layer =            0;               /* 2bit: MPEG layer - should be 0 */ 
	fhADTS->protectBit =       1;               /* 1bit: */
	fhADTS->profile =          1;               /* 2bit: LC Profile: */
	fhADTS->sampRateIdx =      sampRateIdx;     /* 4bit: */
	fhADTS->privateBit =       0;               /* 1bit: */
	fhADTS->channelConfig =    ch;              /* 3bit: */
	fhADTS->origCopy =         0;               /* 1bit: */
	fhADTS->home =             0;               /* 1bit: */

	/* variable fields - can change from frame to frame */ 
	fhADTS->copyBit =          0;              /* 1bit: */
	fhADTS->copyStart =        0;              /* 1bit: */
	fhADTS->frameLength =      nADTSFrameLength;    /* 13bit: */
	fhADTS->bufferFull =       0x07ff;         /* 11bit: */
	fhADTS->numRawDataBlocks = 0;              /* 2bit: */
    
	head0  = fhADTS->sync       <<(32-12);
	head0 |= fhADTS->id         <<(32-12-1);
	head0 |= fhADTS->layer      <<(32-12-1-2);
	head0 |= fhADTS->protectBit <<(32-12-1-2-1);

	head0 |= fhADTS->profile       <<(32-12-1-2-1 -2);
	head0 |= fhADTS->sampRateIdx   <<(32-12-1-2-1 -2 -4);
	head0 |= fhADTS->privateBit    <<(32-12-1-2-1 -2 -4 -1);
	head0 |= fhADTS->channelConfig <<(32-12-1-2-1 -2 -4 -1 - 3);
	head0 |= fhADTS->origCopy      <<(32-12-1-2-1 -2 -4 -1 - 3 - 1);
	head0 |= fhADTS->home          <<(32-12-1-2-1 -2 -4 -1 - 3 - 1- 1);
	head0 |= fhADTS->copyBit       <<(32-12-1-2-1 -2 -4 -1 - 3 - 1- 1- 1);
	head0 |= fhADTS->copyStart     <<(32-12-1-2-1 -2 -4 -1 - 3 - 1- 1- 1- 1);
	head0 |= ((fhADTS->frameLength>>(13-2)) & 0x3);

	head1  = fhADTS->frameLength      <<(32-13+2);
	head1 |= fhADTS->bufferFull       <<(32-13+2-11);
	head1 |= fhADTS->numRawDataBlocks <<(32-13+2-11-2);

	pDst[0] = (unsigned char)(head0>>24)&0xff;
	pDst[1] = (unsigned char)(head0>>16)&0xff;
	pDst[2] = (unsigned char)(head0>>8)&0xff;
	pDst[3] = (unsigned char)(head0>>0)&0xff;
	pDst[4] = (unsigned char)(head1>>24)&0xff;
	pDst[5] = (unsigned char)(head1>>16)&0xff;
	pDst[6] = (unsigned char)(head1>>8)&0xff;
	
	memcpy(&pDst[7], pSrc, frameLength);
	
	return nADTSFrameLength;
}


//TODO,没有加越界判断
HI_S32		RtpAacParse( VOS_CHAR* pBuf, VOS_UINT32 uiLen, VOS_CHAR *pOutData, int *pOutLen)
{
	unsigned short  framePerPacket;
	short  bytePerPacket;
	short frmlenPerPacket[16];
	int		nRead  = 0 , pos = 0 ;
	HI_U8		readBuf[64*1024];
	static int counter=0;
	int i;
  
	*pOutLen = 0 ;
	counter++;

	//取帧个数
	memcpy(&framePerPacket, pBuf , 2 ); 
	pBuf += 2 ;
    framePerPacket >>= 12;  

	 if(framePerPacket)
	 {
		memcpy( frmlenPerPacket , pBuf,  2*framePerPacket );
		pBuf += 2*framePerPacket;

		bytePerPacket = 0;
		for(i=0; i<framePerPacket; i++)
		{
			unsigned short tmp;
			tmp = frmlenPerPacket[i];
			frmlenPerPacket[i] = (((tmp>>8)&0x00ff)|(tmp<<8)&0x00ff00)/8;
			bytePerPacket += frmlenPerPacket[i];
		}
		
		for(i=0; i<framePerPacket; i++)
		{
			memcpy( readBuf+7 ,pBuf, frmlenPerPacket[i] );
			pBuf +=  frmlenPerPacket[i] ;

			//TODO, 需要传入采样率和声道个数
			AACStreamRaw2ADTS(readBuf+7, readBuf, frmlenPerPacket[i], audio_rate/*44100*/, 2);

			memcpy( pOutData ,readBuf, frmlenPerPacket[i]+7 );
			pOutData += frmlenPerPacket[i]+7 ;
			*pOutLen  +=  frmlenPerPacket[i]+7 ; 
		}
	 }

	return	 0;
}


HI_S32 rtpParse(HI_U8 * buff, HI_U32 rtplen)
{
	HI_U8 *point;
	HI_S32		len = 0  , rtpPackLen = 0 , rtpPayLoadAudLen = 0 ;
	HI_U32 pid = 0;
	HI_U32 cnt = 0;
	HI_S32 res = 0;
	VOS_UINT32 rtpPayLoadLen = 0;
	HI_U32 i;
	HI_U8	rtpAudBuf[RTP_PACK_SIZE] , *pRtpPayLoad ;
	HI_S32           ret;
	HI_U32		rtpts = 0;
	HI_U32  seqnum ;
	static HI_U16 videoseq,audio_seq,seq;
	BU32 tmpPtsValue = 0;
	
	RTP_DEBUG("parse rtp len = 0x%d\n",rtplen);
	i = 0;
	if(w >= RTP_BUFF_SIZE)
	{
		RTP_DEBUG("buff loop %d\n",num);
		if (BuffError == FYF_FALSE)
		{
			num = w-r;
			RTP_DEBUG("BuffError = FALSE\n");
			memcpy(&(rtpBuf[0]), &(rtpBuf[r]), w-r);
			w=w-r;
			r=0;
		}
		else
		{
			w=r=0;
			num = 0;
			RTP_DEBUG("BuffError = TRUE\n");
			BuffError = 0;
			return;
		}
	}
	RTP_DEBUG(" parse rtp w = %d r = %d len = %d\n",w,r,rtplen);



	memcpy((rtpBuf + w), buff, rtplen);
	//获取读写指针
	w = w + rtplen;
	point = rtpBuf + r;
	//获取BUFF长度
	num = w - r;
	//num = rtplen;

	//识别是否是帧头

	for(i=0;i<num;i++)
	{
		if( point[i] == 0x24 )
		{
			rtpPackLen = (point[i + 2] << 8 ) | point[i + 3] ;
			if(rtpPackLen > 4096)
			{
				continue;
			}
			
			if(num  < i + 4 + rtpPackLen)
			{
				RTP_DEBUG("\033[0;31m 超出 rtpPackLen = %d, point[0] = %x, num = %d, point[i + 4 + rtpPackLen] = %x, i = %d\033[0m\n", rtpPackLen, point[i], num, point[i + 4 + rtpPackLen], i);
				r = r + i;
				return;
			}
		
			if(point[i + 4 + rtpPackLen] == 0x24)
			{
				break;
			}
 	
		}

	}

	
	r = r + i;
	
	num = w - r;


	while (num > 0)
	{
		point = rtpBuf+r;

		rtpPackLen = (point[2] << 8) | point[3]  ;

		if(rtpPackLen == 0)
		{
			break;
		}

		if(num < 4 + rtpPackLen)
			break;

		if(point[4 + rtpPackLen] != 0x24)
		{
			break;
		}
		 
		//只取 RTP,不要RTCP
		if (point[1] & 0x1)
		{
			RTP_DEBUG("Rtcp pack , skip. %d\n",rtpPackLen);
		}
		else
		{
		    res = RTP_DecodeRtpBuf ( (char*)(point+4) ,rtpPackLen , (char**)&pRtpPayLoad , &rtpPayLoadLen ,&rtpts,&seq);
			if (HI_SUCCESS != res)
			{
				printf("Rtp decode err.\n");
				BuffError = FYF_TRUE;
				r = r + 4 + rtpPackLen;
				break;
			}

			BuffError = FYF_FALSE;

			if( point[1] == 0)
			{
				
				if(seq != videoseq + 1)
				{
					//printf("\033[0;31mdrop a video rtp %d %d\033[0m\n",seq,videoseq);
				}
				videoseq = seq;

				ret = HI_UNF_AVPLAY_GetBuf(s_AVPLAYHandle, HI_UNF_AVPLAY_BUF_ID_ES_VID, ES_READ_BLOCK, &pBuf);
				if(ret != HI_SUCCESS)
				{
					printf("get video buff error 0x%x num = %d\n",ret,num);
					break;
				}
				memcpy(pBuf->pu8Data, pRtpPayLoad, rtpPayLoadLen);
				prertpts = rtpts;

				if (video_rate == 90000)
				{
					pBuf->u64pts = (HI_U64)rtpts ;		
				}
				else
				{
					pBuf->u64pts = (HI_U64)(90000 / video_rate * rtpts  );		
				}

				if (btmpInit == 0)
				{
					btmpInit = 1;
					PTSStart = rtpts / video_rate;
					RTP_DEBUG("\033[0;31m ptsstart = %d(%x), video_rate = %d\033[0m\n",PTSStart,g_PreviewPts, video_rate);
				}


				tmpPtsValue = rtpts / video_rate;
				if (tmpPtsValue > PTSStart)
				{
					ptsValueInMS = tmpPtsValue;
				}
				//printf("video pts = 0x%x rtpseq = %d, ptsValueInMS = %d\n",rtpts/video_rate,seq, ptsValueInMS);

				pBuf->u32validContLen = rtpPayLoadLen;

				ret = HI_UNF_AVPLAY_PutBuf(s_AVPLAYHandle, HI_UNF_AVPLAY_BUF_ID_ES_VID); 
				if(ret != HI_SUCCESS)
				{
					printf("put audio buff error 0x%x\n",ret);
				}	

			}
			else if(point[1] == 2)
			{
				res = RtpAacParse((char*)pRtpPayLoad ,rtpPayLoadLen ,(char*) rtpAudBuf , &rtpPayLoadAudLen );
				if( HI_SUCCESS == res )
				{
				    ret = HI_UNF_AVPLAY_GetBuf(s_AVPLAYHandle, HI_UNF_AVPLAY_BUF_ID_ES_AUD, ES_READ_BLOCK, &pBuf);
					if(ret != HI_SUCCESS)
					{
						printf("get audio buff error 0x%x  num = %d\n",ret,num);
						break;
					}
					memcpy(pBuf->pu8Data,rtpAudBuf,rtpPayLoadAudLen);
					
					if (video_rate == 90000)
					{
						//pBuf->u64pts = (HI_U64)rtpts ;	
						
						pBuf->u64pts = (HI_U64)prertpts;
					}
					else
					{
//						pBuf->u64pts = (HI_U64)( 90000 / video_rate * rtpts  );	
						pBuf->u64pts = (HI_U64)( 90000 / video_rate * prertpts  );	
					}
					//printf("		audio pts = 0x%x, seq = %d \n",prertpts/video_rate, seq);
					
					pBuf->u32validContLen = rtpPayLoadAudLen;		
					ret = HI_UNF_AVPLAY_PutBuf(s_AVPLAYHandle, HI_UNF_AVPLAY_BUF_ID_ES_AUD);	
					if(ret != HI_SUCCESS)
					{
						printf("put audio buff error 0x%x\n",ret);
					}
				}
			}
	
		}

		r = r + 4 + rtpPackLen;
		num = w-r;
	
	}

	return HI_SUCCESS;
}


extern HI_U8  mp4aHead[];
extern HI_U8  mp4Head[];
BU32 g_channel_id = 0;


void OW_DLRTPInfo_Callback(BU32 ch,BU32 filter,BU32 DataStartAddr,BU32 len)
{
	BU08 *buff = (BU08 *)DataStartAddr;

	rtpParse(buff, len);

	return;	

}



int OW_DLRTPInfo(int FreqKhz, int pmtPid)
{
#if 0
	int ret = -1;
	int count = 0;
	BU32 channel_id;
	BU32 filter[10];

	u16VideoPid = 0x1fff;
	u16AudioPid = 0x1fff;
	u16PcrPid = 0x1fff;
	u8AudioType = FYF_ACODEC_TYPE_MP3;
	u8VideoType = FYF_VCODEC_TYPE_MPEG4;
	btmpInit = 0;
	APP_IPI_StopAudioVideo(1);
	g_channel_id = 0;
	if (GUI_IpanelState == 0)
	{	
		APP_Com_API_StopProg(0);
	}
	else
	{
		APP_Com_API_StopProg(1);
	}
	
	FYF_API_CTLVod(3);
	FYF_API_CTLVod(2);

	ret = TRD_LockTuner(FreqKhz, 6875, 2);

	if(FYF_OK != ret)
	{
		OW_ERROR("TRD_LockTuner Error\n");
		return FYF_ERR;
	}

	TRD_SetBroadcastFreq(FreqKhz);
	FYF_API_demux_set_notify(NULL);
	FYF_API_demux_set_notify(OW_DLRTPInfo_Callback);

	FYF_API_demux_create_channel(0, FYF_DEMUX_DATA_PES_CHANNEL, &channel_id, pmtPid);

	g_channel_id = channel_id;

	ret = FYF_API_demux_start_channel(channel_id);
	if (FYF_ERR == ret)
	{
		OW_ERROR("FYF_CPI_demux_start_channel ERROR\n");
		return FYF_ERR;
	}
#endif
	return FYF_OK;
}

int OW_DLRTPInfo_StopChannel(void)
{
#if 0
	BS32 s32ret;
	HI_UNF_AVPLAY_ATTR_S struAttrConfig;
	FYF_API_demux_stop_channel(g_channel_id);
	FYF_API_demux_destroy_channel(g_channel_id);
	FYF_API_demux_set_notify(NULL);
	g_channel_id = 0;

	
	FYF_API_CTLVod(3);

	s32ret = HI_UNF_AVPLAY_GetDefaultConfig(&struAttrConfig, HI_UNF_AVPLAY_STREAM_TYPE_TSI);
    struAttrConfig.stAdecAttr.enType = HI_UNF_ACODEC_TYPE_MP3;
    struAttrConfig.stVdecAttr.enType = HI_UNF_VCODEC_TYPE_MPEG2;
	struAttrConfig.stAdecAttr.stAc3Param.enAc3workmode = HI_UNF_AC3_PCM;
#if (FYF_QUICK_OUPUT == 0)
	struAttrConfig.stSyncAttr.enPreSyncMode = HI_UNF_AVPLAY_PRESYNC_MODE_NORMAL;//HI_UNF_AVPLAY_PRESYNC_MODE_QUICKOUTPUT;
	struAttrConfig.stSyncAttr.bSmoothPlay = HI_FALSE;
	struAttrConfig.stSyncAttr.enSyncRef = HI_UNF_AVPLAY_SYNC_REF_AUDIO;//HI_UNF_AVPLAY_SYNC_REF_PCR;
#else
	struAttrConfig.stSyncAttr.enPreSyncMode = HI_UNF_AVPLAY_PRESYNC_MODE_QUICKOUTPUT;//HI_UNF_AVPLAY_PRESYNC_MODE_QUICKOUTPUT;
	struAttrConfig.stSyncAttr.bSmoothPlay = HI_FALSE;
	struAttrConfig.stSyncAttr.enSyncRef = HI_UNF_AVPLAY_SYNC_REF_PCR;//HI_UNF_AVPLAY_SYNC_REF_PCR;
#endif
	struAttrConfig.stStreamAttr.unStreamAttr.stTsiAttr.u32AudPid = 0x1fff;    
	struAttrConfig.stStreamAttr.unStreamAttr.stTsiAttr.u32VidPid = 0x1fff;
	struAttrConfig.stStreamAttr.unStreamAttr.stTsiAttr.u32PcrPid = 0x1fff;
	
	s32ret = HI_UNF_AVPLAY_SetAttr(s_AVPLAYHandle, &struAttrConfig); 
	if (HI_SUCCESS != s32ret) 
	{ 
		printf("AVPLAY SetAttr failure\n"); 
	} 
#endif
	
}
#endif

int OW_DLVodPidInfo(int FreqKhz, int pmtPid, BU16 * const VidPid, BU16 * const AudPid, BU16 * const PcrPid, BU08 * const AudioType, BU08 * const VideoType)
{
	int ret = -1;
	int count = 0;
	BU32 channel_id;
	BU32 filter[10];
	BU08 coef[4] = {0x2, 0, 0, 0};
	BU08 mask[4] = {0xff, 0, 0, 0}; 
	BU08 excl[4] = {0};

	u16VideoPid = 0x1fff;
	u16AudioPid = 0x1fff;
	u16PcrPid = 0x1fff;
	u8AudioType = FYF_ACODEC_TYPE_MP3;
	u8VideoType = FYF_VCODEC_TYPE_MPEG2;

	printf("freq = %d, pmtpid = %d\n", FreqKhz, pmtPid);
	
	//DMUX_ParseMP4Section(secdata, VidPid, AudPid, PcrPid, AudioType, VideoType);

	ret = TRD_LockTuner(FreqKhz, 6875, 2);

	if(FYF_OK != ret)
	{
		OW_ERROR("TRD_LockTuner Error\n");
		return FYF_ERR;
	}


	TRD_SetBroadcastFreq(FreqKhz);
	FYF_API_demux_set_notify(NULL);
	FYF_API_demux_set_notify(OW_DLVodPidInfo_Callback);

	FYF_API_demux_create_channel(0, FYF_DEMUX_DATA_PSI_CHANNEL, &channel_id, pmtPid);
	FYF_API_demux_create_filter(channel_id, &filter[0]);
	ret = FYF_API_demux_set_filter(channel_id, 
									filter[0],  
									4,
									coef,  
									mask, 
									excl);
	if (FYF_ERR == ret)
	{
		OW_ERROR("FYF_API_demux_set_filter ERROR\n");
		return FYF_ERR;
	}
	
	ret = FYF_API_demux_enable_filter(channel_id, filter[0]);
	if (FYF_ERR == ret)
	{
		OW_ERROR("ipanel_porting_enable_demux_filter ERROR\n");
		return FYF_ERR;
	}
	
	ret = FYF_API_demux_start_channel(channel_id);
	if (FYF_ERR == ret)
	{
		OW_ERROR("FYF_CPI_demux_start_channel ERROR\n");
		return FYF_ERR;
	}

	while (1)
	{
		if (u16VideoPid != DEMUX_INVALID_PID || u16AudioPid != DEMUX_INVALID_PID /*|| u16PcrPid != DEMUX_INVALID_PID*/)
		{
			printf("u16VideoPid = %x, u16AudioPid = %x, u16PcrPid = %x, u8VideoType = %d, u8AudioType = %d\n", u16VideoPid, u16AudioPid, u16PcrPid, u8VideoType, u8AudioType);
			*VidPid = u16VideoPid;
			*AudPid = u16AudioPid;
			*PcrPid = u16PcrPid;
			*VideoType = u8VideoType;
			*AudioType = u8AudioType;
			break;
		}
		else
		{
			if (count >= 30)
			{
				OW_ERROR("OW_DLVodPidInfo Error.\n");
				break;
			}
			FYF_API_thread_sleep(100);
			count++;
		}
	}
	
	FYF_API_demux_stop_channel(channel_id);
	DeleteFilter(channel_id, filter[0]);
	FYF_API_demux_set_notify(NULL);
	
	if (u16VideoPid != DEMUX_INVALID_PID || u16AudioPid != DEMUX_INVALID_PID || u16PcrPid != DEMUX_INVALID_PID)
	{
		return	FYF_OK;
	}
	else
	{
		return  FYF_ERR;
	}
}








