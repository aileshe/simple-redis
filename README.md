# simple-redis
simple-redis  简单的Redis C++封装类

## 下载安装:
```
git clone https://github.com/aileshe/simple-redis.git
```

## 使用 DEMO:
```
#include <iostream>
#include "SimpleRedis.h"
using namespace std;

int main()
{
	try{
		//SimpleRedis *rd = new SimpleRedis();
		//SimpleRedis *rd = new SimpleRedis("127.0.0.1", 6379);

		struct timeval timeout = { 1, 500000 }; // 1.5 seconds
		SimpleRedis *rd = new SimpleRedis("127.0.0.1", 6379, timeout);

		redisReply* reply = NULL;

		/* PING server */
		reply = rd->exec("PING");
		cout<<reply->str<<endl;

		/* Set a key */
		reply = rd->exec("SET %s %s", "foo", "helloword!");
		cout<<reply->str<<endl;
		
		
		// 释放内存
		delete rd;
		rd = NULL;
		reply = NULL;
	}
	catch(const exception &e)
	{
		cerr<<"Get exception : "<<e.what()<<endl;
	}


	return 0;
}

// Linux 下编译:
// g++ main.cpp SimpleRedis.cpp -I ./ -I /usr/local/include/hiredis/ -l hiredis -std=c++11

```
## 联系方式
Author: Dejan

QQ: 673008865
