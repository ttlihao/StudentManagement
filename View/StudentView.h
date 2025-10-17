#ifndef STUDENT_VIEW_H
#define STUDENT_VIEW_H

#include "../Model/Student.h" 

struct StudentView {
    struct Student* student;
};

void displayStudentView(struct StudentView view);

#endif