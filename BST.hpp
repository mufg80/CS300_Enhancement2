//============================================================================
// Name        : Reader.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Header file containing file reading declarations for ABCU app
//============================================================================
#ifndef _BST_HPP_
#define _BST_HPP_
#include <string>
#include <vector>
// Structure for courses
struct Course{
    std::string courseId;
    std::string courseName;
    std::vector<std::string> prereqs;
};

// Node class to hold courses
class Node{
    private:
        Node* left;
        Node* right;
        Course current;
    public:
    Node::Node(Course course);
    Course* ReturnCourse();
    Node* GetLeft();
    Node* GetRight();
    void SetLeft(Node* node);
    void SetRight(Node* node);

};


// Binary Search Tree Class: This class is used to sort courses by prereqs.

class BST{
    private:
    int size;
    Node* root;
    void BST::addNode(Node* node, Course course);
    int compareNoCase(std::string first, std::string second);
    void BST::InOrder(Node* node);
    void BST::PrintCourse(Course course);
    public:
    BST::BST();
    void BST::Insert(Course course);
    void BST::PrintOrdered();
    
    Course* BST::GetCourse(std::string courseNumber);
};


#endif /*!_BST_HPP_*/