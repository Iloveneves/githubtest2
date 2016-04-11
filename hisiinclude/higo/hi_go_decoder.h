/**
 \file
 \brief Decoder模块头文件
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version draft
 \author 1.0
 \date 2008-5-16
 */

#ifndef __HI_GO_DECODE_H__
#define __HI_GO_DECODE_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/

/** @defgroup hi_go_decoder_data_structure Dec模块
 *  @ingroup H2
 *  @brief 详细描述Dec模块的数据结构和数据类型
 *  @{  */
 
/** 解码实例的解码类型 */
typedef enum 
{
    HIGO_DEC_IMGTYPE_JPEG = 0, /**< JPEG格式图片*/
    HIGO_DEC_IMGTYPE_GIF,      /**< GIF格式图片*/
    HIGO_DEC_IMGTYPE_BMP,      /**< BMP格式图片 */
    HIGO_DEC_IMGTYPE_PNG,      /**< PNG格式图片 */
    HIGO_DEC_IMGTPYE_BUTT
} HIGO_DEC_IMGTYPE_E;

/** 码流的来源 */
typedef enum 
{
    HIGO_DEC_SRCTYPE_FILE = 0, /**< 码流来源于文件*/
    HIGO_DEC_SRCTYPE_MEM,      /**< 码流来源于内存*/
    HIGO_DEC_SRCTYPE_STREAM,      /**< 码流来源于流式数据*/    
    HIGO_DEC_SRCTYPE_BUTT
} HIGO_DEC_SRCTYPE_E;

typedef enum 
{
    HIGO_DEC_POSITION_SET = 0,   /*起始位置*/
    HIGO_DEC_POSITION_CUR,       /*从当前位置*/
    HIGO_DEC_POSITION_END,       /*从结束位置*/
    HIGO_DEC_POSITION_BUTT
}HIGO_DEC_POSITION_E;
typedef HI_S32 (*PTR_Dec_WriteStreamCB_FN)(HIGO_DEC_POSITION_E Position,  HI_S32 Offset);
/**码流来源的详细信息*/
typedef union 
{
    //输入源为内存块时需要的信息
    struct
    {
        HI_CHAR* pAddr;      /**< 内存指针地址*/
        HI_U32 Length;       /**< 长度*/
    } MemInfo;

    //图片文件名
    const HI_CHAR *pFileName;
    PTR_Dec_WriteStreamCB_FN pWriteCB;
} HIGO_DEC_SRCINFO_U;

/** 创建解码实例需要的参数 */
typedef struct 
{
    HIGO_DEC_SRCTYPE_E SrcType; /**< 输入码流的类型*/
    HIGO_DEC_SRCINFO_U SrcInfo; /**< 输入源详细信息*/
} HIGO_DEC_ATTR_S;

/** 解码后主信息 */
typedef struct 
{
    HI_U32             Count;           /**< 图片的个数 */
    HIGO_DEC_IMGTYPE_E ImgType;         /**< 输入源类型 */
    HI_U32             ScrWidth;        /**< 屏幕宽度，此处屏幕表示图片文件屏幕，0表示无效 */
    HI_U32             ScrHeight;       /**< 屏幕高度，此处屏幕表示图片文件屏幕，0表示无效 */
    HI_BOOL            IsHaveBGColor;   /**< 是否有背景色 */
    HI_COLOR           BGColor;         /**< 背景颜色 */
} HIGO_DEC_PRIMARYINFO_S;

/** 索引图片格式 */
typedef struct 
{
    HI_U32    OffSetX;                /**< 在屏幕上的X偏移量，此处屏幕表示图片文件屏幕，下同 */
    HI_U32    OffSetY;                /**< 在屏幕上的Y偏移量*/
    HI_U32    Width;                  /**< 原始图片的高度 */
    HI_U32    Height;                 /**< 原始图片的宽度 */
    HI_U8     Alpha;                  /**< 用于图片进行叠加的通道alpha信息，范围是0-255 */
    HI_BOOL   IsHaveKey;              /**< 是否有KEY。 HI_FALSE: KEY不使能；HI_TRUE: KEY使能 */
    HI_COLOR  Key;                    /**< 用于图片进行叠加的colorkey信息 */
    HIGO_PF_E Format;                 /**< 图片源像素格式 */
    HI_U32    DelayTime;              /**< 与上一幅图片的时间间隔(10ms)，0表示该参数无效 */
    HI_U32    DisposalMethod;         /**< 当前帧处理方法，此接口只对GIF有效，0:用户决定处理方式，1:不做处理，2:使用背景色，*/    
} HIGO_DEC_IMGINFO_S;

/** 解码后图片属性 */
typedef struct 
{
    HI_U32    Width;                  /**< 期望解码后宽度 */
    HI_U32    Height;                 /**< 期望解码后高度*/
    HIGO_PF_E Format;                 /**< 期望解码后像素格式*/
} HIGO_DEC_IMGATTR_S;

/** 扩展数据格式类型 */
typedef enum 
{
    HIGO_DEC_EXTEND_EXIF = 0,         /**< EXIF格式的扩展数据 */
    HIGO_DEC_EXTEND_EXIF_BUTT
} HIGO_DEC_EXTENDTYPE_E;

/** 图片基本信息 */
typedef struct 
{
    HI_U32    Width;                /**< 图片的高度 */
    HI_U32    Height;               /**< 图片的宽度 */
    HIGO_PF_E PixelFormat;          /**< 像素格式 */
    HI_U32    VirAddr[3];           /**< 虚拟地址。如果是RGB格式，只有第一个VirAddr[0]为起始地址，
                                                VirAddr[1]和VirAddr[2]无效；如果是YC格式，
                                                VirAddr[0]表示Y分量起始地址，VirAddr[1]表示C分量起始地址，
                                                VirAddr[2]无效 */
    HI_U32    PhyAddr[3];           /**< 物理地址，使用原则同上 */
    HI_U32    Pitch[3];             /**< 行间距，使用原则同上 */
    HI_U32    Palate[256];          /**< 调色板 */
} HIGO_DEC_IMGDATA_S;

/** 输入流信息*/
typedef struct
{
    HI_VOID *pStreamAddr;    /**< 流虚拟起始地址*/
    HI_U32   StreamLen;      /**< 流长度*/
    HI_BOOL  EndFlag;        /**< 码流结束标记*/
    HI_U32   CopyLen;        /**< 实际拷贝的数据长度*/
}HIGO_DEC_WRITESTREAM_S;


/** @} */  /*! <!-- Structure Definition end */
/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/** @defgroup hi_go_decoder_api Dec模块
 *  @ingroup H1
 *  @brief 详细描述Dec模块的API函数
 *  @{  */

/** 
\brief 解码器初始化
\attention \n
::HI_GO_Init中已包含对该接口的调用
\param  无

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE

\see \n
::HI_GO_Init \n
::HI_GO_DeinitDecoder
*/

HI_S32 HI_GO_InitDecoder(HI_VOID);


/** 
\brief 解码器去初始化
\attention \n
::HI_GO_Deinit中已包含对该接口的调用
\param  无

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE

\see \n
::HI_GO_Deinit \n
::HI_GO_InitDecoder
*/

HI_S32 HI_GO_DeinitDecoder(HI_VOID);


 /** 
 \brief 创建解码句柄
 \attention \n
目前支持BMP、PNG、JPEG和GIF格式，图片输入源只可以为内存、文件以及流式，流式目前只对JPEG格式有效。
 \param[in] pSrcDesc 输入源信息
 \param[out] pDecoder 解码实例句柄指针
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_NOTINIT
 \retval ::HIGO_ERR_NOMEM
 \retval ::HIGO_ERR_INVSRCTYPE
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_DestroyDecoder
*/
HI_S32 HI_GO_CreateDecoder(const HIGO_DEC_ATTR_S* pSrcDesc, HI_HANDLE* pDecoder);

 /** 
 \brief 销毁解码句柄
 \attention \n
  无 
 \param[in] Decoder 解码实例句柄
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDecoder
*/
HI_S32 HI_GO_DestroyDecoder(HI_HANDLE Decoder);

/** 
 \brief 流式码流接口
 \attention \n
  无 
 \param[in] Decoder 解码实例句柄
 \param[in] pStream 码流信息指针 
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDecoder
*/
HI_S32 HI_GO_DecWriteStream(HI_HANDLE Decoder, HIGO_DEC_WRITESTREAM_S *pStream);

/** 
 \brief 复位解码器，当需要终止解码的时候就进行解码器复位
 \attention \n
 无
 \param[in] Decoder 解码实例句柄
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDecoder \n
::HI_GO_DestroyDecoder
*/
HI_S32 HI_GO_ResetDecoder(HI_HANDLE Decoder);


 /** 
 \brief 获取图片文件的主要信息
 \attention
 无
 \param[in] Decoder 解码实例句柄
 \param[out] pPrimaryInfo 解码图片主信息指针
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_INVHANDLE
 \retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_DecImgInfo
*/
HI_S32 HI_GO_DecCommInfo(HI_HANDLE Decoder, HIGO_DEC_PRIMARYINFO_S *pPrimaryInfo);

/** 
 \brief 获取指定图片的信息，但同时用户可以设置期望的图象格式
 \attention \n
 无
 \param[in] Decoder 解码实例句柄
 \param[in] Index  图片索引号。JPEG格式图片，主图的索引号是0，缩略图索引号由1开始；
                   GIF格式图片按照在文件中的存储顺序，索引号由0开始递加
 \param[out] pImgInfo 图片信息
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_DecCommInfo
*/
HI_S32 HI_GO_DecImgInfo(HI_HANDLE Decoder, HI_U32 Index, HIGO_DEC_IMGINFO_S *pImgInfo);

 /** 
 \brief 获取单个图片的数据
 \attention \n
 目标图片属性中的图像格式不支持宏块格式
 \param[in] Decoder 解码实例句柄
 \param[in] Index 图片索引号，索引号从0开始
 \param[in] pImgAttr 目标图片属性，为空表示保持原图片属性
 \param[out] pSurface surface句柄
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_DecCommInfo
*/
HI_S32 HI_GO_DecImgData(HI_HANDLE Decoder, HI_U32 Index, HIGO_DEC_IMGATTR_S *pImgAttr, HI_HANDLE *pSurface);


/** 
 \brief 获取扩展数据
 \attention \n
 Hi3110E不支持此函数。
 \param[in] Decoder 解码实例句柄
 \param[in] DecExtendType 获取扩展数据的类型，目前主要只有JPEG的EXIF信息
 \param[out] ppData 指向扩展数据指针的指针
 \param[out] pLength ppData长度
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_INVHANDLE
 \retval ::HIGO_ERR_UNSUPPORTED

\see \n
::HI_GO_ReleaseDecExtendData
*/
HI_S32 HI_GO_DecExtendData(HI_HANDLE Decoder, HIGO_DEC_EXTENDTYPE_E DecExtendType, HI_VOID **ppData, HI_U32 *pLength);

 /** 
 \brief 释放扩展数据
 \attention \n
 Hi3110E不支持此函数。
 \param[in] Decoder Decoder句柄
 \param[in] DecExtendType 获取扩展数据的类型，目前主要只有JPEG的EXIF信息
 \param[in] pData 指向扩展数据指针
 \retval ::HI_SUCCESS
 \retval ::HIGO_ERR_NULLPTR
 \retval ::HIGO_ERR_UNSUPPORTED

\see \n
::HI_GO_DecExtendData
*/
HI_S32 HI_GO_ReleaseDecExtendData(HI_HANDLE Decoder, HIGO_DEC_EXTENDTYPE_E DecExtendType, HI_VOID *pData);
/** @} */  /*! <!-- API declaration end */
#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_DECODE_H__ */
