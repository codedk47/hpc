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
- **abstract** tcpserver {
	- **abstract** recv(void) //用户必须实现这个方法
	- `/* 方法 */`
	- void [__construct(void)](tcpserver_io.md#__construct) //连接构造函数
	- void [__destruct(void)](tcpserver_io.md#__destruct) //连接析构函数
	- void [__set(string $key, string $value)](tcpserver_io.md#__set) //（php魔术方法）添加一条记录
	- mixed [__get(string $key)](tcpserver_io.md#__get) //（php魔术方法）查询一条记录
	- bool [__isset(string $key)](tcpserver_io.md#__isset) //（php魔术方法）判断记录是否存在
	- void [__unset(string $key)](tcpserver_io.md#__unset) //（php魔术方法）删除一条记录
	- int [peek(string &$buffer, int $length)](tcpserver_io.md#peek) //窥视连接数据
	- int [read(string &$buffer, int $length)](tcpserver_io.md#read) //读取连接数据
	- int [write(string $buffer)](tcpserver_io.md#write) //写入连接数据
	- bool [sendfile(string $filename)](tcpserver_io.md#sendfile) //向连接发送文件
	- bool [end(void)](tcpserver_io.md#end) //向连接发送关闭发送信号
	- object [thread(void)](tcpserver_io.md#thread) //获取当前线程实例化对象
	- object [server(void)](tcpserver_io.md#server) //获取服务端实例化对象
	- mixed [sync(callable $callback)](tcpserver_io.md#sync) //同步回调
	- int [id(void)](tcpserver_io.md#id) //获取连接的 id
	- string [ip_info(void)](tcpserver_io.md#ip_info) //获取连接的 IP 信息
	- string [ip_addr(void)](tcpserver_io.md#ip_addr) //获取连接的 IP 地址
	- string [ip_port(void)](tcpserver_io.md#ip_port) //获取连接的 IP 端口
	- int [set_channel(string $name)](tcpserver_io.md#set_channel) //设置连接频道
	- int [send_them(string $buffer)](tcpserver_io.md#send_them) //发送数据给除当前连接以外的所有连接
	- bool [wait_recv(int $timeout)](tcpserver_io.md#wait_recv) //实验性，阻塞等待可读取事件
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
	function __construct()
	{
		$this->name = '阿尔萨斯';
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
	function __construct()
	{
		$this->name = '阿尔萨斯';
		return TRUE;
	}
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->write($this->name . $buf);
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
#### sendfile
<pre>
发送一个文件，成功返回 true 失败返回 false
注意这个方法在 tcp 模式下调用 Windows 的 TransmitFile API
在 ssl 模式下使用的是通常的循环拷贝写入的做法
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->sendfile('./av-998.avi');
		return TRUE;
	}
}
```
#### end
<pre>
向这个 fd 写入关闭发送通知，只关闭发送，成功返回 true 失败返回 false
注意这里仅关闭发送，还可以收到这个 fd 发来的数据，但是服务端却不可以在向这个 fd 继续写数据
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		return $this->end(); //可以用来做控制机制
	}
}
```
#### thread
<pre>
获取当前线程实例化对象
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		print_r($this->thread());
		return TRUE;
	}
}
```
#### server
<pre>
获取服务端实例化对象
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		print_r($this->server());
		return TRUE;
	}
}
```
#### sync
<pre>
同步调用回调一个函数
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$result = $this->sync(function($num){
			return $num;
		}, 123);
		return TRUE;
	}
}
```
#### id
<pre>
取得当前 fd 的 id， id 范围是从 0 到最大连接数减1
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$this->id();
		return TRUE;
	}
}
```
#### ip_info
<pre>
取得当前 fd 的 ip 协议信息
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		echo $this->ip_info(), "\n";
		return TRUE;
	}
}
```
#### ip_addr
<pre>
取得当前 fd 的 ip 协议地址，它是总是返回16进制32长度的字符串
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		echo $this->ip_addr(), "\n";
		return TRUE;
	}
}
```
#### ip_port
<pre>
取得当前 fd 的 ip 协议端口，它是总是返回16进制4长度的字符串
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		echo $this->ip_port(), "\n";
		return TRUE;
	}
}
```
#### set_channel
<pre>
设置当前 fd 所在频道，返回设置频道字符串的 hash 值 int 类型
注意这里使用的 php 内部的 hash 算法也是 times33 算法
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->set_channel('诺森德');
		return TRUE;
	}
}
```
#### send_them
<pre>
把数据发送给除当前 fd 以外的所有连接
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$this->send_them('嗨！大家好');
		return TRUE;
	}
}
```
#### wait_recv
<pre>
等待当前 fd 有数据接收，有数据接收返回 true，其他返回 false
可以理解为阻塞等待读取，不过多了一个超时设置，并且这个超时最大是 65535 秒
这个函数是实验性质，可能在未来的版本中移除，实在想不出有什么情况下会用到，又怕用到的时候没有所以暂时留下了
如果说很多地方需要那么我只能说你服务端逻辑方式有点问题，换种思路试试，不过有时候还真能解决一些头疼的问题
这里使用的是 Windows 的 WSAEventSelect 和 WaitForSingleObject API
注意的是当有可读事件会马上返回
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		//...
		if($this->wait_recv(8)) //最多给你8秒钟考虑时间，注意这里线挂起的，这个线程就这么被无情的霸占了
		{
			//注意的是当有可读事件会马上返回，并不会真的等到8秒
			$len = $this->read($buf, 1024);
			return TRUE;
		}
		return FALSE;
	}
}
```
