
#ifndef __GUI_DRIVER_H_
#define __GUI_DRIVER_H_


typedef struct _tagGUI_ScreenDevice_s * PSD;

typedef struct _tagGUI_ScreenDevice_s 
{
    BU32  doclip;
    BU32  clipminx;        /* minimum x value of cache rectangle */
    BU32  clipminy;        /* minimum y value of cache rectangle */
    BU32  clipmaxx;        /* maximum x value of cache rectangle */
    BU32  clipmaxy;        /* maximum y value of cache rectangle */

    BU32  xres;        /* X screen res (real) */
    BU32  yres;        /* Y screen res (real) */
    BU32  xvirtres;    /* X drawing res (will be flipped in portrait mode) */
    BU32  yvirtres;    /* Y drawing res (will be flipped in portrait mode) */
    BU32  planes;        /* # planes*/
    BU32  bpp;        /* # bits per pixel*/
    BU32  linelen;    /* line length in bytes for bpp 1,2,4,8*/
                /* line length in pixels for bpp 16, 24, 32*/
    BU32  size;        /* size of memory allocated*/

    BU32 gr_foreground;      /* current foreground color */
    BU32 gr_background;      /* current background color */
    BU32 gr_mode;

    BU32 ncolors;    /* # screen colors */
    BU32  pixtype;    /* format of pixel value */
    BU32  flags;        /* device flags */
    void *addr;        /* address of memory allocated (memdc or fb) */
    void *dev_spec;    /* device specific field. */

    PSD  (*Open)(PSD psd);
    void (*Close)(PSD psd);
    void (*SetPalette)(PSD psd,BU32 first,BU32 count,PU32 cmap);
    void (*GetPalette)(PSD psd,BU32 first,BU32 count,PU32 cmap);
    PSD  (*AllocateMemGC)(PSD psd);
    BU32 (*MapMemGC)(PSD mempsd,BU32 w,BU32 h,BU32 planes,BU32 bpp, BU32 linelen,BU32 size,void *addr);
    void (*FreeMemGC)(PSD mempsd);
    BU32 (*ClipPoint)(PSD psd,BU32 x,BU32 y);
    void (*FillRect)(PSD psd,BU32 x,BU32 y,BU32 w,BU32 h,BU32 c);
    
//==================================================================================//
    void (*DrawPixel)(PSD psd, BU32 x, BU32 y, BU32 c);
    BU32 (*ReadPixel)(PSD psd, BU32 x, BU32 y);
    void (*DrawHLine)(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c);
    void (*DrawVLine)(PSD psd, BU32 x, BU32 y, BU32 w, BU32 c);
    void (*Blit)(PSD dstpsd, BU32 dstx, BU32 dsty, BU32 w, BU32 h, 
				PSD srcpsd, BU32 srcx, BU32 srcy);
    void (*PutBox)( PSD psd, BU32 x, BU32 y, BU32 w, BU32 h, void* buf );
    void (*GetBox)( PSD psd, BU32 x, BU32 y, BU32 w, BU32 h, void* buf );
    void (*PutBoxMask)( PSD psd, BU32 x, BU32 y, BU32 w, BU32 h, void *buf, BU32 cxx);
    void (*CopyBox)(PSD psd,BU32 x1, BU32 y1, BU32 w, BU32 h, BU32 x2, BU32 y2);
    void (*UpdateRect) (PSD psd, BU32 l, BU32 t, BU32 r, BU32 b);
//==================================================================================//
//extern function
	void (*Printf)(PU08 fmt);
	void*  (*MemMalloc)(BU32 size);
	void (*MemFree)(void * lpmem);
	void (*TimerDelay)(BU32 ms);

//===================================================================================//
} GUI_ScreenDevice_s,* GUI_ScreenDevice_sp;

/* PSD flags*/
#define	PSF_SCREEN			0x0001	/* screen device*/
#define PSF_MEMORY			0x0002	/* memory device*/
#define PSF_HAVEBLIT		0x0004	/* have bitblit*/
#define PSF_HAVEOP_COPY		0x0008	/* psd->DrawArea can do area copy*/
#define PSF_ADDRMALLOC		0x0010	/* psd->addr was malloc'd*/
#define PSF_ADDRSHAREDMEM	0x0020	/* psd->addr is shared memory*/

/* return*/
#define GUI_SYS_SUCCESS	0x0001
#define GUI_SYS_FAILURE	0x0002
#define GUI_SYS_TIMEOUT	0x0003

/*mutex flags*/
#define GUI_SYS_MUTEX_OBTAIN	0x0001	/*have mutex*/
#define GUI_SYS_MUTEX_NOTOBTAIN	0x0002  /*no mutex*/

/*mutex flags*/
#define GUI_SYS_EVENT_MANUAL_RST	0x0001	/*have mutex*/
#define GUI_SYS_EVENT_INIT_STATUS	0x0002  /*no mutex*/

typedef struct _tagGUI_SystemDevice_s 
{
	/*mutex*/
	BU32 (*CreateMutex)(const PU08 pstrName,BU32 flag );
	BU32 (*DestroyMutex)(BU32 hMutex);
	BU32 (*WaitForMutex)(BU32 hMutex,BU32 TimeoutMs);
	BU32 (*ReleaseMutex)(BU32 hMutex);
	/*event*/
	BU32 (*CreateEvent)(const PU08 pstrName,BU32 flag );
	BU32 (*DestroyEvent)(BU32 hEvent);	
	BU32 (*SetEvent)(BU32 hEvent);
	BU32 (*WaitForSingleEvent)(BU32 hEvent,BU32 TimeoutMs);	
	/*semaphore*/
	BU32 (*CreateSemaphore)(const PU08 pstrName,BU32 nInitCount,BU32 nMaxCount );
	BU32 (*DestroySemaphore)(BU32 hMutex);
	BU32 (*WaitForSemaphore)(BU32 hMutex,BU32 TimeoutMs);
	BU32 (*ReleaseSemaphore)(BU32 hMutex,BU32 nReleaseCount);
	/*MsgQueue*/
	BU32 (*CreateMsgQueue)(const PU08 pstrName,BU32 nMaxMsgs,BU32 nMsgLen,BU32 Options );
	BU32 (*DestroyMsgQueue)(BU32 hMsgQueue);
	BU32 (*ReceiveMsgQueue)(BU32 hMsgQueue,void * pvMsg,BU32 nMaxMsgBYtes,BU32 TimeoutMs);
	BU32 (*SendMsgQueue)(BU32 hMsgQueue,const void * pvMsg,BU32 nMaxMsgBYtes,BU32 TimeoutMs);
	


} GUI_SystemDevice_s ,* GUI_SystemDevice_sp;

#define GUI_THREAD_WAIT_FIFO	0
#define GUI_THREAD_WAIT_PRIO    1


#define GUI_NO_WAIT				0
#define GUI_WAIT_FOREVER		-1 

#define GUI_OK			0
#define GUI_ERR			-1


typedef struct _tagGUI_Semaphore_s
{ 
    BU32 (*FYF_CPI_semaphore_create)(const PU08 name,BS32 initial_count,BU32 
	      wait_mode); 
  	BS32 (*FYF_CPI_semaphore_destroy)(BU32 handle); 
  	BS32 (*FYF_CPI_semaphore_wait)(BU32 handle,BS32 wait_time); 
  	BS32 (*FYF_CPI_semaphore_release)(BU32 handle); 
}GUI_Semaphore_s,*GUI_Semaphore_sp; 



#endif

