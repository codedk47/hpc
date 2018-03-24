## thread
#### 简介
<pre>
这个类是服务端启动创建线程后会做初始化的类，没有太多功能性完全是提供给用户可以自己初始化一些线程上下文的
用户可以继承这个类完善 ctor 方法来实现自己的上下文初始化
</pre>
#### 类摘要
- thread {
	- `/* 方法 */`
	- void __construct(void) //内部构造函数
	- void __destruct(void) //内部析构函数
	- object [server(void)](thread.md#server) //获取服务端实例化对象
	- int [id(void)](thread.md#id) //获取当前线程 id
	- bool [ctor(void)](thread.md#ctor) //用户构造函数
	- void [dtor(void)](thread.md#dtor) //用户析构函数
- }
#### server
<pre>
获取服务端实例化对象
</pre>
```php
class my_thread_init extends thread
{
	function ctor()
	{
		print_r($this->server());
		return TRUE;
	}
}
```
#### id
<pre>
取得当前线程 id
</pre>
```php
class my_thread_init extends thread
{
	function ctor()
	{
		echo $this->id(), "\n";
		return TRUE;
	}
}
```
#### ctor
<pre>
线程构造函数调用完毕后会调用这个方法，用户可以扩展这个方法对当前线程的上下文做初始化
</pre>
```php
class my_thread_init extends thread
{
	function ctor()
	{
		//你可以在这里设置线程的上下文
		$this->sql = @mysqli_connect('127.0.0.1', 'root', '', 'webapp');
		if(!$this->sql)
		{
			$this->server()->console_log(mysqli_connect_error());
			return FALSE; //当返回不为 true 线程初始化失败，并且退出线程
		}
		return TRUE;
	}
}
```
#### dtor
<pre>
线程正常退出后将调用这个方法
</pre>
```php
class my_thread_init extends thread
{
	function dtor()
	{
		$this->server()->console_log('ID:%d 线程退出', $this->id());
	}
}
```
