/**
* SimpleRedis - �򵥵� hiredis ��װ��
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

		/* �����붩�� (pub/sub) */
		// ���Է���: > publish _TEST_ "hi Dejan!"
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

		// �������κ�����... ����ô��!!  -- Dejan
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}


	getchar();
	return 0;
}