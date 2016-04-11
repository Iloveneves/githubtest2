#ifndef __CMD_LINE_H__
#define __CMD_LINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define FUNC(name) {#name, (void *)name}

/* __cdecl方式是C/C++编译器默认的函数调用约定,
    函数参数按照从右向左的顺序入栈，调用者把参数从左向右弹出栈,函数调用者负责清除栈中的参数 */
typedef struct SYMBOL_S
{    
    char *name;            
    int (*fuc)(int args,...);
}SYMBOL;


typedef char *(*ErrStringFunc)(int ret);

#define ARGS_NUM    (10)

int Cmd_line(SYMBOL list[], int num);

/* 可选的接口 */
void Cmd_ErrString_Func(ErrStringFunc name);

/* 解析shell命令行参数 */
int shell_args(int argc, char * argv[]);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CMD_LINE_H__ */

