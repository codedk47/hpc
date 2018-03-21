#ifndef HP_SERVER_H
#define HP_SERVER_H

#include "hp_console.h"

#define hps_error hpc_error

typedef struct HPS_HASH_NODE_T {
	UINT64 h;
	void *p;
	struct HPS_HASH_NODE_T *prev;
	struct HPS_HASH_NODE_T *next;
} HPS_HASH_NODE;

typedef struct HPS_SYNC_LIST_T {
	volatile UINT32 count;
	CRITICAL_SECTION cs;
	HPS_HASH_NODE hn;
} HPS_SYNC_LIST;

typedef struct HPS_IOCP_BASE_T {
	HANDLE h;
	UINT16 concurrent_thread;
} HPS_IOCP_BASE;

typedef struct HPS_IOCP_SOCKET_T {
	OVERLAPPED io;
	SOCKET s;
	BYTE ai[88];
	UINT32 recv_len;
	WSABUF recv_buf;
	UINT32 recv_mod;
	SSL *ssl;
	UINT64 channel;
	HPS_HASH_NODE ctx;
	HPS_HASH_NODE node;
	CRITICAL_SECTION cs;
	volatile time_t lt;
} HPS_IOCP_SOCKET;

typedef struct HPS_IOCP_LISTEN_T {
	BYTE ai_bin[28];
	BYTE ai_len;
	int ai_family;
	int ai_v6only;
	SOCKET s;
	HANDLE h;
	UINT16 concurrent_thread;
	HANDLE *t;
	LPFN_ACCEPTEX acceptex;	//异步接受
	LPFN_DISCONNECTEX disconnectex; //断开连接
	//LPFN_GETACCEPTEXSOCKADDRS getacceptexsockaddrs;
	DWORD bytes;
	//OPENSSL
	SSL_CTX *ssl;
	//函数指针
	int(*accept)(HPS_IOCP_SOCKET *iocp_socket);
	BOOL(*recv)(HPS_IOCP_SOCKET *iocp_socket);
	int(*peek)(HPS_IOCP_SOCKET *iocp_socket, char *str, int len);
	int(*read)(HPS_IOCP_SOCKET *iocp_socket, char *str, int len);
	int(*write)(HPS_IOCP_SOCKET *iocp_socket, char *str, int len);
	BOOL(*sendfile)(HPS_IOCP_SOCKET *iocp_socket, char *filename);
	BOOL(*end)(HPS_IOCP_SOCKET *iocp_socket);
	BOOL(*reuse)(HPS_IOCP_SOCKET *iocp_socket);
	UINT32 max_connected;
	HPS_IOCP_SOCKET *heap;
} HPS_IOCP_LISTEN;

typedef struct HPS_TIMER_CALL_T {
	HPS_HASH_NODE node;
	HANDLE h;
	LPTHREAD_START_ROUTINE f;
	LPVOID p;
	time_t t;
} HPS_TIMER_CALL;

//HN
#define HPS_HN_FOREACH(hn) \
	HPS_HASH_NODE *hn_foreach = hn;\
	while(hn_foreach = hn_foreach->next)
#define HPS_HN_FOREACH_H (hn_foreach->h)
#define HPS_HN_FOREACH_P (hn_foreach->p)
void hps_hn_ctor(HPS_HASH_NODE *hn);
void hps_hn_dtor(HPS_HASH_NODE *hn);
HPS_HASH_NODE *hps_hn_new(UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_hn_node_add_top(HPS_HASH_NODE *hn, HPS_HASH_NODE *node);
HPS_HASH_NODE *hps_hn_node_add_end(HPS_HASH_NODE *hn, HPS_HASH_NODE *node);
HPS_HASH_NODE *hps_hn_node_remove(HPS_HASH_NODE *hn, HPS_HASH_NODE *node);
void hps_hn_node_free(HPS_HASH_NODE *hn, HPS_HASH_NODE *node);
HPS_HASH_NODE *hps_hn_find(HPS_HASH_NODE *hn, UINT64 h);
HPS_HASH_NODE *hps_hn_add_top(HPS_HASH_NODE *hn, UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_hn_add_end(HPS_HASH_NODE *hn, UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_hn_remove(HPS_HASH_NODE *hn, UINT64 h);
HPS_HASH_NODE *hps_hn_insert(HPS_HASH_NODE *hn, UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_hn_update(HPS_HASH_NODE *hn, UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_hn_replace(HPS_HASH_NODE *hn, UINT64 h, UINT64 l);
BOOL hps_hn_delete(HPS_HASH_NODE *hn, UINT64 h);

//SL
#define HPS_SL_FOREACH_ENTER(sl) \
	HPS_HASH_NODE *sl_foreach = &(sl)->hn;\
	EnterCriticalSection(&(sl)->cs);\
	while(sl_foreach = sl_foreach->next)
#define HPS_SL_FOREACH_LEAVE(sl) \
	LeaveCriticalSection(&(sl)->cs);
#define HPS_SL_FOREACH_H (sl_foreach->h)
#define HPS_SL_FOREACH_P (sl_foreach->p)
void hps_sl_ctor(HPS_SYNC_LIST *sl);
void hps_sl_dtor(HPS_SYNC_LIST *sl);
HPS_HASH_NODE *hps_sl_node_add_end(HPS_SYNC_LIST *sl, HPS_HASH_NODE *node);
HPS_HASH_NODE *hps_sl_node_remove(HPS_SYNC_LIST *sl, HPS_HASH_NODE *node);
void hps_sl_node_free(HPS_SYNC_LIST *sl, HPS_HASH_NODE *node);
HPS_HASH_NODE *hps_sl_find(HPS_SYNC_LIST *sl, UINT64 h);
HPS_HASH_NODE *hps_sl_add_end(HPS_SYNC_LIST *sl, UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_sl_update(HPS_SYNC_LIST *sl, UINT64 h, UINT64 l);
HPS_HASH_NODE *hps_sl_replace(HPS_SYNC_LIST *sl, UINT64 h, UINT64 l);
BOOL hps_sl_delete(HPS_SYNC_LIST *sl, UINT64 h);

//SOCKET
BOOL hps_socket_set_addr_exclusive(SOCKET s, BOOL b);
BOOL hps_socket_set_addr_reuse(SOCKET s, BOOL b);
BOOL hps_socket_set_nonblock(SOCKET s, BOOL b);
BOOL hps_socket_set_timeout(SOCKET s, UINT32 msec);
BOOL hps_socket_wait_recv(SOCKET s, UINT16 sec);

//IOCP
int hps_iocp_base(HPS_IOCP_BASE *iocp_base, UINT16 concurrent_thread);
int hps_iocp_listen(HPS_IOCP_BASE *iocp_base, HPS_IOCP_LISTEN *iocp_listen, BYTE *addr, BYTE *port, UINT32 max_connected);
int hps_iocp_listen_openssl(HPS_IOCP_LISTEN *iocp_listen, BYTE *cert, BYTE *key);
BOOL hps_iocp_thread_startup(HPS_IOCP_LISTEN *iocp_listen, LPTHREAD_START_ROUTINE f);
void hps_iocp_thread_wait(HPS_IOCP_LISTEN *iocp_listen);
void hps_iocp_queue_wait(HPS_IOCP_LISTEN *iocp_listen, HPS_IOCP_SOCKET **iocp_socket, WSANETWORKEVENTS *iocp_event);
int hps_iocp_socket_ip_info(HPS_IOCP_SOCKET *iocp_socket, BYTE *info, UINT64 least_48);
void hps_iocp_socket_ip_addr(HPS_IOCP_SOCKET *iocp_socket, BYTE *hex32b);
void hps_iocp_socket_ip_port(HPS_IOCP_SOCKET *iocp_socket, BYTE *hex4b);
BOOL hps_iocp_socket_wait_event(HPS_IOCP_SOCKET *iocp_socket, BYTE bit, UINT16 sec);

//TIMER
HPS_TIMER_CALL *hps_timer_add(HPS_SYNC_LIST *timer, LPTHREAD_START_ROUTINE f, LPVOID p, time_t t);
void hps_timer_clock(HPS_SYNC_LIST *timer, volatile time_t *clock);

#endif//HP_SERVER_H