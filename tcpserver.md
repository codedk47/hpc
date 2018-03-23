## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
</pre>
#### 类摘要
- tcpserver {
	- `/* 属性 */`
	- string [$work_root](tcpserver.md);
	- string [$thread_class](tcpserver.md);
	- string [$io_class](tcpserver.md);
	- string [$concurrent_thread](tcpserver.md);
	- string [$local_socket](tcpserver.md);
	- int [$max_connected](tcpserver.md);
	- string [$kick_timeout](tcpserver.md);
	- `/* 方法 */`
	- void [__construct](tcpserver.md)(void)
	- void [__destruct](tcpserver.md)(void)
	- void [shutdown](tcpserver.md)(void)
	- void [error_log](tcpserver.md)(string $format [, mixed $args [, mixed $... ]])
  - void [console_log](tcpserver.md)(string $format [, mixed $args [, mixed $... ]])
	- bool [set_ssl](tcpserver.md)(string $cert, string $key)
	- bool [add_timer](tcpserver.md)(closure $callback [, int $timer])
	- void [get_online(void)](tcpserver.md);
	- void [get_connects(void)](tcpserver.md);
	- void [get_clock(void)](tcpserver.md);
	- void [send_all(void)](tcpserver.md);
	- void [send_here(void)](tcpserver.md);
	- void [send_channel(void)](tcpserver.md);
	- void [kick_id(void)](tcpserver.md);
	- void [sync_ctor(void)](tcpserver.md);
	- void [sync_dtor(void)](tcpserver.md);
	- void [sync_call(void)](tcpserver.md);
	- void [cache_add(void)](tcpserver.md);
	- void [cache_get(void)](tcpserver.md);
	- void [cache_del(void)](tcpserver.md);
- }
