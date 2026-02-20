#pragma once

#include <string>
#include <vector>

class ABCUCourse
{

public:
    // Variables
    std::string number; // Alphanumeric course number (Ex: MATH201)
    std::string name; // Course name or title (Ex: Discrete Mathematics)
    std::vector<std::string> prerequisites; // Vector of prerequisite course numbers

    // Constructors
    ABCUCourse();
    ABCUCourse(std::string newNumber, std::string newName, std::vector<std::string> newPrerequisites);
    
};