
// Node definitions
#include "BST.hpp"
#include <algorithm>
#include <iostream>


Node* Node::GetLeft(){
    return this->left;
}

Node* Node::GetRight(){   
    return this->right;
}

void Node::SetLeft(Node* node){
    this->left = node;
}

void Node::SetRight(Node* node){
    this->right = node;
}

Course* Node::ReturnCourse(){
    Course* c = &this->current;
    return c;
}

Node::Node(Course course){
    this->current = course;
    this->left = nullptr;
    this->right = nullptr;
}

BST::BST(){
    this->size = 0;
    this->root = nullptr;
}

void BST::Insert(Course course){
    if(this->root == nullptr){
        Node* node = new Node(course);
        this->root = node;
        this->size++;    
    }else{
        this->addNode(this->root, course);
    }
}

void BST::addNode(Node* node, Course course){
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


// Function compares strings after converting them to lowercase, for case insensitive comparisons.
int BST::compareNoCase(std::string first, std::string second){
    // Use transform function by passing iterators and function to use: ::tolower. Then return
    // the comparison.
    transform(first.begin(), first.end(),first.begin(), ::tolower);
    transform(second.begin(), second.end(),second.begin(), ::tolower);
    return first.compare(second);
}

void BST::InOrder(Node* node){
    if(node != nullptr){
        this->InOrder(node->GetLeft());
        this->PrintCourse(*node->ReturnCourse());
        this->InOrder(node->GetRight());
    }
}

void BST::PrintOrdered(){
    this->InOrder(this->root);
}

void BST::PrintCourse(Course course){
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "  " << course.courseId << "  " << course.courseName << std::endl;
    
    for (size_t i = 0; i < course.prereqs.size(); i++)
    {
        std::cout << "            ";
        std::cout << course.prereqs.at(i) << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}