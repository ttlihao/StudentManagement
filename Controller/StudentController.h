#ifndef STUDENT_VIEW_CONTROLLER_H
#define STUDENT_VIEW_CONTROLLER_H

#include "../Model/Student.h"
#include "../View/StudentView.h"

#define MAX_VIEWS 100
void filterAndSortStudentViews(
    const char* idKeyword,
    const char* nameKeyword,
    const char* majorCode,
    float minGPA,
    float maxGPA,
    const char* sortByField
);
extern struct Student students[MAX_VIEWS];
extern struct StudentView views[MAX_VIEWS];
extern int viewCount;

void addStudentView();
void saveStudentViewsToFile(const char* filename);
void loadStudentViewsFromFile(const char* filename);
void displayAllStudentViews();
void searchStudentViewByID(const char* id);
void sortStudentViewsByGPA();

#endif