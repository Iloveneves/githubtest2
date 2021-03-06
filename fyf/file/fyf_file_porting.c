/*********************************************************************
Copyright (c) 2008 - 2010 Embedded Internet Solutions, Inc
All rights reserved. You are not allowed to copy or distribute
the code without permission.
This is the demo implenment of the base Porting APIs needed by iPanel MiddleWare.
Maybe you should modify it accorrding to Platform.

Note: the "int" in the file is 32bits

History:
version        date        name        desc

*********************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/hdreg.h>
#include <errno.h>
#include <time.h>

#include "fyf_def.h"
#include "file/fyf_file.h"

///#define HAVA_FILE_DEBUG
typedef struct FYF_DIR_DESC_TAG
{
	DIR *pDir;
	char PathName[256];
} FYF_DIR_DESC_T;


/********************************************************************************************************
功能：打开操作系统中的文件。只是打开文件,如果文件不存在，不创建文件和目录。
原型：BU32 FYF_API_File_Open(const BU08 *filename, const BU08 *mode)
参数说明：
输入参数：
filename：操作系统支持文件名称字符串；
mode：
b: 打开模式字符串，一律当binary 文件打开。
r: 打开已存在文件,offset 指向文件开头(read only)
w: 写模式打开文件, 不存在就创建, 存在清空， offset 指向文件头
(read&write)
a: 追加模式打开文件, 不存在就创建, 存在追加， offset 指向文件末
(read&write)
h: 表示本文件是超大文件，底层可以使用专用的管理方式。
输出参数：无
返 回：
  != FYF_NULL：文件句柄；
  == FYF_NULL:失败。
********************************************************************************************************/
BU32 FYF_API_File_Open(const BU08 *filename, const BU08 *mode)
{
	FILE *fp = NULL;
	BU08 *ptr;

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[FYF_API_File_Open] filename=%s, mode=%s.\n", filename, mode);
#endif
	if (!filename || !mode)
	{
		FYF_DEBUG("[FYF_API_File_Open] filename=%s, mode=%s, Parameters err.\n", filename, mode);
		return FYF_NULL;
	}

	if (strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0 ||strcmp(mode, "ab+") == 0
	        || strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0 || strcmp(mode, "wb+") == 0
	        || strcmp(mode, "r") == 0 ||strcmp(mode, "rb") == 0 || strcmp(mode, "rb+") == 0)
	{
		fp = fopen(filename, mode);
		if (!fp)
		{
			return FYF_NULL;
		}
	}
	else
	{
		FYF_DEBUG("[ipanel_porting_file_open] ERROR: mode=%s\n",mode);
	}

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[ipanel_porting_file_open] filename=%s, mode=%s, fd=0x%x\n", filename, mode, fp);
#endif
	return (BU32)fp;
}


/********************************************************************************************************
功能：从文件的当前位置读取数据最大不超过len的数据。
原型：BS32 FYF_API_File_Read(BU32 fd, BU08 *buf, BU32 maxlen)
参数说明：
  输入参数：
    fd：FYF_API_File_Open函数返回的打开文件的句柄；
    buf：缓存区地址；
    maxlen：缓存区长度。
  输出参数：buf：读取的数据。
返    回：
  >=0:实际读到数据的长度，0或小于len，表示到文件结尾了。
  FYF_ERR:读出错
********************************************************************************************************/
BS32 FYF_API_File_Read(BU32 fd, BU08 *buf, BU32 maxlen)
{
	BS32 len;
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[FYF_API_File_Read]fd=0x%x,buf=0x%x \n", fd, buf);
#endif
	len = fread(buf, 1, maxlen, (FILE *)fd);
	if (ferror((FILE *)fd) || len<0)
	{
		FYF_DEBUG("[FYF_API_File_Read] Failed to read file!!\n");
		len = FYF_ERR;
	}
	return len;
}

/********************************************************************************************************
功能：写len字节的数据到文件中。
原型：BS32 FYF_API_File_Write(BU32 fd, const BU08 *buf, BU32 len)
参数说明：
  输入参数：
    fd：FYF_API_File_Open函数返回的打开文件的句柄；
    buf：缓存区地址；
    len：缓存区长度。
  输出参数：无
返    回：
  >=0:实际写入数据的长度;
  FYF_ERR:写出错
********************************************************************************************************/
BS32 FYF_API_File_Write(BU32 fd, const BU08 *buf, BU32 len)
{
	BS32 write_size;
#ifdef HAVA_FILE_DEBUG
	IPANEL_DEBUG("[FYF_API_File_Write]fd=0x%x,buf=0x%x \n", fd, buf);
#endif
	if ((write_size = fwrite(buf, 1, len, (FILE*)fd)) < len)
	{
		write_size += fwrite(&buf[write_size], 1, len-write_size, (FILE*)fd);
		if (write_size < len)
			FYF_DEBUG("[FYF_API_File_Write] Failed to write file!!\n");
	}

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[FYF_API_File_Write] write_size = %d\n", write_size);
#endif
	return write_size;
}

/********************************************************************************************************
功能：关闭打开的文件。
原型：BS32 FYF_API_File_Close(BU32 fd)
参数说明：
  输入参数：
    fd：FYF_API_File_Open函数返回的打开文件的句柄；
  输出参数：无
返    回：
  IPANEL_OK:成功;
  FYF_OK:失败.
********************************************************************************************************/
BS32 FYF_API_File_Close(BU32 fd)
{
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[FYF_API_File_Close]\n");
#endif
	if (0 != fclose((FILE*)fd))
	{
		FYF_DEBUG("[FYF_API_File_Close] Failed to close file!!\n");
		return FYF_ERR;
	}
	return FYF_OK;
}

/********************************************************************************************************
功能：删除指定的文件或目录，如果是目录应该是以"/"或"\"结尾。
原型：BS32 FYF_API_File_Delete(const BU08 *filename)
参数说明：
  输入参数：
    name：路径＋文件名（目录名）；
  输出参数：无
返    回：
  FYF_OK:成功;
  FYF_ERR:失败.
********************************************************************************************************/
BS32 FYF_API_File_Delete(const BU08 *filename)
{

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[FYF_API_File_Delete]name=%s\n",name);
#endif

	if (0 != remove(filename))
	{
		FYF_DEBUG("[FYF_API_File_Delete] delete file Fail!\n");
		return FYF_ERR;
	}
	return FYF_OK;
}
/********************************************************************************************************
功能：对文件属性进行控制或读取操作。
原型：INT32_T ipanel_porting_file_ioctl(UINT32_T fd, IPANEL_FILE_IOCTL_e op,VOID *arg)
参数说明：
输入参数：
   fd：ipanel_porting_file_open 函数返回的打开文件的句柄；
   op: 操作命令；
	typedef enum
	{
		FYF_FILE_SEEK_SET = 1,
		FYF_FILE_SEEK_CUR = 2,
		FYF_FILE_SEEK_END = 3,
		FYF_FILE_TRUNCATE = 4,
		FYF_FILE_FLUSH = 5,
		FYF_FILE_GET_POSITION = 6,
		FYF_FILE_GET_CREATE_TIME =7,
		FYF_FILE_GET_MODIFY_TIME = 8,
		FYF_FILE_GET_LENGTH = 9,
	} FYF_FILE_IOCTL_E;
  arg：操作命令所带的参数，当传递枚举型或32 位整数值时，arg 可强制转换
成对应数据类型。

命令（op）                     命令参数（arg）                 说明
FYF_FILE_SEEK_SET       指向下面结构体的指针，
						typedef struct                 将文件读写指针移到距
						{                              文件头指定偏移量的位置
							BU32 uint64_32h;
							BU32 uint64_32l;
						}FYF_UINT64_T;

FYF_FILE_SEEK_CUR       FYF_UINT64_T *,             将文件读写指针移到距
                                                       当前指定偏移量的位置

FYF_FILE_SEEK_END       FYF_UINT64_T *,             将文件读写指针移到距
                                                       文件尾指定偏移量的位置

FYF_FILE_TRUNCATE       FYF_UINT64_T *,             将文件裁减到指定长
                                                       度，尾部内容丢弃。

FYF_FILE_FLUSH             void                     将缓冲区中内容写入存
                                                       储设备

FYF_FILE_GET_POSITION   FYF_UINT64_T *,             读取当前文件读写指针。

FYF_FILE_GET_CREATE_TIME        BU32 *,         获取文件创建时间，返回从1970 年1 月1 日0
                                                       时0 分0 秒开始及时的秒数

FYF_FILE_GET_MODIFY_TIME        BU32 *,         获取文件最后被修改的
                                                       时间，时间说明同上

FYF_FILE_GETINFO_LENGTH         BU32 *              获取文件大小(单位为字节)

输出参数：无。
返 回：
FYF_OK: 操作成功。
FYF_ERR:操作失败。
********************************************************************************************************/

BS32 FYF_API_File_Ioctl(BU32 fd, FYF_FILE_IOCTL_E op, BU32 *arg)
{
	FILE *fp = (FILE*)fd;
	struct stat StatBuf;
	BU32 pos;
	BU32 *time;
	FYF_File_Info_S *fileInfo;
	BU08 file_path[FILE_PATH_NAME_MAX];
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[%s] fd:0x%x, op=%d, arg=%x\n", __FUNCTION__, fd, op, (int)arg);
#endif
	switch (op)
	{
		case FYF_FILE_SEEK_SET:
			pos = (BU32) *arg;
			if (fseek(fp, (BU32)pos, SEEK_SET) != 0)
			{
				FYF_DEBUG("[FYF_FILE_SEEK_SET] Failed to seek!\n");
				return FYF_ERR;
			}
			else
			{
				break;
			}
		case FYF_FILE_SEEK_CUR:
			pos =(BU32) *arg;
			if (fseek(fp, (BU32)pos, SEEK_CUR) != 0)
			{
				FYF_DEBUG("[FYF_FILE_SEEK_SET] Failed to seek!\n");
				return FYF_ERR;
			}
			else
			{
				break;
			}
		case FYF_FILE_SEEK_END:
			pos = (BU32) *arg;
			if (fseek(fp, (BS32)pos, SEEK_END) != 0)
			{
				FYF_DEBUG("[FYF_FILE_SEEK_END] Failed to seek!\n");
				return FYF_ERR;
			}
			else
			{
				break;
			}
		case FYF_FILE_TRUNCATE:
			pos = (BU32) *arg;
			if (ftruncate((int)fp, pos) != 0)
			{
				FYF_DEBUG("[FYF_FILE_TRUNCATE] Failed to ftruncate!\n");
				return FYF_ERR;
			}
			else
			{
				break;
			}
		case FYF_FILE_FLUSH:
			if (fflush(fp) != 0)
			{
				FYF_DEBUG("[FYF_FILE_FLUSH] Failed to flush!\n");
				return FYF_ERR;
			}
			else
			{
				break;
			}
		case FYF_FILE_GET_POSITION:
			if (((*arg) = ftell(fp)) < 0)
			{
				(*arg) = -1;
				FYF_DEBUG("[FYF_FILE_GET_POSITION] Failed to get position!\n");
				return FYF_ERR;
			}
			else
			{
				break;
			}
		case FYF_FILE_GET_CREATE_TIME:
			time = (BU32 *)arg;
			if (fstat((int)fp, &StatBuf) != 0)
			{
				*time= 0;
				FYF_DEBUG("[FYF_FILE_GET_CREATE_TIME] Failed to get create time!\n");
				return FYF_ERR;
			}
			*time= StatBuf.st_ctime;
			break;
		case FYF_FILE_GET_MODIFY_TIME:
			time = (BU32 *)arg;
			if (fstat((int)fp, &StatBuf) != 0)
			{
				*time= 0;
				FYF_DEBUG("[FYF_FILE_GET_MODIFY_TIME] Failed to get modify time!\n");
				return FYF_ERR;
			}
			*time= StatBuf.st_mtime;
			break;
		case FYF_FILE_GET_LENGTH:
			fileInfo = (FYF_File_Info_S *) arg;
			if (fstat((int)fp, &StatBuf) != 0)
			{
				fileInfo->FileSize = 0;
				FYF_DEBUG("[FYF_FILE_GET_LENGTH] Failed to get file length !\n");
				return FYF_ERR;
			}
			fileInfo->FileSize = (BU32)StatBuf.st_size;
			FYF_DEBUG("[FYF_FILE_GET_LENGTH] length= %d!\n", StatBuf.st_size);
			break;
		default:
			return FYF_ERR;
	}
	return FYF_OK;
}

/********************************************************************************************************
功能：对文件属性进行控制或读取操作。
原型：INT32_T ipanel_porting_file_ioctl(UINT32_T fd, IPANEL_FILE_IOCTL_e op,VOID *arg)
参数说明：
输入参数：
   fd：ipanel_porting_file_open 函数返回的打开文件的句柄；
   op: 操作命令；
	typedef enum
	{
		FYF_FILE_GETINFO_CREATE_TIME = 0,
		FYF_FILE_GETINFO_MODIFY_TIME = 1,
		FYF_FILE_GETINFO_LENGTH = 2,
	} FYF_FILE_GETINFO_IOCTL_E;
  arg：操作命令所带的参数，当传递枚举型或32 位整数值时，arg 可强制转换
成对应数据类型。

命令（op）                     命令参数（arg）                 说明

FYF_FILE_GETINFO_CREATE_TIME    FYF_Time_Data_S *,         获取文件创建时间，返回从1970 年1 月1 日0
                                                       时0 分0 秒开始及时的秒数

FYF_FILE_GETINFO_MODIFY_TIME    FYF_Time_Data_S *,         获取文件最后被修改的
                                                       时间，时间说明同上

FYF_FILE_GETINFO_LENGTH         BU32 *              获取文件大小(单位为字节)
输出参数：无。
返 回：
FYF_OK: 操作成功。
FYF_ERR:操作失败。
********************************************************************************************************/

BS32 FYF_API_File_GetInfo_Ioctl(const BU08* FilePath, FYF_FILE_GETINFO_IOCTL_E op, void *arg)
{
	struct stat stat_buf;
	BU32 pos;
	struct tm *tm;
	FYF_Time_Data_S *time;
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[%s] fd:0x%x, op=%d, arg=%x\n", __FUNCTION__, fd, op, (int)arg);
#endif
	switch (op)
	{
		case FYF_FILE_GETINFO_CREATE_TIME:
			time = (FYF_Time_Data_S *)arg;
			if (stat(FilePath, &stat_buf) != 0)
			{
				memset(time,0,sizeof(FYF_Time_Data_S));
				FYF_DEBUG("[FYF_FILE_GETINFO_CREATE_TIME] Failed to get create time!\n");
				return FYF_ERR;
			}
			tm = localtime(&stat_buf.st_ctime);
			time->tm_year = 1900 + tm->tm_year;
			time->tm_mon = 1 + tm->tm_mon;
			time->tm_mday = tm->tm_mday;
			time->tm_hour = tm->tm_hour;
			time->tm_min = tm->tm_min;
			time->tm_sec = tm->tm_sec;
			time->tm_wday = tm->tm_wday;
			break;
		case FYF_FILE_GETINFO_MODIFY_TIME:
			time = (FYF_Time_Data_S *)arg;
			if (stat(FilePath, &stat_buf) != 0)
			{
				memset(time,0,sizeof(FYF_Time_Data_S));
				FYF_DEBUG("[FYF_FILE_GETINFO_MODIFY_TIME] Failed to get create time!\n");
				return FYF_ERR;
			}
			tm = localtime(&stat_buf.st_mtime);
			time->tm_year = 1900 + tm->tm_year;
			time->tm_mon = 1 + tm->tm_mon;
			time->tm_mday = tm->tm_mday;
			time->tm_hour = tm->tm_hour;
			time->tm_min = tm->tm_min;
			time->tm_sec = tm->tm_sec;
			time->tm_wday = tm->tm_wday;
			break;
		case FYF_FILE_GETINFO_LENGTH:
			if (stat(FilePath, &stat_buf) != 0)
			{
				*((BU32*)arg) = 0;
				FYF_DEBUG("[FYF_FILE_GETINFO_LENGTH] Failed to get file length !\n");
				return FYF_ERR;
			}
			*((BU32*)arg) = (BU32)stat_buf.st_size;
			FYF_DEBUG("[FYF_FILE_GETINFO_LENGTH] length= %d!\n", stat_buf.st_size);
			break;
		default:
			return FYF_ERR;
	}
	return FYF_OK;
}

/********************************************************************************************************
功能：在文件系统中创建指定的目录。
原型：BS32 FYF_API_dir_create(const BS08 * dirname)(
参数说明：
  输入参数：
    [logic:]/dir/dir1
    dirname：文件系统目录名称字符串；必须是绝对路径；
输出参数：无
返    回：
  FYF_OK:目录创建成功或目录已存在；
  FYF_ERR:失败.
********************************************************************************************************/
BS32 FYF_API_dir_create(const BS08 * dirname)
{
	char dirnm[FILE_PATH_NAME_MAX];
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[%s] name=%s\n", __FUNCTION__, dirname);
#endif

	strcpy(dirnm, dirname);

	if (access(dirnm, F_OK) == 0)
	{
		FYF_DEBUG("[FYF_API_dir_create] dir exists!!\n");
		return FYF_OK;
	}

	if (-1 == mkdir(dirnm, S_IRWXU))
	{
		FYF_DEBUG("[FYF_API_dir_create] Failed to create dir!!\n");
		return FYF_ERR;
	}
	return FYF_OK;
}

/********************************************************************************************************
功能：删除指定的目录，目录必须为空才能删除。
原型：BS32 FYF_API_dir_remove(const BS08 *dirname)

参数说明：
  输入参数：
    dirname：文件系统目录名称字符串；必须是绝对路径；
输出参数：无
返    回：
  FYF_OK:成功或目录不存在；
  FYF_ERR:失败.
********************************************************************************************************/
BS32 FYF_API_dir_remove(const BS08 *dirname)
{
	char dirnm[FILE_PATH_NAME_MAX];
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[%s] name=%s\n", __FUNCTION__, dirname);
#endif

	if (-1 == rmdir(dirname))
	{
		FYF_DEBUG("[FYF_API_dir_remove] Failed to remove dir!!\n");
		return FYF_ERR;
	}
	return FYF_OK;
}

/********************************************************************************************************
功能：打开文件系统中的目录。
原型：BU32 FYF_API_dir_open(const BS08 *dirname)
参数说明：
  输入参数：
    dirname：文件系统目录名称字符串；必须是绝对路径；
输出参数：无
返    回：
  !＝FYF_NULL：目录操作句柄；
  FYF_NULL:失败.
********************************************************************************************************/
BU32 FYF_API_dir_open(const BS08 *dirname)
{
	FYF_DIR_DESC_T  *pDirDesc;
	BU32 len = strlen(dirname);
#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[ipanel_porting_dir_open]name=%s\n",dirname);
#endif

	if ((pDirDesc=malloc(sizeof(FYF_DIR_DESC_T))) == NULL)
	{
		FYF_DEBUG( "Error alloc mem for desc.\n" );
		return (FYF_NULL);
	}

//	strncpy(pDirDesc->PathName, dirnm, 255);
//	pDirDesc->PathName[255] = '\0';
	strcpy(pDirDesc->PathName, dirname);

	if ((pDirDesc->pDir=opendir(pDirDesc->PathName)) == NULL)
	{
		FYF_DEBUG( "Failed to open dir.\n" );
		return FYF_NULL;
	}

	return (BU32)pDirDesc;
}

/********************************************************************************************************
功能：关闭打开的目录。
原型：BS32 FYF_API_dir_close(BU32 dd)
参数说明：
  输入参数：
    dd：目录操作句柄，由ipanel_porting_opendir 获得。
输出参数：无
返    回：
  FYF_OK:成功；
  FYF_ERR:失败.
********************************************************************************************************/
BS32 FYF_API_dir_close(BU32 dd)
{
	FYF_DIR_DESC_T *pDirDesc = (FYF_DIR_DESC_T *)dd;

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[ipanel_porting_dir_close] pdir=%d\n",dd);
#endif

	if (pDirDesc == NULL)
	{
		return FYF_ERR;
	}

	closedir(pDirDesc->pDir);
	free(pDirDesc);

	return FYF_OK;
}

/********************************************************************************************************
功能：复位目录读指针，使他指向目录里第一个记录。
原型：BS32 FYF_API_dir_rewind(BU32 dd)
参数说明：
  输入参数：
    dd：目录操作句柄，由FYF_API_dir_open 获得。
输出参数：无
返    回：
  FYF_OK:成功；
  FYF_ERR:失败.
********************************************************************************************************/
BS32 FYF_API_dir_rewind(BU32 dd)
{
	FYF_DIR_DESC_T *pDirDesc = (FYF_DIR_DESC_T *)dd;

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[ipanel_porting_dir_rewind] pdir=%d\n",dd);
#endif

	if (pDirDesc == NULL)
	{
		return FYF_ERR;
	}

	rewinddir(pDirDesc->pDir);
	return FYF_OK;
}

/********************************************************************************************************
功能：读取dd 目录中下一个节点的内容。
原型：BS32 FYF_API_dir_read(BU32 dd, IPANEL_DIR *pdir)
参数说明：
  输入参数：
    dd：目录操作句柄，由FYF_API_dir_open 获得。
    pdir: 保存输出节点信息结构的指针
输出参数：
    *pdir:输出节点信息
返    回：
FYF_OK: 成功读取目录中节点信息;
FYF_ERR:读取失败或没有更多节点信息。
********************************************************************************************************/
BS32 FYF_API_dir_read(BU32 dd, FYF_Dir_S *pdir)
{
	FYF_DIR_DESC_T *pDirDesc = (FYF_DIR_DESC_T *)dd;
	struct dirent *pDirEnt;
	struct stat    StatBuf;
	char   aFullFileName[256];

#ifdef HAVA_FILE_DEBUG
	FYF_DEBUG("[ipanel_porting_dir_read] pdir=%x\n",pDirDesc->pDir);
#endif

	if (pDirDesc == NULL)
	{
		return FYF_ERR;
	}

	while ((pDirEnt=readdir(pDirDesc->pDir)) != NULL)
	{
		// filter out the "." && "..":
		if (strcmp( pDirEnt->d_name, "." ) == 0  || strcmp( pDirEnt->d_name, ".." ) == 0 )
		{
			continue;
		}

		strcpy(pdir->name, pDirEnt->d_name);

		if (pDirDesc->PathName[strlen(pDirDesc->PathName)-1] == '/')
		{
			sprintf(aFullFileName, "%s%s", pDirDesc->PathName, pDirEnt->d_name);
		}
		else
		{
			sprintf(aFullFileName, "%s/%s", pDirDesc->PathName, pDirEnt->d_name);
		}

		if (stat(aFullFileName, &StatBuf) != 0 )
		{
			FYF_DEBUG("[ipanel_porting_dir_read]:Error stat.filename=%s\n", aFullFileName);
			return (FYF_ERR);
		}
		else
		{
			if (S_ISDIR(StatBuf.st_mode))
				pdir->flag = 1;   // folder
			else
				pdir->flag = 0;  // file
		}

#ifdef HAVA_FILE_DEBUG
		FYF_DEBUG("[ipanel_porting_dir_read]:File name %s, flag:%d.\n", pdir->name, pdir->flag);
#endif
		return FYF_OK;
	}
	return FYF_ERR;
}


/********************************************************************************************************
功能：判定一个文件是不是目录
原型：BS32 FYF_API_dir_judge(const BS08 * dirname)
参数说明：
  输入参数：
    dirname：文件系统目录名称字符串；必须是绝对路径；
输出参数：

返    回：
FYF_OK: 该文件是文件目录;
FYF_ERR:该文件不是文件目录;
********************************************************************************************************/
BS32 FYF_API_dir_judge(const BS08 * dirname)
{
	struct stat fstat;
	if (stat(dirname, &fstat))
	{
		perror("stat");
		return FYF_ERR;
	}
	if (!S_ISDIR(fstat.st_mode))
	{
		//	FYF_DEBUG(" path %s is not dir \n", dirname);
		return FYF_ERR;
	}
	return FYF_OK;
}

/*2009-08-17 cwd add*/

#define HDIO_GETRO  0x0330

BU32 FYF_API_file_isexist(BS08 *filename)
{
	struct stat buf;
	//printf("stat(filename, &buf) = %d\n", stat(filename, &buf));
	if (stat(filename, &buf) != 0)    //得到文件信息
	{
		if (errno == ENOENT)
		{
			return FYF_FALSE;
		}
	}
	return FYF_TRUE;
}

BS32 FYF_API_file_GetFileInfo(BS08 *filename, FYF_File_Info_S *file)
{
	struct stat fstat;
	struct tm *tm;
	if (stat(filename, &fstat))
	{
		perror("stat");
		return FYF_ERR;
	}

	tm = localtime(&fstat.st_mtime);
	//获取文件修改时间
	file->FileTime.tm_year = 1900 + tm->tm_year;
	file->FileTime.tm_mon = 1 + tm->tm_mon;
	file->FileTime.tm_mday = tm->tm_mday;
	file->FileTime.tm_hour = tm->tm_hour;
	file->FileTime.tm_min = tm->tm_min;
	file->FileTime.tm_sec = tm->tm_sec;
	file->FileTime.tm_wday = tm->tm_wday;
	printf("modify date: %d %d %d %d %d %d %d\n",	file->FileTime.tm_year,file->FileTime.tm_mon,
	       file->FileTime.tm_mday,file->FileTime.tm_hour,
	       file->FileTime.tm_min,file->FileTime.tm_sec,
	       file->FileTime.tm_wday);
	//获取文件大小，单位B
	file->FileSize = fstat.st_size;
	printf("st_size = %d\n",file->FileSize);
	return FYF_OK;
}

BS32 FYF_API_GetPartInfo(BS08 *partname, FYF_Part_Info_S *partinfo)
{
	struct statfs buf;

	if (statfs(partname, &buf))
	{
		printf("statfs error");
		return FYF_ERR;
	}
	partinfo->type = buf.f_type;
	partinfo->size = buf.f_bsize ;
	partinfo->size *= buf.f_blocks;
	partinfo->availsize = buf.f_bfree;
	partinfo->availsize	*= buf.f_bsize;

//	printf("type = %d %d %d %d %d %d %d\n", buf.f_type, buf.f_bsize, buf.f_blocks, buf.f_bfree, buf.f_bavail, buf.f_files, buf.f_ffree);

	return FYF_OK;
}


BS32 FYF_API_USB_MountPart(FYF_Part_Info_S * partinfo)
{
	char* part_direct = "part";
	int ret=0;
	int fd;

	if (partinfo == NULL)
		return FYF_ERR;

	if (strstr(partinfo->USBPartPath, part_direct) == NULL && strstr(partinfo->USBPartPath, "disc") == NULL)
	{
		FYF_ERROR("mount path is error.\n");
		return FYF_ERR;
	}

	if (partinfo->type >= FORMAT_BUTT)
	{
		FYF_ERROR("mount type is error.\n");
		return FYF_ERR;
	}

	switch (partinfo->type)
	{
		case FORMAT_FAT32:
			ret = mount(partinfo->USBPartPath, partinfo->MountPath, "vfat", MS_NOSUID, 0);
			break;

		case FORMAT_FAT16:
			ret = mount(partinfo->USBPartPath, partinfo->MountPath, "msdos", MS_NOSUID, 0);
			break;

		case FORMAT_EXT2:
			ret = mount(partinfo->USBPartPath, partinfo->MountPath, "ext2", MS_NOSUID, 0);
			break;

		case FORMAT_EXT3:
			ret = mount(partinfo->USBPartPath, partinfo->MountPath, "ext3", MS_NOSUID, 0);
			break;

		default:
			FYF_ERROR("mount type is error.\n");
			return FYF_ERR;
	}

	if (ret != 0)
	{
		fd = open(partinfo->USBPartPath, O_RDONLY);
		if (fd < 0)
		{
			FYF_ERROR("open disc error!\n");
			return FYF_ERR;
		}
		ioctl(fd, HDIO_GETRO, &ret);

		if (ret)
		{
			FYF_ERROR("%s is write-protected, mounting read-only.\n", partinfo->USBPartPath);
			partinfo->bMounted = FYF_TRUE;
			partinfo->bProtected = FYF_TRUE;
			return FYF_OK;
		}
		else
		{
			FYF_ERROR("mount %s to %s   failure.\n", partinfo->USBPartPath, partinfo->MountPath);
			return FYF_ERR;
		}
	}

	partinfo->bMounted = FYF_TRUE;

	return FYF_OK;
}

BS32 FYF_API_USB_unMountPart(FYF_Part_Info_S * partinfo)
{
	int ret=0;
	int fd,i;

	if (partinfo == 0)
		return FYF_ERR;

	ret = umount(partinfo->MountPath);
	printf("ret = %d\n", ret);
	if (ret != 0)
	{
		fd = open(partinfo->USBPartPath, O_RDONLY);
		if (fd < 0)
		{
			FYF_ERROR("open disc error!\n");
			return FYF_ERR;
		}
		ioctl(fd, HDIO_GETRO, &ret);
		if (ret)
		{
			return FYF_OK;
		}
		else
		{
			FYF_ERROR("umount %s failure.\n",partinfo->USBPartPath);
			return FYF_ERR;
		}
	}

	FYF_DEBUG("umount %s success \n",partinfo->MountPath);
	return FYF_OK;
}







