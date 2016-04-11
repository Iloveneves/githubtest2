/*===============================================================================

 File Name:

 Description:

 History:

===============================================================================*/

/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/
#include <sys_config.h>
#include <retcode.h>
#include <types.h>
#include <api/libc/alloc.h>
#include <api/libc/printf.h>
#include <api/libc/string.h>
#if 0 /* 090106 */
/*----------------------------------------------------------------------
	Global Variables
----------------------------------------------------------------------*/
void UI_APP_OTA_BootUpSetParams(UINT32 freq,UINT32 symb,UINT32 pid,UINT8 mode, UINT8 otaType);
static void (* __glb_CA_DVT_OTA_SetParams) (UINT32,UINT32,UINT32,UINT8,UINT8) = UI_APP_OTA_BootUpSetParams;
/*-------------------------------------------------------------------------------

	Description:  
                 
                 DVT Description:
                 
                 DVTLoader _descriptor( ){       
                 	    descriptor_tag                       8 uimsbf  0x85 
                 		descriptor_length                    8 uimsbf   
                 		For(i=1; i < N; i++){     
                 			    STBProviderID                8 bslbf   
                 				STB_DataLen                  8 bslbf   
                 				frequency                    32 bslbf   
                 				modulation                   8 bslbf   
                 				symbol_rate                  28 bslbf   
                 				Reserved                     4 bslbf   
                 				DataPID                      16 uimsbf   
                 				For(j=0; j < M; j++){     
                 					PrivateData              8 bslbf   
                 			}     
                 	}   
                 }   

                 Private Data:
                 HardwareVersion      16        Uimsbf
                 SoftwareVersion      16        Uimsbf
                 TableId               8        Uimsbf
                 Start Serial No      40        Uimsbf
                 End Serial No        40        Uimsbf


	Paramenters:  @para 
	
	Return:           

-------------------------------------------------------------------------------*/
#endif
RET_CODE ca_dvt_ota_analyseDes (UINT32 lenTotal, UINT8 * dataBuf)
{
#if 0 /* 090106 */
    UINT32  freq;
    UINT8   modulation;
    UINT32  dataPid;
    UINT32  symbolRate, tempTotalLen = lenTotal;
    UINT16  des_len, tempLen;
	UINT32  tempCount;
	UINT8 * loop1_data;
	UINT8   loop1_len;

	while (lenTotal > 0)
	{
		des_len = dataBuf[1] + 2;
		tempLen = des_len;
		if ((tempTotalLen < lenTotal)||(lenTotal < (UINT32)(lenTotal - des_len)))
		{
		    printf("OTA while 2 error\n");
			return RET_STA_ERR;
		}
		lenTotal -= des_len;
		if(SYS_DVT_DES_ID == dataBuf[0])
		{
		    loop1_data = &dataBuf[2];
			des_len    -= 2;
		    loop1_len  = loop1_data[1] + 2;
			while (des_len > 0)
			{
				if ( SYS_MANUFACTURER_ID == loop1_data[0])
				{
					if (((((loop1_data[13]<<8)&0xFF00)|loop1_data[14]) == SYS_HW_VERSION) &&\
						((((loop1_data[15]<<8)&0xFF00)|loop1_data[16]) >  SYS_SW_VERSION))//Still need to Check serial No
					{
						tempCount = (loop1_data[2]&0xF0)>>4;
						tempCount *= 10;
						tempCount += loop1_data[2]&0x0F;
						tempCount *= 10;
						tempCount += (loop1_data[3]&0xF0)>>4;
						tempCount *= 10;
						tempCount +=loop1_data[3]&0x0F;
						freq = tempCount;//*100;
						
						modulation = loop1_data[6] - 1;// + 3;
						
						tempCount = (loop1_data[7]&0xF0)>>4;
						tempCount *= 10;
						tempCount += loop1_data[7]&0x0F;
						tempCount *= 10;
						tempCount += (loop1_data[8]&0xF0)>>4;
						tempCount *= 10;
						tempCount +=loop1_data[8]&0x0F;
						tempCount *= 10;
						//tempCount +=(loop1_data[9]&0xF0)>>4;
						//tempCount *= 10;
						//tempCount += loop1_data[9]&0x0F;
						symbolRate = tempCount;
						
						dataPid = ((loop1_data[11]<<8)&0xFF00)|loop1_data[12];
						
						__glb_CA_DVT_OTA_SetParams(freq, symbolRate, dataPid, modulation,0);
						return RET_SUCCESS;
					}
				}
				
				loop1_data += loop1_len;
				if ((tempTotalLen < des_len) || (des_len < (UINT32)(des_len - loop1_len)))
        		{
        		    printf("OTA while 3 error\n");
        			return RET_STA_ERR;
        		}    
				des_len    -= loop1_len;
				loop1_len  = loop1_data[1] + 2;		
				
			}	
		}
		dataBuf += tempLen;
	}
	return RET_FAILURE;
 #else
	return RET_FAILURE;
#endif
}



#if 0
static UINT8 testarray[] = {0x65,0x1c, 0x40, 0x1a, 0x06,0x42,0x00,0x00, 0x03, 0x00, 0x68, 0x75, 0x00, 0x04, 0xfd, 0x00,0x10, 0x00, 0x11,0x60, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x10, 0x20,\
0x85,0x1c, 0x30, 0x1a, 0x06,0x42,0x00,0x00, 0x03, 0x00, 0x68, 0x75, 0x00, 0x04, 0xfd, 0x00,0x10, 0x00, 0x11,0x60, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x10, 0x20,\
0x85,0x38, 0x41, 0x1a, 0x06,0x42,0x00,0x00, 0x03, 0x00, 0x68, 0x75, 0x00, 0x04, 0xfd, 0x00,0x10, 0x00, 0x11,0x60, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x10, 0x20, 0x40, 0x1a, 0x06,0x42,0x00,0x00, 0x03, 0x00, 0x68, 0x75, 0x00, 0x04, 0xfd, 0x00,0x10, 0x00, 0x11,0x60, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x10, 0x20, };
void ca_dvt_ota_analyseTest ()
{
    ca_dvt_ota_analyseDes(sizeof(testarray),testarray);
}
#endif

