
#include "app_common.h"
#include "app_ipi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

//进行搜索的文件分类
//#define MAX_FILE_TYPE 3
extern  BU32 semaphoreUSB;

static BU08 CurFilePath[FILE_PATH_LEN] = {0};
static BU32 g_FileInfoNodeRemain = 0;
APP_Dir_Info_S CurDirContent;
APP_Disc_Info_S g_disc_info[USB_PORT_MAX];
APP_File_Group_Info_S g_file_group[FILE_GROUP_MAX];
APP_File_Info_S g_file_info[FILE_NUM_MAX * FILE_GROUP_MAX];
APP_File_Info_S *g_pfile_info[FILE_NUM_MAX * FILE_GROUP_MAX];

BU08 g_bmount[MAX_PART_NUM];

BU32 g_usb_CurDirCurFileNo = 0;

void MID_API_USB_File_Init()
{
	BU32 i;
	memset(g_file_group, 0, sizeof(g_file_group));
	memset(g_file_info, 0, sizeof(g_file_info));
	for (i = 0; i < FILE_NUM_MAX * FILE_GROUP_MAX; i++)
	{
		g_pfile_info[i] = &(g_file_info[i]);
	}
	g_FileInfoNodeRemain = FILE_NUM_MAX * FILE_GROUP_MAX;
	memset(g_bmount, 0, sizeof(g_bmount));
}	


APP_File_Info_S * MID_API_USB_File_Malloc()
{
	if (g_FileInfoNodeRemain > 0)
	{
		return (g_pfile_info[--g_FileInfoNodeRemain]);
	}
	else
	{
		USB_ERROR("file malloc fail, no enough memory!!!\n");
		return NULL;
	}
}	


void  MID_API_USB_File_Free(APP_File_Info_S *pfile)
{
	if (pfile == NULL)
    {
    	USB_ERROR("file free fail\n");
        return;
    }
	memset(pfile, 0, sizeof(APP_File_Info_S));
	g_pfile_info[g_FileInfoNodeRemain++] = pfile;
	return;
}	
	
BS32 MID_API_USB_File_Add(APP_File_Info_S *pfile)
{
	BU32 index;
	if (pfile == NULL)
    {
    	USB_ERROR("\n");
        return APP_ERR;
    }

	if (pfile->FileType >= FILE_TYPE_UNKNOWN)
	{	
		USB_ERROR("file unknow\n");
		return APP_ERR;
	}	
	FYF_API_semaphore_wait(semaphoreUSB, FYF_WAIT_FOREVER);
	for (index = 0; index < FILE_NUM_MAX; index++)
	{	
		if (g_file_group[pfile->FileType].FileInfo[index] == NULL)
		{	
			g_file_group[pfile->FileType].FileInfo[index] = pfile;	
			g_file_group[pfile->FileType].FileNum ++;
//			APP_Printf("file_add %s\n",pfile->FileName);
			FYF_API_semaphore_release(semaphoreUSB);
			return APP_OK;
		}
	}
	FYF_API_semaphore_release(semaphoreUSB);
	return APP_OK;
}



BS32 MID_API_USB_File_Delete(APP_File_Info_S *pfile)
{
	BU32 index;
	if (pfile == NULL)
    {
        return APP_ERR;
    }

	if(pfile->FileType >= FILE_TYPE_UNKNOWN)
	{	
		USB_ERROR("file unknow\n");
		return APP_ERR;
	}	

	for (index = 0; index < FILE_NUM_MAX; index++)
	{	
		if(g_file_group[pfile->FileType].FileInfo[index] != NULL)
		{	
			MID_API_USB_File_Free(pfile);
			g_file_group[pfile->FileType].FileInfo[index]  = 0;			
			g_file_group[pfile->FileType].FileNum -- ;
			break;
		}
	}
	return APP_OK;
}	

BS32 MID_API_USB_File_DelTypePath(APP_FILE_TYPE_E filetype, BS08 * path)
{
	BU32 index;
	APP_File_Group_Info_S *pfilegroup = 0;
	APP_File_Info_S *pfile = 0;
	BU32 pathlen = 0;



	if (filetype > FILE_TYPE_UNKNOWN)
	{
		return APP_ERR;
	}
	USB_DEBUG("File_deltypepath %s type %d \n",path,filetype);
	if (NULL == path)
    {
        return APP_ERR;
    }

	pathlen = strlen(path);

	if(pathlen > FILE_FULLNAME_LEN || pathlen == 0)
	{
        return APP_ERR;
    }
	FYF_API_semaphore_wait(semaphoreUSB, FYF_WAIT_FOREVER);
	pfilegroup = &(g_file_group[filetype]);

	for (index = 0; index < FILE_NUM_MAX; index++)
	{
		if (pfilegroup->FileInfo[index] != NULL)
		{
			pfile = (APP_File_Info_S *)pfilegroup->FileInfo[index];
			if (strncmp(path, pfile->FileName, pathlen) == 0)
			{	
				USB_DEBUG("file_del %s\n",pfile->FileName);
				MID_API_USB_File_Free(pfile);
				pfilegroup->FileInfo[index] = 0;
				pfilegroup->FileNum--;
			}
		}
	}	
	FYF_API_semaphore_release(semaphoreUSB);
	return APP_OK;
}



BS32 MID_API_USB_File_DelType(APP_FILE_TYPE_E filetype)
{
	BU32 index;
	APP_File_Group_Info_S * pfilegroup;
	APP_File_Info_S* pfile;
	if (filetype > FILE_TYPE_UNKNOWN)
	{
		return APP_ERR;
	}
	FYF_API_semaphore_wait(semaphoreUSB, FYF_WAIT_FOREVER);
	pfilegroup = &(g_file_group[filetype]);
	for (index = 0; index < FILE_NUM_MAX; index++)
	{
		if(pfilegroup->FileInfo[index] != 0)
		{
			pfile = (APP_File_Info_S*)pfilegroup->FileInfo[index];
			MID_API_USB_File_Free(pfile);
			pfilegroup->FileInfo[index] = 0;
		}
	}	
	pfilegroup->FileNum = 0;
	FYF_API_semaphore_release(semaphoreUSB);
	return APP_OK;
}


APP_FORMAT_TYPE_E MID_API_USB_File_Name2Format(char * filename)
{
    char *p = NULL;
    p = strrchr(filename, '.');
    if (p == NULL)
    {
        return FORMAT_TYPE_BUTT;
    }
#ifdef ENABLE_PVR
	if( !strncasecmp(p, ".mp3", 4) )
    {
        return FORMAT_TYPE_MP3;
    }
#endif

#ifdef ENABLE_JPEG

    if( !strncasecmp(p, ".jpg", 4) || !strncasecmp(p, ".jpeg", 5) )
    {
        return FORMAT_TYPE_JPG;
    }
    else if( !strncasecmp(p, ".png", 4) )
    {
        return FORMAT_TYPE_PNG;
    }
    else if( !strncasecmp(p, ".bmp", 4) )
    {
        return FORMAT_TYPE_BMP;
    }
    else if( !strncasecmp(p, ".gif", 4) )
    {
        return FORMAT_TYPE_GIF;
    }
#endif
#ifdef ENABLE_TXT
    if( !strncasecmp(p, ".txt", 4))
    {
        return FORMAT_TYPE_TXT;
    }
#endif
#ifdef ENABLE_PVR
	if (!strncasecmp(p, ".pvr", 4))
	{
		return FORMAT_TYPE_TS;
	}
#endif
    return FORMAT_TYPE_BUTT;
}

APP_FILE_TYPE_E MID_API_USB_File_Name2Type(char *filename)
{
    APP_FORMAT_TYPE_E fmttype;
   
    fmttype = MID_API_USB_File_Name2Format(filename);
//	APP_Printf("fmttype = %d\n", fmttype);
	if(fmttype == FORMAT_TYPE_MP3)
	{
		return FILE_TYPE_MUSIC;
	}
	else if(fmttype == FORMAT_TYPE_BMP || fmttype == FORMAT_TYPE_JPG
			|| fmttype == FORMAT_TYPE_PNG || fmttype == FORMAT_TYPE_GIF)
	{
        return FILE_TYPE_IMAGE;
	}
	else if(fmttype == FORMAT_TYPE_TXT)
	{
        return FILE_TYPE_TXT;
	}
	else if (fmttype == FORMAT_TYPE_TS)
	{
		return FILE_TYPE_FILM;
	}
	else
	{
        return FILE_TYPE_UNKNOWN;
	}
}

BS32 APP_USB_API_ClearFileList(BS08 * path)
{

	USB_DEBUG("File_delpath %s \n",path);

	if (0 == path)
    {
        return APP_ERR;
    }
#ifdef ENABLE_PVR
	MID_API_USB_File_DelTypePath(FILE_TYPE_MUSIC, path);
#endif

#ifdef ENABLE_JPEG
	MID_API_USB_File_DelTypePath(FILE_TYPE_IMAGE, path);
#endif
#ifdef ENABLE_PVR
	MID_API_USB_File_DelTypePath(FILE_TYPE_FILM, path);
#endif
#ifdef ENABLE_TXT
	MID_API_USB_File_DelTypePath(FILE_TYPE_TXT, path);
#endif
	return APP_OK;
}

BS32 MID_API_USB_GetDeviceDisc(BU32 phy_port, PATH disk_path[])
{

	BU32 dir1;
	BU32 dir2;
	BU32 dir3;
	FYF_Dir_S ptr;
    char tmp0_direct[256];
    char tmp1_direct[256];
    char save_direct[256];
    char* sys_direct = "/sys/bus/usb/devices/";
    char* scsi_direct = "/dev/scsi/";
    char* lun_direct = "/bus0/target0/lun0/";
    char* disc_direct = "disc";
    char port_direct[16];
    char* host_direct = "host";
    unsigned int disc_num = 0;
    unsigned int  usb_speed = 1;
    
    phy_port = phy_port +1;

    if((phy_port < 0) || (phy_port > MAX_PHY_PART_NUM))
    {
        USB_ERROR("phy port is error.\n");
        return APP_ERR;
    }

	for (usb_speed = 1; usb_speed <= 2; usb_speed ++)
	{
		if ((dir1 = FYF_API_dir_open(sys_direct)) == 0)
	    {
	        USB_ERROR("open dir is error.\n");
	        return APP_ERR;    
	    }

	    memset(tmp0_direct, 0, sizeof(tmp0_direct));
	    memset(tmp1_direct, 0, sizeof(tmp1_direct));
	    memset(port_direct, 0, sizeof(port_direct));
    
	    sprintf(port_direct,"%d-%d", usb_speed, phy_port);
		APP_Printf("%s\n",port_direct);
	 	while (FYF_API_dir_read(dir1, &ptr) == 0)
	    {
	    	APP_Printf("ptr->d_name1 = %s\n", ptr.name);
	        if (strstr(ptr.name, port_direct))
	        {
	            strcat(tmp0_direct, sys_direct); 
	            strcat(tmp0_direct, ptr.name); 

	            if ((dir2 = FYF_API_dir_open(tmp0_direct)) != NULL)
	            {
	              	while (FYF_API_dir_read(dir2, &ptr) == 0)
	                {
	                	APP_Printf("ptr->d_name2 = %s\n", ptr.name);
	                    if (strstr(ptr.name,host_direct))
	                    {
	                        strcat(tmp1_direct, scsi_direct); 
	                        strcat(tmp1_direct, ptr.name); 
	                        strcat(tmp1_direct, lun_direct);
	                        if ((dir3 = FYF_API_dir_open(tmp1_direct)) != NULL)
	                        {
	                        	
	                          	strcpy(save_direct,tmp1_direct);
	                           	while (FYF_API_dir_read(dir3, &ptr) == 0)
	                            {
	                                if (strcmp(ptr.name, disc_direct) == 0) //查找DISK分区
	                                {
	                                    strcat(tmp1_direct, ptr.name);
	                                    strcpy(disk_path[disc_num++], tmp1_direct); 
	                                    strcpy(tmp1_direct, save_direct);
	                                    break;     
	                                }    
	                            }
	                            FYF_API_dir_close(dir3);
	                        }            
	                    }                    
	                }
	                FYF_API_dir_close(dir2);
	            }
	        }
	    }
	    FYF_API_dir_close(dir1);
	    
	}
	
    return disc_num;
}

BS32 MID_API_USB_GetDevicePart(const char *pdisc_path, PATH ppart_path[])
{

	BU32 dir;
	FYF_Dir_S ptr;
    char* disc_direct = "disc";
    char* part_direct = "part";
    unsigned int disc_len = 0;
    unsigned int part_num = 0;  
    char tmp_direct[256];
    char save_direct[256];
    
    if(strstr(pdisc_path, disc_direct) == NULL)
    {
        USB_ERROR("get path is error.\n");
        return FYF_ERR;    
    }    
	
    memset(tmp_direct, 0, sizeof(tmp_direct));
    disc_len = strlen(pdisc_path);
    disc_len -= 4;
    memcpy(tmp_direct,pdisc_path,disc_len);

	if ((dir = FYF_API_dir_open(tmp_direct)) == NULL)    
    {
        USB_ERROR("open dir is error.\n");
        return FYF_ERR;    
    }
    strcpy(save_direct,tmp_direct);
	
    while (FYF_API_dir_read(dir, &ptr) == 0)
    {
        if(strstr(ptr.name,part_direct))   
        {
            strcat(tmp_direct, ptr.name);
            strcpy(ppart_path[part_num++],tmp_direct);
            strcpy(tmp_direct,save_direct);
        } 
    }    
    FYF_API_dir_close(dir);
    return part_num;  
}

#ifdef ENABLE_PVR
BS32 APP_USB_API_AddFile(APP_FILE_TYPE_E filetype, BU08 *filename)
{
	APP_File_Info_S *file_info = NULL;
	file_info = MID_API_USB_File_Malloc();
	if (file_info == NULL)
	{
		APP_PRINT_ERROR("MID_API_USB_File_Malloc error\n");
		return APP_ERR;
	}
	file_info->FileType = filetype;
	file_info->bUserFlag = 1;
	//文件名字
	strcpy(file_info->FileName,  filename);
			
	//增加到文件节点里
	if (MID_API_USB_File_Add(file_info) != APP_OK)
	{
		APP_PRINT_ERROR("MID_API_USB_File_Add error\n");
		return APP_ERR;
	}
	MID_Player_API_AddFilmFile(file_info);
	return APP_OK;
	
	
}
#endif

BU32 APP_USB_API_Connected(APP_USB_PORT_E phy_port)
{
	BS08 usb_device_string[256];

    if (phy_port >= USB_PORT_BUTT)
    {
        USB_ERROR("phy port is error.\n");
        return APP_FAIL;
    }

	memset(usb_device_string, 0, sizeof(usb_device_string));

	sprintf(usb_device_string, "/sys/bus/usb/devices/1-%d", phy_port + 1);
//	APP_Printf("%s\n",usb_device_string);

	return FYF_API_file_isexist(usb_device_string);

}

BU32 APP_USB_API_Mount(APP_USB_PORT_E phy_port)
{
	BU32 ret;
	BS32 disc_num,part_num,i, j, k;
   	PATH disk_path[MAX_DISC_NUM];
    PATH part_path[MAX_PART_NUM];
    FYF_Part_Info_S *partinfo;
//	char *diskdir = "/mnt";
	static BU32 detect_part_times = 0;

	char tmpdisk[256];
	BS08 mount_string[256];
	BS08 *partname[MAX_PART_NUM] = 
		{"C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R"};

    if(phy_port >= USB_PORT_BUTT)
    {
        USB_ERROR("phy port is error.\n");
        return APP_FAIL;
    }

    if(g_disc_info[phy_port].bDetected == APP_TRUE)
    {
		USB_DEBUG("usb is detected\n");
		return APP_SUCCESS;
    }
	sprintf(tmpdisk, "%s/", ROOT_PATH);
	if (FYF_API_dir_create(tmpdisk) != 0)
	{
		USB_ERROR("mkdir %s error\n", tmpdisk);
		return APP_FAIL;
	}

    memset(&(g_disc_info[phy_port]),0,sizeof(APP_Disc_Info_S));

    disc_num = MID_API_USB_GetDeviceDisc(phy_port, disk_path);
    
    if(disc_num < 0)
    {
       	USB_ERROR("get disc num error \n");
       	g_disc_info[phy_port].bErrorStatus = APP_TRUE;
    	return APP_FAIL;
    }	    
    else if(disc_num == 0)
    {
    	USB_ERROR("get disc num = %d \n",disc_num);
    	return APP_FAIL;
    }
	g_disc_info[phy_port].size = 0;
	USB_DEBUG("detect disc %s \n",disk_path[0])
   	strncpy(g_disc_info[phy_port].USBDiskPath, disk_path[0], sizeof(PATH)-1);

    part_num = MID_API_USB_GetDevicePart(g_disc_info[phy_port].USBDiskPath, part_path);
    if(part_num == -1)
    {
       	USB_ERROR("get part num error \n");
       	g_disc_info[phy_port].bErrorStatus = APP_TRUE;
    	return APP_FAIL;
    }	
	else if(part_num == 0 && ++detect_part_times < 5)
    {
       	USB_DEBUG(" part num = %d \n",part_num);
    	return APP_FAIL;    
    }

	

    detect_part_times = 0;
	printf("partnum = %d\n", part_num);
	if (part_num == 0)
	{
		part_num = 1;
		g_disc_info[phy_port].bDetected = APP_TRUE;
	    g_disc_info[phy_port].partnumber = part_num;
		strncpy(g_disc_info[phy_port].MountPath, tmpdisk, sizeof(PATH)-1);
		strncpy(part_path[0], disk_path[0], sizeof(PATH)-1);
	}
	else
	{
		g_disc_info[phy_port].bDetected = APP_TRUE;
	    g_disc_info[phy_port].partnumber = part_num;
		strncpy(g_disc_info[phy_port].MountPath, tmpdisk, sizeof(PATH)-1);
		
	}



	for (i = 0; i < part_num; i++)
	{
		for (j = 0; j < MAX_PART_NUM; j++)
			if (g_bmount[j] == 0)
				break;
		if (j >= MAX_PART_NUM)
		{
			USB_ERROR("mount part is over %d\n", j);
		}
	    partinfo = &(g_disc_info[phy_port].strPartinfo[i]);
    	strncpy(partinfo->USBPartPath, part_path[i], sizeof(PATH)-1); 

		USB_DEBUG("detect part %s type = %d \n",part_path[i],partinfo->type)

		memset(mount_string,0,256);		
		sprintf(mount_string,"%s%s",tmpdisk,partname[j]);	
		USB_DEBUG("%s",mount_string);
		strncpy(partinfo->MountPath, mount_string, sizeof(PATH)-1);

//只读文件系统不能进行创建和删除，所以只能在文件系统中先创建好挂载分区的各个盘符
#if 0
		if (FYF_API_dir_create(partinfo->MountPath) != 0)
		{
			USB_ERROR("mkdir %s error\n", partinfo->MountPath);
			return APP_FAIL;
		}
#endif		
		ret = FYF_API_USB_MountPart(partinfo);
		if(ret != APP_OK)
		{
			partinfo->bErrorStatus = APP_TRUE;
	       	USB_ERROR("mount %s device error \n",mount_string);
		}
		else
		{
			g_bmount[j] = 1;
			partinfo->PartIndex = j;
			USB_DEBUG("mount to %s success \n",mount_string);
		}

		if (APP_OK == FYF_API_GetPartInfo(partinfo->MountPath, partinfo))
		{
			g_disc_info[phy_port].size += partinfo->size;
			APP_PRINT_DEBUG("part info: %d %lld %lld\n", partinfo->type, partinfo->size, partinfo->availsize);
		}
		
	}
	return APP_SUCCESS;
}

BS32 APP_USB_API_unMount(APP_USB_PORT_E phy_port)
{
	BS32 i;
	BU32 part_num;
    FYF_Part_Info_S *partinfo;

    if(phy_port >= USB_PORT_BUTT)
    {
        USB_ERROR("phy port is error.\n");
        return APP_ERR;
    }

    if(g_disc_info[phy_port].bDetected == APP_FALSE)
    {
    	USB_DEBUG("phy port is unmount\n");
       	return APP_ERR;    
    }	

    if(g_disc_info[phy_port].bErrorStatus == APP_TRUE)
    {
       	USB_ERROR(" disc error \n");
    	return APP_ERR;    
    }

	part_num = g_disc_info[phy_port].partnumber;
    if(part_num == 0)
    {
       	USB_DEBUG(" part num = %d \n",part_num);
    	return APP_ERR;    
    }
	
	for (i = part_num - 1; i >= 0; i--)
	{
		partinfo = &(g_disc_info[phy_port].strPartinfo[i]);
		if(partinfo->bErrorStatus == APP_TRUE)
			continue;

		if(partinfo->bMounted == APP_FALSE)
			continue;

		FYF_API_USB_unMountPart(partinfo);
		partinfo->bMounted = APP_FALSE;
		g_bmount[partinfo->PartIndex] = 0;
		partinfo->PartIndex = 0;
/*		if (FYF_API_dir_remove(partinfo->MountPath) != 0)
		{
			USB_ERROR("removedir %s error\n", partinfo->MountPath);
		}
*/
	}
	/*sprintf(tmpdisk, "%s%d/", diskdir, phy_port + 1);
	if (FYF_API_dir_remove(tmpdisk) != 0)
	{
		USB_ERROR("removedir %s error\n", tmpdisk);
	//	return APP_ERR;
	}
	*/
	return APP_OK;
}

BS08 * APP_USB_API_GetMountPath(APP_USB_PORT_E phy_port)
{
	
    if(phy_port >= USB_PORT_BUTT)
    {
        USB_ERROR("phy port is error.\n");
        return APP_FAIL;
    }

    if(g_disc_info[phy_port].bDetected == APP_FALSE)
    {
        USB_ERROR("phy port is not detect.\n");  
       	return APP_FAIL;    
    }	

    if(g_disc_info[phy_port].bErrorStatus == APP_TRUE)
    {
       	USB_ERROR(" disc error \n");
    	return APP_FAIL;    
    }

    return g_disc_info[phy_port].MountPath;
}	


BS32 APP_USB_API_MakeFileList(BS08* ps8FilePath, BU08 depth)
{
	char  filename[256];	
	BU32 dir;
	FYF_Dir_S ptr;
	BU32 count = 0;

	APP_FILE_TYPE_E filetype;
	FYF_File_Info_S fileinfo_tmp;
	APP_File_Info_S *fileinfo;
	//add by cwd 2009-12-14 深度搜索只搜索五层，防止文件过多而搜索太久
	if (depth == 0)
		return;

	APP_Printf("ps8FilePath = %s\n", ps8FilePath);
	if(ps8FilePath == NULL)
		return APP_ERR;

	if (FYF_API_dir_judge(ps8FilePath) != 0)
	{
		return APP_ERR;
	}

	if ((dir = FYF_API_dir_open(ps8FilePath)) == 0)
	{
		 USB_ERROR("open dir is error.\n");
		 return APP_ERR;
	}

	while (FYF_API_dir_read(dir, &ptr) == 0)
	{
		//每个目录下最多访问300个文件
		if (++count > 300)break;
		sprintf(filename,"%s/",ps8FilePath);
		strcat(filename, ptr.name);
		if (FYF_API_dir_judge(filename) == 0)
		{
			APP_USB_API_MakeFileList(filename, depth - 1);
		}
		else
		{
			filetype = MID_API_USB_File_Name2Type(filename);
//			APP_Printf("filetype = %d\n", filetype);
			if(filetype >= FILE_TYPE_IMAGE && filetype < FILE_TYPE_UNKNOWN)
			{	
				fileinfo = MID_API_USB_File_Malloc();
				if(fileinfo == 0)
				{
					FYF_API_dir_close(dir);
					return APP_ERR;
				}
				fileinfo->bUserFlag = 1;
				//文件名字
				strcpy(fileinfo->FileName,  filename);
				//文件类型
				fileinfo->FileType = filetype;
				FYF_API_file_GetFileInfo(filename, &fileinfo_tmp);
				fileinfo->FileTime = fileinfo_tmp.FileTime;
				fileinfo->FileSize = fileinfo_tmp.FileSize;
				//增加到文件节点里
				if (MID_API_USB_File_Add(fileinfo) != APP_OK)
				{
					FYF_API_dir_close(dir);
					return APP_ERR;
				}
				APP_Printf("%-10d%-20s%7d\n",filetype,fileinfo->FileName,fileinfo->FileSize);
				APP_Printf("%d %d %d %d %d\n", fileinfo->FileTime.tm_year, fileinfo->FileTime.tm_mon, fileinfo->FileTime.tm_mday, fileinfo->FileTime.tm_hour, fileinfo->FileTime.tm_min);
			}			
		}
	}
	FYF_API_dir_close(dir);
	return APP_OK; 
}

#ifdef ENABLE_PVR

BS32 APP_USB_API_AddMp3ToDb()
{
	BU08 buf[128], bufsize;
	BU32 i, j, count;
	BU32 fp;
	BS32 fd;
	BS32 pos;
	Player_Mp3File_S mp3file;
	count = g_file_group[FILE_TYPE_MUSIC].FileNum;
	APP_PRINT_DEBUG("APP_USB_API_AddFilmToDb count = %d\n", count);
	MID_Player_API_InitMp3file();
	for (i = 0, j = 0; j < count; i++)
	{
		if (g_file_group[FILE_TYPE_MUSIC].FileInfo[i] == NULL)
		{
			continue;
		}
		
		mp3file.Filelen = g_file_group[FILE_TYPE_MUSIC].FileInfo[i]->FileSize;
		mp3file.filemtime = g_file_group[FILE_TYPE_MUSIC].FileInfo[i]->FileTime;
		strcpy(mp3file.filename, g_file_group[FILE_TYPE_MUSIC].FileInfo[i]->FileName);
		if ((fp = FYF_API_File_Open(mp3file.filename, "r")) == NULL)
		//if ((fd = open(mp3file.filename, O_RDWR, 0x777)) < 0)
		{
			USB_ERROR("open file %s fail\n", mp3file.filename);
			//close(fd);
			FYF_API_File_Close(fp);
		}
		else
		{
			pos = -128;
			FYF_API_File_Ioctl(fp, FYF_FILE_SEEK_END,&pos);
			//lseek(fd, -128, SEEK_END);
			if ((bufsize = FYF_API_File_Read(fp, buf, 128)) != 128)
			//if ((bufsize = read(fd, buf, 128) < 0))
			{
				USB_ERROR("Read file %s fail\n", mp3file.filename);
			}
			else
			{
				if(!((buf[0]=='T'||buf[0]=='t')&&(buf[1]=='A'||buf[1]=='a')&&(buf[2]=='G'||buf[2]=='g')))
				{
					USB_ERROR("file %s is not mp3 file\n", mp3file.filename);
					mp3file.mp3name[0] = 0;
					mp3file.mp3singer[0] = 0;
					mp3file.mp3special[0] = 0;
				}
				else
				{
					memcpy(mp3file.mp3name, buf + 3, 30);
					memcpy(mp3file.mp3singer, buf + 33, 30);
					memcpy(mp3file.mp3special, buf + 63, 30);
					USB_DEBUG("mp3name = %s, mp3singer = %s %s\n", mp3file.mp3name, mp3file.mp3singer, buf + 63);
				}
			}
			//close(fd);
			FYF_API_File_Close(fp);
			MID_Player_API_AddMp3File(&mp3file);
		}
		j++;
	}
	return APP_OK;
}
#endif

#ifdef ENABLE_JPEG
BS32 APP_USB_API_AddPictureToDb()
{
	BU32 i, j;
	BU32 count = g_file_group[FILE_TYPE_IMAGE].FileNum;
	printf("APP_USB_API_AddFilmToDb count = %d\n", count);
	MID_PictureDecoder_API_InitPicfile();
	for (i = 0, j = 0; j < count; i++)
	{
		if (g_file_group[FILE_TYPE_IMAGE].FileInfo[i] == NULL)
		{
			continue;
		}
		
		APP_Printf("%d %s\n", i, g_file_group[FILE_TYPE_IMAGE].FileInfo[i]->FileName);
		MID_PictureDecoder_API_AddPicFile(g_file_group[FILE_TYPE_IMAGE].FileInfo[i]);
		j++;
	}
	
}
#endif

#ifdef ENABLE_PVR
BS32 APP_USB_API_AddFilmToDb()
{
	BU32 i, j;
	BU32 count = g_file_group[FILE_TYPE_FILM].FileNum;
	APP_PRINT_DEBUG("APP_USB_API_AddFilmToDb count = %d\n", count);
	MID_Player_API_InitFilmfile();
	for (i = 0, j = 0; j < count; i++)
	{
		if (g_file_group[FILE_TYPE_FILM].FileInfo[i] == NULL)
		{
			continue;
		}
		APP_Printf("%d %s\n", i, g_file_group[FILE_TYPE_FILM].FileInfo[i]->FileName);
		MID_Player_API_AddFilmFile(g_file_group[FILE_TYPE_FILM].FileInfo[i]);
		j++;
	}
}

#endif
BS32 APP_USB_API_SearchUSBPort(APP_USB_PORT_E usb_port)
{
	BU32 partnum = g_disc_info[usb_port].partnumber;
	BU32 i;
	for (i = 0; i < partnum; i++)
	{
		APP_Printf("g_disc_info[usb_port].strPartinfo[i].MountPath = %s\n", g_disc_info[usb_port].strPartinfo[i].MountPath);
		APP_USB_API_MakeFileList(g_disc_info[usb_port].strPartinfo[i].MountPath, 5);
	}
}

BS32 APP_USB_API_ClearUSBPort(APP_USB_PORT_E usb_port)
{
	BU32 partnum = g_disc_info[usb_port].partnumber;
	BU32 i;
	for (i = 0; i < partnum; i++)
	{
		APP_Printf("g_disc_info[usb_port].strPartinfo[i].MountPath = %s\n", g_disc_info[usb_port].strPartinfo[i].MountPath);
		APP_USB_API_ClearFileList(g_disc_info[usb_port].strPartinfo[i].MountPath);
	}
	memset(&(g_disc_info[usb_port]), 0, sizeof(APP_Disc_Info_S));
}
void USBDetectThread()
{

//	APP_Printf("USBDetectThread = %d\n",   FYF_API_thread_getCurID());
	BU08 usbflag = 0;
	MID_API_USB_File_Init();
	FYF_API_thread_sleep(10000);
	while (1)
	{
		if (APP_USB_API_Connected(USB_PORT_0) == APP_SUCCESS)
		{
			if (g_disc_info[USB_PORT_0].bDetected == APP_FALSE)
			{
				if (APP_SUCCESS == APP_USB_API_Mount(USB_PORT_0))
				{
					APP_Printf("usb mount /mnt/disc1 success\n");
					APP_USB_API_SearchUSBPort(USB_PORT_0);
					//APP_USB_API_MakeFileList("/mnt/Disc1");
					APP_PRINT_DEBUG("FileInfoNodeRemain = %d\n", g_FileInfoNodeRemain);
					usbflag = 1;//
				}
				
			}
		}
		else
		{
			if (g_disc_info[USB_PORT_0].bDetected == APP_TRUE)
			{
				if (APP_OK == APP_USB_API_unMount(USB_PORT_0))
				{
					APP_Printf("usb unmount /mnt/disc1 success\n");
					APP_USB_API_ClearUSBPort(USB_PORT_0);
					//APP_USB_API_ClearFileList("/mnt/Disc1");
					APP_PRINT_DEBUG("FileInfoNodeRemain = %d\n", g_FileInfoNodeRemain);
					usbflag = 2;//APP_USB_API_AddMp3ToDb();
				}
			}
		}
		
		if (APP_USB_API_Connected(USB_PORT_1) == APP_SUCCESS)
		{
			if (g_disc_info[USB_PORT_1].bDetected == APP_FALSE)
			{
				if (APP_SUCCESS == APP_USB_API_Mount(USB_PORT_1))
				{
					APP_Printf("usb mount /mnt/disc2 success\n");
					APP_USB_API_SearchUSBPort(USB_PORT_1);
					//APP_USB_API_MakeFileList("/mnt/Disc2");
					usbflag = 1;//APP_USB_API_AddMp3ToDb();
					APP_PRINT_DEBUG("FileInfoNodeRemain = %d\n", g_FileInfoNodeRemain);
				}
		
			}
		}
		else
		{
			if (g_disc_info[USB_PORT_1].bDetected == APP_TRUE)
			{
				if (APP_OK == APP_USB_API_unMount(USB_PORT_1))
				{
					APP_Printf("usb unmount /mnt/disc2 success\n");
					//APP_USB_API_ClearFileList("/mnt/Disc2");
					APP_USB_API_ClearUSBPort(USB_PORT_1);
					APP_PRINT_DEBUG("FileInfoNodeRemain = %d\n", g_FileInfoNodeRemain);
					usbflag = 2;//APP_USB_API_AddMp3ToDb();
				}
			}
		}
		if (usbflag)
		{
#ifdef ENABLE_PVR
			APP_USB_API_AddMp3ToDb();
#endif
#ifdef ENABLE_JPEG
			APP_USB_API_AddPictureToDb();
#endif
#ifdef ENABLE_PVR
			APP_USB_API_AddFilmToDb();
#endif

			UI_APP_API_USBDetected_CallBack(usbflag);
			usbflag = 0;		
		}
		FYF_API_thread_sleep(1000);
	}
}

BS32 APP_USB_API_GetTypeFileNum(APP_FILE_TYPE_E filetype, BU32 *filenum)
{
	if (filetype >= FILE_TYPE_UNKNOWN)
	{	
		USB_ERROR("file type unknow\n");
		return APP_ERR;
	}
	*filenum = g_file_group[filetype].FileNum;
	return APP_OK;
}

BS32 APP_USB_API_GetTypeFileGroup(APP_FILE_TYPE_E filetype, APP_File_Group_Info_S *filegroup)
{
	if (filegroup == NULL)
	{
		USB_ERROR("file group is NULL pointer\n");
		return APP_ERR;
	}
	if (filetype >= FILE_TYPE_UNKNOWN)
	{	
		USB_ERROR("file type unknow\n");
		return APP_ERR;
	}	
	FYF_API_semaphore_wait(semaphoreUSB, FYF_WAIT_FOREVER);
	memcpy(filegroup, &(g_file_group[filetype]), sizeof(APP_File_Group_Info_S));
	FYF_API_semaphore_release(semaphoreUSB);
}

BS32 APP_USB_API_GetTypeFileInfo(APP_FILE_TYPE_E filetype, BU32 index, APP_File_Info_S *fileinfo)
{
	if (fileinfo == NULL)
	{
		USB_ERROR("file group is NULL pointer\n");
		return APP_ERR;
	}
	if (filetype >= FILE_TYPE_UNKNOWN)
	{	
		USB_ERROR("file type unknow\n");
		return APP_ERR;
	}	
	if (index >= g_file_group[filetype].FileNum)
	{
		USB_ERROR("index is over\n");
		return APP_ERR;
	}
	FYF_API_semaphore_wait(semaphoreUSB, FYF_WAIT_FOREVER);
	memcpy(fileinfo, (g_file_group[filetype].FileInfo[index]), sizeof(APP_File_Info_S));
	FYF_API_semaphore_release(semaphoreUSB);
	
}
	
BS32 APP_USB_API_isDir(const BS08* dirpath)
{
	if (dirpath == NULL)
	{
		USB_ERROR("param is error\n");
		return APP_ERR;
	}

	return FYF_API_dir_judge(dirpath);
}

BS32 APP_USB_API_EnterDir(BS08 *dirpath, APP_Dir_Info_S * filedir)
{
	BU32 dir;	
	FYF_Dir_S ptr;
	FYF_File_Info_S fileinfo_tmp;
	BU08 flag = 0;
	BU08 root_path_flag = 0;
	BU32 len = strlen(dirpath);
	if (dirpath == NULL || filedir == NULL)
	{
		USB_ERROR("param is error\n");
		return APP_ERR;
	}	
	if ((dir = FYF_API_dir_open(dirpath)) == NULL)
	{
		return APP_ERR;
	}
	filedir->FileNum = 0;
	if (dirpath[len-1] != '/') 
	{
		dirpath[len++] = '/';
		dirpath[len] = 0;
		flag = 1;
	}
	if (strcmp(dirpath, "/mnt/") == 0)
	{
		root_path_flag = 1;
	}
	while (FYF_API_dir_read(dir, &ptr) == 0)
	{
		//cwd add 
		//根目录下没被挂载的盘符不显示
		if (root_path_flag)
		{
			APP_Printf("ptr.name = %s\n", ptr.name);
			if (g_bmount[ptr.name[0] - 'C'] == 0)
				continue;

		}
		memcpy(filedir->FileInfo[filedir->FileNum].FileName,ptr.name,strlen(ptr.name)+1);
		strcat(dirpath, ptr.name);
		memcpy(filedir->FileInfo[filedir->FileNum].FilePath,dirpath,strlen(dirpath)+1);
		if(ptr.flag == 1)//目录节点
		{
			filedir->FileInfo[filedir->FileNum].FileType = FILE_TYPE_DIR;
		}
		else //文件节点
		{
			filedir->FileInfo[filedir->FileNum].FileType = MID_API_USB_File_Name2Type(ptr.name);
		}
		FYF_API_file_GetFileInfo(dirpath, &fileinfo_tmp);
		filedir->FileInfo[filedir->FileNum].FileTime = fileinfo_tmp.FileTime;
		filedir->FileInfo[filedir->FileNum].FileSize = fileinfo_tmp.FileSize;
		filedir->FileNum++;	
		dirpath[len] = 0;
	}
	if (flag == 1)
		dirpath[len-1] = 0;
	if(FYF_API_dir_close(dir) != 0)
	{
		return APP_ERR;
	}
	return APP_OK;
	
}

BS32 APP_USB_API_DetectedUSB()
{
	if (g_disc_info[USB_PORT_0].bDetected == APP_FALSE && g_disc_info[USB_PORT_1].bDetected == APP_FALSE)
	{
		return APP_ERR;
	}
	return APP_OK;
}
/******************************
描述:检测当前U盘个数及存在的U口位置，
	0--2个U口都没插入U盘
	1--1个U盘插入1号U口
	2--1个U盘插入2号U口
	3--2个U口都插入U盘
*******************************/
BU32 APP_USB_API_DetectedWhichUSB()
{
	if (g_disc_info[USB_PORT_0].bDetected == APP_FALSE && g_disc_info[USB_PORT_1].bDetected == APP_FALSE)
	{
		return 0;
	}
	else if(g_disc_info[USB_PORT_0].bDetected == APP_TRUE && g_disc_info[USB_PORT_1].bDetected == APP_FALSE)
	{
		return 1;
	}
	else if(g_disc_info[USB_PORT_0].bDetected == APP_FALSE && g_disc_info[USB_PORT_1].bDetected == APP_TRUE)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

/******************************
打开该目录后保存目录信息
*******************************/
BU32 APP_USB_API_SaveDirInfo(BU08* DirPath)
{
	if(DirPath == NULL)
	{
		return APP_FAIL;
	}
	return APP_USB_API_EnterDir(DirPath,&CurDirContent);
}
/******************************
打开当前目录并保存目录信息
*******************************/
BU32 APP_USB_API_SaveCurDirInfo(void)
{
	APP_Printf("APP_USB_API_SaveCurDirInfo \n");
	APP_Printf("CurFilePath = %s \n",CurFilePath);	
	return APP_USB_API_EnterDir(CurFilePath,&CurDirContent);
}
/******************************
初始化当前路径
*******************************/
BU32 APP_USB_API_InitCurPath(void)
{

	//BU08 *disc_path  = "/mnt";
	if(CurFilePath == NULL)
	{
		return APP_FAIL;
	}
/*	if(g_disc_info[USB_PORT_0].bDetected == APP_TRUE && g_disc_info[USB_PORT_1].bDetected == APP_FALSE)
	{
		strcpy(CurFilePath,disc1_path);
		CurFilePath[5] = 0;
	}
	else if(g_disc_info[USB_PORT_0].bDetected == APP_FALSE && g_disc_info[USB_PORT_1].bDetected == APP_TRUE)
	{
		strcpy(CurFilePath,disc2_path);
		CurFilePath[5] = 0;
	}
	else 
	*/
	if(g_disc_info[USB_PORT_0].bDetected == APP_TRUE || g_disc_info[USB_PORT_1].bDetected == APP_TRUE)
	{
		strcpy(CurFilePath,ROOT_PATH);
		CurFilePath[4] = 0;
	}
	else
	{
		memset(CurFilePath,0,sizeof(CurFilePath));
		return APP_FAIL;
	}
	return APP_SUCCESS;
}
/******************************
设置当前路径
*******************************/
BU32 APP_USB_API_SetCurPath(const BU08* FilePath)
{
	if(FilePath == NULL || CurFilePath == NULL)
	{
		return APP_FAIL;
	}
	strcpy(CurFilePath,FilePath);
	return APP_SUCCESS;
}
/******************************
获取当前路径
*******************************/
BU32 APP_USB_API_GetCurPath(BU08* FilePath)
{
	if(FilePath == NULL || CurFilePath == NULL)
	{
		return APP_FAIL;
	}
	strcpy(FilePath,CurFilePath);
	APP_Printf("CurFilePath = %s\n",CurFilePath);
	return APP_SUCCESS;
}

BU32 APP_USB_API_GetRelativePath(BU08 *abspath, BU08* relpath)
{
	BU32 i, len, cplen;
	if (abspath == NULL || relpath == NULL)
	{
		return APP_FAIL;
	}
	
	len = strlen(abspath);
	for (i = len - 1; i >= 0; i--)
	{
		if (abspath[i] == '/' && i != len-1)break;
	}

	memcpy(relpath, &(abspath[i + 1]), len - i);
	return APP_SUCCESS;
	
}

/******************************
获取目录名或文件名
*******************************/
BU32 APP_USB_API_GetFileName(const BU08* FilePath,BU08* FileName)
{
	if(FilePath == NULL || FileName == NULL)
	{
		return APP_FAIL;
	}
	return APP_USB_API_GetRelativePath(FilePath,FileName);
}
/******************************
获取目录或文件的绝对路径
*******************************/
BU32 APP_USB_API_GetFilePath(const BU08* FileName,BU08* FilePath)
{
	BU32 len;
	if(FileName == NULL || FilePath == NULL || CurFilePath == NULL)
	{
		return APP_FAIL;
	}
	memset(FilePath,0,strlen(FilePath));
	len = strlen(CurFilePath);
	if(CurFilePath[len-1] == '/') 
	{
		strcat(FilePath,CurFilePath);
		strcat(FilePath,FileName);
	}
	else
	{
		sprintf(FilePath,"%s/%s",CurFilePath,FileName);
	}
	return APP_SUCCESS;
}
/******************************
获取根目录
*******************************/
BU32 APP_USB_API_GetRootName(const BU08* FilePath,BU08* RootName)
{
	//BU08 *disc1_path = "/mnt";
	//BU08 *disc2_path = "/mnt";
//	BU08 *disc_path  = "/mnt";
	if(FilePath == NULL || RootName == NULL)
	{
		return APP_FAIL;
	}
	/*
	if(g_disc_info[USB_PORT_0].bDetected == APP_TRUE && g_disc_info[USB_PORT_1].bDetected == APP_FALSE)
	{
		if(strstr(FilePath,disc1_path) == NULL)
		{
			return APP_FAIL;
		}
	}
	else if(g_disc_info[USB_PORT_0].bDetected == APP_FALSE && g_disc_info[USB_PORT_1].bDetected == APP_TRUE)
	{
		if(strstr(FilePath,disc2_path) == NULL)
		{
			return APP_FAIL;
		}
	}
	*/
	if(g_disc_info[USB_PORT_0].bDetected == APP_TRUE || g_disc_info[USB_PORT_1].bDetected == APP_TRUE)
	{
		if(strstr(FilePath,ROOT_PATH) == NULL)
		{
			return APP_FAIL;
		}
		strncpy(RootName,FilePath,4);
		RootName[4] = 0;
		return APP_SUCCESS;
	}
	else
	{
		return APP_FAIL;
	}
}

/******************************
获取上级目录路径
*******************************/
BU32 APP_USB_API_GetParentDirPath(const BU08* FilePath,BU08* ParentDirPath)
{
	BU32 i, len;
	BU08 root_path[FILE_PATH_LEN];
	if(FilePath == NULL || ParentDirPath == NULL)
	{
		return APP_FAIL;
	}
	len = strlen(FilePath);
	for(i = len - 1; i >= 0; i--)
	{
		if(FilePath[i] == '/' && i != len-1) break;
	}
	strncpy(ParentDirPath, FilePath, i);
	ParentDirPath[i] = 0;
	if(APP_USB_API_GetRootName(FilePath,root_path) == APP_FAIL)
	{
		memset(ParentDirPath,0,strlen(ParentDirPath));
		return APP_FAIL;
	}
	//判断ParentDirPath是不是包含了根目录的路径
	if(strstr(ParentDirPath,root_path) == NULL)
	{
		memset(ParentDirPath,0,strlen(ParentDirPath));
		return APP_FAIL;
	}
	return APP_SUCCESS;
}
/******************************
获取上级目录名
*******************************/
BU32 APP_USB_API_GetParentDirName(const BU08* FilePath,BU08* ParentDirName)
{
	BU08 ParentDirPath[FILE_PATH_LEN];
	if(APP_USB_API_GetParentDirPath(FilePath,ParentDirPath) == APP_SUCCESS)
	{
		return APP_USB_API_GetFileName(ParentDirPath,ParentDirName);
	}
	return APP_FAIL;
}
/******************************
获取目录下文件名和目录名
*******************************/
BU32 APP_USB_API_GetSubFileName(const BU08* DirPath,BU32 Index,BU08* SubFileName)
{
	APP_Dir_Info_S file_dir;
	if(DirPath == NULL || SubFileName == NULL)
	{
		return APP_FAIL;
	}
	if(APP_USB_API_EnterDir(DirPath,&file_dir) != 0)
	{
		return APP_FAIL;
	}
	strcpy(SubFileName,file_dir.FileInfo[Index].FileName);
	return APP_SUCCESS;
}
/******************************
获取目录下文件和目录路径
*******************************/
BU32 APP_USB_API_GetSubFilePath(const BU08* DirPath,BU32 Index,BU08* SubFilePath)
{
	APP_Dir_Info_S file_dir;
	if(DirPath == NULL || SubFilePath == NULL)
	{
		return APP_FAIL;
	}
	if(APP_USB_API_EnterDir(DirPath,&file_dir) != 0)
	{
		return APP_FAIL;
	}
	strcpy(SubFilePath,file_dir.FileInfo[Index].FilePath);
	return APP_SUCCESS;
}
/******************************
获取目录下的文件总数
*******************************/
BU32 APP_USB_API_GetFileCount(const BU08* DirPath)
{
	APP_Dir_Info_S file_dir;
	if(DirPath == NULL)
	{
		return APP_FAIL;
	}
	if(APP_USB_API_EnterDir(DirPath,&file_dir) != 0)
	{
		return APP_FAIL;
	}
	return file_dir.FileNum;
}
/******************************
获取当前文件或目录的根目录
*******************************/
BU32 APP_USB_API_GetCurRootName(BU08* RootName)
{
	if(CurFilePath == NULL || RootName == NULL)
	{
		return APP_FAIL;
	}
	return APP_USB_API_GetRootName(CurFilePath,RootName);
}
/******************************
获取当前目录的上级目录路径
*******************************/
BU32 APP_USB_API_GetCurParentDirPath(BU08* ParentDirPath)
{
	if(CurFilePath == NULL || ParentDirPath == NULL)
	{
		return APP_FAIL;
	}
	return APP_USB_API_GetParentDirPath(CurFilePath,ParentDirPath);
}
/******************************
获取当前目录的上级目录名
*******************************/
BU32 APP_USB_API_GetCurParentDirName(BU08* ParentDirName)
{
	BU08 ParentDirPath[FILE_PATH_LEN];
	if(ParentDirName == NULL)
	{
		return APP_FAIL;
	}
	if(APP_USB_API_GetCurParentDirPath(ParentDirPath) == APP_SUCCESS)
	{
		return APP_USB_API_GetFileName(ParentDirPath,ParentDirName);
	}
	return APP_FAIL;
}
/******************************
获取当前目录下的文件总数
*******************************/
BU32 APP_USB_API_GetCurFileCount(void)
{
	return CurDirContent.FileNum;
}
/******************************
获取当前目录下文件和目录信息
*******************************/
BU32 APP_USB_API_GetCurSubFileInfo(BU32 Index, APP_File_Info_S* SubFileInfo)
{
	if(SubFileInfo == NULL)
	{
		return APP_FAIL;
	}
	if(Index >= CurDirContent.FileNum)
	{
		return APP_FAIL;
	}
	APP_Printf("CurDirContent.FileInfo[%d].FileName = %s\n",Index,CurDirContent.FileInfo[Index].FileName);
	memcpy(SubFileInfo,&(CurDirContent.FileInfo[Index]),sizeof(APP_File_Info_S));
	APP_Printf("SubFileInfo->FileName = %s\n",SubFileInfo->FileName);
	return APP_SUCCESS;
}
/******************************
获取当前目录下文件名和目录名
*******************************/
BU32 APP_USB_API_GetCurSubFileName(BU32 Index,BU08 *SubFileName)
{
	if(SubFileName == NULL)
	{
		return APP_FAIL;
	}
	if(Index >= CurDirContent.FileNum)
	{
		return APP_FAIL;
	}
	strcpy(SubFileName,CurDirContent.FileInfo[Index].FileName);
	return APP_SUCCESS;
}
/******************************
获取当前目录下文件和目录路径
*******************************/
BU32 APP_USB_API_GetCurSubFilePath(BU32 Index,BU08 *SubFilePath)
{
	if(SubFilePath == NULL)
	{
		return APP_FAIL;
	}
	if(Index >= CurDirContent.FileNum)
	{
		return APP_FAIL;
	}
	strcpy(SubFilePath,CurDirContent.FileInfo[Index].FilePath);
	return APP_SUCCESS;
}
/******************文件操作********************/
/******************************
打开文件
*******************************/
BU32 APP_USB_API_OpenFile(const BU08* FilePath,const BU08 *Mode)
{
	BU32 fp;
	if(FilePath == NULL)
	{
		return APP_FAIL;
	}
	if((fp = FYF_API_File_Open(FilePath,Mode)) == 0)
	{
		return APP_FAIL;
	}
	return fp;
}
/******************************
读文件
*******************************/
BU32 APP_USB_API_ReadFile(BU32 fd, BU08 *buf, BU32 maxlen)
{
	BS32 read_len;
	if(fd < 0)
	{
		return APP_FAIL;
	}
	if((read_len = FYF_API_File_Read(fd,buf,maxlen)) <= 0)
	{
		return APP_FAIL;
	}
	return (BU32)read_len;
}
/******************************
写文件
*******************************/
BU32 APP_USB_API_WriteFile(BU32 fd, const BU08 *buf, BU32 len)
{
	BS32 write_len;
	if(fd < 0)
	{
		return APP_FAIL;
	}
	
	if((write_len = FYF_API_File_Write(fd,buf,len)) <= 0)
	{
		return APP_FAIL;
	}
	return write_len;
}
/******************************
关闭文件
*******************************/
BU32 APP_USB_API_CloseFile(BU32 fd)
{
	if(fd < 0)
	{
		return APP_FAIL;
	}
	if((FYF_API_File_Close(fd)) != 0)
	{
		return APP_FAIL;
	}
	return APP_SUCCESS;
}


/********************************************************************************************************
功能：对文件属性进行控制或读取操作。
原型：BU32 APP_USB_API_FileIoctl(BU32 fd,APP_FILE_IOCTL_E op,BU32 *arg)
参数说明：
输入参数：
   fd：APP_USB_API_File_Open 函数返回的打开文件的句柄；
   op: 操作命令；
   arg：操作命令所带的参数，当传递枚举型或32 位整数值时，arg 可强制转换
成对应数据类型。

命令（op）             命令参数（arg）               说明
APP_FILE_SEEK_SET                           将文件读写指针移到距
						                    文件头指定偏移量的位置

APP_FILE_SEEK_CUR       BU32 *,             将文件读写指针移到距
                                                       当前指定偏移量的位置

APP_FILE_SEEK_END       BU32 *,             将文件读写指针移到距
                                                       文件尾指定偏移量的位置

APP_FILE_TRUNCATE       BU32 *,             将文件裁减到指定长
                                                       度，尾部内容丢弃。

APP_FILE_FLUSH             void                     将缓冲区中内容写入存
                                                       储设备

APP_FILE_GET_POSITION   BU32 *,             读取当前文件读写指针。

输出参数：无。
返 回：
FYF_OK: 操作成功。
FYF_ERR:操作失败。
********************************************************************************************************/
BU32 APP_USB_API_FileIoctl(BU32 fd,APP_FILE_IOCTL_E op,BU32 *arg)
{
	switch (op)
	{
	case APP_FILE_SEEK_SET:
		if(FYF_API_File_Ioctl(fd,FYF_FILE_SEEK_SET,arg)!= 0)
		{
			return APP_FAIL;
		}
		return APP_SUCCESS;
		break;
	case APP_FILE_SEEK_CUR:
		if(FYF_API_File_Ioctl(fd,FYF_FILE_SEEK_CUR,arg)!= 0)
		{
			return APP_FAIL;
		}
		return APP_SUCCESS;
		break;
	case APP_FILE_SEEK_END:
		if(FYF_API_File_Ioctl(fd,FYF_FILE_SEEK_END,arg)!= 0)
		{
			return APP_FAIL;
		}
		return APP_SUCCESS;
		break;
	case APP_FILE_TRUNCATE:
		if(FYF_API_File_Ioctl(fd,FYF_FILE_SEEK_END,arg)!= 0)
		{
			return APP_FAIL;
		}
		return APP_SUCCESS;
		break;
	case APP_FILE_FLUSH:
		if(FYF_API_File_Ioctl(fd,FYF_FILE_FLUSH,0)!= 0)
		{
			return APP_FAIL;
		}
		return APP_SUCCESS;
		break;
	case APP_FILE_GET_POSITION:
		if(FYF_API_File_Ioctl(fd,FYF_FILE_GET_POSITION,arg)!= 0)
		{
			return APP_FAIL;
		}
		return APP_SUCCESS;
		break;
	default:
		break;
	}
}

/******************************
读文本文件内容
*******************************/
BU32 APP_USB_API_ReadTextFileContent(const BU08 *FilePath, BU08 *Buf, BU32 maxlen)
{
	#if 0
	BS32 read_len;
	BU32 fp;
	if(FilePath == NULL || Buf == NULL)
	{
		return APP_FAIL;
	}
	if((fp = FYF_API_File_Open(FilePath,"r+")) == 0)
	{
		return APP_FAIL;
	}
	while(!feof((FILE*)fp))
	{
		Buf[i++] = fgetc((FILE*)fp);
	}
	if((FYF_API_File_Close(fp)) != 0)
	{
		return APP_FAIL;
	}
	return APP_SUCCESS;
	return i;
	#endif

}

/******************************
删除文件
*******************************/
BU32 APP_USB_API_DeleteFile(const BU08* FilePath)
{
	if(FilePath == NULL)
	{
		return APP_FAIL;
	}
	if(FYF_API_File_Delete(FilePath) == 0)
	{
		return APP_SUCCESS;
	}
	else
	{
		return APP_FAIL;
	}		
}
/******************目录操作********************/
/******************************
删除目录
*******************************/
BU32 APP_USB_API_DeleteDir(const BU08* DirPath)
{
	BU32 dir;
	BS32 ret;
	FYF_Dir_S ptr;
	BU08 dir_path[FILE_PATH_LEN];
	BU32 len = strlen(DirPath);
	if (DirPath == NULL)
	{
		USB_ERROR("param is error\n");
		return APP_ERR;
	}
	memcpy(dir_path,DirPath,len+1);
	if ((dir = FYF_API_dir_open(dir_path)) == 0)
	{
		return APP_ERR;
	}
	if(dir_path[len-1] != '/') 
	{
		dir_path[len++] = '/';
		dir_path[len] = 0;
	}
	ret = FYF_API_dir_read(dir, &ptr);
#if 0
	while(FYF_API_dir_read(dir, &ptr) == 0)
	{
		strcat(dir_path, ptr.name);
		if(ptr.flag == 1)
		{
			if(APP_USB_API_DeleteDir(dir_path) == APP_ERR)
			{
				return APP_ERR;
			}
		}
		else
		{
		APP_USB_API_DeleteFile(dir_path);
		}
		dir_path[len] = 0;
	}
#endif
	if(FYF_API_dir_close(dir) != 0)
	{
		return APP_ERR;
	}
	if (ret == FYF_ERR)
	{
		if(FYF_API_dir_remove(DirPath) == 0)
		{
			return APP_SUCCESS;
		}
	}
	return APP_FAIL;
}

/******************获取文件信息********************/
/******************************
通过文件名获取文件信息
*******************************/
BU32 APP_USB_API_GetFileInfoByFileName(BS08 *filename, APP_File_EventInfo_S *fileinfo)
{
    FYF_File_Info_S file;
    if(FYF_API_file_GetFileInfo(filename, &file) != FYF_OK)
    {
        return APP_ERR;
    }
	APP_Printf("FYF_API_file_GetFileInfo:---FileSize:%d, time:%d-%d",file.FileSize,file.FileTime.tm_mon,file.FileTime.tm_mday);
	memcpy(fileinfo, (APP_File_EventInfo_S *)&file, sizeof(APP_File_EventInfo_S));
	return APP_SUCCESS;
}
/******************************
获取当前目录下选中的文件索引
*******************************/
BU32 APP_USB_API_GetCurDirCurFileNo()
{
    return g_usb_CurDirCurFileNo;
}

void APP_USB_API_SetCurDirCurFileNo(BU32 curNo)
{
    g_usb_CurDirCurFileNo = curNo;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

