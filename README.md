# CS300_Enhancement2CS 340 README

About the Project/Project Title
ABCU Course App is a C++ application designed to manage a Binary Search Tree (BST) of university courses. It allows users to load course data from a file, print all courses in sorted order, display details of a specific course by ID, and validate course prerequisites, in effect, handling all operations for maintaining a list of courses.

Motivation
Managing course data, including prerequisites, is critical for academic advisors and students at ABCU. Manually verifying course details and prerequisites is time-consuming and error-prone. This application automates the process by organizing courses in a BST, enabling fast retrieval and validation of course information, thus saving time and ensuring data integrity.

Getting Started
To get started, follow the steps below:
Ensure a C++ compiler (e.g., g++) is installed (see Installation).

Place the course data file (e.g., CourseList.txt) in the same directory as the executable.

Compile the provided C++ files (ABCUApp.cpp, BST.cpp, ABCUApp.hpp, BST.hpp) using a C++ compiler.

Run the compiled executable to launch the ABCU Course App.

Use the interactive menu to:
Load up to 100 courses from a file into the BST (Option 1).

Print all courses in alphabetical order by course ID (Option 2).

Print details of a specific course by entering its ID (Option 3).

Exit the program (Option 4).

Installation
To set up the environment:
Install a C++ compiler (e.g., g++) by following the instructions at GCC Installation.

Ensure the C++ Standard Library is available, as the project uses features like string, vector, etc.

Download or clone the project files (ABCUApp.cpp, ABCUApp.hpp, BST.cpp, BST.hpp) to a local directory.

Compile the project using a command like:
bash

g++ ABCUApp.cpp BST.cpp -o ABCUCourseApp

Ensure the course data file (e.g., CourseList.txt) is in the same directory as the executable. The file should be a comma-separated text file with each line containing a course ID, course name, and optional prerequisite IDs.

Usage
The ABCU Course App supports three primary use cases:
Load Courses: Reads up to 100 courses from a specified text file into a BST, validating course IDs, names, and prerequisites. IMPORTANT - all prerequsites must exist as a courseID in first column or it is invalid and file is not read.

Print All Courses: Displays all courses in the BST in alphabetical order by course ID, including course names (descriptions).

Print Single Course: Retrieves and displays details of a specific course, including its ID, name, and prerequisites, based on a user-provided course ID.

Code Example
Example A: Load Courses from File
The BuildStructureFromFile function prompts the user for a file name (without extension), appends .txt, and loads course data into the BST. 

![alt text](Images/case1.png)

This reads from a file (e.g., CourseList.txt), parses each line into a Course object, and inserts it into the BST if the course ID is unique and prerequisites are valid and name and id are within appropriate lengths.

Example B: Print All Courses
The PrintCoursesInOrder function performs an in-order traversal of the BST to display all courses. 

![alt text](Images/case2.png)

Also notes number of courses in memory.

![alt text](Images/case2a.png)

**Example C: Print a Single Course**  
The `PrintOneCourse` function prompts the user for a course ID and displays its details.

![alt text](Images/case3.png)