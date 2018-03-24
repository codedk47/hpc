## tcpserver_io
#### 简介
<pre>
这是一个服务端连接后对这个 fd 操作的抽象类，这类并不是每次有新连接后都实例化一次，而是线程类初始化完成后紧跟着初始化一次
然后通过 php 线程全局对象指针指向某个 fd 进行操作
这个类必须被继承，并且一定要实现 recv 方法，每次服务端触发 recv 函数时候这个 fd 都至少有1个字节可读
继承这个类后的所有属性将不被载入，所以扩展这个类的所有属性都是无效的
比如
class my_io_class_test extends tcpserver_io
{
	public $name = '阿尔萨斯'; //这个属性并不会随着实例化后更新到 fd 的上下文
}
不过可以在构造函数里写属性，连接建立完成后总是会调用 __construct 方法
还有这个类的 __construct 和 __destruct 方法除外其他方法都是不可覆盖的
任何时候服务端触发的方法在最终返回结果不为 true 总是立即断开连接并且释放这个连接的上下文
</pre>
#### 类摘要
- tcpserver {
	- `/* 方法 */`
	- void [__construct(void)](tcpserver_io.md#__construct)
	- void [__destruct(void)](tcpserver_io.md#__destruct)
	- void [__set(string $key, string $value)](tcpserver_io.md#__set)
	- mixed [__get(string $key)](tcpserver_io.md#__get)
	- bool [__isset(string $key)](tcpserver_io.md#__isset)
	- void [__unset(string $key)](tcpserver_io.md#__unset)
	- int [peek(string &$buffer, int $length)](tcpserver_io.md#peek)
	- int [read(string &$buffer, int $length)](tcpserver_io.md#read)
	- int [write(string &$buffer)](tcpserver_io.md#write)
	
	
	- bool [wait_recv(string $key)](tcpserver_io.md#wait_recv); //实验性
- }
#### __construct
<pre>
任何时候新的连接建立完成后会调用这个方法
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->name = '阿尔萨斯'; //这样写才有效
		return TRUE;
	}
}
```
#### __destruct
<pre>
任何时候连断开后总是会调用这个方法
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __destruct()
	{
		$this->server()->console_log('ID:%d 断开了连接', $this->id());
	}
}
```
#### __set
<pre>
php 的魔术方法，保存这个连接上下文key=value的方法，目前只支持字符串
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->name = $buf;
		return TRUE;
	}
}
```
#### __get
<pre>
php 的魔术方法，查询这个上下文key，失败返回 NULL
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->name = $buf;
		return TRUE;
	}
}
```
#### __isset
<pre>
php 的魔术方法，查询这个上下文key是否存在，返回 true 或者 false
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		if(isset($this->name))
		{
			//...
		}
		return TRUE;
	}
}
```
#### __unset
<pre>
php 的魔术方法，删除这个上下文key=value
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		if(isset($this->name))
		{
			unset($this->name);
		}
		return TRUE;
	}
}
```
#### peek
<pre>
读取这个 fd 数据，并不会从 socket 缓冲区里移除这些读取到的数据，返回实际读取长度
第一个参数是引用把数据传递给某个变量，第二个参数是需要读取多少数据
注意如果不把数据读从 socket 缓冲区移除而继续丢进 IOCP 队列，会马上继续激活 recv 函数
所以每次尽量吧 socket 缓冲区数据全部移除
注意每次服务端触发 recv 方法总是可以读到至少1个字节
如果后续继续读取数据并且非阻塞 socket 没有数据时候返回 0，只有在遇到错误才返回 -1
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->peek($buf, 128);
		return TRUE;
	}
}
```
#### read
<pre>
读取这个 fd 数据，并且从 socket 缓冲区移除读取到的数据，除外与 peek 函数调用方法一致
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 128);
		return TRUE;
	}
}
```
#### write
<pre>
写入这个 fd 数据，返回实际写入长度，在遇到错误时候返回 -1
这个函数是同步的，在没有错误的时候总是会返回与实际写入长度一致的结果
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->write('最近还好吗？');
		return TRUE;
	}
}
```
