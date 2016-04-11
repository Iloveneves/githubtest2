#include <os/tds2/alitypes.h>
#include <basic_types.h>
#include <api/libtsi/sec_pmt.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_vd.h>
#include <irca/pubh/s3_vddd.h>
#include "irca_debug.h"
#include "irca_drv.h"

#define DECSRAMBLER_API

static struct dmx_device *cas_desc_dev = NULL;

Irca_Desc_Struct   IrcaDesc[MAX_DESC_NUM];
Irca_EsDecs_Struct IrcaEsDesc[MAX_DESC_NUM];


void TP_Desc_init(void)
{
	BYTE  i;
 
	for (i=0 ;i<MAX_DESC_NUM ; i++)
	{
		IrcaDesc[i].DescStatus = DESCFREE;
		IrcaEsDesc[i].DescIndex = INVALID_DESC_INDEX ;
		IrcaEsDesc[i].EsPid = 0x1fff;
	}
	cas_desc_dev = (struct dmx_device *)dev_get_by_type(NULL, HLD_DEV_TYPE_DMX);
}
void TP_Desc_GetEsPid( WORD EsPid , BYTE Estype)
{
	BYTE i;
	BYTE Temp = MAX_DESC_NUM ;

	for (i=0 ;i<MAX_DESC_NUM ; i++)
	{
		if (IrcaEsDesc[i].EsPid == 0x1fff)
		{
			if (Temp==MAX_DESC_NUM)
			{
				Temp = i;
			}
		}
		else
		{
			if (IrcaEsDesc[i].EsPid == EsPid)
			{
				Temp = i;
				break;
			}
		}
	}
	if (Temp!=MAX_DESC_NUM)
	{
		IrcaEsDesc[Temp].EsPid = EsPid;

		switch(Estype)
		{
			case MPEG1_AUDIO_STREAM:
			case MPEG2_AUDIO_STREAM:

				IrcaEsDesc[Temp].EsType = DES_AUDIO;
				break;

			case MPEG1_VIDEO_STREAM:
			case MPEG2_VIDEO_STREAM:
			case MPEG4_VIDEO_STREAM:
			case H264_VIDEO_STREAM:	
				
				IrcaEsDesc[Temp].EsType = DES_VIDEO;
				break;
			/*case PRIVATE_DATA_STREAM:
				break;*/

			default:
				IrcaEsDesc[Temp].EsType = DES_TTX;
				break;
		}
	}
	else
	{
		IrCa_Debug_Err(("TP_Desc_GetEsPid fail! [EsPid=0x%x Estype = %d]\n",EsPid,Estype));
	}
	return ;
}

void TP_Desc_AfterStopProg(void)
{
	BYTE  i;
	for (i=0 ;i<MAX_DESC_NUM ; i++)
	{
		IrcaEsDesc[i].EsPid = 0x1fff;
	}
}


#define DECSRAMBLER_API

vd_ref_id TP_Desc_Open( vd_driver_type_e eDriverType, 
						vd_rsc_id wResourceID, void *pvData )
{
	BYTE  DescId=INVALID_DESC_INDEX;
	BYTE  i;
	
	IrCa_Debug_VdDes(("TP_Desc_Open [eDriverType=%d] [wResourceID=%d]\n",
		eDriverType,wResourceID));/**/

	for (i=0 ;i<MAX_DESC_NUM ; i++)
	{
		if(IrcaDesc[i].DescStatus == DESCFREE)
		{
			IrcaDesc[i].DescStatus = DESCOCCUPIED ;
			DescId = i;
			break;
		}
	}
	if (i==MAX_DESC_NUM)
	{
		IrCa_Debug_Err(("OpenDescrambler fail!\n"));
		return VD_REF_ID_ERROR;
	}

	if(RET_SUCCESS!=dmx_io_control(cas_desc_dev, DMX_IS_TS_ENTER_CSA, 0))
		dmx_io_control(cas_desc_dev, DMX_BYPASS_CSA, 0);

	return (vd_ref_id)DescId;
	
}

ia_result TP_Desc_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{
	BYTE i ;
	IrCa_Debug_VdDes(("TP_Desc_Close [eDriverType=%d] [wRefId=%d]\n",
		eDriverType,wRefId));/**/

	if (wRefId > VD_REF_ID_MAX || wRefId >= MAX_DESC_NUM)//wRefId < 0 || 
	{
		IrCa_Debug_Err(("TP_Desc_Close fail! [ invalid wRefId=%d]\n",wRefId));
		return IA_FAIL;
	}
	if (IrcaDesc[wRefId].DescStatus == DESCOCCUPIED)
	{
		IrcaDesc[wRefId].DescStatus= DESCFREE; 
	}
	for (i=0 ;i<MAX_DESC_NUM ; i++)
	{
		if (IrcaEsDesc[i].DescIndex == wRefId ) 
		{
			IrcaEsDesc[i].DescIndex = INVALID_DESC_INDEX;
			break;
		}
	}
	return IA_SUCCESS;
}

ia_result TP_Desc_IO(vd_driver_type_e eDriverType, 
						vd_ref_id wRefId, ia_word16 wOpCode,
						ia_word32 wDataWriteSize, void *pvDataWrite, 
						ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	BYTE i ;
	
	/*IrCa_Debug_VdDes(("TP_Desc_IO [eDriverType=%d] [wRefId=%d]",eDriverType,wRefId));*/

	if (wRefId > VD_REF_ID_MAX
		|| pvDataWrite == NULL || wDataWriteSize <= 0)//wRefId < 0 || 
	{
		IrCa_Debug_Err(("TP_Desc_IO fail! [ invalid param ]\n"));
		return IA_INVALID_PARAMETER;
	}
	
	switch (wOpCode)
	{
		case VD_DD_SET_PID :
		{
			vd_dd_pid_st* pWrite;
			WORD Pid = 0x1fff;  

			pWrite = (vd_dd_pid_st*)pvDataWrite;
			Pid = (WORD)(pWrite->wPid);

			IrCa_Debug_VdDes(("[wOpCode = VD_DD_SET_PID] Pid=0x%x\n",Pid));/**/

			if (Pid >= 0x1fff)
			{
				IrCa_Debug_Err(("VD_DD_SET_PID fail! [ invalid Pid = 0x%x ]\n",Pid));
				return IA_INVALID_PARAMETER;
			}
			if (IrcaDesc[wRefId].DescStatus == DESCFREE)
			{
				IrCa_Debug_Err(("Set Pid Error wRefId =%d\n",wRefId));
				return IA_INVALID_PARAMETER;
			}
			for (i=0 ;i<MAX_DESC_NUM ; i++)
			{
				if(IrcaEsDesc[i].DescIndex == wRefId )
				{
					IrcaEsDesc[i].DescIndex = INVALID_DESC_INDEX;
				}
			}
			
			for (i=0 ;i<MAX_DESC_NUM ; i++)
			{
				if (IrcaEsDesc[i].EsPid == Pid ) 
				{
					IrcaEsDesc[i].DescIndex = wRefId;
					break;
				}
			}
			if (i==MAX_DESC_NUM)
			{
				IrCa_Debug_Err(("SetDescramblerPID fail! [ Pid = 0x%x ]\n",Pid));
				return IA_FAIL;
			}
			ErrCode = IA_SUCCESS;
		}
		
		break;

		case VD_DD_SET_KEY:
		{
			vd_dd_key_st* pstWrite;
			BYTE*  pWrite8;

			pstWrite = (vd_dd_key_st*)pvDataWrite;
			pWrite8 = (BYTE*)(pvDataWrite + sizeof(vd_dd_key_st));

			

			/*IrCa_Debug_VdDes(("[wOpCode = VD_DD_SET_KEY] eKeyType=%d Key=0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \n",
				pstWrite->eKeyType,
				pWrite8[sizeof(vd_dd_key_st)+0],pWrite8[sizeof(vd_dd_key_st)+1],
				pWrite8[sizeof(vd_dd_key_st)+2],pWrite8[sizeof(vd_dd_key_st)+3],
				pWrite8[sizeof(vd_dd_key_st)+4],pWrite8[sizeof(vd_dd_key_st)+5],
				pWrite8[sizeof(vd_dd_key_st)+6],pWrite8[sizeof(vd_dd_key_st)+7]));*/
			
			if (pstWrite->eAlgorithm != VD_DD_DVB)
			{
				IrCa_Debug_Err(("VD_DD_SET_KEY fail! [ eAlgorithm = 0x%x ]\n",pstWrite->eAlgorithm));
				return IA_INVALID_PARAMETER;
			}
			if (pstWrite->wKeyLength != 8)
			{
				IrCa_Debug_Err(("VD_DD_SET_KEY fail! [ wKeyLength = %d ]\n",pstWrite->wKeyLength));
				return IA_INVALID_PARAMETER;
			}
			
			for (i=0 ;i<MAX_DESC_NUM ; i++)
			{
				if (IrcaEsDesc[i].DescIndex == wRefId ) 
				{
					break;
				}
			}
			if (i==MAX_DESC_NUM)
			{
				IrCa_Debug_Err(("Find desc fail! [ wRefId = 0x%x ]\n",wRefId));
				return IA_FAIL;
			}		

			//pWrite8[3]= (UINT8)pWrite8[0]+(UINT8)pWrite8[1]+(UINT8)pWrite8[2];
		//	pWrite8[7]= (UINT8)pWrite8[4]+(UINT8)pWrite8[5]+(UINT8)pWrite8[6]; 

			
			if (pstWrite->eKeyType == VD_DD_KEY_ODD)
			{
				if (dmx_cfg_cw(cas_desc_dev, IrcaEsDesc[i].EsType, 3,  (UINT32 *)pWrite8 )!=SUCCESS)
					IrCa_Debug_Err(("SetDescramblerOddKey fail! \n"));
			}
			else if (pstWrite->eKeyType == VD_DD_KEY_EVEN)
			{
				 if(dmx_cfg_cw(cas_desc_dev, IrcaEsDesc[i].EsType, 2, (UINT32 *)pWrite8 )!= SUCCESS)
					IrCa_Debug_Err(("SetDescramblerEvenKey fail! \n"));
			}
		}
			ErrCode = IA_SUCCESS;
			break;

		default :
			IrCa_Debug_Err(("TP_Desc_IO  [ invalid  wOpCode=%d ]\n",wOpCode));
			ErrCode = VD_NOT_SUPPORTED;
			break;
	}
	
	return ErrCode;
}


