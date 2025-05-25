//============================================================================
// Name        : ABCUApp.cpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Course app for ABCU: Additional enhancements created for CS499 Capstone
//============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include <string> 
#include <vector>
#include "ABCUApp.hpp"
#include "BST.hpp"
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace BST;
// Main menu, access point of program. 
int main(int argc, char* argv[]) {

    // Greeting
    std::cout << "         Welcome to ABCU Course App         " << std::endl;
        
    BinarySearchTree tree;
    int input;

    // Endless loop to cycle program until terminated.
    while(true){
        // Show menu to user and get user input.
        OutputMenuItems();
        GetUserInt(input);

        // Start switch case.
        switch(input){
            case 1:
                BuildTreeCaseOne(tree);
                break;
            case 2:
                PrintOrderedCaseTwo(tree);
            break;
            case 3:
                PrintCourseCaseThree(tree);
            break;
            case 4:
                DeleteCaseFour(tree);
            break;
            case 5:
                InsertCourseCaseFive(tree);
            break;
            case 6:
                // Menu Item for terminating program, say good bye and IF condition below will terminate loop.
                std::cout << "            Good bye!" << std::endl;                
            break;
            default:
                // Default case, User specified non existing menu item. Let them know.
                std::cout << "            This is not an appropriate entry. Please try again." << std::endl;
            break;
        }
        if(input == 6){
            break;
        }
    }
    return 0;
}


void BuildTreeCaseOne(BST::BinarySearchTree &tree)
{
    // Clear structures so that if case 1 is ran multiple times, it will not grow.
    // Menu case to load file, as for file path.
    // filePath = getFilePath();

    // Load file request, try to read file at users request.
    bool isRead = ReadCourseFile("CourseList.txt", &tree);
    if (isRead)
    {
        std::cout << "Tree populated with courses." << std::endl;
    }
    else
    {
        std::cout << "Tree failed to populate with courses." << std::endl;
        tree.Clear();
    }
}


void PrintOrderedCaseTwo(BST::BinarySearchTree &tree)
{
    // Print list of course names.

    if(tree.GetSize() == 0){
        std::cout << "No courses found." << std::endl;
    }else{
        tree.PrintOrdered();
    }
    
}

void PrintCourseCaseThree(BST::BinarySearchTree &tree)
{
    std::string message = "Which course (by ID) would you like to know about?";
    std::string userinput = "";
    // Get specified course from user.
    GetUserString(message, &userinput);

    // Use name to print specific course and its prerequisites.
    tree.PrintOneCourse(userinput);
}

void DeleteCaseFour(BST::BinarySearchTree &tree)
{
    std::string message = "Which course (by ID) would you like to delete?";
    std::string userinput = "";
    // Get specified course from user.
    GetUserString(message, &userinput);
    std::vector<std::string> coursesAffected;
    coursesAffected = tree.FindCoursesInvalidOnDelete(userinput);

    if(coursesAffected.size() > 0){

        std::cout << "Courses to be deleted: ";
        for (size_t i = 0; i < coursesAffected.size(); i++)
        {
            if(i == 0){
                std::cout << " " << coursesAffected.at(i) << std::endl;
            }else{
                std::cout << "                        " << coursesAffected.at(i) << std::endl;
            }
        
        }
        message = "Confirm deletion? Type 1 to delete or 2 to cancel";
        userinput = "";
        GetUserString(message, &userinput);
        if(userinput.compare("1") == 0){
            for (size_t i = 0; i < coursesAffected.size(); i++)
            {
                bool b = tree.RemoveCoursewithId(coursesAffected.at(i));
                if(b){
                        std::cout << coursesAffected.at(i) << " Deleted." << std::endl;
                }else{
                        std::cout << coursesAffected.at(i) << " not Deleted." << std::endl;
                }
            }
        }

    }else{
        std::cout << "No courses found." << std::endl;
    }

    
   
}

void InsertCourseCaseFive(BST::BinarySearchTree &tree)
{
    // Get specified course from user.
    std::string message = "What is course ID?";
    std::string userinput = "";
    Course newcourse;
    GetUserString(message, &userinput);
    newcourse.courseId = userinput;

    message = "What is course name?";
    userinput = "";
    GetUserString(message, &userinput);
    newcourse.courseName = userinput;

    
    message = "What is course prereqs? They must be ID's separated by commas (,) and must exist.";
    std::vector<std::string> inputList;

    GetUserList(message, &inputList);
    newcourse.prereqs = inputList;
    // Menu Item for adding course

    bool validated = tree.ValidateSingleCourse(newcourse);
    if(validated){
        bool b = tree.Insert(newcourse);
        if(b){
            std::cout << "Inserted." << std::endl;
        }else{
            std::cout << "Unable to insert. Please try again." << std::endl;
        }
    }else{
        std::cout << "Unable to insert. Please try again." << std::endl;
    }
    
}

// Function to request the name of the course the user is interested in.
void GetUserString(std::string message, std::string *input){
    std::cout << "            " << message << std::endl;
    std::getline(std::cin, *input);
    BufferCheck();
}

// Function to request the name of the course the user is interested in.
void GetUserList(std::string message, std::vector<std::string> *inputList){
    std::cout << "            " << message << std::endl;
    std::string req;
    std::getline(std::cin, req);

    std::istringstream iss(req);
    std::string prereq;
    while (std::getline(iss, prereq, ',')) {
        inputList->push_back(prereq);
    }
    BufferCheck();
}

void GetUserInt(int &input)
{
    std::string strInput;
    std::getline(std::cin, strInput);

    try
    {
        input = std::stoi(strInput);
    }
    catch(const std::exception& e)
    {
        input = 10;
    }
    BufferCheck();
}

void BufferCheck(){
    if (!std::cin.good())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// A function to get file name from user.
std::string GetFilePath(){
    
    std::cout << "            Enter the file name for the courses list." << std::endl;
    std::string input;
    std::cin >> input;
    std::cout << std::endl;
    return input;
}

// Function for outputting menu to user.
void OutputMenuItems(){
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "                    Menu Options                 " << std::endl;
    std::cout << "               1) Load Data Structure            " << std::endl;
    std::cout << "               2) Print Course List              " << std::endl;
    std::cout << "               3) Print Course                   " << std::endl;
    std::cout << "               4) Delete Course                   " << std::endl;
    std::cout << "               5) Add Course                   " << std::endl;
    std::cout << "               6) Exit                           " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
}

// Read courses function takes the path to file and a vector of vectors. This function reads course list from
// hard drive and builds a vector of vectors. Calls helper function for checks on data.
bool ReadCourseFile(std::string filepath, BinarySearchTree* tree){

    // Try catch to catch failures on stream and let user know of issue.
    try{
    std::ifstream readfile(filepath);
    std::string line;

    // Check fail and report failure and return.
    if(readfile.fail()){
        std::cout << std::endl;
        std::cout << "            Failure to open a file of this name, please" << std::endl;
        std::cout << "            make sure the file exists in programs directory." << std::endl;
        std::cout << std::endl;
        return false;
    }

    // Nested while loops, first reads lines delimited by new lines, second is delimited on commas.
    // Getline called on stream to take a line until newline character.
    while(getline(readfile, line)){

        // Take string taken from stream and use stringstream so that getline can be used again on commas.
        std::istringstream linestream(line);

        // Create vector of strings to get pieces of line delimited by commas.
        std::vector<std::string> listOfPieces;
        std::string piece;

        // Getline uses third argument of comma to get pieces of line then adds to vector.
        while(getline(linestream, piece,',')){
            listOfPieces.push_back(piece);
        }
        // Reached end of particular line, add vector of string to vector.
        Course* c = new Course();
        if(listOfPieces.size() > 1){
            c->courseId = listOfPieces.at(0);
            c->courseName = listOfPieces.at(1);
        }
        for (size_t i = 2; i < listOfPieces.size(); i++)
        {
            c->prereqs.push_back(listOfPieces.at(i));
        }

        
        bool b = tree->Insert(*c);
        if(!b){
            std::cout << "Not inserted: " << c->courseId << std::endl;
        }
        
        
    }
    }
    catch(std::ifstream::failure e){
        // Report errors to user.
        std::cerr << "            Error opening/reading file." << std::endl;
        return false;
    }
    
    bool b = tree->ValidateCourses();
    
    return b;
}
