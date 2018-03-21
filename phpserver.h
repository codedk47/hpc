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

//PHPSERVERȫ�ֶ���
zend_module_entry	phpserver;						//�����ģ��
zend_class_entry	*phpserver_ce_thread;			//�߳���������
zend_class_entry	*phpserver_ce_tcpserver;		//tcpserver��
//zend_class_entry	*phpserver_ce_udpserver;		//udpserver��
zend_class_entry	*phpserver_ce_io;				//�߼�������
void				*phpserver_ls;					//��������Դ
zval				*phpserver_zval_object;			//�����ʵ������
HPS_IOCP_BASE		phpserver_iocp_base;			//IOCP����
HPS_IOCP_LISTEN		phpserver_iocp_listen;			//IOCP����
CRITICAL_SECTION	phpserver_sync;					//ͬ����
HPS_SYNC_LIST		phpserver_connects;				//���ӱ�
HPS_SYNC_LIST		phpserver_cache;				//�����
HPS_SYNC_LIST		phpserver_timer;				//��ʱ����
volatile time_t		phpserver_clock;				//��ǰʱ���뼶��������ʱ������Ч��
zend_class_entry	*phpserver_user_ce_thread;		//�û��߳���������
zend_class_entry	*phpserver_user_ce_io;			//�û��߼�������

//PHPSERVER�̶߳���
ZEND_DECLARE_MODULE_GLOBALS(phpserver);
ZEND_BEGIN_MODULE_GLOBALS(phpserver)
UINT32				tid;							//�߳�ID
HPC_STR				*tob;							//�߳����������
zval				zobj_thread;					//��������
zval				zobj_io;						//�߼�������
HPS_IOCP_SOCKET		*iocp_socket;					//��ǰIOCP
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