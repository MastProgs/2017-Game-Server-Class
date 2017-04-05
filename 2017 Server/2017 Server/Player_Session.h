#pragma once

enum EVENT_TYPE { E_RECV, E_SEND };

using OVLP_EX = struct OVERLAP_EX
{
	WSAOVERLAPPED over = { 0 };
	WSABUF wsabuf = { 0 };
	Packet iocp_buf[MAX_BUF_SIZE] = { 0 };
	EVENT_TYPE event_type = { E_RECV };
};

class Player_Session
{
public:
	Player_Session(SOCKET s, bool connected, unsigned long long id, vector<Player_Session*> *);
	~Player_Session();

	int send_packet(const Packet *buf);
	int wsa_recv();
	bool get_connect_state();
	void close_socket();

	int packet_ressembly(DWORD packet_size);
private:
	POS m_pos;
	unsigned long long m_id = { 0 };
	bool m_b_connected = { false };

	SOCKET m_s = { 0 };
	OVLP_EX m_ovlp;
	BUFS m_buf;

	const vector<Player_Session*> * m_clients;

	// �ּ����� �������� �־ ����.
	mutex m_lock;

	void process_packet();
};

