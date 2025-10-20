#ifndef SUBJECT_CONTROLLER_H
#define SUBJECT_CONTROLLER_H

#include "../Model/Subject.h"

#define MAX_SUBJECTS 50

extern struct Subject subjects[MAX_SUBJECTS];
extern int subjectCount;

void addSubject();
void displayAllSubject();
void updateSubJect();
void Delete();
void loadSubjectsFromFile(const char* filename);
int findSubjectIndex(const char* code);

#endif