#include <iostream>
#include <cstdio>

using namespace std;

extern "C" {
#include "include\lua.h"
#include "include\lauxlib.h"
#include "include\lualib.h"
}

int addnum_c(lua_State *L) {
	int a = (int)lua_tonumber(L, -2);
	int b = (int)lua_tonumber(L, -1);
	int result = a + b;
	lua_pop(L, 3);
	lua_pushnumber(L, result);
	return 1;
}

int main() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, "script.lua");
	lua_pcall(L, 0, 0, 0);

	lua_register(L, "c_addnum", addnum_c); // 만약 c_addnum 을 실행하라는 명령이 있다면, addnum_c 함수를 찾아 실행하라, 라고 등록

	lua_getglobal(L, "addnum_lua");
	lua_pushnumber(L, 100);
	lua_pushnumber(L, 200);
	lua_pcall(L, 2, 1, 0);

	int result = (int)lua_tonumber(L, -1);
	cout << result << endl;

	lua_pop(L, 1);
	lua_close(L);
}