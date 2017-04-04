#pragma once

#define MAX_BUF_SIZE 4000
#define MAX_PACKET_SIZE 255

#define MAX_USER 10

#define SERVER_PORT 9000

using Packet = unsigned char;

enum PACKET_TYPE
{
	MOVE,
};

#pragma pack(push, 1)	// push 하며 1 바이트 단위로 정렬

#pragma pack(pop)		// 원래 정렬값 복원