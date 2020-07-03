// Task1_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char** argv)
{
	// step 00: validate the command parameter
	int serverPort;
	if (argc == 2) {
		serverPort = validateServerPort(argv[1]);
		if (serverPort == -1) {
			cout << "Bad parameter!!!" << endl;
			return 1;
		}
	}
	else {
		cout << "Usage: " << argv[0] << " <server port>" << endl;
		cout << "Example: " << argv[0] << " 5500";
		return 1;
	}

	SOCKADDR_IN serverAddr;
	SOCKET listenSock;
	HANDLE completionPort;
	SYSTEM_INFO systemInfo;
	WSADATA wsaData;

	if (WSAStartup((2, 2), &wsaData) != 0) {
		printf("WSAStartup() failed with error %d\n", GetLastError());
		return 1;
	}

	// Step 1: Setup an I/O completion port
	if ((completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL) {
		printf("CreateIoCompletionPort() failed with error %d\n", GetLastError());
		return 1;
	}

	// Step 2: Determine how many processors are on the system
	GetSystemInfo(&systemInfo);

	// Step 3: Create worker threads based on the number of processors available on the
	// system. Create two worker threads for each processor	
	for (int i = 0; i < (int)systemInfo.dwNumberOfProcessors * 2; i++) {
		// Create a server worker thread and pass the completion port to the thread
		if (_beginthreadex(0, 0, serverWorkerThread, (void*)completionPort, 0, 0) == 0) {
			printf("Create thread failed with error %d\n", GetLastError());
			return 1;
		}
	}

	// Step 4: Create a listening socket
	if ((listenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		printf("WSASocket() failed with error %d\n", WSAGetLastError());
		return 1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(serverPort);
	if (bind(listenSock, (PSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("bind() failed with error %d\n", WSAGetLastError());
		return 1;
	}

	// Prepare socket for listening
	if (listen(listenSock, 20) == SOCKET_ERROR) {
		printf("listen() failed with error %d\n", WSAGetLastError());
		return 1;
	}

	cout << getTime() << ": Server started!!!" << endl;
	cout << getTime() << ": The server is now ready to accept connections on port " << serverPort << endl;
	_beginthreadex(0, 0, serverStatusReporter, 0, 0, 0);

	// communicate with clients
	communicateWithClients(listenSock, completionPort);

	// step 05: close listen socket
	closesocket(listenSock);

	// step 06: terminate winsock
	WSACleanup();

	return 0;
}