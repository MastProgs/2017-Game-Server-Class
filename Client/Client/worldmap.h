#pragma once
#include"stdafx.h"

#define MAX_MAP_SIZE 8

using position = struct Position
{
	int x{ 0 };
	int y{ 0 };
};

class Player
{
public:
	Player() { pos.x = 4; pos.y = 4; };
	~Player() {};

	void move_right() { if (!(pos.x > MAX_MAP_SIZE - 2)) { ++pos.x; } };
	void move_left() { if (!(pos.x < 1)) { --pos.x; } };
	void move_up() { if (!(pos.y < 1)) { --pos.y; } };
	void move_down() { if (!(pos.y > MAX_MAP_SIZE - 2)) { ++pos.y; } };

	position get_pos() const { return pos; }

private:
	position pos;
};

class worldmap {
public:
	worldmap() {};
	~worldmap() {};

	Player player;
private:
	int map[MAX_MAP_SIZE][MAX_MAP_SIZE]{ 0 };
};

static worldmap g_map;