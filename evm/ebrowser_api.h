#ifndef __EN_API_
#define __EN_API_

typedef void eBrowserHandler;
typedef void eBrowserParam;

/*
    int eBrowser_setup(eBrowserHandler* pThis, eBrowserParam* param);
    Description
    Init eBrowser parameter, must be called before eBrowser_open
    Parameter
    pThis: pointer which returned by this function point to a instance of eBrowser.
    param: browser parameter structure used to init the browser.
*/
int eBrowser_setup(eBrowserHandler* pThis, eBrowserParam* param);

/*
    int eBrowser_open(eBrowserHandler* pThis, char* url);
    Description
    Start the eBrowser, will allocate resource and start thread.
    Parameter
    pThis: the pointer of instance of eBrowser.
*/
int eBrowser_open(eBrowserHandler* pThis, char* url);

/*
    int eBrowser_suspend(eBrowserHandler* pThis);
    Description
    Suspend eBrowser, suspend all browser thread, but did not release resource.
    Parameter
    pThis: the pointer of instance of eBrowser.
*/
int eBrowser_suspend(eBrowserHandler* pThis);

/*
    int eBrowser_resume(eBrowserHandler* pThis);
    Description
    Resume browser thread to run status.
    Parameter
    pThis: the pointer of instance of eBrowser.
*/
int eBrowser_resume(eBrowserHandler* pThis);

/*
    int eBrowser_close(eBrowserHandler* pThis);
    Description
    Close eBrowser and release all resource.
    Parameter
    pThis: the pointer of instance of eBrowser.
*/
int eBrowser_close(eBrowserHandler* pThis);

/*
    const char* eBrowser_version(eBrowserHandler* pThis);
    Description
    Return the version.
    Parameter
    pThis: the pointer of instance of eBrowser.
*/
const char* eBrowser_version(eBrowserHandler* pThis);

/*
    char eBrowser_setUrl(eBrowserHandler* pThis, unsigned char* url)
    Description: 
    This function is used to navigate to a new url. 

    Parameter
    this: the pointer of instance of eBrowser.
    url is the new location( the complete url )
*/
char eBrowser_setUrl(eBrowserHandler* pThis, unsigned char* url);

/*
    int eBrowser_setErrorPage(eBrowserHandler* pThis, char* url);
    Description: 
    This function is used to set error page used by browser.  Usually the url should looks like
    diskfile://.../error.htm. Then browser will call your loadDiskFile callback to get the content.

    Parameter
    this: the pointer of instance of eBrowser.
    buffer: is the buffer start pointer
*/
int eBrowser_setErrorPage(eBrowserHandler* pThis, char* url);

/*
    char eBrowser_setFSAddress(eBrowserHandler* pThis, void* buffer, int size)
    Description: 
    This function is used to set EnReach File System buffer address. 

    Parameter
    this: the pointer of instance of eBrowser.
    buffer: is the buffer start pointer
    size: the length of the file system
*/
int eBrowser_setFSAddress(eBrowserHandler* pthis, void* buffer, int size);

/*
    char eBrowser_setMemory(eBrowserHandler* pThis, void* buffer)
    Description: 
    This function is used to set memory buffer. 

    Parameter
    this: the pointer of instance of eBrowser.
    buffer: is the buffer start pointer
*/
int eBrowser_setMemory(eBrowserHandler* pthis, void* buffer);

/*
    char eBrowser_setVideoInfo(eBrowserHandler* pThis, void* buffer, unsigned int w, unsigned int h)
    Description: 
    This function is used to set video information. 

    Parameter
    this: the pointer of instance of eBrowser.
    buffer: is the buffer start pointer
    w: is the width of the buffer
    h: is the height of the buffer
*/
int eBrowser_setVideoInfo(eBrowserHandler* pthis, void* buffer, unsigned int w, unsigned int h);

/*
    char eBrowser_setCacheMemory(eBrowserHandler* pThis, void* buffer,int size)
    Description: 
    This function is used to set cache memory buffer. 

    Parameter
    this: the pointer of instance of eBrowser.
    buffer: is the buffer start pointer
    size:is the size of the cache buffer
*/
int eBrowser_setCacheMemory(eBrowserHandler* pThis, void* buffer, int size);

/*
    char eBrowser_clearCache(eBrowserHandler* this);
    Description: 
    This function is used to clear cache. 

    Parameter
    this: the pointer of instance of eBrowser.
*/
char eBrowser_clearCache(eBrowserHandler* pThis);

/*
    void eBrowser_setHighlightBox(eBrowserHandler* pThis, short enable)
    Description
    This function used to enable/disable browser's highlight box.

    Parameter
    pThis: the pointer of instance of eBrowser.
    int enable: if true then enable highlight, otherwise disable the highlight box for anchor and control
*/
void eBrowser_setHighlightBox(eBrowserHandler* pThis, int enable);

/*
    void eBrowser_setHighlightTextColor(eBrowserHandler* pThis, unsigned int text_bg, unsigned int text_fg);
    Description
    This function used to set the highlight forecolor and background color for a text node, like text anchor.

    Parameter
    pThis: the pointer of instance of eBrowser.
    int text_bg: background color for the text the format is RGB
    int text_fg: foreground color for the text the format is RGB
*/
void eBrowser_setHighlightTextColor(eBrowserHandler* pThis, unsigned int text_bg, unsigned int text_fg);

/*
    void eBrowser_setNoScroll(eBrowserHandler* pThis, char noscroll);
    Description
    This function used to set the scroll bahavior to force no scrool or not.

    Parameter
    pThis: the pointer of instance of eBrowser.
    char noscroll: if true then force all page no scroll
*/
void eBrowser_setNoScroll(eBrowserHandler* pThis, char noscroll);

/*
    void eBrowser_setLinkColor(eBrowserHandler* pThis, int rgbColor);
    Description
    This function used to set the default color for a link..

    Parameter
    pThis: the pointer of instance of eBrowser.
    int rgbColor: is the RGB format color for the a link
*/
void eBrowser_setLinkColor(eBrowserHandler* pThis, int rgbColor);

/*
    void eBrowser_setTextColor(eBrowserHandler* pThis, int rgbColor);
    Description
    This function used to set the default color for general text.

    Parameter
    pThis: the pointer of instance of eBrowser.
    int rgbColor: is the RGB format color for the general
*/
void eBrowser_setTextColor(eBrowserHandler* pThis, int rgbColor);

/*
    void eBrowser_setCustomizedStartScreen(void (*myCustomizedStartScreen)());
    Description
    This function used to set a new customized functoin to show boot logo.

    Parameter
    pThis: the pointer of instance of eBrowser.
    void (*myCustomizedStartScreen)(): is the customized function to show boot logo
*/
void eBrowser_setCustomizedStartScreen(void (*myCustomizedStartScreen)());

/*
    void eCallback_cleanWindow(unsigned char* framebuffer, unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h);
    Description: 
    This callback will be called to clean the framebuffer. 

    Parameter:
    framebuffer:framebuffer is the address of the frame buffer
    win_x:Output window horizontal position;
     	win_y:Output window vertical position;
     	win_w:Output window width;
     	win_h:Output window height;

    void regist_callback_cleanWindow(eCallback_cleanWindow* callback);
    Description:
    This function is used to regist the eCallback_cleanWindow callback. It must be called after 
    eBrowser_setup and before eBrowser_open

    说明如下：
    这个绘图函数清除 显示屏幕 的某一矩形区域
    win_x, win_y确定该矩形区域的位置
    win_w, win_h确定该矩形区域的大小
    		
*/
typedef void eCallback_cleanWindow(unsigned char* framebuffer, unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h);
void regist_callback_cleanWindow(eCallback_cleanWindow* callback);

/*
    void eCallback_updateScreen(unsigned char* src, unsigned char* dst, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
    Description: 
    This callback will be called to update the screen when need to refresh the screen. 

    Parameter:
    src is the address of the source buffer
    dst is the address of the destnation buffer
    (srcX, srcY, srcW, srcH) source rectangle in the source buffer
    (dstX, dstY) is the upper-left at the destination buffer

    void regist_callback_updateScreen(eCallback_updateScreen* callback);
    Description:
    This function is used to regist the eCallback_updateScreen callback. It must be called 
    after eBrowser_setup and before eBrowser_open
*/
typedef void eCallback_updateScreen(unsigned char* src, unsigned char* dst, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
void regist_callback_updateScreen(eCallback_updateScreen* callback);

void regist_callback_getSystemData(void (*myGetSystemData)(int type, char **p,char *len), void (*myGetSystemDataFree)(char *p));

/*
    void eBrowser_show(bool flag);
    Description
    This function is used to display browser to screen or clean screen. For some platform maybe just used for render the browser to screen (can not clean the screen).

    Parameter:
    flag:  is true then render the browser, otherwise clean the screen.
*/
void eBrowser_show(char flag);

/*
    void eBrowser_event(unsigned char keyStatus, unsigned short scancode);
    Description
    This function used to pass the event to browser(keyboard, mouse)

    Parameter:
    keyStatus: is the indicator for Shift, Ctrl, Alt function;
    scancode: is the scancode of the pressed key;
*/
void eBrowser_event(unsigned char status, unsigned short scancode);

/*
    void eBrowser_keyToScancode( unsigned char *status, unsigned char *scancode, unsigned short key );
    Description
    This function used to convert a key to scancode, and it's shift status

    Parameter:
    status: is output parameter, return the indicator for Shift, Ctrl, Alt function;
    scancode: is output parameter, return the scancode of the pressed key;
    key: is input parameter, it is the value of pressed key
*/
void eBrowser_keyToScancode( unsigned char *status, unsigned char *scancode, unsigned short key);

/*按键处理函数*/
//和IE一样 回退至上一页
void eBrowser_key_backward();
//和IE一样 进入到下一页
void eBrowser_key_forward();
// refresh刷新当前页面
void eBrowser_key_refresh();

// 激活、取消工具栏
void eBrowser_key_toolbar();

// 激活、取消虚拟键盘
void eBrowser_key_vkb();
 //确认操作
void eBrowser_key_enter();
// up key 向上移动
void eBrowser_key_up();
// down key 向下移动
void eBrowser_key_down();
// up key 向左移动
void eBrowser_key_left();
// up key 向右移动
void eBrowser_key_right();

// 播放vod
void eBrowser_vod_play();
// 暂停vod
void eBrowser_vod_pause();
// 快进vod
void eBrowser_vod_ff();
// 快退vod
void eBrowser_vod_fb();
// 停止vod
void eBrowser_vod_stop();

/*
    void eCallback_setSmallWindow(unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h);
    Description: 
    This callback will be called to set small window. 

    Parameter:
    win_x:Output window horizontal position;
    win_y:Output window vertical position;
    win_w:Output window width;
    win_h:Output window height;

    void regist_callback_setSmallWindow(eCallback_setSmallWindow* callback);
    Description:
    This function is used to regist the eCallback_setSmallWindow callback. It must be called after 
    eBrowser_setup and before eBrowser_open
*/
typedef void eCallback_setSmallWindow(unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h);
void regist_callback_setSmallWindow(eCallback_setSmallWindow* callback);

/*
    char eCallback_setTuner(unsigned int freq, unsigned int SymbolRate,unsigned int qam);
    Description: 
    This callback will be called to setTuner. 

    Parameter:
    freq:点播节目所在的物理频点;
    SymbolRate:符号率,如6875;
    qam:调制值,如64;

    void regist_callback_setTuner(eCallback_setTuner* callback);
    Description:
    This function is used to regist the eCallback_setTuner callback. It must be called after 
    eBrowser_setup and before eBrowser_open
*/

// return 0 : success
// return 1: failed
typedef char eCallback_setTuner(unsigned int freq, unsigned int SymbolRate,unsigned int qam);
void regist_callback_setTuner(eCallback_setTuner* callback);

typedef char eCallback_PlayTV(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int vid, unsigned int aid, unsigned int pcrid);
void regist_callback_PlayTV(eCallback_PlayTV* callback);

typedef char eCallback_PlayTV_Srv(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int service_id);
void regist_callback_PlayTV_Srv(eCallback_PlayTV_Srv* callback);

typedef char eCallback_PlayTV_Pmt(unsigned int freq, unsigned int SymbolRate,unsigned int qam, unsigned int pmt_pid);
void regist_callback_PlayTV_Pmt(eCallback_PlayTV_Pmt* callback);

typedef char eCallback_StopTV(unsigned char* url, int type);
void regist_callback_StopTV(eCallback_StopTV* callback);

typedef char eCallback_StartOC(unsigned int freq, unsigned int SymbolRate, unsigned int qam,unsigned int service_id);
void regist_callback_StartOC(eCallback_StartOC* callback);

typedef char eCallback_StopOC();
void regist_callback_StopOC(eCallback_StopOC* callback);


typedef char eCallback_enreach_StartOC(unsigned int freq, unsigned int SymbolRate, unsigned int qam,unsigned int service_id);
void regist_callback_enreach_StartOC(eCallback_enreach_StartOC* callback);

typedef char eCallback_enreach_StopOC();
void regist_callback_enreach_StopOC(eCallback_enreach_StopOC* callback);

typedef int eCallback_rtspPlay(char *url, unsigned char alpha, unsigned char isfullScreen, unsigned short x, unsigned short y, unsigned short w, unsigned short h);
void regist_callback_rtspPlay(eCallback_rtspPlay* callback);

/*
    void eCallback_GetNetworkSetting(unsigned long* ip, unsigned long* mask, unsigned long* gateway, unsigned long* dns);	Description: 
    This callback will be called to set network setting for ebrowser. 

    Parameter:
    unsigned long* ip: ip address;
    unsigned long* mask:mask;
    unsigned long* gateway:gateway;
    unsigned long* dns:dns;

    void regist_callback_GetNetworkSetting(eCallback_GetNetworkSetting* callback);
    Description:
    This function is used to regist the eCallback_GetNetworkSetting callback. It must be called after 
    eBrowser_setup and before eBrowser_open
*/
typedef void eCallback_GetNetworkSetting(unsigned long* ip, unsigned long* mask, unsigned long* gateway, unsigned long* dns);
void regist_callback_GetNetworkSetting(eCallback_GetNetworkSetting* callback);

int eBrowser_delayTV(unsigned int pFreq, unsigned int pSymbolRate, unsigned int pQam , unsigned int pNetworkId, unsigned int pTransFlow, unsigned int pService_id , unsigned int pPmt_pid, unsigned int pvid, unsigned int paid, unsigned int pPcrid,char *url,unsigned long time,char *hosturl);
void regist_callback_delayTvZeroExit(int(*my_delayTvZeroExit) (void));

void regist_callback_TvFullScreen(int(*my_TvFullScreen) (void));

void regist_callback_Customized_exit_eb(void (*my_Customized_eixt_eb)(char * exitflag));

typedef void eCallback_task_destroyself(void);
void regist_callback_task_destroyself(eCallback_task_destroyself *callback);

/*******************************时移******************************************/

/*向STB 传送 当前播放时间*/
typedef void (*cb_stb_messagetime)(char*messagetimetype,int time1,int time2,int time3,int time4,int time5,int time6);
void regist_callback_MessageTime(void (*my_stb_messagetime)(char*messagetimetype,int time1,int time2,int time3,int time4,int time5,int time6));

typedef  void (*cb_stb_initstbshowgui)(char*CurrentChannel,char*PrevChannel,char*NextChannel,
    int sYear,int sMonth,int sDay, int sHour, int sMinute, int sSecond, 
    int cYear,int cMonth,int cDay, int cHour, int cMinute, int cSecond,
    int eYear,int eMonth,int eDay, int eHour, int eMinute, int eSecond);
void regist_callback_InitSTBshowGUI(void (*my_stb_initstbshowgui)
    (char*CurrentChannel,char*PrevChannel,char*NextChannel,
    int sYear,int sMonth,int sDay, int sHour, int sMinute, int sSecond, 
    int cYear,int cMonth,int cDay, int cHour, int cMinute, int cSecond,
    int eYear,int eMonth,int eDay, int eHour, int eMinute, int eSecond));

typedef void (*cb_StbPostMessage)(char *message);
void regist_callback_StbPostMessage(void (*my_stb_StbPostMessage)(char *message));

typedef enum eBrow_Stus {
    Invalid_Status = 0,
    WebPage_Status,
    VOD_Status,
    TimeShift_Status
}eBrowser_Status;

int eBrowser_GetStatus(eBrowserHandler* pThis);
void regist_callback_StbGetPara(char *(*my_cb_GetPara)(char*type));


#define EBROWSER_OK 0
#define EBROWSER_ERROR 1


/****************************haisi interface*****************************************/
void Callback_setSmallWindow(unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h);
char Ebrowser_SetTuner(unsigned int u32Freq, unsigned int u32SymbolRate, unsigned int u32Qam);
char Callback_setTuner(unsigned int freq, unsigned int SymbolRate,unsigned int qam);
char Callback_PlayTV(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int vid, unsigned int aid, unsigned int pcrid);
char Callback_PlayTV_Srv(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int service_id);
char Callback_PlayTV_Pmt(unsigned int freq, unsigned int SymbolRate,unsigned int qam, unsigned int pmt_pid);
char Callback_StopTV(unsigned char* url, int type);
int CallBack_TvFullScreen();
void Callback_cleanWindow(unsigned char* framebuffer, unsigned short win_x,unsigned short win_y,unsigned short win_w,unsigned short win_h);
void Callback_updateScreen(unsigned char* src, unsigned char* dst, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY);
char Callback_StartOC(unsigned int freq, unsigned int SymbolRate, unsigned int qam, unsigned int service_id);
char Callback_StopOC();
void CallBack_ExitEBrowser(char *exitFlag);



/* 浏览器线程状态管理接口 */

unsigned long EnReach_task_create(char* name, void *proc,
                                  void* args, unsigned long stackSize);

unsigned long EnReach_task_suspend(unsigned long id);

unsigned long EnReach_task_resume(unsigned long id);

void EnReach_task_yield(void);

void EnReach_task_destroy(unsigned long pid);

void EnReach__destroyself(void);

unsigned long time_getCurrentMSec();

void EnReach_task_sleep(unsigned long ms);

unsigned long EnReach_task_getid(void);


/* 浏览器信号量管理接口 */

unsigned long EnReach_semaphore_create(char *name);

void EnReach_semaphore_delete(unsigned long id);

void EnReach_semaphore_lock(unsigned long id);

void EnReach_semaphore_unlock(unsigned long id);

int EnReach_GetTSID(int iFre, int iSym, int iQam);

char* EnReach_GetSTBID(void);
void HI_SetTVState(int bState);
void Callback_GetNetworkSetting(unsigned long* ip, unsigned long* mask,
                               unsigned long* gateway, unsigned long* dns);



#endif 

