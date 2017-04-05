#pragma once

class IOCP
{
public:
	IOCP();
	~IOCP();

	bool m_b_debug_mode = { false };
	bool m_b_server_shut_down = { false };

	char* get_server_IP();
	void init_server();
	void stop_IOCP();

private:

	HANDLE m_hiocp = { 0 };
	char m_server_ip[50] = { 0 };
	short m_cpu_core = { 1 };
	unsigned long long m_ui_player_key = { _UI64_MAX };

	vector<Player_Session*> m_clients;

	void accept_thread();
	void worker_thread();
	void create_threads();
	void release_IOCP();

	void err_quit(wchar_t *msg, int err_no);
	void err_display(char *msg, int err_no, int line, char *func, int id);

	short get_cpu_core();
	void display_client_info(const unsigned int &key, const SOCKADDR_IN& addr_in);
	bool is_max_user();
	void shut_down_for_self_accept();
};

