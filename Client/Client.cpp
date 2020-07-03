// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char** argv)
{
	int serverPort;
	struct in_addr serverAddrInput;
	// step 0: validate the parameter
	if (argc == 3) {
		serverPort = portNumberValid(argv[2]);
		serverAddrInput.s_addr = inet_addr(argv[1]);
	}
	if (argc != 3 || serverPort<0 || serverAddrInput.s_addr == INADDR_NONE) {
		cout << "Usage: " << argv[0] << " <server address> <server port>" << endl;
		return 1;
	}

	// step 1: initiate winsock
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		cout << "Winsock version not supported!!!";
		return 1;
	}
	cout << "Client started!!!" << endl;

	// step 2: construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// set time-out for receiving = 10 secs
	int tv = 10000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(int));

	// step 03: specify the server address
	sockaddr_in serverAddr;
	serverAddr.sin_addr.s_addr = serverAddrInput.s_addr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);

	// step 04: request to establish a connection to server
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		cout << "Error! Can not connect to server: " << GetLastError() << endl;
		WSACleanup();
		return 1;
	}
	cout << "Server connected! " << endl;

	// step 05: interacts with user via user interface
	createUserInterface(client);

	// step 6: close socket
	closesocket(client);

	// step 7: terminate winsock
	WSACleanup();

	return 0;
}