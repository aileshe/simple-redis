/**
 * 简单的Redis
 * @Author: Dejan - QQ:673008865
 * @Include: hiredis
 */

#pragma once
#include <hiredis.h>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

class SimpleRedis
{
	public:
		SimpleRedis() throw(std::runtime_error);
		~SimpleRedis();
		SimpleRedis(const char *host, const int port) throw(std::runtime_error);
		SimpleRedis(const char *host, const int port, const struct timeval timeout)
			throw(std::runtime_error);
		redisReply* exec(const char *format, ...) throw(std::runtime_error);
		void destroy();

	private:
		const char *rd_host = "127.0.0.1"; // 主机IP
		const int rd_port = 6379; // 端口
		redisContext *rd_handle = NULL; // redis 连接句柄
		const struct timeval rd_timeout = { 1, 500000 }; // 1.5 seconds
		redisReply *rd_reply = NULL; // 执行回调
		const int rd_buffer_len = 1024; // buffer 长度
};
