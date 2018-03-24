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
- }
