#include <stdio.h>
#include <time.h>

#include "app_common.h"
#include "app_ipi_common.h"

BU32 semaphoreSQL = 0;

BS08 g_StrSQL[APP_STR_SQL_LENGTH];

static sqlite3 *db;
static char *zErrMsg = 0;
static int rc;

static char **selectResult = 0;//�洢��ѯ������Ϣ
static int nResult = 0;		//����
static int mResult = 0;		//����
static BS32 iResult = 0;		//��ǰ��

//���ݱ�����FLASHλ�ñ�־
#ifdef FLASH_8M
#define FYF_FLS_USER_DATA_SIZE  (1024*64)
#else
#define FYF_FLS_USER_DATA_SIZE  (1024*128)
#endif
static BU08 FYF_FLS_USER_DATA[FYF_FLS_USER_DATA_SIZE];
static FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;
extern NetSetInfo IpInfomation;
/*
���ڽ����ݿ�д��flash
*/
void SQL_API_SaveToFlash(const char * fileName)
{
#ifdef ENABLE_NVOD
	unsigned char * nvod_buf;
	unsigned int nvod_buf_size = 0;
#endif
	MFS_HANDLE handle;
	BS32 flashHandle;
	int size;
	char sizeInFlash[4];
	int index;
	BU08 dba_flag_tmp;

	SQL_API_Semaphore_Wait();	
	FYF_API_printf("SQL_API_SaveToFlash Enter\n");
	FYF_FLS_IOCTL_param.bufferSize = FYF_FLS_USER_DATA_SIZE;
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_START_SAVEFLASH, 0, 0);

	handle = MFS_API_Open(fileName,0);
	size = MFS_API_GetSize(handle);


	FYF_API_printf("--------------DBA size = %d--------------\n", size);

	sizeInFlash[0] = (size>>24)&0xff;
	sizeInFlash[1] = (size>>16)&0xff;
	sizeInFlash[2] = (size>>8)&0xff;
	sizeInFlash[3] = size&0xff;

	FYF_API_Memset (FYF_FLS_IOCTL_param.buffer, 0, FYF_FLS_USER_DATA_SIZE);
	MFS_API_Read(handle,FYF_FLS_IOCTL_param.buffer+5,size);
#ifdef ENABLE_NVOD
	nvod_buf_size = FYF_API_get_nvod_data (&nvod_buf);
	memcpy(FYF_FLS_IOCTL_param.buffer+5+size, nvod_buf, nvod_buf_size);
#endif

	memcpy(FYF_FLS_IOCTL_param.buffer + 60 * 1024, &IpInfomation, sizeof(IpInfomation));

 	FYF_FLS_IOCTL_param.buffer[0]=0;
	FYF_FLS_IOCTL_param.buffer[1]=sizeInFlash[0];
	FYF_FLS_IOCTL_param.buffer[2]=sizeInFlash[1];
	FYF_FLS_IOCTL_param.buffer[3]=sizeInFlash[2];
	FYF_FLS_IOCTL_param.buffer[4]=sizeInFlash[3];
	FYF_FLS_IOCTL_param.buffer[FYF_FLS_USER_DATA_SIZE-1]=0x47;
	for(index = 1;index < FYF_FLS_USER_DATA_SIZE;index++)
	{
		FYF_FLS_IOCTL_param.buffer[0] += FYF_FLS_IOCTL_param.buffer[index];		//CRC����
	}
 
	if(0xaa == app_system_data.dba_flag)
	{
#if (GUI_PLATFORM == Hisi)
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_SYSDATABK, (BU32)&FYF_FLS_IOCTL_param);
#else
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, 0x1b0000, (BU32)&FYF_FLS_IOCTL_param);
#endif
		app_system_data.dba_flag = 0xbb;
 	}
	else if(0xbb == app_system_data.dba_flag || 0xff == app_system_data.dba_flag)
	{
#if (GUI_PLATFORM == Hisi)
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_SYSDATA, (BU32)&FYF_FLS_IOCTL_param);
#else
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, 0x1a0000, (BU32)&FYF_FLS_IOCTL_param);
#endif
		app_system_data.dba_flag = 0xaa;
 	}
	else
	{
		APP_PRINT_ERROR("dba_flag error!!!!!!!!!\n");
	}

	APP_SYS_API_SaveSysInfo ();
	MFS_API_Close(handle);	
	FYF_API_printf("SQL_API_SaveToFlash Out\n");	
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_END_SAVEFLASH, 0, 0);

	SQL_API_Semaphore_Release ();
}

/*
���ڴ�flash�������ݿ�
*/
void SQL_API_LoadFromFlash(const char * fileName)
{
	MFS_HANDLE handle;
	int size;
	BU08 isFlashNull = 0;
	unsigned char sizeInFlash[4];
	int index; 

	SQL_API_Semaphore_Wait();
	FYF_FLS_IOCTL_param.bufferSize = FYF_FLS_USER_DATA_SIZE;
	if (0xff == app_system_data.dba_flag)
	{ 
	//	app_system_data.dba_flag = 0xaa;//��һ�ο���
		APP_PRINT_DEBUG("��һ�ο���\n");
		APP_SYS_API_SaveSysInfo ();
	}
	else
	{
		if(0xaa == app_system_data.dba_flag)
		{
#if (GUI_PLATFORM == Hisi)
			FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_SYSDATA, (BU32)&FYF_FLS_IOCTL_param);
#else
			FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, 0x1a0000, (BU32)&FYF_FLS_IOCTL_param);
#endif
		}
		else if(0xbb == app_system_data.dba_flag)
		{
#if (GUI_PLATFORM == Hisi)
			FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_SYSDATABK, (BU32)&FYF_FLS_IOCTL_param);
#else
			FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, 0x1b0000, (BU32)&FYF_FLS_IOCTL_param);
#endif
		}

		isFlashNull = 0;
		for(index = 1;index<FYF_FLS_USER_DATA_SIZE;index++)
		{
			isFlashNull += FYF_FLS_IOCTL_param.buffer[index];		//CRC����
		}

		if(isFlashNull != FYF_FLS_IOCTL_param.buffer[0] || 0x47 != FYF_FLS_IOCTL_param.buffer[FYF_FLS_USER_DATA_SIZE-1])			//������һ��
		{
			if(0xaa == app_system_data.dba_flag)
			{
#if (GUI_PLATFORM == Hisi)
				FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_SYSDATABK, (BU32)&FYF_FLS_IOCTL_param);
#else
				FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, 0x1b0000, (BU32)&FYF_FLS_IOCTL_param);
#endif
			}
			else if(0xbb == app_system_data.dba_flag)
			{
#if (GUI_PLATFORM == Hisi)
				FYF_API_fls_ioctl(FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_SYSDATA, (BU32)&FYF_FLS_IOCTL_param);
#else
				FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, 0x1a0000, (BU32)&FYF_FLS_IOCTL_param);
#endif
			}
			isFlashNull = 0;
			for(index = 1;index<FYF_FLS_USER_DATA_SIZE;index++)
			{
				isFlashNull += FYF_FLS_IOCTL_param.buffer[index];		//CRC����
			}
		}

		handle = MFS_API_Open(fileName,0);
		if(app_system_data.dba_flag == 0xaa || app_system_data.dba_flag == 0xbb)
		{
			if(isFlashNull == FYF_FLS_IOCTL_param.buffer[0]  && 0x47 == FYF_FLS_IOCTL_param.buffer[FYF_FLS_USER_DATA_SIZE-1])		//CRC������ȷ
			{

				sizeInFlash[0]=FYF_FLS_IOCTL_param.buffer[1];
				sizeInFlash[1]=FYF_FLS_IOCTL_param.buffer[2];
				sizeInFlash[2]=FYF_FLS_IOCTL_param.buffer[3];
				sizeInFlash[3]=FYF_FLS_IOCTL_param.buffer[4];
				size = (sizeInFlash[0]<<24) | (sizeInFlash[1]<<16) | (sizeInFlash[2]<<8) | (sizeInFlash[3]);

				MFS_API_Write(handle,FYF_FLS_IOCTL_param.buffer+5,size);
#ifdef ENABLE_NVOD
				FYF_API_set_nvod_data (FYF_FLS_IOCTL_param.buffer+5+size);
#endif

				memset(&IpInfomation, 0, sizeof(IpInfomation));
				memcpy(&IpInfomation, FYF_FLS_IOCTL_param.buffer + 60 * 1024, sizeof(IpInfomation));

			}
		}
		MFS_API_Close(handle);
	}

	SQL_API_Semaphore_Release ();
}
static BU32 dba_ipi_init_flag = 0;


void DBA_IPI_Init()
{
	FYF_FLS_IOCTL_param.buffer = FYF_FLS_USER_DATA;
	FYF_FLS_IOCTL_param.bufferSize = FYF_FLS_USER_DATA_SIZE;

	SQL_API_Semaphore_Wait();

	if(dba_ipi_init_flag == 1)
	{
		rc = sqlite3_close (db);
		MFS_API_Remove ("flash.db");
	}

	rc = sqlite3_open(":memory:",&db);

	SQL_API_Exec("attach 'flash.db' as flash");
	SQL_API_Semaphore_Release();

	SQL_API_LoadFromFlash("flash.db");

	SQL_API_Semaphore_Wait();
	SQL_API_Exec("CREATE TABLE flash.sortTable \
(bouquetId INTEGER, bouquetName TEXT, PRIMARY KEY(bouquetId));");

	SQL_API_Exec("CREATE TABLE flash.program \
(service_id INTEGER(32) primary key, ��� INTEGER, ��Ŀ�� TEXT, ���� INTEGER(8), ���� INTEGER(8), \
ϲ�� INTEGER(8), ɾ�� INTEGER(8), ���� INTEGER(8), �ƶ� INTEGER(8), \
���� INTEGER(8), �������� INTEGER(8), Ƶ�� INTEGER, ���Ʒ�ʽ TEXT, \
������ INTEGER, �������� INTEGER, ��ƵPID INTEGER, \
��ƵPID INTEGER, PCRPID INTEGER, ecmPID INTEGER(16), audioEcmPID INTEGER(16), videoEcmPID INTEGER(16), \
sCAT_Sys_ID INTEGER(16), TSID INTEGER(32), \
����ID TEXT, PID INTEGER, OriNetId INTEGER(16), ��Ƶ���� INTEGER(16), ��Ƶ���� INTEGER(16), ������С INTEGER(8));");
	SQL_API_Exec("CREATE INDEX IF NOT EXISTS flash.iPro ON program(service_id, ���);");
		
	SQL_API_Exec("CREATE TABLE flash.eventReserved \
(��� INTEGER, ��Ŀ�� TEXT, ���� INTEGER(8), �¼��� INTEGER, \
��ʾ��Ϣ״̬ INTEGER(8), ��ϸ��Ϣ TEXT, �¼����� TEXT, \
ԤԼ״̬ INTEGER(8), ��ʼСʱ INTEGER(8), ��ʼ���� INTEGER(8), \
����Сʱ INTEGER(8), �������� INTEGER(8), ���� INTEGER(32), ���� INTEGER, PRIMARY KEY(���, �¼���,����,��ʼСʱ,��ʼ����));");

	SQL_API_Exec("CREATE TABLE flash.broadcast \
(service_id INTEGER(32) primary key, ��� INTEGER, ��Ŀ�� TEXT, \
���� INTEGER(8), Ƶ�� INTEGER, ���Ʒ�ʽ TEXT, \
������ INTEGER, �������� INTEGER, ��ƵPID INTEGER, \
��ƵPID INTEGER, PCRPID INTEGER, TSID INTEGER(32), PID INTEGER, OriNetId INTEGER(16));");
	SQL_API_Exec("CREATE INDEX IF NOT EXISTS flash.iBroad ON program(service_id, ���);");

	SQL_API_Semaphore_Release();

	dba_ipi_init_flag = 1;
}

/*
ִ��һ��SQL���
*/
BU08 SQL_API_Exec(BS08 * strSQL)
{
#ifndef _SQL_DEBUG
	rc = sqlite3_exec(db, strSQL, NULL, 0, 0);
#else
	rc = sqlite3_exec(db, strSQL, NULL, 0, &zErrMsg);
		
	if (zErrMsg != 0 && strlen(zErrMsg) != 0)
	{
		FYF_API_printf(strSQL);
		FYF_API_printf("\n");
		FYF_API_printf(zErrMsg);
		FYF_API_printf("===================SQL_ERROR!\n");
		
		return 0;
	}
#endif
	if(rc == 0)
		return 1;
	else
		return 0;
}

/*
ִ��һ��SQL���
*/
void SQL_API_Select(BS08 * sql)
{
#ifndef _SQL_DEBUG
	if(selectResult != 0)
	{
		sqlite3_free_table(selectResult);
		selectResult = 0;
	}
	rc = sqlite3_get_table(db, sql, &selectResult, &nResult, &mResult, 0);	
 	iResult = 1;
#else
	sqlite3_free_table(selectResult);
	rc = sqlite3_get_table(db, sql, &selectResult, &nResult, &mResult, &zErrMsg);	
	if (zErrMsg != 0 && strlen(zErrMsg) != 0)
	{
		FYF_API_printf(sql);
		FYF_API_printf("\n");
		FYF_API_printf(zErrMsg);
		FYF_API_printf("===================SQL_ERROR!\n");
	}
	
  	iResult = 1;
#endif
}

void SQL_API_Semaphore_Wait(void)
{
	BU32 rv = 0;

	rv = FYF_API_semaphore_wait(semaphoreSQL, FYF_WAIT_FOREVER);//4000);

}

void SQL_API_Semaphore_Release(void)
{


	FYF_API_semaphore_release(semaphoreSQL);
}
/*
����ǰ�������ƶ�һ��
*/
void SQL_API_QueryNext(void)
{
	iResult++;
}

/*
��ת��ָ����(0~n)
*/
void SQL_API_GotoRow(BU32 nRow)
{
	iResult = nRow + 1;
}

/*
��ǰ���Ƿ񳬳����һ��
*/
BU08 SQL_API_Eof()
{
	if (iResult <= nResult)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void SQL_Select(struct dbaData_s *pDbaData, BS08 * sql)
{
	if(pDbaData->selectResult != 0)
	{
		sqlite3_free_table(pDbaData->selectResult);
		pDbaData->selectResult = 0;
	}
	rc = sqlite3_get_table(db, sql, &(pDbaData->selectResult), &(pDbaData->nResult), &(pDbaData->mResult), 0);	
}

void SQL_GetStringData(struct dbaData_s *pDbaData, BU32 row, BU32 column, BU08 *str, BU32 strSize)
{
	if (column >= (BU32)(pDbaData->mResult))
	{
		strcpy(str, "");
		return;
	}

	if(pDbaData->selectResult[(row + 1) * (pDbaData->mResult) + column] == null)
	{
		strcpy(str, "");
	}
	else
	{
		if(strlen(pDbaData->selectResult[(row + 1) * (pDbaData->mResult) + column]) >= strSize && strSize != 0)
		{
			memcpy(str,pDbaData->selectResult[(row + 1) * pDbaData->mResult +column], strSize - 1);
			str[strSize - 1] = '\0';
		}
		else
		{
			strcpy(str, pDbaData->selectResult[(row + 1) * pDbaData->mResult + column]);
		}
	}
}

static const BU32 GUI_Pow10[10] = {1 , 10, 100, 1000, 10000,100000, 1000000, 10000000, 100000000, 1000000000};
BU32 DBA_API_AddStrDec(PU08 ps)
{
	BU32 c;
	BU32 d;
	BU32 i;
	BU32 len;
	BU32 va;
	BU32 nt = 0;

	i = 0;
	c =0;
	va =0;
	len = strlen(ps);
	while (len--)
	{
		if(i == 0 && ((BU32)ps[i]-0x30) >= 10)
		{
			i++;
			nt = 1;
			continue;
		}
		else
		{
			d = GUI_Pow10[len];
			c = ((BU32)ps[i]-0x30) * d;
			va += c;
			i++;				
		}
	}

	if(nt)
	{
		va = 0 - va;
	}
	return va;
}


BU32 SQL_GetNumberData(struct dbaData_s *pDbaData, BU32 row, BU32 column)
{
	BU08 number[11];
	SQL_GetStringData(pDbaData, row, column, number, 11);
	return DBA_API_AddStrDec(number);
}

/*
�õ���ǰ�е�ĳ��ֵ
*/
void SQL_IPI_GetQueryStringByIndex(BU32 FieldIndex,BU08 *data, BU32 dataSize)
{
	if (FieldIndex >= (BU32)mResult)
	{
		strcpy(data, "");
		return;
	}

	if(selectResult[iResult * mResult + FieldIndex] == null)
	{
		strcpy(data, "");
	}
	else
	{
		//GUI_API_Strcpy(data, selectResult[iResult * mResult +FieldIndex]);

		if(strlen(selectResult[iResult * mResult +FieldIndex]) >= dataSize && dataSize != 0)
		{
			memcpy(data,selectResult[iResult * mResult +FieldIndex], dataSize - 1);
			data[dataSize - 1] = '\0';
		}
		else
		{
			strcpy(data, selectResult[iResult * mResult +FieldIndex]);
		}
	}
}

/*
�õ���ǰ�е�ĳ��ֵ
*/
BU32 SQL_API_GetData(BU32 index,BU08 *data, BU32 dataSize, APP_DATA_TYPE_e dataType)
{
	if (dataType == APP_DATA_TYPE_STRING)
	{
		SQL_IPI_GetQueryStringByIndex(index, data, dataSize);
	}
	else
	{
		BU08 number[11];
		SQL_IPI_GetQueryStringByIndex(index, number, 11);
		return DBA_API_AddStrDec(number);
	}
}

/*
�õ���ѯ���������
*/
BU32 SQL_API_GetTotalNum(void)
{
	return nResult;
}










