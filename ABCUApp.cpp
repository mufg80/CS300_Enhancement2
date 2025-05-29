//============================================================================
// Name        : ABCUApp.cpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Description : Implementation file for the ABCU Course App, providing functionality
//               to manage a Binary Search Tree (BST) of courses, including loading,
//               printing and adding courses, as well as handling user input.
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
int main(int argc, char *argv[])
{
    std::string filepath = "";
    // Display welcome message to the user.
    std::cout << "         Welcome to ABCU Course App         " << std::endl;

    BinarySearchTree tree;
    int input;

    // Main program loop, runs until the user chooses to exit.
    while (true)
    {

        // Display menu options and get user input.
        OutputMenuItems();
        GetUserInt(input);

        // Handle user input with a switch statement.
        switch (input)
        {
        case 1:
            BuildStructureFromFile(filepath, tree); // Load course data into the BST from file.
            break;
        case 2:
            PrintCoursesInOrder(tree); // Print all courses in order.
            break;
        case 3:
            PrintOneCourse(tree); // Print details of a specific course.
            break;
        case 4:
            // Exit option: Display goodbye message and exit loop.
            std::cout << "            Good bye!" << std::endl;
            break;
        default:
            // Handle invalid menu selections.
            std::cout << "            This is not an appropriate entry. Please try again." << std::endl;
            break;
        }
        if (input == 4)
        {
            break; // Terminate loop when user selects exit.
        }
    }
    return 0; // Successful program termination.
}

// Loads course data from a file into the Binary Search Tree (Case 1).
// Parameters:
//   tree - Reference to the BinarySearchTree to populate with course data.
void BuildStructureFromFile(std::string &filePath, BST::BinarySearchTree &tree)
{

    if (filePath.size() < 1 || !std::filesystem::exists(filePath))
    {
        std::string message = "            Enter the file name for the courses list (no extension).";
        std::string userInput = "";

        GetUserString(message, &userInput);

        if (userInput.size() < 1)
        {
            std::cout << "Improper filename. Please try again." << std::endl;
            return;
        }
        userInput += ".txt";
        filePath = userInput;
    }

    // Attempt to read course data from the default file "CourseList.txt".
    bool isRead = ReadCourseFile(filePath, &tree);

    if (isRead)
    {
        std::cout << "Tree populated with courses." << std::endl;
    }
    else
    {
        std::cout << "Tree failed to populate with courses." << std::endl;
        tree.Clear(); // Clear tree on failure to maintain consistency.
    }
}

// Prints all courses in the Binary Search Tree in order (Case 2).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void PrintCoursesInOrder(BST::BinarySearchTree &tree)
{
    if (tree.GetSize() == 0)
    {
        std::cout << "No courses found." << std::endl;
    }
    else
    {
        tree.PrintOrdered(); // Print courses in sorted order.
        std::cout << "" << std::endl;
        std::cout << "Courses: " << tree.GetSize() << std::endl;
        std::cout << "" << std::endl;
    }
}

// Prints details of a specific course based on user input (Case 3).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void PrintOneCourse(BST::BinarySearchTree &tree)
{
    std::string message = "Which course (by ID) would you like to know about?";
    std::string userinput;

    // Prompt user for course ID and print course details.
    GetUserString(message, &userinput);
    tree.PrintSingleCourse(userinput);
}

// Prompts the user for a string input and stores it in the provided pointer.
// Parameters:
//   message - The prompt message displayed to the user.
//   input   - Pointer to a string where the user's input will be stored.
void GetUserString(std::string message, std::string *input)
{
    std::cout << "            " << message << std::endl;
    std::getline(std::cin, *input);
    BufferCheck(); // Clear input buffer to prevent errors
}

// Prompts the user for an integer input and stores it in the provided reference.
// Parameters:
//   input - Reference to an integer where the user's input will be stored.
void GetUserInt(int &input)
{
    std::string strInput;
    std::getline(std::cin, strInput);

    // Convert string to integer, handle invalid input.
    try
    {
        input = std::stoi(strInput);
    }
    catch (const std::exception &e)
    {
        input = 10; // Default value for invalid input.
    }
    BufferCheck(); // Clear input buffer to prevent errors.
}

// Clears the input buffer to prevent errors from residual data.
void BufferCheck()
{
    if (!std::cin.good())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Displays the menu options for the ABCU Course App.
void OutputMenuItems()
{
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "                    Menu Options                 " << std::endl;
    std::cout << "               1) Load Next 100 Courses to Memory" << std::endl;
    std::cout << "               2) Print Course List              " << std::endl;
    std::cout << "               3) Print Course                   " << std::endl;
    std::cout << "               4) Exit                           " << std::endl;
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
}

// Reads course data from a file and populates the Binary Search Tree.
// Parameters:
//   filepath      - The path to the file containing course data.
//   tree          - Pointer to the BinarySearchTree to store the course data.
// Returns: True if the file was successfully read and the tree was populated, false otherwise.
bool ReadCourseFile(std::string filePath, BinarySearchTree *tree)
{

    if (!std::filesystem::exists(filePath))
    {
        std::cerr << "Error, File doesn't exist." << std::endl;
        return false;
    }

    try
    {
        std::ifstream readfile(filePath);
        std::string line;

        // Check for file opening failure.
        if (readfile.fail())
        {
            std::cout << std::endl;
            std::cout << "            Failure to open a file of this name, please" << std::endl;
            std::cout << "            make sure the file exists in programs directory." << std::endl;
            std::cout << std::endl;
            return false;
        }

        // Get next 100 courses

        int starting = tree->GetSize();
        int index = 0;
        int ending = starting + 100;

        // Read file line by line.
        while (getline(readfile, line))
        {
            std::istringstream linestream(line);
            std::vector<std::string> listOfPieces;
            std::string piece;

            // Parse line by commas.
            while (getline(linestream, piece, ','))
            {
                listOfPieces.push_back(piece);
            }

            if (index >= starting && index < ending)
            {
                // Create a new Course object from parsed data.
                Course *c = new Course();
                if (listOfPieces.size() > 1)
                {
                    c->courseId = listOfPieces.at(0);
                    c->courseName = listOfPieces.at(1);
                }
                for (size_t i = 2; i < listOfPieces.size(); i++)
                {
                    c->prereqs.push_back(listOfPieces.at(i));
                }

                // Insert course into the tree.
                bool success = tree->Insert(*c);
                if (!success)
                {
                    std::cout << "Not inserted: " << c->courseId << std::endl;
                }
                delete c; // Free allocated memory.
            }

            index++;
        }
    }
    catch (std::ifstream::failure &e)
    {
        // Handle file reading errors.
        std::cerr << "            Error opening/reading file." << std::endl;
        return false;
    }

    // Validate all courses in the tree.
    bool valid = tree->ValidateCourses();
    return valid;
}
