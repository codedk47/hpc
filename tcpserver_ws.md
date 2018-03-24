## tcpserver_ws
#### 简介
<pre>
这个类继承于 tcpserver_http 类，这是一个 websocket 服务端功能抽象类，用户可以继承该类来完成一个自己的 websocket 服务端
该类已覆盖了 tcpserver_http 类的 recv 和 send 方法
同时用户也必须实现 recv_frame 方法，websocket 服务端在接受到一个帧后会触发这个方法
注意这个这个类的 send 方法回自动调用编码帧完成编码
如果需要吧数据发给其他连接需要用户自己手动调用编码帧后的数据在进行发送
服务端的 send_all、send_here、send_channel、和 tcpserver_io 类的 send_them 并不会编码任何数据
既然继承于 tcpserver_http 类那么 tcpserver_ws 类也会触发一次 recv_req 方法后才会进入接收帧的逻辑
用户可以自己实现 recv_req 方法来判断客户端的一些 cookie 登录信息等流程控制
</pre>
#### 类摘要
- **abstract** tcpserver_ws **extends** [tcpserver_http](tcpserver_http.md) {
	- **abstract** recv_frame(void) //用户必须实现这个方法
	- recv_req(void) //用户选择实现这个方法
	- `/* 方法 */`
- }
