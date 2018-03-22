## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿修改该实例化后的属性值（可以多线程读）
</pre>
#### 类摘要
* tcpserver {
- `/* 属性 */`
- string [$work_root](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- string [$thread_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- string [$io_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- string [$concurrent_thread](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- string [$local_socket](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- int [$max_connected](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- string [$kick_timeout](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
- `/* 方法 */`
- void [tcpserver::__construct(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
* }
