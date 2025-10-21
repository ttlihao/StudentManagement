#ifndef STUDENT_CONTROLLER_H
#define STUDENT_CONTROLLER_H

#include "../Model/Student.h"

// Logic to create a new student
int handleAddStudent(struct Student newStudent);

// Logic to find a student
struct Student* handleFindStudent(const char* id);

// Logic to delete a student
int handleDelStudent(const char* id);

// Logic to filter and sort students, returns a dynamically allocated array of pointers
struct Student** handleFilterAndSort(
    const char* idKeyword, const char* nameKeyword, const char* majorCode,
    float minGPA, float maxGPA,
    const char* sortByField, int sortOrder,
    int* resultCount // Output parameter for the number of results
);
void handleEditStudentSession(const char* studentId);
// Helper for case-insensitive string search
const char* stristr(const char* haystack, const char* needle);

#endif