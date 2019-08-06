/**
* SimpleRedis - 简单的 hiredis 封装类
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
	/* 获取Redis连接句柄 */
	redisContext* get()
	{
		return mRedis;
	}

	/* 清理执行结果 */
	void freeReply()
	{
		if (mReply)
		{
			freeReplyObject(mReply);
			mReply = NULL;
		}
	}

	/* 执行任意命令 */
	template<typename... Args>
	redisReply* exec(const char* format, Args&&... args)
	{
		if('\0' == format[0])
			throw std::runtime_error("format is null!!\n");
		// 先清理一下上一次的结果
		freeReply();
		mReply = (redisReply*)redisCommand(mRedis, format, std::forward<Args>(args)...);
		return mReply;
	}

	/* 构造函数 */
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
		// 判断连接密码认证
		if (REDIS_PASS[0] != '\0')
		{
			mReply = (redisReply*)redisCommand(mRedis, "AUTH %s", REDIS_PASS);
			if (mReply->type == REDIS_REPLY_ERROR)
				throw std::runtime_error("Redis Authentication failed！\n");
			freeReply();
		}
	}

	/* 析构函数 */
	~SimpleRedis()
	{
		// 空间清理
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

