#include "DatabaseHelper.h"
#include "ABCUCourse.h"
#include "sqlite3.h"

#include <string>
#include <vector>

using namespace std;

vector<ABCUCourse> DatabaseHelper::courses = {};

int DatabaseHelper::createDB(const char* s)
{
    sqlite3* DB;
    int exit = 0;

    exit = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}

int DatabaseHelper::createTable(const char* s)
{
    sqlite3* DB;

    string sql = "CREATE TABLE IF NOT EXISTS courses ("
        "number TEXT PRIMARY KEY, "
        "name TEXT NOT NULL );"
        "CREATE TABLE IF NOT EXISTS prerequisites ("
        "course_number TEXT NOT NULL, "
        "prereq_number TEXT NOT NULL, "
        "FOREIGN KEY(course_number) REFERENCES courses(number) );";

    try
    {
        int exit = 0;
        char* messageError;

        exit = sqlite3_open(s, &DB);

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

        if (exit != SQLITE_OK) {
            cerr << "Error Create Table: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else
            cout << "Table created Successfully" << endl;
    }
    catch (const exception& e)
    {
        cerr << e.what();
    }

    return 0;
}

int DatabaseHelper::insertData(const char* s)
{
    sqlite3* DB;
    int exit = 0;
    char* messageError;

    exit = sqlite3_open(s, &DB);

    string sql("INSERT INTO courses (number, name) VALUES"
        "('MATH201', 'Discrete Mathematics'), "
        "('CSCI300', 'Introduction to Algorithms'), "
        "('CSCI350', 'Operating Systems'), "
        "('ENGL202', 'English Literature and Composition'), "
        "('CSCI101', 'Introduction to Programming in C++'), "
        "('CSCI100', 'Introduction to Computer Science'), "
        "('CSCI301', 'Advanced Programming in C++'), "
        "('MUSI202', 'Music Composition'), "
        "('GAMM202', 'Game Design and Development'), "
        "('CSCI400', 'Large Software Development'), "
        "('CSCI200', 'Data Structures'), "
        "('MATH202', 'Simple Mathematics'), "
        "('ENGL201', 'English Language and Literature'), "
        "('MUSI201', 'Music Theory'), "
        "('GAMM201', 'Game Theory'), "
        "('SCIV201', 'Scientific Visualization'), "
        "('SOPH201', 'Sophomore Seminar'); "
        "INSERT INTO prerequisites (course_number, prereq_number) VALUES"
        "('CSCI300', 'CSCI200'), ('CSCI300', 'MATH201'), ('CSCI350', 'CSCI300'), ('ENGL202', 'ENGL201'), ('CSCI101', 'CSCI100'), ('CSCI301', 'CSCI101'), ('MUSI202', 'MUSI201'), ('GAMM202', 'GAMM201'), ('CSCI400', 'CSCI301'), ('CSCI400', 'CSCI350'), ('CSCI200', 'CSCI101'), ('MATH202', 'MATH201'); ");

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Data already inserted: " << messageError << endl;
        sqlite3_free(messageError);
    }
    else
        cout << "Data inserted Successfully" << endl;

    return 0;
}

int DatabaseHelper::selectCourses(const char* s, int (*callback)(void*, int, char**, char**))
{
    sqlite3* DB;

    int exit = sqlite3_open(s, &DB);

    string sql = "SELECT * FROM courses";

    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

    return 0;
}

int DatabaseHelper::selectPrereqs(const char* s, const string course,int(*callback)(void*, int, char**, char**))
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);

    string sql = "SELECT * FROM prerequisites WHERE course_number = '" + course + "'";

    sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

    if (exit != SQLITE_OK) {
        cerr << "selectPrereqs Error: " << messageError << endl;
        sqlite3_free(messageError);
    }
    else
        // cout << "Prerequisites selected Successfully" << endl;

    return 0;
}

int DatabaseHelper::loadCourses(void* NotUsed, int argc, char** argv, char** azColName)
{
    ABCUCourse newCourse;
    for (int i = 0; i < argc; i++) {
        if (i % 2 == 0) {
            cout << "number: " << argv[i] << endl;
            newCourse.number = argv[i];
        }
        else {
            cout << "name: " << argv[i] << endl;
            newCourse.name = argv[i];
            courses.push_back(newCourse);
        }
    }

    return 0;
}

int DatabaseHelper::loadPrereqs(const char* s, int(*callback)(void*, int, char**, char**))
{
    for (auto course : courses)
    {
        selectPrereqs(s, course.number, callback);
    }
    return 0;
}

int DatabaseHelper::loadPrereqsCallback(void* NotUsed, int argc, char** argv, char** azColName)
{
    string courseNumber = argv[0];
    int courseIndex = -1;
    
    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.at(i).number == courseNumber)
        {
            courseIndex = i;
            break;
        }
    }
    
    if (courseIndex == -1) return 1;
    
    for (int i = 0; i < argc; i++) {
        if (i % 2 != 0) {
            courses.at(courseIndex).prerequisites.push_back(argv[i]);
        }
    }
    
    return 0;
}