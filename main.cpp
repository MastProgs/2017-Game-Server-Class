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
	lua_pcall(L, 0, 0, 0);	// �ϴ� ���� �ѹ��� �ؾ� ���� ���� ������ ���� �ȴ�.

	lua_getglobal(L, "plustwo"); // �Լ��� �о� ����
	lua_pushnumber(L, 100);	// �Լ� ���� ��ǲ�� ���ÿ� ��� ����
	lua_pcall(L, 1, 1, 0);	// ���� �ϳ�, ��� �� �ϳ� ( ���� ���� ����� ���� pop )

	int result = (int)lua_tonumber(L, -1);

	cout << "Result : " << result << endl;

	lua_pop(L, 1);
	lua_close(L);
}