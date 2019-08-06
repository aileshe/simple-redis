/**
* SimpleRedis - 简单的 hiredis 封装类
* @Author: Dejan - QQ:673008865
* @Include: hiredis
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include "SimpleRedis.hpp"
using namespace std;

int main(int argc, char **argv)
{
	try
	{
		unique_ptr<SimpleRedis> redis(new SimpleRedis);
		redisReply* reply = NULL;

		/* PING server */
		reply = redis->exec("PING");
		cout << reply->str << endl;

		/* Set a key */
		reply = redis->exec("SET %s %s", "food", "hello word!");
		cout << reply->str << endl;

		/* Get key */
		reply = redis->exec("GET %s", "food");
		cout << reply->str << endl;

		/* 发布与订阅 (pub/sub) */
		// 测试发布: > publish _TEST_ "hi Dejan!"
		reply = redis->exec("subscribe _TEST_");
		redis->freeReply();
		while (redisGetReply(redis->get(), (void **)&reply) == REDIS_OK)
		{
			if (NULL == reply) return 0;

			if (reply->type == REDIS_REPLY_ARRAY)
			{
				for (int i = 0; i < reply->elements; i++)
				{
					printf("[%d] => %s \n", i, reply->element[i]->str);
				}
			}
		}

		// 无需做任何清理... 就这么简单!!  -- Dejan
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}


	getchar();
	return 0;
}