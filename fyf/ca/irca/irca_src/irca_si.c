#include <basic_types.h>
#include <api/libc/string.h>
#include <api/libtsi/sie.h>
#include <os/tds2/alitypes.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/ca_task.h>
#include "irca_drv.h"
#include "irca_debug.h"


static BYTE   				PMTData[MAX_PMTDATA_LEN];
static BYTE   				CATData[MAX_CATDATA_LEN];
static BYTE   				NITData[8];
static BYTE				PMTVersion = 0xff;
static BYTE				CATVersion = 0xff;
static BYTE				NITVersion = 0xff;
static UINT16			sCurServiceId = 0xffff;


void IRCA_CurProgNo(UINT16 CurProgNo)
{
	sCurServiceId=CurProgNo;
}


void IRCA_ParsePMT(UINT8 *PmtData, INT32 length, UINT16 param)
{
	INT16  	iInfoLength=0, iEsInfoLength;
	INT16 		iNoOfBytesParsedSoFar;
	INT16 		iDesLength = 0;
	UINT8 		*pucSectionData;
	BYTE 		ucVersionNumber, ucCurNextIndicator;
	WORD 		ESPid ,ProgramNumber;
    WORD        CABytes = 0;
	BYTE        *EsNum , *EsCADescs ;
	INT16		temp;
	UINT32 		DelayTime;

	pucSectionData = PmtData ;

	ucCurNextIndicator = pucSectionData[5] & 0x01;
	if (ucCurNextIndicator != 1)
	{
		return;
	}

	ucVersionNumber = (pucSectionData[5] & 0x3E) >> 1;
	
	if (PMTVersion== ucVersionNumber)
	{
		return;
	}
	else
	{
		TP_Desc_AfterStopProg();
	}
	

	/*program_number */
	ProgramNumber = (pucSectionData[3] << 8) | pucSectionData[4];

	DelayTime = osal_get_tick();

	printf("Get PMT of prog %d  time =%ld\n",ProgramNumber, DelayTime);

	/*pyh:若实际得到的PMT的ServiceId不等于换台时记录的sCurServiceId，舍弃该PMT*/
	if (ProgramNumber != sCurServiceId)
	{
		IrCa_Debug_Err((" [tpca_si] Get PMT Err 5: this ProgramNumber[%d] != sCurServiceId[%d]\n ",
			ProgramNumber,sCurServiceId));
		
		return;
	}

	//STTBX_Print((" [tpca_si] Get a new PMT \n "));
	
	MEMSET(PMTData , 0 , MAX_PMTDATA_LEN );
	CABytes = 0;

	iInfoLength = ((pucSectionData[10]&0x0f) << 8) | pucSectionData[11];
	/* point to the first loop descriptor() */
	pucSectionData += 12;
	
	if (iInfoLength>0)
	{
		iNoOfBytesParsedSoFar = 0;

		while (iNoOfBytesParsedSoFar < iInfoLength)
		{
			iDesLength = pucSectionData[1] + 2;

			if (pucSectionData[0] == 0x09)
			{
				for( temp = 0; temp < iDesLength; temp++ )
				{
					PMTData[ 17 + CABytes + temp ] = pucSectionData[ temp ];
				}
				CABytes = CABytes + iDesLength ;
				/*glbdescrcount ---8 uimsbf*/
				(PMTData[16]) ++ ;
				
			}
			pucSectionData += iDesLength;
			iNoOfBytesParsedSoFar += iDesLength;
		}
	}

	EsNum = &PMTData[17 + CABytes] ;
	*EsNum = 0;
	CABytes ++ ;

	iInfoLength = length -4 -iInfoLength-12;
		
	while (iInfoLength > 0)
	{
		ESPid = ((pucSectionData[1] & 0x1F) << 8) | (pucSectionData[2]);

		TP_Desc_GetEsPid(ESPid,  pucSectionData[0]); 
		
		*EsNum += 1;

		/*streamPID ---16 uimsbf*/
		PMTData[ 17 + CABytes ] = pucSectionData[ 1 ] & 0x1F;
		PMTData[ 17 + CABytes +1 ] = pucSectionData[ 2 ];

		EsCADescs = &PMTData[ 17 + CABytes + 2 ];
		*EsCADescs = 0 ;
		CABytes += 3 ;

		iEsInfoLength = ((pucSectionData[3] & 0xF)<<8)|(pucSectionData[4]);
		pucSectionData += 5;

		if (iEsInfoLength>0)
		{
			iNoOfBytesParsedSoFar = 0;
			while (iNoOfBytesParsedSoFar < iEsInfoLength)
			{
				iDesLength = pucSectionData[1] + 2;
				if (pucSectionData[0] == 0x09)
				{
					for( temp = 0; temp < iDesLength; temp++ )
					{
						PMTData[ 17 + CABytes + temp ] = pucSectionData[ temp ];
					}
					*EsCADescs += 1;
					CABytes =CABytes + iDesLength ;
				}
				pucSectionData += iDesLength;
				iNoOfBytesParsedSoFar += iDesLength;
			}
		}
		iInfoLength -= (iEsInfoLength + 5);
	}

	CABytes = CABytes + 15 ;

	/*msg_length ---16 uimsbf*/
	PMTData[ 0 ] = ( BYTE )( ( CABytes >> 8 ) & 0xff );
	PMTData[ 1 ] = ( BYTE )( CABytes & 0xff );
	/*service_id ---16 uimsbf*/
	PMTData[ 14 ] = ( BYTE )( ( ProgramNumber >> 8 ) & 0xff );
	PMTData[ 15 ] = ( BYTE )( ProgramNumber & 0xff );

	IRCA_NewProg(PMTData);
	PMTVersion = ucVersionNumber;
	
}

UINT8 IRCA_ParseCAT(UINT16 pid, struct si_filter_t *filter, UINT8 reason, UINT8 *CatData, INT32 length)
{
	INT16 iInfoLength, section_length;
	INT16 iNoOfBytesParsedSoFar;
	INT16 iDesLength = 0;
	BYTE ucCurNextIndicator, ucVersionNumber;
	BYTE ucLastSectionNumber, ucSectionNumber;
	BYTE *paucDataBuff;
	BYTE  CaBytes = 0  , CaDescs = 0 ;
	INT16  Temp =0;


	section_length = ((CatData[1] & 0x0f) << 8) | (CatData[2]);

	ucVersionNumber = (CatData[5] & 0x3E) >> 1;
	ucCurNextIndicator = CatData[5] & 0x01;
	ucSectionNumber = CatData[6];
	ucLastSectionNumber = CatData[7];

	if (ucCurNextIndicator != 1)
	{
		return sie_started;
	}

	
/*
	CATSection.uLastSectionNumber = ucLastSectionNumber;
	CATSection.aucSectionNumber[ucSectionNumber / 8] =
	CATSection.aucSectionNumber[ucSectionNumber / 8] | (0x80 >> (ucSectionNumber % 8));*/

	if (CATVersion == ucVersionNumber)
	{
		//STTBX_Print((" [tpca_si] Get CAT Err 3 : scTableVerNum the same!\n "));
		return sie_started;
	}

	/*irdeto 回传CAT的方法，参考Embedded_CA_Task_API_for_DVB_Client.pdf中的
      4.3 CA_CLIENT__DEFINE_EMM_SERVICE*/
      
	iInfoLength = section_length - 4 - 5;

 	iNoOfBytesParsedSoFar = 0;

	/* point to CA_descriptor() */
	paucDataBuff = CatData + 8;

	MEMSET( CATData , 0 , MAX_CATDATA_LEN  );
	CaBytes = 0 ;
	CaDescs = 0 ;
	
	while (iNoOfBytesParsedSoFar < iInfoLength)
	{
		iDesLength = paucDataBuff[1] + 2;

		if (paucDataBuff[0] == 0x09)
		{
			for ( Temp = 0 ; Temp < iDesLength; Temp++ )
			{
 				CATData[ 11+CaBytes+Temp ] = paucDataBuff[Temp];
			}
			CaBytes += iDesLength ;
			CaDescs ++ ;
		}
		paucDataBuff += iDesLength;
		iNoOfBytesParsedSoFar += iDesLength;
	}
	
	CaBytes = CaBytes + 9 ;

	/* msg_length ---16 uimsbf */
	CATData[ 0 ] = (BYTE)( ( CaBytes >> 8 ) & 0xff );
	CATData[ 1 ] = (BYTE)( CaBytes & 0xff );
	/* system_count ---8 uimsbf */
	CATData[ 10 ] = CaDescs;
	
	IRCA_NewTs(CATData);
	CATVersion = ucVersionNumber;
	return sie_started;

	
}


UINT8 IRCA_GETEMM(UINT16 pid, struct si_filter_t *filter, UINT8 reason, UINT8 *CatData, INT32 length)
{
	printf("Get section tableid =0x%x pid =0x%x ,length=%d  time=%l\n" ,CatData[0],pid,length,osal_get_tick());
}



/******************************分析NIT表***************************************/
void IRCA_ParseNIT(UINT8 *NitBuf, INT32 length, UINT16 param)
{
	BYTE *pucSectionData;
	INT16 iNoOfDataBytes;
	INT16 iInfoLength;
	INT16 iNoOfBytesParsedSoFar;
	BYTE ucLastSectionNumber;
	BYTE ucSectionNumber;
	BYTE ucVersionNumber;
	BYTE ucCurNextIndicator;
	BYTE ucOldVersionNumber;
	BYTE pNITData[10];
	INT16 TransportDesLength = 0;

	pucSectionData = NitBuf + 10;

	ucVersionNumber = (NitBuf[5] & 0x3E) >> 1;/**/
	ucCurNextIndicator = NitBuf[5] & 0x01;
	ucSectionNumber = NitBuf[6];
	ucLastSectionNumber = NitBuf[7];

	if ((ucCurNextIndicator != 1)||(NITVersion == ucVersionNumber))
	{
		return;
	}

	/***********************************
	NITSection.uLastSectionNumber = ucLastSectionNumber;
	NITSection.aucSectionNumber[ucSectionNumber / 8] =
	NITSection.aucSectionNumber[ucSectionNumber / 8] | (0x80 >> (ucSectionNumber % 8));
*/
	/*
	 * read the network_descriptors_length (12)
	 */
	iInfoLength = ((NitBuf[8] & 0xF)<<8 | NitBuf[9]);

	pucSectionData += iInfoLength;


	iInfoLength = ((pucSectionData[0]&0x0f)<<8)|pucSectionData[1]; /*transport_stream_loop_length*/
	pucSectionData += 2; /*point to the first loop*/
	if (iInfoLength > 0)
	{
		iNoOfBytesParsedSoFar = 0;
		while (iNoOfBytesParsedSoFar < iInfoLength)
		{
			TransportDesLength = ((pucSectionData[4]&0x0f)<<8)|pucSectionData[5];

			MEMSET(pNITData,0,10);
			/*msg_length ---16 uimsbf*/
			pNITData[ 1 ] = 8;
			/*group_handle ---32 uimsbf*/
			pNITData[ 5 ] = 0;
			/*original_network_id ---16 uimsbf*/
			pNITData[ 6 ] = pucSectionData[2];
			pNITData[ 7 ] = pucSectionData[3];
			/*transport_id ---16 uimsbf*/
			pNITData[ 8 ] = pucSectionData[0];
			pNITData[ 9 ] = pucSectionData[1];

			IRCA_GetNIT(pNITData);
	
			pucSectionData += (TransportDesLength+6);
			iNoOfBytesParsedSoFar += (TransportDesLength+6);
		}
	}

	NITVersion = ucVersionNumber;

}

void IRCA_SetPMTVersion(BYTE PMTVer)
{
	PMTVersion = PMTVer;
}

void IRCA_SetCATVersion(BYTE CATVer)
{
	CATVersion = CATVer;
}

void IRCA_SetNITVersion(BYTE NITVer)
{
	NITVersion = NITVer;
}


