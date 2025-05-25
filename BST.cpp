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
    int BinarySearchTree::compareNoCase(std::string first, std::string second) {
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
        if (!this->isCourseIdUnique(course)) {
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
        this->addNode(this->root.get(), course);
        return this->size > oldSize; // Return true if size increased.
    }

    // Recursively adds a node with the given course to the tree.
    // Parameters:
    //   node   - Pointer to the current node in the recursive traversal.
    //   course - The Course object to insert.
    void BinarySearchTree::addNode(Node *node, Course course) {
        if (!node) return; // Defensive check, should not occur in normal usage.

        Course *c = node->ReturnCourse();
        // Insert to left if course ID is less than current node's ID.
        if (this->compareNoCase(c->courseId, course.courseId) > 0) {
            if (node->GetLeft() == nullptr) {
                node->SetLeft(std::make_unique<Node>(course));
                this->size++;
            } else {
                addNode(node->GetLeft(), course);
            }
        }
        // Insert to right if course ID is greater than or equal to current node's ID.
        else {
            if (node->GetRight() == nullptr) {
                node->SetRight(std::make_unique<Node>(course));
                this->size++;
            } else {
                addNode(node->GetRight(), course);
            }
        }
    }

    // Prints all courses in the tree in sorted order (in-order traversal).
    void BinarySearchTree::PrintOrdered() {
        if (this->root != nullptr) {
            this->inOrder(this->root.get());
        }
    }

    // Recursively performs in-order traversal to print courses.
    // Parameters:
    //   node - Pointer to the current node in the recursive traversal.
    void BinarySearchTree::inOrder(Node *node) {
        if (node->GetLeft() != nullptr) {
            this->inOrder(node->GetLeft());
        }
        this->printCourse(*node->ReturnCourse());
        if (node->GetRight() != nullptr) {
            this->inOrder(node->GetRight());
        }
    }

    // Prints details of a single course, including ID, name, and prerequisites.
    // Parameters:
    //   course - The Course object to print.
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
            findCourse(this->root.get(), id, empty);
        }
    }

    // Recursively searches for a course by ID.
    // Parameters:
    //   node  - Pointer to the current node in the recursive traversal.
    //   id    - The course ID to search for.
    //   empty - Reference to a Course object to store the found course.
    void BinarySearchTree::findCourse(Node *node, std::string id, Course &empty) {
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

    // Finds courses that would become invalid if a specified course is deleted.
    // Parameters:
    //   coursetodelete - The ID of the course to be deleted.
    // Returns: A vector of course IDs that would be affected by the deletion.
    std::vector<std::string> BinarySearchTree::FindCoursesInvalidOnDelete(std::string coursetodelete) {
        std::vector<Course> removeList;
        std::vector<std::string> removeListStr;

        if (this->size == 0) {
            return removeListStr; // Empty tree, return empty list.
        }

        // Find the course to delete.
        Course c;
        FindCourse(coursetodelete, c);
        if (c.courseId.size() > 0) {
            removeList.push_back(c);
        } else {
            return removeListStr; // Course not found, return empty list.
        }

        // Recursively find all courses that depend on the deleted course.
        int needDeleted = removeList.size();
        int deleted = 0;
        while (needDeleted > deleted) {
            findCoursesInvalidRecursively(this->root.get(), &removeList, removeList.at(deleted));
            deleted++;
            needDeleted = removeList.size();
        }

        // Convert course objects to their IDs.
        for (size_t i = 0; i < removeList.size(); i++) {
            removeListStr.push_back(removeList.at(i).courseId);
        }
        return removeListStr;
    }

    // Recursively finds courses that would become invalid due to a course deletion.
    // Parameters:
    //   node      - Pointer to the current node in the recursive traversal.
    //   courseIDs - Pointer to a vector to store affected courses.
    //   course    - The Course object to be deleted.
    void BinarySearchTree::findCoursesInvalidRecursively(Node *node, std::vector<Course> *courseIDs, Course course) {
        if (node->GetLeft() != nullptr) {
            this->findCoursesInvalidRecursively(node->GetLeft(), courseIDs, course);
        }

        // Check if the current course has the deleted course as a prerequisite.
        Course *c = node->ReturnCourse();
        for (size_t i = 0; i < c->prereqs.size(); i++) {
            if (compareNoCase(c->prereqs.at(i), course.courseId) == 0) {
                courseIDs->push_back(*c);
            }
        }

        if (node->GetRight() != nullptr) {
            this->findCoursesInvalidRecursively(node->GetRight(), courseIDs, course);
        }
    }

    // Validates all courses in the tree, ensuring valid names and prerequisites.
    // Returns: True if all courses are valid, false otherwise.
    bool BinarySearchTree::ValidateCourses() {
        std::vector<std::tuple<std::string, std::string>> list;
        this->getListOfCourseNames(&list, this->root.get());

        bool isGood = true;
        for (size_t i = 0; i < list.size(); i++) {
            auto [x, y] = list.at(i);
            Course c;
            c.courseId = x;
            c.courseName = y;
            isGood = validateNameDesciption(c) && isGood;
        }
        this->checkPrereqsRecursively(this->root.get(), &isGood, &list);
        return isGood;
    }

    // Recursively collects course IDs and names into a list.
    // Parameters:
    //   list - Pointer to a vector of tuples containing course IDs and names.
    //   node - Pointer to the current node in the recursive traversal.
    void BinarySearchTree::getListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node *node) {
        if (node->GetLeft() != nullptr) {
            this->getListOfCourseNames(list, node->GetLeft());
        }
        list->push_back(std::make_tuple(node->ReturnCourse()->courseId, node->ReturnCourse()->courseName));
        if (node->GetRight() != nullptr) {
            this->getListOfCourseNames(list, node->GetRight());
        }
    }

    // Recursively checks prerequisites for all courses.
    // Parameters:
    //   node - Pointer to the current node in the recursive traversal.
    //   b    - Pointer to a boolean indicating if all prerequisites are valid.
    //   list - Pointer to a vector of tuples containing course IDs and names.
    void BinarySearchTree::checkPrereqsRecursively(Node *node, bool *b, std::vector<std::tuple<std::string, std::string>> *list) {
        if (node->GetLeft() != nullptr) {
            this->checkPrereqsRecursively(node->GetLeft(), b, list);
        }
        std::vector<std::string> prereqs = node->ReturnCourse()->prereqs;
        if (!prereqs.empty()) {
            *b = this->checkPrereqsOneCourse(*node->ReturnCourse(), *list) && *b;
        }
        if (node->GetRight() != nullptr) {
            this->checkPrereqsRecursively(node->GetRight(), b, list);
        }
    }

    // Prints details of a single course by ID.
    // Parameters:
    //   id - The course ID to print.
    void BinarySearchTree::PrintOneCourse(std::string id) {
        Course course;
        this->FindCourse(id, course);
        if (!course.courseId.empty() && !course.courseName.empty()) {
            this->printCourse(course);
        } else {
            std::cout << "Course not found." << std::endl;
        }
    }

    // Removes a course from the tree by ID.
    // Parameters:
    //   id - The course ID to remove.
    // Returns: True if the course was removed, false if not found.
    bool BinarySearchTree::RemoveCoursewithId(std::string id) {
        Node *parent = nullptr;
        Node *nodeToDelete = this->root.get();
        if (nodeToDelete == nullptr) {
            return false; 
        }

        // Special case: Removing the root node.
        if (compareNoCase(nodeToDelete->ReturnCourse()->courseId, id) == 0) {
            if (nodeToDelete->GetLeft() == nullptr && nodeToDelete->GetRight() == nullptr) {
                this->root.reset();
                this->size--;
                return true;
            } else if (nodeToDelete->GetLeft() == nullptr) {
                this->root = this->root->MoveRight();
                this->size--;
                return true;
            } else if (nodeToDelete->GetRight() == nullptr) {
                this->root = this->root->MoveLeft();
                this->size--;
                return true;
            } else {
                Node *parent = nullptr;
                Node *successor = this->root->GetRight();
                while (successor->GetLeft() != nullptr) {
                    parent = successor;
                    successor = successor->GetLeft();
                }
                *this->root->ReturnCourse() = *successor->ReturnCourse();
                if (parent == nullptr) {
                    this->root->ResetRight();
                } else {
                    parent->ResetLeft();
                }
                this->size--;
                return true;
            }
        }

        // Find the node to delete and its parent.
        while (nodeToDelete) {
            if (compareNoCase(nodeToDelete->ReturnCourse()->courseId, id) > 0) {
                parent = nodeToDelete;
                nodeToDelete = nodeToDelete->GetLeft();
            } else if (compareNoCase(nodeToDelete->ReturnCourse()->courseId, id) < 0) {
                parent = nodeToDelete;
                nodeToDelete = nodeToDelete->GetRight();
            } else {
                break;
            }
        }
        if (!nodeToDelete) {
            return false; 
        }

        // Determine if nodeToDelete is parent's left or right child.
        bool isLeftChild = parent && compareNoCase(nodeToDelete->ReturnCourse()->courseId, parent->ReturnCourse()->courseId) < 0;

        // Case 1: No children
        if (nodeToDelete->GetLeft() == nullptr && nodeToDelete->GetRight() == nullptr) {
            if (isLeftChild) {
                parent->ResetLeft();
            } else if (parent) {
                parent->ResetRight();
            }
            this->size--;
            return true;
        }
        // Case 2: No left child.
        else if (nodeToDelete->GetLeft() == nullptr) {
            if (isLeftChild) {
                parent->SetLeft(std::move(nodeToDelete->MoveRight()));
            } else if (parent) {
                parent->SetRight(std::move(nodeToDelete->MoveRight()));
            }
            this->size--;
            return true;
        }
        // Case 3: No right child.
        else if (nodeToDelete->GetRight() == nullptr) {
            if (isLeftChild) {
                parent->SetLeft(std::move(nodeToDelete->MoveLeft()));
            } else if (parent) {
                parent->SetRight(std::move(nodeToDelete->MoveLeft()));
            }
            this->size--;
            return true;
        }
        // Case 4: Two children.
        else {
            Node *succParent = nullptr;
            Node *successor = nodeToDelete->GetRight();
            while (successor->GetLeft() != nullptr) {
                succParent = successor;
                successor = successor->GetLeft();
            }
            *nodeToDelete->ReturnCourse() = *successor->ReturnCourse();
            if (succParent == nullptr) {
                nodeToDelete->ResetRight();
            } else {
                succParent->ResetLeft();
            }
            this->size--;
            return true;
        }
    }

    // Validates a single course, checking its ID, name, and prerequisites.
    // Parameters:
    //   course - The Course object to validate.
    // Returns: True if the course is valid, false otherwise.
    bool BinarySearchTree::ValidateSingleCourse(Course course) {
        return validateNameDesciption(course);
    }

    // Clears all nodes in the tree.
    void BinarySearchTree::Clear() {
        if (this->root == nullptr) {
            return; // Empty tree
        }
        if (this->root->GetLeft() != nullptr) {
            this->recursiveClear(this->root->GetLeft());
        }
        if (this->root->GetRight() != nullptr) {
            this->recursiveClear(this->root->GetRight());
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
    bool BinarySearchTree::validateNameDesciption(Course course) {
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
        getListOfCourseNames(&names, this->root.get());
        return checkPrereqsOneCourse(course, names);
    }

    // Checks if a course's prerequisites exist in the tree.
    // Parameters:
    //   course - Reference to the Course object to validate.
    //   names  - Reference to a vector of tuples containing course IDs and names.
    // Returns: True if all prerequisites exist, false otherwise.
    bool BinarySearchTree::checkPrereqsOneCourse(BST::Course &course, std::vector<std::tuple<std::string, std::string>> &names) {
        for (size_t j = 0; j < course.prereqs.size(); j++) {
            bool found = false;
            for (size_t i = 0; i < names.size(); i++) {
                auto [x, y] = names.at(i);
                if (compareNoCase(course.prereqs.at(j), x) == 0) {
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
    bool BinarySearchTree::isCourseIdUnique(Course course) {
        if (this->root == nullptr) {
            return true; // Empty tree, ID is unique.
        }
        std::vector<std::tuple<std::string, std::string>> list;
        getListOfCourseNames(&list, this->root.get());
        if (list.size() < 1) {
            return true; // No courses, ID is unique.
        }
        for (size_t i = 0; i < list.size(); i++) {
            auto [x, y] = list.at(i);
            if (compareNoCase(x, course.courseId) == 0) {
                return false; // ID already exists
            }
        }
        return true;
    }

    // Recursively clears all nodes in the subtree rooted at the given node.
    // Parameters:
    //   node - Pointer to the current node in the recursive traversal.
    void BinarySearchTree::recursiveClear(Node *node) {
        if (node->GetLeft() != nullptr) {
            this->recursiveClear(node->GetLeft());
        }
        if (node->GetRight() != nullptr) {
            this->recursiveClear(node->GetRight());
        }
        node->ResetLeft();
        node->ResetRight();
        this->size--;
    }

} // namespace BST