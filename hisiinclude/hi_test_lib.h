/*******************************************************************************
*             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: 
* Description:
*
* History:
* Version   Date              Author    DefectNum       Description
  0.1       2008.08.11        qianli
  1.0       2008.08.15        qianli    更改了文件导入功能的文件内容格式
  1.0.1       2008.08.22        qianli    Standby更名为Manual模式。
*******************************************************************************/
#ifndef _CONFIG_HI_TEST_LIB_H_
#define _CONFIG_HI_TEST_LIB_H_

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */


#include <unistd.h>  
#include <stdlib.h> //system, exit, sleep etc.
#include <stdio.h>
typedef enum
{
    TEST_MENU,
    ALL_CASE = TEST_MENU,
    AUTO_CASE,
    MANU_CASE,
}CASE_ATTR_t;
typedef enum
{
    UN_SPECIAL_CASE,
    WITH_SPECIAL_CASE,    
    SPECIAL_CASE_FINDED,
}SPECIAL_CASE_t;

typedef struct TESTCASE_S
{    
    char *name;            
    void (*fuc)(void);
    CASE_ATTR_t attr; 
}TESTOBJ;



#define AUTO_CASE(name) {#name, name, AUTO_CASE}
#define MANU_CASE(name) {#name, name, MANU_CASE}
#define MENU(name)      {#name, name, TEST_MENU}

#define SHOW (65271)
#define BACK (83421)
#define QUIT (31415)
#define RUN  (65225)
#define SPECIAL (104687)

extern unsigned int g_Magic;
extern unsigned int g_FailedCase;
extern unsigned int g_PassedCase;
extern unsigned char g_isPassed; /* 0：失败，1：成功*/
extern SPECIAL_CASE_t g_isSpecial;
extern unsigned char g_TargetCase[100];
extern unsigned char g_RunAll;
extern unsigned char g_StandBy;
extern CASE_ATTR_t g_CaseType;

extern FILE *g_LogFp;  /* 记录错误日志 */

int HI_Get_Input();
void HI_Test_Puppet(void);
void HI_Test_isStandBy(void);
void HI_Quit(void);


typedef char *(*ErrStringGet)(int ret);
typedef void (*clear)(void);
extern ErrStringGet g_GetErrString;
extern volatile clear g_ClearFunc;

/* 可选的函数返回值打印回调接口 */
void HI_ErrString_Register(ErrStringGet name);

/* 用例结束后的清理函数 */
void HI_ClearFunc_Register(clear func);


/* 解析传入的命令行参数 */
int HI_Cmd_Analyse(int argc, char * argv[]);


#define STR(misc) #misc


#define WAIT(nextstep...)\
    do{\
        if(1 == g_StandBy)\
        {\
            int passed;\
            printf("\033[0;35m""press enter to contunue.....");\
            printf(#nextstep"\n\033[0m");\
            read(0, &passed, 4);\
        }\
        else if(0 == g_StandBy)\
        {\
            printf("\033[0;35m""wait for 3 seconds.....""\n\033[0m");\
            sleep(3);\
        }\
    }while(0)


#define HI_PASSED()\
do{\
    if(!g_isPassed)\
    {\
        g_isPassed = 1;\
        HI_FAILED();\
    }\
    if(g_ClearFunc)\
        g_ClearFunc();\
    printf("\033[0;32m""case: %s passed\n""\033[0m", __FUNCTION__);\
    g_PassedCase++;\
}while(0)

#define HI_FAILED()\
do{\
    if(g_ClearFunc)\
        g_ClearFunc();\
    fprintf(g_LogFp, "case: %s failed!!!\n", __FUNCTION__);\
    fflush(g_LogFp);\
    printf("\033[0;31m""case: %s failed!!!\n""\033[0m", __FUNCTION__);\
    g_FailedCase++;\
    return;\
}while(0)

#define HI_ISPASSED()\
do{\
    if(1 == g_StandBy)\
    {\
        int passed = 0;\
        printf("\033[0;35m""passed or not?(y/n)\n""\n\033[0m");\
        read(STDIN_FILENO, &passed, 4);\
        if((passed & 0xff) != 'y') \
            g_isPassed = 0;\
    }\
    else\
        g_isPassed = 0;\
}while(0)


#define HI_SHOW_MENU(TestCase)\
do{\
    if(0 == g_RunAll)\
    {\
        int num = sizeof(TestCase)/sizeof(TestCase[0]);\
        int i = 0;\
        printf("\n*******************************************%s*******************************************\n", STR(TestCase));\
        while (i < num)\
        {\
            printf("*%-4d %-45s\t", i+1, TestCase[i].name);\
            i++;\
            if(0 == i%2)\
                printf("\n");\
        }\
        printf("\n*******************************************%s*******************************************\n", STR(TestCase));\
    }\
}while(0)

#define LAST_ITEM_NUM (2)
#define RUN_ALL_CASE (num-LAST_ITEM_NUM)



#define HI_CHECK_INPUT(TestCase, input)\
{\
    int num = sizeof(TestCase)/sizeof(TestCase[0]);\
    if ( (input) <= 0 || (input) > num)\
    {\
        if(SHOW == input)\
            {HI_SHOW_MENU(TestCase);    continue;}\
        else if(QUIT == input)\
            {input = num-1; exit(0);}\
        else if(BACK == input)\
            {input = num-2; printf("back to up\n");     break;}\
        else if(RUN == input)\
            {input = RUN;}\
        else if(SPECIAL == input)\
            {input = RUN;}\
        else\
            {printf("Input error\n");   continue;}\
    }\
}

#define HI_IMPLEMENT(TestCase)\
{\
    int input = 0;\
    if(g_Magic > 0 && g_isSpecial != SPECIAL_CASE_FINDED)\
        {input = RUN;}\
    else\
        input = HI_Get_Input();\
    HI_CHECK_INPUT(TestCase, input);\
    if (RUN == (input))\
    {\
        g_RunAll = 1;\
        g_Magic++;\
        HI_RUN_ALL(TestCase);\
        if(g_Magic > 0)\
            break;\
    }\
    else\
    {\
    printf("\n%s:\n", TestCase[input-1].name);\
    TestCase[input-1].fuc();\
    }\
}

#define LAST_ITEM \
    {"Manual or Auto?  1:Manual 0:Auto", HI_Test_isStandBy,TEST_MENU},\
    {"Quit", HI_Quit,TEST_MENU}
            


#define HI_RUN_ALL(TestCase)\
do{\
    int num = sizeof(TestCase)/sizeof(TestCase[0]);\
    int i = 0;\
    for (i = 0; i < RUN_ALL_CASE; i++)\
    {\
        if(UN_SPECIAL_CASE == g_isSpecial)\
        {\
            if(TEST_MENU == TestCase[i].attr || ALL_CASE == g_CaseType || g_CaseType == TestCase[i].attr)\
            {\
                printf("\n%s:\n", TestCase[i].name);\
                TestCase[i].fuc();\
                printf("\n\n");\
            }\
        }\
        else if(WITH_SPECIAL_CASE == g_isSpecial)\
        {\
            if(0 == strcmp(TestCase[i].name, g_TargetCase))\
            {\
                g_isSpecial = SPECIAL_CASE_FINDED;\
                TestCase[i].fuc();\
                printf("\n\n");\
                break;\
            }\
            else if(TEST_MENU == TestCase[i].attr)\
                TestCase[i].fuc();\
        }\
        else if(SPECIAL_CASE_FINDED == g_isSpecial)\
            {break;}\
    }\
    g_Magic--;\
}while(0)



#define expect(obj, expect)\
do{\
    HI_S32 ret = obj;\
    if ((expect) != (ret))\
    {\
        fprintf(g_LogFp, "\ncompare error at %d line! %s != %s actual value is %s\n", __LINE__, #obj, #expect, g_GetErrString(ret));\
        fflush(g_LogFp);\
        printf("\033[0;31m""compare error at %d line! %s != %s actual value is %s\n""\033[0m", __LINE__, #obj, #expect,  g_GetErrString(ret));\
        HI_FAILED();\
    }\
    else\
        printf("%s == %s at %d line\n", #obj, #expect, __LINE__);\
}while(0)




#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_CONFIG_HI_TEST_LIB_H_*/
