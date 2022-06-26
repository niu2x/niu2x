ping
====

lua api
-------
- animal_ping_setup:
	启动ping所需要的线程池（只有1个线程），后续如果同时发起多个ping调用，他们会放进队列里依次执行.

- animal_ping_cleanup:
	等待所有任务完成，并关闭销毁线程池；
	如果当前有任务未完成，会阻塞；期间阻止提交新任务;

- animal_ping_busy:
	当前线程池是否有ping任务正在或等待执行;
	为false时，再调用animal_ping_cleanup 应该不会阻塞


- animal_ping_icmp(const char* host, int ttl, int cb):
	提交一个ping任务到线程池, 内部会找设备上的ping命令，起子进程

	- host: 域名或ip
	- ttl: ping 的ttl值
	- cb: lua callback
- animal_ping_tcp(const char* host, unsigned short port, int cb):
	提交一个ping任务到线程池, tcp connect成功就算成功，不发送接受任何数据；

	- host:
	- port:
	- cb: lua callback

- animal_ping_http(const char* url, int cb):
	提交一个ping任务到线程池，通过curl 发起get请求访问url

	- url:
	- cb: lua callback


lua callback 参数:
------------------
- status: 

	- 0 是成功，其他是错误码
- timecost: 

	- -1: 无意义
	- > 0: 耗时 (失败的情况下也可能有值，代表经历了XX时间后，连接确认失败了)

- dnstimecost:

	- -1: 无意义
	- > 0: (only valid for tcp ping) dns 解析耗时

- filesize:

	- -1: 无意义
	- > 0: (only valid for http ping) http请求get到的响应的字节数;
	- 可以结合timecost，算下载速度





note
----
- 为了方便，所有cpp在uv_wrapper.cpp中include, 这样不用去修改各个平台的编译配置



