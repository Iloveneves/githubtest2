/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the NVRAM Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_NVRAM_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_NVRAM_API_FUNCTOTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*NVRAM information -- address and size, usually is FLASH Memory*/
int ipanel_porting_nvram_info(unsigned char **addr, int *numberofsection, int *sect_size);

/*Read NVRAM flash data, the function is belong to NVRAM part.*/
int ipanel_porting_nvram_read(unsigned int address, unsigned char *buf,  int nbytes);

/*Write data to NVRAM flash , the function is belong to NVRAM part.*/
int ipanel_porting_nvram_burn(unsigned int address, const char *to, int len);

/*return the status of the NVRAM block.*/
int ipanel_porting_nvram_status(unsigned int address, int len);

#ifdef __cplusplus
}
#endif

#endif
