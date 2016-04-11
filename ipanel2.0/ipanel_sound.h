/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the SOUND Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_SOUND_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_SOUND_API_FUNCTOTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*initialize sound. you maybe create a new pthread /task to play data.*/
void ipanel_porting_sound_init(void);

/*exit from sound, free all resource in function ipanel_porting_sound_init().*/
void ipanel_porting_sound_exit(void);

/*get STB sound channel parameters*/
int ipanel_porting_sound_get_info(int *numberofbuf, int *buf_size);

/*open sound device, maybe you should stop the running channel.*/
unsigned int ipanel_porting_sound_setup(int format, int samplerate, int bitsperSample, int channels, int optionflag);

/*play sound data*/
int ipanel_porting_sound_play(unsigned int handle, const char *buffer, int len);

/*poll playing buffer is complete.*/
int ipanel_porting_sound_poll(unsigned int event[3]);

/*close sound device*/
int ipanel_porting_sound_close(unsigned int handle);

#ifdef __cplusplus
}
#endif

#endif
