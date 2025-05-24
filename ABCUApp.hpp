//============================================================================
// Name        : Reader.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Header file containing file reading declarations for ABCU app
//============================================================================
#ifndef _ABCUApp_HPP_
#define _ABCUApp_HPP_
#include <string>
#include <vector>
#include "BST.hpp"
using namespace BST;

// Function Declarations

// Gets file path string from user.
std::string GetFilePath();

void GetUserInt(int &input);

void DeleteCaseFour(BST::BinarySearchTree &tree);

void BuildTreeCaseOne(BST::BinarySearchTree &tree);

void PrintOrderedCaseTwo(BST::BinarySearchTree &tree);

void PrintCourseCaseThree(BST::BinarySearchTree &tree);

void InsertCourseCaseFive(BST::BinarySearchTree &tree);

// Function that gets courses id from user.
void GetUserString(std::string message, std::string *input);

void GetUserList(std::string message, std::vector<std::string> *inputList);

void BufferCheck();

// Menu output.
void OutputMenuItems();

// Reads filepath and creates a vector of vectors.
bool ReadCourseFile(std::string filepath, BinarySearchTree* dataStructure);

// Checks data structure for validation rules.
//bool examineReadFile(BST& BuildDataStructure);

#endif /*!_ABCUApp_HPP_*/