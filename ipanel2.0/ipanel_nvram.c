/*********************************************************************
  Copyright (c) 2005 Embedded Internet Solutions, Inc
  All rights reserved. You are not allowed to copy or distribute
  the code without permission.
  This is the demo implenment of the NVRAM Porting APIs needed by iPanel MiddleWare. 
  Maybe you should modify it accorrding to Platform.

Note: the "int" in the file is 32bits

$ver0.0.0.1 $author Zouxianyun 2005/04/28
 *********************************************************************/

#include "ipanel_nvram.h"
#include "trd_ipanel_init.h"

#define NVRAM_IPANEL_FILE "/dev/mtdblock/7" 

#define NVRAM_MIN_SIZE (0x10000) /*64k*/

#ifdef USE_NVRAM
static unsigned char *NVRAM_Base = (unsigned char*)0x008000000; /*physical Flash Memory area address*/
static int NVRAM_Num_Sect = 1;
static int NVRAM_Size_Sect = FLASH_SECTOR_SIZE;

static struct WriteQueQue {
    unsigned int nvram_address;
    unsigned int data_address;
    unsigned int len;
}writeQue[6] = {0};

static struct StatuQueQue {
    unsigned int nvram_address;
    unsigned int len;
    int status;
}statusQue[6] = {0};


static void real_write_nvram_process(void);
#else
static unsigned char *NVRAM_Base = NULL;
static int NVRAM_Num_Sect = 0;
static int NVRAM_Size_Sect = 0;
#endif

/******************************************************************/
/*Description: Get Flash Memory Address information.              */
/*Input      : NVRAM Address Pointer, section number&Size Pointer */
/*Output     : NVRAM Address, number of sections, section size.   */
/*     sect size must be 32k/64k/128k/256k.                       */
/*Return     : 0 -- success, -1 -- fail.                          */
/******************************************************************/
/**the base NVRAM address which give to iPanel MiddleWare         */
int ipanel_porting_nvram_info(unsigned char **addr, int *numberofsections, int *sect_size)
{
    *addr = NVRAM_Base;
    *numberofsections = NVRAM_Num_Sect;
    *sect_size = NVRAM_Size_Sect;

    if ( (*addr == NULL) || (*numberofsections == 0) || (*sect_size == 0) || 
            ((*numberofsections) * (*sect_size) < NVRAM_MIN_SIZE) ||
            ((*sect_size != 0x8000) && (*sect_size != 0x10000) && (*sect_size != 0x20000) && (*sect_size != 0x40000)) )
    {
        *addr = NULL;
        *numberofsections = 0;
        *sect_size =0;
        return -1;
    }

    return 0;
}

/**********************************************************************/
/*Description: Read Data From Flash Memory.                           */
/*Input      : NVRAM Address, Buffer, and numberofbytes need to read. */
/*Output     : Save Data to Buffer                                    */
/*Return     : Read real nbytes, -1 -- fail.                          */
/**********************************************************************/
int ipanel_porting_nvram_read(unsigned int address, unsigned char *buf,  int nbytes)
{
#ifdef USE_NVRAM
    if (address < (unsigned int)NVRAM_Base || address > (unsigned int)NVRAM_Base + NVRAM_Num_Sect*NVRAM_Size_Sect - nbytes) {
        memset((unsigned char*)address, 0, nbytes);
        return -1;
    }
    memcpy (buf, (unsigned char *)address, nbytes);
    return nbytes;
#else
    return 0;
#endif
}

/******************************************************************/
/*Description: Write Flash Memory, and no block.                  */
/*Input      : NVRAM Address, data, length need to write to nvram */
/*Output     : No                                                 */
/*Return     : Write real nbytes. and  -1 -- fail.                */
/******************************************************************/
int ipanel_porting_nvram_burn(unsigned int address, const char *to, int len)
{
#ifdef USE_NVRAM
    int i;
    if (address < (unsigned int)NVRAM_Base || address > (unsigned int)NVRAM_Base + NVRAM_Num_Sect*NVRAM_Size_Sect - len) {
        return -1;
    }
    /*post message [address,to,len] to queque*/
    for(i = 0; i < 6; i++) {
        if (writeQue[i].nvram_address == 0) {
            writeQue[i].nvram_address = address;
            writeQue[i].data_address = (unsigned int)to;
            writeQue[i].len = len;

            statusQue[i].nvram_address = address;
            statusQue[i].len = len;
            statusQue[i].status = 0;
            return len;
        }
    }

    return -1;
#else
    return 0;
#endif
}

/******************************************************************/
/*Description: Check burn is finished.                            */
/*Input      : NVRAM address, len need to write                   */
/*Output     : No                                                 */
/*Return     : 0 -- writing, 1 -- success and complete, -1 - fail.*/
/******************************************************************/
int ipanel_porting_nvram_status(unsigned int address, int len)
{
#ifdef USE_NVRAM
    int i;

    for(i = 0; i < 6; i++) {
        if (statusQue[i].nvram_address == address && statusQue[i].len == len) 
            return statusQue[i].status;
    }

    return -1;    
#else
    return 0;
#endif
}

#ifdef USE_NVRAM
/*There maybe in another task/ process/ pthread .*/
static void real_write_nvram_process(void)
{
    int s32FlashFd = -1;
    unsigned int u32offset = address - NVRAM_Base;
    int s32Ret;
    int i;
    /*poll message [address,to,len] to queque*/
    for(i = 0; i < 6; i++) {
        if (writeQue[i].nvram_address != 0) {
            /*write data to nvram flash*/
            /*and set status to statusQue[i].status*/

            u32offset = writeQue[i].nvram_address - NVRAM_Base;
            
            s32FlashFd= open(NVRAM_IPANEL_FILE, O_RDWR | O_SYNC);

            if(-1== s32FlashFd)
            {
                printf("ipanel nvram open failed\n");
                return -1;
            }

            if( -1 == lseek(s32FlashFd, u32offset, SEEK_SET))
            {
                printf("ipanel nvram  lseek failed\n");
                return -1;
            }


            s32Ret = write(s32FlashFd, writeQue[i].data_address, writeQue[i].len );

            /* if write success */
            if ( writeQue[i].len == s32Ret )
            {
                statusQue[i].status = 1;
                writeQue[i].nvram_address = 0;
            }  
            else
            {
                printf("ipanel nvram write failed\n");                
                statusQue[i].status = -1;
            }

            close(s32FlashFd);

        }
    }
}
#endif


