//============================================================================
// Name        : Reader.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Header file containing file reading declarations for ABCU app
//============================================================================
#ifndef _READER_HPP_
#define _READER_HPP_
#include <string>
#include <vector>


// Function Declarations

// Gets file path string from user.
std::string getFilePath();

// Function that gets courses id from user.
std::string getCourseRequest();

// Menu output.
void outputMenuItems();

// Reads filepath and creates a vector of vectors.
bool readCourseFile(std::string filepath, BST& dataStructure);

// Checks data structure for validation rules.
//bool examineReadFile(BST& BuildDataStructure);

#endif /*!_READER_HPP_*/