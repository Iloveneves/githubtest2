

#ifndef _SVRDEBUG_H
#define _SVRDEBUG_H

//extern HI_U32 g_u32StbDebugLevel ;      /*��ӡ����*/
//extern HI_U32 g_u32StbLogModule;        /*��ӡģ��*/

//#define SVR_DEBUG

/*��ӡ��Ϣ��ģ��*/
typedef enum hiDEBUG_STB_MODULE_E
{
	APPMNGSVR 	= 0x00000001,
	TUNERSVR 	= 0x00000002,
	AVSVR 		= 0x00000004,
	PGSVR 		= 0x00000008,
	EPGSVR 		= 0x00000010,
	NVODSVR 	= 0x00000020,
	BOOKSVR 	= 0x00000040,
	SECTIONSVR	= 0x00000080,
	IOSVR 		= 0x00000100,
	PSISI    		= 0x00000200,
	DB    		= 0x00000400,
	CASVR 		= 0x00000800,
	VBISVR 		= 0x00001000,
	IPSVR 		= 0x00002000,
	NETWORKSVR = 0x00004000,
	MP3SVR 		= 0x00008000,
	MEDIUMSVR 	= 0x00010000,
	EBROWSERSVR=0x00020000,
	ALLSVR 		= 0xffffffff 
}DEBUG_STB_MODULE_E;

/* ����8���ȼ�*/
typedef enum hiSTB_LOG_ERRLEVEL_E
{
	HI_STB_LOG_LEVEL_DEBUG = 0,  /* debug-level                                  */
	HI_STB_LOG_LEVEL_INFO ,       /* informational                                */
	HI_STB_LOG_LEVEL_NOTICE,     /* normal but significant condition             */
	HI_STB_LOG_LEVEL_WARNING,    /* warning conditions                           */
	HI_STB_LOG_LEVEL_ERROR,      /* error conditions                             */
	HI_STB_LOG_LEVEL_CRIT,       /* critical conditions                          */
	HI_STB_LOG_LEVEL_ALERT,      /* action must be taken immediately             */
	HI_STB_LOG_LEVEL_FATAL,      /* just for compatibility with previous version */
	HI_STB_LOG_LEVEL_BUTT
} STB_LOG_ERRLEVEL_E;


HI_U32 g_u32StbDebugLevel = HI_STB_LOG_LEVEL_WARNING;/*��ӡ����,Ĭ��ΪDEBUG�ȼ�*/
HI_U32 g_u32StbLogModule = TUNERSVR;//EBROWSERSVR|SECTIONSVR;/*��ӡģ��,Ĭ��Ϊ����ӡ�κ�ģ��*/
#define SVR_DEBUG
#ifdef SVR_DEBUG
#define SVR_TRACE( level, module, fmt, args... )\
do { \
    if ( ( ( level >= g_u32StbDebugLevel ) && ( module & g_u32StbLogModule)  ) ) \
    { \
        printf( "%s -%s(): %d Line: ", __FILE__,  __FUNCTION__,  __LINE__ );\
            printf( fmt, ##args);\
    }\
} while (0);

#else
#define SVR_TRACE( level, module, fmt, args... )
#endif


#ifdef SVR_DEBUG

#define SVR_ASSERT(expr)\
do{\
    if (!(expr)) { \
        printf("\nASSERT failed at:\n  >File name: %s\n  >Line No. : %d\n  >Condition: %s\n",\
                __FILE__, __LINE__, #expr);\
            abort();\
    } \
}while(0);

#else
#define SVR_ASSERT(expr)
#endif




#endif
