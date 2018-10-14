/**
 * 简单的Redis
 * @Author: Dejan - QQ:673008865
 * @Include: hiredis
 */

#include "SimpleRedis.h"

/**
 * 默认连接
 * @description  host:127.0.0.1  port:6379  timeout: 1.5 seconds
 */
SimpleRedis::SimpleRedis() throw(std::runtime_error)
{
	rd_handle = redisConnect(rd_host, rd_port);
	if ( NULL == rd_handle || rd_handle->err )
	{
		if (rd_handle)
		{	
			std::string errstr = std::string("Connection error: ") + rd_handle->errstr;
			redisFree(rd_handle);
			rd_handle = NULL;
			throw std::runtime_error( errstr );
		}
		else
		{
			throw std::runtime_error("Connection error: can't allocate redis context\n");
		}
	}
}

/**
 * 有参构造1
 * @param  char* host   IP地址
 * @param  int   port   端口
 */
SimpleRedis::SimpleRedis(const char *host, const int port) throw(std::runtime_error)
	:rd_host(host),rd_port(port)
{
	rd_handle = redisConnect(rd_host, rd_port);
	if ( NULL == rd_handle || rd_handle->err )
	{
		if (rd_handle)
		{	
			std::string errstr = std::string("Connection error: ") + rd_handle->errstr;
			redisFree(rd_handle);
			rd_handle = NULL;
			throw std::runtime_error( errstr );
		}
		else
		{
			throw std::runtime_error("Connection error: can't allocate redis context\n");
		}
	}
}

/**
 * 有参构造2
 * @param  char*          host     IP地址
 * @param  int            port     端口
 * @param  struct timeval timeout  超时时间设置
 */
SimpleRedis::SimpleRedis(const char *host, const int port, const struct timeval timeout) throw(std::runtime_error)
	:rd_host(host),rd_port(port),rd_timeout(timeout)
{
	rd_handle = redisConnectWithTimeout(rd_host, rd_port, rd_timeout);
	if ( NULL == rd_handle || rd_handle->err )
	{
		if (rd_handle)
		{	
			std::string errstr = std::string("Connection error: ") + rd_handle->errstr;
			redisFree(rd_handle);
			rd_handle = NULL;
			throw std::runtime_error( errstr );
		}
		else
		{
			throw std::runtime_error("Connection error: can't allocate redis context\n");
		}
	}
}


/**
 * 执行入口函数
 * @param char* format,...  可变参数
 */
redisReply* SimpleRedis::exec(const char *format, ...) throw(std::runtime_error)
{
	if( NULL != rd_reply ) freeReplyObject(rd_reply);

	if ( NULL == format )
	{
		throw std::runtime_error("const char* format is null !");
	}

	char cmd_str[rd_buffer_len]; // "SET %s %s" => "SET [key] [value]"

	va_list args;
	va_start(args, format);
	vsprintf(cmd_str, format, args);
	va_end(args);

	rd_reply = (redisReply*)redisCommand(rd_handle, cmd_str);
	return rd_reply;
}

/**
 * 释放内存
 */
void SimpleRedis::destroy()
{
	if( NULL != rd_reply ){
		freeReplyObject(rd_reply);
		rd_reply = NULL;
	}
	if( NULL != rd_handle ){
		redisFree(rd_handle);
		rd_handle = NULL;
	}
}

/**
 * 析构函数
 */
SimpleRedis::~SimpleRedis()
{
	this->destroy();
}


