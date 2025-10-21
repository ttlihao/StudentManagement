#ifndef SUBJECT_MODEL_H
#define SUBJECT_MODEL_H

#include "Subject.h"
void loadSubjects();
void saveSubjects();
struct Subject* getAllSubjects();
int getSubjectCount();
struct Subject* findSubjectByCode(const char* code);
#endif