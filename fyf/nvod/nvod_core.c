#include "nvod_core.h"


static FYF_nvod_ref_service_sp pfyf_nvod_ref_service = 0;
static FYF_time_shifted_service_sp pfyf_time_shifted_service = 0;
static unsigned int fyf_nvod_ref_service_index[FYF_NVOD_REF_SERVICE_NUM];
static unsigned int fyf_nvod_ref_service_num;
static FYF_nvod_data_s fyf_nvod_data;

void FYF_API_service_descriptor(unsigned char * data,FYF_service_descriptor_sp pDescriptor)
{
	unsigned char * pBuf;
	unsigned int len,copyLen;
	switch(data[0])
	{
	case FYF_SERVICE_TYPE_NVOD_REF:
		pDescriptor->service_type = FYF_SERVICE_TYPE_NVOD_REF;
		
		len = data[1];
		copyLen = (len>SERVICE_PROVIDER_NAME_LEN)?SERVICE_PROVIDER_NAME_LEN:len;
		memset (pDescriptor->service_data.service_type_nvod_ref.service_provider_name, 0, SERVICE_PROVIDER_NAME_LEN);
		if(copyLen)
		{
			memcpy (pDescriptor->service_data.service_type_nvod_ref.service_provider_name, &data[2], copyLen);
			pDescriptor->service_data.service_type_nvod_ref.service_provider_name[SERVICE_PROVIDER_NAME_LEN-1] = 0;	
		}

		pBuf = data+len+2;
		len = pBuf[0];
		copyLen = (len>SERVICE_NAME_LEN)?SERVICE_NAME_LEN:len;
		memset (pDescriptor->service_data.service_type_nvod_ref.service_name, 0, SERVICE_NAME_LEN);
		if(copyLen)
		{
			memcpy (pDescriptor->service_data.service_type_nvod_ref.service_name, &pBuf[1], copyLen);
			pDescriptor->service_data.service_type_nvod_ref.service_name[SERVICE_NAME_LEN-1] = 0;
		}
		break;
	default:		
		pDescriptor->service_type = FYF_SERVICE_TYPE_UNKNOWN;
		break;
	}
}

void FYF_API_nvod_ref_service_descriptor(unsigned short service_id,unsigned char *data)
{
	unsigned int index,existed;
	FYF_service_descriptor_s descriptor;

	if(pfyf_nvod_ref_service == 0)
	{
		return;
	}
	existed = 0;
	for(index = 0;index<FYF_NVOD_REF_SERVICE_NUM;index++)
	{
		if(pfyf_nvod_ref_service[index].service_id == service_id)
		{
			existed = 1;
			break;
		}
	}
	if(existed)
	{
		return;
	}
	for(index = 0;index<FYF_NVOD_REF_SERVICE_NUM;index++)
	{
		if(pfyf_nvod_ref_service[index].used == 0)
		{
			break;
		}
	}
	if(FYF_NVOD_REF_SERVICE_NUM == index)
	{
		return;
	}
	FYF_API_service_descriptor (data, &descriptor);
	{
		strcpy (pfyf_nvod_ref_service[index].service_provider_name, descriptor.service_data.service_type_nvod_ref.service_provider_name);
		strcpy (pfyf_nvod_ref_service[index].service_name,descriptor.service_data.service_type_nvod_ref.service_name);
		pfyf_nvod_ref_service[index].service_id = service_id;
		pfyf_nvod_ref_service[index].used = 1;
	}
}

void FYF_API_time_shifted_service_descriptor(unsigned short service_id,unsigned char * data)
{
	unsigned int index,existed;

	if(pfyf_time_shifted_service==0)
	{
		return;
	}
	existed = 0;
	for(index = 0;index<FYF_NVOD_TIME_SHIFTED_SERVICE_NUM;index++)
	{
		if(pfyf_time_shifted_service[index].service_id == service_id)
		{
			existed = 1;
			break;
		}
	}
	if(existed)
	{
		return;
	}
	for(index = 0;index<FYF_NVOD_TIME_SHIFTED_SERVICE_NUM;index++)
	{
		if(pfyf_time_shifted_service[index].used == 0)
		{
			break;
		}
	}
	if(FYF_NVOD_TIME_SHIFTED_SERVICE_NUM == index)
	{
		return;
	}
	pfyf_time_shifted_service[index].reference_service_id = (data[0]<<8) | data[1];
	pfyf_time_shifted_service[index].service_id = service_id;
	pfyf_time_shifted_service[index].used = 1;
}

//void FYF_API_time_save_pid(unsigned short service_id,unsigned short vpid,unsigned short apid,unsigned short pcrpid)
void FYF_API_time_save_pid(FYF_time_shifted_service_sp ShiftServiceContents)
{
	unsigned int index;
	
	for(index = 0;index<FYF_NVOD_TIME_SHIFTED_SERVICE_NUM;index++)
	{
		if(pfyf_time_shifted_service[index].service_id == ShiftServiceContents->service_id && pfyf_time_shifted_service[index].used == 1)
		{
			break;
		}
	}
	if(index!=FYF_NVOD_TIME_SHIFTED_SERVICE_NUM)
	{
		pfyf_time_shifted_service[index].vpid= ShiftServiceContents->vpid;
		pfyf_time_shifted_service[index].apid= ShiftServiceContents->apid;
		pfyf_time_shifted_service[index].pcrpid= ShiftServiceContents->pcrpid;
		pfyf_time_shifted_service[index].ecmPID= ShiftServiceContents->ecmPID;
		pfyf_time_shifted_service[index].audioEcmPid= ShiftServiceContents->audioEcmPid;
		pfyf_time_shifted_service[index].videoEcmPid= ShiftServiceContents->videoEcmPid;
		pfyf_time_shifted_service[index].sCAT_Sys_ID= ShiftServiceContents->sCAT_Sys_ID;
	}
}

//void FYF_API_time_get_pid(unsigned short service_id,unsigned short *vpid,unsigned short *apid,unsigned short *pcrpid)
unsigned int FYF_API_time_get_pid(unsigned short service_id,FYF_time_shifted_service_sp ShiftServiceContents)
{
	unsigned int ret = 0;
	unsigned int index;
	
	for(index = 0;index<FYF_NVOD_TIME_SHIFTED_SERVICE_NUM;index++)
	{
		if(pfyf_time_shifted_service[index].service_id == service_id && pfyf_time_shifted_service[index].used == 1)
		{
			ret = 1;
			break;
		}
	}
	if(index!=FYF_NVOD_TIME_SHIFTED_SERVICE_NUM)
	{
		ShiftServiceContents->vpid=pfyf_time_shifted_service[index].vpid ;
		ShiftServiceContents->apid=pfyf_time_shifted_service[index].apid ;
		ShiftServiceContents->pcrpid=pfyf_time_shifted_service[index].pcrpid ;
		ShiftServiceContents->ecmPID=pfyf_time_shifted_service[index].ecmPID ;
		ShiftServiceContents->audioEcmPid=pfyf_time_shifted_service[index].audioEcmPid ;
		ShiftServiceContents->videoEcmPid=pfyf_time_shifted_service[index].videoEcmPid ;
		ShiftServiceContents->sCAT_Sys_ID=pfyf_time_shifted_service[index].sCAT_Sys_ID ;
	}
	return ret;

}

void FYF_API_nvod_init(void)
{
	if(pfyf_nvod_ref_service == 0)
	{
		pfyf_nvod_ref_service = fyf_nvod_data.fyf_nvod_ref_service;//malloc (sizeof(FYF_nvod_ref_service_s)*FYF_NVOD_REF_SERVICE_NUM);
	}
	if(pfyf_time_shifted_service == 0)
	{
		pfyf_time_shifted_service = fyf_nvod_data.fyf_time_shifted_service;//malloc (sizeof(FYF_time_shifted_service_s)*FYF_NVOD_TIME_SHIFTED_SERVICE_NUM);
	}
	if(0xff == fyf_nvod_data.flag)
	{
		memset (pfyf_nvod_ref_service, 0, sizeof(FYF_nvod_ref_service_s)*FYF_NVOD_REF_SERVICE_NUM);
		memset (pfyf_time_shifted_service, 0, sizeof(FYF_time_shifted_service_s)*FYF_NVOD_TIME_SHIFTED_SERVICE_NUM);
		fyf_nvod_data.flag = 0x00;
	}
}

unsigned int FYF_API_nvod_num(void)
{
	unsigned int index;

	if(pfyf_nvod_ref_service == 0)
	{
		return 0;
	}
	fyf_nvod_ref_service_num = 0;
	for(index = 0;index<FYF_NVOD_REF_SERVICE_NUM;index++)
	{
		if(pfyf_nvod_ref_service[index].used == 1)
		{
			fyf_nvod_ref_service_index[fyf_nvod_ref_service_num]=index;
			fyf_nvod_ref_service_num++;
		}
	}
	return fyf_nvod_ref_service_num;
}

void FYF_API_nvod_ref_service(unsigned int index,unsigned short * service_id,unsigned char** service_provider_name,unsigned char** service_name)
{
	if(pfyf_nvod_ref_service == 0)
	{
		return;
	}
	*service_id = pfyf_nvod_ref_service[index].service_id;
	if(service_provider_name != 0)
	{
		*service_provider_name = pfyf_nvod_ref_service[index].service_provider_name;
	}
	if(service_name != 0)
	{
	*service_name = pfyf_nvod_ref_service[index].service_name;
	}
}

unsigned int FYF_API_time_shifted_service(unsigned short ref_service_id,unsigned short * service_id,unsigned int len)
{
	unsigned int index,num;

	num = 0;
	if(pfyf_time_shifted_service == 0)
	{
		return 0;
	}
	for(index = 0;index<FYF_NVOD_TIME_SHIFTED_SERVICE_NUM;index++)
	{
		if(pfyf_time_shifted_service[index].reference_service_id == ref_service_id)
		{
			if(num<len)
			{
				service_id[num] = pfyf_time_shifted_service[index].service_id;
				num++;
			}
		}
	}
	return num;
}

void FYF_API_nvod_test(void)
{
	unsigned int nvod_num;
	unsigned int index,service_id_num,service_id_index;
	unsigned short ref_service_id;
	unsigned char * service_provider_name,*service_name;
	unsigned short service_id[10];
	
	nvod_num = FYF_API_nvod_num ();
	printf("nvod ref service num = %d\n",nvod_num);
	for(index = 0;index<nvod_num;index++)
	{
		FYF_API_nvod_ref_service (index, &ref_service_id, &service_provider_name, &service_name);
		printf("---ref service no %d:\n",index);
		printf("ref_service_id = 0x%x\n",ref_service_id);
		printf("service_provider_name = %s\n",service_provider_name);
		printf("service_name = %s\n",service_name);
		service_id_num = FYF_API_time_shifted_service (ref_service_id, service_id, 10);
		printf("service_id_num = %d, time shifted service id:", service_id_num);
		for(service_id_index = 0;service_id_index<service_id_num;service_id_index++)
		{
			printf("0x%x ",service_id[service_id_index]);
		}
		printf("\n");
	}	

}

unsigned int FYF_API_get_nvod_data(unsigned char ** buf)
{
	*buf=(unsigned char *)&fyf_nvod_data;
	return sizeof(FYF_nvod_data_s);
}

void FYF_API_set_nvod_data(unsigned char * buf)
{
	memcpy (&fyf_nvod_data, buf, sizeof(FYF_nvod_data_s));
}

void FYF_API_InitNvodInfo(void)
{
	memset(pfyf_nvod_ref_service, 0, sizeof(FYF_nvod_ref_service_s)*FYF_NVOD_REF_SERVICE_NUM);
	memset (pfyf_time_shifted_service, 0, sizeof(FYF_time_shifted_service_s)*FYF_NVOD_TIME_SHIFTED_SERVICE_NUM);
	fyf_nvod_data.flag = 0x00;
}



