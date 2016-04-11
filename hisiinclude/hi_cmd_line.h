#ifndef __CMD_LINE_H__
#define __CMD_LINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define FUNC(name) {#name, (void *)name}

/* __cdecl��ʽ��C/C++������Ĭ�ϵĺ�������Լ��,
    �����������մ��������˳����ջ�������߰Ѳ����������ҵ���ջ,���������߸������ջ�еĲ��� */
typedef struct SYMBOL_S
{    
    char *name;            
    int (*fuc)(int args,...);
}SYMBOL;


typedef char *(*ErrStringFunc)(int ret);

#define ARGS_NUM    (10)

int Cmd_line(SYMBOL list[], int num);

/* ��ѡ�Ľӿ� */
void Cmd_ErrString_Func(ErrStringFunc name);

/* ����shell�����в��� */
int shell_args(int argc, char * argv[]);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CMD_LINE_H__ */

