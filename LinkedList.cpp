//============================================================================
// Name        : LinkedList.cpp
// Author      : Lamine Coulibaly
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold course information
struct Course {
    string courseId;
    string title;
    vector<string> prerequisites;
};

// Internal node structure for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// Class to manage the Binary Search Tree
class CourseTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (node->course.courseId > course.courseId) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    void inOrderPrint(Node* node) {
        if (node != nullptr) {
            inOrderPrint(node->left);
            cout << node->course.courseId << ": " << node->course.title << endl;
            inOrderPrint(node->right);
        }
    }

    Node* searchNode(Node* node, string courseId) {
        if (node == nullptr || node->course.courseId == courseId) {
            return node;
        }
        if (courseId < node->course.courseId) {
            return searchNode(node->left, courseId);
        } else {
            return searchNode(node->right, courseId);
        }
    }

    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    CourseTree() {
        root = nullptr;
    }

    virtual ~CourseTree() {
        deleteTree(root);
    }

    bool isEmpty() {
        return root == nullptr;
    }

    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    void AlphanumericPrint() {
        if (root == nullptr) {
            cout << "No courses loaded." << endl;
            return;
        }
        inOrderPrint(root);
    }

    Course Search(string courseId) {
        Node* node = searchNode(root, courseId);
        if (node != nullptr) {
            return node->course;
        }
        Course emptyCourse;
        emptyCourse.courseId = "";
        return emptyCourse;
    }
};

// Function to load courses from a CSV file
void LoadCourses(string filePath, CourseTree* tree) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error opening file. Please check the file name and try again." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> data;

        while (getline(ss, token, ',')) {
            data.push_back(token);
        }

        if (data.size() < 2) {
            continue; // Skip improperly formatted lines
        }

        Course course;
        course.courseId = data[0];
        course.title = data[1];

        for (size_t i = 2; i < data.size(); ++i) {
            course.prerequisites.push_back(data[i]);
        }

        tree->Insert(course);
    }
    file.close();
    cout << "Course data loaded successfully." << endl;
}

// Main execution block
int main() {
    CourseTree* courseTree = new CourseTree();
    int choice = 0;
    string fileName = "";
    string searchId = "";

    cout << "Welcome to the ABCU Advising Assistance Program." << endl;

    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Alphanumeric Course List" << endl;
        cout << "3. Print Course Information" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter the name of the course data file (e.g., ABCU_Advising_Input.csv): ";
                cin >> fileName;
                LoadCourses(fileName, courseTree);
                break;

            case 2:
                cout << "\nHere is a sorted list of all courses:\n" << endl;
                courseTree->AlphanumericPrint();
                break;

            case 3:
                cout << "What course do you want to know about? (e.g., CS300): ";
                cin >> searchId;
                // Convert input to uppercase for standard matching
                for (auto &c: searchId) c = toupper(c); 

                {
                    Course course = courseTree->Search(searchId);
                    if (course.courseId != "") {
                        cout << "\nCourse: " << course.courseId << " | Title: " << course.title << endl;
                        cout << "Prerequisites: ";
                        if (course.prerequisites.empty()) {
                            cout << "None" << endl;
                        } else {
                            for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                                cout << course.prerequisites[i] << " ";
                            }
                            cout << endl;
                        }
                    } else {
                        cout << "Error: Course " << searchId << " not found." << endl;
                    }
                }
                break;

            case 9:
                cout << "Thank you for using the ABCU Advising Assistance Program." << endl;
                break;

            default:
                cout << "Error: Invalid option. Please enter 1, 2, 3, or 9." << endl;
                break;
        }
    }

    delete courseTree; // Clean up memory
    return 0;
}

