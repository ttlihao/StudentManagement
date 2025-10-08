#ifndef STUDENT_VIEW_H
#define STUDENT_VIEW_H

#include "../Model/Student.h"  // Adjust path based on your folder structure

// Struct to encapsulate view logic for a student
struct StudentView {
    struct Student* student;
};

// Function to display only essential student information
void displayStudentView(struct StudentView view);

#endif