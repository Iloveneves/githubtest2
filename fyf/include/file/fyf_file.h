
#ifndef _FYF_FILE_H_
#define _FYF_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif


#define FILE_PATH_NAME_MAX   (128 + 64)
#define FILE_FULLNAME_LEN	128
typedef char PATH[FILE_FULLNAME_LEN];

typedef struct _FYF_Dir_S
{
	BU32 flag; 			//0 表示是个文件节点，1表示是目录节点/
	BS08 name[FILE_FULLNAME_LEN]; 	//节点名字符串，不包含上级路径/
	void *handle; 		//保留给底层实现者关联管理信息使用/
}FYF_Dir_S;

typedef enum _FYF_FORMAT_TYPE_E
{
    FORMAT_FAT32 = 0,
    FORMAT_FAT16,
    FORMAT_EXT2,
    FORMAT_EXT3,
    FORMAT_BUTT,
}FYF_FORMAT_TYPE_E;
#if 0
typedef struct _FYF_Part_Info_S
{
          unsigned int part_type;
          unsigned long long b_size;
}FYF_Part_Info_S;
#endif
typedef struct _FYF_Part_Info_S
{
	BU08 bDetected;
	BU08 bErrorStatus;
	BU08 bNoFormat;
    BU08 bMounted;    	
	BU08 bProtected;    
	BU32 type;   
	BU64 size;   
	BU64  availsize;   
	PATH  USBPartPath;
	PATH  MountPath;
	BU08 PartIndex;
}FYF_Part_Info_S;

typedef struct _FYF_Time_Data_S
{
    BU08 tm_sec;      /* seconds after the minute   - [0,59]  */
    BU08 tm_min;      /* minutes after the hour     - [0,59]  */
    BU08 tm_hour;     /* hours after the midnight   - [0,23]  */
    BU08 tm_wday;     /* days since Sunday          - [0,6]   */
    BU08 tm_mday;     /* day of the month           - [1,31]  */
    BU08 tm_mon;      /* months since January       - [0,11]  */
    BU16 tm_year;     /* years since 1900                     */
    BU32 tm_yday;     /* days since Jan 1st         - [0,365] */
    BU32 tm_isdst;    /* Daylight Savings Time flag           */
}FYF_Time_Data_S;

typedef struct _FYF_File_Info_S
{
	FYF_Time_Data_S FileTime; 
	BU32 FileSize;	/* B */
}FYF_File_Info_S;

typedef enum _FYF_FILE_IOCTL_E
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

typedef enum _FYF_FILE_GETINFO_IOCTL_E
{
	FYF_FILE_GETINFO_CREATE_TIME = 0,
	FYF_FILE_GETINFO_MODIFY_TIME = 1,
	FYF_FILE_GETINFO_LENGTH = 2,
} FYF_FILE_GETINFO_IOCTL_E;

/********************************************************************
功能：打开操作系统中的文件。只是打开文件,如果文件不存在，不创建文件和目录。
********************************************************************/
BU32 FYF_API_File_Open(const BU08 *filename, const BU08 *mode);
/********************************************************************
功能：从文件的当前位置读取数据最大不超过len的数据。
********************************************************************/
BS32 FYF_API_File_Read(BU32 fd, BU08 *buf, BU32 maxlen);
/********************************************************************
功能：写len字节的数据到文件中。
********************************************************************/
BS32 FYF_API_File_Write(BU32 fd, const BU08 *buf, BU32 len);
/********************************************************************
功能：关闭打开的文件。
********************************************************************/
BS32 FYF_API_File_Close(BU32 fd);
/********************************************************************
功能：删除指定的文件或目录，如果是目录应该是以"/"或"\"结尾。
********************************************************************/
BS32 FYF_API_File_Delete(const BU08 *filename);

BS32 FYF_API_dir_create(const BS08 * dirname);

BS32 FYF_API_dir_remove(const BS08 *dirname);

BU32 FYF_API_dir_open(const BS08 *dirname);

BS32 FYF_API_dir_close(BU32 dd);

BS32 FYF_API_dir_rewind(BU32 dd);

BS32 FYF_API_dir_read(BU32 dd, FYF_Dir_S *pdir);

BU32 FYF_API_file_isexist(BS08 *filename);

BS32 FYF_API_file_GetFileInfo(BS08 *filename, FYF_File_Info_S *file);

BS32 FYF_API_USB_MountPart(FYF_Part_Info_S * partinfo);

BS32 FYF_API_USB_unMountPart(FYF_Part_Info_S * partinfo);

#ifdef __cplusplus
}
#endif

#endif

