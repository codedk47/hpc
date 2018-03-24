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
	- void [__get(string $key)](tcpserver_io.md#__get)
	- void [__isset(string $key)](tcpserver_io.md#__isset)
	- void [__unset(string $key)](tcpserver_io.md#__unset)
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
		return TRUE; //这个是控制机制，可以实现用户的登录之类的判断
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
