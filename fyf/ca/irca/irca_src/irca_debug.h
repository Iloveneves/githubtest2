/*---------------------------------------------------------------------------------
   File Name	: irca_debug.h

   Description	: debug for ir ca driver
                  Create by PYH.

   Version		: REL_V1.0.0

   Copyright (c) 2008-2100 Triumph Corporation.  All rights reserved.

   History      : 1.2008/07/01 PYH - Create file and add debug interferc.
----------------------------------------------------------------------------------*/

#ifndef __IRCA_DEBUG_H
#define __IRCA_DEBUG_H

#include <api/libc/printf.h>


#define IRCA_DRV_DEBUG_ERR		(1)
#define IRCA_DRV_DEBUG  		(1)

#define IRCA_DRV_DEBUG_OS		(0)
#define IRCA_DRV_DEBUG_VDSC     (0)
#define IRCA_DRV_DEBUG_VDSTG    (0)
#define IRCA_DRV_DEBUG_VDDMX    (0)
#define IRCA_DRV_DEBUG_VDDES    (0)
#define IRCA_DRV_DEBUG_VDTIM   (0)
#define IRCA_DRV_DEBUG_VDLOD   (0)

#if IRCA_DRV_DEBUG_ERR
    #define IrCa_Debug_Err(x)                          			\
            {                                          			\
				printf("\n  [!! IrCa ERR !!]*******"); 	\
				printf x;                            		\
				printf("\n"); 							\
            }
#else
    #define IrCa_Debug_Err(x) { }
#endif

#if IRCA_DRV_DEBUG
    #define IrCa_Debug(x)                          			\
            {                                          			\
				printf x;                            	\
            }
#else
    #define IrCa_Debug(x) { }
#endif

#if IRCA_DRV_DEBUG_OS
    #define IrCa_Debug_Os(x)                          			\
            {                                          			\
				printf("\n[IrCa OS]"); 				\
				printf x;                           		\
				printf("\n"); 								\
            }
#else
    #define IrCa_Debug_Os(x) { }
#endif

#if IRCA_DRV_DEBUG_VDSC
    #define IrCa_Debug_VdSc(x)                          		\
            {                                          			\
				printf("\n  [IrCa SmartCard]"); 			\
				printf x;                            		\
				printf("\n"); 								\
            }
#else
    #define IrCa_Debug_VdSc(x) { }
#endif

#if IRCA_DRV_DEBUG_VDSTG
    #define IrCa_Debug_VdStg(x)                          			\
            {                                          			\
				printf("\n   [IrCa Storage]   "); 				\
				printf x;                            	\
				printf("\n"); 								\
            }
#else
    #define IrCa_Debug_VdStg(x) { }
#endif

#if IRCA_DRV_DEBUG_VDDMX
    #define IrCa_Debug_VdDmx(x)                          			\
            {         											\
            	printf("\n  [IrCa Dmux]"); 			\
				printf x;                           		\
            }
#else
    #define IrCa_Debug_VdDmx(x) { }
#endif

#if IRCA_DRV_DEBUG_VDDES
    #define IrCa_Debug_VdDes(x)                          			\
            {                                          			\
				printf("\n   [IrCa Desc]   "); 				\
				printf x;                             	\
				printf("\n"); 								\
			}
#else
    #define IrCa_Debug_VdDes(x) { }
#endif

#if IRCA_DRV_DEBUG_VDTIM
    #define IrCa_Debug_VdTim(x)                          			\
            {                                          			\
				printf("\n   [IrCa Timer]   "); 				\
				printf x;                             	\
				printf("\n"); 								\
            }
#else
    #define IrCa_Debug_VdTim(x) { }
#endif

#if IRCA_DRV_DEBUG_VDLOD
    #define IrCa_Debug_VdLod(x)                          			\
            {                                          			\
				printf("\n   [IrCa Loader]   "); 				\
				printf x;                            	\
				printf("\n"); 								\
            }
#else
    #define IrCa_Debug_VdLod(x) { }
#endif

#endif

