//============================================================================
// Name        : BST.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : None
// Description : Header file defining the Binary Search Tree (BST) and related
//               structures for the ABCU Course App. Contains declarations for
//               Course structure, Node class, and BinarySearchTree class to manage
//               course data, including sorting and validating courses by prerequisites.
//============================================================================

#ifndef _BST_HPP_
#define _BST_HPP_

#include <string>
#include <vector>
#include <memory>

namespace BST {

    // Structure representing a course in the ABCU Course App.
    struct Course {
        std::string courseId;      // Unique identifier for the course.
        std::string courseName;    // Name of the course.
        std::vector<std::string> prereqs; // List of prerequisite course IDs.
    };

    // Node class representing a node in the Binary Search Tree, holding a Course object.
    class Node {
    private:
        std::unique_ptr<Node> leftTree;   // Pointer to the left child node.
        std::unique_ptr<Node> rightTree;  // Pointer to the right child node.
        Course currentCourse;               // Course data stored in the node.

    public:
        // Constructor: Initializes a node with a given course.
        // Parameters:
        //   course - The Course object to store in the node.
        Node(Course course);

        // Returns a pointer to the course stored in the node.
        Course* ReturnCourse();

        // Gets the left child node.
        // Returns: Raw pointer to the left child node.
        Node* GetLeft();

        // Gets the right child node.
        // Returns: Raw pointer to the right child node.
        Node* GetRight();

        // Moves ownership of the left child node to the caller.
        // Returns: Unique pointer to the left child node.
        std::unique_ptr<Node> MoveLeft();

        // Moves ownership of the right child node to the caller.
        // Returns: Unique pointer to the right child node.
        std::unique_ptr<Node> MoveRight();

        // Sets the left child node.
        // Parameters:
        //   node - Unique pointer to the new left child node.
        void SetLeft(std::unique_ptr<Node> leftNode);

        // Sets the right child node.
        // Parameters:
        //   node - Unique pointer to the new right child node.
        void SetRight(std::unique_ptr<Node> rightNode);

        // Resets the left child node to nullptr.
        void ResetLeft();

        // Resets the right child node to nullptr.
        void ResetRight();
    };

    // Binary Search Tree class for managing courses, sorted by course ID and validated by prerequisites.
    class BinarySearchTree {
    private:
        int size;                     // Number of nodes in the tree.
        std::unique_ptr<Node> root;   // Root node of the tree.

        // Recursively adds a node with the given course to the tree.
        // Parameters:
        //   node   - Pointer to the current node in the recursive traversal.
        //   course - The Course object to insert.
        void addNode(Node* node, Course course);

        // Performs case-insensitive string comparison.
        // Parameters:
        //   first  - First string to compare.
        //   second - Second string to compare.
        // Returns: Integer indicating comparison result (<0 if first < second, 0 if equal, >0 if first > second).
        int compareNoCase(std::string first, std::string second);

        // Recursively prints courses in-order (sorted by course ID).
        // Parameters:
        //   node - Pointer to the current node in the recursive traversal.
        void inOrder(Node* node);

        // Prints details of a single course.
        // Parameters:
        //   course - The Course object to print.
        void printCourse(Course course);

        // Recursively searches for a course by ID and stores it in the provided reference.
        // Parameters:
        //   node  - Pointer to the current node in the recursive traversal.
        //   id    - The course ID to search for.
        //   empty - Reference to a Course object to store the found course.
        void findCourse(Node* node, std::string courseId, Course& empty);

        // Recursively collects course IDs and names into a list.
        // Parameters:
        //   list - Pointer to a vector of tuples containing course IDs and names.
        //   node - Pointer to the current node in the recursive traversal.
        void getListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node* node);

        // Recursively checks prerequisites for all courses.
        // Parameters:
        //   node - Pointer to the current node in the recursive traversal.
        //   isGood - Pointer to a boolean indicating if all prerequisites are valid.
        //   list - Pointer to a vector of tuples containing course IDs and names.
        void checkPrereqsRecursively(Node* node, bool* isGood, std::vector<std::tuple<std::string, std::string>> *list);

        // Validates the name and description of a course.
        // Parameters:
        //   course - The Course object to validate.
        // Returns: True if the course name and description are valid, false otherwise.
        bool validateNameDesciption(Course course);

        // Checks if a course's prerequisites exist in the tree.
        // Parameters:
        //   course - Reference to the Course object to validate.
        //   names  - Reference to a vector of tuples containing course IDs and names.
        // Returns: True if all prerequisites exist, false otherwise.
        bool checkPrereqsOneCourse(BST::Course &course, std::vector<std::tuple<std::string, std::string>> &list);

        // Checks if a course ID is unique in the tree.
        // Parameters:
        //   course - The Course object to check.
        // Returns: True if the course ID is unique, false if it already exists.
        bool isCourseIdUnique(Course course);

        // Recursively finds courses that would become invalid if a given course is deleted.
        // Parameters:
        //   node      - Pointer to the current node in the recursive traversal.
        //   courseIDs - Pointer to a vector to store IDs of affected courses.
        //   course    - The Course object to be deleted.
        void findCoursesInvalidRecursively(Node* node, std::vector<Course>* coursesList, Course course);

        // Recursively clears all nodes in the tree.
        // Parameters:
        //   node - Pointer to the current node in the recursive traversal.
        void recursiveClear(Node* node);

    public:
        // Constructor: Initializes an empty Binary Search Tree.
        BinarySearchTree();

        // Returns the number of courses in the tree.
        int GetSize();

        // Inserts a new course into the tree.
        // Parameters:
        //   course - The Course object to insert.
        // Returns: True if insertion is successful, false otherwise.
        bool Insert(Course course);

        // Prints all courses in the tree in sorted order.
        void PrintOrdered();

        // Searches for a course by ID and stores it in the provided reference.
        // Parameters:
        //   id    - The course ID to search for.
        //   empty - Reference to a Course object to store the found course.
        void FindCourse(std::string id, Course& empty);

        // Finds courses that would become invalid if a specified course is deleted.
        // Parameters:
        //   coursetodelete - The ID of the course to be deleted.
        // Returns: A vector of course IDs that would be affected by the deletion.
        std::vector<std::string> FindCoursesInvalidOnDelete(std::string courseToDelete);

        // Validates all courses in the tree, ensuring valid prerequisites.
        // Returns: True if all courses are valid, false otherwise.
        bool ValidateCourses();

        // Prints details of a single course by ID.
        // Parameters:
        //   id - The course ID to print.
        void PrintOneCourse(std::string courseId);

        // Removes a course from the tree by ID.
        // Parameters:
        //   id - The course ID to remove.
        // Returns: True if the course was removed, false if not found.
        bool RemoveCoursewithId(std::string courseId);

        // Validates a single course, checking its ID, name, and prerequisites.
        // Parameters:
        //   course - The Course object to validate.
        // Returns: True if the course is valid, false otherwise.
        bool ValidateSingleCourse(Course course);

        // Clears all courses from the tree.
        void Clear();
    };

} // namespace BST

#endif // !_BST_HPP_