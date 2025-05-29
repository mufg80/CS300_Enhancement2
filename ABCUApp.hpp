//============================================================================
// Name        : ABCUApp.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Description : Header file for the ABCU application, containing declarations
//               for functions to manage a Binary Search Tree (BST) of courses,
//               handle user input, and process course data from a file.
//============================================================================
#pragma once

#include <string>
#include <vector>
#include "BST.hpp"

// Prompts the user for an integer input and stores it in the provided reference.
// Parameters:
//   input - Reference to an integer where the user's input will be stored.
void GetUserInt(int &userInput);

// Builds a Binary Search Tree by reading course data from a file (Case 1).
// Parameters:
//   tree - Reference to the BinarySearchTree to populate with course data.
void BuildStructureFromFile(std::string &filepath, BST::BinarySearchTree &courseTree);

// Prints the courses in the Binary Search Tree in ordered traversal (Case 2).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void PrintCoursesInOrder(BST::BinarySearchTree &courseTree);

// Prints details of a specific course from the Binary Search Tree (Case 3).
// Parameters:
//   tree - Reference to the BinarySearchTree containing course data.
void PrintOneCourse(BST::BinarySearchTree &courseTree);

// Prompts the user for a string input and stores it in the provided pointer.
// Parameters:
//   message - The prompt message displayed to the user.
//   input   - Pointer to a string where the user's input will be stored.
void GetUserString(std::string message, std::string *userInput);

// Clears the input buffer to prevent input errors from residual data.
void BufferCheck();

// Displays the menu options for the ABCU application.
void OutputMenuItems();

// Reads course data from a file and populates the Binary Search Tree.
// Parameters:
//   filepath      - The path to the file containing course data.
//   dataStructure - Pointer to the BinarySearchTree to store the course data.
// Returns: True if the file was successfully read and the tree was populated, false otherwise.
bool ReadCourseFile(std::string filepath, BST::BinarySearchTree *courseTree);
