/*********************************************************************
    Copyright (c) 2005 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    There are the Url and Network and Cursor Porting APIs needed by iPanel MiddleWare. 
    
    Note: the "int" in the file is 32bits
    
    $ver0.0.0.1 $author Zouxianyun 2005/04/28
*********************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_URL_NET_CURSOR_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_URL_NET_CURSOR_API_FUNCTOTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*get start page*/
const char *ipanel_porting_get_startpage(void);

/*set start page*/
int ipanel_porting_set_startpage(const char *url);

/*get home page*/
const char *ipanel_porting_get_homepage(void);

/*set home page*/
int ipanel_porting_set_homepage(const char *url);

/*get help page*/
const char *ipanel_porting_get_helppage(void);

/*set help page*/
int ipanel_porting_set_helppage(const char *url);

/*connect network*/
int ipanel_porting_network_connect(const char *devname);

/*disconnect network*/
int ipanel_porting_network_disconnect(const char *devname);

/*get network parameter*/
int ipanel_porting_network_get_parameter(const char *devname, const char *name, char *buf, int len);

/*set network parameter*/
int ipanel_porting_network_set_parameter(const char *devname, const char *name, const char *val);

/*get mouse or cursor position*/
void ipanel_porting_cursor_get_position(int *x, int *y);

/*set mouse or cursor position*/
void ipanel_porting_cursor_set_position(int x, int y);

/*get cursor shape*/
int ipanel_porting_cursor_get_shape(void);

/*set cursor shape*/
int ipanel_porting_cursor_set_shape(int shape);

/*int ipanel_porting_cursor_show(int showflag);*/

#ifdef __cplusplus
}
#endif

#endif
