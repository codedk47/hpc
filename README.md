#### Hypertext Preprocessor Console（hpc）
<pre>
该项目暂时不开源有兴趣可以加Q群（456357348）技术讨论吹牛B等-_-b
目前服务端基础功能已经过测试免费提供使用，希望能得到大家的想法与支持
</pre>
#### Hypertext Preprocessor Server（hps）
<pre>
hps 是为了让 php 可以编写高性能的服务端而使用纯c语言开发的
基于 Windows 平台 IOCP 网络事件驱动，多线程+非阻塞模式
采用 PHP7.2.3 和 OpenSSL1.1.0g 源代码编译
如果觉得好用请给我github上点个🌟
</pre>
- [下载 hpc-1.1.3-VC15-x64.zip](https://github.com/codedk47/hpc/raw/master/hpc-1.1.3-VC15-x64.zip)
#### PHP extend declared classes
- [thread](thread.md) (标准线程上下文类，可继承)
- [tcpserver](tcpserver.md) (TCP服务端主要类，不可继承)
- [tcpserver_io](tcpserver_io.md) (I/O 类，必须被继承)
- [tcpserver_http](tcpserver_http.md) (HTTP 类继承 I/O 类，必须被继承)
- [tcpserver_ws](tcpserver_ws.md) (WebSocket 类继承 HTTP 类，必须被继承)
- 以后将加入更多的类和方法
#### 开始编写一个简单echo服务端
```php
<?php
//服务端内部所有 "tcpserver_" 开头的类都继承该类的方法 比如 HTTP 类
class demo_io extends tcpserver_io //扩展一个标准的 I/O 类
{
	function __construct() //任何时候 fd 连接建立完成后都会调用这个函数
	{
		//某些类会覆盖此函数比如 HTTP 类
		return TRUE; //任何时候在最后返回不为 true 都会立即断开链接，并且释放有关这个 fd 一切使用的上下文
	}
	function recv() //当 fd 有数据可读时服务端会调用这个函数
	{
		//某些类会覆盖此函数比如 HTTP 类，但是一定有另外一个触发回调函数 比如 HTTP 类触发函数是 recv_req
		$len = $this->read($buf, 1024); //读取当前 fd 数据
		$this->send($buf); //发送数据给这个 fd
		return TRUE; //任何时候在最后返回不为 true 都会立即断开链接
	}
	function __destruct() //任何时候 fd 异常断开连接后都会调用此函数
	{
		//所以在这里继续向这个 fd 写数据是没用的
	}
}
//启动tcp服务端
tcpserver(function()
{
	$this->io_class = 'demo_io'; //设置服务端 I/O 逻辑处理类的名称，demo_io 就是上面写好的的类
	$this->local_socket = 'tcp://*:8014'; //本地socket地址，*代表同时监听IPv6和IPv4地址，0.0.0.0 或[::]
}
//一个简单的echo服务端就写好并且启动了，记住上面的规则了吗？好吧让我开始了解一下都有什么类和方法吧！
```
#### 演示 HTTP 服务端
```php
<?php
class demo_http extends tcpserver_http
{
 	function recv_req()
	{
		$this->send_chunked('hello php server!!!');
		return TRUE;
	}
}
tcpserver(function()
{
	$this->io_class = 'demo_http';
	$this->local_socket = 'tcp://*:80';
}
```
#### 演示 WebSocket 服务端
```php
<?php
class demo_ws extends tcpserver_ws
{
	function recv_frame()
	{
		$frame = $this->frame_content();
		return $this->send_frame($frame);
	}
}
tcpserver(function()
{
	$this->io_class = 'demo_ws';
	$this->local_socket = 'tcp://*:8014';
}
```
#### 简单嘛？或许你应该去看看最上面类的手册
<pre>
推荐了解过程
先了解 tcpserver 类，这个是服务端含有的功能，线程类和I/O类都是可以调用主类方法的
接下来看 tcpserver_io 类，以后所有tcp协议都将建继承这个类的方法，也包括用户想写自己协议逻辑也必须继承这个类
关于 thread 类，基本没什么功能，完全是为了在线程启动时候初始化一些线程上下文
比如预先建立mysql连接，当客户端请求查询数据直接 可以调用当前线程的mysql查询，减少socket建立与mysql握手过程
当然 thread 也可以写个死循环让它去采集数据什么的
</pre>
#### 服务端内部流程控制，其实也是IOCP的基本机制
<pre>
----------------------------------------------------------------------------------------------------
服务端启动 -> 创建最大连接数 socket 并且做端口完成异步绑定 -> [定时器循环... -> 激活某个定时器线程 -> 定时器循环...]
	-> 线程初始化1 -> [IOCP完成队列... -> 某个 socket 事件 -> 事件处理... -> IOCP完成队列...]
	-> 线程初始化2 -> [IOCP完成队列... -> 某个 socket 事件 -> 事件处理... -> IOCP完成队列...]
	-> 线程初始化3 -> [IOCP完成队列... -> 某个 socket 事件 -> 事件处理... -> IOCP完成队列...]
	-> 线程初始化N -> [IOCP完成队列... -> 某个 socket 事件 -> 事件处理... -> IOCP完成队列...]
----------------------------------------------------------------------------------------------------
这就是IOCP最基本的机制，IOCP只有一个内核的原子队列，在N个线程上只需要调用完成队列函数就解决问题了，天生自带光环
所以只要有线程在空闲的情况下绝对不会出现用户排队的情况，服务端也是独占地址模式，不是地址重用这也会增加一定安全性
注意：这个机制区别于每个线程维护一个队列的服务端，如果处理10个用户刚好在同一个负责线程就不能有效解决效排队处理问题
这个服务端其实就是一个很简单的服务端，并没有太多花哨操作方法，目前所有代码加起来可能只有4千行左右
其中实现包含有 IOCP基本网络库封装(OpenSSL兼容)、hash_node、sync_list、嵌入zend、扩展5个PHP操作类
除此之外就在也没有包含其他任何库，所有代码目前也只是一个人完成，所以它很精简，也只能运行在 Windows 平台上
</pre>
