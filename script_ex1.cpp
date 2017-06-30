#include <iostream>
#include <cstdio>

using namespace std;

extern "C" {
#include "include\lua.h"
#include "include\lauxlib.h"
#include "include\lualib.h"
}

int main() {
	int row, col;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, "script.lua");
	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, "rows"); // rows �� ���� stack �� ���
	lua_getglobal(L, "cols"); // cols �� ���� stack �� ��´�.

	row = (int)lua_tonumber(L, -2); // row �� �ڿ� �ι��� �����Ƿ� -2
	col = (int)lua_tonumber(L, -1); // col �� ž �������� -1 ��° �����Ƿ� -1
	// top �� 0, 0���� �ƹ� �����͵� ����.

	cout << "Rows " << row << " Cols " << col << endl;

	lua_pop(L, 2);
	lua_close(L);
}