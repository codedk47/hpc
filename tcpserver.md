## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
服务端只能通过tcpserver函数回调启动
比如
function(function(){
	$this->io_class = 'my_io_class_test';
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
	- int [$max_connected](tcpserver.md#max_connected);
	- string [$kick_timeout](tcpserver.md#kick_timeout);
	- `/* 方法 */`
	- void [__construct(void)](tcpserver.md#__construct)
	- void __destruct(void) //目前不可用
	- void shutdown(void) //目前不可用
	- void [error_log(string $format [, mixed $args [, mixed $... ]])](tcpserver.md#error_log)
	- void [console_log(string $format [, mixed $args [, mixed $... ]])](tcpserver.md#console_log)
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
class my_io_class_test extends tcpserver_io
{
	function recv() //至少实现这个方法
	{
		$len = $this->read($buf, 1024);
		//一大堆乱七八糟操作
		return TRUE; //保持连接
	}
}
tcpserver(function(){
	$this->io_class = 'my_io_class_test';
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
一般情况下可以先设置当前服务器CPU核心数x2（等等貌似网上都这么说的-，-别打我）
</pre>
```php
tcpserver(function(){
	$this->concurrent_thread = 4;
});
```
#### max_connected
<pre>
这个就比较简单了，这个就是设置最大连接数的，最大不能超过20万（默认1000）
服务端采用的是静态socket表，一次向操作系统申请最大连接数个IOCP结构体并且初始化，做异步处理
抽象类 I/O 下的方法 $this->id() 其实就是这个IOCP在这个socket静态表的索引
</pre>
```php
tcpserver(function(){
	$this->max_connected = 4;
});
```
#### kick_timeout
<pre>
踢出长时间未进行收发操作的连接，0为不开启检测（默认是0）
该方法是异步取消这个IOCP的队列状态，并且做清理，同样会触发抽象类 I/O 下的方法 $this->__destruct() 方法
在不开启的情况下有些连接断线很难检测到，推荐在实际环境下一定要设置一个合适的超时时间不要用TCP的默认2小时
这样可以防止一些僵尸连接，万一最大连接满了真正的用户却连接不上服务端（可以防止某些攻击和网络异常）
踢出超时服务端会有提示但不做服务端记录保存
</pre>
```php
tcpserver(function(){
	$this->kick_timeout = 8; //对于HTTP协议来说8秒钟够用了，8秒不进行收发直接取消这个I/O
});
```
#### __construct
<pre>
服务端启动方法，该方法只能通过tcpserver函数回调启动
</pre>
```php
tcpserver(function(){
	//在这里写一些对服务端操作的控制参数
});
```
#### error_log
<pre>
服务端控制台输出并且记录到日志
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->server()->error_log('新连接被分配到 ID:%d', $this->id());
		return TRUE;
	}
}
```
#### console_log
<pre>
服务端控制台输出，与 error_log() 方法不同的是，console_log() 只显示不做记录
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->server()->console_log('新连接被分配到 ID:%d', $this->id());
		return TRUE;
	}
}
```
