#pragma once
#include "stdafx.h"

IOCP::IOCP()
{
	_wsetlocale(LC_ALL, L"korean");
	m_b_debug_mode = true;

	get_server_IP();
	get_cpu_core();

	init_server();
	create_threads();
}


IOCP::~IOCP()
{
	release_IOCP();
}

char * IOCP::get_server_IP() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct hostent* hostinfo;
	char hostname[50];
	memset(hostname, 0, sizeof(hostname));
	memset(m_server_ip, 0, sizeof(m_server_ip));

	int nError = gethostname(hostname, sizeof(hostname));
	if (nError == 0)
	{
		hostinfo = gethostbyname(hostname);
		strcpy(m_server_ip, inet_ntoa(*reinterpret_cast<struct in_addr*>(hostinfo->h_addr_list[0])));
	}
	WSACleanup();

	if (true == m_b_debug_mode) { printf("Server IP Address = %s\n", m_server_ip); }	
	return m_server_ip;
}

void IOCP::init_server() {

	m_b_server_shut_down = false;
	m_clients.reserve(MAX_USER);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	if (NULL == m_hiocp) { err_quit(L"IOCP::Init_Server", WSAGetLastError()); }

}

void IOCP::release_IOCP() {
	
	m_b_server_shut_down = true;

	for (auto player : m_clients) {
		if (nullptr != player) {
			if (player->get_connect_state()) { player->close_socket();	}
			delete player;
		}
	}

	WSACleanup();
}

void IOCP::accept_thread()
{
	int retval = { 0 };

	// socket() - IPv4 ( AF_INET )
	SOCKET listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == listen_sock) { err_quit(L"socket()", WSAGetLastError()); };

	// bind()
	struct sockaddr_in serveraddr = { 0 };
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = ::bind(listen_sock, reinterpret_cast<struct sockaddr *>(&serveraddr), sizeof(serveraddr));
	if (SOCKET_ERROR == retval) { err_quit(L"bind()", WSAGetLastError()); }

	// listen()
	int waiting_clients_limit = /*SOMAXCONN*/5;
	retval = listen(listen_sock, waiting_clients_limit);
	if (SOCKET_ERROR == retval) { err_quit(L"Listen()", WSAGetLastError()); }

	while (false == m_b_server_shut_down) {
		// accept()
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		SOCKET client_sock = WSAAccept(listen_sock, reinterpret_cast<sockaddr *>(&clientaddr), &addrlen, NULL, NULL);
		if (INVALID_SOCKET == client_sock) { err_display("Accept::WSAAccept", WSAGetLastError(), __LINE__, __FUNCTION__, -1); while (true);	}

		if (true == is_max_user()) {
			closesocket(client_sock);
			continue;
		}

		/* DB login check */

		m_ui_player_key += 1;
		display_client_info(m_ui_player_key, clientaddr);

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), m_hiocp, m_ui_player_key, 0);
		m_clients.emplace_back(new Player_Session(client_sock, true, m_ui_player_key));

		/* other client info send recv & view list */

		retval = m_clients[m_ui_player_key]->wsa_recv();
		if (SOCKET_ERROR == retval) {
			int err_no = WSAGetLastError();
			if (ERROR_IO_PENDING != err_no) { err_display("Accept::WSARecv", err_no, __LINE__, __FUNCTION__, m_ui_player_key); }
		}
	}
}

void IOCP::worker_thread()
{
	while (false == m_b_server_shut_down)
	{
		unsigned long long id = { 0 };
		DWORD io_size = { 0 };
		OVLP_EX *ovlp = { nullptr };

		bool result = GetQueuedCompletionStatus(m_hiocp, &io_size, &id, reinterpret_cast<LPOVERLAPPED *>(ovlp), 1000 * 60);
		if (false == result || 0 == io_size) {
			// GQCS 가 시간되어 return && server shut down 상태인 경우
			if (nullptr == ovlp && false == m_b_server_shut_down) { if (true == m_b_debug_mode) { printf("Worker Thread Returned !! \n"); } continue; }
			if (false == result) { err_display("GQCS()", GetLastError(), __LINE__, __FUNCTION__, id); }

			m_clients[id]->close_socket();
			if (true == m_b_debug_mode) { printf("Client No. %%3llu"); }

			/* send msg & check out view list */

			continue;
		}
		else if (E_RECV == ovlp->event_type) {
			int retval = m_clients[id]->packet_ressembly(io_size);
			if (SOCKET_ERROR == retval) {
				int err_no = WSAGetLastError();
				if (ERROR_IO_PENDING != err_no) { err_display("WorkerThreadStart::WSARecv", err_no, __LINE__, __FUNCTION__, id); }
			}
			continue;
		}
		else if (E_SEND == ovlp->event_type) {
			delete ovlp;
			continue;
		}
		else {
			if (true == m_b_debug_mode) { printf("Unknown IOCP event !!\n"); }
			exit(-1);
		}
	}
}

void IOCP::create_threads()
{
	vector<thread *> worker_threads;
	worker_threads.reserve(m_cpu_core);

	for (int i = 0; i < m_cpu_core; ++i)
	{
		worker_threads.emplace_back(new thread{ &IOCP::worker_thread, this });
	}

	thread acceptThread{ &IOCP::accept_thread, this };
	while (true == m_b_server_shut_down) { Sleep(1000); }

	for (auto thread : worker_threads) {
		thread->join();
		delete thread;
	}

	acceptThread.join();
}

void IOCP::err_quit(wchar_t *msg, int err_no) {
	//if (false == m_b_debug_mode) { return; }

	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void IOCP::err_display(char *msg, int err_no, int line, char *func, int id) {
	if (false == m_b_debug_mode) { return; }

	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s [ %s - %d ] Client No. %3llu ", func, msg, line, id);
	wprintf(L"에러 %s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

short IOCP::get_cpu_core()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_cpu_core = static_cast<int>(si.dwNumberOfProcessors);
	if (true == m_b_debug_mode) { printf("CPU Core Count = %d, threads = %d\n", m_cpu_core, m_cpu_core * 2); }

	return m_cpu_core;
}

void IOCP::display_client_info(const unsigned int & key, const SOCKADDR_IN & addr_in)
{
	if (false == m_b_debug_mode) { return; }
	printf("[ No. %3llu ] Client IP = %s, Port = %d is Connected\n", key, inet_ntoa(addr_in.sin_addr), ntohs(addr_in.sin_port));
}

bool IOCP::is_max_user()
{
	if (MAX_USER <= m_ui_player_key + 1) {
		if (true == m_b_debug_mode) { printf("Max User Overflow!!\n"); }
		return true;
	}

	return false;
}
