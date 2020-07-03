#pragma once
#include <WinSock2.h>
#include "datastruct.h"

#define QUESTION_FILE_LEVEL1 "questions_level_1.txt"
#define QUESTION_FILE_LEVEL2 "questions_level_2.txt"
#define QUESTION_FILE_LEVEL3 "questions_level_3.txt"

bool generateQuestionSet(question* questionSet);