#ifndef STUDENT_MODEL_H
#define STUDENT_MODEL_H

#include "Student.h"
#define MAX_STUDENTS 200

void loadStudents();


void saveStudents();


int addStudent(struct Student student);


struct Student* findStudentById(const char* id);


int updateStudent(const char* id, struct Student updatedStudent);


int deleteStudent(const char* id);


struct Student* getAllStudents();


int getStudentCount();
void calculateStudentGPA(struct Student* student);
int getNextStudentID();
void cleanupModel();

#endif 