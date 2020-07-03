#pragma once
#include <WinSock2.h>
#include "datastruct.h"

bool sendData(char* data, int dataSize, SOCKET client);
bool recvData(char* data, int dataSize, SOCKET client);
