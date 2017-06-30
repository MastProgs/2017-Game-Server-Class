#include <iostream>
#include <cstdio>

using namespace std;

extern "C" {
#include "include\lua.h"
#include "include\lauxlib.h"
#include "include\lualib.h"
}

int main() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, "script.lua");
	lua_pcall(L, 0, 0, 0);	// 일단 실행 한번은 해야 내부 변수 값들이 정의 된다.

	lua_getglobal(L, "plustwo"); // 함수를 읽어 놓음
	lua_pushnumber(L, 100);	// 함수 인자 인풋값 스택에 담아 놓음
	lua_pcall(L, 1, 1, 0);	// 인자 하나, 결과 값 하나 ( 리턴 스택 말고는 전부 pop )

	int result = (int)lua_tonumber(L, -1);

	cout << "Result : " << result << endl;

	lua_pop(L, 1);
	lua_close(L);
}