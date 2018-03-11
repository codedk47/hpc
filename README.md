# Hypertext Preprocessor Console
<h2>Hypertext Preprocessor Server</h2>
<h4>PHP7.2 For Windows IOCP Socket Server</h4>
<pre>
该项目暂时不开源有兴趣可以加Q群（456357348）技术讨论吹牛B等-_-b
目前服务端基础功能已经过高可用稳定测试性能可靠免费提供使用，希望能得到大家功能上扩展意见
主要实现以下类
PHP extend declared classes
	thread			//标准线程上下文类，可继承
	tcpserver		//TCP服务端主要类，不可继承
	tcpserver_io	//标准I/O逻辑类，必须被继承
	tcpserver_http	//基本HTTP类继承I/O类，必须被继承
	tcpserver_ws	//基本WebSocket类继承HTTP类，必须被继承
	
//启动tcp服务端
tcpserver(function()
{
		$this->thread_class = 'pt'; //设置逻辑线程上下文
		$this->io_class = 'demo_http'; //设置服务器PHP逻辑处理类
		$this->concurrent_thread = 4; //设置并发行线程数量，建议服务器CPU*2(默认0由服务端自行判断)
		$this->local_socket = 'ssl://*:8014'; //监听本地socket地址， * 代表同时监听IPv6和IPv4地址， 0.0.0.0 或 [::]
		$this->max_connected = 1000; //服务端最大连接数，不能超过20万
		//$this->kick_timeout = 600; //踢除长时间未数据收发的链接
		//$this->set_ssl('./test.cer','./test.key'); //使用SSL加密通信
		//添加定时器
		//$this->add_timer(function(){
		//	while(1) {
		// 		//每个2秒显示一次在线数
		//		sleep(5);
		//		$this->console_log('Current online %d', $this->get_online());
		//	}
		//}, time() + 5 );
});
</pre>
