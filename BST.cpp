#include "BST.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace BST
{

    // Node definition

    // Public method to return left child node.
    Node *Node::GetLeft()
    {
        return this->left.get();
    }

    // Public method to return right child node.
    Node *Node::GetRight()
    {
        return this->right.get();
    }

    std::unique_ptr<Node> Node::MoveLeft()
    {
        return std::move(this->left);
    }

    std::unique_ptr<Node> Node::MoveRight()
    {
        return std::move(this->right);
    }

    // Public method to set left child node (updated to take unique_ptr).
    void Node::SetLeft(std::unique_ptr<Node> node)
    {
        this->left = std::move(node);
    }

    // Public method to set right child node (updated to take unique_ptr).
    void Node::SetRight(std::unique_ptr<Node> node)
    {
        this->right = std::move(node);
    }

    void Node::ResetLeft()
    {
        this->left.reset();
    }

    void Node::ResetRight()
    {
        this->right.reset();
    }

    // Public method to return pointer to node's course structure.
    Course *Node::ReturnCourse()
    {
        return &this->current;
    }

    // Constructor for node class to instantiate with a course.
    Node::Node(Course course) : current(course)
    {
        // left and right are automatically nullptr (no need to set explicitly)
    }

    // BST
    BinarySearchTree::BinarySearchTree()
    {
        this->size = 0;
        // root is automatically nullptr
    }

    // Function compares strings after converting them to lowercase, for case-insensitive comparisons.
    int BinarySearchTree::compareNoCase(std::string first, std::string second)
    {
        transform(first.begin(), first.end(), first.begin(), ::tolower);
        transform(second.begin(), second.end(), second.begin(), ::tolower);
        int b = first.compare(second);
        return b;
    }

    bool BinarySearchTree::Insert(Course course)
    {
        bool b = this->isCourseIdUnique(course);
        if(!b){
            return false;
        }
        if (this->root == nullptr)
        {
            this->root = std::make_unique<Node>(course);
            this->size++;
            return true;
        }
        else
        {
            int size = this->size;
            this->addNode(this->root.get(), course);
            if(size < this->size){
                return true;
            }else{
                return false;
            }
        }
    }

    void BinarySearchTree::addNode(Node *node, Course course)
    {
        if (!node)
            return; // Defensive check, should not happen in normal usage

        Course *c = node->ReturnCourse();
        if (this->compareNoCase(c->courseId, course.courseId) > 0)
        {
            if (node->GetLeft() == nullptr)
            {
                node->SetLeft(std::make_unique<Node>(course));
                this->size++;
            }
            else
            {
                addNode(node->GetLeft(), course);
            }
        }
        else
        {
            if (node->GetRight() == nullptr)
            {
                node->SetRight(std::make_unique<Node>(course));
                this->size++;
            }
            else
            {
                addNode(node->GetRight(), course);
            }
        }
    }

    void BinarySearchTree::PrintOrdered()
    {
        if (this->root != nullptr)
        {
            this->inOrder(this->root.get());
        }
    }

    void BinarySearchTree::inOrder(Node *node)
    {
        if (node->GetLeft() != nullptr)
        {
            this->inOrder(node->GetLeft());
        }

        this->printCourse(*node->ReturnCourse());

        if (node->GetRight() != nullptr)
        {
            this->inOrder(node->GetRight());
        }
    }

    void BinarySearchTree::printCourse(Course course)
    {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << course.courseId << "    " << course.courseName << std::endl;
        std::cout << "Prereqs:   ";
        for (size_t i = 0; i < course.prereqs.size(); i++)
        {
            if (i != 0)
            {
                std::cout << "           ";
            }
            std::cout << course.prereqs.at(i) << std::endl;
        }
        if(course.prereqs.size() == 0){
            std::cout << "" << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
    }
    void BinarySearchTree::FindCourse(std::string id, Course &empty)
    {
        if (this->root != nullptr)
        {
            findCourse(this->root.get(), id, empty);
        }
    }

   

    void BinarySearchTree::findCourse(Node *node, std::string id, Course &empty)
    {
        if (compareNoCase(node->ReturnCourse()->courseId, id) == 0)
        {
            empty = *node->ReturnCourse();
        }
        if (node->GetLeft() != nullptr)
        {
            findCourse(node->GetLeft(), id, empty);
        }
        if (node->GetRight() != nullptr)
        {
            findCourse(node->GetRight(), id, empty);
        }
    }

     std::vector<std::string> BinarySearchTree::FindCoursesInvalidOnDelete(std::string coursetodelete)
    {
        std::vector<std::string> removeList;

        Course c;
        FindCourse(coursetodelete, c);
        if(c.courseId.size() >0){
            removeList.push_back(c.courseId);
        }

        
        
    }

    
    void BinarySearchTree::findCoursesInvalidRecursively(std::vector<std::string> *courseIDs, Course course)
    {
    }

    bool BinarySearchTree::ValidateCourses()
    {
        std::vector<std::tuple<std::string, std::string>> list;
        auto listptr = &list;
        Node *n = this->root.get();
        this->getListOfCourseNames(listptr, n);

        bool isGood = true;
        bool *isGoodptr = &isGood;
        for (size_t i = 0; i < list.size(); i++)
        {
            auto [x, y] = list.at(i);
            Course c;
            c.courseId = x;
            c.courseName = y;
            isGood = validateNameDesciption(c);
        }
        this->checkPrereqsRecursively(this->root.get(), isGoodptr, listptr);
        return isGood;
    }

    void BinarySearchTree::getListOfCourseNames(std::vector<std::tuple<std::string, std::string>> *list, Node *node)
    {
        if (node->GetLeft() != nullptr)
        {
            this->getListOfCourseNames(list, node->GetLeft());
        }
        std::tuple<std::string, std::string> myTuple(node->ReturnCourse()->courseId, node->ReturnCourse()->courseName);
        list->push_back(myTuple);

        if (node->GetRight() != nullptr)
        {
            this->getListOfCourseNames(list, node->GetRight());
        }
    }

    void BinarySearchTree::checkPrereqsRecursively(Node *node, bool *b, std::vector<std::tuple<std::string, std::string>> *list)
    {
        if (node->GetLeft() != nullptr)
        {
            this->checkPrereqsRecursively(node->GetLeft(), b, list);
        }
        std::vector<std::string> prereqs = node->ReturnCourse()->prereqs;

        if (!prereqs.empty())
        {
            bool c = this->checkPrereqsOneCourse(*node->ReturnCourse(), *list);
            *b = c;
        }
        if (node->GetRight() != nullptr)
        {
            this->checkPrereqsRecursively(node->GetRight(), b, list);
        }
    }

    void BinarySearchTree::PrintOneCourse(std::string id)
    {
        Course course;
        this->FindCourse(id, course);
        if (!course.courseId.empty() && !course.courseName.empty())
        {
            this->printCourse(course);
        }
        else
        {
            std::cout << "Course not found." << std::endl;
        }
    }
    bool BinarySearchTree::RemoveCoursewithId(std::string id)
    {
        Node *parent = nullptr;
        Node *nodeToDelete = this->root.get();
        if (nodeToDelete == nullptr)
        {
            return false;
        }

        // Special root node removal case
        if (compareNoCase(nodeToDelete->ReturnCourse()->courseId, id) == 0)
        {
            if (nodeToDelete->GetLeft() == nullptr && nodeToDelete->GetRight() == nullptr)
            {
                this->root.reset();
                this->size--;
                return true;
            }
            else if (nodeToDelete->GetLeft() == nullptr)
            {
                this->root = this->root->MoveRight();
                this->size--;
                return true;
            }
            else if (nodeToDelete->GetRight() == nullptr)
            {
                this->root = this->root->MoveLeft();
                this->size--;
                return true;
            }
            else
            {
                Node *parent = nullptr;
                Node *successor = this->root->GetRight();
                while (successor->GetLeft() != nullptr)
                {
                    parent = successor;
                    successor = successor->GetLeft();
                }
                *this->root->ReturnCourse() = *successor->ReturnCourse();
                if (parent == nullptr)
                {
                    this->root->ResetRight();
                }
                else
                {
                    parent->ResetLeft();
                }
                this->size--;
                return true;
            }
        }

        // Non-root node removal
        parent = nullptr;
        nodeToDelete = this->root.get();
        while (nodeToDelete)
        {
            if (compareNoCase(nodeToDelete->ReturnCourse()->courseId, id) > 0)
            {
                parent = nodeToDelete;
                nodeToDelete = nodeToDelete->GetLeft();
            }
            else if (compareNoCase(nodeToDelete->ReturnCourse()->courseId, id) < 0)
            {
                parent = nodeToDelete;
                nodeToDelete = nodeToDelete->GetRight();
            }
            else
            {
                break;
            }
        }
        if (!nodeToDelete)
            return false; // Node not found

        // Determine if nodeToDelete is parent's left or right child
        bool isLeftChild = parent && compareNoCase(nodeToDelete->ReturnCourse()->courseId, parent->ReturnCourse()->courseId) < 0;

        // Case 1: No children
        if (nodeToDelete->GetLeft() == nullptr && nodeToDelete->GetRight() == nullptr)
        {
            if (isLeftChild)
            {
                parent->ResetLeft();
            }
            else if (parent)
            { // parent != nullptr
                parent->ResetRight();
            }
            this->size--;
            return true;
        }
        // Case 2: No left child
        else if (nodeToDelete->GetLeft() == nullptr)
        {
            if (isLeftChild)
            {
                parent->SetLeft(std::move(nodeToDelete->MoveRight()));
            }
            else if (parent)
            {
                parent->SetRight(std::move(nodeToDelete->MoveRight()));
            }
            this->size--;
            return true;
        }
        // Case 3: No right child
        else if (nodeToDelete->GetRight() == nullptr)
        {
            if (isLeftChild)
            {
                parent->SetLeft(std::move(nodeToDelete->MoveLeft()));
            }
            else if (parent)
            {
                parent->SetRight(std::move(nodeToDelete->MoveLeft()));
            }
            this->size--;
            return true;
        }
        // Case 4: Two children
        else
        {
            Node *succParent = nullptr;
            Node *successor = nodeToDelete->GetRight();
            while (successor->GetLeft() != nullptr)
            {
                succParent = successor;
                successor = successor->GetLeft();
            }
            *nodeToDelete->ReturnCourse() = *successor->ReturnCourse();
            if (succParent == nullptr)
            {
                nodeToDelete->ResetRight();
            }
            else
            {
                succParent->ResetLeft();
            }
            this->size--;
            return true;
        }
    }

    bool BinarySearchTree::ValidateSingleCourse(Course course)
    {
        bool b = validateNameDesciption(course);
        return b;

    }

    void BinarySearchTree::Clear()
    {
        if(this->root == nullptr){
            return;
        }
        if(this->root->GetLeft() != nullptr){
            this->recursiveClear(this->root->GetLeft());
        }
        if(this->root->GetRight() != nullptr){
            this->recursiveClear(this->root->GetRight());
        }
        this->root->ResetLeft();
        this->root->ResetRight();
        this->root = nullptr;
        this->size--;
    }

    bool BinarySearchTree::validateNameDesciption(Course course)
    {
        if(course.courseId.length() != 7){
            return false;
        }
        if(course.courseName.length() < 3 || course.courseName.length() > 40){
            return false;
        }
        if(course.prereqs.size() < 1){
            return true;
        }
        std::vector<std::tuple<std::string, std::string>> names;
        getListOfCourseNames(&names, this->root.get());

        bool isgood = checkPrereqsOneCourse(course, names);
        return isgood;
    }

    bool BinarySearchTree::checkPrereqsOneCourse(BST::Course &course, std::vector<std::tuple<std::string, std::string>> &names)
    {
        for (size_t j = 0; j < course.prereqs.size(); j++)
        {
            bool b = false;
            for (size_t i = 0; i < names.size(); i++)
            {
                auto [x, y] = names.at(i);
                if (compareNoCase(course.prereqs.at(j), x) == 0)
                {
                    b = true;
                }
            }
            if (!b)
            {
                return false;
            }
        }
        return true;
    }

    bool BinarySearchTree::isCourseIdUnique(Course course)
    {
        std::vector<std::tuple<std::string, std::string>> list;

        if(this->root == nullptr){
            return true;
        }
        getListOfCourseNames(&list, this->root.get());

        if(list.size() < 1){
            return true;
        }
        bool isUnique = true;
        for (size_t i = 0; i < list.size(); i++)
        {
            auto[x,y] = list.at(i);
            if(compareNoCase(x, course.courseId) == 0){
                isUnique = false;
                break;
            }
        }
        return isUnique;
    }


    void BinarySearchTree::recursiveClear(Node* node)
    {
        if(node->GetLeft() != nullptr){
            this->recursiveClear(node->GetLeft());
        }
        if(node->GetRight() != nullptr){
            this->recursiveClear(node->GetRight());
        }
        node->ResetLeft();
        node->ResetRight();
        node = nullptr;
        this->size--;
    }

} // namespace BST