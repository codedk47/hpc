## tcpserver_http
#### 简介
<pre>
这个类继承于 tcpserver_io 类，这是一个 http 服务端功能抽象类，用户可以继承该类来完成一个自己的 http 服务端
该类已覆盖了 tcpserver_io 类的 __construct 和 recv 方法
同时用户也必须实现 recv_req 方法，http 服务端在接受到一个请求后会触发这个方法
这个 http 类目前只支持 HTTP/1.0 和 1.1
注意这个这个类会在收到完整的 http 头部请求后立即调用 recv_req 而非等待后续 body 数据
后续数据必须通过 get_content 函数来取得
同时这样也增加一定的安全和快速回应性，用户可以在代码中控制 body 数据 比如 POST 数据大小等不同逻辑的限制
</pre>
#### 类摘要
- **abstract** tcpserver_http **extends** [tcpserver_io](tcpserver_io.md) {
	- **abstract** recv_req(void) //用户必须实现这个方法
	- `/* 方法 */`
	- bool recv(void) //内部核心逻辑机制（请不要调用和覆盖该方法，可能更加开放 http 抽象类而预留）
	- string [req_head([bool $all = false])](tcpserver_http.md#req_head) //获取请求头内容
	- string [req_method(void)](tcpserver_http.md#req_method) //获取请求方法如 GET POST HEAD 等
	- string [req_url(void)](tcpserver_http.md#req_url) //获取请求 URL
	- string [req_file([string $index = 'index.html'])](tcpserver_http.md#req_file) //获取请求 URL 文件
	- mixed [req_query([string $name = NULL])](tcpserver_http.md#req_query) //获取请求 URL 某个参数
	- array [req_querys(void)](tcpserver_http.md#req_querys) //获取请求 URL 所有参数
	- string [req_ver(void)](tcpserver_http.md#req_ver) //获取请求 HTTP 版本
	- mixed [req_find(string $header_name)](tcpserver_http.md#req_find) //查询请求头
	- bool [req_gzip_on(void)](tcpserver_http.md#req_gzip_on) //判断请求是否支持 gzip
	- bool [req_etag_eq(string $etag)](tcpserver_http.md#req_etag_eq) //判断请求 etag 是否一致
	- mixed [get_cookie(string $name)](tcpserver_http.md#get_cookie) //获得请求 cookie 的 name 值
	- array [get_cookies(void)](tcpserver_http.md#get_cookies) //获取所有 cookie 值
	- mixed [get_content(callable $callback)](tcpserver_http.md#get_content) //获取请求后续内容如 POST 上传文件等
	- int [get_content_offset(void)](tcpserver_http.md#get_content_offset) //获取请求后续内容当前读取偏移值
	- string [get_content_buffer(void)](tcpserver_http.md#get_content_buffer) //获取请求后续内容当前读取的内容
	- array [get_content_format(void)](tcpserver_http.md#get_content_format) //获取请求后续内容当前内容并且做格式化
	- string [res_head(void)](tcpserver_http.md#res_head) //取得响应头全部内容
	- bool [res_status(string $http_status)](tcpserver_http.md#res_status) //响应HTTP状态
	- bool [res_append(string $header_content)](tcpserver_http.md#res_append) //响应头信息添加内容
	- bool [res_length(int $content_length)](tcpserver_http.md#res_length) //响应头信息添加内容长度
	- bool [res_type(string $content_type)](tcpserver_http.md#res_type) //响应头信息添加内容类型
	- bool [res_gzip(void)](tcpserver_http.md#res_gzip) //响应头信息添加 gzip
	- bool [res_etag(string $etag)](tcpserver_http.md#res_etag) //响应头信息添加 etag 值
	- bool [res_send(void)](tcpserver_http.md#res_send) //发送响应头信息
	- bool [set_cookie(string $name [, string $value = '' [, int $expire = 0 [, string $path = '' [, string $domain = '' [, bool $secure = false [, bool $httponly = false ]]]]]]](tcpserver_http.md#set_cookie) //响应头添加 cookie 设置
	- bool [send_chunked(string $buffer)](tcpserver_http.md#send_chunked) //发送分块
	- bool [send_file(string $filename)](tcpserver_http.md#send_file) //发送文件
	- bool [send_echo(callable $callback)](tcpserver_http.md#send_echo) //发送回调里 php 所有输出到 http 内容
	- bool [send_403(void)](tcpserver_http.md#send_403) //发送 403 状态
	- bool [send_404(void)](tcpserver_http.md#send_404) //发送 404 状态
- }
#### req_head
<pre>
获取当前请求头内容，参数为 true 时返回完整内容，为 false 从头信息开始地方返回
req_ 开头的方法都是跟请求头有关的操作，res_ 开头的方法都是跟响应头有关的操作
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked($this->req_head(TRUE));
		return TRUE;
	}
}
```
#### req_method
<pre>
获取当前请求头方法
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked('用户请求方法是：' . $this->req_method());
		return TRUE;
	}
}
```
#### req_url
<pre>
获取请求 URL
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked('用户请求 URL 是：' . $this->req_url());
		return TRUE;
	}
}
```
#### req_file
<pre>
获取请求 URL 文件，参数是默认文档文件名，它只返回一个虚拟的地址，这个函数并不会去判断文件是否真实存在
当路径以 / 结尾时候总是将参数的文档名作为文件名，默认 index.html
这个函数可以配合 send_file 一起使用
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_file($this->req_file('default.html'));
		return TRUE;
	}
}
```
#### req_query
<pre>
获取请求 URL 某个参数，可选参数为字段名，如果找到返回字段名的值，其他返回 NULL
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked($this->req_query('key') ?? '没有key字段');
		return TRUE;
	}
}
```
#### req_querys
<pre>
获取请求 URL 所有参数
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		print_r($this->req_querys());
		return TRUE;
	}
}
```
#### req_ver
<pre>
获取请求 HTTP 版本，类似 HTTP/1.0 或者  HTTP/1.1
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked('当前请求的 HTTP 协议版本是：' . $this->req_ver());
		return TRUE;
	}
}
```
#### req_find
<pre>
查询请求头，找到返回该头的内容断，没找到返回 NULL
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		//对于请求头并不是所有浏览器和某些爬虫或者个人写的数据抓去等都会设置某些头
		//在查询头尽量做好判断
		$result = $this->req_find('Accept-Language') ?? '火星语？';
		$this->send_chunked('当前请求语言是：' . $result);
		return TRUE;
	}
}
```
#### req_gzip_on
<pre>
判断请求是否支持 gzip 压缩协议，这里是弱判断 Accept-Encoding: gzip 测试几个浏览器都是小写
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$result = $this->req_gzip_on() ? '开启' : '关闭';
		$this->send_chunked('当前请求 gzip 支持：' . $result);
		return TRUE;
	}
}
```
#### req_etag_eq
<pre>
判断请求 etag 是否一致，这个是 HTTP/1.1 增加的 etag 头，这个函数可以判断是否是某个 hash 值
hash 值是 times33 算法
可以用来判断控制一些数据发送，节省带宽流量，不过目前用户必须实现自己的 304 状态
下面我给一个简单的列子来控制数据发送
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$content = '复活吧！我的勇士！';
		$this->res_etag($content);
		if($this->req_etag_eq($content)) //判断 etag 是否一致
		{
			//一致的话我们直接发送 304 状态
			//通知对方我就不传送下面的数据了，你自己吧缓存拿出来用吧我和你的内容是一样的
			$this->res_status('HTTP/1.1 304 Not Modified');
			$this->res_send(); //发送响应头即可
		}
		else
		{
			//如果不一致我们就发送数据
			$this->send_chunked($content);
		}
		return TRUE;
	}
}
```
#### get_cookie
<pre>
获取所有 cookie 字段值，可选参数字段，找到返回字段内容，其他返回 NULL
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$result = $this->get_cookie('name') ?? '没找到 name 内容';
		$this->send_chunked('Cookie: ' . $result);
		return TRUE;
	}
}
```
#### get_cookies
<pre>
获取所有 cookie 值
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		print_r($this->get_cookies());
		return TRUE;
	}
}
```
#### get_content
<pre>
获取请求后续内容如 POST 上传文件等，这个方法机制最好配合 return 一起实现，并且在回调里写后续输入内容后的控制
后续内容是根据对方请求内容长度来申请内存的，所以用户可以在逻辑代码中何时加入 get_content 方法显得尤为重要
当后续内容输入偏移等于文档长度会调用回调，用户可以在那里处理最终结果
下面是一个表单提交处理，用户自己实现一个表单提交页面，这里只是服务端代码
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		if($this->req_file()=='/uploaded')
		{
			//在这之前尽量不要有任何内容输出
			//注意这里我是 return 这个方法的
			return $this->get_content(3200000000,function() //测试接收3G大小数据
			{
				//当数据接收完毕时候会回调这个函数
				$this->send_chunked('接收内容大小：' . $this->get_content_offset());
				
				$post_raw = $this->get_content_buffer(); //获取原始内容
				
				$post = $this->get_content_format(); //获取格式化后的内容
				
				print_r($post); //打印格式化后的内容
				
				if(isset($post['uploaded_file']) //判断时候有这个字段并且是文件上传
					&& is_object($post['uploaded_file'])){
					//如果用户表单有个 uploaded_file 字段并且是文件提交那么在服务端就是对象化了
					//$post['uploaded_file'] 这个是一个上传文件是对象化里面有自己的方法
					$post['uploaded_file']->move_to('./' . time() . '.up'); //移动文件
					$post['uploaded_file']->image_type(); //判断图像类型
					//以后这个这个上传对象可能会加入跟多的方法
				}
				return TRUE;
			});
		}
		else
		{
			$this->send_file('upload.html'); //用户自己写一个表单提交页面
		}
		return TRUE;
	}
}
```
#### get_content_offset
<pre>
获取请求后续内容当前读取偏移值，它总是能返回当前读取的偏移值而不是内容总长度
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked('当前请求后续输入内容偏移值：' . $this->get_content_offset());
		return TRUE;
	}
}
```
#### get_content_buffer
<pre>
获取请求后续内容当前读取的内容，它总是返回当前已经读取到的原始数据
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked($this->get_content_buffer()); //千万不要将上传的大文件打印到屏幕，你懂的
		return TRUE;
	}
}
```
#### get_content_format
<pre>
获取请求后续内容当前内容并且做格式化，如果当前读取的内容偏移值不等于内容长度，函数不执行任何操作
注意当调用这个函数时候，如果有文件上传，这个时候才会吧上传文件创建出 php 临时文件
并且用户还可以选择一次控制权，对这个对象流进行判断检测等操作，最终可以选择移动文件到哪里
否则当请求结束内存和临时文件也会释放和销毁
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		print_r($this->get_content_format());
		return TRUE;
	}
}
```
#### res_head
<pre>
取得响应头全部内容，res_ 开头的方法都是跟响应头有关的操作
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked($this->res_head());
		return TRUE;
	}
}
```
#### res_append
<pre>
响应头信息添加内容，如果服务端含有的响应头添加尽量使用服务端提供的响应头方法
比如 res_length、res_type、res_gzip、res_etag 等，可以避免重复添加
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->res_append('X-Address-Info: ' . $this->ip_info());
		$this->send_chunked('看看头信息是否有 X-Address-Info 项');
		return TRUE;
	}
}
```
#### res_length
<pre>
响应头信息添加内容长度，服务 send 方法使用的是兼容 chunked 发送
所以用户并不必自己手动增加内容长度控制，除非用户需要一次发送固定长度数据
下面给一个列子
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$content = '我是很长的内容';
		$this->res_length(strlen($content)); //响应头信息添加内容长度
		$this->res_send(); //发送响应头信息
		$this->send_chunked($content); //发送内容，这样就绕开了 chunked 方式
		return TRUE;
	}
}
```
#### res_type
<pre>
响应头信息添加内容类型，服务端默认文档类型 text/html; charset=utf-8
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->res_type('text/plain; charset=utf-8');
		$this->send_chunked("文本信息\n换行显示");
		return TRUE;
	}
}
```
#### res_gzip
<pre>
响应头信息添加 gzip
注意 gzip + chunked 方式，并不是分段压缩的必须整段压缩后分段发送
在 wikipedia 上有对于 chunked + gzip 是不是分段压缩的明确说明
HTTP servers sometimes use
compression (gzip or deflate) to allow reducing time spent for
transmission. Chunked transfer encoding can be used to delimit
parts of the compressed object. In this case, it is worth
noting that the chunks are not individually compressed.Instead, the complete payload is compressed and
the output of the compression process is chunked using the scheme
described in this article.
看完上面让我们也来个骚操作，gzip + chunked 发送，这个机制只有在某些情况下才显得很有效率，并不适用于所有情况下
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$content = '需要被发送的内容';
		if($this->req_gzip_on()) //先判断对方浏览器支不支持，别一顿乱操作发现白搭
		{
			$this->res_gzip(); //响应头信息添加 gzip
			$this->send_chunked(gzencode($content)); //gzip 压缩后发送
		}
		else
		{
			$this->send_chunked($content);
		}
		return TRUE;
	}
}
```
#### res_etag
<pre>
响应头信息添加 etag 值
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$content = '需要发送的内容';
		$this->res_etag($content); //响应头信息添加 etag 值
		$this->send_chunked($content);
		return TRUE;
	}
}
```
#### res_send
<pre>
发送响应头信息
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->res_send(); //发送响应头信息
		return TRUE;
	}
}
```
#### set_cookie
<pre>
响应头添加 cookie 设置，该方法参数于 php 的 setcookie 一致
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->set_cookie('name', 'value');
		$this->send_chunked('添加了 cookie');
		return TRUE;
	}
}
```
#### send_chunked
<pre>
发送分块内容，成功返回 true，失败返回 false
服务端内部实现了结尾机制，除非用户想发送后立即关闭才需要自己发送 chunked 结尾
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_chunked('123');
		$this->send_chunked('456');
		$this->send_chunked('678');
		return TRUE;
	}
}
```
#### send_file
<pre>
发送文件，该方法会自动判断请求 etag 值后在选择是否继续发送，成功返回 true，失败 false
如果 etag 一致后发送 304 状态也表示发送成功返回 true
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_file('./filename.html');
		return TRUE;
	}
}
```
#### send_echo
<pre>
送回调里 php 所有输出到 http 内容，就是捕获回调函数里所有输出定向到 http 的 send 方法
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		//$this->send_echo('phpinfo'); //发送 php 信息页面
		$this->send_echo(function(){
			print_r($this->req_querys()); //打印所有请求询问参数
			print_r($this->req_cookies()); //打印所有 cookie 值
			//所有输出定向到 http 的 send 方法
		});
		return TRUE;
	}
}
```
#### send_403
<pre>
发送 403 状态
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send_403(); //页面风格类似 apache
		return TRUE;
	}
}
```
#### send_404
<pre>
发送 404 状态
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		if(!$this->send_file($this->req_file()))
		{
			$this->send_403();
		}
		return TRUE;
	}
}
```
