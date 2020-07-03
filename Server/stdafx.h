// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 

#include "targetver.h"
#include "io.h"
#include "datastruct.h"
#include "business.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <time.h> 
#include <iostream>
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <process.h>
#include <psapi.h>

#define BUFF_SIZE 4096
#define RECEIVE 0
#define SEND 1

using namespace std;

// TODO: reference additional headers your program requires here