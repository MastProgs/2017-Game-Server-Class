#pragma once
#include "stdafx.h"

Player_Session::Player_Session(SOCKET s, bool connected, unsigned long long id, vector<Player_Session*> * p) : m_s{ s }, m_b_connected{ connected }, m_id{ id }, m_clients{ p }
{
	// initialize in here **

	m_ovlp.wsabuf.buf = reinterpret_cast<CHAR*>(m_ovlp.iocp_buf);
	m_ovlp.wsabuf.len = sizeof(m_ovlp.iocp_buf);
}


Player_Session::~Player_Session()
{

}

int Player_Session::send_packet(const Packet *buf)
{
	OVLP_EX *over = new OVLP_EX;
	over->event_type = E_SEND;
	over->wsabuf.buf = reinterpret_cast<char *>(over->iocp_buf);
	over->wsabuf.len = buf[0];
	memcpy(over->iocp_buf, buf, buf[0]);
	
	DWORD flags{ 0 };
	int retval = WSASend(m_s, &over->wsabuf, 1, NULL, flags, &over->over, NULL);
	if (SOCKET_ERROR == retval) {
		if (ERROR_IO_PENDING != WSAGetLastError()) {
			printf("Client No. %3llu Send Packet ERROR !!\n", m_id);
			while (true);
		}
	}
	return retval;
}

int Player_Session::wsa_recv() {
	DWORD flags = { 0 };
	return WSARecv(m_s, &m_ovlp.wsabuf, 1, NULL, &flags, &m_ovlp.over, NULL);
}

void Player_Session::close_socket()
{
	closesocket(m_s);

	m_b_connected = false;
}

int Player_Session::packet_ressembly(DWORD packet_size)
{
	Packet *buf_ptr = m_ovlp.iocp_buf;

	int remained = packet_size;
	while (0 < remained) {
		if (0 == m_buf.size_curr) { m_buf.size_curr = buf_ptr[0]; }
		int required = m_buf.size_curr - m_buf.size_prev;
		if (remained >= required) {
			memcpy(m_buf.buf + m_buf.size_prev, buf_ptr, required);
			process_packet();
			buf_ptr += required;
			remained -= required;
			m_buf.size_curr = 0;
			m_buf.size_prev = 0;
		}
		else {
			memcpy(m_buf.buf + m_buf.size_prev, buf_ptr, remained);
			buf_ptr += remained;
			m_buf.size_prev += remained;
			remained = 0;
		}
	}

	return wsa_recv();
}

void Player_Session::process_packet()
{
	char event_type = m_buf.buf[1];

	switch (event_type)
	{
	case MOVE: {

		cs_packet_move packet = *reinterpret_cast<cs_packet_move*>(&m_buf.buf[2]);

		if (packet.input_key & KEY_UP) { if (-1 != (m_pos.y - 1)) { --m_pos.y; }}
		else if (packet.input_key & KEY_DOWN) { if (MAX_MAP_SIZE != (m_pos.y + 1)) { ++m_pos.y; }}
		else if (packet.input_key & KEY_RIGHT) { if (MAX_MAP_SIZE != (m_pos.x + 1)) { ++m_pos.x; } }
		else if (packet.input_key & KEY_LEFT) { if (-1 != (m_pos.x - 1)) { --m_pos.x; } }

		sc_packet_move my_pos;
		my_pos.size = sizeof(my_pos);
		my_pos.id = m_id;
		my_pos.pos = m_pos;
		
		for (auto players : *m_clients) {
			if (false == players->get_connect_state()) { continue; }
			send_packet(reinterpret_cast<Packet*>(&my_pos));
		}

		break;
	}
	default:
		break;
	}
}
