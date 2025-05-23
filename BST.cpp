#include "BST.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace BST {

    // Node definition

    // Public method to return left child node.
    Node* Node::GetLeft() {
        return this->left.get();
    }

    // Public method to return right child node.
    Node* Node::GetRight() {   
        return this->right.get();
    }

    // Public method to set left child node (updated to take unique_ptr).
    void Node::SetLeft(std::unique_ptr<Node> node) {
        this->left = std::move(node);
    }

    // Public method to set right child node (updated to take unique_ptr).
    void Node::SetRight(std::unique_ptr<Node> node) {
        this->right = std::move(node);
    }

    // Public method to return pointer to node's course structure.
    Course* Node::ReturnCourse() {
        return &this->current;
    }

    // Constructor for node class to instantiate with a course.
    Node::Node(Course course) : current(course) {
        // left and right are automatically nullptr (no need to set explicitly)
    }

    // BST 
    BinarySearchTree::BinarySearchTree() {
        this->size = 0;
        // root is automatically nullptr
    }

    // Function compares strings after converting them to lowercase, for case-insensitive comparisons.
    int BinarySearchTree::compareNoCase(std::string first, std::string second) {
        transform(first.begin(), first.end(), first.begin(), ::tolower);
        transform(second.begin(), second.end(), second.begin(), ::tolower);
        return first.compare(second);
    }

    void BinarySearchTree::Insert(Course course) {
        if (this->root == nullptr) {
            this->root = std::make_unique<Node>(course);
            this->size++;    
        } else {
            this->addNode(this->root.get(), course);
        }
    }

    void BinarySearchTree::addNode(Node* node, Course course) {
        if (!node) return; // Defensive check, should not happen in normal usage

        Course* c = node->ReturnCourse();
        if (this->compareNoCase(c->courseId, course.courseId) < 0) {
            if (node->GetLeft() == nullptr) {
                node->SetLeft(std::make_unique<Node>(course));
                this->size++;
            } else {
                addNode(node->GetLeft(), course);
            }
        } else {
            if (node->GetRight() == nullptr) {
                node->SetRight(std::make_unique<Node>(course));
                this->size++;
            } else {
                addNode(node->GetRight(), course);
            }
        }
    }

    void BinarySearchTree::PrintOrdered() {
        if (this->root != nullptr) {
            this->inOrder(this->root.get());
        }
    }

    void BinarySearchTree::inOrder(Node* node) {
        if (node->GetLeft() != nullptr) {
            this->inOrder(node->GetLeft());
        }
        
        this->printCourse(*node->ReturnCourse());

        if (node->GetRight() != nullptr) {
            this->inOrder(node->GetRight());
        }
    }

    void BinarySearchTree::printCourse(Course course) {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << course.courseId << "    " << course.courseName << std::endl;
        std::cout << "Prereqs:   ";
        for (size_t i = 0; i < course.prereqs.size(); i++) {
            if (i != 0) {
                std::cout << "           ";
            }
            std::cout << course.prereqs.at(i) << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
    }
void BinarySearchTree::FindCourse(std::string id, Course& empty) {
        if (this->root != nullptr) {
            findCourse(this->root.get(), id, empty);
        }
    }

    void BinarySearchTree::findCourse(Node* node, std::string id, Course& empty) {
        if (compareNoCase(node->ReturnCourse()->courseId, id) == 0) {
            empty = *node->ReturnCourse();
        }
        if (node->GetLeft() != nullptr) {
            findCourse(node->GetLeft(), id, empty);
        }
        if (node->GetRight() != nullptr) {
            findCourse(node->GetRight(), id, empty);
        }
    }

    bool BinarySearchTree::ValidateCourses() {
        std::vector<std::tuple<std::string, std::string>> list;
        auto listptr = &list;
        Node* n = this->root.get();
        this->getListOfCourseNames(listptr, n);

        bool isGood = true;
        bool* isGoodptr = &isGood;
        for (size_t i = 0; i < list.size(); i++) {
            auto [x, y] = list.at(i);
            if (x.empty() || y.empty()) {
                isGood = false;
            }
        }
        this->checkPrereqs(this->root.get(), isGoodptr, listptr);
        return isGood;
    }

    void BinarySearchTree::getListOfCourseNames(std::vector<std::tuple<std::string, std::string>>* list, Node* node) {
        if (node->GetLeft() != nullptr) {
            this->getListOfCourseNames(list, node->GetLeft());
        }
        std::tuple<std::string, std::string> myTuple(node->ReturnCourse()->courseId, node->ReturnCourse()->courseName);
        list->push_back(myTuple);

        if (node->GetRight() != nullptr) {
            this->getListOfCourseNames(list, node->GetRight());
        }
    }

    void BinarySearchTree::checkPrereqs(Node* node, bool* b, std::vector<std::tuple<std::string, std::string>>* list) {
        if (node->GetLeft() != nullptr) {
            this->checkPrereqs(node->GetLeft(), b, list);
        }
        std::vector<std::string> prereqs = node->ReturnCourse()->prereqs;
        
        if (!prereqs.empty()) {
            for (size_t i = 0; i < prereqs.size(); i++) {
                bool isGood = false;
                for (size_t j = 0; j < list->size(); j++) {
                    auto [x, y] = list->at(j);
                    if (prereqs.at(i) == x) {
                        isGood = true;
                    }
                }
                if (!isGood) {
                    *b = false;
                }
            }
        }
        if (node->GetRight() != nullptr) {
            this->checkPrereqs(node->GetRight(), b, list);
        }
    }

    void BinarySearchTree::PrintOneCourse(std::string id) {
        Course course;
        this->FindCourse(id, course);
        if (!course.courseId.empty() && !course.courseName.empty()) {
            this->printCourse(course);
        } else {
            std::cout << "Course not found." << std::endl;
        }
    }

} // namespace BST