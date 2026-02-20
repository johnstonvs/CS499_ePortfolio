#include "BinarySearchTree.h"
#include "LinkedList.h"
#include "DatabaseHelper.h"
#include "ABCUCourse.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
    const char* dir = "Database/course.db";

    DatabaseHelper::createDB(dir);
    DatabaseHelper::createTable(dir);
    DatabaseHelper::insertData(dir);
    DatabaseHelper::selectCourses(dir, DatabaseHelper::loadCourses);
    DatabaseHelper::loadPrereqs(dir, DatabaseHelper::loadPrereqsCallback);
    
    for (int i = 0; i < DatabaseHelper::courses.size(); i++)
    {
        cout << DatabaseHelper::courses.at(i).number << " " << DatabaseHelper::courses.at(i).name << endl;
        for (int j = 0; j < DatabaseHelper::courses.at(i).prerequisites.size(); j++)
        {
            cout << "    " << DatabaseHelper::courses.at(i).prerequisites.at(j) << endl;
        }
    }

    // Process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "Data/CS 300 ABCU_Advising_Program_Input.csv";
    }

    BinarySearchTree* bst = new BinarySearchTree();
    LinkedList* llist = new LinkedList();

    // TESTING
    bst->LoadCoursesDB(dir);

    int structChoice = 0;
    while (structChoice != 1 && structChoice != 2)
    {
        cout << endl << "Choose a Data Structure:" << endl;
        cout << "   1. Binary Search Tree" << endl;
        cout << "   2. Linked List" << endl;
        cout << "Enter choice: ";
        cin >> structChoice;
    }
    
    int choice = 0;
    string courseID;
    clock_t ticks;
    
    while (choice != 9)
        {
        cout << endl <<  (structChoice == 1 ? "Binary Search Tree Menu:" : "Linked List Menu:") << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Display Individual Course" << endl;
        cout << "  4. Choose Data Structure" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            switch (structChoice)
            {
            case 1:
                bst->LoadCoursesCSV(csvPath);
                break;
            case 2:
                llist->LoadCourses(csvPath);
                break;
            }
            choice = 0;
            break;

        case 2:
            cout << endl << "Number | Name | Prerequisites" << endl;
            cout << "=============================" << endl;
            switch (structChoice)
            {
            case 1:
                ticks = clock();
                bst->PrintAll(bst->GetRoot());
                ticks = clock() - ticks;
                cout << endl << "Elapsed time: " << ticks << " clock ticks" << endl;
                break;
            case 2:
                ticks = clock();
                llist->PrintAll();
                ticks = clock() - ticks;
                cout << endl << "Elapsed time: " << ticks << " clock ticks" << endl;
                break;
            }
            choice = 0;
            break;

        case 3:
            cout << "Enter course ID to find: ";
            cin >> courseID;
            switch (structChoice)
            {
            case 1:
                ticks = clock();
                bst->PrintIndividual(courseID);
                ticks = clock() - ticks;
                cout << endl << "Elapsed time: " << ticks << " clock ticks" << endl;
                break;
            case 2:
                ticks = clock();
                llist->PrintIndividual(courseID);
                ticks = clock() - ticks;
                cout << endl << "Elapsed time: " << ticks << " clock ticks" << endl;
                break;
            }
            choice = 0;
            break;
            
        case 4:
            structChoice = 0;
            while (structChoice != 1 && structChoice != 2)
            {
                cout << endl << "Choose a Data Structure:" << endl;
                cout << "   1. Binary Search Tree" << endl;
                cout << "   2. Linked List" << endl;
                cout << "Enter choice: ";
                cin >> structChoice;
            }
            choice = 0;
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
