//============================================================================
// Name        : ABCUApp.cpp
// Author      : Shannon Musgrave
// Version     : 1.0
// Copyright   : 
// Description : Course app for ABCU
//============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include <string> 
#include <vector>
#include "ABCUApp.hpp"
#include <iomanip>
#include <limits>
#include <algorithm>

// Main menu, access point of program. 
int main(int argc, char* argv[]) {

    // Use this boolean to change between prereq sorter and alphanumerical sorting.
    // << Extra Credit Attempt >>
    bool usePrereqSorter = false;
    // Greeting
    std::cout << "         Welcome to ABCU Course App         " << std::endl;
    
    // Variable declarations
    std::string filePath;
    vectorOfVectors structure;
    std::vector<Course> alphNumSortedCourses;
    std::vector<Course> reqSortedCourses;
    int input;
    bool isRead = false;

    // Endless loop to cycle program until terminated.
    while(true){
        // Show menu to user and get user input.
        outputMenuItems();
        std::cin >> input;

        // Check input to make sure user entered integer. If not throw it out 
        // and assign as default a 5, which will trigger the default case in switch statement.
        if(!std::cin.good()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = 5;
        }

        // Start switch case.
        switch(input){
            case 1:

                // Clear structures so that if case 1 is ran multiple times, it will not grow.
                structure.clear();
                alphNumSortedCourses.clear();
                // Menu case to load file, as for file path.
                filePath = getFilePath();

                // Load file request, try to read file at users request.
                isRead = readCourseFile(filePath, structure);

                // Build Vector of courses if there were no problems with data.
                if(isRead){
                    BuildDataStructure(alphNumSortedCourses, structure);
                }
                
            break;
            case 2:
            {
                // Print list of courses.
                // Boolean to switch between prereq sorter and alphanumerical sort
                // is at first line in main method.
                if(usePrereqSorter){

                    // <<<Extra credit attempt>>>
                    // Sort list by prereqs, if not possible empty list.
                    reqSortedCourses = sortByReqs(alphNumSortedCourses);
                    printList(reqSortedCourses);
                }else{

                    // Menu case for printing sorted list in alphanumerical order.
                    // Sort vector of courses using recursive quick sort function.
                    quickSort(alphNumSortedCourses, 0,alphNumSortedCourses.size() -1);
                    printList(alphNumSortedCourses);
                }
            
            }
            break;
            case 3:
            {
                // Get specified course from user.
                std::string userInput = getCourseRequest();

                // When in prereq sorting mode, must call here so that binary search will
                // still work.
                if(usePrereqSorter){
                    quickSort(alphNumSortedCourses, 0,alphNumSortedCourses.size() -1);
                }
                 // User Binary Search algorithm to efficiently find index of desired course.
                int index = binarySearch(alphNumSortedCourses, userInput);

                // Use function to print course at the specified index.
                printCourse(alphNumSortedCourses, index);
            }
            break;
            case 4:

                // Menu Item for terminating program, say good bye and IF condition below will terminate loop.
                std::cout << "            Good bye!" << std::endl;
                break;
            break;
            default:
                // Default case, User specified non existing menu item. Let them know.
                std::cout << "            This is not an appropriate entry. Please try again." << std::endl;
        }
        if(input == 4){
            break;
        }

    }
    

    return 0;
}
// A function to get file name from user.
std::string getFilePath(){
    
    std::cout << "            Enter the file name for the courses list." << std::endl;
    std::string input;
    std::cin >> input;
    std::cout << std::endl;
    return input;
}

// Function that prints specified course requested by user. Takes a list of courses and the index of the specified course.
void printCourse(std::vector<Course>& courses, int index){
   
    // Check that a proper index was passed to function, -1 means item was not found. Let user know and return.
    if(index == -1 || index >= courses.size()){
        std::cout << "            Hmm, I cannot find this course, please try again." << std::endl;
    }else{
        // Course was found print it out.
        std::cout << "            Course requested." << std::endl; 
        printSeparation();
        std::cout << "ID: " << courses.at(index).courseId << "  |  ";
        std::cout << "Name: " << courses.at(index).courseName << std::endl;
       

        // Check if this course has prereqs. Notify them of none if thats the case.
        if(courses.at(index).prereqs.size() > 0){
            std::cout << "Prerequisites:";
            // Prereqs exist use binary search to find each key and print title.
            // boolean first, used for formatting
            bool first = true;
            for(std::string id : courses.at(index).prereqs){
                int prereqIndex = binarySearch(courses, id);

                // Doublecheck it exists and print it out.
                if(prereqIndex != -1){
                    if(first){
                       
                        std::cout << "  ";
                        first = false;
                       
                    }else{
                        std::cout << "                ";
                    }
                    std::cout << courses.at(prereqIndex).courseId << "  |  ";
                    std::cout << courses.at(prereqIndex).courseName << std::endl;
                }
            }
        }else{
            std::cout << "Prerequisites:  None" << std::endl;
        }
        std::cout << std::endl;
    }
}

// Function to request the name of the course the user is interested in.
std::string getCourseRequest(){
    std::cout << "            Which course do you want to know about?  " << std::endl;
    std::string input;
    std::cin >> input;
    return input;
}

// Binary search algorithm for use on sorted lists. Takes the list, and the key to search for. List
// must be sorted before calling this function.
int binarySearch(std::vector<Course>& courses, std::string key){
    int mid = 0;
    int low = 0;
    int high = courses.size() - 1;

    // Run loop until item is found, if not found, return -1. 
    while(high >= low){
        mid = (high + low) / 2;
        
        // Use case insensitive comparison function so that user is not burdened by needing proper
        // case spelling.
        if(compareNoCase(courses.at(mid).courseId, key) < 0){
            low = mid + 1;
        }else if(compareNoCase(courses.at(mid).courseId, key) > 0){
            high = mid - 1;
        }else{
            // Course found, return index to caller.
            return mid;
        }
    }
    return -1;
}

// Prints the ordered list, list must be sorted before calling this function.
void printList(std::vector<Course>& courses){
    std::cout << std::endl;
    std::cout << "            List requested." << std::endl;
    printSeparation();
    // Foreach loop through courses vector.
    if(courses.size() > 0){
        for(Course c : courses){
            // Output id and name.
            std::cout << c.courseId << "   |   ";
            std::cout << c.courseName << std::endl;
        }
    }else{
        std::cout << std::endl;
        std::cout << "            No courses to print, list empty." << std::endl;
        std::cout << std::endl;
    }
   
}


// Extra credit attempt, sorting by prereqs, if not possible to take class in order 
// of prereqs, print empty list.
std::vector<Course> sortByReqs(std::vector<Course>& courses){
    // Instantiate prereqsorter class to handle sorting, pass courses vector into constructor.
    PrereqSorter sorter(courses);
    // Check boolean to see if list was able to be sorted by prereqs.
    if(sorter.listOrdered()){
        return sorter.getOrderedList();
    }else{
        std::vector<Course> empty;
        return empty;
    }
}

// Quicksort algorithm for sorting the list of courses. Takes list, starting point, and ending point. Recursive function.
void quickSort(std::vector<Course>& courses, int begin, int end){
    int mid = 0;
    if(begin >= end){
        return;
    }

    // Call partition in each resursive iteration, sorting done in partition function.
    mid = partition(courses, begin, end);

    // With list divided by mid variable, recursively call quickSOrt with each half.
    quickSort(courses, begin, mid);
    quickSort(courses, mid+1, end);
}

// Partitioning function is helper function called by quicksort. Takes list, start and end points. 
int partition(std::vector<Course>& courses, int begin, int end){
    // Initialize some variables.
    int low = begin;
    int high = end;
    int middle = low + ((high - low)/2);
    Course pivot = courses.at(middle);
    bool done = false;

    // Loop until boolean is finished.
    while(!done){

        // Increment until finding a course id less than pivot courses id.
        while(courses.at(low).courseId.compare(pivot.courseId) < 0){
            low +=1;
        }

        // Decrement until finding a course id greater than pivot courses id.
        while(pivot.courseId.compare(courses.at(high).courseId) < 0){
            high -=1;
        }

        // Check if finished.Finished if index positions have met or passed one another.
        if(low >= high){
            done = true;
        }else{
            // If not finished, swap the found courses and adjust index variables.
            std::swap(courses.at(low), courses.at(high));
            low +=1;
            high -= 1;
        }
    }
    return high;
}

// Function bulding list of courses using a vector of vectors. This function expects the vectors to
// be in proper form. Checks must be made on vectors before calling this function.
void BuildDataStructure(std::vector<Course>& courses, vectorOfVectors twoD){
    
    // For each loop for used on vector of vectors, line variable is a vector of string.
    for(std::vector<std::string> line : twoD){
        // Create course object to build and push onto courses list.
        Course course;
        course.courseId = line.at(0);
        course.courseName = line.at(1);
        int j;

        // Get the prereqs by starting a for loop at index position two.
        for(j = 2; j < line.size(); j++){
            // Avoid if empty string making it into list.
            if(!line.at(j).empty()){
                course.prereqs.push_back(line.at(j));
            }
        }
        // Course should be fully built, push it onto the list of courses.
        courses.push_back(course);
    }
    
}

// Function for outputting menu to user.
void outputMenuItems(){
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "                    Menu Options                 " << std::endl;
    std::cout << "               1) Load Data Structure            " << std::endl;
    std::cout << "               2) Print Course List              " << std::endl;
    std::cout << "               3) Print Course                   " << std::endl;
    std::cout << "               4) Exit                           " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
}

// Read courses function takes the path to file and a vector of vectors. This function reads course list from
// hard drive and builds a vector of vectors. Calls helper function for checks on data.
bool readCourseFile(std::string filepath, vectorOfVectors& dataStructure){

    // Try catch to catch failures on stream and let user know of issue.
    try{
    std::ifstream readfile(filepath);
    std::string line;

    // Check fail and report failure and return.
    if(readfile.fail()){
        std::cout << std::endl;
        std::cout << "            Failure to open a file of this name, please" << std::endl;
        std::cout << "            make sure the file exists in programs directory." << std::endl;
        std::cout << std::endl;
        return false;
    }

    // Nested while loops, first reads lines delimited by new lines, second is delimited on commas.
    // Getline called on stream to take a line until newline character.
    while(getline(readfile, line)){

        // Take string taken from stream and use stringstream so that getline can be used again on commas.
        std::istringstream linestream(line);

        // Create vector of strings to get pieces of line delimited by commas.
        std::vector<std::string> listOfPieces;
        std::string piece;

        // Getline uses third argument of comma to get pieces of line then adds to vector.
        while(getline(linestream, piece,',')){
            listOfPieces.push_back(piece);
        }
        // Reached end of particular line, add vector of string to vector.
        dataStructure.push_back(listOfPieces);
    }
    }
    catch(std::ifstream::failure e){
        // Report errors to user.
        std::cerr << "            Error opening/reading file." << std::endl;
        return false;
    }

    // Examine vectors for errors.
    if(examineReadFile(dataStructure)){
        // Vector of vectors is good, let user know file is loadedd into memory.
        std::cout << std::endl;
        std::cout << "            File loaded successfully." << std::endl;
        std::cout << std::endl;
        return true;
       
    }else{
        // Function reported error with file format, let user know file failed to load.
        std::cout << std::endl;
        std::cout << "            File not loaded." << std::endl;
        std::cout << std::endl;
       return false;
    }
}

// Function to examine the vector of vectors validating correctness. Validation checked:
// 1: Must have at least one line.
// 2: First two columns must have data in each row.
// 3: All prereqs must exist in column 0 as course ID in another row.
bool examineReadFile(vectorOfVectors& dataStructure){

    // Vector of strings created to hold all course ids in column zero of dataStructure.
    std::vector<std::string> courseNames;

    // Make sure we don't have an empty vector.
    if(dataStructure.size() == 0){
        std::cout << std::endl;
        std::cout << "            Error: No lines were read from file." << std::endl;
        std::cout << std::endl;
        return false;
    }

    // Start checks.
    for(int i = 0; i < dataStructure.size(); i++){
        // Check if line had at least two items.
        if(dataStructure.at(i).size() < 2){
            std::cout << std::endl;
            std::cout << "            Error: Line missing some fields, less than 2 existing." << std::endl;
            std::cout << std::endl;
            return false;
        }
        // Be sure that first two columns both have information.
        if((dataStructure.at(i).at(0).empty() || dataStructure.at(i).at(1).empty())){
            // Course information is invalid should have two fields minimum.
            std::cout << std::endl;
            std::cout << "            Error: A line from the file did not have both a name and Id." << std::endl;
            std::cout << std::endl;
            return false;
        }else{
            // If a line has id and name, load id into a list of string for prereqs later.
            if(!dataStructure.at(i).at(0).empty()){
                // Copy index 0 of each row to compare prereqs against.
                courseNames.push_back(dataStructure.at(i).at(0));
            }
        }
    }
    // Vector of vectors looks good, double check prereqs.
    for(std::vector<std::string> list : dataStructure){
        // Skip lists with no prereqs.
        if(list.size() > 2){

            // Start at index 2, after id and name, check each prereq.
            for(int i = 2; i < list.size(); i++){
                // Set found bit to false.
                bool found = false;
                // Check against all ids in list.
                for(std::string name : courseNames){
                    if(name.compare(list.at(i)) == 0){
                        // Prereq found, mark found.
                        found = true;
                    }
                }
                // Prereq not found in ids, let user know and return false.
                if(!found){
                    std::cout << std::endl;
                    std::cout << "            Error: Prereq " << list.at(i) << " not found as course name in file." << std::endl;
                    std::cout << std::endl;
                    return false;
                }
            }
        }

    }
    return true;
}

// Function compares strings after converting them to lowercase, for case insensitive comparisons.
int compareNoCase(std::string first, std::string second){
    // Use transform function by passing iterators and function to use: ::tolower. Then return
    // the comparison.
    transform(first.begin(), first.end(),first.begin(), ::tolower);
    transform(second.begin(), second.end(),second.begin(), ::tolower);
    return first.compare(second);
}

// Utility function for printing repetitive line separators.
void printSeparation(){
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
}




// PrereqSorter class used to sort courses according to their prereq list. This class uses two lists
// to hold data while it sorts them. It must be instantiated with a full course and has no option for adding
// or inserting more courses. 


// Private method that sorts holding vector into ordered vector.
void PrereqSorter::orderList(){
  // If course has prereqs bump up
    int i = holding.size();
    int j = 0;

    // Worst case scenario is only moving one course over per iteration, nested
    // loops will iterate enough for this scenario.
    while(j < i){
        // List size is size of passed in list to be sorted. If ordered is at this
        // size algorithm is finished.
        if(ordered.size() == listSize){
            break;
        }
        // boolean checks if one was moved during an iteration.
        bool movedOne = false;
        int k = 0;
        while(k < i){
            // Safety check to make sure we dont get out of range.
            if(k >= holding.size()){
                k++;
                continue;
            }
            // bools created for troubleshooting purposes, after copy, courseID is
            // set to string.empty so its not copied again.
            bool courseNotEmpty = !holding.at(k).courseId.empty();
            // Function call checks if prereqs are already in ordered list.
            bool prereqsInOrdered = allPrereqsInOrderedList(k);
            // If all true, copy to ordered, set id to string.empty and set 
            // flag movedOne.
            if(courseNotEmpty && prereqsInOrdered){
                ordered.push_back(holding.at(k));
                holding.at(k).courseId = "";
                movedOne = true;
            }
            k++;
        }
        // If internal loop didn't move one we are done and cannot move another, stop looping.
        if(!movedOne){
            break;
        }
        j++;
    }
}

// Mehod that checks ordered list prereqs for new entry. returns true if a courses
// Prereqs all exist in ordered list, if true, this is candidate for copy.
bool PrereqSorter::allPrereqsInOrderedList(int index){
    // Get pointer to course to check.
    Course* c = &(holding.at(index));
    // Foreach loop to check the prereqs list
    for(std::string s : c->prereqs){
        // Set flag to false.
        bool isFound = false;
        // Cycle through ordered checking if this prereq exists.
        for(Course c : ordered){
            // If found set flag.
            if(c.courseId.compare(s) == 0){
                isFound = true;
            }
        }
        // Check each prereq, if not found return false.
        if(isFound == false){
            return false;
        }
    }
    // If all prereqs where found, return true.
    return true;
}
// Only constructor, must have referance to list of courses.
PrereqSorter::PrereqSorter(std::vector<Course>& unsorted){
    // Cannot instantiate prereqsorter to empty list, throw exception.
    if(unsorted.size() == 0){
       std::cout << "            Cannot instantiate Prereqsorter with empty vector." << std::endl;
    }
    // Get size of the list for checks later.
    this->listSize = unsorted.size();
    // Move items to ordered list or holding list, ordered gets all courses with no prereqs.
    // Holding gets all others.
    for(Course course: unsorted){
        if(course.prereqs.size() == 0){
            this->ordered.push_back(course);
        }else{
            this->holding.push_back(course);
        }
    }
    // Call orderList to finish sorting.
    orderList();
}
// If ordered size is equal to list size then all were copied and list is ordered.
bool PrereqSorter::listOrdered(){
    if(this->ordered.size() == listSize){
        return true;
    }else return false;
}
// Return final result, empty vector if unordered or ordered vector if list was able to be sorted.
std::vector<Course> PrereqSorter::getOrderedList(){
    if(this->listOrdered()){
        return ordered;
    }else{
        std::vector<Course> empty;
        return empty;
  }
}