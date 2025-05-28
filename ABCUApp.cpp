//============================================================================
// Name        : ABCUApp.cpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : None
// Description : Implementation file for the ABCU Course App, providing functionality
//               to manage a Binary Search Tree (BST) of courses, including loading,
//               printing, deleting, and adding courses, as well as handling user input.
//               Developed as part of the CS499 Capstone project.
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
#include <filesystem>

// Using BST namespace for BinarySearchTree and Course classes.
using namespace BST; 

// Main entry point of the ABCU Course App.
// Parameters:
//   argc - Number of command-line arguments.
//   argv - Array of command-line argument strings.
// Returns: 0 on successful program termination.
int main(int argc, char* argv[]) {

    // Display welcome message to the user.
    std::cout << "         Welcome to ABCU Course App         " << std::endl;
        

    BinarySearchTree tree; 
    int input; 

    // Main program loop, runs until the user chooses to exit.
    while(true) {

        // Display menu options and get user input.
        OutputMenuItems();
        GetUserInt(input);

        // Handle user input with a switch statement.
        switch(input) {
            case 1:
                BuildStructureFromFile(tree); // Load course data into the BST from file.
                break;
            case 2:
                PrintCoursesInOrder(tree); // Print all courses in order.
                break;
            case 3:
                PrintOneCourse(tree); // Print details of a specific course.
                break;
            case 4:
                DeleteCourseFromTree(tree); // Delete a specified course.
                break;
            case 5:
                InsertCaseToTree(tree); // Add a new course to the BST.
                break;
            case 6:
                Rebalance(tree);
                break;
            case 7:
                PrintThreeTiers(tree);
                break;
            case 8:
                // Exit option: Display goodbye message and exit loop.
                std::cout << "            Good bye!" << std::endl;                
                break;
            default:
                // Handle invalid menu selections.
                std::cout << "            This is not an appropriate entry. Please try again." << std::endl;
                break;
        }
        if(input == 8) {
            break; // Terminate loop when user selects exit.
        }
    }
    return 0; // Successful program termination.
}

// Loads course data from a file into the Binary Search Tree (Case 1).
// Parameters:
//   tree - Reference to the BinarySearchTree to populate with course data.
void BuildStructureFromFile(BST::BinarySearchTree &tree) {

    // Clear the tree to prevent duplicate entries if case 1 is ran multiple times.
    tree.Clear();

    // Attempt to read course data from the default file "CourseList.txt".
    bool isRead = ReadCourseFile("CourseList.txt", &tree);

    if (isRead) {
        std::cout << "Tree populated with courses." << std::endl;
    } else {
        std::cout << "Tree failed to populate with courses." << std::endl;
        tree.Clear(); // Clear tree on failure to maintain consistency.
    }
}

// Prints all courses in the Binary Search Tree in order (Case 2).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void PrintCoursesInOrder(BST::BinarySearchTree &tree) {
    if(tree.GetSize() == 0) {
        std::cout << "No courses found." << std::endl;
    } else {
        tree.PrintOrdered(); // Print courses in sorted order.
    }
}

// Prints details of a specific course based on user input (Case 3).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void PrintOneCourse(BST::BinarySearchTree &tree) {
    std::string message = "Which course (by ID) would you like to know about?";
    std::string userinput;

    // Prompt user for course ID and print course details.
    GetUserString(message, &userinput);
    tree.PrintOneCourse(userinput);
}

// Deletes a specified course from the Binary Search Tree (Case 4).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void DeleteCourseFromTree(BST::BinarySearchTree &tree) {
    if(tree.GetSize() == 0){
        std::cout << "        Tree Empty." << std::endl;
        return;
    }
    std::string message = "Which course (by ID) would you like to delete?";
    std::string userinput;

    // Prompt user for course ID to delete
    GetUserString(message, &userinput);
    std::vector<std::string> coursesAffected = tree.FindCoursesInvalidOnDelete(userinput);

    // Remove duplicate strings from vec.
    std::sort(coursesAffected.begin(), coursesAffected.end());
    coursesAffected.erase(std::unique(coursesAffected.begin(), coursesAffected.end()), coursesAffected.end());

    if(coursesAffected.size() > 0) {
        // Display courses that will be affected by the deletion.
        std::cout << "Courses to be deleted: ";
        for (size_t i = 0; i < coursesAffected.size(); i++) {
            if(i == 0) {
                std::cout << " " << coursesAffected.at(i) << std::endl;
            } else {
                std::cout << "                        " << coursesAffected.at(i) << std::endl;
            }
        }

        // Confirm deletion with user.
        message = "Confirm deletion? Type 1 to delete or 2 to cancel";
        userinput.clear();
        GetUserString(message, &userinput);
        if(userinput == "1") {
            // Delete each affected course
            for (size_t i = 0; i < coursesAffected.size(); i++) {
                bool success = tree.RemoveCourseWithId(coursesAffected.at(i));
                if(success) {
                    std::cout << coursesAffected.at(i) << " Deleted." << std::endl;
                } else {
                    std::cout << coursesAffected.at(i) << " not Deleted." << std::endl;
                }
            }
        }
    } else {
        std::cout << "No courses found." << std::endl;
    }
}

// Inserts a new course into the Binary Search Tree based on user input (Case 5).
// Parameters:
//   tree - Reference to the BinarySearchTree to insert the course into.
void InsertCaseToTree(BST::BinarySearchTree &tree) {
    std::string message;
    std::string userinput;
    Course newcourse;

    // Prompt for course ID.
    message = "What is course ID?";
    GetUserString(message, &userinput);
    newcourse.courseId = userinput;

    // Prompt for course name.
    message = "What is course name?";
    GetUserString(message, &userinput);
    newcourse.courseName = userinput;

    // Prompt for prerequisites.
    message = "What is course prereqs? They must be ID's separated by commas (,) and must exist.";
    std::vector<std::string> inputList;
    GetUserList(message, &inputList);
    newcourse.prereqs = inputList;

    // Validate and insert the new course.
    bool validated = tree.ValidateSingleCourse(newcourse);
    if(validated) {
        bool success = tree.Insert(newcourse);
        if(success) {
            std::cout << "Inserted." << std::endl;
        } else {
            std::cout << "Unable to insert. Please try again." << std::endl;
        }
    } else {
        std::cout << "Unable to insert. Please try again." << std::endl;
    }
}

// Prompts the user for a string input and stores it in the provided pointer.
// Parameters:
//   message - The prompt message displayed to the user.
//   input   - Pointer to a string where the user's input will be stored.
void GetUserString(std::string message, std::string *input) {
    std::cout << "            " << message << std::endl;
    std::getline(std::cin, *input);
    BufferCheck(); // Clear input buffer to prevent errors
}

// Prompts the user for a comma-separated list of strings and stores them in a vector.
// Parameters:
//   message   - The prompt message displayed to the user.
//   inputList - Pointer to a vector of strings to store the user's input.
void GetUserList(std::string message, std::vector<std::string> *inputList) {
    std::cout << "            " << message << std::endl;
    std::string req;
    std::getline(std::cin, req);

    // Parse input string by commas.
    std::istringstream iss(req);
    std::string prereq;
    while (std::getline(iss, prereq, ',')) {
        inputList->push_back(prereq);
    }
    BufferCheck(); // Clear input buffer to prevent errors.
}

// Prompts the user for an integer input and stores it in the provided reference.
// Parameters:
//   input - Reference to an integer where the user's input will be stored.
void GetUserInt(int &input) {
    std::string strInput;
    std::getline(std::cin, strInput);

    // Convert string to integer, handle invalid input.
    try {
        input = std::stoi(strInput);
    } catch(const std::exception& e) {
        input = 10; // Default value for invalid input.
    }
    BufferCheck(); // Clear input buffer to prevent errors.
}

// Clears the input buffer to prevent errors from residual data.
void BufferCheck() {
    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Prompts the user for a file name and returns it.
// Returns: A string containing the user-provided file path.
std::string GetFilePath() {
    std::cout << "            Enter the file name for the courses list." << std::endl;
    std::string input;
    std::cin >> input;
    std::cout << std::endl;
    return input;
}

// Displays the menu options for the ABCU Course App.
void OutputMenuItems() {
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "                    Menu Options                 " << std::endl;
    std::cout << "               1) Load Data Structure            " << std::endl;
    std::cout << "               2) Print Course List              " << std::endl;
    std::cout << "               3) Print Course                   " << std::endl;
    std::cout << "               4) Delete Course                   " << std::endl;
    std::cout << "               5) Add Course                   " << std::endl;
    std::cout << "               6) Rebalance Tree                   " << std::endl;
    std::cout << "               7) Print Tiers                   " << std::endl;
    std::cout << "               8) Exit                           " << std::endl;
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
}

// Reads course data from a file and populates the Binary Search Tree.
// Parameters:
//   filepath      - The path to the file containing course data.
//   tree          - Pointer to the BinarySearchTree to store the course data.
// Returns: True if the file was successfully read and the tree was populated, false otherwise.
bool ReadCourseFile(std::string filepath, BinarySearchTree* tree) {

    if(!std::filesystem::exists(filepath)){
        std::cerr << "Error, File doesn't exist." << std::endl;
        return false;
    }


    try {
        std::ifstream readfile(filepath);
        std::string line;

        // Check for file opening failure.
        if(readfile.fail()) {
            std::cout << std::endl;
            std::cout << "            Failure to open a file of this name, please" << std::endl;
            std::cout << "            make sure the file exists in programs directory." << std::endl;
            std::cout << std::endl;
            return false;
        }

        // Read file line by line.
        while(getline(readfile, line)) {
            std::istringstream linestream(line);
            std::vector<std::string> listOfPieces;
            std::string piece;

            // Parse line by commas.
            while(getline(linestream, piece, ',')) {
                listOfPieces.push_back(piece);
            }

            // Create a new Course object from parsed data.
            Course* c = new Course();
            if(listOfPieces.size() > 1) {
                c->courseId = listOfPieces.at(0);
                c->courseName = listOfPieces.at(1);
            }
            for (size_t i = 2; i < listOfPieces.size(); i++) {
                c->prereqs.push_back(listOfPieces.at(i));
            }

            // Insert course into the tree.
            bool success = tree->Insert(*c);
            if(!success) {
                std::cout << "Not inserted: " << c->courseId << std::endl;
            }
            delete c; // Free allocated memory.
        }
    } catch(std::ifstream::failure& e) {
        // Handle file reading errors.
        std::cerr << "            Error opening/reading file." << std::endl;
        return false;
    }

    // Validate all courses in the tree.
    bool valid = tree->ValidateCourses();
    return valid;
}

// Calls function to rebalance BST if it is not empty.
// Parameters:
//   tree          - Pointer to the BinarySearchTree to store the course data.
// Returns: None
void Rebalance(BST::BinarySearchTree &courseTree)
{
    if(courseTree.GetSize() == 0){
        std::cout << "        Tree Empty." << std::endl;
        return;
    }
    courseTree.RebalanceTree();
}

// Calls function to print 3 levels of tree if tree is not empty.
// Parameters:
//   tree          - Pointer to the BinarySearchTree to store the course data.
// Returns: None
void PrintThreeTiers(BST::BinarySearchTree &courseTree)
{
    if(courseTree.GetSize() == 0){
        std::cout << "        Tree Empty." << std::endl;
        return;
    }
    std::cout << "" << std::endl;
    std::cout << "Tree Displayed to first 3 tiers: " << std::endl;
    std::cout << "" << std::endl;    
    courseTree.PrintTopThreeLevelsOfTree();
    std::cout << "" << std::endl;
}
