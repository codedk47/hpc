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
	- string [req_head(void)](tcpserver_http.md#req_head)
	- void [req_method(void)](tcpserver_http.md#req_method)
	- void [req_url(void)](tcpserver_http.md#req_url)
	- void [req_file(void)](tcpserver_http.md#req_file)
	- void [req_query(void)](tcpserver_http.md#req_query)
	- void [req_querys(void)](tcpserver_http.md#req_querys)
	- void [req_ver(void)](tcpserver_http.md#req_ver)
	- void [req_find(void)](tcpserver_http.md#req_find)
	- void [req_gzip_on(void)](tcpserver_http.md#req_gzip_on)
	- void [req_etag_eq(void)](tcpserver_http.md#req_etag_eq)
	- void [get_cookie(void)](tcpserver_http.md#get_cookie)
	- void [get_cookies(void)](tcpserver_http.md#get_cookies)
	- void [get_content(void)](tcpserver_http.md#get_content)
	- void [get_content_offset(void)](tcpserver_http.md#get_content_offset)
	- void [get_content_buffer(void)](tcpserver_http.md#get_content_buffer)
	- void [get_content_format(void)](tcpserver_http.md#get_content_format)
	- void [res_head(void)](tcpserver_http.md#res_head)
	- void [res_status(void)](tcpserver_http.md#res_status)
	- void [res_append(void)](tcpserver_http.md#res_append)
	- void [res_length(void)](tcpserver_http.md#res_length)
	- void [res_type(void)](tcpserver_http.md#res_type)
	- void [res_send(void)](tcpserver_http.md#res_send)
	- void [res_etag(void)](tcpserver_http.md#res_etag)
	- void [set_cookie(void)](tcpserver_http.md#set_cookie)
	- void [send_file(void)](tcpserver_http.md#send_file)
	- void [send_echo(void)](tcpserver_http.md#send_echo)
	- void [send_403(void)](tcpserver_http.md#send_403)
	- void [send_404(void)](tcpserver_http.md#send_404)
	- void [send(void)](tcpserver_http.md#send) //请不要覆盖该方法
	- void recv(void) //请不要覆盖该方法
- }
