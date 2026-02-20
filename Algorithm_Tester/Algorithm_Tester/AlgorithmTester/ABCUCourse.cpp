#include "ABCUCourse.h"

using namespace std;

ABCUCourse::ABCUCourse()
{
    // Set values to 0 or empty indicating no data
    this->number = "0";
    this->name = "0";
    this->prerequisites = {};
}

ABCUCourse::ABCUCourse(string newNumber, string newName, vector<string> newPrerequisites)
{
    // Set this courses variables as the input variables
    this->number = newNumber;
    this->name = newName;
    this->prerequisites = newPrerequisites;
}