#ifndef SUBJECT_MODEL_H
#define SUBJECT_MODEL_H

#include "Subject.h"
void loadSubjects();
void saveSubjects();
struct Subject* getAllSubjects();
int getSubjectCount();
int addSubject(struct Subject newSubject);
int deleteSubject(const char* code);
int updateSubject(const char* code, struct Subject updatedInfo);
struct Subject* findSubjectByCode(const char* code);
#endif