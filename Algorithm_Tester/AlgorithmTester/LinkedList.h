#pragma once
#include <string>

struct Node;
class ABCUCourse;

class LinkedList
{
    // Variables
private:
    Node* head;
    Node* tail;
    int size = 0;    
    
    // Methods
public:
    LinkedList();
    void Append(ABCUCourse course);
    void Prepend(ABCUCourse course);
    void PrintCourse(ABCUCourse course);
    void PrintIndividual(std::string courseNumber);
    void PrintAll();
    void LoadCourses(std::string csvPath);
    
};
