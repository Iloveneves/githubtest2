/*
author     :szhy
create date:2008-12-03
*/

/*
** 2004 May 22
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains code that is specific to windows.
*/
//#include "malloc.h"
#include "string.h"
#include "sql/sqlite3.h"
#include "mfs/mfs_api.h"

void MFS_Sleep(unsigned long dwMilliseconds)
{

}

int MFS_CloseHandle(void * hObject)
{
	MFS_HANDLE myHandle = (MFS_HANDLE)hObject;
	return MFS_API_Close(myHandle);
}

int MFS_ReadFile(void * hFile,void * lpBuffer,unsigned long nNumberOfBytesToRead, unsigned long * lpNumberOfBytesRead)
{
	int ret;
	MFS_HANDLE myHandle = (MFS_HANDLE)hFile;
	ret = MFS_API_Read(myHandle,lpBuffer,nNumberOfBytesToRead);
	*lpNumberOfBytesRead = ret;
	return 1;
}

unsigned long MFS_GetLastError(void)
{
	return 1;
}

unsigned long MFS_SetFilePointer(void * hFile,long lDistanceToMove,long * lpDistanceToMoveHigh,unsigned long dwMoveMethod)
{
	MFS_HANDLE myHandle;
	switch (dwMoveMethod)
	{
		case MFS_SEEK_SET:
			myHandle = (MFS_HANDLE)hFile;
			MFS_API_Seek(myHandle,lDistanceToMove,MFS_SEEK_SET);
	}
	*lpDistanceToMoveHigh = (lDistanceToMove & 0xffff0000)>>16;
	return lDistanceToMove & 0x0000ffff;
}

int MFS_WriteFile(void* hFile,const void * lpBuffer,unsigned long nNumberOfBytesToWrite,unsigned long * lpNumberOfBytesWritten)
{
	int ret;
	MFS_HANDLE myHandle = (MFS_HANDLE)hFile;
	ret = MFS_API_Write(myHandle,lpBuffer,nNumberOfBytesToWrite);
	*lpNumberOfBytesWritten = ret;
	return 1;
}

unsigned long MFS_GetFileSize(void * hFile,unsigned long * lpFileSizeHigh)
{
	MFS_HANDLE myHandle = (MFS_HANDLE)hFile;
	return MFS_API_GetSize(myHandle);
}

unsigned long MFS_GetFileAttributesA(const char * lpFileName)
{
	return 0xffffffff;
}

int MFS_DeleteFileA(const char * lpFileName)
{
	return MFS_API_Remove(lpFileName);
}
void* MFS_CreateFileA(const char * lpFileName,unsigned long dwShareMode,void* hTemplateFile)
{
	return (void *)MFS_API_Open(lpFileName,"w+");
}

/*
** The winFile structure is a subclass of sqlite3_file* specific to the win32
** portability layer.
*/
typedef struct winFile winFile;
struct winFile
{
	const sqlite3_io_methods *pMethod;/* Must be first */
	void* h;               /* Handle for accessing the file */
	unsigned char locktype; /* Type of lock currently held on this file */
	short sharedLockByte;   /* Randomly chosen byte used as a shared lock */
};

/*****************************************************************************
** The next group of routines implement the I/O methods specified
** by the sqlite3_io_methods object.
******************************************************************************/

/*
** Close a file.
**
** It is reported that an attempt to close a handle might sometimes
** fail.  This is a very unreasonable result, but windows is notorious
** for being unreasonable so I do not doubt that it might happen.  If
** the close fails, we pause for 100 milliseconds and try again.  As
** many as MX_CLOSE_ATTEMPT attempts to close the handle are made before
** giving up and returning an error.
*/
#define MX_CLOSE_ATTEMPT 3
static int winClose(sqlite3_file *id)
{
	int rc, cnt = 0;
	winFile *pFile = (winFile*)id;
	do
	{
		rc = MFS_CloseHandle(pFile->h);
	}
	while ( rc==0 && cnt++ < MX_CLOSE_ATTEMPT && (MFS_Sleep(100), 1));
	return rc ? SQLITE_OK : SQLITE_IOERR;
}

/*
** Read data from a file into a buffer.  Return SQLITE_OK if all
** bytes were read successfully and SQLITE_IOERR if anything goes
** wrong.
*/
static int winRead(
    sqlite3_file *id,          /* File to read from */
    void *pBuf,                /* Write content into this buffer */
    int amt,                   /* Number of bytes to read */
    sqlite3_int64 offset       /* Begin reading at this offset */
)
{
	long upperBits;
	long lowerBits = (long)offset;
	unsigned long rc;
	unsigned long got;
	winFile *pFile = (winFile*)id;
	rc = MFS_SetFilePointer(pFile->h, lowerBits, &upperBits, MFS_SEEK_SET);
	if ( rc==((unsigned long)-1) && MFS_GetLastError()!=0 )
	{
		return SQLITE_FULL;
	}
	if ( !MFS_ReadFile(pFile->h, pBuf, amt, &got) )
	{
		return SQLITE_IOERR_READ;
	}
	if ( got==(unsigned long)amt )
	{
		return SQLITE_OK;
	}
	else
	{
		memset(&((char*)pBuf)[got], 0, amt-got);
		return SQLITE_IOERR_SHORT_READ;
	}
}

/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
static int winWrite(
    sqlite3_file *id,         /* File to write into */
    const void *pBuf,         /* The bytes to be written */
    int amt,                  /* Number of bytes to write */
    sqlite3_int64 offset      /* Offset into the file to begin writing at */
)
{
	long upperBits;
	long lowerBits = (long)offset;
	unsigned long rc;
	unsigned long wrote;
	winFile *pFile = (winFile*)id;
	rc = MFS_SetFilePointer(pFile->h, lowerBits, &upperBits, MFS_SEEK_SET);
	if ( rc==((unsigned long)-1) && MFS_GetLastError()!=0 )
	{
		return SQLITE_FULL;
	}
	while (amt>0  && (rc = MFS_WriteFile(pFile->h, pBuf, amt, &wrote))!=0 && wrote>0)
	{
		amt -= wrote;
		pBuf = &((char*)pBuf)[wrote];
	}
	if ( !rc || amt>(int)wrote )
	{
		return SQLITE_FULL;
	}
	return SQLITE_OK;
}

/*
** Truncate an open file to a specified size
*/
static int winTruncate(sqlite3_file *id, sqlite3_int64 nByte)
{
	return SQLITE_OK;
}

/*
** Make sure all writes to a particular file are committed to disk.
*/
static int winSync(sqlite3_file *id, int flags)
{
	return SQLITE_OK;
}

/*
** Determine the current size of a file in bytes
*/
static int winFileSize(sqlite3_file *id, sqlite3_int64 *pSize)
{
	winFile *pFile = (winFile*)id;
	unsigned long upperBits;
	*pSize = MFS_GetFileSize(pFile->h, &upperBits);
	return SQLITE_OK;
}

/*
** Acquire a reader lock.
** Different API routines are called depending on whether or not this
** is Win95 or WinNT.
*/
static int getReadLock(winFile *pFile)
{
	return 1;
}

/*
** Undo a readlock
*/
static int unlockReadLock(winFile *pFile)
{
	return 1;
}

/*
** Lock the file with the lock specified by parameter locktype - one
** of the following:
**
**     (1) SHARED_LOCK
**     (2) RESERVED_LOCK
**     (3) PENDING_LOCK
**     (4) EXCLUSIVE_LOCK
**
** Sometimes when requesting one lock state, additional lock states
** are inserted in between.  The locking might fail on one of the later
** transitions leaving the lock state different from what it started but
** still short of its goal.  The following chart shows the allowed
** transitions and the inserted intermediate states:
**
**    UNLOCKED -> SHARED
**    SHARED -> RESERVED
**    SHARED -> (PENDING) -> EXCLUSIVE
**    RESERVED -> (PENDING) -> EXCLUSIVE
**    PENDING -> EXCLUSIVE
**
** This routine will only increase a lock.  The winUnlock() routine
** erases all locks at once and returns us immediately to locking level 0.
** It is not possible to lower the locking level one step at a time.  You
** must go straight to locking level 0.
*/
static int winLock(sqlite3_file *id, int locktype)
{
	return 0;
}

/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
static int winCheckReservedLock(sqlite3_file *id)
{
	return 1;
}

/*
** Lower the locking level on file descriptor id to locktype.  locktype
** must be either NO_LOCK or SHARED_LOCK.
**
** If the locking level of the file descriptor is already at or below
** the requested locking level, this routine is a no-op.
**
** It is not possible for this routine to fail if the second argument
** is NO_LOCK.  If the second argument is SHARED_LOCK then this routine
** might return SQLITE_IOERR;
*/
static int winUnlock(sqlite3_file *id, int locktype)
{
	return 0;
}

/*
** Control and query of the open file handle.
*/
static int winFileControl(sqlite3_file *id, int op, void *pArg)
{
	return SQLITE_OK;
}

/*
** Return the sector size in bytes of the underlying block device for
** the specified file. This is almost always 512 bytes, but may be
** larger for some devices.
**
** SQLite code assumes this function cannot fail. It also assumes that
** if two files are created in the same file-system directory (i.e.
** a database and its journal file) that the sector size will be the
** same for both.
*/
static int winSectorSize(sqlite3_file *id)
{
	return 512;
}

/*
** Return a vector of device characteristics.
*/
static int winDeviceCharacteristics(sqlite3_file *id)
{
	return 0;
}

/*
** This vector defines all the methods that can operate on an
** sqlite3_file for win32.
*/
static const sqlite3_io_methods winIoMethod =
{
	1,                        /* iVersion */
	winClose,
	winRead,
	winWrite,
	winTruncate,
	winSync,
	winFileSize,
	winLock,
	winUnlock,
	winCheckReservedLock,
	winFileControl,
	winSectorSize,
	winDeviceCharacteristics
};

/***************************************************************************
** Here ends the I/O methods that form the sqlite3_io_methods object.
**
** The next block of code implements the VFS methods.
****************************************************************************/
/*
** Open a file.
*/
static int winOpen(
    sqlite3_vfs *pVfs,        /* Not used */
    const char *zName,        /* Name of the file (UTF-8) */
    sqlite3_file *id,         /* Write the SQLite file handle here */
    int flags,                /* Open mode flags */
    int *pOutFlags            /* Status return flags */
)
{
	void* h;
	unsigned long dwShareMode;
	int len;
	void *zConverted;
	winFile *pFile = (winFile*)id;
	len = strlen(zName);
	zConverted = (void *)FYF_CPI_malloc_porting(len+1);
	strcpy(zConverted,zName);
	if ( zConverted==0 )
	{
		return SQLITE_NOMEM;
	}
	dwShareMode = 0;
	h = MFS_CreateFileA((char*)zConverted,
	                    dwShareMode,
	                    (void *)0
	                   );
	if ( h==(void *)-1 )
	{
		FYF_CPI_free_porting(zConverted);
		if ( flags & SQLITE_OPEN_READWRITE )
		{
			return winOpen(0, zName, id, ((flags|SQLITE_OPEN_READONLY)&~SQLITE_OPEN_READWRITE), pOutFlags);
		}
		else
		{
			return SQLITE_CANTOPEN;
		}
	}
	if ( pOutFlags )
	{
		if ( flags & SQLITE_OPEN_READWRITE )
		{
			*pOutFlags = SQLITE_OPEN_READWRITE;
		}
		else
		{
			*pOutFlags = SQLITE_OPEN_READONLY;
		}
	}
	memset(pFile, 0, sizeof(*pFile));
	pFile->pMethod = &winIoMethod;
	pFile->h = h;
	{
		FYF_CPI_free_porting(zConverted);
	}
//  OpenCounter(+1);
	return SQLITE_OK;
}

/*
** Delete the named file.
**
** Note that windows does not allow a file to be deleted if some other
** process has it open.  Sometimes a virus scanner or indexing program
** will open a journal file shortly after it is created in order to do
** whatever does.  While this other process is holding the
** file open, we will be unable to delete it.  To work around this
** problem, we delay 100 milliseconds and try to delete again.  Up
** to MX_DELETION_ATTEMPTs deletion attempts are run before giving
** up and returning an error.
*/
#define MX_DELETION_ATTEMPTS 5
static int winDelete(
    sqlite3_vfs *pVfs,          /* Not used on win32 */
    const char *zFilename,      /* Name of file to delete */
    int syncDir                 /* Not used on win32 */
)
{
	int cnt = 0;
	unsigned long rc;
	int len;
	void *zConverted;

	len = strlen(zFilename);
	zConverted = (void *)FYF_CPI_malloc_porting(len+1);
	strcpy(zConverted,zFilename);
	if ( zConverted==0 )
	{
		return SQLITE_NOMEM;
	}
	do
	{
		MFS_DeleteFileA(zConverted);
	}
	while ( (rc = MFS_GetFileAttributesA(zConverted))!=0xffffffff && cnt++ < MX_DELETION_ATTEMPTS && (MFS_Sleep(100), 1));
	FYF_CPI_free_porting(zConverted);
	return rc==0xffffffff ? SQLITE_OK : SQLITE_IOERR_DELETE;
}

/*
** Check the existance and status of a file.
*/
static int winAccess(
    sqlite3_vfs *pVfs,         /* Not used on win32 */
    const char *zFilename,     /* Name of file to check */
    int flags                  /* Type of test to make on this file */
)
{
	return 0;
}

/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at pVfs->mxPathname characters.
*/
static int winGetTempname(sqlite3_vfs *pVfs, int nBuf, char *zBuf)
{
	return SQLITE_OK;
}

/*
** Turn a relative pathname into a full pathname.  Write the full
** pathname into zOut[].  zOut[] will be at least pVfs->mxPathname
** bytes in size.
*/
static int winFullPathname(
    sqlite3_vfs *pVfs,            /* Pointer to vfs object */
    const char *zRelative,        /* Possibly relative input path */
    int nFull,                    /* Size of output buffer in bytes */
    char *zFull                   /* Output buffer */
)
{
	strcpy(zFull , zRelative);
	return SQLITE_OK;
}

/*
** Write up to nBuf bytes of randomness into zBuf.
*/
static int winRandomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf)
{
	int n = 0;
	char tmp[5];
	tmp[0]=tmp[1]=tmp[2]=tmp[3]=48;
	tmp[4]=0;
	memcpy(&zBuf[n],tmp,5);
	n = 5;
	return n;
}

/*
** Sleep for a little while.  Return the amount of time slept.
*/
static int winSleep(sqlite3_vfs *pVfs, int microsec)
{
	MFS_Sleep((microsec+999)/1000);
	return ((microsec+999)/1000)*1000;
}

/*
** Return a pointer to the sqlite3DefaultVfs structure.   We use
** a function rather than give the structure global scope because
** some compilers (MSVC) do not allow forward declarations of
** initialized structures.
*/
sqlite3_vfs winVfs =
{
	1,                  /* iVersion */
	sizeof(winFile),    /* szOsFile */
	260,           /* mxPathname */
	0,                  /* pNext */
	"win32",            /* zName */
	0,                 	/* pAppData */
	winOpen,           	/* xOpen */
	winDelete,         	/* xDelete */
	winAccess,         	/* xAccess */
	winGetTempname,    	/* xGetTempName */
	winFullPathname,   	/* xFullPathname */
	0,         			/* xDlOpen */
	0,        			/* xDlError */
	0,          		/* xDlSym */
	0,        			/* xDlClose */
	winRandomness,      /* xRandomness */
	winSleep,           /* xSleep */
	0     			    /* xCurrentTime */
};



void FYF_API_fls_test(void)
{
	signed long flashHandle;
	static signed long ret;
	char buf[6]={'0','1','2','3','4','5'};
	char buf1[6];
	flashHandle = FYF_API_fls_open("flash.db",1024*64);

	ret = FYF_API_fls_write(buf,6,1,flashHandle);
	ret = FYF_API_fls_read(buf1,6,1,flashHandle);
}
#if 1
int calltimes = 0;
static int sqlcallback(void* notused,int argc,char** argv,char** azColName)
{
	int i;
	calltimes++;
	for (i=0;i<argc;i++)
	{
		printf("%s=%s\n",azColName[i],argv[i]);
	}
	printf("number %d callback\n",calltimes);
	return 0;
}
char **selectResult = 0;//存储查询到的信息
int nResult = 0;		//行数
int mResult = 0;		//列数

void FYF_API_sql_test(void)
{
	sqlite3 * db = NULL;
	int rc;
	char * szErrMsg = NULL;
	int i;
	char statement[50];

	rc = sqlite3_open("flash.db",&db);
	rc = sqlite3_exec(db,"create table employee(id integer primary key,age integer,name varchar(10));",NULL,0,&szErrMsg);
	for (i=0;i<200;i++)
	{
		sprintf(statement,"insert into employee values(%d,%d,'%s');",i,i,"fl");
		rc = sqlite3_exec(db,statement,NULL,0,0);
	}
	rc=sqlite3_exec(db,"select * from employee;",sqlcallback,0,&szErrMsg);
//	rc = sqlite3_get_table(db,"select * from employee;",&selectResult,&nResult,&mResult,&szErrMsg);

	sqlite3_close(db);
}
#endif
/******************************************************************************/
/*Description: init sql module                                                */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     :                                                                */
/******************************************************************************/
void FYF_API_sql_init(void)
{
	sqlite3_vfs_register(&winVfs,0);

	//FYF_API_sql_test();
	//FYF_API_fls_test();
}


