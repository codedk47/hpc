#ifndef TCPSERVER_HTTP_H
#define TCPSERVER_HTTP_H

#include "phpserver.h"
#include "tcpserver_io.h"
//#include "php_open_temporary_file.h"

zend_module_entry tcpserver_http;
zend_class_entry *tcpserver_http_ce;
zend_class_entry *tcpserver_http_uploaded_file_ce;

typedef struct tcpserver_http_req_content_t {
	UINT64 length;
	UINT64 offset;
	BYTE buffer[1];
} tcpserver_http_req_content;

typedef struct tcpserver_http_req_t {
	UINT16 length;
	UINT32 method;
	UINT16 url_len;
	BYTE *url_str;
	BYTE *ver_str;
	BYTE *headers;
	HPS_HASH_NODE *ctx;
	void *extend;
	tcpserver_http_req_content *content;
	BYTE buffer[1];
} tcpserver_http_req;

typedef struct tcpserver_http_res_t {
	UINT64 status;
	UINT16 offset;
	UINT16 length;
	BYTE buffer[1];
} tcpserver_http_res;

ZEND_DECLARE_MODULE_GLOBALS(tcpserver_http);
ZEND_BEGIN_MODULE_GLOBALS(tcpserver_http)
tcpserver_http_req *req;
tcpserver_http_res *res;
ZEND_END_MODULE_GLOBALS(tcpserver_http)
#define TCPSERVER_HTTP_G(q) ZEND_MODULE_GLOBALS_ACCESSOR(tcpserver_http, q)
#define TCPSERVER_HTTP_CTX_REQ 0x10
#define TCPSERVER_HTTP_CTX_EXTEND 0x11
#define TCPSERVER_HTTP_GET_REQ (TCPSERVER_HTTP_G(req) = PHPSERVER_THIS_CTX_GET(TCPSERVER_HTTP_CTX_REQ)->p)
#define TCPSERVER_HTTP_REQ_EXTEND(req, len) (req->extend = (req->ctx = PHPSERVER_THIS_CTX_ADD(TCPSERVER_HTTP_CTX_EXTEND, len)) ? req->ctx->p : NULL)

#define TCPSERVER_HTTP_MAX_IHB 8192 //最大输入头缓冲区
#define TCPSERVER_HTTP_MAX_OHB 6144 //最大输出头缓冲区
#define TCPSERVER_HTTP_METHOD_GET 542393671
#define TCPSERVER_HTTP_METHOD_PUT 542397776
#define TCPSERVER_HTTP_METHOD_POST 1414745936
#define TCPSERVER_HTTP_METHOD_HEAD 1145128264
#define TCPSERVER_HTTP_METHOD_TRACE 1128354388
#define TCPSERVER_HTTP_METHOD_DELETE 1162626372
#define TCPSERVER_HTTP_METHOD_OPTIONS 1230262351
#define TCPSERVER_HTTP_METHOD_CONNECT 1313754947
#define TCPSERVER_HTTP_VERSION_11(req) (*(UINT64*)req->ver_str == 3543824036068086856)

#define TCPSERVER_HTTP_RES_SEND			(1<<0)
#define TCPSERVER_HTTP_RES_CONNECTION	(1<<1)
#define TCPSERVER_HTTP_RES_LENGTH		(1<<2)
#define TCPSERVER_HTTP_RES_TYPE			(1<<3)
#define TCPSERVER_HTTP_RES_GZIP			(1<<4)
#define TCPSERVER_HTTP_RES_ETAG			(1<<5)
#define TCPSERVER_HTTP_RES_CHUNKED		(1<<6)

int tcpserver_http_req_ctor(tcpserver_http_req *req);
void tcpserver_http_req_dtor(tcpserver_http_req *req);
BYTE *tcpserver_http_req_find(tcpserver_http_req *req, BYTE *key, UINT64 len, UINT16 *lp);
BOOL tcpserver_http_req_keep_alive(tcpserver_http_req *req);
UINT64 tcpserver_http_req_content_length(tcpserver_http_req *req);
BOOL tcpserver_http_req_gzip_on(tcpserver_http_req *req);
BOOL tcpserver_http_req_etag_eq(tcpserver_http_req *req, UINT64 etag);
BOOL tcpserver_http_req_expect(tcpserver_http_req *req);

void tcpserver_http_res_init();
BOOL tcpserver_http_res_status(BYTE *str, UINT64 len, BOOL clear);
BOOL tcpserver_http_res_status_100(BOOL clear);
BOOL tcpserver_http_res_status_101(BOOL clear);
BOOL tcpserver_http_res_status_304(BOOL clear);
BOOL tcpserver_http_res_append(UINT64 status, BYTE *str, UINT64 len);
BOOL tcpserver_http_res_appendf(UINT64 status, BYTE *str, ...);
//BOOL tcpserver_http_res_remove(BYTE *str, UINT64 len);
//BOOL tcpserver_http_res_replace(BYTE *str, UINT64 len);
BOOL tcpserver_http_res_length(UINT64 length);
BOOL tcpserver_http_res_keep_alive(BOOL keep);
BOOL tcpserver_http_res_type(BYTE *str, UINT64 len);
BOOL tcpserver_http_res_gzip();
BOOL tcpserver_http_res_etag(UINT64 etag);
BOOL tcpserver_http_res_chunked();
BOOL tcpserver_http_res_send();

BOOL tcpserver_http_send_403();
BOOL tcpserver_http_send_404();
BOOL tcpserver_http_send_500();
BOOL tcpserver_http_send_chunked(BYTE *str, UINT32 len);

static zval *content_add_auto_index(zval *arr, BYTE *key_str, BYTE key_len)
{
	zend_ulong all = php_count_recursive(arr, COUNT_RECURSIVE);
	sapi_module.treat_data(PARSE_STRING, estrndup(key_str, key_len), arr);
	if (php_count_recursive(arr, COUNT_RECURSIVE) == all)
	{
		return NULL;
	}
	zval *ptr = arr;
	while (Z_TYPE_P(ptr) == IS_ARRAY)
	{
		zend_hash_internal_pointer_end(Z_ARRVAL_P(ptr));
		ptr = zend_hash_get_current_data(Z_ARRVAL_P(ptr));
	}
	zval_ptr_dtor(ptr);
	return ptr;
}

#endif//TCPSERVER_HTTP_H