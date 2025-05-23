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
    
    // Variable declarations
    std::string filePath;
    BinarySearchTree tree;
    int input;
    bool isRead = false;

    // Endless loop to cycle program until terminated.
    while(true){
        // Show menu to user and get user input.
        outputMenuItems();
        std::cin >> input;

        // Check input to make sure user entered integer. If not throw it out 
        // and assign as default a 5, which will trigger the default case in switch statement.
        if(!std::cin.good()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = 5;
        }

        // Start switch case.
        switch(input){
            case 1:

                // Clear structures so that if case 1 is ran multiple times, it will not grow.
                // Menu case to load file, as for file path.
                //filePath = getFilePath();

                // Load file request, try to read file at users request.
                isRead = readCourseFile("CourseList.txt", &tree);
                if(isRead){
                    std::cout << "Tree populated with courses." << std::endl;
                }else{
                    std::cout << "Tree failed to populate with courses." << std::endl;
                }

                
            break;
            case 2:
            {
                // Print list of course names.
               
               tree.PrintOrdered();
            
            }
            break;
            case 3:
            {
                // Get specified course from user.
                std::string userInput = getCourseRequest();

                // Use name to print specific course and its prerequisites.
                tree.PrintOneCourse(userInput);

            }
            break;
            case 4:

                // Menu Item for terminating program, say good bye and IF condition below will terminate loop.
                std::cout << "            Good bye!" << std::endl;
                break;
            break;
            default:
                // Default case, User specified non existing menu item. Let them know.
                std::cout << "            This is not an appropriate entry. Please try again." << std::endl;
        }
        if(input == 4){
            break;
        }

    }
    return 0;
}

// A function to get file name from user.
std::string getFilePath(){
    
    std::cout << "            Enter the file name for the courses list." << std::endl;
    std::string input;
    std::cin >> input;
    std::cout << std::endl;
    return input;
}

// Function to request the name of the course the user is interested in.
std::string getCourseRequest(){
    std::cout << "            Which course do you want to know about?  " << std::endl;
    std::string input;
    std::cin >> input;
    return input;
}


// Function for outputting menu to user.
void outputMenuItems(){
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "                    Menu Options                 " << std::endl;
    std::cout << "               1) Load Data Structure            " << std::endl;
    std::cout << "               2) Print Course List              " << std::endl;
    std::cout << "               3) Print Course                   " << std::endl;
    std::cout << "               4) Exit                           " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
}

// Read courses function takes the path to file and a vector of vectors. This function reads course list from
// hard drive and builds a vector of vectors. Calls helper function for checks on data.
bool readCourseFile(std::string filepath, BinarySearchTree* tree){

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
        tree->Insert(*c);
        
        
    }
    }
    catch(std::ifstream::failure e){
        // Report errors to user.
        std::cerr << "            Error opening/reading file." << std::endl;
        return false;
    }
    
    bool b = tree->ValidateCourses();
    
    return true;
}
