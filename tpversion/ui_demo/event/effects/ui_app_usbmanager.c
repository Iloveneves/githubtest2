#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//#define TXTDEMO
#ifdef TXTDEMO
#include "txtdemodata.h"
#include "aaaa.h"

typedef struct _txtNode
{
	struct _txtNode * next;
	struct _txtNode  *prev;
	int line;
	int num;
	int flag;
	int vline;
	char * txtdata;
}TxtNode;

TxtNode * txtroot; 
TxtNode * nodeindex[1000]; 
int linelen = 0;
int allline = 0;
void TXT_API_CreateIndex( char * buf)
{
	TxtNode * root;
	TxtNode * nodetemp;
	int buflen;
	int i = 0;
	int line = 0;
	buflen = strlen(buf);
	root = (TxtNode *)malloc(sizeof(TxtNode));
	txtroot = root;
	root->prev = NULL;
	root->next = NULL;
	root->num = 0;
	root->line = line;
	root->flag = 0;
	root->txtdata = buf;
	nodeindex[0] = txtroot;
	for(;;)
	{
		linelen++;
		if((buf[i] == 0x0d) && (buf[i+1] == 0x0a))
		{
			line++;
			root->num = linelen;
			nodetemp = (TxtNode *)malloc(sizeof(TxtNode));
			nodetemp->txtdata = &buf[i+1];
			buf[i] = 0;
			nodetemp->prev = root;
			nodetemp->line = line;
			nodetemp->num = 0;
			nodetemp->flag = 0;
			nodetemp->next = NULL;
			nodeindex[line] = nodetemp;
			root->next = nodetemp;
			root = nodetemp;
			linelen = 0;
			i++;
		}
		else if(buf[i] == 0)
		{
			break;
		}
		else
		{
		}
		i++;
	}
}
char * TXT_API_GetLineData(int line)
{
	return nodeindex[line]->txtdata;
}
int TXT_API_GetLinelen(int line)
{
	return nodeindex[line]->num;
}
int TXT_API_GetAllLine(void)
{
	return allline;
}

#define TXT_HEIGHT 300
#define TXT_WIDTH  150
#define TXT_TOP 	100
#define TXT_LEFT  	100
#define TXT_LINEDIS 30

int vlinenum;
int vlineno;
void txtdemo(BU32 hWnd)
{
	int i = 0;
	int j = 0 ;
	int len = 0;
	int datalen = 0;
	int heightlen = 0 ;
	int linenum = 0;
	int k = 0;
	char * data  = NULL;
	GUI_Rect_s disprect;
	int num = 0;
	GUI_API_BeginPaint(hWnd);
	
	TXT_API_CreateIndex(aaaa);
	heightlen = TXT_HEIGHT / TXT_LINEDIS;
	APP_Printf("heightlen = %d \n",heightlen );
	
	for (;;)
	{
		
		data = TXT_API_GetLineData(j);
		len = TXT_API_GetLinelen(j);
		
		datalen = GUI_API_GetTextLenght(data,len);
		
		num = datalen / TXT_WIDTH;
		APP_Printf("j = %d datalen = %d num = %d len = %d \n",j,datalen,num,len );
		APP_Printf("%s \n",data);
		//GUI_API_TextOut(hWnd,50  ,50,data,30,RGB(255,0,0),RGB(0,255,0),0);					
		if(k+num+1 < heightlen)
		{
			linenum = num+1;
			
			for(i=0;i<linenum;i++)
			{
				APP_Printf("linenum = %d i = %d\n",linenum,i );
				disprect.xpos = TXT_WIDTH*i;		
				disprect.ypos = 0;
				disprect.width =TXT_WIDTH;
				disprect.height = 30;
				GUI_API_TextSetValue(TEXT_SVL_RECT,&disprect,0); 				
				GUI_API_TextOut(hWnd,TXT_LEFT  ,TXT_TOP+ i*TXT_LINEDIS,data,GUI_API_Strlen(data),RGB(255,0,0),RGB(0,255,0),0);			
				GUI_API_TextOut(hWnd,TXT_LEFT  ,TXT_TOP+ i*TXT_LINEDIS,"1",1,RGB(255,0,0),RGB(0,255,0),0);			
			}	
			k += num+1;
			
		}	
		else
		{
			linenum = (k+num+1) -heightlen ;

			for(i=0;i<linenum;i++)
			{
				APP_Printf("-linenum = %d i = %d\n",linenum,i );
				disprect.xpos =TXT_WIDTH*i;	
				disprect.ypos = 0;
				disprect.width =TXT_WIDTH;
				disprect.height = 30;
				GUI_API_TextSetValue(TEXT_SVL_RECT,&disprect,0); 
				GUI_API_TextOut(hWnd,TXT_LEFT  ,TXT_TOP+ i*TXT_LINEDIS,data,GUI_API_Strlen(data),RGB(255,0,0),RGB(0,255,0),0);			
				GUI_API_TextOut(hWnd,TXT_LEFT  ,TXT_TOP+ i*TXT_LINEDIS,"2",1,RGB(255,0,0),RGB(0,255,0),0);			
			}		
			k += linenum;
			vlineno = j;
			vlinenum = linenum;
			break;
			
		}

		j++;
	}
	GUI_API_EndPaint(hWnd);
	
}
#endif

#define PAGE_USB_NUM  7
#define FILE_NAME_LEN 26
//#define FILE_PATH_LEN 255

//节目列表相关结构体定义
typedef struct FILELIST_S
{
	BU08 *filename;
	BU08 driveType;
	BU08 folderType;
	BU08 PictureType;
	BU08 mp3Type;
//	BU08 movieType;
//	BU08 etcType;
//	BU08 textType;
}FILELIST_S;

static BU32 usb_curpage = 0;
static BU32 usb_pagenum = 0;
static BU32 usb_rest= 0;
static BU08 usb_curfilepath[FILE_PATH_LEN];
static BU08 usb_filename[PAGE_USB_NUM][FILE_NAME_LEN];

BU08 g_Usb_SelectFileName[FILE_FULLNAME_LEN];//选中的文件的文件名

static FILELIST_S usb_fileinfo_s[PAGE_USB_NUM]=
{
	{usb_filename[0],0,0,0,0},
	{usb_filename[1],0,0,0,0},
	{usb_filename[2],0,0,0,0},
	{usb_filename[3],0,0,0,0},
	{usb_filename[4],0,0,0,0},
	{usb_filename[5],0,0,0,0},
	{usb_filename[6],0,0,0,0},
};

void UsbManagerList_IPI_ShowCurPage(void)
{

	BU32 index,i;
	BU32 PageNum;
	APP_File_Info_S fileinfo;
	if(usb_pagenum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(usb_curpage >= usb_pagenum)
	{
		usb_curpage = 0;
	}
	
	if(usb_curpage+1 == usb_pagenum)
	{	
		PageNum = usb_rest;
	}
	else
	{
		PageNum = PAGE_USB_NUM;
	}
	if(usb_curpage == 0)
	{
	//根目录显示
		sprintf(usb_fileinfo_s[0].filename,".");
		usb_fileinfo_s[0].filename[1] = 0;
		usb_fileinfo_s[0].driveType = 1;
		usb_fileinfo_s[0].folderType = 0;
		usb_fileinfo_s[0].mp3Type= 0;
		usb_fileinfo_s[0].PictureType = 0;
	//上级目录显示
		sprintf(usb_fileinfo_s[1].filename,"..");
		usb_fileinfo_s[1].filename[2] = 0;
		usb_fileinfo_s[1].driveType = 1;
		usb_fileinfo_s[1].folderType = 0;
		usb_fileinfo_s[1].mp3Type= 0;
		usb_fileinfo_s[1].PictureType = 0;
	//当前目录下内容
		for(i=2; i<PageNum; i++)
		{	
			index = i - 2 + usb_curpage*PAGE_USB_NUM;
			if(APP_FAIL == APP_USB_API_GetCurSubFileInfo(index,&fileinfo))
			{
				GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_DATA, (BU32)usb_fileinfo_s, i);
				return;
			}
			memcpy(usb_fileinfo_s[i].filename,fileinfo.FileName,FILE_NAME_LEN);
			usb_fileinfo_s[i].filename[FILE_NAME_LEN - 1] = '\0';
			
			switch(fileinfo.FileType)
			{
			case FILE_TYPE_DIR:
				 usb_fileinfo_s[i].driveType = 0;
				 usb_fileinfo_s[i].folderType = 1;
				 usb_fileinfo_s[i].mp3Type= 0;
				 usb_fileinfo_s[i].PictureType = 0;
				 break;
			case FILE_TYPE_MUSIC:
				 usb_fileinfo_s[i].driveType = 0;
				 usb_fileinfo_s[i].folderType = 0;
				 usb_fileinfo_s[i].mp3Type= 1;
				 usb_fileinfo_s[i].PictureType = 0;
				 break;
			case FILE_TYPE_IMAGE:
				 usb_fileinfo_s[i].driveType = 0; 
				 usb_fileinfo_s[i].folderType = 0;
				 usb_fileinfo_s[i].mp3Type= 0;
				 usb_fileinfo_s[i].PictureType = 1;
				 break;
			default:
				 usb_fileinfo_s[i].driveType = 0; 
				 usb_fileinfo_s[i].folderType = 0;
				 usb_fileinfo_s[i].mp3Type= 0;
				 usb_fileinfo_s[i].PictureType = 0;
				 break;
			}
		}
	}
	else
	{
		for(i=0; i<PageNum; i++)
		{	
			index = i -2 + usb_curpage*PAGE_USB_NUM;
			if(APP_FAIL == APP_USB_API_GetCurSubFileInfo(index,&fileinfo))
			{
				GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_DATA, (BU32)usb_fileinfo_s, i);
				return;
			}
			memcpy(usb_fileinfo_s[i].filename,fileinfo.FileName,FILE_NAME_LEN);
			usb_fileinfo_s[i].filename[FILE_NAME_LEN - 1] = '\0';
			switch(fileinfo.FileType)
			{
			case FILE_TYPE_DIR:
				 usb_fileinfo_s[i].driveType = 0; 
				 usb_fileinfo_s[i].folderType = 1;
				 usb_fileinfo_s[i].mp3Type= 0;
				 usb_fileinfo_s[i].PictureType = 0;
				 break;
			case FILE_TYPE_MUSIC:
				 usb_fileinfo_s[i].driveType = 0;
				 usb_fileinfo_s[i].folderType = 0;
				 usb_fileinfo_s[i].mp3Type= 1;
				 usb_fileinfo_s[i].PictureType = 0;
				break;
			case FILE_TYPE_IMAGE:
				 usb_fileinfo_s[i].driveType = 0; 
				 usb_fileinfo_s[i].folderType = 0;
				 usb_fileinfo_s[i].mp3Type= 0;
				 usb_fileinfo_s[i].PictureType = 1;
				break;
			default:
				 usb_fileinfo_s[i].driveType = 0; 
				 usb_fileinfo_s[i].folderType = 0;
				 usb_fileinfo_s[i].mp3Type= 0;
				 usb_fileinfo_s[i].PictureType = 0;
				break;
			}
		}
	}
	GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_DATA, (BU32)usb_fileinfo_s, PageNum);
	
}

void UsbManagerList_IPI_SetPageParam(void)
{
	BU32 count;
    BU32 curNo;
	count = APP_USB_API_GetCurFileCount();
	curNo = APP_USB_API_GetCurDirCurFileNo();
	if(count == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_UsbManager, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_UsbManager, WDT_SBR_SV_MIN, 0, 0); 
		GUI_ENC_API_SetValue(ID_SBR_FileList_UsbManager, WDT_SBR_SV_VALUE, 0, 0);
	}
	else
	{
		
		//包括一个根目录和上级目录
		count +=2;
		usb_pagenum = (count + PAGE_USB_NUM -1 ) / PAGE_USB_NUM;
		usb_rest = count % PAGE_USB_NUM;
		usb_curpage = curNo / PAGE_USB_NUM;
		if(usb_rest == 0)
		{
			usb_rest = PAGE_USB_NUM;
		}
		GUI_ENC_API_SetValueUnPaint(ID_LBP_FileList_UsbManager, WDT_LBP_SV_INDEX, curNo % PAGE_USB_NUM, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_UsbManager, WDT_SBR_SV_MAX, (BU32)count -1, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_FileList_UsbManager, WDT_SBR_SV_MIN, 0, 0); 
		GUI_ENC_API_SetValue(ID_SBR_FileList_UsbManager, WDT_SBR_SV_VALUE, 0, 0);

	}
}

void UsbManagerList_IPI_ShowFileInfo(void)
{
    BU32 Index;
	BU32 FileNo;
	APP_File_Info_S fileinfo;
	BU32 file_size;
	static BU08 file_size_str[11];
	static BU08 usb_date[60];
    GUI_ENC_API_GetValue(ID_LBP_FileList_UsbManager,WDT_LBP_SV_INDEX,&Index,0);
	FileNo = (Index + usb_curpage*PAGE_USB_NUM);
	GUI_ENC_API_SetValue(ID_SBR_FileList_UsbManager, WDT_SBR_SV_VALUE,FileNo, 0);
	if(FileNo < 2 )
	{
		GUI_ENC_API_SetValue(ID_STA_Line1_UsbManager,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line2_UsbManager,WDT_STA_SV_STRING,(BU32)"",0);
	}
	if(FileNo >= 2)
	{
		if(APP_FAIL == APP_USB_API_GetCurSubFileInfo(FileNo - 2,&fileinfo))
		{
			return;
		}
		//显示文件大小，当为目录是显示为空
		if(fileinfo.FileType == FILE_TYPE_DIR)
		{
			GUI_ENC_API_SetValue(ID_STA_Line1_UsbManager, WDT_STA_SV_STRING,(BU32)"", 0);
		}
		else
		{
			file_size = fileinfo.FileSize;
			if(file_size < 1024 && file_size >= 0)
			{
				sprintf(file_size_str,"%d B",file_size);
			}
			else if(file_size >= 1024 && file_size < 1048576)
			{
				file_size = file_size >> 10;
				sprintf(file_size_str,"%d KB",file_size);
			}
			else if(file_size >= 1048576 && file_size < 1073741824)
			{
				file_size = file_size >> 20;
				sprintf(file_size_str,"%d MB",file_size);
			}
			else
			{
				file_size = file_size >> 30;
				sprintf(file_size_str,"%d GB",file_size);
			}
			GUI_ENC_API_SetValue(ID_STA_Line1_UsbManager, WDT_STA_SV_STRING,(BU32)file_size_str, 0);
		}
		//显示文件修改时间
		sprintf(usb_date,"%04d-%02d-%02d  %02d:%02d",fileinfo.FileTime.tm_year,fileinfo.FileTime.tm_mon,fileinfo.FileTime.tm_mday,fileinfo.FileTime.tm_hour,fileinfo.FileTime.tm_min);	
		GUI_ENC_API_SetValue(ID_STA_Line2_UsbManager,WDT_STA_SV_STRING,(BU32)usb_date,0);
		APP_PRINT_DEBUG("FileNo = %d, file_size_str = %s, usb_date = %s", FileNo, file_size_str, usb_date);
	}
}

BU32 UsbManagerList_IPI_ShowCurPath(void)
{
    BU08 file_path[FILE_PATH_LEN];
	BU32 len;
	APP_USB_API_GetCurPath(file_path);
	len = strlen(file_path);
	memset(usb_curfilepath, 0, sizeof(usb_curfilepath));

	APP_Printf("UI_APP_UsbManager_Enter-----%s--%d\n",file_path,len);
	if(len == 4 || len == 5)
	{
		sprintf(usb_curfilepath," ");
	}
	else if(len > 5)
	{
		strncpy(usb_curfilepath,file_path+5,sizeof(file_path)-5);
		strncpy(usb_curfilepath + 1, ":", 1);
		strncpy(usb_curfilepath + 2,file_path + 6, sizeof(file_path)-6);
	}
	else
	{
		return APP_FAIL;
	}
	GUI_ENC_API_SetValue(ID_STA_Directory_UsbManager, WDT_STA_SV_STRING,(BU32)usb_curfilepath, 0);
    return APP_SUCCESS;
}
void UI_APP_UsbManager_Enter(void)
{

    BU32 count;
	BU32 len;
	BU32 hwnd;
	GUI_Rect_s rect;
	BU08 file_path[FILE_PATH_LEN];
    BU32 ret;
	if(GUI_ENC_API_GetPrevState() == ID_DLG_EffectsInfo)
	{
	    UsbManagerList_IPI_SetPageParam();
		UsbManagerList_IPI_ShowCurPage();
	}
	else
	{
	 	UsbManagerList_IPI_SetPageParam();
		UsbManagerList_IPI_ShowCurPage();
		hwnd = GUI_API_GetDlgItem(ID_VFM_UsbManager);
		GUI_API_GetWindowRect(hwnd,&rect);
		APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);//播放节目
	}
	UsbManagerList_IPI_ShowFileInfo();//显示文件信息
	ret = UsbManagerList_IPI_ShowCurPath();
	if(APP_FAIL == ret)
	{
	    APP_PRINT_ERROR("ret = %d", ret);
	    return;
	}
}

void UI_APP_UsbManager_End(void)
{
    
}

void UI_APP_UsbManager_LbpKey(BU32 key)
{
	BU32 Index;
	BU32 FileNo;
	BU32 count;
	BU32 language;
	APP_File_Info_S fileinfo;
	BU32 file_size;
	BU08 file_size_str[11];
	BU08 usb_date[60];
	BU08 FilePath[FILE_PATH_LEN];
	BU32 hwnd;
	BU32 len, ret;
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		GUI_ENC_API_GetValue(ID_LBP_FileList_UsbManager,WDT_LBP_SV_INDEX,&Index,0);
		FileNo = (Index + usb_curpage*PAGE_USB_NUM);
		APP_USB_API_SetCurDirCurFileNo(FileNo);
		UsbManagerList_IPI_ShowFileInfo();/*显示文件信息*/
		break;
	case GUI_VK_RIGHT:
		
		 break;
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_LBP_FileList_UsbManager,WDT_LBP_SV_INDEX,&Index,0);
		FileNo = (Index + usb_curpage*PAGE_USB_NUM);
		if(FileNo == 0)
		{
			//cwd add 2009-12-05
			APP_USB_API_GetCurPath(FilePath);
			if (strcmp(FilePath, ROOT_PATH) == 0)
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"当前目录已经是根目录",0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
				return;
			}
			APP_USB_API_GetCurRootName(FilePath);
			APP_USB_API_SetCurPath(FilePath);
			APP_USB_API_SaveCurDirInfo();
			UsbManagerList_IPI_SetPageParam();
			UsbManagerList_IPI_ShowCurPage();
		
		}
		else if(FileNo == 1)
		{
			if(APP_USB_API_GetCurParentDirPath(FilePath) == APP_FAIL)
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"当前目录已经是根目录",0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
				return;
			}
			APP_USB_API_SetCurPath(FilePath);
			APP_USB_API_SaveCurDirInfo();
			UsbManagerList_IPI_SetPageParam();
			UsbManagerList_IPI_ShowCurPage();
		}
		else
		{
			if(APP_FAIL == APP_USB_API_GetCurSubFileInfo(FileNo - 2,&fileinfo))
			{
				return;
			}
			#if 0
			if(fileinfo.FileType == FILE_TYPE_DIR)
			{
				APP_Printf("fileinfo.FileType == FILE_TYPE_DIR \n");
				if(APP_USB_API_GetFileCount(fileinfo.FilePath) == 0)
				{
				    //提示当前目录为空
					GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_EmptyDir,0);
					GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
				}
				else
				{
				    usb_curpage = 0;
					APP_USB_API_SetCurPath(fileinfo.FilePath);
					APP_USB_API_SaveCurDirInfo();
					UsbManagerList_IPI_SetPageParam();
				}
			}
			else if(fileinfo.FileType == FILE_TYPE_MUSIC)
			{
				/*****切换到播放器窗口******/
                memset(g_Usb_SelectFileName, 0, sizeof(g_Usb_SelectFileName));
				strncpy(g_Usb_SelectFileName, fileinfo.FilePath, strlen(fileinfo.FilePath));
				APP_Player_API_SetCurPlayerType(APP_PLAYER_TYPE_MP3_e);
                APP_Com_API_ShowOtherBmp(4);
				GUI_ENC_API_SwitchState(ID_DLG_Player, 1);
			}
			else if(fileinfo.FileType == FILE_TYPE_FILM)
			{
			    /*****切换到播放器窗口******/
                memset(g_Usb_SelectFileName, 0, sizeof(g_Usb_SelectFileName));
				strncpy(g_Usb_SelectFileName, fileinfo.FilePath, strlen(fileinfo.FilePath));
				APP_Player_API_SetCurPlayerType(APP_PLAYER_TYPE_PVR_e);
                //APP_Com_API_StopProg(0);
				APP_player_API_DetachAV();
				GUI_ENC_API_SwitchState(ID_DLG_Player, 1);
			}
			else if(fileinfo.FileType == FILE_TYPE_IMAGE)
			{
				APP_Printf("fileinfo.FileType == FILE_TYPE_IMAGE \n");
				//图片浏览提示框
			} 
			else if(fileinfo.FileType == FILE_TYPE_TXT)
			{
				APP_Printf("fileinfo.FileType == FILE_TYPE_TXT \n");
			    //hwnd = GUI_API_GetDlgItem(ID_EDT_Usb);
				//GUI_API_TextOut(hwnd,100  ,100,"hello world",10,RGB(255,0,0),RGB(0,255,0),0);
				//txtdemo(GUI_API_GetDesktopWindow());
				//read_txt
				//APP_USB_API_OpenFile(fileinfo.FilePath, "rw");
				//GUI_ENC_API_SetValue(ID_EDT_Usb,WDT_EDT_SV_PAGE_INDEX,0,0);
				//GUI_ENC_API_SetValue(ID_EDT_Usb, WDT_EDT_SV_VALUE, (BU32)txt_buf, leng);
				//GUI_ENC_API_SetFocus(ID_EDT_Usb);
			}
			#endif
			if(fileinfo.FileType == FILE_TYPE_DIR)
			{
				APP_Printf("fileinfo.FileType == FILE_TYPE_DIR \n");
				if(APP_USB_API_GetFileCount(fileinfo.FilePath) == 0)
				{
				    //提示当前目录为空
				   	UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
					GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_EmptyDir,0);
					GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
				}
				else
				{
				    APP_USB_API_SetCurDirCurFileNo(0);
					APP_USB_API_SetCurPath(fileinfo.FilePath);
					APP_USB_API_SaveCurDirInfo();
					UsbManagerList_IPI_SetPageParam();
					UsbManagerList_IPI_ShowCurPage();
				}
			}
			else if(fileinfo.FileType == FILE_TYPE_IMAGE)
			{
				APP_Printf("fileinfo.FileType == FILE_TYPE_IMAGE \n");
				//图片浏览提示框
			}
			else
			{
				APP_Printf("fileinfo.FileType == FILE_TYPE_UNKOWN \n");
				//无法识别该文件类型对话框
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NotSupportFileType,0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
			}
		}
		ret = UsbManagerList_IPI_ShowCurPath();
		if(APP_FAIL == ret)
		{
		    APP_PRINT_ERROR("ret = %d", ret);
		    return;
		}
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		break;
	case GUI_VK_EXIT:
		//APP_Com_API_StopProg(0);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_GREEN:
		if(APP_USB_API_GetCurParentDirPath(FilePath) == APP_FAIL)
		{
			return;
		}
		APP_USB_API_SetCurPath(FilePath);
		APP_USB_API_SaveCurDirInfo();
		UsbManagerList_IPI_SetPageParam();
		UsbManagerList_IPI_ShowCurPage();
		ret = UsbManagerList_IPI_ShowCurPath();
		if(APP_FAIL == ret)
		{
		    APP_PRINT_ERROR("ret = %d", ret);
		    return;
		}
		break;
	case GUI_VK_YELLOW:
		GUI_ENC_API_GetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_INDEX, &Index, 0);
		FileNo = (Index  + usb_curpage*PAGE_USB_NUM);
		if(FileNo == 0)
		{
			//提示不能删除根目录
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NotDelRoot,0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
		}
		else if(FileNo == 1)
		{
			//提示不能删除上级目录
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NotDelParentDir,0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
		}
		else if(FileNo >= 2)
		{
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_FILE_DELETE);
		    //提示提示是否确定删除
			UI_APP_API_SetDelFileNo(FileNo - 2 + 1 );
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_Del_Exit,0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo, 1);
		}
		break;
	default:
		break;
	}

}

void UI_APP_UsbManager_Change(void)
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_FileList_UsbManager, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			usb_curpage++;
			break;
		case -1:
			if(usb_curpage == 0)
				usb_curpage = usb_pagenum -1;
			else
				usb_curpage--;
			if(usb_curpage == usb_pagenum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_FileList_UsbManager, WDT_LBP_SV_INDEX, PAGE_USB_NUM-1, 0);
			}
			break;
		default:
			break;
	}
	UsbManagerList_IPI_ShowCurPage();//显示当前页
}




/*
检测到usb插入和拔出的处理，插入时弹出框
而拔出时，在不同的状态下进行不同的处理，尤其是在
使用usb文件时要先关闭掉，在进行卸载
*/


void UI_APP_API_USBInsertDispose(BU32 cur_state)
{
	UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_USB_INSERT);
	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_DetectUsbDevice,0);
	switch(cur_state)
	{
	case ID_DLG_Search:
	case ID_DLG_SearchInfo:
	case ID_DLG_Null:
	case ID_DLG_MenuLock:
		break;
	default:
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
		break;
	}
}

void UI_APP_API_USBEvulsionDispose(BU32 cur_state)
{
	UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_USB_EVULSION);
	GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_USBPullOut,0);
	switch(cur_state)
	{
#ifdef ENABLE_PVR
	case ID_DLG_Player:
		APP_Player_API_Stop();
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
		break;
#endif
#ifdef ENABLE_JPEG
	case ID_DLG_PhotoFrame:
	case ID_DLG_PictureShow:
		APP_PictureDecoder_API_Stop();
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
		break;
#endif
#ifdef ENABLE_PVR
	case ID_DLG_PvrList:
	case ID_DLG_PvrFullScreen:
		APP_Player_API_Stop();
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
		break;
	case ID_DLG_TimeShift:
		APP_PVR_API_StopRePlay(1);
		APP_PVR_API_SetDemux(0);
		APP_PVR_API_AttachAV();
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
		break;
#endif
#ifdef ENABLE_USB
	case ID_DLG_UsbManager:
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
		break;
#endif
	case ID_DLG_Search:
	case ID_DLG_SearchInfo:
	case ID_DLG_Null:
	case ID_DLG_MenuLock:
		break;
	default:
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
		break;
	}

}
void UI_APP_API_USBDetected_CallBack(BU08 bflag)
{
	BU32 cur_state = GUI_ENC_API_GetCurState();

	if (bflag == 1)
	{
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_DETECT_USB_INSERT, cur_state, 0);
	}
	else if (bflag == 2)
	{
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_DETECT_USB_EVULSION, cur_state, 0);
	}

}

