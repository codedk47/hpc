## tcpserver_http
#### 简介
<pre>
这个类继承于 tcpserver_io 这是一个 http 服务端功能抽象类，用户可以继承该类来完成一个自己的 http 服务端
该类已经实现了 tcpserver_io 类的 __construct 和 recv 方法
同时用户也必须实现 recv_req 方法，http 服务端在接受到一个请求后会触发这个方法
这个 http 类是完全自己写的没有含有第三方库目前只支持 HTTP/1.0 和 1.1
注意这个这个类会在收到完整的 http 头部请求后立即调用 recv_req 而非等待后续 body 数据
后续数据必须通过 get_content 函数来取得
同时这样也增加一定的安全和快速回应性，用户可以在代码中控制 body 数据 比如 POST 数据大小等不同逻辑的限制
</pre>
#### 类摘要
- **abstract** tcpserver_http **extends** [tcpserver_io](tcpserver_io.md) {
	- **abstract** recv_req(void) //用户必须实现这个方法
	- `/* 方法 */`
	- string [req_head([bool $all = false])](tcpserver_http.md#req_head) //获取请求头内容
	- string [req_method(void)](tcpserver_http.md#req_method) //获取请求方法如 GET POST HEAD 等
	- string [req_url(void)](tcpserver_http.md#req_url) //获取请求 URL
	- string [req_file([string $index = 'index.html'])](tcpserver_http.md#req_file) //获取请求 URL 文件
	- mixed [req_query([string $name = NULL])](tcpserver_http.md#req_query) //获取请求 URL 某个参数
	- array [req_querys(void)](tcpserver_http.md#req_querys) //获取请求 URL 所有参数
	- string [req_ver(void)](tcpserver_http.md#req_ver) //获取请求 HTTP 版本
	- mixed [req_find(string $header_name)](tcpserver_http.md#req_find) //查询请求头
	- bool [req_gzip_on(void)](tcpserver_http.md#req_gzip_on) //判断请求师傅支持 gzip
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
	- bool [set_cookie(string $name[, string $value[, int $expire[, string $path[, string $domain[, bool $secure[, bool $httponly]]]]]])](tcpserver_http.md#set_cookie) //响应头添加 cookie 设置
	- bool [res_send(void)](tcpserver_http.md#res_send) //发送响应头信息
	- bool [send_file(string $filename)](tcpserver_http.md#send_file) //发送文件
	- bool [send_echo(callable $callback)](tcpserver_http.md#send_echo) //发送回调里 php 所有输出到 http 内容
	- bool [send_403(void)](tcpserver_http.md#send_403) //发送 403 状态
	- bool [send_404(void)](tcpserver_http.md#send_404) //发送 404 状态
	- bool [send(string $buffer)](tcpserver_http.md#send) //请不要覆盖该方法，发送内容
	- void recv(void) //请不要覆盖该方法，内部基本 http 逻辑机制
- }
