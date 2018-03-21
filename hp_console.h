#ifndef HP_CONSOLE_H
#define HP_CONSOLE_H

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock")

//WINDOWS头文件
#include "WS2tcpip.h"
//#include "WS2ipdef.h"
#include "WinSock2.h"
#include "mswsock.h"
//#include "mstcpip.h"
//#include "wincrypt.h"
//#include "windows.h"

//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//#include "process.h"

//OpenSSL http://www.slproweb.com/products/Win32OpenSSL.html
#include "openssl/ssl.h"
#include "openssl/err.h"

/*
 C/C++ > 常规 > 附加包含目录（PHP源代码目录）
;D:\php-7.1.0-src;D:\php-7.1.0-src\main;D:\php-7.1.0-src\TSRM;D:\php-7.1.0-src\Zend

 C/C++ > 预处理器 > 预处理器定义
;ZEND_DEBUG=0;PHP_WIN32;ZEND_WIN32;ZTS

 连接器 > 输入 > 附加依赖项（PHP运行库）
;D:\php-7.1.0-Win32-VC14-x64\php7embed.lib

*/
#include "php.h"
#include "php_main.h"
#include "php_variables.h"
#include "zend_smart_str.h"
#include "ext/hash/php_hash.h"

#ifndef PHP_WIN32
	#define PHP_CONSOLE_API SAPI_API
#else
	#define PHP_CONSOLE_API
#endif

#ifdef ZTS
ZEND_TSRMLS_CACHE_EXTERN()
#endif

static char const HPC_HEX_CHARSET[16] = "0123456789abcdef";
WSADATA hpc_wsad;

BEGIN_EXTERN_C()

PHP_CONSOLE_API int hpc_init(int argc, char **argv);
PHP_CONSOLE_API void hpc_shutdown(void);

extern PHP_CONSOLE_API sapi_module_struct hpc_module;

typedef struct HPC_STR_T {
	UINT64 len;
	BYTE buf[1];
} HPC_STR;

#define hpc_error hpc_error_dump(__FUNCTION__, __FILE__, __LINE__)
PHP_CONSOLE_API void hpc_error_dump(BYTE *fn, BYTE *file, UINT16 line);
PHP_CONSOLE_API HPC_STR *hpc_str_alloc(UINT64 len);
PHP_CONSOLE_API void hpc_str_free(HPC_STR *str);
PHP_CONSOLE_API void hpc_str_show(HPC_STR *str);
PHP_CONSOLE_API HPC_STR *hpc_str_zstr(zend_string *zstr);
PHP_CONSOLE_API void hpc_localtime_format(BYTE *least_20);
PHP_CONSOLE_API void hpc_log(BOOL error_log, BYTE *format_str, ...);
PHP_CONSOLE_API BOOL hpc_random_bytes(BYTE *buf, UINT len);
PHP_CONSOLE_API BOOL hpc_random_int(PUINT64 uint64p);
PHP_CONSOLE_API BOOL hpc_random_range(PUINT64 uint64p, INT64 min, INT64 max);
PHP_CONSOLE_API void hpc_set_title(BYTE *str);
PHP_CONSOLE_API int hpc_execute_code(BYTE *code, zval *ret);
PHP_CONSOLE_API int hpc_execute_script(BYTE *filename);

END_EXTERN_C()

#endif//HP_CONSOLE_H