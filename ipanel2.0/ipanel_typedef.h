/******************************************************************************/
/*    Copyright (c) 2005 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the SOUND Porting APIs needed by iPanel  */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_TYPEDEFINE____H_
#define _IPANEL_MIDDLEWARE_PORTING_TYPEDEFINE____H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int				INT32_T;
typedef unsigned int	UINT32_T;
typedef short			INT16_T;
typedef unsigned short	UINT16_T;
typedef char			CHAR_T;
typedef unsigned char	BYTE_T;
#define CONST			const
#define VOID			void

#ifdef __cplusplus
}
#endif

#endif // _IPANEL_MIDDLEWARE_PORTING_TYPEDEFINE____H_
