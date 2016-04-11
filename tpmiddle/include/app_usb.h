#ifndef _H_APP_USB_H_
#define _H_APP_USB_H_

//usb设备挂载的根目录
#define ROOT_PATH "/mnt"

#define USB_PORT_MAX	2
#define MAX_PHY_PART_NUM  2
#define MAX_DISC_NUM  4
#define MAX_PART_NUM  20

#define FILE_GROUP_MAX	3
#define FILE_NUM_MAX	1024

#define FILE_PATH_LEN 128

#define USB_INFO
#ifdef USB_INFO
#define USB_DEBUG( fmt, args... )\
do { \
    printf( "\033[1;33m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define USB_DEBUG( fmt, args... )
#endif

#define USB_WRAMING
#ifdef USB_WRAMING
#define USB_ERROR( fmt, args... )\
do { \
    printf( "\033[0;31m\n%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
    printf( fmt, ##args);\
    printf("\033[0m\n");\
} while (0);
#else
#define USB_ERROR( fmt, args... )
#endif

typedef enum _APP_USB_PORT_E
{
    USB_PORT_0 = 0,
    USB_PORT_1,
    USB_PORT_BUTT
}APP_USB_PORT_E;

typedef enum _APP_USB_STATUS_E
{
    USB_STATUS_NO_DEVICE = 0,
    USB_STATUS_USB1_CHECK,
    USB_STATUS_USB2_CHECK,
    USB_STATUS_ALL_CHECK,
    USB_STATUS_BUTT
}APP_USB_STATUS_E;

typedef enum _APP_FILE_IOCTL_E
{
	APP_FILE_SEEK_SET = 1,
	APP_FILE_SEEK_CUR = 2,
	APP_FILE_SEEK_END = 3,
	APP_FILE_TRUNCATE = 4,
	APP_FILE_FLUSH = 5,
	APP_FILE_GET_POSITION = 6,
	APP_FILE_GET_CREATE_TIME =7,
	APP_FILE_GET_MODIFY_TIME = 8,
	APP_FILE_GET_LENGTH = 9,
} APP_FILE_IOCTL_E;

/*
typedef struct
{
	BU08 bDetected;
	BU08 bErrorStatus;
	BU08 bNoFormat;
    BU08 bMounted;    	
	BU08 bProtected;    
	BU32 type;   
	BU32 size;   
	BU32  availsize;   
	PATH  USBPartPath;
	PATH  MountPath;
}Part_Info;
*/
typedef struct _APP_Disc_Info_S
{
	BU08 bDetected;
	BU08 bErrorStatus;
	BU08 bProtected;
    BU08  partnumber;
    BU32  primary_part;    
    BU32  syspartnumber;
	BU64  size; 
	PATH  USBDiskPath;
	PATH  MountPath;
	FYF_Part_Info_S strPartinfo[16];
}APP_Disc_Info_S;



typedef enum _APP_FORMAT_TYPE_E
{
    /* music */
    FORMAT_TYPE_MP3 = 0,
    FORMAT_TYPE_TS,
    /* image */
    FORMAT_TYPE_JPG,    /* including JPEG */
    FORMAT_TYPE_PNG,
    FORMAT_TYPE_BMP,
    FORMAT_TYPE_GIF,
    FORMAT_TYPE_TXT,
    FORMAT_TYPE_BUTT
} APP_FORMAT_TYPE_E;

typedef enum _APP_FILE_TYPE_E
{  
	FILE_TYPE_IMAGE = 0,
	FILE_TYPE_MUSIC,  
	FILE_TYPE_FILM,              
	FILE_TYPE_TXT,    
	FILE_TYPE_DIR,     //目录节点 
	FILE_TYPE_UNKNOWN,
	FILE_TYPE_BUTT
} APP_FILE_TYPE_E;

typedef struct _APP_File_Info_S
{
	BU08 bUserFlag; //0表示没有在用，1表示端口1，2表示端口2
	BU08 FileName[FILE_FULLNAME_LEN]; 
	BU08 FilePath[FILE_FULLNAME_LEN]; 
	APP_FILE_TYPE_E FileType;	
	FYF_Time_Data_S FileTime; 
	BU32 FileSize;	/* B */
}APP_File_Info_S;


typedef struct _APP_File_Group_Info_S
{
	BU32 		FileNum;
	APP_File_Info_S   *FileInfo[FILE_NUM_MAX];	
}APP_File_Group_Info_S;


typedef struct _APP_Dir_Info_S
{
	BU32 		FileNum;
	BU32        FileSize; //字节
	APP_File_Info_S 	FileInfo[FILE_NUM_MAX];	
}APP_Dir_Info_S;


typedef struct _APP_Time_Data_S
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
}APP_Time_Data_S;

typedef struct _APP_File_EventInfo_S
{
	APP_Time_Data_S FileTime; 
	BU32 FileSize;	/* B */
}APP_File_EventInfo_S;

BS32 APP_USB_API_DetectedUSB();

/******************************
描述:检测当前U盘个数及存在的U口位置，
	0--2个U口都没插入U盘
	1--1个U盘插入1号U口
	2--1个U盘插入2号U口
	3--2个U口都插入U盘
*******************************/
BU32 APP_USB_API_DetectedWhichUSB();
/******************************
打开该目录后保存目录信息
*******************************/
BU32 APP_USB_API_SaveDirInfo(BU08* DirPath);
/******************************
打开当前目录并保存目录信息
*******************************/
BU32 APP_USB_API_SaveCurDirInfo(void);
/******************************
设置当前路径
*******************************/
BU32 APP_USB_API_SetCurPath(const BU08* FilePath);
/******************************
获取当前路径
*******************************/
BU32 APP_USB_API_GetCurPath(BU08* FilePath);
/******************************
获取目录名或文件名
*******************************/
BU32 APP_USB_API_GetFileName(const BU08* FilePath,BU08* FileName);
/******************************
获取目录或文件的绝对路径
*******************************/
BU32 APP_USB_API_GetFilePath(const BU08* FileName,BU08* FilePath);
/******************************
获取根目录
*******************************/
BU32 APP_USB_API_GetRootName(const BU08* FilePath,BU08* RootName);
/******************************
获取上级目录路径
*******************************/
BU32 APP_USB_API_GetParentDirPath(const BU08* FilePath,BU08* ParentDirPath);
/******************************
获取上级目录名
*******************************/
BU32 APP_USB_API_GetParentDirName(const BU08* FilePath,BU08* ParentDirName);
/******************************
获取目录下文件名和目录名
*******************************/
BU32 APP_USB_API_GetSubFileName(const BU08* DirPath,BU32 Index,BU08* SubFileName);
/******************************
获取目录下文件和目录路径
*******************************/
BU32 APP_USB_API_GetSubFilePath(const BU08* DirPath,BU32 Index,BU08* SubFilePath);
/******************************
获取目录下的文件总数
*******************************/
BU32 APP_USB_API_GetFileCount(const BU08* DirPath);
/******************************
获取当前文件或目录的根目录
*******************************/
BU32 APP_USB_API_GetCurRootName(BU08* RootName);
/******************************
获取当前目录的上级目录路径
*******************************/
BU32 APP_USB_API_GetCurParentDirPath(BU08* ParentDirPath);
/******************************
获取当前目录的上级目录名
*******************************/
BU32 APP_USB_API_GetCurParentDirName(BU08* ParentDirName);
/******************************
获取当前目录下的文件总数
*******************************/
BU32 APP_USB_API_GetCurFileCount(void);
/******************************
获取当前目录下文件和目录信息
*******************************/
BU32 APP_USB_API_GetCurSubFileInfo(BU32 Index, APP_File_Info_S* SubFileInfo);
/******************************
获取当前目录下文件名和目录名
*******************************/
BU32 APP_USB_API_GetCurSubFileName(BU32 Index,BU08 *SubFileName);
/******************************
获取当前目录下文件和目录路径
*******************************/
BU32 APP_USB_API_GetCurSubFilePath(BU32 Index,BU08 *SubFilePath);
/******************文件操作和属性********************/
/******************************
打开文件
*******************************/
BU32 APP_USB_API_OpenFile(const BU08* FilePath,const BU08 *Mode);
/******************************
读文件
*******************************/
BU32 APP_USB_API_ReadFile(BU32 fd, BU08 *buf, BU32 maxlen);
/******************************
写文件
*******************************/
BU32 APP_USB_API_WriteFile(BU32 fd, const BU08 *buf, BU32 len);
/******************************
关闭文件
*******************************/
BU32 APP_USB_API_CloseFile(BU32 fd);
/******************************
对文件属性进行控制或读取操作。
******************************/
BU32 APP_USB_API_FileIoctl(BU32 fd,APP_FILE_IOCTL_E op,BU32 *arg);
/******************************
读文本文件内容
*******************************/
BU32 APP_USB_API_ReadTextFileContent(const BU08 *FilePath, BU08 *Buf, BU32 maxlen);
/******************************
删除文件
*******************************/
BU32 APP_USB_API_DeleteFile(const BU08* FilePath);
/******************目录操作********************/
/******************************
删除目录
*******************************/
BU32 APP_USB_API_DeleteDir(const BU08* DirPath);
/******************************
通过文件名获取文件信息
*******************************/
BU32 APP_USB_API_GetFileInfoByFileName(BS08 *filename, APP_File_EventInfo_S *file);
#endif

