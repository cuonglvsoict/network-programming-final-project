#pragma once
#include <WinSock2.h>
#include "datastruct.h"

int portNumberValid(char* port);

bool sendMessageWithType(SOCKET connSock, mess_type type);
bool recvMessage(SOCKET connSock, message *message);
bool sendMessage(SOCKET connSock, message *message);
