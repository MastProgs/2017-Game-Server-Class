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

	lua_getglobal(L, "rows"); // rows 를 먼저 stack 에 담고
	lua_getglobal(L, "cols"); // cols 를 먼저 stack 에 담는다.

	row = (int)lua_tonumber(L, -2); // row 가 뒤에 두번재 있으므로 -2
	col = (int)lua_tonumber(L, -1); // col 은 탑 기준으로 -1 번째 있으므로 -1
	// top 이 0, 0에는 아무 데이터도 없다.

	cout << "Rows " << row << " Cols " << col << endl;

	lua_pop(L, 2);
	lua_close(L);
}