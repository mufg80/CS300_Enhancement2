//============================================================================
// Name        : Reader.hpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Header file containing file reading declarations for ABCU app
//============================================================================
#ifndef _READER_HPP_
#define _READER_HPP_
#include <string>
#include <vector>
// Type definition for vector of vectors.
typedef std::vector<std::vector<std::string>> vectorOfVectors;
// Structure for courses
struct Course{
    std::string courseId;
    std::string courseName;
    std::vector<std::string> prereqs;
};

// Function Declarations

// Gets file path string from user.
std::string getFilePath();

// Reads filepath and creates a vector of vectors.
bool readCourseFile(std::string filepath, vectorOfVectors& dataStructure);

// Checks data structure for validation rules.
bool examineReadFile(vectorOfVectors& BuildDataStructure);

// Menu output.
void outputMenuItems();

// Builds vector of courses from vector of vectors.
void BuildDataStructure(std::vector<Course>& courses, vectorOfVectors twoD);

// Quick sort algorithm for sorting alphanumerically
void quickSort(std::vector<Course>& courses, int begin, int end);
int partition(std::vector<Course>& courses, int begin, int end);

// Function that instantiates prereqsorter class for sorting by prereqs
std::vector<Course> sortByReqs(std::vector<Course>& courses);

// Function that handles printing entire list to output.
void printList(std::vector<Course>& courses);

// Binary search algorithm for finding item in sorted list.
int binarySearch(std::vector<Course>& courses, std::string key);

// Function that gets courses id from user.
std::string getCourseRequest();
// Function that prints specified course to user.
void printCourse(std::vector<Course>& courses, int index);

// Case insensitive compare function
int compareNoCase(std::string first, std::string second);

// Outputs a couple newlines and a dashed line for console.
void printSeparation();



// Prereq sorter: This class is used to sort courses by prereqs.

class PrereqSorter{
    private:
    // List size used to hold size of list passed in.
    int listSize;
    // Two vectors for one for holding extras and one for ordered list.
    std::vector<Course> holding;
    std::vector<Course> ordered;
    // Private method actually doing the ordering.
    void orderList();
    // Private method running a check of a course at a specific index in holding vector.
    bool allPrereqsInOrderedList(int index);
    public:
    // Constructure taking a list to be sorted.
    PrereqSorter(std::vector<Course>& unsorted);
    // Lets caller know if list was successfully sorted.
    bool listOrdered();
    // Method to get the ordered list, returns empty list if failed.
    std::vector<Course> getOrderedList();
};


#endif /*!_READER_HPP_*/