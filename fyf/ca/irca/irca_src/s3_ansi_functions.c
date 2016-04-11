/**
 * $Id: s3_ansi_functions.c,v 1.7 2005/02/23 10:59:52 dwinkel Exp $
 *
 * Copyright © 2003-2005 Irdeto Access B.V., All rights reserved.
 *
 * This file is proivided as part of the SoftCell 3 Integration library.
 * 
 * This file, together with s3_ansi_functions.h, provides an abstraction layer between SoftCell 
 * and the ANSI-C standard library.
 * 
 * This file is provided as an example and may need to be altered to match the target platform.
 * Functions may need to meet additional requirements above those described in the ANSI C standard.
 * 
 * Most functions map directly to the ANSI defined library function.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_ansi_functions.h>
#include <osal/osal_task.h>


void* ansi_memcpy( void *pvDest, const void *pvSrc, unsigned int uiCount )
{
	return memcpy( pvDest, pvSrc, uiCount );
}

void* ansi_memmove( void *pvDest, const void *pvSrc, unsigned int uiCount )
{
	return memmove( pvDest, pvSrc, uiCount );
}

int ansi_memcmp( const void *pvData1, const void *pvData2, unsigned int uiLength )
{
	return memcmp( pvData1, pvData2, uiLength );
}

void* ansi_memset( void *pvData, int iData, unsigned int uiLength )
{
	return memset( pvData, iData, uiLength );
}

unsigned int ansi_strlen( const char *pcString )
{
	return strlen( pcString );
}

unsigned int ansi_sprintf( char *pcbuffer, const char *pcFormat, ... )
{
	unsigned int retVal;
	va_list args;
	
	va_start( args, pcFormat );
	retVal = vsprintf( pcbuffer, pcFormat, args );
	va_end( args );
	return retVal;
}

int DBG_DRV_Printf(const char *format, ...) 
{ 
	//osal_task_dispatch_off();

      int n; 
      va_list arg_ptr; 
      va_start(arg_ptr, format); 
      n = vprintf(format, arg_ptr); 
      va_end(arg_ptr); 

  	//osal_task_dispatch_on();

} 

long int ansi_time( long int *pliTimer )
{
	//return time( pliTimer ); //[irca] pyh:ËÆºõÃ»ÓÐµ÷ÓÃÕâ¸öº¯Êý
}

//[irca] pyh:for ca_task.h
ia_bool CA_MEM_Protect (void *pvAddr, ia_uint32 uLen)
{
	/* check for NULL as minimum requirement. */
	if( pvAddr != NULL )
		return IA_TRUE;
	else
		return IA_FALSE;
}

