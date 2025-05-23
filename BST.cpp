

#include "BST.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace BST{

    // Node definition

    // Public method to return left child node.
    Node* Node::GetLeft(){
        return this->left;
    }

    // Public method to return right child node.
    Node* Node::GetRight(){   
        return this->right;
    }

    // Public method to set left child node.
    void Node::SetLeft(Node* node){
        this->left = node;
    }

    // Public method to set right child node.
    void Node::SetRight(Node* node){
        this->right = node;
    }

    // Public method to return pointer to nodes course structure.
    Course* Node::ReturnCourse(){
        Course* c = &this->current;
        return c;
    }

    // Constructor for node class to instaniate with a course.
    Node::Node(Course course){
        this->current = course;
        this->left = nullptr;
        this->right = nullptr;
    }

    // BST 
    BinarySearchTree::BinarySearchTree(){
        this->size = 0;
        this->root = nullptr;
    }

    

    // Function compares strings after converting them to lowercase, for case insensitive comparisons.
    int BinarySearchTree::compareNoCase(std::string first, std::string second){
        // Use transform function by passing iterators and function to use: ::tolower. Then return
        // the comparison.
        transform(first.begin(), first.end(),first.begin(), ::tolower);
        transform(second.begin(), second.end(),second.begin(), ::tolower);
        return first.compare(second);
    }


    void BinarySearchTree::Insert(Course course){
        if(this->root == nullptr){
            Node* node = new Node(course);
            this->root = node;
            this->size++;    
        }else{
            this->addNode(this->root, course);
        }
    }

    void BinarySearchTree::addNode(Node* node, Course course){
        Course* c = node->ReturnCourse();
        if(this->compareNoCase(c->courseId, course.courseId)){
            if(node->GetLeft() == nullptr){
                node->SetLeft(new Node(course));
                this->size++;
            }else{
                addNode(node->GetLeft(), course);
            }
        }else{
            if(node->GetRight() == nullptr){
                node->SetRight(new Node(course));
                this->size++;
            }else{
                addNode(node->GetRight(), course);
            }
        }
    }

    
    void BinarySearchTree::PrintOrdered(){
        if(this->root != nullptr){
            this->inOrder(this->root);
        }
    }


    void BinarySearchTree::inOrder(Node* node){
        if(node->GetLeft() != nullptr){
            this->inOrder(node->GetLeft());
        }
        
        this->printCourse(*node->ReturnCourse());

        if(node->GetRight() != nullptr){
            this->inOrder(node->GetRight());
        }
        
    }
    void BinarySearchTree::printCourse(Course course){
        std::cout << "------------------------------------------" << std::endl;
        std::cout << course.courseId << "    " << course.courseName << std::endl;
        std::cout << "Prereqs:   ";
        for (size_t i = 0; i < course.prereqs.size(); i++)
        {
            if(i != 0){
                std::cout << "           ";
            }
            
            std::cout << course.prereqs.at(i) << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
    }

     void BinarySearchTree::FindCourse(std::string id, Course& empty)
    {
        if(this->root != nullptr){
            findCourse(this->root, id, empty);
        }
    }

    
   
    void BinarySearchTree::findCourse(Node *node, std::string id, Course &empty)
    {
        if(compareNoCase(node->ReturnCourse()->courseId, id) == 0){
            empty = *node->ReturnCourse();
        }
        if(node->GetLeft() != nullptr){
            findCourse(node->GetLeft(), id, empty);
        }
        if(node->GetRight() != nullptr){
            findCourse(node->GetRight(), id, empty);
        }
    }


    // Function to examine the vector of vectors validating correctness. Validation checked:
    // 1: Must have at least one line.
    // 2: First two columns must have data in each row.
    // 3: All prereqs must exist in column 0 as course ID in another row.


    bool BinarySearchTree::ValidateCourses()
    {
        std::vector<std::tuple<std::string, std::string>> list;
        auto listptr = &list;
        Node* n = this->root;
        this->getListOfCourseNames(listptr, n);

        bool isGood = true;
        bool* isGoodptr = &isGood;
        for (size_t i = 0; i < list.size(); i++)
        {
            auto [x,y] = list.at(i);
            if(x.compare("") == 0){
                isGood = false;
            }
            if(y.compare("") == 0){
                isGood = false;
            }
        }
        this->checkPrereqs(this->root, isGoodptr, listptr);
        return isGood;
    }

    
    void BinarySearchTree::getListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node* node){
        if(node->GetLeft() != nullptr){
            this->getListOfCourseNames(list, node->GetLeft());
        }
        std::tuple<std::string, std::string> myTuple(node->ReturnCourse()->courseId, node->ReturnCourse()->courseName);
        list->push_back(myTuple);

        if(node->GetRight() != nullptr){
            this->getListOfCourseNames(list, node->GetRight());
        }
    }

    

    void BinarySearchTree::checkPrereqs(Node *node, bool *b, std::vector<std::tuple<std::string, std::string>> *list)
    {
        if(node->GetLeft() != nullptr){
            this->checkPrereqs(node->GetLeft(), b, list);
        }
        std::vector<std::string> prereqs = node->ReturnCourse()->prereqs;
        
        if(prereqs.size() != 0){
            for (size_t i = 0; i < prereqs.size(); i++)
            {
                bool isGood = false;
                for (size_t j = 0; j < list->size(); j++)
                {
                    auto [x,y] = list->at(j);
                    if(prereqs.at(i).compare(x) == 0){
                        isGood = true;
                    }
                    
                }
                if(!isGood){
                    *b = false;
                }
            }
            
        }
        if(node->GetRight() != nullptr){
            this->checkPrereqs(node->GetRight(), b, list);
        }
    }


    void BinarySearchTree::PrintOneCourse(std::string id)
    {
        Course course;
        this->FindCourse(id, course);
        if(course.courseId.compare("") != 0 && course.courseName.compare("") != 0){
            this->printCourse(course);
        }else{
            std::cout << "Course not found." << std::endl;
        }
    }





}

