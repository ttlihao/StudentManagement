#ifndef STUDENT_CONTROLLER_H
#define STUDENT_CONTROLLER_H

#include "../Model/Student.h"
#include "../View/StudentView.h"

#define MAX_VIEWS 100
#define PAGE_SIZE 5
extern struct Student students[MAX_VIEWS];
extern struct StudentView views[MAX_VIEWS];
extern int viewCount;

void addStudentView();
void saveStudentViewsToFile(const char* filename);
void loadStudentViewsFromFile(const char* filename);
void displayAllStudentViews();
void deleteStudentView();
void filterAndSortStudentViews(
    const char* idKeyword,
    const char* nameKeyword,
    const char* majorCode,
    float minGPA,
    float maxGPA,
    const char* sortByField,
    int sortOrder
);
void deleteStudent(const char* idKeyword);
void editStudentInfo();

#endif