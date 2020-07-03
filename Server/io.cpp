#include "stdafx.h"

/**
* @todo
* @param
* @return
*/
bool generateQuestionSet(question* questionSet) {
	ifstream fin1(QUESTION_FILE_LEVEL1);
	ifstream fin2(QUESTION_FILE_LEVEL2);
	ifstream fin3(QUESTION_FILE_LEVEL3);

	if (!fin1.is_open() || !fin2.is_open() || !fin3.is_open()) {
		cout << "Failed to generate question set." << endl;
		return false;
	}

	fin1.seekg(0, ios::end);
	int size1 = fin1.tellg() / sizeof(question);
	fin2.seekg(0, ios::end);
	int size2 = fin2.tellg() / sizeof(question);
	fin3.seekg(0, ios::end);
	int size3 = fin3.tellg() / sizeof(question);

	int index = 0;
	srand(time(NULL));

	while (index < 5) {
		bool flag = true;
		int pick = rand() % size1;
		fin1.seekg(pick * sizeof(question), ios::beg);
		fin1.read((char*)&questionSet[index], sizeof(question));
		for (int i = 0; i < index; i++) {
			if (!strcmp(questionSet[i].questionId, questionSet[index].questionId)) {
				flag = false;
				break;
			}
		}
		if (flag) index++;
	}

	while (index < 10) {
		bool flag = true;
		int pick = rand() % size2;
		fin2.seekg(pick * sizeof(question), ios::beg);
		fin2.read((char*)&questionSet[index], sizeof(question));
		for (int i = 5; i < index; i++) {
			if (!strcmp(questionSet[i].questionId, questionSet[index].questionId)) {
				flag = false;
				break;
			}
		}
		if (flag) index++;
	}

	while (index < 15) {
		bool flag = true;
		int pick = rand() % size3;
		fin3.seekg(pick * sizeof(question), ios::beg);
		fin3.read((char*)&questionSet[index], sizeof(question));
		for (int i = 10; i < index; i++) {
			if (!strcmp(questionSet[i].questionId, questionSet[index].questionId)) {
				flag = false;
				break;
			}
		}
		if (flag) {
			index++;
		}
	}

	fin1.close(); fin2.close(); fin3.close();
	return true;
}