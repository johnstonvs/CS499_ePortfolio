#include "LinkedList.h"
#include "ABCUCourse.h"

#include <iostream>
#include <fstream>

using namespace std;

struct Node {
    ABCUCourse course;
    Node *next;

    // default constructor
    Node() {
        next = nullptr;
    }

    // initialize with a bid
    Node(ABCUCourse aCourse) {
        course = aCourse;
        next = nullptr;
    }
};

LinkedList::LinkedList() {
    //set head and tail equal to nullptr
    head = nullptr;
    tail = nullptr;
}

void LinkedList::Append(ABCUCourse course)
{
    // Create new node
    Node* newNode = new Node(course);

    // if there is nothing at the head...
    if (head == nullptr)
    {
        // new node becomes the head and the tail
        head = newNode;
        tail = newNode;
    }
    else
    {
        // make current tail node point to the new node
        tail->next = newNode;
        
        // and tail becomes the new node
        tail = newNode;
    }

    //increase size count
    size++;
}

void LinkedList::Prepend(ABCUCourse course)
{
    // Create new node
    Node* newNode = new Node(course);

    // if there is already something at the head...
    if (head != nullptr)
    {
        // new node points to current head as its next node
        newNode->next = head;
    }
    else
    {
        // Make new node the list tail.
        tail = newNode;
    }

    // head now becomes the new node
    head = newNode;

    // increase size count
    size++;
}

void LinkedList::PrintCourse(ABCUCourse course)
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

void LinkedList::PrintIndividual(std::string courseNumber)
{
    // start at the head of the list
    Node* currentNode = head;

    // While currentNode exists
    while (currentNode != nullptr)
    {
        // if the current node matches, return current bid
        if (currentNode->course.number == courseNumber)
        {
            PrintCourse(currentNode->course);
            return;
        }
        // else current node is equal to next node
        currentNode = currentNode->next;
    }

    // Print that course was no found
    cout << "Course with number " << courseNumber << " was not found." << endl;
}

void LinkedList::PrintAll()
{
    // start at the head of the list
    Node* currentNode = head;

    // While currentNode exists
    while (currentNode != nullptr)
    {
        // print current course
        PrintCourse(currentNode->course);
        
        // current node is equal to next node
        currentNode = currentNode->next;
    }
}

void LinkedList::LoadCourses(std::string csvPath)
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
    
    // Go through each course in newCourses and append to linked list
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
        
        // Add new course to the linked list
        Append(ABCUCourse(number, name, prerequisites));
    }
}
