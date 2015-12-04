/*
 * =====================================================================================
 *
 *  @file  btl_head.hpp
 *
 *  @brief 战斗服务器协议包头
 *
 *  compiler  gcc4.3.2 
 *	
 *  platform  Linux
 *
 * copyright:  TaoMee, Inc. ShangHai CN. All rights reserved
 *
 * =====================================================================================
 */
#ifndef btl_head_h__
#define btl_head_h__

enum
{
	/* 数据长度的最大值 */
	e_msg_len_max = 1024 * 8,
};

#pragma pack(1)

/* 跟battle交互的协议格式 */
struct btl_proto_head_t {
	uint32_t len; /* 协议的长度 */
	uint32_t seq_num; /* 序列号 */
	uint16_t cmd; /* 命令号 */
	uint32_t ret; /* DB返回的错误码 */
	uint32_t user_id; /* 米米号 */
	uint32_t pad; /* 校验值 */
	uint8_t body[]; /* 包体信息 */
};	   
 
//与客户端交互的协议
struct cli_proto_head_t {
	uint32_t len; /* 协议的长度 */
	uint16_t cmd; /* 协议的命令号 */
	uint32_t user_id; /* 用户的米米号 */
	uint32_t seq_num;/* 序列号 */
	uint32_t ret; /* S->C, 错误码 */
	uint8_t body[]; /* 包体信息 */
}__attribute__((packed));

#pragma pack()

#endif // btl_head_h__
