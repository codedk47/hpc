## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
</pre>
#### 类摘要
- tcpserver {
	- `/* 属性 */`
	- string [$work_root](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- string [$thread_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置线程上下文
	- string [$io_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置服务器I/O处理类
	- string [$concurrent_thread](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置并发行线程数量
	- string [$local_socket](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 监听本地socket地址
	- int [$max_connected](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 服务端最大连接数
	- string [$kick_timeout](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 踢除超时设置
	- `/* 方法 */`
	- void [tcpserver::__construct](https://github.com/codedk47/hpc/bolb/master/tcpserver.md)(void)
	- void [tcpserver::__destruct](https://github.com/codedk47/hpc/bolb/master/tcpserver.md)(void)
	- void [tcpserver::shutdown](https://github.com/codedk47/hpc/bolb/master/tcpserver.md)(void)
	- void [tcpserver::error_log](https://github.com/codedk47/hpc/bolb/master/tcpserver.md)(string $format [, mixed $args [, mixed $... ]])
	- void [tcpserver::set_ssl(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::add_timer(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::get_online(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::get_connects(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::get_clock(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::send_all(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::send_here(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::send_channel(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::kick_id(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::sync_ctor(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::sync_dtor(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::sync_call(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::cache_add(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::cache_get(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
	- void [tcpserver::cache_del(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- }
