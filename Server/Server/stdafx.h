#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <time.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <mutex>
#include <iostream>
#include <vector>
#include <thread>
//#include <future>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

#include "protocol.h"
#include "Player_Session.h"
#include "IOCP.h"
#include "win_main_class.h"
