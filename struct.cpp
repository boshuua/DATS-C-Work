#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Define a struct to hold student information
struct Student {
    string name;
    float grade;
};

int main() {
    vector<Student> students; // Vector to store students
    const int numberOfStudents = 3; // Fixed number of students

    // Input multiple students
    for (int i = 0; i < numberOfStudents; i++) {
        Student s1;

        cout << "Enter Student Name: ";
        cin.ignore(); // Clear the newline character from the input buffer
        getline(cin, s1.name); // Use getline to allow spaces in names

        cout << "Enter Student Grade (0-100): ";
        cin >> s1.grade;

        // Add student to the vector
        students.push_back(s1);
    }

    // Display all student info
    cout << "\nStudent Information:\n";
    for (const auto& student : students) {
        cout << "Name: " << student.name << ", Grade : " << student.grade << "%" << endl;
    }

    // Calculate average grade and highest grade
    float totalGrade = 0.0;
    float highestGrade = 0.0; // Initialize to 0

    for (const auto& student : students) {
        totalGrade += student.grade;
        if (student.grade > highestGrade) {
            highestGrade = student.grade; // Update highest grade
        }
    }

    float averageGrade = totalGrade / numberOfStudents;
    cout << "\nAverage Grade: " << averageGrade << endl;
    cout << "Highest Grade: " << highestGrade << endl;

    return 0;
}
