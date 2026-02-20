#pragma once
#include <string>

struct Node;
class ABCUCourse;

class BinarySearchTree
{
    // Variables
private:
    Node* rootNode;

    // Methods
public:
    BinarySearchTree(); // Constructor
    void AddNode(Node* node, ABCUCourse newCourse); // Adds a node with course data to tree in proper location
    void PrintCourse(ABCUCourse course); // Prints a courses formatted data
    void PrintIndividual(std::string courseNumber); // Prints an invididual course based on input course number
    void PrintAll(Node* node); // Prints all courses in the tree in alphanumeric order
    inline Node* GetRoot() { return this->rootNode; } // Returns ths rootNode
    void LoadCoursesCSV(std::string csvPath);
    void LoadCoursesDB(const char* directory);
};
