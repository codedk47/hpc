#ifndef TCPSERVER_WS_H
#define TCPSERVER_WS_H

#include "phpserver.h"
#include "tcpserver_http.h"

zend_module_entry tcpserver_ws;
zend_class_entry *tcpserver_ws_ce;
/*
Frame format:
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
*/
typedef struct tcpserver_ws_frame_t {
	UINT64 offset;
	UINT64 length;
	BYTE *buffer;
	BYTE *masking_key;
	HPS_HASH_NODE *node_buffer;
	struct head_info_t {
		BYTE opcode : 4;
		BYTE rsv3 : 1;
		BYTE rsv2 : 1;
		BYTE rsv1 : 1;
		BYTE fin : 1;
		BYTE payload_len : 7;
		BYTE mask : 1;
		BYTE buffer[129];
	} hi; //×¢ÒâÎ»ÓòÅÅÁÐË³Ðò
} tcpserver_ws_frame;

tcpserver_ws_frame tcpserver_ws_null_frame;

#define TCPSERVER_WS_CTX_FRAME 0x20
#define TCPSERVER_WS_CTX_BUFFER 0x21
#define TCPSERVER_WS_MAX_IFB 0xffff
//#define TCPSERVER_WS_MAX_IFBA (TCPSERVER_WS_MAX_IFB * 2)
#define TCPSERVER_WS_FRAME ((tcpserver_ws_frame*)TCPSERVER_HTTP_G(req)->extend)

BYTE tcpserver_ws_pack_hi(BYTE *out, BYTE fin, BYTE opcode, UINT64 length);

#endif//TCPSERVER_WS_H