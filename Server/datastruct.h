
#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE

#include "stdafx.h"

typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF dataBuff;
	CHAR buffer[BUFF_SIZE];
	int bufLen;
	int recvBytes;
	int sentBytes;
	int operation;
} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct {
	SOCKET socket;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

struct message {
	char messageCode;
	char payload[2048];
};

enum mess_type {
	START = 10,
	END_GAME = 11,
	READY = 12,
	QUIT = 13,

	QUESTION = 14,
	KEY = 15,
	RES_HELP = 16,

	ANSWER = 17,
	STOP = 18,
	PHONE_HELP = 19,
	AUDIENCE_HELP = 20,
	ELIMINATION_HELP = 21,

	INVALID = 22,
};

struct question {
	char questionId[10];
	int level;
	char questionContent[200];
	char answer[4][200];
	int key;
};

struct gameStatus {
	bool playing;
	int currentQuestionIndex;
	question currentQuestion;
	int currPrize;
	int nextPrize;
	bool phoneHelp;
	bool audienceHelp;
	bool eliminationHelp;
};

struct session {
	SOCKET clientSock;
	gameStatus status;
	question questionSet[15];
};

struct answer {
	char questionId[10];
	int choice;
};

struct key {
	char questionId[10];
	int questionKey;
};

struct phoneHelpResponse {
	char questionId[10];
	int key;
};

struct audienceHelpResponse {
	char questionId[10];
	float rate[4];
};

struct eliminationHelpResponse {
	char questionId[10];
	int elimination1;
	int elimination2;
};

#endif // !DATA_STRUCTURE