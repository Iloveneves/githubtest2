/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/10/20                                           	  */
/******************************************************************************/
#ifndef __FYF_PANEL_H_
#define __FYF_PANEL_H_
typedef struct
{ 
	BU32 (*FYF_CPI_panel_read_key)(void);
	BU32 (*FYF_CPI_panel_show_led)(BU08 StartLedIndex,PU08 DisData,BU08 Length);        
}FYF_PANEL_PORTING; 
/******************************************************************************/
/*Description: register panel                                                 */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_panel_register(FYF_PANEL_PORTING *porting);
/******************************************************************************/
/*Description: read key		                                                  */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_panel_read_key(void);
/******************************************************************************/
/*Description: show led		                                                  */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_panel_show_led(BU08 StartLedIndex,PU08 DisData,BU08 Length);
#endif



