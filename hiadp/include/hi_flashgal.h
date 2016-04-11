#ifndef _HI_FLASHGAL_H
#define _HI_FLASHGAL_H
#include "hi_type.h"
typedef struct hi_flash
{
	HI_S32  	fd;
	HI_S32	mtd_id;
}HI_FLASH_S;

HI_S32 hi_flash_erase(HI_FLASH_S *dev,  HI_U32 startblockno,  HI_U32 blockcount);
 HI_S32 hi_flash_read(HI_FLASH_S *dev, HI_U8 *data, HI_S32 len);
 HI_S32 hi_flash_close(HI_FLASH_S * dev);
 HI_S32 hi_flash_open(HI_FLASH_S * dev);
 HI_S32 hi_flash_write(HI_FLASH_S * dev, HI_U8 * data, HI_S32 len);
 HI_S32 hi_flash_lseek(HI_FLASH_S *dev, HI_S32 offset, HI_S32 origin);
 HI_S32 hi_flash_get_size(HI_FLASH_S* dev);
 HI_S32 hi_flash_erase_all(HI_FLASH_S * dev);

#endif