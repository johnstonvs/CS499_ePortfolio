#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ABCUCourse;

class DatabaseHelper
{
public:
	static vector<ABCUCourse> courses;
	static int createDB(const char* s);
	static int createTable(const char* s);
	static int insertData(const char* s);
	static int selectCourses(const char* s, int (*callback)(void*, int, char**, char**));
	static int selectPrereqs(const char* s, const string course, int (*callback)(void*, int, char**, char**));
	static int loadCourses(void* NotUsed, int argc, char** argv, char** azColName);
	static int loadPrereqs(const char* s, int (*callback)(void*, int, char**, char**));
	static int loadPrereqsCallback(void* NotUsed, int argc, char** argv, char** azColName);
};

