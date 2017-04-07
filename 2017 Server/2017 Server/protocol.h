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

enum PACKET_TYPE
{
	INIT,
	DISCONNECT_PLAYER,
	MOVE,

};

#define KEY_UP		0b00000001
#define KEY_DOWN	0b00000010
#define KEY_LEFT	0b00000100
#define KEY_RIGHT	0b00001000

#pragma pack(push, 1)	// push 하며 1 바이트 단위로 정렬

using sc_packet_init = struct SERVER_TO_CLIENT_PACKET_INIT
{
	BYTE size = { sizeof(BYTE) + sizeof(BYTE) + sizeof(unsigned long long) + sizeof(POS) };
	BYTE type = { INIT };
	unsigned long long id = { 0 };
	POS pos;
};

using sc_packet_player_disconnect = struct SERVER_TO_CLIENT_PLAYER_DISCONNECT
{
	BYTE size = { sizeof(BYTE) + sizeof(BYTE) + sizeof(unsigned long long) };
	BYTE type = { DISCONNECT_PLAYER };
	unsigned long long id = { 0 };
};

using sc_packet_move = struct SERVER_TO_CLIENT_PACKET_MOVE
{
	BYTE size = { sizeof(BYTE) + sizeof(BYTE) + sizeof(unsigned long long) + sizeof(POS) };
	BYTE type = { MOVE };
	unsigned long long id = { 0 };
	POS pos;
};

using cs_packet_move = struct CLIENT_TO_PACKET_MOVE {
	BYTE size = { sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE) };
	BYTE type = { MOVE };
	BYTE input_key = { KEY_DOWN };
};

#pragma pack(pop)		// 원래 정렬값 복원