#include "stdafx.h"

/*
* @todo check whether the parm is a valid port number
* @param port: string contains port number
* @return if valid, return the port number with int type, if not valid, return -1
*/
int portNumberValid(char* port) {
	for (int i = 0; i < strlen(port); i++) {
		if (!isdigit(port[i])) {
			return -1;
		}
	}
	int serverPort = atoi(port);
	if (serverPort > 65535) {
		return -1;
	}
	else {
		return serverPort;
	}
}

/*
* @todo
* @param 
* @return
*/
bool sendMessageWithType(SOCKET connSock, mess_type type) {
	message mess;
	mess.messageCode = type;
	return sendData((char*)&mess, sizeof(message), connSock);
}

/*
* @todo
* @param
* @return
*/
bool recvMessage(SOCKET connSock, message *mess) {
	return recvData((char*)mess, sizeof(message), connSock);
}

/*
* @todo
* @param
* @return
*/
bool sendMessage(SOCKET connSock, message *mess) {
	return sendData((char*)mess, sizeof(message), connSock);
}