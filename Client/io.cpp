#include "stdafx.h"

/*
* @todo wrap function for sending data to server
* @param data: char pointer points to the data buff
* @param dataSize: size of data
* @param client: the connection socket
* @return true if sending ok, return false in the otherwise
*/
bool sendData(char* data, int dataSize, SOCKET client) {
	if (send(client, data, dataSize, 0) == SOCKET_ERROR) {
		return false;
	}
	return true;
}

/*
* @todo wrap function for receiving data from server
* @param data: char pointer points to the data buff
* @param dataSize: size of data
* @param client: the connection socket
* @return true if receiving ok, return false in the otherwise
*/
bool recvData(char* data, int dataSize, SOCKET client) {
	if (recv(client, data, dataSize, 0) == SOCKET_ERROR) {
		return false;
	}
	return true;
}