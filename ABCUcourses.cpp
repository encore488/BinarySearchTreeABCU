//============================================================================
// By: Christopher Williams
// This is the main file. Contains a binary search tree for sorting and storing Course structures 
// I wrote everything in this .cpp file, but nothing in the CSVparser files
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include "CSVparser.hpp"
#include <vector>

using namespace std;



// /The basic structure to hold course information
struct Course {
    string number; // unique identifier
    string name;
    string prereq1;
    string prereq2;
};

// A Course must be placed in a node before it can be used in a tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }
};

class BinarySearchTree {

private:
    void addNode(Node*, Course);
    void recursDelete(Node*);
    void printANum(Node*);
    void showCInfo(Course);

public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    Course Search(string, Node*);
    void Insert(Course);
    void printCourseInformation(string);
    void listAlphaNum();
    void Remove(string);
};

// Default Constructor for BST
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    recursDelete(root);
}

void BinarySearchTree::recursDelete(Node* curr) {
    //Find left and right pointers of current node
    Node* rt = curr->right;
    Node* lft = curr->left;

    if (curr != nullptr) {
        //start with bottom left node, then siblings, then parent, in order
        recursDelete(lft);

        delete curr;

        recursDelete(rt);
    }
}

// Prints list in alphanumeric order
void BinarySearchTree::listAlphaNum() {
    printANum(root);
}

//Recursive function to print all nodes in order, alphanumerically
void BinarySearchTree::printANum(Node* thisNode) {
    if (thisNode != nullptr) {
        //Recursively iterate through nodes, starting with the left end
        printANum(thisNode->left);
        printANum(thisNode->right);

        showCInfo(thisNode->course);
    }
}
//Insert new course using recursive function "addNode". We must start at root incase root is null
void BinarySearchTree::Insert(Course newCourse) {
    // If the root node is empty, then assign newNode's values to it
    if (root == nullptr) {
        root = new Node();
        root->course.name = newCourse.name;
        root->course.number = newCourse.number;
        root->course.prereq1 = newCourse.prereq1;
        root->course.prereq2 = newCourse.prereq2;
    }
    else {
        addNode(root, newCourse);
    }
}

// Order Courses alphanumerically as they are inserted into a BST
void BinarySearchTree::addNode(Node* node, Course newCourse) {
    int comparison = node->course.number.compare(newCourse.number);
    //If new course is less than current node, look to the left child
    if (comparison > 0) {
        if (node->left == nullptr) {
            node->left = new Node();
            node->left->course.name = newCourse.name;
            node->left->course.number = newCourse.number;
            node->left->course.prereq1 = newCourse.prereq1;
            node->left->course.prereq2 = newCourse.prereq2;
        }
        else {
            this->addNode(node->left, newCourse);
        }
    }
    //If new course is greater than current node, look to the right child
    else {
        //If this node is empty, new bid goes here
        if (node->right == nullptr) {
            node->right = new Node();
            node->right->course.name = newCourse.name;
            node->right->course.number = newCourse.number;
            node->right->course.prereq1 = newCourse.prereq1;
            node->right->course.prereq2 = newCourse.prereq2;
        }
        else {
            //Recursively look at right child node
            this->addNode(node->right, newCourse);
        }
    }
}

//Return the course with this number
Course BinarySearchTree::Search(string courseNum, Node* curr) {

    if (curr != nullptr) {
        //Make this comparison AFTER checking for nullptr to avoid errors
        int comparison = curr->course.number.compare(courseNum);

        if (comparison == 0) {
            //If current node is the key, return it
            return curr->course;
        }
        else if (comparison > 0) {
            //If courseNum is less than current node number, recursively call for left child node
            return BinarySearchTree::Search(courseNum, curr->left);
        }
        else {
            //If courseNum is greater than this nodes number, recursively call right child node
            return BinarySearchTree::Search(courseNum, curr->right);
        }
    }
    else {
        //If this node is null, return empty course
        Course course;
        return course;
    }
}

//Search for and print all info for course with this number
void BinarySearchTree::printCourseInformation(string courseNum) {
    Course myCourse;

    try {
        myCourse = Search(courseNum, root);
        if (myCourse.number.empty() || myCourse.name.empty()) {
            cout << "Course Not Found." << endl;
        }
        else {
            cout << "Found Course. " << endl;
            showCInfo(myCourse);
        }
    }
    catch (...) {
        cout << "Course Not Found. Error in Search() or printCourseInformation()" << endl;
    }

}


void BinarySearchTree::showCInfo(Course myCourse) {
    try {
        cout << "Course Name: " << myCourse.name << "  Number: " << myCourse.number
            << "  Prerequisites: " << myCourse.prereq1 << ", " << myCourse.prereq2 << endl;
    }
    catch(...) {
        cout << "A course could not be printed" << endl;
    }
}

void BinarySearchTree::Remove(string courseNum) {
    //Par is a pointer to the parent node of curr (the current node)
    Node* par = nullptr;
    Node* curr = root;

    while (curr != nullptr) {
        int comparison = curr->course.number.compare(courseNum);

        //If node is found...
        if (comparison == 0) {
            if (curr->left == nullptr && curr->right == nullptr) {
                //If it is the root, then delete
                if (par == nullptr) {
                    root = nullptr;
                }
                //If it is the left child, delete that
                else if (par->left == curr) {
                    par->left = nullptr;
                }
                else {
                    //else it must be the right
                    par->right = nullptr;
                }
            }
            //If there is only a left child node
            else if (curr->right == nullptr) {
                //If this is the root, assign root to left node
                if (par == nullptr) {
                    root = curr->left;
                }
                //Else, assign the left child to this node
                else if (par->left == curr) {
                    par->left = curr->left;
                }
                else {
                    par->right = curr->left;
                }

            }
            //If there is only a right child node
            else if (curr->left == nullptr) {
                //If this is the root, assign root to right node
                if (par == nullptr) {
                    root = curr->right;
                }
                //Else, assign the right child to this node
                else if (par->right == curr) {
                    par->right = curr->right;
                }
                else {
                    par->left = curr->right;
                }
            }
            //If it has two children
            else {
                //succ points to the successor
                Node* succ = curr->right;
                //loop till you run out of left pointers
                while (succ->left != nullptr) {
                    succ = succ->left;
                }
                //Create copy of successor course
                Course succCourse = succ->course;                
                delete succ;
                curr->course = succCourse;
            }
            //Node has been found and removed, so exit function
            cout << "Course found and removed" << endl;
            return;
        }
        //If the Course we search for is lexigraphically lesser than curr, move to the left child node
        else if (comparison > 0) {
            par = curr;
            curr = curr->left;
        }
        else {
            par = curr;
            curr = curr->right;
        }
    }
    cout << "Course not found" << endl;
}


//Returns true if string is not empty and this is not an existing course
bool notAcourse(string cNumber, vector<Course> csvVect) {
    if (cNumber.empty()) {
        return false;
    }
    for (int i = 0; i < csvVect.size(); i++) {
        if (csvVect[i].number == cNumber) {
            return false;
        }
    }
    return true;
}

//Quality check to make sure there are no missing essential values or prereqs that dont match courses
bool qaCheck(vector<Course> csvVect) {
    for (int i = 0; i < csvVect.size(); i++) {
        if (csvVect[i].number.empty() || csvVect[i].name.empty()) {
            cout << "A name or number is missing from a course." << endl;
            return false;
        }
        else if (notAcourse(csvVect[i].prereq1, csvVect) || notAcourse(csvVect[i].prereq2, csvVect)) {
            cout << "There is a prerequisite that does not match any course in row " << i+1 << endl;
            cout << csvVect[i].name << " rejected." << endl;
            return false;
        }
    }
    return true;
}


//Load csv values into a vector and perform QA
vector<Course> loadCourses(string csvPath, BinarySearchTree* bst) {
    vector<Course> csvVect;

    cout << "Loading CSV file from " << csvPath << endl;

    //Open file and read like it is std::cin
    ifstream inputFile;
    inputFile.open(csvPath);
    string line = "";

    if (!inputFile.is_open()) {
        cout << "Could not open file: " << csvPath << endl;
        return csvVect;
    }
    else {
        cout << "Opened File." << endl;
    }
    //Get first line in file (Header), and discard it
    getline(inputFile, line);
    line = "";
    
    try {
        while (getline(inputFile, line)) {
            stringstream inputString(line);
            string number;
            string name;
            string prereq1;
            string prereq2;

            getline(inputString, number, ',');
            getline(inputString, name, ',');
            getline(inputString, prereq1, ',');
            getline(inputString, prereq2, ',');

            Course course;
            course.name = name;
            course.number = number;
            course.prereq1 = prereq1;
            course.prereq2 = prereq2;
            csvVect.push_back(course);
            line = "";
        }
    }
    catch (csv::Error& e) {
        cout << "There was an error loading the csv file." << endl;
        std::cerr << e.what() << std::endl;
    }

    if (qaCheck(csvVect)) {
        cout << "Quality check passed!" << endl;
    }
    else {
        cout << "Quality check failed!" << endl;
    }
    return csvVect;
}





int main() {
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    string csvPath = "ABCUprograms.csv";
    int menuInput = 0;
    string laterInput;
    vector<Course> myCourses;

    while (menuInput != 9) {
        //Menu
        cout << "Welcome to the Course Planner." << endl;
        cout << "    1.) Load Data Structure" << endl;
        cout << "    2.) Print Ordered Course List" << endl;
        cout << "    3.) Print Single Course" << endl;
        cout << "    4.) Remove Course" << endl;
        cout << "    9.) Exit" << endl;
        cout << "Enter a number: " << endl;
        cin >> menuInput;

        switch (menuInput) {
        case 1:
            myCourses = loadCourses(csvPath, bst);
            for (auto course : myCourses) {
                bst->Insert(course);
            }
            break;
        case 2:
            bst->listAlphaNum();
            break;
        case 3:
            cout << "Enter Course Number: " << endl;
            cin >> laterInput;
            bst->printCourseInformation(laterInput);
            break;
        case 4:
            cout << "Enter Course Number: " << endl;
            cin >> laterInput;
            bst->Remove(laterInput);
            break;
        case 5:
            Course newCourse;
            cout << "Enter Course Number: " << endl;
            cin >> laterInput;
            newCourse = bst->Search(laterInput, bst->root);
            bst->Insert(newCourse);
            break;
        }
    }
    cout << "Fine. Leave Then. See if I Care." << endl;
    return 0;
}