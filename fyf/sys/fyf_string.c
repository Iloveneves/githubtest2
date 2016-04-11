#include <api/libc/string.h>
#include <api/libc/time.h>

int memcmp(const void * buf1,const void * buf2,unsigned int len)
{
	return libc_memcmp(buf1, buf2, len);
}
int strncmp(const char *dest,const char *src,int len)
{
	char dest1[len+1],src1[len+1];
	libc_memcpy(dest1,dest,len);
	libc_memcpy(src1,src,len);
	dest1[len]=0;
	src1[len]=0;
	return libc_strcmp(dest1,src1);
}
unsigned long atoi(char * str)
{
	return libc_atoi(str);
}

struct tm *localtime(const time_t *timer)
{
	struct tm res;

	memset(&res,0,sizeof(struct tm));
	return gmtime_r(timer,&res);
}
int isdigit(int c)
{
	if(c>='0' && c<= '9')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isspace(int c)
{
	if(((c) == ' ') || ((c) == '\r') || ((c) == '\n') || \
				((c) == '\t') || ((c) == '\f'))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isxdigit(int c)
{
	if(isdigit(c) || ((c) >= 'a' && (c) <= 'f') || \
				((c) >= 'A' && (c) <= 'F'))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
#define	isupper(ch)	((ch) >= 'A' && (ch) <= 'Z')
#define	islower(ch)	((ch) >= 'a' && (ch) <= 'z')
#define	isalpha(ch)	(isupper(ch) || islower(ch))
int isalnum(int c)
{
	if(isalpha(c) || isdigit(c))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
int toupper(int c)
{
	if(c>='a' && c<='z')
	{
		return (c-32);
	}
	else
	{
		return c;
	}	
}
int tolower(int c)
{
	if(c>='A' && c<='Z')
	{
		return (c+32);
	}
	else
	{
		return c;
	}	
}




