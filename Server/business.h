#include "datastruct.h"
#define MAXIMUM_LOGIN_FAILED 3

int validateServerPort(char* serverPort);
char* getTime();
void communicateWithClients(SOCKET listenSock, HANDLE completionPort);
unsigned __stdcall serverWorkerThread(LPVOID completionPortID);
unsigned __stdcall serverStatusReporter(void * param);