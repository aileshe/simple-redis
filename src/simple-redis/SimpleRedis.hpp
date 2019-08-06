/**
* SimpleRedis - �򵥵� hiredis ��װ��
* @Author: Dejan - QQ:673008865
* @Include: hiredis
*/

#pragma once
#include <hiredis/hiredis.h>
#include "Config.h"

#ifdef _MSC_VER // Windows
	#ifdef _WIN64
		#ifdef DEBUG
			#pragma comment(lib, "hiredis_x64d.lib")
		#else
			#pragma comment(lib, "hiredis_x64.lib")
		#endif // DEBUG
	#else // x32
		#ifdef DEBUG
			#pragma comment(lib, "hiredis_x86d.lib")
		#else
			#pragma comment(lib, "hiredis_x86.lib")
		#endif // DEBUG
	#endif // _WIN64
	#pragma comment(lib, "Ws2_32.lib")
#elif __GNUC__ // Linux
#endif

class SimpleRedis
{
public:
	/* ��ȡRedis���Ӿ�� */
	redisContext* get()
	{
		return mRedis;
	}

	/* ����ִ�н�� */
	void freeReply()
	{
		if (mReply)
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
	}

	/* ִ���������� */
	template<typename... Args>
	redisReply* exec(const char* format, Args&&... args)
	{
		if('\0' == format[0])
			throw std::runtime_error("format is null!!\n");
		// ������һ����һ�εĽ��
		freeReply();
		mReply = (redisReply*)redisCommand(mRedis, format, std::forward<Args>(args)...);
		return mReply;
	}

	/* ���캯�� */
	SimpleRedis()
	{
		mRedis = redisConnect(REDIS_HOST, REDIS_PORT);
		if (mRedis == NULL || mRedis->err) 
		{
			if (mRedis)
				throw std::runtime_error("Error: " + std::string(mRedis->errstr) + "\n");
			else
				throw std::runtime_error("Can't allocate redis context\n");
		}
		// �ж�����������֤
		if (REDIS_PASS[0] != '\0')
		{
			mReply = (redisReply*)redisCommand(mRedis, "AUTH %s", REDIS_PASS);
			if (mReply->type == REDIS_REPLY_ERROR)
				throw std::runtime_error("Redis Authentication failed��\n");
			freeReply();
		}
	}

	/* �������� */
	~SimpleRedis()
	{
		// �ռ�����
		freeReply();
		if (mRedis)
		{
			redisFree(mRedis);
			mRedis = NULL;
		}
	}

private:
	redisContext* mRedis = NULL;
	redisReply* mReply = NULL;
};

