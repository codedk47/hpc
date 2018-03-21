#ifndef PHPSERVER_H
#define PHPSERVER_H

#define PHPSERVER_VERSION "1.1.2.0"

#include "hp_console.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "process.h"

#include "zend_interfaces.h"
#include "zend_closures.h"
#include "ext/standard/php_standard.h"
#include "formatted_print.c"

#include "hp_server.h"

typedef struct phpserver_timer_caller_t {
	zend_function fun;
	zend_fcall_info fci;
	zend_fcall_info_cache fcc;
	zval ret;
} phpserver_timer_caller;

//PHPSERVER全局定义
zend_module_entry	phpserver;						//服务端模块
zend_class_entry	*phpserver_ce_thread;			//线程上下文类
zend_class_entry	*phpserver_ce_tcpserver;		//tcpserver类
//zend_class_entry	*phpserver_ce_udpserver;		//udpserver类
zend_class_entry	*phpserver_ce_io;				//逻辑处理类
void				*phpserver_ls;					//主进程资源
zval				*phpserver_zval_object;			//服务端实例对象
HPS_IOCP_BASE		phpserver_iocp_base;			//IOCP配置
HPS_IOCP_LISTEN		phpserver_iocp_listen;			//IOCP监听
CRITICAL_SECTION	phpserver_sync;					//同步锁
HPS_SYNC_LIST		phpserver_connects;				//连接表
HPS_SYNC_LIST		phpserver_cache;				//缓存表
HPS_SYNC_LIST		phpserver_timer;				//定时器表
volatile time_t		phpserver_clock;				//当前时钟秒级（启动定时器后生效）
zend_class_entry	*phpserver_user_ce_thread;		//用户线程上下文类
zend_class_entry	*phpserver_user_ce_io;			//用户逻辑处理类

//PHPSERVER线程定义
ZEND_DECLARE_MODULE_GLOBALS(phpserver);
ZEND_BEGIN_MODULE_GLOBALS(phpserver)
UINT32				tid;							//线程ID
HPC_STR				*tob;							//线程输出缓冲区
zval				zobj_thread;					//上下文类
zval				zobj_io;						//逻辑处理类
HPS_IOCP_SOCKET		*iocp_socket;					//当前IOCP
HashTable			*php_context_resolve;			//php_context_resolve
HashTable			*php_context_resources;			//php_context_resources
ZEND_END_MODULE_GLOBALS(phpserver)
#define PHPSERVER_G(q) ZEND_MODULE_GLOBALS_ACCESSOR(phpserver, q)
#define PHPSERVER_MAX_RW 0x7fffffff
#define PHPSERVER_MAX_TOB 0xffff
#define PHPSERVER_THIS_IOCP PHPSERVER_G(iocp_socket)
#define PHPSERVER_THIS_CTX (&PHPSERVER_THIS_IOCP->ctx)
#define PHPSERVER_THIS_CTX_ADD(id, size) hps_hn_add_end(PHPSERVER_THIS_CTX, id, size)
#define PHPSERVER_THIS_CTX_GET(id) hps_hn_find(PHPSERVER_THIS_CTX, id)
#define PHPSERVER_GC_FULL_COUNT(zv) \
	GC_REFCOUNT(Z_COUNTED_P(zv)) = 0x7ffffff0
#define PHPSERVER_GC_NULL_TYPE(zv) \
	PHPSERVER_GC_FULL_COUNT(zv);\
	GC_TYPE_INFO(Z_COUNTED_P(zv)) = 0
#define PHPSERVER_GC_KEEP_ZVAL(dst, src) \
	PHPSERVER_GC_FULL_COUNT(src);\
	Z_COUNTED_P(dst) = Z_COUNTED_P(src);\
	Z_TYPE_INFO_P(dst) = Z_TYPE_INFO_P(src)
#include "php_context.h"

#define TCPSERVER_THIS_PEEK(buf, len) phpserver_iocp_listen.peek(PHPSERVER_THIS_IOCP, buf, len)
#define TCPSERVER_THIS_READ(buf, len) phpserver_iocp_listen.read(PHPSERVER_THIS_IOCP, buf, len)
#define TCPSERVER_THIS_WRITE(buf, len) phpserver_iocp_listen.write(PHPSERVER_THIS_IOCP, buf, len)

#endif//PHPSERVER_H