#include "stdafx.h"

/**
* @todo
* @param
* @return
*/
void formatPrint(char* src) {
	char line[81];
	int len = strlen(src);
	int i = 0;

	int j;
	for (j = 0; j < 70 && i < len; j++) {
		line[j] = src[i++];
	}
	while (line[j - 1] != ' ' && i != len) {
		j--; i--;
	}
	line[j] = 0;
	cout << line;
	if (i < len) cout << endl;

	while (i < len) {
		int j;
		for (j = 0; j < 80 && i < len; j++) {
			line[j] = src[i++];
		}
		while (line[j - 1] != ' ' && i != len) {
			j--; i--;
		}
		line[j] = 0;
		cout << line;
		if (i < len) cout << endl;
	}
}

void displayQuestion(gameStatus* status) {
	system("CLS");
	cout << "==========================================================================" << endl;
	cout << "\t\t\tWho Wants to Be a Millionaire?" << endl;
	cout << "\t\t\t\tIT version" << endl;
	cout << "==========================================================================" << endl << endl;
	cout << "Question " << (status->currentQuestionIndex + 1) << " ~ $" << status->nextPrize
		<< ". If you miss this question, you will go home with $" << (status->currPrize / 2) << "." << endl;
	cout << "Enter S/s to walk away with $" << status->currPrize << "." << endl;
	cout << "Enter A, B, C or D to answer the question." << endl;
	if (status->phoneHelp || status->audienceHelp || status->eliminationHelp) {
		cout << "Enter H/h to use your remaining lifeline: ";
		if (status->phoneHelp) {
			cout << "| Phone ";
		}
		if (status->audienceHelp) {
			cout << "| Audience ";
		}
		if (status->eliminationHelp) {
			cout << "| 50:50 ";
		}
		cout << "|" << endl;
	}
	else {
		cout << "You have used all of your lifeline." << endl;
	}
	cout << "--------------------------------- GOOD LUCK ------------------------------" << endl << endl;
	cout << "Question " << (status->currentQuestionIndex + 1) << ": ";
	formatPrint(status->currentQuestion.questionContent);
	cout << endl;
	for (int i = 0; i < 4; i++) {
		cout << "\t" << status->currentQuestion.answer[i] << endl;
	}
}

/**
* @todo
* @param
* @return
*/
bool getHelpOption(gameStatus *stt, message *mess) {
	memcpy(mess->payload, stt->currentQuestion.questionId, sizeof(stt->currentQuestion.questionId));
	cout << "\n\tYour remaining lifeline: ";
	if (stt->phoneHelp) {
		cout << "| Phone ";
	}
	if (stt->audienceHelp) {
		cout << "| Audience ";
	}
	if (stt->eliminationHelp) {
		cout << "| 50:50 ";
	}
	cout << "|" << endl;

	if (stt->phoneHelp) {
		cout << "\t\tEnter P/p to make a phone call to Mr. TungBT." << endl;
	}
	if (stt->audienceHelp) {
		cout << "\t\tEnter A/a to ask the audiences." << endl;
	}
	if (stt->eliminationHelp) {
		cout << "\t\tEnter E/e to eliminate two wrong answers." << endl;
	}
	cout << "\t\tEnter C/c to cancel." << endl;
	while (true) {
		cout << "\tYour option is: ";
		string input;
		getline(cin, input);
		char c;
		if (input.size() > 1) {
			c = 'x';
		}
		else {
			c = input[0];
		}

		switch (c) {
		case 'C':
		case 'c': {
			return false;
		}
		case 'P':
		case 'p': {
			if (stt->phoneHelp) {
				mess->messageCode = PHONE_HELP;
				return true;
			}
			else {
				cout << "\tInvalid option!!!" << endl;
				break;
			}
		}
		case 'A':
		case 'a': {
			if (stt->audienceHelp) {
				mess->messageCode = AUDIENCE_HELP;
				return true;
			}
			else {
				cout << "\tInvalid option!!!" << endl;
				break;
			}
		}
		case 'E':
		case 'e': {
			if (stt->eliminationHelp) {
				mess->messageCode = ELIMINATION_HELP;
				return true;
			}
			else {
				cout << "\tInvalid option!!!" << endl;
				break;
			}
		}
		default: {
			cout << "\tInvalid option!!!" << endl;
		}
		}

	}
}

/**
* @todo
* @param
* @return
*/
void handleUserChoice(gameStatus *status, message *mess) {
	while (true) {
		cout << "\tYour answer is: ";
		string input;
		getline(cin, input);
		char c;
		if (input.size() > 1) {
			c = 'x';
		}
		else {

			c = input[0];
		}
		switch (c) {
		case 'A':
		case 'a': {
			answer ans;
			ans.choice = 0;
			mess->messageCode = ANSWER;
			memcpy(ans.questionId, status->currentQuestion.questionId, sizeof(ans.questionId));
			memcpy(mess->payload, (char*)&ans, sizeof(answer));
			return;
		}
		case 'B':
		case 'b': {
			answer ans;
			ans.choice = 1;
			mess->messageCode = ANSWER;
			memcpy(ans.questionId, status->currentQuestion.questionId, sizeof(ans.questionId));
			memcpy(mess->payload, (char*)&ans, sizeof(answer));
			return;
		}
		case 'C':
		case 'c': {
			answer ans;
			ans.choice = 2;
			mess->messageCode = ANSWER;
			memcpy(ans.questionId, status->currentQuestion.questionId, sizeof(ans.questionId));
			memcpy(mess->payload, (char*)&ans, sizeof(answer));
			return;
		}
		case 'D':
		case 'd': {
			answer ans;
			ans.choice = 3;
			mess->messageCode = ANSWER;
			memcpy(ans.questionId, status->currentQuestion.questionId, sizeof(ans.questionId));
			memcpy(mess->payload, (char*)&ans, sizeof(answer));
			return;
		}
		case 'H':
		case 'h': {
			if (status->phoneHelp || status->audienceHelp || status->eliminationHelp) {
				if (getHelpOption(status, mess)) {
					return;
				}
				displayQuestion(status);
				break;
			}
			else {
				cout << "\tYou have used all of your lifeline." << endl;
				break;
			}
		}
		case 's':
		case 'S': {
			mess->messageCode = STOP;
			return;
		}
		default: {
			cout << "\tInvalid option!!!" << endl;
		}
		}
	}
}

/**
* @todo
* @param
* @return
*/
void displayHelpResponse(gameStatus * status, message*helpRes) {
	switch (helpRes->messageCode) {
	case PHONE_HELP: {
		phoneHelpResponse *res = (phoneHelpResponse*)helpRes->payload;
		cout << "\n\tMr. TungBT said the correct answer is ";
		switch (res->key) {
		case 0: {
			cout << "A" << endl;
			break;
		}
		case 1: {
			cout << "B" << endl;
			break;
		}
		case 2: {
			cout << "C" << endl;
			break;
		}
		case 3: {
			cout << "D" << endl;
			break;
		}
		}
		cout << "\tBe careful who you trust @_@" << endl;
		break;
	}
	case ELIMINATION_HELP: {
		system("CLS");
		cout << "==========================================================================" << endl;
		cout << "\t\t\tWho Wants to Be a Millionaire?" << endl;
		cout << "\t\t\t\tIT version" << endl;
		cout << "==========================================================================" << endl << endl;
		cout << "Question " << (status->currentQuestionIndex + 1) << " ~ $" << status->nextPrize
			<< ". If you miss this question, you will go home with $" << (status->currPrize / 2) << "." << endl;
		cout << "Enter S/s to walk away with $" << status->currPrize << "." << endl;
		cout << "Enter A, B, C or D to answer the question." << endl;
		if (status->phoneHelp || status->audienceHelp || status->eliminationHelp) {
			cout << "Enter H/h to use your remaining lifeline: ";
			if (status->phoneHelp) {
				cout << "| Phone ";
			}
			if (status->audienceHelp) {
				cout << "| Audience ";
			}
			if (status->eliminationHelp) {
				cout << "| 50:50 ";
			}
			cout << endl;
		}
		else {
			cout << "You have used all of your lifeline." << endl;
		}
		cout << "----------------------------- GOOD LUCK ----------------------------------" << endl << endl;
		cout << "Question " << (status->currentQuestionIndex + 1) << ": " ;
		formatPrint(status->currentQuestion.questionContent);
		cout << endl;

		eliminationHelpResponse *res = (eliminationHelpResponse*)helpRes->payload;
		for (int i = 0; i < 4; i++) {
			if (i != res->elimination1 && i != res->elimination2) {
				cout << "\t" << status->currentQuestion.answer[i] << endl;
			}
		}

		break;
	}
	case AUDIENCE_HELP: {
		audienceHelpResponse *res = (audienceHelpResponse*)helpRes->payload;
		cout << endl;
		cout << "\tAudience choosing A: " << res->rate[0] * 100 << "%" << endl;
		cout << "\tAudience choosing B: " << res->rate[1] * 100 << "%" << endl;
		cout << "\tAudience choosing C: " << res->rate[2] * 100 << "%" << endl;
		cout << "\tAudience choosing D: " << res->rate[3] * 100 << "%" << endl;
		break;
	}
	default: {
	}
	}
}

/**
* @todo
* @param
* @return
*/
void displayGameStatus(gameStatus *status) {
	if (status->playing) {
		if (status->currentQuestionIndex < 15 && status->currentQuestionIndex > 0) {
			cout << "\n\tCorrect answer!!!\n\tYou got $" << status->currPrize << endl;
			cout << "\tEnter to go to next question and get $" << status->nextPrize << "...";
			cin.get();

		}
		else if (status->currentQuestionIndex >= 15) {
			cout << endl << "---------------------------- YOU WON $1 Million --------------------------- " << endl;
		}
	}
	else {
		cout << "-------------------------------- END GAME --------------------------------" << endl;
		cout << "\tThe correct answer is ";
		switch (status->currentQuestion.key) {
		case 0: {
			cout << "A" << endl;
			break;
		}
		case 1: {
			cout << "B" << endl;
			break;
		}
		case 2: {
			cout << "C" << endl;
			break;
		}
		case 3: {
			cout << "D" << endl;
			break;
		}
		}
		cout << "\tYou will go home with $" << status->currPrize << endl;
	}
}

/**
* @todo
* @param
* @return
*/
void startGame(SOCKET client) {
	system("CLS");
	cout << "==========================================================================" << endl;
	cout << "\t\t\tWho Wants to Be a Millionaire?" << endl;
	cout << "\t\t\t\tIT version" << endl;
	cout << "==========================================================================" << endl << endl;
	cout << "\tShow the rules" << endl;
	cout << "\tEnter to start...";
	cin.get();
	if (!sendMessageWithType(client, START)) {
		cout << "\tFailed to request to server." << endl;
		return;
	}
	
	message mess;
	if (!recvMessage(client, &mess)) {
		cout << "\tFailded to receive response from server. " << endl;
		return;
	} 

	gameStatus status;
	memcpy(&status, mess.payload, sizeof(gameStatus));

	while (status.playing && status.currentQuestionIndex < 15) {
		displayGameStatus(&status);
		displayQuestion(&status);

		handleUserChoice(&status, &mess);
		char submit = mess.messageCode;

		if (!sendMessage(client, &mess)) {
			cout << "\tConnection interrupted." << endl;
			getchar();
			return;
		}

		if (!recvMessage(client, &mess)) {
			cout << "\tConnection interrupted." << endl;
			getchar();
			return;
		}

		while (submit != ANSWER && submit != STOP) {
			displayHelpResponse(&status, &mess);
			handleUserChoice(&status, &mess);
			submit = mess.messageCode;

			if (!sendMessage(client, &mess)) {
				cout << "\tConnection interrupted." << endl;
				getchar();
				return;
			}

			if (!recvMessage(client, &mess)) {
				cout << "\tConnection interrupted." << endl;
				getchar();
				return;
			}

			displayHelpResponse(&status, &mess);
		}

		memcpy(&status, mess.payload, sizeof(gameStatus));
	}
	
	displayGameStatus(&status);
	cout << "\n\tEnter any key to continue: ";
	getchar();

}

/**
* @todo
* @param
* @return
*/
void createUserInterface(SOCKET client) {
	while (true) {
		system("CLS");
		cout << "==========================================================================" << endl;
		cout << "\t\t\tWho Wants to Be a Millionaire?" << endl;
		cout << "\t\t\t\tIT version" << endl;
		cout << "==========================================================================" << endl << endl;
		cout << "\t1. Start new game" << endl;
		cout << "\t2. Exit" << endl;
		cout << "\n\tYour option is: ";
		string input;
		getline(cin, input);
		char c;
		if (input.size() > 1) {
			c = '3';
		}
		else {
			c = input[0];
		}
		switch (c) {
		case '1': {
			// play request 
			startGame(client);
			break;
		}
		case '2': {
			// exit request
			cout << "\tExit called." << endl;
			return;
		}
		default: {
			// invalip option from user
			break;
		}
		}
	}
}