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
#include <memory>
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
            std::unique_ptr<Node>left;
            std::unique_ptr<Node> right;
            Course current;

        public:
            Node(Course course);
            Course* ReturnCourse();
            Node* GetLeft();
            Node* GetRight();
            std::unique_ptr<Node> MoveLeft();
            std::unique_ptr<Node> MoveRight();
            void SetLeft(std::unique_ptr<Node> node);
            void SetRight(std::unique_ptr<Node> node);
            void ResetLeft();
            void ResetRight();
    };


    // Binary Search Tree Class: This class is used to sort courses by prereqs.

    class BinarySearchTree{

        private:
            int size;
            std::unique_ptr<Node> root;
            void addNode(Node* node, Course course);
            int compareNoCase(std::string first, std::string second);
            void inOrder(Node* node);
            void printCourse(Course course);
            void findCourse(Node* node, std::string id, Course& empty);
            void getListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node* node);
            void checkPrereqsRecursively(Node* node, bool* b, std::vector<std::tuple<std::string, std::string>> *list);
            bool validateNameDesciption(Course course);
            bool checkPrereqsOneCourse(BST::Course &course, std::vector<std::tuple<std::string, std::string>> &names);
            bool isCourseIdUnique(Course course);
            void findCoursesInvalidRecursively(Node* node, std::vector<Course>* courseIDs, Course course);
            void recursiveClear(Node* node);
        public:
            BinarySearchTree();
            int GetSize();
            bool Insert(Course course);
            void PrintOrdered();
            void FindCourse(std::string id, Course& empty);
            std::vector<std::string> FindCoursesInvalidOnDelete(std::string coursetodelete);
            bool ValidateCourses();
            void PrintOneCourse(std::string id);
            bool RemoveCoursewithId(std::string id);
            bool ValidateSingleCourse(Course course);
            void Clear();
           
    };

}
#endif /* _BST_HPP_ */