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
namespace BST{
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
            Node(Course course);
            Course* ReturnCourse();
            Node* GetLeft();
            Node* GetRight();
            void SetLeft(Node* node);
            void SetRight(Node* node);

    };


    // Binary Search Tree Class: This class is used to sort courses by prereqs.

    class BinarySearchTree{

        private:
            int size;
            Node* root;
            void addNode(Node* node, Course course);
            int compareNoCase(std::string first, std::string second);
            void inOrder(Node* node);
            void printCourse(Course course);
            void findCourse(Node* node, std::string id, Course& empty);
            void getListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node* node);
            void checkPrereqs(Node* node, bool* b, std::vector<std::tuple<std::string, std::string>> *list);
        public:
            BinarySearchTree();
            void Insert(Course course);
            void PrintOrdered();
            void FindCourse(std::string id, Course& empty);
            bool ValidateCourses();
            void PrintOneCourse(std::string id);
    };

}
#endif /*_BST_HPP_*/