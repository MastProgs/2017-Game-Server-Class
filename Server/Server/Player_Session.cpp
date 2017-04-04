#pragma once
#include "stdafx.h"

Player_Session::Player_Session(SOCKET s, bool connected, unsigned long long id) : m_s{ move(s) }, m_b_connected{ connected }, m_id{ id }
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

bool Player_Session::get_connect_state()
{
	return m_b_connected;
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
	PACKET_TYPE event_type = *reinterpret_cast<PACKET_TYPE *>(&m_buf.buf[1]);

	switch (event_type)
	{
	case MOVE: {

		break;
	}
	default:
		break;
	}
}
