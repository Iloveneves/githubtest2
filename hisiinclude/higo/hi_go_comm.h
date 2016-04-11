/**
 \file
 \brief HiGo公共内容头文件
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-5-16
 */

#ifndef __HI_GO_COMM_H__
#define __HI_GO_COMM_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/** @defgroup hi_go_common_data_structure 公用数据类型 
 *  @ingroup H2 
 *  @brief 详细描述各模块公用的数据结构和数据类型
 *  @{  */

/** 调色板最大颜色数 */
#define MAX_PALETTE_COLOR_SIZE 256

/** 颜色值 */
typedef HI_U32 HI_COLOR;

/** 调色板 */
typedef HI_COLOR HI_PALETTE[MAX_PALETTE_COLOR_SIZE];


/** 矩形 */
typedef struct
{
    HI_S32 x, y;

    HI_S32 w, h;
} HI_RECT;

/** 区域 */
typedef struct
{
    HI_S32 l;
    HI_S32 t;
    HI_S32 r;
    HI_S32 b;
} HI_REGION;

/** 流式位置*/


/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/** @defgroup hi_go_common_api SYS模块
*   @ingroup H1
*   @brief 详细描述SYS模块的API函数
*   @{  */

/** 
\brief HiGo初始化
\attention \n
使用HiGo功能必须先调用该接口，完成HiGo的初始化
\param 无

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INITFAILED

\see \n
::HI_GO_Deinit
*/
HI_S32	HI_GO_Init(HI_VOID);

/** 
\brief HiGo去初始化
\attention \n
不再使用HiGo功能时需要调用该接口，释放资源
\param 无

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_Init
*/
HI_S32	HI_GO_Deinit(HI_VOID);

/** 
\brief 获取版本信息
\attention \n
无
\param[out] ppVersion  版本信息字符串输出地址，不可为空
\param[out] ppBuildTime Build时间字符串输出地址，不可为空

\retval 无

\see \n
无
*/
HI_VOID HI_GO_GetVersion(HI_CHAR ** ppVersion, HI_CHAR **ppBuildTime);

/** 
\brief 将Surface中的数据转换成BMP格式图象输出。
\attention \n
输出文件名为[year]-[date]-[hour]-[min]-[second]-[ms].bmp
输出图片固定为16位图。
\param[in] Surface需要进行截屏的数据。
\param[in] pRect区域指针。
\retval none
\return none
*/
HI_S32 HI_GO_Surface2Bmp(HI_HANDLE Surface, HI_RECT *pRect);


/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_COMM_H__ */
