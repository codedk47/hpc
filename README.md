## Hypertext Preprocessor Console
<pre>
该项目暂时不开源有兴趣可以加Q群（456357348）技术讨论吹牛B等-_-b
目前服务端基础功能已经过测试免费提供使用，希望能得到大家的想法与支持
</pre>
#### 开始编写一个简单echo服务端
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
	$this->io_class = 'demo_io'; //设置服务端 I/O 逻辑处理类的名称，demo_io 就是上面写好的的类
	$this->local_socket = 'tcp://*:8014'; //本地socket地址，*代表同时监听IPv6和IPv4地址，0.0.0.0 或[::]
}
//一个简单的echo服务端就写好并且启动了，记住上面的规则了吗？好吧让我开始了解一下都有什么类和方法吧！
?>
```
[TOC]
