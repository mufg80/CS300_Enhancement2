//============================================================================
// Name        : ABCUApp.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Header file containing declarations for ABCU app
//============================================================================
#ifndef _ABCUApp_HPP_
#define _ABCUApp_HPP_
#include <string>
#include <vector>
#include "BST.hpp"
using namespace BST;

std::string GetFilePath();

void GetUserInt(int &input);

void BuildTreeCaseOne(BST::BinarySearchTree &tree);

void PrintOrderedCaseTwo(BST::BinarySearchTree &tree);

void PrintCourseCaseThree(BST::BinarySearchTree &tree);

void DeleteCaseFour(BST::BinarySearchTree &tree);

void InsertCourseCaseFive(BST::BinarySearchTree &tree);

void GetUserString(std::string message, std::string *input);

void GetUserList(std::string message, std::vector<std::string> *inputList);

void BufferCheck();

void OutputMenuItems();

bool ReadCourseFile(std::string filepath, BinarySearchTree* dataStructure);


#endif /*!_ABCUApp_HPP_*/