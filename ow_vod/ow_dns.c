/*----------------------------------------------------------------------------
   File Name:ow_dns.c

   Description: 

   Version:REL_V1.0.0

   Copyright (c) 2006-2100 Triumph Corporation.  All rights reserved.
-----------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */

#include "ow_debug.h"

#define DNS_BUF_SIZE 	(1024)
/* private variables ------------------------------------------------------ */

static PARSEDNS ParseBuf;
static char *DNSBuf = NULL;

char sends[SEND_BUF_LEN] = {0};
char recvs[RECV_BUF_LEN] =  {0};



/* ----------------------------------------------------------------------------
   Name: DNS_ResetParseBuf()

   Description:
   Default and reset DNS buf parameters in PARSEDNS.

   Parameters:

   Return Value:
   ---------------------------------------------------------------------------- */

void DNS_DefaultParseBuf(PARSEDNS * buf)
{
	FYF_API_Memset((char*)buf, 0, sizeof(PARSEDNS));
}

/* ----------------------------------------------------------------------------
   Name: DNS_CreateDNSPacket()

   Description:
   Create one packet for DNS.

   Parameters:
   addr:                        the pointer to the name of destination[IN]
   DNSsendBuff:                 the pointer to DNS buf[OUT]
   bufLen:                      the pointer to value of the buf's len[OUT]

   Return Value:
   ---------------------------------------------------------------------------- */

void DNS_CreateDNSPacket(char *addr,char *DNSSendBuff,char *bufLen)
{
	char len = 0;
	//int i = 0;
	unsigned char dianCount = 0;
	char nameLen[10] = {0};//记录每个点前字符的个数
	char tempOff = 0;
	unsigned char srcOff = 0;
	char *dataPointer = NULL,*dataTemp = NULL;
	
	
	/***为标识字段**/
    DNSSendBuff[0] = 0x3e;
	DNSSendBuff[1] = 0x3a;

	/**为标志字段，该字段设置了TC表示该报文是可截断的***/
    DNSSendBuff[2] = 0x01;
	DNSSendBuff[3] = 0x00;

	/**查询报文数量为1***/
    DNSSendBuff[4] = 0x00;
	DNSSendBuff[5] = 0x01;

	/**表示回答，授权和额外信息都为0***/
    DNSSendBuff[6] = 0x00;
	DNSSendBuff[7] = 0x00;
    DNSSendBuff[8] = 0x00;
	DNSSendBuff[9] = 0x00;
    DNSSendBuff[10] = 0x00;
	DNSSendBuff[11] = 0x00;
	
	srcOff = 12;


	/**赋值地址**/

	dataTemp = addr;
	while(1)
	{
		if(NULL == (dataPointer = strstr(dataTemp,".")))
		{
			nameLen[dianCount] = (char) (addr + strlen(addr) - dataTemp);
			memset(&DNSSendBuff[srcOff + tempOff],nameLen[dianCount],1);
		
			memcpy(&DNSSendBuff[srcOff + tempOff +1],dataTemp,nameLen[dianCount]);
			dianCount++;
			break;
		}
		else
		{
			nameLen[dianCount] = (char) (dataPointer - dataTemp );
			memset(&DNSSendBuff[srcOff + tempOff], nameLen[dianCount],1);
		
			memcpy(&DNSSendBuff[srcOff + tempOff + 1],dataTemp,nameLen[dianCount]);
			tempOff += (nameLen[dianCount] + 1);
			dianCount++;
			dataTemp = dataPointer + 1;
		}
	}


	len = strlen(addr);
	srcOff +=(len+1);
	DNSSendBuff[srcOff] = 0x00;

	/***为类型，1表示A查询****/
	DNSSendBuff[srcOff+1] = 0x00;
	DNSSendBuff[srcOff+2] = 0x01;

	/***为类型，1表示Internet数据****/
	DNSSendBuff[srcOff+3] = 0x00;
	DNSSendBuff[srcOff+4] = 0x01;


	*bufLen = srcOff+5 ;

}


/* ----------------------------------------------------------------------------
   Name: DNS_ParseDNSRecBuf()

   Description:
   Parse the buf rec from the DNS server to get ip.

   Parameters:
   buf:                         the pointer to buf reced from DNS server[IN]
   bufLen:                      the len of buf[in]
   ipAddr:                      the pointer to ip addr[OUT]

   Return Value:
   ---------------------------------------------------------------------------- */


int DNS_ParseDNSRecBuf(char *buf, char *ipString, unsigned int *ipValue, int allLen, char *ipAddr)
{
	unsigned short offset = 42,srcOff=0;
	unsigned short dataLen = 0,lenTemp = 0;
	int i=0;
	int dataCount = 0;

	DNS_DefaultParseBuf(&ParseBuf);
	
	
	ParseBuf.head.id = (buf[0] << 8) | (buf[1]);
	ParseBuf.head.flags = (buf[2] << 8) | (buf[3]);
	ParseBuf.head.questNum = (buf[4] << 8) | (buf[5]);
	ParseBuf.head.answerNum = (buf[6] << 8) | (buf[7]);
	
	if(ParseBuf.head.answerNum == 0)
	{
		OW_ERROR("refuse dns.\n");
		return FYF_ERR;
	}
	ParseBuf.head.authorNum = (buf[8] << 8) | (buf[9]);
	ParseBuf.head.additionNum = (buf[10] << 8) | (buf[11]);
	
	offset = 12;
	while(1)
	{
		lenTemp = buf[offset];
		memcpy(&(ParseBuf.head.name[srcOff]), &(buf[offset + 1]), lenTemp);

		srcOff += lenTemp;
		memset(&(ParseBuf.head.name[srcOff]), '.', 1);
		srcOff += 1;
		offset = offset + lenTemp + 1;
		if(buf[offset] == 0)
		{
			break;
		}
	}

	offset += 1;

	ParseBuf.types.classs = (buf[offset] << 8) | (buf[offset + 1]);
	ParseBuf.types.type = (buf[offset + 2] << 8) | (buf[offset + 3]);

	offset += 4;
	dataLen = allLen - offset;

	while(allLen > offset)
	{
		ParseBuf.res[dataCount].yuPointer = (buf[offset] << 8) | (buf[offset + 1]);
		ParseBuf.res[dataCount].name = (buf[offset + 2] << 8) | (buf[offset + 3]);
		ParseBuf.res[dataCount].type = (buf[offset + 4] << 8)| (buf[offset + 5]);
		ParseBuf.res[dataCount].ttl = (buf[offset + 6] << 24) | (buf[offset + 7] << 16) | (buf[offset + 8] << 8) | (buf[offset + 9]);
		ParseBuf.res[dataCount].length = (buf[offset + 10] << 8) | (buf[offset + 11]);
		
		offset += 12;

		if(ParseBuf.res[dataCount].length == 4)//说明后面跟着ip地址
		{
			ParseBuf.res[dataCount].addr = ((unsigned char)buf[offset] << 24) | ((unsigned char)buf[offset + 1] << 16)|((unsigned char)buf[offset + 2] << 8) | ((unsigned char)buf[offset + 3]);
			sprintf((char*)ParseBuf.res[dataCount].ipStr, "%d.%d.%d.%d", (unsigned char)buf[offset], (unsigned char)buf[offset + 1], (unsigned char)buf[offset + 2], (unsigned char)buf[offset + 3]);
			offset += ParseBuf.res[dataCount].length;
			dataCount++;
		}
		else
		{
			offset += ParseBuf.res[dataCount].length;
		}
	}
	
	for(i = 0; i < dataCount; i++)
	{
		OW_DEBUG("host ip:%s\n",ParseBuf.res[i].ipStr);
	}
	
	if((ipAddr != NULL) && (dataCount > 0))
	{
		sprintf(ipAddr, "%s", ParseBuf.res[0].ipStr);
		OW_DEBUG("get tsg ip success: %s \n",ipAddr);
	}
	else
	{
		OW_ERROR("get tsg ip err.\n");
	}

	return FYF_OK;
	
}


/* ----------------------------------------------------------------------------
   Name: DNS_GetIPByDNS(char *ipAddName,char *ipAddr)

   Description:
   Get detination's  ip addr by its name.

   Parameters:
   ipAddName:                   the pointer to name of detination[IN]
   ipAddr:                      the pointer to ip addr[OUT]

   Return Value:
   ---------------------------------------------------------------------------- */
char tmpip[20] = {0};

int DNS_GetIPByDNS(char *ipAddName, char *ipAddr)
{
    int iBytes,status;
    int s;
	char buflen = 0;
	unsigned int ipValue = 0;
    struct sockaddr_in ServerAddr;
	int ip;
	int res;
	int ret = -1;

	printf("DNS_GetIPByDNS\n");
	if((ipAddName == NULL) || (ipAddr == NULL))
	{
		OW_ERROR("DNS_GetIPByDNS  (ipAdd == NULL) || (ipAddr == NULL).\n");
		return FYF_ERR;
	}
	
	if (NULL == DNSBuf)
	{
		DNSBuf = (char *) malloc(DNS_BUF_SIZE);
	}

	if (NULL == DNSBuf)
	{
		OW_ERROR("DNS_GetIPByDNS  NULL == DNSBuf Error.\n");
		return FYF_ERR;
	}

	
#if 1			/*hqx:do it later*/
	OW_VOD_SetDhcpFlag(1);
#endif
	if(OW_VOD_GetDhcpFlag())
	{
		FYF_API_ETH_DNSGet(tmpip);
		RTSP_ParseIpString2Value(tmpip, &ip);
	}
	else
	{
		//TP_Get_ManualDNSserver(0);
		//RTSP_ParseIpString2Value(char * ipAdd,int * ipValue)(dnsbuf1, &ip);
	}
	
	printf("dnsip = %x\n", ip);

	s = socket(AF_INET, SOCK_DGRAM, 0);
	ServerAddr.sin_family 		= AF_INET;
	ServerAddr.sin_port 		= htons(DNS_PORT);
	ServerAddr.sin_addr.s_addr 	= ntohl(ip);


	ret = RTSP_SOCKET_SetNoneBlock(s);
	if (ret < 0)
	{
		return FYF_ERR;
	}

	status = connect(s, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
	/*if (0 != (connect(s, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr))))
	{
		OW_ERROR("DNS_GetIPByDNS connect Error.\n");
	}*/
	/*else
	{
		FYF_API_Memset(DNSBuf, 0, DNS_BUF_SIZE);
		DNS_CreateDNSPacket(ipAddName, DNSBuf, &buflen);

		if ((res = RTSP_SOCKET_Send(s, DNSBuf, buflen)) < 0)
		{
			TP_Print_Debug(ow_rtsp, ow_dos, "TP_GetIPByDNS RTSP_Send err!!!\n");
		}
		else
		{
			printf("send %d,sends = %s \n ", res, DNSBuf);
		    FD_ZERO(&fds_e);
			FD_ZERO(&fds_r);
			FD_SET(s, &fds_r); 
			
			for (timeOut = 0;timeOut < 6;timeOut++)
			{
			    nactive = select(s+1, &fds_r, NULL, &fds_e, &tm);
				if (nactive == 0)
				{
					printf("dns select timeout !!!!\n");
				}
				else if (nactive > 0)
				{
					if (FD_ISSET(s, &fds_r)) 
					{
						memset(DNSBuf, 0, DNS_BUF_SIZE);
						if ((res = RTSP_Rec(s, DNSBuf, REC_BUF_LEN, 100)) < 0)
						{
							TP_Print_Debug(ow_rtsp, ow_dos, "TP_GetIPByDNS TP_OW_Rec err!!!\n");
						}
						else
						{
							printf("host name: %s \n", ipAddName);
							if (DNS_ParseDNSRecBuf(DNSBuf, res, ipAddr) == TP_FAIL)
							{
								TP_Print_Debug(ow_rtsp, ow_dos, "TP_GetIPByDNS TP_ParseDNSRecBuf err!!!\n");
							}
							else
							{
								close(s);
								if (NULL != DNSBuf)
								{
									free(DNSBuf);
									DNSBuf = NULL;
								}
								return TP_SUCCESS;
							}
						}
					}
				}
			}
		}
	}
*/
	FYF_API_Memset(sends, 0, SEND_BUF_LEN);
	FYF_API_Memset(recvs, 0, RECV_BUF_LEN);
    DNS_CreateDNSPacket(ipAddName, sends, &buflen); 
    	

	if((iBytes = RTSP_SOCKET_Send(s, sends, buflen)) < 0)
	{
		OW_ERROR("DNS_GetIPByDNS RTSP_SOCKET_Send Error, iBytes = %d\n", iBytes);
		close(s);
		return FYF_ERR;
	}

   	printf("send %d, sendbuf = %s\n",iBytes, sends);
	/*for(i = 0; i < iBytes; i++)
	{
		printf("%x,",(unsigned char)sends[i]);
		 if(i%25 == 0&&i!=0)
			 printf("\n");
	}
	printf("\n");*/
	
	printf("DNS_GetIPByDNS, ipaddr = %s, ipaddr = %s\n", ipAddName, ipAddr);
	if((iBytes = RTSP_SOCKET_Receive(s, recvs, RECV_BUF_LEN, 100))<0)
	{
		OW_ERROR("DNS_GetIPByDNS RTSP_SOCKET_Receive Error, iBytes = %d\n", iBytes);
		OW_VOD_SetNetErrFlag(FYF_TRUE);
		close(s);
		return FYF_ERR;
	}
	
    OW_DEBUG("recv %d,recvBuff = %s\n",iBytes, recvs);
	/*for(i = 0; i < iBytes; i++)
	{
		printf("%x,",(unsigned char)recvs[i]);
		 if(i%25 == 0&&i!=0)
			 printf("\n");
	}
	printf("\n");*/

	OW_DEBUG("host name: %s \n", ipAddName);

	close(s);

	if(DNS_ParseDNSRecBuf(recvs, 0, &ipValue, iBytes, ipAddr) == FYF_ERR)
	{
		OW_ERROR("DNS_GetIPByDNS DNS_ParseDNSRecBuf Error, iBytes = %d\n", iBytes);
		return FYF_ERR;
	}
	
    return FYF_OK;    
}











