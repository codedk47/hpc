# Hypertext Preprocessor Console
## Hypertext Preprocessor Server
#### PHP7.2 For Windows IOCP Socket Server
<pre>
该项目暂时不开源有兴趣可以加Q群（456357348）技术讨论吹牛B等-_-b
目前服务端基础功能已经过高可用稳定测试性能可靠免费提供使用，希望能得到大家功能上扩展意见
主要实现以下类
PHP extend declared classes
	thread		//标准线程上下文类，可继承
	tcpserver	//TCP服务端主要类，不可继承
	tcpserver_io	//标准I/O逻辑类，必须被继承
	tcpserver_http	//基本HTTP类继承I/O类，必须被继承
	tcpserver_ws	//基本WebSocket类继承HTTP类，必须被继承

```php
<?php
//服务端内部所有 "tcpserver_" 开头的类都继承该类的方法 比如 HTTP 类
class demo_io extends tcpserver_io //扩展一个标准的 I/O 类
{
    function __construct() //任何时候 fd 连接完成后都会调用这个函数
	{
		//某些类会覆盖此函数比如 HTTP 类
		return TRUE; //任何时候在最后返回不为 true 都会立即断开链接，并且释放有关这个 fd 一切使用的上下文
	}
	function recv() //当 fd 有数据可读时服务端会调用这个函数
	{
		//某些类会覆盖此函数比如 HTTP 类，但是一定有另外一个触发回调函数 比如 HTTP 类触发函数是 recv_req
		$len = $this->read($buf, 1024); //读取当前 fd 数据
		$this->write($buf); //写入当前 fd 数据
		return TRUE; //任何时候在最后返回不为 true 都会立即断开链接
	}
	function __destruct() //任何时候 fd 断开连接后都会调用此函数
	{
		//所以在这里继续向这个 fd 写数据是没用的
	}
}
//启动tcp服务端
tcpserver(function()
{
	$this->io_class = 'demo_io'; //设置服务端 I/O 逻辑处理类的名称，demo_io 就是刚才扩展 tcpserver_io 的逻辑处理类
	$this->local_socket = 'tcp://*:8014'; //监听本地socket地址，* 代表同时监听IPv6和IPv4地址， 0.0.0.0 或 [::]
}
//一个简单的echo服务端就写好并且启动了，记住上面的规则了吗？好吧让我开始吧！
?>
```
