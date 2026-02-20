#include "BinarySearchTree.h"
#include "ABCUCourse.h"
#include "DatabaseHelper.h"

#include <iostream>
#include <fstream>


using namespace std;

// Internal node structure for the binary search tree
struct Node {

    // Variables
    ABCUCourse course; // Stores the course data
    Node* leftChild; // Pointer to this nodes left child
    Node* rightChild; // Pointer to this nodes right child

    // Methods
    Node()
    {
        // Default constructor
        // Set children to nullptr indicating they're empty
        this->leftChild = nullptr;
        this->rightChild = nullptr;
    }

    Node(ABCUCourse newCourse)
    {
        // Constructor with new course
        // Set this node course to input course
        this->course = newCourse;

        // Set children to nullptr indicating they're empty
        this->leftChild = nullptr;
        this->rightChild = nullptr;
    }
};

BinarySearchTree::BinarySearchTree()
{
    // Makes rootNode nullptr indicating tree is empty
    this->rootNode = nullptr;
}

void BinarySearchTree::AddNode(Node* node, ABCUCourse newCourse)
{
    // If tree is empty (rootNode is nullptr) set root node as new node with input course
    if (this->rootNode == nullptr)
    {
        this->rootNode = new Node(newCourse);
    }
    else // Tree is not empty
    {
        // If input node course number is greater than new course number, new course goes left
        if (node->course.number > newCourse.number)
        {
            if (node->leftChild == nullptr) // If left child is empty
            {
                // Set new course as left child
                node->leftChild = new Node(newCourse);
            }
            else // If node has left child
            {
                // Call AddNode recursively on the left child
                AddNode(node->leftChild, newCourse);
            }
        }
        else // new course number is greater than node course number, new course goes right
        {
            if (node->rightChild == nullptr) // If right child is empty
            {
                // Set new course as right child
                node->rightChild = new Node(newCourse);
            }
            else // If node has right child
            {
                // Call AddNode recursively on the right child
                AddNode(node->rightChild, newCourse);
            }
        }
    }
}

void BinarySearchTree::PrintCourse(ABCUCourse course)
{
    cout << course.number << " | " << course.name;
    if (!course.prerequisites.empty())
    {
        cout << " |";
        for (int i = 0; i < course.prerequisites.size(); i++)
        {
            cout << " " << course.prerequisites.at(i) << ",";
        }
    }
    cout << endl;
}

void BinarySearchTree::PrintIndividual(string courseNumber)
{
    // Search for the node with the course number starting with root node
    Node* currentNode = this->rootNode;

    while (currentNode != nullptr) // Indicates end of a branch
    {
        if (currentNode->course.number == courseNumber) // If course was found
        {
            // Print the course and return
            cout << endl << "Number | Name | Prerequisites" << endl;
            PrintCourse(currentNode->course);
            return;
        }

        if (courseNumber < currentNode->course.number) // If course number to find is less than current node number
        {
            currentNode = currentNode->leftChild; // Check left child
        }
        else
        {
            currentNode = currentNode->rightChild; //Check right child
        }
    }
    // Print that course was no found
    cout << "Course with number " << courseNumber << " was not found." << endl;
}

void BinarySearchTree::PrintAll(Node* node)
{
    if (node != nullptr) // Indicates end of branch reached
    {
        PrintAll(node->leftChild); // Print the leaves to left of current node
        PrintCourse(node->course); // Print this node
        PrintAll(node->rightChild); // Print leaves to right of current node
    }
}

void BinarySearchTree::LoadCoursesCSV(std::string csvPath)
{
    cout << "Loading CSV file " << csvPath << endl;
    
    // Input stream to read the csv file
    ifstream inCourses;

    // Vector of courses data from the csv file
    vector<string> newCourses;

    // Open the input file
    inCourses.open(csvPath);

    // If file is not open
    if (!inCourses.is_open())
    {
        // Print error message and return
        cout << "ERROR: Could not open " << csvPath << endl;
        return;
    }

    // To store each line
    string line;

    // Read each line to the newCourses vector until end of file
    while (getline(inCourses, line))
    {
        newCourses.push_back(line);
    }

    // Close the file
    inCourses.close();

    // Go through each course in newCourses and add to the bst
    for (int i = 0; i < newCourses.size(); i++)
    {
        // Parse course string into a usable vector of strings.
        string unparsedCourse = newCourses.at(i);

        vector<string> parsedCourse;

        string currentData = "";
        for (int j = 0; j < unparsedCourse.size(); j++)
        {
            if (unparsedCourse[j] != ',')
            {
                currentData += unparsedCourse[j];
            }
            else // End of piece of data
            {
                // Push the data to the vector and reset currentData value if it exists
                if (currentData != "") parsedCourse.push_back(currentData);
                currentData = "";
            }
        }

        // Add last bit of data if exists
        if (currentData != "") parsedCourse.push_back(currentData);

        // Set values to store data
        string number = "";
        string name = "";
        vector<string> prerequisites;

        for (int j = 0; j < parsedCourse.size(); j++)
        {
            if (number == "")
            {
                number = parsedCourse.at(j);
            }
            else if (name == "")
            {
                name = parsedCourse.at(j);
            }
            else // Only prerequisites left
            {
                prerequisites.push_back(parsedCourse.at(j));
            }
        }

        // Add new course to the bst with the new data
        AddNode(GetRoot(), ABCUCourse(number, name, prerequisites));
    }
}

void BinarySearchTree::LoadCoursesDB(const char* directory)
{

}
