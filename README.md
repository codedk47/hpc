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
//DEMO
//暂时没有手册自己讲究去理解吧
// print_r(get_class_methods('thread'));
// print_r(get_class_methods('tcpserver'));
// print_r(get_class_methods('tcpserver_io'));
// print_r(get_class_methods('tcpserver_http'));
// print_r(get_class_methods('tcpserver_ws'));
//线程扩展类
class demo_thread extends thread
{
	function ctor()
	{
		//你可以在这里设置线程的上下文
		// $this->sql = @mysqli_connect('127.0.0.1', 'root', '', 'webapp');
		// if(!$this->sql)
		// {
		// 	$this->server()->console_log(mysqli_connect_error());
		// 	return FALSE;
		// }
		// $this->server()->console_log('Thread ID:%d startup', $this->id());

		// $this->client = @stream_socket_client('tcp://127.0.0.1:1688');
		// if(!$this->client)
		// {
		// 	return FALSE;
		// }

		return TRUE;
	}

	// function sql_get_name(string $name, $io)
	// {
	// 	$result = $this->sql->query('select * from rx_hr_staff where username="'.$name.'"');
	// 	if($result)
	// 	{
	// 		while($row = $result->fetch_assoc())
	// 		{
	// 			$io->send(join("\n",$row));
	// 		}
	// 	}
	// }
}

//演示 I/O 服务端
class demo_io extends tcpserver_io
{
	function __construct()
	{
		//$this->set_channel('world');
		//$this->server()->console_log('%d -> %s connected', $this->id(), $this->ip_info());
		return TRUE;
	}
	function __destruct()
	{
		//$this->server()->console_log('%d -> %s disconnected', $this->id(), $this->ip_info());
	}
	function recv()
	{
		//传输到一个PHP流（该方法暂时注释掉了，想了一下在无法全局保存资源情况下这个方法几乎没用）
		// $this->transfer_to_php_stream($this->thread()->client);
		// $context = stream_get_contents($this->thread()->client);
		// if($context)
		// {
		// 	var_dump(bin2hex($context));
		// 	$this->write($context);
		// }
		// while(1)
		// {
		// 	$len = $this->read($buf, 255);
		// 	var_dump($len);
		// 	if($len>0)
		// 	{
		// 		var_dump($buf);
		// 		$this->write($buf);
		// 	}
		// 	sleep(1);
		// }

		//测试简单的echo压力
		$len = $this->read($buf, 8014);
		$this->write($buf);
		return TRUE;
	}
}

//演示 http 服务端
class demo_http extends tcpserver_http
{
 	function recv_req()
	{

		// return $this->send('hello php server!!!');

		// $this->thread()->sql_get_name(10092, $this);

		// $this->send(sprintf('<h1>Current server time is %d</h1>', $this->server()->get_clock() ));

		// $this->res_type('text/plain; charset=utf-8');
		// $this->send_echo(function(){
		// 	var_dump($this->get_cookie());
		// 	var_dump($this->get_cookie('server'));
		// 	var_dump($this->get_cookies());

		// 	var_dump($this->req_query());
		// 	var_dump($this->req_query('aa'));
		// 	var_dump($this->req_querys());
		// });

		// $this->send_echo('phpinfo', INFO_MODULES);

		if($this->req_file()=='/ss')
		{
			return $this->get_content(3200000000,function() //测试接收3G大小数据
			{

				$this->send($this->get_content_offset().'--ok');

				//var_dump($this->get_content_buffer()); //千万不要接收大数据打印到屏幕上！！！你懂的

				$q=$this->get_content_format();
				print_r($q);

				if(isset($q['upfile']) && get_class($q['upfile']) == 'tcpserver_http_uploaded_file')
				{
					var_dump( $q['upfile']->image_type() );
					//var_dump( stream_get_contents($q['upfile']->file) );
					//var_dump($q['upfile']->move_to('new.txt'));
				}


				return TRUE;
			});
		}
		else
		{
			$this->send_file($this->req_file('index.html'));
		}

 		return TRUE;
	}
}

//演示 websocket 服务端
class demo_ws extends tcpserver_ws
{
	function recv_frame()
	{
		// var_dump($this->frame_hi());
		// var_dump($this->frame_fin());
		// var_dump($this->frame_opcode());
		// var_dump($this->frame_length());
		// var_dump($this->frame_mask());
		// var_dump($this->frame_context());
		// $this->thread()->sql_get_name($this->frame_context(), $this);

		$buffer = $this->frame_content();

		$encode_frame = $this->encode_frame($buffer);
		//发送帧
		// $this->send($buffer);

		//连续帧发送
		// $this->send($buffer, 1, FALSE);
		// $this->send($buffer, 0, FALSE );
		// $this->send($buffer, 0, TRUE);

		//发送给所有连接
		// $this->server()->send_all($encode_frame);

		//发送给指定id连接
		//$this->server()->send_here($encode_frame,[999, 998]);

		//发送给所在频道的所有连接
		//$this->server()->send_channel($encode_frame,'world');

		//发送给除了我以外的所有连接
		$this->send_them($encode_frame);

		return TRUE;
	}
}

//启动tcp服务端
tcpserver(function()
{
	// $this->work_root = 'work'; //设置线程上下文目录（当前服务端路径下的文件夹）
	$this->thread_class = 'demo_thread'; //设置逻辑线程上下文
	$this->io_class = 'demo_http'; //设置服务器PHP逻辑处理类
	// $this->concurrent_thread = 0; //设置并发行线程数量，建议服务器CPU*2(默认0由服务端自行判断,最大不能超过CPU*4)
	$this->local_socket = 'ssl://*:8014'; //监听本地socket地址，* 代表同时监听IPv6和IPv4地址， 0.0.0.0 或 [::]
	// $this->max_connected = 10000; //服务端最大连接数，不能超过20万
	// $this->kick_timeout = 900; //踢除长时间未数据收发的连接（0为不开启检测）


	// $this->set_ssl('./test.cer','./test.key'); //使用SSL加密通信

	//添加定时器
	// $this->add_timer(function()
	// {
	// 	while(1)
	// 	{
	// 		sleep(2); //每个2秒显示一次在线数
	// 		$this->console_log('Current online %d', $this->get_online());
	// 	}
	// }, time() + 5 ); //服务器启动后5秒开始运行

	//添加自定义取消完成端口某个连接
	// $this->add_timer(function()
	// {
	// 	while(1)
	// 	{
	// 		sleep(2);
	// 		$this->kick_id(999); //专门取消个 id 采用的是静态socket表 id 是 从 0 到 最大连接数的，自己可以实现查数据库踢出特定id 保持服务端连接干净
	// 	}
	// }); //服务器启动后5秒开始运行
});

</pre>
