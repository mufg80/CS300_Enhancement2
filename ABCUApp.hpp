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
std::string getFilePath();

// Function that gets courses id from user.
std::string getCourseRequest();

// Menu output.
void outputMenuItems();

// Reads filepath and creates a vector of vectors.
bool readCourseFile(std::string filepath, BinarySearchTree* dataStructure);

// Checks data structure for validation rules.
//bool examineReadFile(BST& BuildDataStructure);

#endif /*!_ABCUApp_HPP_*/