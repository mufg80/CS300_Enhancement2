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
#include "BST.cpp"
#include <iomanip>
#include <limits>
#include <algorithm>

// Main menu, access point of program. 
int main(int argc, char* argv[]) {

    // Greeting
    std::cout << "         Welcome to ABCU Course App         " << std::endl;
    
    // Variable declarations
    std::string filePath;
    BST tree;
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
                isRead = readCourseFile("CourseList.txt", tree);

                
            break;
            case 2:
            {
                // Print list of course names.
               
               
            
            }
            break;
            case 3:
            {
                // Get specified course from user.
                std::string userInput = getCourseRequest();

                // Use name to print specific course and its prerequisites.

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
bool readCourseFile(std::string filepath, BST& tree){

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
        tree.Insert(*c);
        
        
    }
    }
    catch(std::ifstream::failure e){
        // Report errors to user.
        std::cerr << "            Error opening/reading file." << std::endl;
        return false;
    }

    tree.PrintOrdered();

    // Examine vectors for errors.
    // if(examineReadFile(tree)){
    //     // Vector of vectors is good, let user know file is loadedd into memory.
    //     std::cout << std::endl;
    //     std::cout << "            File loaded successfully." << std::endl;
    //     std::cout << std::endl;
    //     return true;
       
    // }else{
    //     // Function reported error with file format, let user know file failed to load.
    //     std::cout << std::endl;
    //     std::cout << "            File not loaded." << std::endl;
    //     std::cout << std::endl;
    //    return false;
    // }
    return true;
}

// Function to examine the vector of vectors validating correctness. Validation checked:
// 1: Must have at least one line.
// 2: First two columns must have data in each row.
// 3: All prereqs must exist in column 0 as course ID in another row.
// bool examineReadFile(BST& dataStructure){

//     // Vector of strings created to hold all course ids in column zero of dataStructure.
//     std::vector<std::string> courseNames;

//     // Make sure we don't have an empty vector.
//     if(dataStructure.size() == 0){
//         std::cout << std::endl;
//         std::cout << "            Error: No lines were read from file." << std::endl;
//         std::cout << std::endl;
//         return false;
//     }

//     // Start checks.
//     for(int i = 0; i < dataStructure.size(); i++){
//         // Check if line had at least two items.
//         if(dataStructure.at(i).size() < 2){
//             std::cout << std::endl;
//             std::cout << "            Error: Line missing some fields, less than 2 existing." << std::endl;
//             std::cout << std::endl;
//             return false;
//         }
//         // Be sure that first two columns both have information.
//         if((dataStructure.at(i).at(0).empty() || dataStructure.at(i).at(1).empty())){
//             // Course information is invalid should have two fields minimum.
//             std::cout << std::endl;
//             std::cout << "            Error: A line from the file did not have both a name and Id." << std::endl;
//             std::cout << std::endl;
//             return false;
//         }else{
//             // If a line has id and name, load id into a list of string for prereqs later.
//             if(!dataStructure.at(i).at(0).empty()){
//                 // Copy index 0 of each row to compare prereqs against.
//                 courseNames.push_back(dataStructure.at(i).at(0));
//             }
//         }
//     }


//     // Vector of vectors looks good, double check prereqs.
//     for(std::vector<std::string> list : dataStructure){
//         // Skip lists with no prereqs.
//         if(list.size() > 2){

//             // Start at index 2, after id and name, check each prereq.
//             for(int i = 2; i < list.size(); i++){
//                 // Set found bit to false.
//                 bool found = false;
//                 // Check against all ids in list.
//                 for(std::string name : courseNames){
//                     if(name.compare(list.at(i)) == 0){
//                         // Prereq found, mark found.
//                         found = true;
//                     }
//                 }
//                 // Prereq not found in ids, let user know and return false.
//                 if(!found){
//                     std::cout << std::endl;
//                     std::cout << "            Error: Prereq " << list.at(i) << " not found as course name in file." << std::endl;
//                     std::cout << std::endl;
//                     return false;
//                 }
//             }
//         }

//     }
//     return true;
// }