#include "stdafx.h"

// global variables
map<SOCKET, session*> socket2session;
CRITICAL_SECTION criticalSection;

/*
* @todo check whether the param is a valid port number
* @param port: string contains port number
* @return if valid, return the port number with int type.
* if not valid, return -1
*/
int validateServerPort(char* serverPort) {
	// check whether the parameter is valid port number
	// if valid returns the port number
	// else return -1
	for (int i = 0; i < strlen(serverPort); i++) {
		if (!isdigit(serverPort[i])) {
			return -1;
		}
	}
	int result = atoi(serverPort);
	if (result > 65535) {
		return -1;
	}
	return result;
}

/**
* @todo
* @param
* @return
*/
char* getTime() {
	time_t my_time = time(NULL);
	char* result = ctime(&my_time);
	result[strlen(result) - 1] = 0;
	return result;
}

/**
* @todo
* @param
* @return
*/
int getPrizeInCorrectCase(int questionIndex) {
	switch (questionIndex) {
	case 0: return 500;
	case 1: return 1000;
	case 2: return 2000;
	case 3: return 3000;
	case 4: return 4000;
	case 5: return 5000;
	case 6: return 7000;
	case 7: return 10000;
	case 8: return 20000;
	case 9: return 30000;
	case 10: return 50000;
	case 11: return 100000;
	case 12: return 250000;
	case 13: return 500000;
	case 14: return 1000000;
	}
}

/*
* function
* @param
* return
*/
bool createNewGame(session* userSession) {
	userSession->status.audienceHelp = true;
	userSession->status.eliminationHelp = true;
	userSession->status.phoneHelp = true;
	userSession->status.currPrize = 0;
	userSession->status.nextPrize = getPrizeInCorrectCase(0);
	userSession->status.currentQuestionIndex = 0;
	userSession->status.playing = true;

	return generateQuestionSet(userSession->questionSet);
}

/*
* @todo handle request message
* @param session: the session of connection socket
* @param requestMess: a message received from client
* @param response: response message to send to client
* @return
*/
void handleRequestMessage(session* userSession, message* requestMess, message* responseMess) {
	// check the message code and call the corresponding handler
	switch ((*requestMess).messageCode) {
	case START: {
		// create a question set
		createNewGame(userSession);

		// wrap the first question
		responseMess->messageCode = QUESTION;
		memcpy(&userSession->status.currentQuestion, &userSession->questionSet[0], sizeof(question));
		userSession->status.currentQuestion.key = -1;
		memcpy(responseMess->payload, &userSession->status, sizeof(gameStatus));

		break;
	}
	case ANSWER: {
		// check the key
		// if right anser, send the next question
		// if wrong anser, send the ENDGAME message
		question ques = userSession->questionSet[userSession->status.currentQuestionIndex];
		answer *ans = (answer*)requestMess->payload;

		if (strcmp(ans->questionId, ques.questionId) == 0 && ans->choice == ques.key && userSession->status.playing) {
			// update money
			userSession->status.currPrize = userSession->status.nextPrize;
			userSession->status.nextPrize = getPrizeInCorrectCase(userSession->status.currentQuestionIndex + 1);

			// get the next question
			memcpy(&userSession->status.currentQuestion,
				&userSession->questionSet[++userSession->status.currentQuestionIndex],
				sizeof(question));
			userSession->status.currentQuestion.key = -1;

			responseMess->messageCode = QUESTION;
			memcpy(responseMess->payload, &userSession->status, sizeof(gameStatus));
		}
		else {
			// update status
			userSession->status.currPrize = userSession->status.currPrize / 2;
			userSession->status.playing = false;
			responseMess->messageCode = END_GAME;
			userSession->status.currentQuestion.key = ques.key;
			memcpy(responseMess->payload, &userSession->status, sizeof(gameStatus));
		}

		break;
	}
	case STOP: {
		question ques = userSession->questionSet[userSession->status.currentQuestionIndex];
		answer *ans = (answer*)requestMess->payload;

		userSession->status.playing = false;
		responseMess->messageCode = END_GAME;
		userSession->status.currentQuestion.key = ques.key;
		memcpy(responseMess->payload, &userSession->status, sizeof(gameStatus));
		break;
	}
	case PHONE_HELP: {
		userSession->status.phoneHelp = false;
		question ques = userSession->questionSet[userSession->status.currentQuestionIndex];
		phoneHelpResponse response;
		memcpy(response.questionId, ques.questionId, sizeof(response.questionId));

		srand((unsigned)time(NULL));
		if ((double)rand() / (double)RAND_MAX < 0.9) {
			response.key = ques.key;
		}
		else {
			for (int i = 0; i < 4; i++) {
				if (i != ques.key) {
					response.key = i;
					break;
				}
			}
		}

		responseMess->messageCode = PHONE_HELP;
		memcpy(responseMess->payload, &response, sizeof(phoneHelpResponse));

		break;
	} 
	case ELIMINATION_HELP: {
		userSession->status.eliminationHelp = false;
		question ques = userSession->questionSet[userSession->status.currentQuestionIndex];
		eliminationHelpResponse response;
		memcpy(response.questionId, ques.questionId, sizeof(response.questionId));

		for (int i = 0; i < 4; i++) {
			if (i != ques.key) {
				response.elimination1 = i;
				for (int j = i + 1; j < 4; j++) {
					if (j != ques.key) {
						response.elimination2 = j;
						break;
					}
				}
				break;
			}
		}

		responseMess->messageCode = ELIMINATION_HELP;
		memcpy(responseMess->payload, &response, sizeof(eliminationHelpResponse));

		break;
	}
	case AUDIENCE_HELP: {
		userSession->status.audienceHelp = false;
		question ques = userSession->questionSet[userSession->status.currentQuestionIndex];
		audienceHelpResponse response;
		memcpy(response.questionId, ques.questionId, sizeof(response.questionId));

		srand((unsigned)time(NULL));
		double sum = 0;
		for (int i = 0; i < 4; i++) {
			response.rate[i] = (double)rand() / (double)RAND_MAX;
			sum += response.rate[i];
		}

		if (ques.level < 3) {
			response.rate[ques.key] += 0.8;
			sum += 0.8;
		}

		for (int i = 0; i < 4; i++) {
			response.rate[i] /= sum;
		}

		responseMess->messageCode = AUDIENCE_HELP;
		memcpy(responseMess->payload, &response, sizeof(audienceHelpResponse));

		break;
	}
	default: {
		responseMess->messageCode = INVALID;
		break;
	}
	}
}

/**
* @todo
* @param
* @return
*/
unsigned __stdcall serverStatusReporter(void *param) {

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS_EX pmc;
	DWORD processID = GetCurrentProcessId();

	while (true) {
		Sleep(5000);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, processID);
		GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
		cout << getTime() << ": * " << socket2session.size() << " clients connected, " << pmc.PrivateUsage << " bytes in use" << endl;
	}
}

void destroySession(SOCKET socket) {
	session *userSession = socket2session[socket];
	EnterCriticalSection(&criticalSection);
	socket2session.erase(socket);
	LeaveCriticalSection(&criticalSection);
	cout << getTime() << ": Session on socket " << socket << " destroyed" << endl;
	if (closesocket(socket) != SOCKET_ERROR) {
		cout << getTime() << ": Socket " << socket << " closed" << endl;
	}
	free(userSession);
}

/**
* @todo
* @param
* @return
*/
unsigned __stdcall serverWorkerThread(LPVOID completionPortID)
{
	HANDLE completionPort = (HANDLE)completionPortID;
	DWORD transferredBytes;
	LPPER_HANDLE_DATA perHandleData;
	LPPER_IO_OPERATION_DATA perIoData;
	DWORD flags;

	while (TRUE) {
		if (GetQueuedCompletionStatus(completionPort, &transferredBytes,
			(LPDWORD)&perHandleData, (LPOVERLAPPED *)&perIoData, INFINITE) == 0) {
			cout << getTime() << ": Connection on socket " << perHandleData->socket << " aborted" << endl;
			destroySession(perHandleData->socket);
			continue;
		}

		// Check to see if an error has occurred on the socket and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket
		if (transferredBytes == 0) {
			cout << getTime() << ": Connection on socket " << perHandleData->socket << " finished" << endl;
			destroySession(perHandleData->socket);
			continue;
		}

		// Check to see if the operation field equals RECEIVE. If this is so, then
		// this means a WSARecv call just completed so update the recvBytes field
		// with the transferredBytes value from the completed WSARecv() call
		if (perIoData->operation == RECEIVE) {
			perIoData->recvBytes += transferredBytes;

			if (perIoData->recvBytes < sizeof(message)) {
				// Post another WSARecv() request.
				// Since WSARecv() is not guaranteed to send all of the bytes requested,
				// continue posting WSARecv() calls until all bytes received
				ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
				perIoData->dataBuff.buf = perIoData->buffer + perIoData->recvBytes;
				perIoData->dataBuff.len = sizeof(message) - perIoData->recvBytes;
				perIoData->operation = RECEIVE;

				if (WSARecv(perHandleData->socket,
					&(perIoData->dataBuff),
					1,
					&transferredBytes,
					0,
					&(perIoData->overlapped),
					NULL) == SOCKET_ERROR) {
					if (WSAGetLastError() != ERROR_IO_PENDING) {
						cout << getTime() << ": WSARecv() failed on socket " << perHandleData->socket << " with error " << GetLastError() << endl;
						destroySession(perHandleData->socket);
						continue;
					}
				}
			}
			else {
				message reqMess, resMess;
				memcpy(&reqMess, perIoData->buffer, sizeof(message));
				handleRequestMessage(socket2session[perHandleData->socket], &reqMess, &resMess);

				memcpy(perIoData->buffer, (char*)&resMess, sizeof(message));

				perIoData->sentBytes = 0;
				perIoData->operation = SEND;
			}
		}
		else {
			perIoData->sentBytes += transferredBytes;
		}

		if (perIoData->sentBytes < sizeof(message)) {
			// Post another WSASend() request.
			// Since WSASend() is not guaranteed to send all of the bytes requested,
			// continue posting WSASend() calls until all received bytes are sent.
			ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
			perIoData->dataBuff.buf = perIoData->buffer + perIoData->sentBytes;
			perIoData->dataBuff.len = sizeof(message) - perIoData->sentBytes;
			perIoData->operation = SEND;

			if (WSASend(perHandleData->socket,
				&(perIoData->dataBuff),
				1,
				&transferredBytes,
				0,
				&(perIoData->overlapped),
				NULL) == SOCKET_ERROR) {
				if (WSAGetLastError() != ERROR_IO_PENDING) {
					cout << getTime() << ": WSASend() failed on socket " << perHandleData->socket << " with error " << GetLastError() << endl;
					destroySession(perHandleData->socket);
					continue;
				}
			}
		}
		else {
			// No more bytes to send post another WSARecv() request
			perIoData->recvBytes = 0;
			perIoData->operation = RECEIVE;
			flags = 0;
			ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
			perIoData->dataBuff.len = BUFF_SIZE;
			perIoData->dataBuff.buf = perIoData->buffer;
			if (WSARecv(perHandleData->socket,
				&(perIoData->dataBuff),
				1,
				&transferredBytes,
				&flags,
				&(perIoData->overlapped), NULL) == SOCKET_ERROR) {
				if (WSAGetLastError() != ERROR_IO_PENDING) {
					cout << getTime() << ": WSARecv() failed on socket " << perHandleData->socket << " with error " << GetLastError() << endl;
					destroySession(perHandleData->socket);
					continue;
				}
			}
		}
	}
}

/**
* @todo communicate with client
* @param listenSock: the listen socket
* @param completionPort: I/O completion port
* @return
*/
void communicateWithClients(SOCKET listenSock, HANDLE completionPort) {

	SOCKET acceptSock;
	LPPER_HANDLE_DATA perHandleData;
	LPPER_IO_OPERATION_DATA perIoData;
	DWORD transferredBytes;
	DWORD flags;

	InitializeCriticalSection(&criticalSection);

	while (true) {
		// Accept connections
		if ((acceptSock = WSAAccept(listenSock, NULL, NULL, NULL, 0)) == SOCKET_ERROR) {
			cout << getTime() << ": WSAAccept() failed  with error " << GetLastError() << endl;
			continue;
		}

		// Create a socket information structure to associate with the socket
		if ((perHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL) {
			cout << getTime() << ": GlobalAlloc() failed with error " << GetLastError() << endl;
			continue;
		}

		// Associate the accepted socket with the original completion port
		cout << getTime() << ": Socket number " << acceptSock << " got connected" << endl;
		perHandleData->socket = acceptSock;
		if (CreateIoCompletionPort((HANDLE)acceptSock, completionPort, (DWORD)perHandleData, 0) == NULL) {
			cout << getTime() << ": CreateIoCompletionPort() failed with error " << GetLastError() << endl;
			continue;
		}

		// Create per I/O socket information structure to associate with the WSARecv call
		if ((perIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL) {
			cout << getTime() << ": GlobalAlloc() failed with error " << GetLastError() << endl;
			continue;
		}

		ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
		perIoData->sentBytes = 0;
		perIoData->recvBytes = 0;
		perIoData->dataBuff.len = BUFF_SIZE;
		perIoData->dataBuff.buf = perIoData->buffer;
		perIoData->operation = RECEIVE;
		flags = 0;

		if (WSARecv(acceptSock, &(perIoData->dataBuff), 1, &transferredBytes, &flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				cout << getTime() << ": WSARecv() failed with error " << GetLastError() << endl;
				continue;
			}
		}

		session *userSession = (session*)calloc(1, sizeof(session));
		memcpy(&userSession->clientSock, &acceptSock, sizeof(acceptSock));
		EnterCriticalSection(&criticalSection);
		socket2session.insert({ acceptSock, userSession });
		LeaveCriticalSection(&criticalSection);
	}

	DeleteCriticalSection(&criticalSection);
}