/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.        
    
    $ver0.0.0.1 $author Huangzhiguo 2005/10/18
*********************************************************************/
#include "ipanel_osfile.h"
#include "trd_ipanel_init.h"

#define OPEN_FILE_MAX_ON_SAME_TIME   (10)
#define OPEN_DIRECT_MAX_ON_SAME_TIME (5)

/**********************************************************************************/
/*Description: open a OS file                                                     */
/*Input      : file name, and mode. the mode is only 'rb', other no supper        */
/*Output     : No                                                                 */
/*Return     : file description, -1 fail.                                         */
/**********************************************************************************/
int ipanel_porting_localfile_open(const char *filename, const char *mode)
{
	int index = -1;
	IPANEL_DEBUG("ipanel_porting_localfile_open\n");
/*
        printf("[ipanel_porting_localfile_open]\n");

	for (index = 1; index < OPEN_FILE_MAX_ON_SAME_TIME; index++) {
		if (-1 == eisfile[index]) break;
	}
	
	if (index == OPEN_FILE_MAX_ON_SAME_TIME) return -1;

        printf("[ipanel_porting_localfile_open]The file name is %s\n", filename);

	eisfile[index] = open(filename, O_RDONLY);
	
	if (-1 != eisfile[index]) return index;
	else return -1;
	
	index = open(filename, O_RDONLY);
	if (-1 != index) return index;
	else return -1;
*/
	return index;
}


/**********************************************************************************/
/*Description: Read data from the OS file                                         */
/*Input      : the buffer and max length to save data                             */
/*Output     : data                                                               */
/*Return     : real data length, -1 -- fail.                                      */
/**********************************************************************************/
int ipanel_porting_localfile_read(int fd, char* buffer, int nbytes)
{

	int len = -1;
	IPANEL_DEBUG("ipanel_porting_localfile_read\n");
/*
	if (fd > 0 && fd<OPEN_FILE_MAX_ON_SAME_TIME && -1 != eisfile[fd])
		len = read(eisfile[fd], buffer, nbytes);

        len = read(fd, buffer, nbytes);
*/	
	return len;
}

/**********************************************************************************/
/*Description: Close the OS file                                                  */
/*Input      : the OS file description                                            */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail.                                          */
/**********************************************************************************/
int ipanel_porting_localfile_close(int fd)
{
	int ret = -1;
	IPANEL_DEBUG("ipanel_porting_localfile_close\n");
/*
	if (fd > 0 && fd<OPEN_FILE_MAX_ON_SAME_TIME && -1 != eisfile[fd])
	{	    
		ret = close(eisfile[fd]);
		eisfile[fd] = -1;
	}

        close(fd);
*/
	return ret;
}


#if 0
/**********************************************************************************/
/*Description: open a OS directory                                                */
/*Input      : path name                                                          */
/*Output     : No                                                                 */
/*Return     : directory description, -1 fail.                                    */
/**********************************************************************************/
int ipanel_porting_localdir_open(const char *path)
{
	int index;
	for(index = 0; index < OPEN_DIRECT_MAX_ON_SAME_TIME; index++) {
		if (NULL == eisdir[index]) break;
	}

	if (OPEN_DIRECT_MAX_ON_SAME_TIME == index) return -1;

	eisdir[index] = opendir(path);

	if(NULL != eisdir[index]) return index;
	else return -1;
}


/**********************************************************************************/
/*Description: Read data from the OS directory                                    */
/*Input      : address.                                                           */
/*Output     : directory EisDir data struct                                       */
/*Return     : 0 -- success, -1 -- fail.                                      */
/**********************************************************************************/
int ipanel_porting_localdir_read(int dd, EisDir **dir)
{
	int ret = -1;
	
	if (dd>=0 && dd<OPEN_DIRECT_MAX_ON_SAME_TIME && NULL != eisdir[dd]) {
		while(readdir(eisdir[dd])) { /*scandir( data); */
			/*write information to eisdirdata*/
			/*such as :*/
			eisdirdata[dd].numberofsubdirs = 0;
			eisdirdata[dd].numberoffiles = 2;
			eisdirdata[dd].filename[0] = "file1-demo-ipanel";
			eisdirdata[dd].filename[1] = "file2-demo-ipanel";
		}
		*dir = &eisdirdata[dd];
		ret = 0;
	}

	return ret;
}


/**********************************************************************************/
/*Description: Close the OS directory                                             */
/*Input      : the OS directory description                                       */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail.                                          */
/**********************************************************************************/
int ipanel_porting_localdir_close(int dd)
{
	int ret = -1;

	if (dd>=0 && dd<OPEN_DIRECT_MAX_ON_SAME_TIME && NULL != eisdir[dd]) {
		memset(eisdirdata[dd], 0, sizeof(EisDir));
		ret = dirclose(eisdir[dd]);
	}

	return ret;
}


/**********************************************************************************/
/*Description: Create a new OS directory                                          */
/*Input      : path name, directory name                                          */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 fail.                                             */
/**********************************************************************************/
int ipanel_porting_localdir_mkdir(const char *path, const char *dir)
{
	int ret = -1;
	char newdir[256];

	strcpy(newdir, path);
	if(newdir[strlen(path)-1]!='/') strcat(newdir, "/");
	strcat(newdir, dir);

	if (exist(path) && !exist(newdir)) {
		ret = mkdir(newdir);
	}

	return ret;
}
#endif
