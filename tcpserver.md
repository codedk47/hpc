## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
</pre>
#### 类摘要
- tcpserver {
	- `/* 属性 */`
	- string [$work_root](tcpserver.md#work_root);
	- string [$thread_class](tcpserver.md#thread_class);
	- string [$io_class](tcpserver.md);
	- string [$concurrent_thread](tcpserver.md);
	- string [$local_socket](tcpserver.md);
	- int [$max_connected](tcpserver.md);
	- string [$kick_timeout](tcpserver.md);
	- `/* 方法 */`
	- void [__construct(void)](tcpserver.md)
	- void [__destruct(void)](tcpserver.md)
	- void [shutdown(void)](tcpserver.md)
	- void [error_log(string $format [, mixed $args [, mixed $... ]])](tcpserver.md)
	- void [console_log(string $format [, mixed $args [, mixed $... ]])](tcpserver.md)
	- bool [set_ssl(string $cert, string $key)](tcpserver.md)
	- bool [add_timer(closure $callback[, int $timer])](tcpserver.md)
	- int [get_online(void)](tcpserver.md)
	- array [get_connects(void)](tcpserver.md)
	- int [get_clock(void)](tcpserver.md);
	- int [send_all(string $message)](tcpserver.md);
	- int [send_here(array $id, string $message)](tcpserver.md);
	- int [send_channel(string $channel, string $message)](tcpserver.md);
	- bool [kick_id(int $id)](tcpserver.md);
	- int [sync_ctor(void)](tcpserver.md);
	- void [sync_dtor(int $sync)](tcpserver.md);
	- mixed [sync_call(int $sync, callable $callback)](tcpserver.md);
	- bool [cache_add(string $key, string $value)](tcpserver.md);
	- mixed [cache_get(string $key)](tcpserver.md);
	- bool [cache_del(string $key)](tcpserver.md);
- }
----
#### work_root
<pre>
设置线程上下文当前服务端路径目录或者绝对路径目录（默认当前服务端路径下的work）
注意该文件夹必须存在否则服务端无法启动
</pre>
```php
tcpserver(function(){
	$this->work_root = 'work';
});
```
#### thread_class
<pre>
当服务端启动时会实例化该类，在这里用户可以加入自己对这个线程的上下文控制（默认thread）
</pre>
```php
tcpserver(function(){
	$this->thread_class = 'thread';
});
```
