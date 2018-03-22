## tcpserver
#### 简介
<pre>
这是一个服务端主进程类大部分线程都可以调用该类的方法，但是请勿修改该实例化后的属性值（可以多线程读）
</pre>
#### 类摘要

* tcpserver {
  - /* 属性 */
  - `string` [$work_root](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
  - `string` [$thread_class](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
  - `string` [$concurrent_thread](https://github.com/codedk47/hpc/bolb/master/tcpserver.md);
  

  - string $work_root; //设置线程上下文目录（当前服务端路径下的文件夹）
  * string $thread_class; //设置逻辑线程上下文
  * int $concurrent_thread; //设置并发行线程数量，建议服务器CPU*2(默认0由服务端自行判断,最大不能超过CPU*4)
  * string $local_socket; //监听本地socket地址，* 代表同时监听IPv6和IPv4地址， 0.0.0.0 或 [::]
  * int $max_connected; //服务端最大连接数，不能超过20万
  * int $kick_timeout; //踢除长时间未数据收发的连接（0为不开启检测）
  * /* 方法 */
  * void [tcpserver::__construct(void)](https://github.com/codedk47/hpc/bolb/master/tcpserver.md) //构造函数tcpserver函数就是调用这个函数
}
