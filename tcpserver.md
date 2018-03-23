## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
服务端只能通过tcpserver函数回调启动
比如
function(function(){
	$this->io_class = 'my_io_class_name';
	$this->concurrent_thread = 32;
	$this->local_socket = 'tcp://0.0.0.0:80';
});
</pre>
#### 类摘要
- tcpserver {
	- `/* 属性 */`
	- string [$work_root](tcpserver.md#work_root);
	- string [$thread_class](tcpserver.md#thread_class);
	- string [$io_class](tcpserver.md#io_class);
	- string [$local_socket](tcpserver.md#local_socket);
	- int [$concurrent_thread](tcpserver.md#concurrent_thread);
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
#### io_class
<pre>
服务端 I/O 操作类，相当于每个与服务端建立的连接都是这个类实例化后的对象
用户必须实现一个自己的 I/O 类，并且必须继承 tcpserver_io 抽象类
</pre>
```php
class my_io_class_name extends tcpserver_io
{
	function recv() //至少实现这个方法
	{
		//一大堆乱七八糟操作
		return TRUE; //保持连接
	}
}
tcpserver(function(){
	$this->io_class = 'my_io_class_name';
});
```
#### local_socket
<pre>
服务端绑定的本地socket地址，注意协议用ssl服务端将判断是否加载证书其他请用tcp协议(默认tcp://*:8014)
</pre>
```php
tcpserver(function(){
	$this->local_socket = 'tcp://0.0.0.0:80';
});
```
#### concurrent_thread
<pre>
服务端并发线程，（默认是当前服务器CPU核心数,最大不能超过当前服务器CPU核心数x4）
设置这个并发数量有一定技巧
对于复杂逻辑同步操作比较多可以设置最大线程
对于简单逻辑可以设置当前服务器CPU核心数
比如你有一个收发文件的逻辑或者等待其他计算结果的逻辑需要很多时间，
同时又必须保证其他用户可以与服务端进行操作，当然线程最大化了，不然（CPU挂起也是浪费）
还有需要根据服务端I/O密集程度和数据包大小的收发量来考量最终设置结果，简单缺会影响到并发量的一个设置
</pre>
```php
tcpserver(function(){
	$this->concurrent_thread = 4;
});
```
