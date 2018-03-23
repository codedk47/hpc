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
	- void [__construct(void)](tcpserver.md)
	- void [__destruct(void)](tcpserver.md)
	- void [shutdown(void)](tcpserver.md)
	- void [error_log(string [, ...])](tcpserver.md)
	- void [console_log(string [, ...])](tcpserver.md)
	- bool [set_ssl(string, string)](tcpserver.md)
	- bool [add_timer(closure [, int])](tcpserver.md)
	- int [get_online(void)](tcpserver.md)
	- array [get_connects(void)](tcpserver.md)
	- int [get_clock(void)](tcpserver.md);
	- int [send_all(string)](tcpserver.md);
	- int [send_here(array, string)](tcpserver.md);
	- int [send_channel(string, string)](tcpserver.md);
	- bool [kick_id(void)](tcpserver.md);
	- int [sync_ctor(void)](tcpserver.md);
	- void [sync_dtor(int)](tcpserver.md);
	- mixed [sync_call(callable)](tcpserver.md);
	- bool [cache_add(void)](tcpserver.md);
	- mixed [cache_get(void)](tcpserver.md);
	- bool [cache_del(void)](tcpserver.md);
- }
