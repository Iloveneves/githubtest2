/* 2008-11-05 */
#ifndef __SQLITE3_PORT_H__
#define __SQLITE3_PORT_H__
typedef struct
{
	void* (*SQLITE3_port_malloc)(int size);
	void* (*SQLITE3_port_realloc)(void * ptr,int size);
	void  (*SQLITE3_port_free)(void * ptr);
	void  (*SQLITE3_port_printf)(char * inf);
}SQLITE3_port_s;
/******
******/
void SQLITE3_API_register_port(SQLITE3_port_s * port);
/******
******/
void SQLITE3_API_debug(int enable);
#endif
