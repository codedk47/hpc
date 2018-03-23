## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿在多线程修改这个类的属性值（可以多线程读）
服务端只能通过tcpserver函数回调启动
比如
tcpserver(function(){
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
	- bool [set_ssl(string $cert, string $key)](tcpserver.md#set_ssl)
	- bool [add_timer(closure $callback[, int $timer])](tcpserver.md#add_timer)
	- int [get_online(void)](tcpserver.md#get_online)
	- array [get_connects(void)](tcpserver.md#get_connects)
	- int [get_clock(void)](tcpserver.md#get_clock);
	- int [send_all(string $message)](tcpserver.md#send_all);
	- int [send_here(array $id, string $message)](tcpserver.md#send_all);
	- int [send_channel(string $channel, string $message)](tcpserver.md#send_channel);
	- bool [kick_id(int $id)](tcpserver.md#kick_id);
	- int [sync_ctor(void)](tcpserver.md#sync_ctor);
	- void [sync_dtor(int $sync)](tcpserver.md#sync_dtor);
	- mixed [sync_call(int $sync, callable $callback)](tcpserver.md#sync_call);
	- bool [cache_add(string $key, string $value)](tcpserver.md#cache_add);
	- mixed [cache_get(string $key)](tcpserver.md#cache_get);
	- bool [cache_del(string $key)](tcpserver.md#cache_del);
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
指定绑地址定如 0.0.0.0 或 [::]，*代表同时监听IPv6和IPv4地址
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
#### set_ssl
<pre>
设置服务端加密通讯证书和密钥，只有在使用 ssl 协议下服务端才真正去读取这个证书和密钥
服务端采用的 OpenSSL 加密库，版本请看首页编译情况说明
</pre>
```php
tcpserver(function(){
	//操作不区分先后顺序，不过只有设置 ssl 协议服务端才会去加载 ./myserver.cer 和 ./myserver.key
	$this->local_socket = 'tcp://*:8014'; //不加载 ssl
	$this->local_socket = 'ssl://*:8014'; //尝试加载 ssl，会判断是否真正设置了证书和密钥
	$this->set_ssl('./myserver.cer', './myserver.key');
});
```
#### add_timer
<pre>
给服务端添加一个定时器，可以理解增加计划任务，任务是在其他线程进行执行的也称之为异步
但是调用 add_timer 会立即创建这个线程并且挂起直到时间点触发任务
服务端定时器是秒级的别试图它那来做高精度定时需求，但是你可以个创建出另外一个线程去做些什么不是吗？
在没有必要的情况下千万不要在定时器里增加定时器，尤其是当一个定时器线程退出后，在刚才线程创建的定时器还在运行这个时候注意
对代码严谨程度一定要做好，不然很容易让服务器异常退出，我尝试过捕获异常全失败了，这里比较难控制
</pre>
```php
tcpserver(function(){
	$this->add_timer(function(){ //增加一个定时器
		$this->add_timer(function(){ //定时器触发后又增加一个定时器
			//...
		});
		//这个定时器任务做完退出了，刚才创建的定时器还在运行，如果刚才创建的定时器代码有问题很难捕获异常
		//没有必要的情况下不要这样写
	});
	//下面来个正常点的写法
	$this->add_timer(function(){ //增加一个定时器
		while(1){
			sleep(2); //每个2秒显示一次在线数
			$this->console_log('当前服务端在线 %d', $this->get_online());
		}
	}, time() + 5 ); //服务器初始化完成后5秒开始运行
});
```
#### get_online
<pre>
取得当前服务端连接数
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->server()->console_log('之前服务端在线 %d', $this->server()->get_online());
		//注意当前连接数不含有这个连接，只有在构造函数真正返回 true 后这个连接才被加入到连接表
		//连接表是双向同步的所以会有一定的性能影响对新连接加入到在线连接表速度
		return TRUE;
	}
}
```
#### get_connects
<pre>
取得当前服务端连接id数组
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		print_r($this->server()->get_connects());
		//注意当前服务端连接id数组数不含有这个连接，只有在构造函数真正返回 true 后这个连接才被加入到连接表
		//调用这个函数是锁表操作取得的有一定性能影响
		return TRUE;
	}
}
```
#### get_clock
<pre>
取得当前服务端时钟秒级,注意只有当服务器启动完成后这个定时器才会根据系统时间更新
做这个目的是为了不要平凡调用time()函数，没必要高精度的话就从当前服务器时钟取得当前时间戳
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->server()->cosnole_log('%s加入连接ID:%d',
			date('Y-m-d H:i:s', $this->server()->get_clock()),
			$this->id()
		);
		return TRUE;
	}
}
```
#### send_all
<pre>
将数据发送给当前连接表所有连接
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->server()->sed_all($buf);
		return TRUE;
	}
}
```
#### send_here
<pre>
将数据发送给当前连接表指定连接id
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->server()->sed_here($this->server()->get_connects(), $buf);
		return TRUE;
	}
}
```
#### send_channel
<pre>
将数据发送给当前连接表指定连接频道
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->set_channel('灰谷森林');
		return TRUE;
	}
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->server()->send_channel('灰谷森林', $buf);
		return TRUE;
	}
}
```
#### kick_id
<pre>
向这个连接id发送关闭发送信号
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function __construct()
	{
		$this->server()->kick_id(0);
		return TRUE;
	}
}
```
#### sync_ctor
<pre>
构造一个同步地址，同步机制使用的是 Windows 的 CRITICAL_SECTION 结构体
返回这个申请内存成功后的地址，失败返回 0
</pre>
```php
tcpserver(function(){
	$this->sync_id = $this->sync_ctor(); //创建同步地址
});
```
#### sync_dtor
<pre>
销毁一个同步地址的内存地址并且释放这段内存
</pre>
```php
tcpserver(function(){
	if($this->sync_id = $this->sync_ctor()) //创建同步地址
	{
		$this->sync_dtor($this->sync_id); //销毁同步地址
	}
});
```
#### sync_call
<pre>
调用一个同步地址回调一个函数
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->server()->sync_call($this->server()->sync_id, function() //调用同步地址
		{
			//确保这里代码是同步与 $this->server()->sync_id 地址操作的
			//比如并发10个人抢一个座位，这个操作呢显得有位重要，不能乱排队以此进行谁先到给谁
		});
		return TRUE;
	}
}
tcpserver(function(){
	$this->io_class = 'my_io_class_test';
	$this->sync_id = $this->sync_ctor(); //创建同步地址
});
```
#### cache_add
<pre>
同步向服务端缓存表里增加一个key=value，成功返回 true，失败返回 false
</pre>
```php
tcpserver(function(){
	$this->io_class = 'my_io_class_test';
	$this->cache_add('key_name', 'hello php');
});
```
#### cache_get
<pre>
同步查询服务端缓存表里一个key值，成功返回该key的value，失败返回 NULL 类型
</pre>
```php
class my_io_class_test extends tcpserver_io
{
	function recv()
	{
		$len = $this->read($buf, 1024);
		$this->write($this->server()->cache_get('key_name'));
		return TRUE;
	}
}
tcpserver(function(){
	$this->io_class = 'my_io_class_test';
	$this->cache_add('key_name', 'hello php');
});
```
#### cache_del
<pre>
同步删除服务端缓存表里一个key值，成功返回 true，失败返回 false
</pre>
```php
tcpserver(function(){
	$this->io_class = 'my_io_class_test';
	$this->cache_add('key_name', 'hello php');
	$this->cache_del('key_name');
});
```
