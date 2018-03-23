## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
</pre>
#### 类摘要
- tcpserver {
	- `/* 属性 */`
	- string [$work_root](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置线程上下文目录（当前服务端路径下的文件夹）
	- string [$thread_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置线程上下文
	- string [$io_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置服务器I/O处理类
	- string [$concurrent_thread](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 设置并发行线程数量
	- string [$local_socket](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 监听本地socket地址
	- int [$max_connected](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 服务端最大连接数
	- string [$kick_timeout](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 踢除超时设置
	- `/* 方法 */`
	- void [tcpserver::__construct(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md); 服务端启动函数
- }
