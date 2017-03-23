#pragma once
class for_debug
{
public:
	for_debug();
	~for_debug();

	void out(wchar_t*);
private:
	int x{ 25 };
	int y{ 85 };
	int size{ 0 };
	const int end_of_y{ 725 };
	const int max_size{ 32 };
	const int raito{ 20 };
	const RECT debug_zone = { 20, 80, 350, 748 };
};