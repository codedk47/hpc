## tcpserver_ws
#### 简介
<pre>
这个类继承于 tcpserver_http 类，这是一个 websocket 服务端功能抽象类，用户可以继承该类来完成一个 websocket 服务端
该类已覆盖了 tcpserver_http 类的 recv 方法
服务端目前只支持 65652 大小的数据帧，连接建立握手成功后向操作系统申请
并不会每次帧都去动态申请内存，所以有比较大的数据帧内容请发连续帧，用户并且自己写逻辑规则
同时用户也必须实现 recv_frame 方法，websocket 服务端在接受到一个帧后会触发这个方法
注意这个这个类的 send_frame 方法回自动调用编码帧完成编码后在发送
如果需要把数据发给其他连接需要用户自己手动调用编码帧后的数据在进行发送
服务端的 send_all、send_here、send_channel、和 tcpserver_io 类的 send_them 并不会编码任何数据
既然继承于 tcpserver_http 类那么 tcpserver_ws 类也会触发一次 recv_req 方法后才会进入接收帧的逻辑
用户可以自己实现 recv_req 方法来判断客户端的一些 cookie 登录信息等流程控制
</pre>
#### 类摘要
- **abstract** tcpserver_ws **extends** [tcpserver_http](tcpserver_http.md) {
	- **abstract** recv_frame(void) //用户必须实现这个方法
	- recv_req(void) //用户选择实现这个方法
	- `/* 方法 */`
	- bool recv(void) //内部核心逻辑机制（不要调用，服务端会自动解码帧）
	- bool [send_frame(string $content[, int $opcode = 1[, bool $fin = true]])](tcpserver_ws.md#send_frame) //发送一个数据给客户端
	- array [frame_hi(void)](tcpserver_ws.md#frame_hi) //帧头信息
	- bool [frame_fin(void)](tcpserver_ws.md#frame_fin) //帧头信息 fin （表示是否是尾帧）
	- mixed [frame_rsv(int $rsv)](tcpserver_ws.md#frame_rsv) //帧头信息 rsv
	- int [frame_opcode(void)](tcpserver_ws.md#frame_opcode) //帧头信息 opcode （ 0 - 15 ）
	- bool [frame_mask(void)](tcpserver_ws.md#frame_mask) //帧头信息 是否有 mask
	- int [frame_length(void)](tcpserver_ws.md#frame_length) //帧内容长度
	- string [frame_content(void)](tcpserver_ws.md#frame_content) //帧内容
	- string [frame_encode(string $content[, int $opcode = 1[, bool $fin = true]])](tcpserver_ws.md#frame_encode) //将内容编码成帧
- }
#### send_frame
<pre>
发送内容给当前连接，成功返回 true 失败返回 false
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		$buffer = $this->frame_content();
		//连续3帧发送，到终端后会自己合并完整帧
		$this->send_frame($buffer, 1, FALSE);
		$this->send_frame($buffer, 0, FALSE );
		$this->send_frame($buffer, 0, TRUE);
		//发送一帧
		$this->send_frame($buffer);
		return TRUE;
	}
}
tcpserver(function(){
	$this->io_class = 'myser';
	$this->local_socket = 'tcp://*8080';
});
```
#### frame_hi
<pre>
帧头信息
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_hi());
		return TRUE;
	}
}
```
#### frame_fin
<pre>
帧头信息 fin，可以判断是否是最后一帧
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_fin());
		return TRUE;
	}
}
```
#### frame_rsv
<pre>
帧头信息 rsv，参数 1-3 返回 true 或者 false，其他返回 NULL
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_rsv(1));
		return TRUE;
	}
}
```
#### frame_opcode
<pre>
帧头信息 opcode，返回值 0-15
0x0 ：continuation frame
0x1 ：text frame
0x2 ：binary frame
0x3 - 0x7 ：保留，for non-control frame
0x8 ：close frame
0x9 ：ping frame
0xA ：pong frame
0xB - 0xF ：保留，for control-frame
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_opcode());
		return TRUE;
	}
}
```
#### frame_mask
<pre>
帧头信息 是否有 mask
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_mask());
		return TRUE;
	}
}
```
#### frame_length
<pre>
帧内容长度
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_length());
		return TRUE;
	}
}
```
#### frame_content
<pre>
帧内容
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		var_dump($this->frame_content());
		return TRUE;
	}
}
```
#### frame_encode
<pre>
将内容编码成帧，这个是一个预留 api 可以将数据编码成帧后调用 server 对象下 的 send_all 等方法发送给其连接
应为只有 tcpserver_ws 类下的 send_frame 方法会自动将内容编码为帧，其他发送方法并不会任何编码
</pre>
```php
class myser extends tcpserver_ws
{
	function recv_frame()
	{
		$frame = $this->frame_encode('要发送的内容');
		$this->server()->send_all($frame);
		return TRUE;
	}
}
```
