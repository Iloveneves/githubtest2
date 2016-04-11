

#define FYF_NVOD_REF_SERVICE_NUM (10)
#define FYF_NVOD_TIME_SHIFTED_SERVICE_NUM (100)

enum
{
	FYF_SERVICE_TYPE_TV = 0x01,	/* 数字电视业务 */
	FYF_SERVICE_TYPE_RADIO = 0x02, /* 数字音频广播业务 */
	FYF_SERVICE_TYPE_NVOD_REF = 0x04, /* NVOD参考业务 */
	FYF_SERVICE_TYPE_UNKNOWN = 0x80	/* 未知业务 */
}FYF_SERVICE_TYPE_e;
#define SERVICE_PROVIDER_NAME_LEN   (0x20)
#define SERVICE_NAME_LEN            (0x20)
/* 业务描述符 */
typedef struct
{
	unsigned char service_type;
	union
	{
		struct
		{
			unsigned char service_provider_name[SERVICE_PROVIDER_NAME_LEN];
			unsigned char service_name[SERVICE_NAME_LEN];
		}service_type_nvod_ref;
	}service_data;
}FYF_service_descriptor_s;
typedef FYF_service_descriptor_s * FYF_service_descriptor_sp;
/* nvod参考业务 */
typedef struct
{
	unsigned short service_id;
	unsigned char service_provider_name[SERVICE_PROVIDER_NAME_LEN];
	unsigned char service_name[SERVICE_NAME_LEN];
	unsigned char used;
}FYF_nvod_ref_service_s;
typedef FYF_nvod_ref_service_s * FYF_nvod_ref_service_sp;
/* nvod时移业务 */
typedef struct
{
	unsigned short service_id;
	unsigned short reference_service_id;
	unsigned short vpid;
	unsigned short apid;
	unsigned short pcrpid;
	unsigned short ecmPID;
	unsigned short audioEcmPid;
	unsigned short videoEcmPid;
	unsigned short sCAT_Sys_ID;
	unsigned char used;
}FYF_time_shifted_service_s;
typedef FYF_time_shifted_service_s * FYF_time_shifted_service_sp;

typedef struct
{
	unsigned char flag;
	FYF_nvod_ref_service_s fyf_nvod_ref_service[FYF_NVOD_REF_SERVICE_NUM];
	FYF_time_shifted_service_s fyf_time_shifted_service[FYF_NVOD_TIME_SHIFTED_SERVICE_NUM];
}FYF_nvod_data_s;
