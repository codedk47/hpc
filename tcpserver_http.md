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
	- bool [res_etag(string $etag)](tcpserver_http.md#res_etag) //响应头信息添加 etag 值
	- bool [set_cookie(string $name [, string $value = '' [, int $expire = 0 [, string $path = '' [, string $domain = '' [, bool $secure = false [, bool $httponly = false ]]]]]]](tcpserver_http.md#set_cookie) //响应头添加 cookie 设置
	- bool [res_send(void)](tcpserver_http.md#res_send) //发送响应头信息
	- bool [send_file(string $filename)](tcpserver_http.md#send_file) //发送文件
	- bool [send_echo(callable $callback)](tcpserver_http.md#send_echo) //发送回调里 php 所有输出到 http 内容
	- bool [send_403(void)](tcpserver_http.md#send_403) //发送 403 状态
	- bool [send_404(void)](tcpserver_http.md#send_404) //发送 404 状态
	- bool [send(string $buffer)](tcpserver_http.md#send) //请不要覆盖该方法，发送 http 内容
- }
#### req_head
<pre>
获取当前请求头内容，参数为 true 时返回完整内容，为 false 从头信息开始地方返回
</pre>
```php
class simple_http_server extends tcpserver_http
{
	function recv_req()
	{
		$this->send($this->req_head(TRUE));
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
		$this->send('用户请求方法是：' . $this->req_method());
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
		$this->send('用户请求 URL 是：' . $this->req_url());
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
		$this->send($this->req_query('key') ?? '没有key字段');
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
		$this->send('当前请求的 HTTP 协议版本是：' . $this->req_ver());
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
		$this->send('当前请求语言是：' . $result);
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
		$this->send('当前请求 gzip 支持：' . $result);
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
			$this->send($content);
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
		$this->send('Cookie: ' . $result);
		return TRUE;
	}
}
```
