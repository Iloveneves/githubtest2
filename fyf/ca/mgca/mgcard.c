#include "mgdef.h"
#include "../../inc/fyf_def.h"
#include "../../inc/ca/fyf_ca.h"
#include "../../dev/fyf_smc.h"
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Send_Command(MG_U8 *MG_SendComm, MG_U8 MG_CommLen, MG_U8 *MG_RecvResp,MG_U16 *SW, MG_U8 * NumberRead)
{

		BS32 ReturnResult;
		MG_U8 NumReaded=0;
		BU08 status[258];
		BU08 status1[2];		
		BS16 act_size = 0; 
		BU08 cmd[5];
		BU32 i;


		*NumberRead = 0;
#if 1
		printf("byszCommand=");
		for(i=0;i<MG_CommLen;i++)
		{
			printf("%x  ",MG_SendComm[i]);
		}
		printf("\n");
#endif		
		while(1)
		{
			ReturnResult = FYF_API_smc_iso_transfer(MG_SendComm,MG_CommLen, status,1000, &NumReaded);
#if 1
				printf("byszReply=");
				for(i=0;i<NumReaded;i++)
				{
					printf("%x  ",status[i]);
					
				}
				printf("\n");

				printf("NumReaded=%d\n",NumReaded);
#endif			
			if(ReturnResult!=0)
			{
				printf("MG_STBPort_Send_Command fail\n");
				return MG_FAILING;/*´íÎó*/
			}
			status1[0] = status[0];
			status1[1] = status[1];
						
aa:		

			if((status1[0] == 0x90) &&(status1[1] == 0x00))
			{
				*SW = 0x9000;
				*NumberRead = NumReaded-2;
				return MG_SUCCESS;
			}
			else if(status1[0] == 0x61)
			{
				act_size = status1[1];
				cmd[0] = 0;
				cmd[1] = 0xc0;
				cmd[2] = 0;
				cmd[3] = 0;
				cmd[4] = act_size;
				ReturnResult=FYF_API_smc_iso_transfer(cmd,5, status,act_size,  &NumReaded);
#if 1
				printf("byszReply=");
				for(i=0;i<NumReaded;i++)
				{
					printf("%x  ",status[i]);
					
				}
				printf("\n");

				printf("NumReaded=%d\n",NumReaded);
#endif				
				if(ReturnResult!=0)
				{
					printf("MG_STBPort_Send_Command fail\n");
					return MG_FAILING;/*´íÎó*/
				}
				status1[0] = status[0];
				status1[1] = status[1];
				goto aa;
				
			}
			else
			{
				*SW = ((MG_U16)status[NumReaded-2])<<8|status[NumReaded-1];
	
				memcpy(MG_RecvResp,status,NumReaded-2);
				
				*NumberRead = NumReaded-2;				
				
				return MG_SUCCESS;
			}
		}

		return MG_SUCCESS;/*³É¹¦*/


}

