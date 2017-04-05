#pragma once

#define MAX_BUF_SIZE 4000
#define MAX_PACKET_SIZE 255

#define MAX_USER 10

#define SERVER_PORT 9000

#define MAX_MAP_SIZE 5

using Packet = unsigned char;

using POS = struct POSITION
{
	int x = { 0 };
	int y = { 0 };
};

using BUFS = struct PACKET_INFO
{
	Packet buf[MAX_PACKET_SIZE] = { 0 };
	int size_prev = { 0 };
	int size_curr = { 0 };
};

enum PACKET_TYPE
{
	MOVE,
};

#define KEY_UP		0b00000001
#define KEY_DOWN	0b00000010
#define KEY_LEFT	0b00000100
#define KEY_RIGHT	0b00001000

#pragma pack(push, 1)	// push 하며 1 바이트 단위로 정렬

using sc_packet_move = struct SERVER_TO_CLIENT_PACKET_MOVE
{
	BYTE size = { sizeof(POS) + sizeof(BYTE) + sizeof(BYTE) };
	BYTE type = { MOVE };
	POS pos;
};

#pragma pack(pop)		// 원래 정렬값 복원