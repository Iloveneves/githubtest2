#ifndef _HIOS_NTFS_H_
#define _HIOS_NTFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/statvfs.h>
#include <pthread.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "volume.h"
#include "list.h"
#include "types.h"
#include "inode.h"
#include "unistr.h"
#include "runlist.h"
#include "debug.h"
#include "logging.h"
#include "dir.h"
#include "ntfstime.h"
#include "dirstream.h"

#define NTFS_DBG 0
#define NTFS_MAX_FILE 1024
#define NTFS_FD_BASE 50000
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

typedef enum {
	ATIME_ENABLED,
	ATIME_DISABLED,
	ATIME_RELATIVE
} hios_ntfs_atime_t;

typedef struct {
	ntfs_volume *vol;
	ntfs_inode *cached_ni;
	hios_ntfs_atime_t atime;
	u32 device_count;
	char *mnt_point;
	char cached_path[MAX_PATH];
} hios_ntfs_context_t;

typedef struct {
    	ntfs_attr *na;
    	ntfs_inode *ni;
    	char *path;
    	off_t f_pos;
    	int flags;
    	int count;
}hios_ntfs_file_t;

typedef struct {
    struct list_head list;
	ntfs_volume *vol;
	void *date;
} hios_ntfs_dirent_t;

typedef struct {
	struct list_head list;
	char name[MAX_PATH];
	ntfs_inode *ni;
	void *date;
}hios_ntfs_dir_t;

typedef struct {
    	struct list_head list;
    	struct dirent *dirent_buf;
    	void *date;
}hios_ntfs_direnty_t;

typedef struct {
    	int fd;
    	FILE *fp;
}hios_FILE;


int hios_mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data);
int hios_umount(const char *target);
int hios_open(const char *pathname, int flags, mode_t mode);
int hios_close(int fd);
int hios_mkdir(const char *pathname, mode_t mode);
int hios_remove(const char *pathname);
off_t hios_lseek(int fildes, off_t offset, int whence);
int hios_fstat(int fildes, struct stat *buf);
ssize_t hios_read(int fd, void *buf, size_t count);
ssize_t hios_pread(int fd, void *buf, size_t count, off_t offset);
ssize_t hios_write(int fd, const void *buf, size_t count);
ssize_t hios_pwrite(int fd, const void *buf, size_t count, off_t offset);
int hios_statvfs(const char *org_path, struct statvfs *buf);
DIR *hios_opendir(const char *org_path);
struct dirent *hios_readdir(DIR *dirp);
int hios_closedir(DIR *dirp);
int hios_fsync(int fd);

hios_FILE *hios_fopen(const char *org_path, const char *mode);
size_t hios_fread(void *ptr, size_t size, size_t nmemb, hios_FILE *stream);
size_t hios_fwrite(const void *ptr, size_t size, size_t nmemb, hios_FILE *stream);
int hios_fclose(hios_FILE *fp);
int hios_fseek(hios_FILE *stream, long offset, int whence);
long hios_ftell(hios_FILE *stream);
int hios_fflush(hios_FILE *stream);


#endif
