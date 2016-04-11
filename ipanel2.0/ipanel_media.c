/*********************************************************************
  Copyright (c) 2005 Embedded Internet Solutions, Inc
  All rights reserved. You are not allowed to copy or distribute
  the code without permission.
  This is the demo implenment by iPanel MiddleWare. 
  Maybe you should modify it accorrding to Platform.        

  $ver0.0.0.1 $author Huangzhiguo 2005/10/18
 *********************************************************************/
#include "trd_ipanel_init.h"
#include "ipanel_base.h"
#include "ipanel_media.h"
#include "ipanel_audio.h"
#include "ipanel_porting_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 0:success, others: fail */

/**********************************************************************************/
/*Description: Open a media URL                                                   */
/*Input      : media url string, media type such as "MPEG" "mp3" NULL etc         */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_media_open(const char *url, const char *mediatype)
{
	int ret = -1;
	
	printf("ipanel_porting_media_open\n");
	
	if (NULL != url && strlen(url)>0) {
		/*open a physical media url here.*/
		ret = 0;
	}

	return ret;}


/**********************************************************************************/
/*Description: Close the opened media                                             */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_media_close(void)
{
	printf("ipanel_porting_media_close\n");

	/*close the physical media url here.*/
	return 0;
}


/**********************************************************************************/
/*Description: get media track length for time                                    */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : length, uints is seconds                                           */
/**********************************************************************************/
int ipanel_porting_media_duration(void)
{
	printf("ipanel_porting_media_duration\n");
	/*media track length is 1minute 30 seconds*/
	return 90;
}


/**********************************************************************************/
/*Description: get media status                                                   */
/*Input      : save speed buffer                                                  */
/*Output     : speed (optioned)                                                   */
/*Return     : media status                                                       */
/**********************************************************************************/
const char *ipanel_porting_media_status(int *speed)
{
    IPANEL_DEBUG("ipanel_porting_media_status\n");
    /*if running*/
    *speed = 0;   /* no use*/
    return "running";

    /*if slow
     *speed = 2; //will be used
     return "slow";

     if forward
     *speed = 5; //will be used
     return "forward";
     */
}


/**********************************************************************************/
/*Description: Reset the opened media                                             */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_reset_media_setting(void)
{
    IPANEL_DEBUG("ipanel_porting_reset_media_setting\n");
    /*reset physical media here*/
    return 0;
}


/**********************************************************************************/
/*Description: Set TV screen area to play. (0, 0, 0, 0) is full srceen            */
/*Input      : TV screen area x pixls, y pixls, w width, and h height.            */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_video_location(int x, int y, int w, int h)
{
	FYF_Rect_s fyfRect;
    IPANEL_DEBUG("ipanel_porting_video_location\n");
    IPANEL_DEBUG("x %d, y %d, w %d, h %d", x,y,w,h);
    //Hi_AV_SetVOOutputWindow(x,y,w,h);
    fyfRect.h = h;
	fyfRect.w = w;
	fyfRect.x = x + 35;
	fyfRect.y = y + 25;
	
    FYF_API_vout_ioctl(NULL,FYF_VOUT_SET_WIN_LOCATION, (void *)&fyfRect);
    return 0;
}


/**********************************************************************************/
/*Description: play the opened media                                              */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_play(void)
{
    IPANEL_DEBUG("ipanel_porting_program_play\n");
    /*play the physical opened media*/
    return 0;
}


/**********************************************************************************/
/*Description: pause the opened media                                             */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_pause(void)
{
    IPANEL_DEBUG("ipanel_porting_program_pause\n");
    /*pause the physical opened media*/
    return 0;
}


/**********************************************************************************/
/*Description: stop the opened media                                              */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_stop(void)
{
	FYF_VDEC_STOP_MODE_e fyfStopMode = FYF_VDEC_LAST_FRAME;
    IPANEL_DEBUG("ipanel_porting_program_stop\n");
    /*stop the physical opened media*/
	
    //Hi_AV_PlayStop();
	//FYF_API_vdec_ioctl(FYF_API_vdec_open(), FYF_VDEC_STOP, (void *)&fyfStopMode);
	//FYF_API_adec_ioctl(FYF_API_vdec_open(), FYF_ADEC_STOP, (void *)&fyfStopMode);
	//APP_IPI_StopAudioVideo();
	APP_Com_IPI_StopProg(1);
    return 0;
}


/**********************************************************************************/
/*Description: resume the opened media                                            */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_resume(void)
{
    IPANEL_DEBUG("ipanel_porting_program_resume\n");
    /*resume the physical opened media*/
    return 0;
}


/**********************************************************************************/
/*Description: play the opened media fast                                         */
/*Input      : multi speed                                                        */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_forward(int number)
{
    IPANEL_DEBUG("ipanel_porting_program_forward\n");
    int ret = -1;

    if (number == 2 || number == 5) {
        /*play_speed *= number;*/
        /*play the physical opened media new speed here.*/
        ret = 0;
    }

    return ret;
}


/**********************************************************************************/
/*Description: play the opened media slow                                         */
/*Input      : division speed                                                     */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_slow(int number)
{
    IPANEL_DEBUG("ipanel_porting_program_slow\n");
    int ret = -1;

    if (number == 2 || number == 5) {
        /*play_speed /= number;*/
        /*play the physical opened media new speed here.*/
        ret = 0;
    }

    return ret;
}


/**********************************************************************************/
/*Description: repeat the opened media times                                      */
/*Input      : repeat times, -1 - no limits                                       */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_repeat(int number)
{
    IPANEL_DEBUG("ipanel_porting_program_repeat\n");
    int ret = -1, times;

    if (number > 0) {
        for (times = 0; times < number; times++) {
            /*play the physical opened media here.*/
            ;
        }
        ret = 0;
    }
    else if (number == -1) {
        while(1) {
            /*play the physical opened media here.*/
            ;
        }
        ret = 0;
    }

    return ret;
}



/**********************************************************************************/
/*Description: rewind the opened media                                            */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_rewind(void)
{
    IPANEL_DEBUG("ipanel_porting_program_rewind\n");
    /*rewind the physical opened media*/
    return 0;
}


/**********************************************************************************/
/*Description: seek a position of the opened media                                */
/*Input      : mode (0 - time mode, 1 - chapter mode, other fail.)                */
/*             value is (0 ~86399) in time mode, and >0 in chapter mode.          */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_seek(int mode, int value)
{
    IPANEL_DEBUG("ipanel_porting_program_seek\n");
    int ret = -1;

    if (value > 0) {
        if (1 == mode) {
            /*seek a position according chapter here.*/
            ret = 0;
        }
        else if (0==mode && value<86399) {
            /*seek a position according time here.*/
            ret = 0;
        }
    }

    return ret;
}



/**********************************************************************************/
/*Description: break the opened media                                             */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_break(void)
{
    IPANEL_DEBUG("ipanel_porting_program_break\n");
    /*break the physical opened media*/
    return 0;
}


/**********************************************************************************/
/*Description: reverse the opened media                                           */
/*Input      : reverse speed                                                      */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_program_reverse(int number)
{
    IPANEL_DEBUG("ipanel_porting_program_reverse\n");
    /*reverse the physical media here.*/
    return 0;
}


/**********************************************************************************/
/*Description: set the media mode, audio or video mode                            */
/*Input      : "av" or "tv"                                                       */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_video_tvav(const char *mode)
{
    IPANEL_DEBUG("ipanel_porting_video_tvav\n");
    int ret = -1;

    //if ( strcmp(mode, "av")==0 || strcmp(mode, "tv")==0 ) {
    //    ret = 0;
    //}
    return ret;
}


/**********************************************************************************/
/*Description: set parameter value,                                               */
/*Input      : parameter name and parameter value.                                */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail                                           */
/**********************************************************************************/
int ipanel_porting_vod_set_param(const char *name, const char *value)
{
    IPANEL_DEBUG("ipanel_porting_vod_set_param\n");
    /*set parameter value here*/
    return 0;
}


/**********************************************************************************/
/*Description: get parameter value,                                               */
/*Input      : parameter name                                                     */
/*Output     : No                                                                 */
/*Return     : parameter value                                                    */
/**********************************************************************************/
const char *ipanel_porting_vod_get_param(const char*name)
{
    IPANEL_DEBUG("ipanel_porting_vod_get_param\n");
    char *val = NULL;
    /*if (strcmp(name, "media") == 0)
      val = "Media";  //etc
     */
    return val;
}


#ifdef __cplusplus
}
#endif

