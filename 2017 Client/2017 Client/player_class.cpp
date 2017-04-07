#include "stdafx.h"

player_class::player_class()
{
}

player_class::~player_class()
{
}

void player_class::process_packet(Packet *buf)
{
	char type = buf[1];

	switch (type)
	{
	case MOVE: {
		sc_packet_move packet = *reinterpret_cast<sc_packet_move*>(buf);

		if (packet.id != m_id) {
			m_other_players.insert(make_pair(packet.id, packet.pos));
			return;
		}

		m_pos = packet.pos;

		break;
	}
	case DISCONNECT_PLAYER: {
		sc_packet_player_disconnect packet = *reinterpret_cast<sc_packet_player_disconnect*>(buf);
		m_other_players.erase(packet.id);
		break;
	}
	case INIT: {

		sc_packet_init packet = *reinterpret_cast<sc_packet_init*>(&buf[2]);
		m_id = packet.id;
		m_pos = packet.pos;

		break;
	}
	default:
		break;
	}
}

int player_class::key_input(WPARAM keyboard)
{
	switch (keyboard)
	{
	case VK_UP: {
		return KEY_UP;
		break;
	}
	case VK_DOWN: {
		return KEY_DOWN;
		break;
	}
	case VK_LEFT: {
		return KEY_LEFT;
		break;
	}
	case VK_RIGHT: {
		return KEY_RIGHT;
		break;
	}
	case VK_ESCAPE: {
		PostQuitMessage(0);
		return 0;
	}
	default:
		return 0;
		break;
	}
}
