//============================================================================
// Name        : BST.cpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : None
// Description : Implementation file for the Binary Search Tree (BST) used in the
//               ABCU Course App. Defines methods for the Node and BinarySearchTree
//               classes to manage course data, including insertion, deletion,
//               printing, and validation of courses and their prerequisites.
//============================================================================

#include "BST.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace BST {

    // Node class implementation.

    // Constructor: Initializes a node with a given course.
    // Parameters:
    //   course - The Course object to store in the node.
    Node::Node(Course course) : currentCourse(course) {
        // left and right are automatically initialized to nullptr by unique_ptr.
    }

    // Returns a raw pointer to the left child node.
    // Returns: Raw pointer to the left child node, or nullptr if none exists.
    Node *Node::GetLeft() {
        return this->leftTree.get();
    }

    // Returns a raw pointer to the right child node.
    // Returns: Raw pointer to the right child node, or nullptr if none exists.
    Node *Node::GetRight() {
        return this->rightTree.get();
    }

    // Transfers ownership of the left child node to the caller.
    // Returns: Unique pointer to the left child node.
    std::unique_ptr<Node> Node::MoveLeft() {
        return std::move(this->leftTree);
    }

    // Transfers ownership of the right child node to the caller.
    // Returns: Unique pointer to the right child node.
    std::unique_ptr<Node> Node::MoveRight() {
        return std::move(this->rightTree);
    }

    // Sets the left child node.
    // Parameters:
    //   node - Unique pointer to the new left child node.
    void Node::SetLeft(std::unique_ptr<Node> node) {
        this->leftTree = std::move(node);
    }

    // Sets the right child node.
    // Parameters:
    //   node - Unique pointer to the new right child node.
    void Node::SetRight(std::unique_ptr<Node> node) {
        this->rightTree = std::move(node);
    }

    // Resets the left child node to nullptr.
    void Node::ResetLeft() {
        this->leftTree.reset();
    }

    // Resets the right child node to nullptr.
    void Node::ResetRight() {
        this->rightTree.reset();
    }

    // Returns a pointer to the course stored in the node.
    Course *Node::ReturnCourse() {
        return &this->currentCourse;
    }

    // BinarySearchTree class implementation.

    // Constructor: Initializes an empty Binary Search Tree.
    BinarySearchTree::BinarySearchTree() {
        this->size = 0; // Initialize size to 0.
    }

    // Returns the number of courses in the tree.
    // Returns: The size of the tree.
    int BinarySearchTree::GetSize() {
        return this->size;
    }

    // Compares two strings case-insensitively after converting them to lowercase.
    // Parameters:
    //   first  - First string to compare.
    //   second - Second string to compare.
    // Returns: Integer (<0 if first < second, 0 if equal, >0 if first > second).
    int BinarySearchTree::CompareNoCase(std::string first, std::string second) {
        std::transform(first.begin(), first.end(), first.begin(), ::tolower);
        std::transform(second.begin(), second.end(), second.begin(), ::tolower);
        return first.compare(second);
    }

    // Inserts a new course into the tree if its ID is unique.
    // Parameters:
    //   course - The Course object to insert.
    // Returns: True if insertion is successful, false if the course ID already exists or insertion fails.
    bool BinarySearchTree::Insert(Course course) {
        // Check if course ID is unique
        if (!this->IsCourseIdUnique(course)) {
            return false;
        }

        // If tree is empty, set the root node.
        if (this->root == nullptr) {
            this->root = std::make_unique<Node>(course);
            this->size++;
            return true;
        }

        // Otherwise, recursively add the node.
        int oldSize = this->size;
        this->AddNode(this->root.get(), course);
        return this->size > oldSize; // Return true if size increased.
    }

    // Recursively adds a node with the given course to the tree.
    // Parameters:
    //   node   - Pointer to the current node in the recursive traversal.
    //   course - The Course object to insert.
    void BinarySearchTree::AddNode(Node *node, Course course) {
        if (!node) return; // Defensive check, should not occur in normal usage.

        Course *c = node->ReturnCourse();
        // Insert to left if course ID is less than current node's ID.
        if (this->CompareNoCase(c->courseId, course.courseId) > 0) {
            if (node->GetLeft() == nullptr) {
                node->SetLeft(std::make_unique<Node>(course));
                this->size++;
            } else {
                AddNode(node->GetLeft(), course);
            }
        }
        // Insert to right if course ID is greater than or equal to current node's ID.
        else {
            if (node->GetRight() == nullptr) {
                node->SetRight(std::make_unique<Node>(course));
                this->size++;
            } else {
                AddNode(node->GetRight(), course);
            }
        }
    }

    // Prints all courses in the tree in sorted order (in-order traversal).
    void BinarySearchTree::PrintOrdered() {
        if (this->root != nullptr) {
            this->InOrder(this->root.get());
        }
    }

    // Recursively performs in-order traversal to print courses.
    // Parameters:
    //   node - Pointer to the current node in the recursive traversal.
    void BinarySearchTree::InOrder(Node *node) {
        if (node->GetLeft() != nullptr) {
            this->InOrder(node->GetLeft());
        }
        this->PrintCourse(*node->ReturnCourse());
        if (node->GetRight() != nullptr) {
            this->InOrder(node->GetRight());
        }
    }

    // Recursive function to traverse tree in order and build up a vector.
    // Used by tree rebalancing function to get list of courses in order.
    void BinarySearchTree::ListInOrder(Node *node, std::vector<Course>* courses)
    {
        if (node->GetLeft() != nullptr) {
            this->ListInOrder(node->GetLeft(), courses);
        }
        courses->push_back(*node->ReturnCourse());

        if (node->GetRight() != nullptr) {
            this->ListInOrder(node->GetRight(), courses);
        }
    }

    // Prints details of a single course, including ID, name, and prerequisites.
    // Parameters:
    //   course - The Course object to print.
    void BinarySearchTree::PrintCourse(Course course) {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << course.courseId << "    " << course.courseName << std::endl;
        std::cout << "Prereqs:   ";
        for (size_t i = 0; i < course.prereqs.size(); i++) {
            if (i != 0) {
                std::cout << "           ";
            }
            std::cout << course.prereqs.at(i) << std::endl;
        }
        if (course.prereqs.size() == 0) {
            std::cout << "" << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
    }

    // Searches for a course by ID and stores it in the provided reference.
    // Parameters:
    //   id    - The course ID to search for.
    //   empty - Reference to a Course object to store the found course.
    void BinarySearchTree::FindCourse(std::string id, Course &empty) {
        if (this->root != nullptr) {
            FindCourse(this->root.get(), id, empty);
        }
    }

    // Recursively searches for a course by ID.
    // Parameters:
    //   node  - Pointer to the current node in the recursive traversal.
    //   id    - The course ID to search for.
    //   empty - Reference to a Course object to store the found course.
    void BinarySearchTree::FindCourse(Node *node, std::string id, Course &empty) {
        if (CompareNoCase(node->ReturnCourse()->courseId, id) == 0) {
            empty = *node->ReturnCourse();
        }
        if (node->GetLeft() != nullptr) {
            FindCourse(node->GetLeft(), id, empty);
        }
        if (node->GetRight() != nullptr) {
            FindCourse(node->GetRight(), id, empty);
        }
    }

    // Recursively finds courses that would become invalid due to a course deletion.
    // Parameters:
    //   node      - Pointer to the current node in the recursive traversal.
    //   courseIDs - Pointer to a vector to store affected courses.
    //   course    - The Course object to be deleted.
    void BinarySearchTree::FindCoursesInvalidRecursively(Node *node, std::vector<Course> *courseIDs, Course course) {
        if (node->GetLeft() != nullptr) {
            this->FindCoursesInvalidRecursively(node->GetLeft(), courseIDs, course);
        }

        // Check if the current course has the deleted course as a prerequisite.
        Course *c = node->ReturnCourse();
        for (size_t i = 0; i < c->prereqs.size(); i++) {
            if (CompareNoCase(c->prereqs.at(i), course.courseId) == 0) {
                courseIDs->push_back(*c);
            }
        }

        if (node->GetRight() != nullptr) {
            this->FindCoursesInvalidRecursively(node->GetRight(), courseIDs, course);
        }
    }

    // Validates all courses in the tree, ensuring valid names and prerequisites.
    // Returns: True if all courses are valid, false otherwise.
    bool BinarySearchTree::ValidateCourses() {
        std::vector<std::tuple<std::string, std::string>> list;
        this->GetListOfCourseNames(&list, this->root.get());

        bool isGood = true;
        for (size_t i = 0; i < list.size(); i++) {
            auto [x, y] = list.at(i);
            Course c;
            c.courseId = x;
            c.courseName = y;
            isGood = ValidateNameDescription(c) && isGood;
        }
        this->CheckPrereqsRecursively(this->root.get(), &isGood, &list);
        return isGood;
    }

    // Recursively collects course IDs and names into a list.
    // Parameters:
    //   list - Pointer to a vector of tuples containing course IDs and names.
    //   node - Pointer to the current node in the recursive traversal.
    void BinarySearchTree::GetListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node *node) {
        if (node->GetLeft() != nullptr) {
            this->GetListOfCourseNames(list, node->GetLeft());
        }
        list->push_back(std::make_tuple(node->ReturnCourse()->courseId, node->ReturnCourse()->courseName));
        if (node->GetRight() != nullptr) {
            this->GetListOfCourseNames(list, node->GetRight());
        }
    }

    // Recursively checks prerequisites for all courses.
    // Parameters:
    //   node - Pointer to the current node in the recursive traversal.
    //   b    - Pointer to a boolean indicating if all prerequisites are valid.
    //   list - Pointer to a vector of tuples containing course IDs and names.
    void BinarySearchTree::CheckPrereqsRecursively(Node *node, bool *b, std::vector<std::tuple<std::string, std::string>> *list) {
        if (node->GetLeft() != nullptr) {
            this->CheckPrereqsRecursively(node->GetLeft(), b, list);
        }
        std::vector<std::string> prereqs = node->ReturnCourse()->prereqs;
        if (!prereqs.empty()) {
            bool badCourse = this->CheckPrereqsOneCourse(*node->ReturnCourse(), *list);
            if(!badCourse){
                std::cout << "Bad Course: " << node->ReturnCourse()->courseId << std::endl;
                *b = badCourse;
            }

        }
        if (node->GetRight() != nullptr) {
            this->CheckPrereqsRecursively(node->GetRight(), b, list);
        }
    }

    // Prints details of a single course by ID.
    // Parameters:
    //   id - The course ID to print.
    void BinarySearchTree::PrintOneCourse(std::string id) {
        Course course;
        this->FindCourse(id, course);
        if (!course.courseId.empty() && !course.courseName.empty()) {
            this->PrintCourse(course);
        } else {
            std::cout << "Course not found." << std::endl;
        }
    }

    // Prints top 3 tiers of tree for visualization of BST's balance.
    void BinarySearchTree::PrintTopThreeLevelsOfTree()
    {
        std::string root = " NULL  ";
        std::string left = " NULL  ";
        std::string right = " NULL  ";
        std::string leftLeft = " NULL  ";
        std::string leftRight = " NULL  ";
        std::string rightLeft = " NULL  ";
        std::string rightRight = " NULL  ";
        if(this->root.get() == nullptr){
            std::cout << "       " << std::endl;
            return;
        }else{
            root = this->root.get()->ReturnCourse()->courseId;
            Node* leftNode = this->root.get()->GetLeft();
            Node* rightNode = this->root.get()->GetRight();

            if(leftNode != nullptr){
                left = leftNode->ReturnCourse()->courseId;       
                Node* leftLeftNode = leftNode->GetLeft();
                Node* leftRightNode = leftNode->GetRight();
                if(leftLeftNode != nullptr){
                    leftLeft = leftLeftNode->ReturnCourse()->courseId;
                }
                if(leftRightNode != nullptr){
                    leftRight = leftRightNode->ReturnCourse()->courseId;
                }
            }
            if(rightNode != nullptr){
                right = rightNode->ReturnCourse()->courseId;
                Node* rightLeftNode = rightNode->GetLeft();
                Node* rightRightNode = rightNode->GetRight();
                if(rightLeftNode != nullptr){
                    rightLeft = rightLeftNode->ReturnCourse()->courseId;
                }
                if(rightRightNode != nullptr){
                    rightRight = rightRightNode->ReturnCourse()->courseId;
                }
            }
        }

        std::cout << "                    " << root << std::endl;
        std::cout << "         " << left << "             " << right << std::endl;
        std::cout << "     " << leftLeft << "  " << leftRight << "   " << rightLeft << "  " << rightRight << std::endl;
    }

    // Rebalances tree by calling private function ListInOrder with root.
    // Once ordered vector is built, this clears BST and loads from middle.
    void BinarySearchTree::RebalanceTree()
    {
        // Safety check.
        if(this->root.get() == nullptr){
            return;
        }

        // Built vector in sorted order.
        std::vector<Course> courses;
        ListInOrder(this->root.get(), &courses);

        if(courses.empty()){
            return;
        }

        this->Clear();

        // Use for loops to reinsert.
        this->root = std::move(BuildBalancedTree(courses, 0, courses.size()));
        this->size = courses.size();
    }

    
    // Called Recursively to build tree balanced. Similar to
    // Quicksort algorithm where it partitions vector into chunks until it reaches 
    // batches of 1. 
    // Parameters
    //  courses - ordered vector of courses.
    //  start  - low end of range to work on.
    //  end  - high end of range to work on.
    std::unique_ptr<Node> BinarySearchTree::BuildBalancedTree(const std::vector<Course> &courses, size_t start, size_t end)
    {
        if(start >= end || start >= courses.size()){
            return nullptr;
        }

        size_t mid = start + (end-start)/2;
        std::unique_ptr<Node> node = std::make_unique<Node>(courses[mid]);
        node->SetLeft(BuildBalancedTree(courses, start, mid));
        node->SetRight(BuildBalancedTree(courses, mid + 1, end));

        return node;
    }

    // Clears all nodes in the tree.
    void BinarySearchTree::Clear() {
        if (this->root == nullptr) {
            return; // Empty tree
        }
        if (this->root->GetLeft() != nullptr) {
            this->RecursiveClear(this->root->GetLeft());
        }
        if (this->root->GetRight() != nullptr) {
            this->RecursiveClear(this->root->GetRight());
        }
        this->root->ResetLeft();
        this->root->ResetRight();
        this->root = nullptr;
        this->size = 0; // Reset size to 0.
    }

    // Validates the name and description of a course.
    // Parameters:
    //   course - The Course object to validate.
    // Returns: True if the course ID and name are valid and prerequisites exist, false otherwise.
    bool BinarySearchTree::ValidateNameDescription(Course course) {
        // Check course ID length (must be exactly 7 characters).
        if (course.courseId.length() != 7) {
            return false;
        }
        // Check course name length (must be between 3 and 40 characters).
        if (course.courseName.length() < 3 || course.courseName.length() > 40) {
            return false;
        }
        // If no prerequisites, course is valid.
        if (course.prereqs.size() < 1) {
            return true;
        }
        // Check if all prerequisites exist in the tree.
        std::vector<std::tuple<std::string, std::string>> names;
        GetListOfCourseNames(&names, this->root.get());
        return CheckPrereqsOneCourse(course, names);
    }

    // Checks if a course's prerequisites exist in the tree.
    // Parameters:
    //   course - Reference to the Course object to validate.
    //   names  - Reference to a vector of tuples containing course IDs and names.
    // Returns: True if all prerequisites exist, false otherwise.
    bool BinarySearchTree::CheckPrereqsOneCourse(BST::Course &course, std::vector<std::tuple<std::string, std::string>> &names) {
        for (size_t j = 0; j < course.prereqs.size(); j++) {
            bool found = false;
            for (size_t i = 0; i < names.size(); i++) {
                auto [x, y] = names.at(i);
                if (CompareNoCase(course.prereqs.at(j), x) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }

    // Checks if a course ID is unique in the tree.
    // Parameters:
    //   course - The Course object to check.
    // Returns: True if the course ID is unique, false if it already exists.
    bool BinarySearchTree::IsCourseIdUnique(Course course) {
        if (this->root == nullptr) {
            return true; // Empty tree, ID is unique.
        }
        std::vector<std::tuple<std::string, std::string>> list;
        GetListOfCourseNames(&list, this->root.get());
        if (list.size() < 1) {
            return true; // No courses, ID is unique.
        }
        for (size_t i = 0; i < list.size(); i++) {
            auto [x, y] = list.at(i);
            if (CompareNoCase(x, course.courseId) == 0) {
                return false; // ID already exists
            }
        }
        return true;
    }

    // Recursively clears all nodes in the subtree rooted at the given node.
    // Parameters:
    //   node - Pointer to the current node in the recursive traversal.
    void BinarySearchTree::RecursiveClear(Node *node) {
        if (node->GetLeft() != nullptr) {
            this->RecursiveClear(node->GetLeft());
        }
        if (node->GetRight() != nullptr) {
            this->RecursiveClear(node->GetRight());
        }
        node->ResetLeft();
        node->ResetRight();
        this->size--;
    }


} // namespace BST